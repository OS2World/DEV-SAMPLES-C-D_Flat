// ------------ cursor.cpp
//
// modified for OS/2 operation - jw13nov93

#include <dos.h>
#include "cursor.h"
#include "desktop.h"

Cursor::Cursor()
    {
    VioGetCurType(&ci, 0);
    cs = 0;
    Save();
    }

Cursor::~Cursor()
    {
    Restore();
    }


// ------ get cursor shape and position
void Cursor::GetCursor(){};


// -------- get the current cursor position
void Cursor::GetPosition(int &x, int &y)
    {
    USHORT	sx, sy;

    VioGetCurPos(&sy, &sx, 0);
    x = sx;
    y = sy;
    }

// ------ position the cursor

void Cursor::SetPosition(int x, int y)
    {
    VioSetCurPos((USHORT)y, (USHORT)x, 0);
    }


// ------ save the current cursor configuration
void Cursor::Save()
    {
    USHORT x, y;

    if (cs < MAXSAVES)    
        {
	    VioGetCurPos(&y, &x, 0);
	    cursorpos[cs]	= (x<<8) | y;

	    VioGetCurType(&ci, 0);
	    cursorshape[cs] = (ci.yStart << 8) | ci.cEnd;
	    cs++;
        }
    }

// ---- restore the saved cursor configuration
void Cursor::Restore()
    {
    USHORT row = (USHORT)(cursorpos[cs] & 0xff);
    USHORT col = (USHORT)((cursorpos[cs] >> 8) & 0xff);

    if (cs)    
        {
        --cs;
	    VioSetCurPos(row, col, 0);
	    SetType(cursorshape[cs]);
        }
    }

/* ---- set the cursor type ---- */
void Cursor::SetType(unsigned t)
    {
    ci.yStart = (USHORT)((t >> 8) & 0xff);
    ci.cEnd   = (USHORT)(t & 0xff);
    VioSetCurType(&ci, 0);
    }


/* ----- swap the cursor stack ------- */
void Cursor::SwapStack()
    {
    if (cs > 1)    
        {
        swap(cursorpos[cs-2], cursorpos[cs-1]);
        swap(cursorshape[cs-2], cursorshape[cs-1]);
        }
    }


/* ------ hide the cursor ------ */
void Cursor::Hide()
    {
    USHORT t;

    t = ci.attr;
    ci.attr = 0xffff;
    VioSetCurType(&ci, 0);
    ci.attr = t;
    }


/* ------ show the cursor ------ */
void Cursor::Show()
    {
    VioSetCurType(&ci, 0);
    }



