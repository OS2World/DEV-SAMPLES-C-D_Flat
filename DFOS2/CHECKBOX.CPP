// ------------- checkbox.cpp

#include "checkbox.h"
#include "desktop.h"

CheckBox::CheckBox(const char *lbl, int lf, int tp, DFWindow *par) :
                    Button(lbl, lf, tp, par)
{
    setchar = 'X';
    (*text)[0] = '[';
    (*text)[2] = ']';
    windowtype = CheckBoxWindow;
}








