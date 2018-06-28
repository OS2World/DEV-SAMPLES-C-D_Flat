// ------------- notice.cpp

#include <ctype.h>
#include "notice.h"

static Color col = {
	BLACK,			// fg
	GREEN,			// bg
	BLACK,			// selected fg
	GREEN,			// selected bg
	BLACK,			// frame fg
	GREEN,			// frame bg
	BLACK,			// highlighted fg
	GREEN			// highlighted bg
};

NoticeBox::NoticeBox(const char *text, const char *ttl) :
		Dialog(ttl, 7, 20),
		notice(text, 1, 2, (DFWindow *)this),
		ok(0, 4, (DFWindow *)this)
{
	int wd = max(10,max(strlen(text)+2,strlen(ttl)+2));
	Size(Left()+wd-1, Bottom());
	ok.Move(Left()+((wd-8)/2), ok.Top());
	colors = col;
}

static Color ecol = {
	YELLOW, RED,
	YELLOW, RED,
	YELLOW, RED,
	YELLOW, RED
};

ErrorMessageBox::ErrorMessageBox(const char *text, const char *ttl) :
		NoticeBox(text, ttl)
{
	desktop.speaker().Beep();
	colors = ecol;
}

static Color yncol = {
	BLACK,GREEN,
	BLACK,GREEN,
	BLACK,GREEN,
	BLACK,GREEN
};

YesNoBox::YesNoBox(const char *text, const char *ttl) :
		Dialog(ttl, 7, 20),
		notice(text, 1, 2, (DFWindow *)this),
		yes(0, 4, (DFWindow *)this),
		no(0, 4, (DFWindow *)this)
{
	int wd = max(21,max(strlen(text)+2,strlen(ttl)+2));
	Size(Left()+wd-1, Bottom());
	yes.Move(Left()+((wd-17)/2), yes.Top());
	no.Move(Left()+((wd-17)/2)+9, no.Top());
	colors = yncol;
}

void YesNoBox::Keyboard(int key)
{
	switch (tolower(key))	{
		case 'n':
			CancelFunction();
			break;
		case 'y':
			OKFunction();
			break;
		default:
			Dialog::Keyboard(key);
			break;
	}
}

