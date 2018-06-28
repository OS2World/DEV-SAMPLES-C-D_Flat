// --------- notice.h

#ifndef NOTICE_H
#define NOTICE_H

#include "dflatpp.h"

// ----------- Notice message window
class NoticeBox : public Dialog	{
	Label notice;
	// ----- command button
	OKButton ok;
public:
	NoticeBox(const char *text, const char *ttl);
};

inline void Notice(const char *txt, const char *ttl = "Notice")
{
	NoticeBox(txt, ttl).Execute();
}

// --------- Error message window
class ErrorMessageBox : public NoticeBox	{
public:
	ErrorMessageBox(const char *text, const char *ttl);
};

inline void ErrorMessage(const char *txt, const char *ttl = "Error")
{
	ErrorMessageBox(txt, ttl).Execute();
}

// ------- Yes/No query window
class YesNoBox : public Dialog	{
	Label notice;
	// ----- command button
	YesButton yes;
	NoButton  no;
public:
	YesNoBox(const char *txt, const char *ttl);
    virtual void Keyboard(int key);
};

inline Bool YesNo(const char *txt, const char *ttl = "")
{
	YesNoBox yn(txt, ttl);
	yn.Execute();
	return yn.OKExit();
}

#endif
