// ----------- screen.h
//
// modified for OS/2 operation      - jw21sep93
// added fn members for performance - jr28nov93

#ifndef SCREEN_H
#define SCREEN_H

#define INCL_BASE
#define INCL_NOPMAPI
#include <os2.h>

#include "dflatdef.h"
#include "rectangl.h"

const char SPACECHAR = 0x20;

class Screen
    {
    VIOMODEINFO mi;
    unsigned    height;
    unsigned    width;

    // ---- compute video offset address
    unsigned vad(int x, int y) { return y * (width*2) + x*2; }
public:
    Screen();
    unsigned Height() { return height; }
    unsigned Width()  { return width; }

    void         Scroll(Rect &rc, int d, int fg, int bg);
    unsigned int GetVideoChar(int x, int y);
    void         PutVideoChar(int x, int y, unsigned int c);
    void         PutVideoCharNR( int x, int y, unsigned int c );
    void         WriteVideoString(const char *s,int x,int y,int fg,int bg);

    void         GetBuffer(Rect &rc, char *bf);
    void         PutBuffer(Rect &rc, char *bf);
    char        *getVptr(int x, int y);

    void Refresh( void );
    void Refresh( int scol, int srow, int len );
    };


inline char *Screen::getVptr(int x, int y)
    {
    return( (char *)mi.buf_addr + ((y*mi.col + x)<<1) );
    }


inline int clr(int fg, int bg)
    {
    return fg | (bg << 4);
    }

#endif



