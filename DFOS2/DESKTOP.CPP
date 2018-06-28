// --------------- desktop.cpp

#include <new.h>
#include "desktop.h"

DeskTop desktop;

void NoMemory()
{
    exit(-1);
}

DeskTop::DeskTop()
{
    apwnd = 0;
	infocus = 0;
	focuscapture = 0;
    set_new_handler(NoMemory);
    syscursor.Hide();
    drive = getdisk();
	path = new char[129];
    getcwd(path, 129);
}

DeskTop::~DeskTop()
{
    syscursor.Show();
    setdisk(drive);
	chdir(path);
	delete [] path;
}

Bool DeskTop::DispatchEvents()
{
    syskeyboard.DispatchEvent();
    sysmouse.DispatchEvent();
    sysclock.DispatchEvent();
    return (Bool) (apwnd != 0);
}

inline Bool inShadow(Rect &rc, int x, int y)
{
	return (Bool)
	(((x == rc.Right()+1) && (y > rc.Top() && y < rc.Bottom()+2))
							||
	((y == rc.Bottom()+1) && (x > rc.Left() && x < rc.Right()+2)));
}

// ---- find the window that coordinates are in
DFWindow *DeskTop::inWindow(DFWindow *Wnd, int x, int y)
{
	DFWindow *hWnd;
	if (Wnd != 0)	{
		// ------ search window's children
		DFWindow *sWnd = Wnd->Last();
		while (sWnd != 0)	{
			hWnd = inWindow(sWnd, x, y);
			if (hWnd != 0)
				return hWnd;
			sWnd = sWnd->Prev();
		}
		// ---- test this window
		if (!Wnd->isVisible() || !Wnd->WindowRect().Inside(x, y))
			Wnd = 0;
	}
	return Wnd;
}



