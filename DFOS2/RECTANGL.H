// -------- rectangl.h

#ifndef RECTANGL_H
#define RECTANGL_H

#include "dflatdef.h"

class Rect	{
    int left,top,right,bottom;
public:
	Rect() {};
	Rect(int l, int t, int r, int b)
		{ left = l; top = t; right = r; bottom = b; }
	int &Top()		{ return top; }
	int &Bottom()	{ return bottom; }
	int &Left()		{ return left; }
	int &Right()	{ return right; }
	int Height()	{ return bottom-top+1; }
	int Width()		{ return right-left+1; }
	Bool Inside(int x, int y)
		{ return (Bool)(x >= left && x <= right && y >= top && y <= bottom); }
	Rect subRectangle(Rect &rc);
	Bool ValidRectangle()
		{ return (Bool) (right != -1 && top != -1); }
	Bool operator==(Rect rc)
		{ return (Bool) (right == rc.right && top == rc.top &&
						 left == rc.left && bottom == rc.bottom); }
};

#endif



