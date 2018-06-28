// ---------- desktop.h

#ifndef DESKTOP_H
#define DESKTOP_H

#include <dir.h>
#include "screen.h"
#include "cursor.h"
#include "keyboard.h"
#include "mouse.h"
#include "speaker.h"
#include "clock.h"
#include "frame.h"

class DFWindow;
class Application;
class Dialog;

class DeskTop {
    Application *apwnd;     // application window
    DFWindow *infocus;      // current window with the focus
    DFWindow *focuscapture; // current window with captured focus
	int drive;				// drive upon open
	char *path;				// subdirectory upon open
    // ------- the desktop devices
    Screen   sysscreen;     // the system screen
    Mouse    sysmouse;      // the system mouse
    Keyboard syskeyboard;   // the system keyboard
    Cursor   syscursor;     // the system cursor
    Clock    sysclock;      // the system clock
    Speaker  sysspeaker;    // the system speaker
	Frame    sysframe;		// dummy for moving and sizing
	DFWindow *inWindow(DFWindow *wnd, int x, int y);
public:
    DeskTop();
    ~DeskTop();
    Application *ApplWnd() { return apwnd; }
    void SetApplication(Application *ApWnd) { apwnd = ApWnd; }
    Bool DispatchEvents();
    DFWindow *InFocus()      { return infocus; }
    DFWindow *FocusCapture() { return focuscapture; }
    void SetFocus(DFWindow *wnd) { infocus = wnd; }
    void SetFocusCapture(DFWindow *wnd) { focuscapture = wnd; }
	DFWindow *inWindow(int x, int y)
		{ return inWindow((DFWindow *)apwnd, x, y); }
    // ------- the desktop devices
    Mouse&    mouse()       { return sysmouse;    }
    Screen&   screen()      { return sysscreen;   }
    Keyboard& keyboard()    { return syskeyboard; }
    Cursor&   cursor()      { return syscursor;   }
    Clock&    clock()       { return sysclock;    }
    Speaker&  speaker()     { return sysspeaker;  }
	Frame&    frame()       { return sysframe; }
};

extern DeskTop desktop;

#endif

