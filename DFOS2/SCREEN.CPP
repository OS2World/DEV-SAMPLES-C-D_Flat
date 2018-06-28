// ----------- screen.cpp
//
// modified for OS/2 operation                   - jw21sep93
// direct video buffer manipulation enhancements - jr28nov93

#include <string.h>
#include "desktop.h"


Screen::Screen()
    {
    // Get current display configuration
    USHORT blen = 0;
    ULONG  badr = 0;

    mi.cb = sizeof(mi);
    VioGetMode(&mi, 0);
    VioGetBuf((PULONG16)&badr, (PUSHORT16)&blen, 0 );
    mi.buf_addr=(ULONG)(char *)(((badr >> 3) & 0xffff0000L)|(badr & 0xffffL));
    mi.buf_length = blen;
    width  = mi.col;
    height = mi.row;
    }


// --------- Update whole of Physical Display Buffer 
//           (performance enhancement by Jim Riblett)

void Screen::Refresh( void )
    {
    VioShowBuf( (USHORT)0, (USHORT)mi.buf_length, 0 );
    }

// --------- Update whole of Physical Display Buffer 
//           (performance enhancement by Jim Riblett)

void Screen::Refresh( int scol, int srow, int len )
    {
    VioShowBuf( (USHORT)(getVptr( scol, srow ) - mi.buf_addr),
                (USHORT)(len<<1), 
                (USHORT)0 );
    }

// --------- scroll the screen d: 1 = up, 0 = dn

void Screen::Scroll(Rect &rc, int d, int fg, int bg)
    {
    char    cell[2];

    cell[0] = SPACECHAR;
    cell[1] = clr(fg,bg);

    desktop.mouse().Hide();

    if (d == 1 )
        {
        VioScrollUp((USHORT)rc.Top(), 
                    (USHORT)rc.Left(), 
                    (USHORT)rc.Bottom(), 
                    (USHORT)rc.Right(), 
                    1, 
                    (PBYTE16)&cell, 
                    0);
        }
    else
        {
        VioScrollDn((USHORT)rc.Top(), 
                    (USHORT)rc.Left(), 
                    (USHORT)rc.Bottom(), 
                    (USHORT)rc.Right(), 
                    1, 
                    (PBYTE16)&cell, 
                    0);
        }
    desktop.mouse().Show();
    }


// -------- read a character of video memory

unsigned int Screen::GetVideoChar(int x, int y)
    {
    USHORT c;
    USHORT cb = 2;

    desktop.mouse().Hide();
    VioReadCellStr((char *) &c, &cb, (USHORT)y, (USHORT)x, 0);
    desktop.mouse().Show();
    return (unsigned int) c;
    }


// -------- write a character of video memory
//           (performance enhanced version by Jim Riblett)

void Screen::PutVideoChar(int x, int y, unsigned int c)
    {
    if( x >= 0 && x < width && y >= 0 && y < height )
        {
        char *p = getVptr( x, y );
        char *t = (char *)&c;
        p[0] = t[0];
        p[1] = t[1];

        desktop.mouse().Hide();
        VioShowBuf( (USHORT)(p - mi.buf_addr), 2, 0 );
        desktop.mouse().Show();
        }
    }


// -------- write a character of video memory without refreshing
//           (performance enhancement by Jim Riblett)

void Screen::PutVideoCharNR( int x, int y, unsigned int c )
    {
    if( x >= 0 && x < width && y >= 0 && y < height )
        {
        char *p = getVptr( x, y );
        char *t = (char *)&c;
        p[0] = t[0];
        p[1] = t[1];
        }
    }


// --------- Write a string to video memory
//           (performance enhanced version by Jim Riblett)

void Screen::WriteVideoString(const char *s, int x, int y, int fg, int bg)
    {
    if( x >= 0 && x < width && y >= 0 && y < height )
        {
        int   len  = strlen( s );
        char  *ln  = getVptr( x, y );
        char  *cp1 = ln;
        char  *eor = (getVptr( 0, (y+1) )) - 1;
        int   diff = eor - cp1;

        if( len > x - width )
            {
            len = width - x;
            }

        unsigned char col = clr( fg,bg );

        while( *s && diff )
            {
            *cp1++ = *s;
            *cp1++ = col;
            diff -= 2;
            if( diff <= 0 )
                {
                break;
                }
            s++;
            }

        desktop.mouse().Hide();
        VioShowBuf( (USHORT)(ln - mi.buf_addr), (USHORT)(len<<1), (USHORT)0 );
        desktop.mouse().Show();
        }
    }


// -- read a rectangle of video memory into a save buffer
//           (performance enhanced version by Jim Riblett)

void Screen::GetBuffer(Rect &rc, char *bf)
    {
    if ((rc.Left() < width) && (rc.Top() < height))
        {
        int ht = rc.Bottom()-rc.Top()+1;
        int row = rc.Top();
        int cb =  (rc.Right()-rc.Left()+1) * 2;
    
        desktop.mouse().Hide();
        while (ht--)    
            {
            memcpy( bf, getVptr( rc.Left(), row ), cb );
            bf = (char *)bf + cb;
            row++;
            }
        desktop.mouse().Show();
        }
    }

// -- write a rectangle of video memory from a save buffer
//           (performance enhanced version by Jim Riblett)

void Screen::PutBuffer(Rect &rc, char *bf)
    {
    if ((rc.Left() < width) && (rc.Top() < height))
        {
    	int ht  = rc.Bottom()-rc.Top()+1;
    	int cb  = (rc.Right()-rc.Left()+1) * 2;
    	int row = rc.Top();
    	
    	desktop.mouse().Hide();
    	while (ht--)    
    	    {
            memcpy( getVptr( rc.Left(), row ), bf, cb );
    	    bf += cb;
    	    row++;
    	    }
        Refresh();
    	desktop.mouse().Show();
        }
    }

