// ---------- mouse.h
//
// modified for OS/2 operation - jw21sep93

#ifndef MOUSE_H
#define MOUSE_H

#define INCL_SUB
#define INCL_NOPMAPI
#include <os2.h>

#include "dfwindow.h"
#include "dtimer.h"      // name changed to avoid collision with Borland

class Mouse    
    {
	enum State
	    {
	    STATE_MOUSE_IDLE,
	    STATE_MOUSE_DEPRESSED,
	    STATE_MOUSE_RELEASED,
	    STATE_MOUSE_DWAIT,
	    STATE_MOUSE_REPEAT
	    };

	enum Event
	    {
	    EVENT_MOUSE_MOVED,
	    EVENT_MOUSE_DEPRESSED,
	    EVENT_MOUSE_RELEASED,
	    EVENT_MOUSE_TIMEREXPIRED
	    };

    HMOU         hMou;          // mouse device handle
    MOUEVENTINFO previnfo;      // previous mouse info struct
    MOUEVENTINFO currinfo;      // current mouse info struct
    NOPTRRECT    inclRect;      // inclusion rectangle
    Bool         installed;     // True = mouse is installed
    Bool         timerRunning;  // True = timer was running when last checked
    Timer        timer;         // mouse timer (used for double-clicks,
                                //   delays and repeats)

    int          delayticks;    // delay before mouse typematic activates
    int          repeatticks;   // delay between mouse typematic repeats
    int          doubleticks;   // double click period

    enum State   curState;      // current state of mouse

    DFWindow    *MouseWindow(int mx, int my);
    void         HandleEvent(enum Event event);

public:
    Mouse();
    ~Mouse();
    Bool Installed() { return installed; }
    void GetPosition(int &x, int &y);   // get mouse position
    void SetPosition(int x, int y);     // set mouse position
    void Show();                        // show the mouse cursor
    void Hide();                        // hide the mouse cursor
    void SetTravel(int minx, int maxx, int miny, int maxy);
    void DispatchEvent();               // process mouse events
    };

// -------- timer delays for mouse repeat, double clicks

const int REPEAT    =  2;
const int DELAY     =  20;
const int DOUBLE    =  14;

// -------- event detection constants

const USHORT Movement = MOUSE_MOTION |
                        MOUSE_MOTION_WITH_BN1_DOWN |
                        MOUSE_MOTION_WITH_BN2_DOWN |
                        MOUSE_MOTION_WITH_BN3_DOWN;

const USHORT Depression = MOUSE_BN1_DOWN |
                          MOUSE_MOTION_WITH_BN1_DOWN;

#endif


