// -------- toolbar.h

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "pbutton.h"

const COLORS ToolBarBG = BLUE;

// ------ Toolbar class
class ToolBar : public DFWindow {
    void ParentSized(int xdif, int ydif);
public:
    ToolBar(DFWindow *par);
};

// ------- Toolbar button class
class ToolButton : public PushButton    {
    void SetColors();
    void InitWindow(const char *lbl);
    DFWindow *oldFocus;
    virtual void ButtonCommand();
    virtual Bool SetFocus();
public:
    ToolButton(const char *lbl, int lf, int tp, DFWindow *par=0);
    ToolButton(const char *lbl, DFWindow *par=0);
    virtual void Paint();
    virtual void Border();
};

#endif
