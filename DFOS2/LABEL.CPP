// ------------- label.cpp

#include <ctype.h>
#include "label.h"

void Label::InitLabel(const char *txt)
{
	SetText(txt);
	const char *ac = strchr(txt, SHORTCUTCHAR);
	if (ac != 0)
		Control::LatestShortcut = tolower(*(ac+1));
	Disable();
}

Label::Label(const char *txt, int lf, int tp, int wd, DFWindow *par)
					: TextBox(lf, tp, 1, wd ? wd : strlen(txt), par)
{
	InitLabel(txt);
}

Label::Label(const char *txt, int lf, int tp, DFWindow *par)
					: TextBox(lf, tp, 1, strlen(txt), par)
{
	InitLabel(txt);
}

// -------- paint the window 
void Label::Paint()
{
	if (Parent() != 0)
		SetColor(Parent()->Colors());
    shortcutfg = RED;
    if (visible)
        WriteShortcutLine(0, colors.fg, colors.bg);
}




