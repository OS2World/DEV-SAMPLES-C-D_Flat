// -------- listbox.h

#ifndef LISTBOX_H
#define LISTBOX_H

#include "textbox.h"

const int LISTSELECTOR = 4; // selected list box entry

class ListBox : public TextBox    {
    Bool addmode;       // adding extended selections mode
    int anchorpoint;    // anchor point for extended selections
    int selectcount;    // count of selected items

    void OpenWindow();
protected:
    int selection;        // current selection
    virtual void ClearSelection();
public:
    ListBox(const char *ttl, int lf, int tp, int ht, int wd,
        DFWindow *par=0) : TextBox(ttl, lf, tp, ht, wd, par)
            { OpenWindow(); }
    ListBox(const char *ttl, int ht, int wd, DFWindow *par=0)
                        : TextBox(ttl, ht, wd, par)
            { OpenWindow(); }
    ListBox(int lf, int tp, int ht, int wd, DFWindow *par=0)
                        : TextBox(lf, tp, ht, wd, par)
            { OpenWindow(); }
    ListBox(int ht, int wd, DFWindow *par=0) : TextBox(ht,wd,par)
            { OpenWindow(); }
    ListBox(const char *ttl)    : TextBox(ttl)
            { OpenWindow(); }
    // -------- listbox API messages
    virtual void ClearText();
    virtual void Paint();
    virtual void Keyboard(int key);
    virtual void SetSelection(int sel);
    virtual void ButtonReleased(int mx, int my);
    virtual void LeftButton(int mx, int my);
    virtual void DoubleClick(int mx, int my);
	virtual int Selection() { return selection; }
    virtual void ScrollUp();
    virtual void ScrollDown();
    virtual void ScrollRight();
    virtual void ScrollLeft();
    virtual void PageUp();
    virtual void PageDown();
    virtual void PageRight();
    virtual void PageLeft();
};

#endif


