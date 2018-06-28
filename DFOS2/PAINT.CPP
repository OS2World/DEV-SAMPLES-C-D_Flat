// ---------- paint.cpp
// Calls Screen.Refresh() for performance enhancement  - jr28nov93

#include "desktop.h"
#include "dfwindow.h"

void DFWindow::ClearWindow()
{
    String ln(ClientWidth(), clearch);

    int h = ClientHeight();
    for (int y = 0; y < h; y++)
        WriteClientString(ln, 0, y, colors.fg, colors.bg);
}

void DFWindow::Paint(Rect rc)
{
    if (visible)    {
        String ln(rc.Width(), clearch);
        int x = rc.Left()-ClientLeft();
        int y = rc.Top()-ClientTop();
        for (int i = 0; i < rc.Height(); i++)
            WriteClientString(ln, x, y+i, colors.fg, colors.bg);
    }
}

void DFWindow::Paint()
{
    if (visible)
        ClearWindow();
}

void DFWindow::WriteString(String &ln, int x, int y, int fg, int bg)
{
    int xx = x - Left();
    int scol = x;
    int srow = y;
    if (ln.Strlen()+xx > Width())
        ln.ChangeLength(Width()-xx);
    if (this->isDescendedFrom(desktop.InFocus()))
        desktop.screen().WriteVideoString(ln, x, y, fg, bg);
    else
    {
        const char *cp = ln;
        desktop.mouse().Hide();
        while( *cp )
        {
            WriteCharNR(*cp++, x++, y, fg, bg);
        }
        desktop.screen().Refresh( scol, srow, strlen( ln ) );
        desktop.mouse().Show();
    }
}


void DFWindow::WriteCharNR( int ch, int x, int y, int fg, int bg )
{
    if( ShadowedRect().Inside( x, y ) )
    {
        // --- coordinates are inside target window
        // --- see if overlapped by sibling
        DFWindow *sibl = Next();
        while (sibl != 0)
        {
            if( sibl->isVisible() && sibl->ShadowedRect().Inside(x, y) )
            {
                return;
            }
            sibl = sibl->Next();
        }
        desktop.screen().PutVideoCharNR( x, y, (ch & 255) | (clr(fg,bg) << 8) );
    }
}

void DFWindow::WriteChar(int ch, int x, int y, int fg, int bg)
{
    if (ShadowedRect().Inside(x, y))    {
        // --- coordinates are inside target window
        // --- see if overlapped by sibling
        DFWindow *sibl = Next();
        while (sibl != 0)   {
            if (sibl->isVisible() && sibl->ShadowedRect().Inside(x, y))
                return;
            sibl = sibl->Next();
        }
        desktop.screen().PutVideoChar(x, y, (ch & 255) | (clr(fg,bg) << 8));
    }
}



