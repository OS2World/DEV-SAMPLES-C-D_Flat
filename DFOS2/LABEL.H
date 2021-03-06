// ------------ label.h

#ifndef LABEL_H
#define LABEL_H

#include "textbox.h"

class Label : public TextBox	{
	void InitLabel(const char *txt);
public:			 
	Label(const char *txt, int lf, int tp, int wd=0,
							DFWindow *par = 0);
	Label(const char *txt, int lf, int tp, DFWindow *par = 0);
    virtual void Paint();
};

#endif
