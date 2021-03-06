// ------------- cursor.h
//
// modified for OS/2 operation - jw21sep93

#ifndef CURSOR_H
#define CURSOR_H


#define INCL_BASE
#define INCL_NOPMAPI
#include <os2.h>

const int MAXSAVES = 50;  // depth of cursor save/restore stack

class Cursor 
    {
    VIOCURSORINFO ci;

    // --- cursor save/restore stack
    int  cursorpos[MAXSAVES];
    int  cursorshape[MAXSAVES];
    int  cs;                     // count of cursor saves in effect
    void Cursor::GetCursor();

public:
    Cursor();
    ~Cursor();
    void SetPosition(int x, int y);
    void GetPosition(int &x, int &y);
    void SetType(unsigned t);
    void NormalCursor() { SetType(0x0607); }
    void BoxCursor()    { SetType(0x0107); }
    void Hide();
    void Show();
    void Save();
    void Restore();
    void SwapStack();
    };

inline void swap(int& a, int& b)
    {
    int x = a;
    a = b;
    b = x;
    }

#endif
