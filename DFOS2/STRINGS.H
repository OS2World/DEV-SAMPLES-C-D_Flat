// -------- strings.h

#ifndef STRINGS_H
#define STRINGS_H

#include <iostream.h>
#include <string.h>
#include "dflatdef.h"

// ============================
// BASIC-like String Class
// ============================
class String    {
	char *sptr;
	int length;
	void putstr(const char *s);
public:
	// -------- construct a null string
	String() { sptr = 0; length = 0; }
	// --- construct with char * initializer
	String(const char *s);
	// ------- copy constructor
	String(String& s);
	// -------- construct with a size and fill character
	String(int len, char fill = 0);
	// ------- destructor
	~String() { delete sptr; }
	// ------ return the length of a string
	int Strlen() { return strlen(sptr); }
	int StrBufLen() { return length; }
	// ------ change the buffer length of a string
	void ChangeLength(unsigned int newlen);
	// ---- substring: right len chars
	String right(int len);
	// ---- substring: left len chars
	String left(int len);
	// ---- substring: middle len chars starting from where
	String mid(int len, int where);
	int FindChar(unsigned char ch);
	// ---------- assignment
	String& operator=(String& s);
	// ---------- conversion to char *
	operator const char *() const { return sptr; }
	// --- concatenation operator (str1 + str2;)
	String operator+(String& s);
	// --- concatenation operator (str1 += str2;)
	void operator+=(String& s);
    // --- concatenation operator (str1 + "str")
    String operator+(const char *s);
	// --- concatenation operator (str1 += "str")
	void operator+=(const char *s);

	// ------- relational operators
	Bool operator==(String& s)
		{ return (Bool) (strcmp(sptr,s.sptr) == 0); }
	Bool operator!=(String& s)
		{ return (Bool) (strcmp(sptr,s.sptr) != 0); }
	Bool operator>(String& s)
		{ return (Bool) (strcmp(sptr,s.sptr) > 0); }
	Bool operator<(String& s)
		{ return (Bool) (strcmp(sptr,s.sptr) < 0); }
	Bool operator<=(String& s)
		{ return (Bool) (!(*this > s)); }
	Bool operator>=(String& s)
		{ return (Bool) (!(*this < s)); }
	// ------- subscript
	char& operator[](int n) { return sptr[n]; }
	// ------- stream I/O
	friend ostream& operator<< (ostream& os, String& str);
	friend istream& operator>> (istream& is, String& str);
};

#endif

