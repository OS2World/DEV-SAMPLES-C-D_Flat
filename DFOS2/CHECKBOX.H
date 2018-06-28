// ----------- checkbox.h

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "button.h"

class CheckBox : public Button    {
public:
    CheckBox(const char *lbl, int lf, int tp, DFWindow *par);
};

#endif

