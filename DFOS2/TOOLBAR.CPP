// ------------ toolbar.cpp

#include "toolbar.h"

// ----- construct a Toolbar
ToolBar::ToolBar(DFWindow *par) : DFWindow(par)
{
    windowtype = ToolbarWindow;
    if (par != 0)    {
        // --- put it into the application window
        Move(par->ClientLeft(), par->ClientTop());
        Size(par->ClientRight(), par->ClientTop()+2);
        par->SetAttribute(TOOLBAR);
    }
    colors.fg = colors.bg = ToolBarBG;
}

// ---- resize the menubar when the application window resizes
void ToolBar::ParentSized(int xdif, int)
{
    Size(Right()+xdif, Bottom());
}
