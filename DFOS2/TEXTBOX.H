// ------------- textbox.h
// -- Fixed a bug with TextLine() which caused an Access violation - jr27nov93

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "control.h"

const int SHORTCUTCHAR = '~';

class ScrollBar;

class TextBox : public Control    
    {
    ScrollBar *hscrollbar;      // horizontal scroll bar
    ScrollBar *vscrollbar;      // vertical scroll bar
    unsigned  *TextPointers;    // -> list of line offsets
    Bool       resetscrollbox;

    void OpenWindow();
protected:
    // ---- text buffer
    String *text;               // window text
    int     wlines;             // number of lines of text
    int     textwidth;          // width of longest line in textbox

    // ---- text display
    int wtop;               	// text line on top of display
    int wleft;              	// left position in window viewport
    int BlkBegLine;         	// beginning line of marked block
    int BlkBegCol;          	// beginning column of marked block
    int BlkEndLine;         	// ending line of marked block
    int BlkEndCol;          	// ending column of marked block
    int shortcutfg;         	// shortcut key color

    const char *TextLine(int line);
    int  DisplayShortcutField(String sc, int x, int y, int fg, int bg);
    void WriteShortcutLine(int lno, int fg, int bg);
    void WriteTextLine(int lno, int fg, int bg);
    void BuildTextPointers();
    void SetScrollBoxes();
public:
    TextBox(const char *ttl,int lf,int tp,int ht,int wd,DFWindow *par=0)
                        : Control(ttl, lf, tp, ht, wd, par)
            { OpenWindow(); }
    TextBox(const char *ttl, int ht, int wd, DFWindow *par=0)
                        : Control(ttl, ht, wd, par)
            { OpenWindow(); }
    TextBox(int lf, int tp, int ht, int wd, DFWindow *par=0)
                        : Control(lf, tp, ht, wd, par)
            { OpenWindow(); }
    TextBox(int ht, int wd, DFWindow *par=0) : Control(ht,wd,par)
            { OpenWindow(); }
    TextBox(const char *ttl)    : Control(ttl)
            { OpenWindow(); }

    // -------- textbox API messages
    virtual void ScrollUp();
    virtual void ScrollDown();
    virtual void ScrollRight();
    virtual void ScrollLeft();
    virtual void PageUp();
    virtual void PageDown();
    virtual void PageRight();
    virtual void PageLeft();
    virtual void Home();
    virtual void End();
    virtual void CloseWindow();
    virtual void AddText(const char *txt);
    virtual void SetText(const char *txt);
    virtual void SetTextLength(unsigned int len);
    virtual void ClearText();
    virtual void Show();
    virtual void Paint();
    virtual void Keyboard(int key);
    String ExtractTextLine(int lno);
	virtual const String GetText() { return *text; }
	unsigned int TextLength(){ return text ? text->Strlen() : 0; }
    void ClearTextBlock() { BlkBegLine=BlkEndLine=BlkBegCol=BlkEndCol=0; }
    void HorizontalPagePosition(int pct);
    void VerticalPagePosition(int pct);
	int LineCount() { return wlines; }
	int TextWidth() { return textwidth; }
	virtual void SetScrollBars();
	int LineLength(int lno);
    };

#endif

