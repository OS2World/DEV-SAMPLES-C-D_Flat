// ------------ dfwindow.h
// -- mods for performance - WriteCharNR member added - jr27nov93

#ifndef DFWINDOW_H
#define DFWINDOW_H

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <stdlib.h>
#include <conio.h>

#include "dflatdef.h"
#include "rectangl.h"
#include "strings.h"
#include "family.h"

// -------- window attribute flags
const int MOVEABLE   = 0x0001;
const int SIZEABLE   = 0x0002;
const int BORDER     = 0x0004;
const int TITLEBAR   = 0x0008;
const int CONTROLBOX = 0x0010;
const int MINBOX     = 0x0020;
const int MAXBOX     = 0x0040;
const int SHADOW     = 0x0080;
const int SAVESELF   = 0x0100;
const int FRAMEWND   = 0x0200;
const int MENUBAR    = 0x0400;
const int STATUSBAR  = 0x0800;
const int VSCROLLBAR = 0x1000;
const int HSCROLLBAR = 0x2000;
const int TOOLBAR    = 0x4000;

#if !defined(__COLORS)
#define __COLORS

// --------------- Color Macros
enum COLORS {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};

#endif

// ------ window shadow attributes
const unsigned char ShadowFG = DARKGRAY;
const unsigned char ShadowBG = BLACK;

// --------------- border characters
const unsigned char FOCUS_NW   = '\xc9';
const unsigned char FOCUS_NE   = '\xbb';
const unsigned char FOCUS_SE   = '\xbc';
const unsigned char FOCUS_SW   = '\xc8';
const unsigned char FOCUS_SIDE = '\xba';
const unsigned char FOCUS_LINE = '\xcd';
const unsigned char NW         = '\xda';
const unsigned char NE         = '\xbf';
const unsigned char SE         = '\xd9';
const unsigned char SW         = '\xc0';
const unsigned char SIDE       = '\xb3';
const unsigned char LINE       = '\xc4';

// ----------------- title bar characters
const unsigned char CONTROLBOXCHAR = '\xf0';
const unsigned char MAXPOINTER     = 24;
const unsigned char MINPOINTER     = 25;
const unsigned char RESTOREPOINTER = 18;

// ----------- window states
enum WndState     {
	OPEN,
    CLOSING,
    CLOSED
};

// ---------- window colors
struct Color {
    COLORS fg, bg;      // standard colors
    COLORS sfg, sbg;    // selected text colors
    COLORS ffg, fbg;    // window frame colors
    COLORS hfg, hbg;    // highlighted text colors
};

class Application;
class StatusBar;
class PopDown;

struct BoxLines	{
	int nw, n, ne, e, se, s, sw, w;
};

class DFWindow    {
protected:
    WndType windowtype;   // window type
    int prevmouseline;    // holders for
    int prevmousecol;     // mouse coordinates
private:
    String *title;              // window title
    // -------------- window attributes
    int restored_attrib;        // attributes when restored
    Rect restored_rc;           // restored state rect
    // ----- control menu
    PopDown *ctlmenu;
    void DeleteCtlMenu();
    // --------- common window contructor code
    void InitWindow(const char *ttl,
        int lf, int tp, int ht, int wd, DFWindow *par);
    void InitWindow(int lf, int tp, int ht, int wd,
        DFWindow *par);
protected:
	// -------- for clearing the window
    char clearch;
    // --------------- video memory save data
    char *videosave;      // video save buffer
    Bool visible;         // True = window has been shown
    int attrib;           // Window attribute flags
    Bool DblBorder;       // True = dbl border on focus
    Color colors;         // window colors
    WndState windowstate; // OPEN, CLOSING, CLOSED
    Rect rect;            // window coordinates
                          // (0/0 to 79/24)
    // ------ previous capture focus handles
    DFWindow *prevcapture;
	DFWindow *prevfocus;
    // -------- system-wide
    void Enqueue();
    void Dequeue();
    void NextSiblingFocus();
    void PrevSiblingFocus();
    void WriteClientString(String &ln,int x,int y,int fg,int bg);
    void WriteWindowString(String &ln,int x,int y,int fg,int bg);
    void WriteWindowChar(int ch,int x,int y,int fg,int bg);
    void WriteCharNR( int ch, int x, int y, int fg, int bg );
    void WriteClientChar(int ch,int x,int y,int fg,int bg);
    void WriteChar(int ch, int x, int y, int fg, int bg);
    virtual void WriteString(String &ln, int x, int y, int fg, int bg);
    void OpenCtlMenu();
    // ------------- client window coordinate adjustments
    virtual void AdjustBorders();
    int BorderAdj;              // adjust for border
    int TopBorderAdj;           // adjust for top border
    int BottomBorderAdj;        // adjust for bottom border
    // -----
    Bool HitControlBox(int x, int y)
        { return (Bool)(x-Left() == 2 && y-Top() == 0 &&
            (attrib & CONTROLBOX)); }
	void ChangePosition(int x, int y);
    virtual void ParentSized(int, int) {}
public:
	// ---- window parent/children structure
	Family<DFWindow> family;
	// ( above must be public so can access others members )
    // -------- constructors
    DFWindow(const char *ttl, int lf, int tp, int ht, int wd,
                DFWindow *par = 0)
        { InitWindow(ttl, lf, tp, ht, wd, par); }
    DFWindow(const char *ttl, int ht, int wd, DFWindow *par = 0)
        { InitWindow(ttl, -1, -1, ht, wd, par); }
    DFWindow(int lf, int tp, int ht, int wd, DFWindow *par = 0)
        { InitWindow(lf, tp, ht, wd, par); }
    DFWindow(int ht, int wd, DFWindow *par = 0)
        { InitWindow(-1, -1, ht, wd, par); }
    DFWindow(const char *ttl, DFWindow *par = 0)
        { InitWindow(ttl, 0, 0, -1, -1, par); }
    DFWindow(DFWindow *par = 0)
        { InitWindow(0, 0, -1, -1, par); }
    // -------- destructor
    virtual ~DFWindow()
        { if (windowstate != CLOSED) CloseWindow(); }
    // ------- window dimensions and position
    Rect WindowRect()    { return rect; }
    Rect ShadowedRect();
    int Right()          { return rect.Right(); }
    int Left()           { return rect.Left(); }
    int Top()            { return rect.Top(); }
    int Bottom()         { return rect.Bottom(); }
    int Height()         { return Bottom() - Top() + 1; }
    int Width()          { return Right() - Left() + 1; }
    // ------ client space dimensions and position
    Rect ClientRect();
    int ClientRight()    { return Right()-BorderAdj; }
    int ClientLeft()     { return Left()+BorderAdj; }
    int ClientTop()      { return Top()+TopBorderAdj; }
    int ClientBottom()   { return Bottom()-BottomBorderAdj; }
    int ClientHeight()   { return Height()-TopBorderAdj-
                              BottomBorderAdj; }
    int ClientWidth()    { return Width()-BorderAdj*2; }

	// ---- this window's parent
    DFWindow *Parent()   { return family.Parent(); }
	void SetParent(DFWindow *par)  { family.SetParent(par); };
	// ----- this window's children
    DFWindow *First()    { return family.FirstChild(); }
	DFWindow *Last()     { return family.LastChild(); }
	// ----- this window's siblings
	DFWindow *Next()	  { return family.NextSibling(); }
	DFWindow *Prev()	  { return family.PrevSibling(); }
	Bool isDescendedFrom(DFWindow *ancestor);

    Bool isVisible()     { return visible; }
    int Attribute()      { return attrib; }
    void SetAttribute(int atr)
                         { attrib |= atr; AdjustBorders(); }
    void ClearAttribute(int atr)
                         { attrib &= ~atr; AdjustBorders(); }
    WndType WindowType() { return windowtype; }
    // ----- Control Menu messages
    void CtlMenuMove();
    void CtlMenuSize();
    // -------- API messages
    virtual void CloseWindow();
    virtual void Show();
    virtual void Hide();
    virtual Bool SetFocus();
    virtual void ResetFocus();
    virtual void EnterFocus(DFWindow *child) {}
    virtual void LeaveFocus(DFWindow *child) {}
	virtual void OKFunction() {}
	virtual void CancelFunction() {}
	virtual void HelpFunction();
    void CaptureFocus();
    void ReleaseFocus();
    virtual void Paint();
    virtual void Paint(Rect rc);
    virtual void Border();
    virtual void DrawBorder(BoxLines bl);
    virtual void Shadow();
    virtual void Title();
	virtual void SetTitle(const char *ttl);
    virtual void ClearWindow();
    virtual void ShiftChanged(int sk);
    virtual void Keyboard(int key);
	virtual void KeyReleased() {}
    virtual void DoubleClick(int mx, int my);
    virtual void LeftButton(int mx, int my);
    virtual void ButtonReleased(int mx, int my);
    virtual void MouseMoved(int mx, int my) {}
    virtual void Move(int x, int y);
    virtual void Size(int x, int y);
    virtual void ClockTick() {}
    WndState State() { return windowstate; }
	void SetColor(Color clr) { colors = clr; }
	Color Colors()       { return colors; }
    COLORS ClientFG()    { return colors.fg; }
    COLORS ClientBG()    { return colors.bg; }
    COLORS SelectedFG()  { return colors.sfg; }
    COLORS SelectedBG()  { return colors.sbg; }
    COLORS FrameFG()     { return colors.ffg; }
    COLORS FrameBG()     { return colors.fbg; }
    COLORS HighlightFG() { return colors.ffg; }
    COLORS HighlightBG() { return colors.fbg; }
	void SetClearChar(char ch) { clearch = ch; }
	virtual void ControlSelected(DFWindow *Wnd) {}
	virtual void ControlChosen(DFWindow *Wnd) {}
};

inline void DFWindow::WriteClientString(String &ln,
                    int x,int y,int fg,int bg)
{
    WriteString(ln,x+ClientLeft(),y+ClientTop(),fg,bg);
}

inline void DFWindow::WriteWindowString(String &ln,
                    int x,int y,int fg,int bg)
{
    WriteString(ln,x+Left(),y+Top(),fg,bg);
}

inline void DFWindow::WriteWindowChar(int ch,int x,int y,int fg,int bg)
{
    WriteChar(ch, x+Left(), y+Top(), fg, bg);
}

inline void DFWindow::WriteClientChar(int ch,int x,int y,int fg,int bg)
{
    WriteChar(ch, x+ClientLeft(), y+ClientTop(), fg, bg);
}

#endif


