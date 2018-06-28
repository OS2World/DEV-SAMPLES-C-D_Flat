// ------------- mouse.cpp
//
// rewritten for OS/2 operation - jw26sep93

#include <stdio.h>
#include "desktop.h"

// removed Moved(), LeftButton() and ButtonReleased() as these are only used 
// internally by the mouse, and should not really ever have been public.
// Possibly the same should happen for GetPosition.

Mouse::Mouse()
    {
    USHORT EventMask = MOUSE_MOTION | 
                       MOUSE_BN1_DOWN | 
                       MOUSE_MOTION_WITH_BN1_DOWN;

    installed = False;

    if (MouOpen(NULL, &hMou) == NO_ERROR)
        {
        installed = True;
        MouFlushQue(hMou);

        // should probably save and restore mouse state

        repeatticks = REPEAT;
        delayticks  = DELAY;
        doubleticks = DOUBLE;

        previnfo.col = 
        previnfo.row = -1;

        curState = STATE_MOUSE_IDLE;

        MouSetEventMask(&EventMask, hMou);

        SetTravel(0,
                  desktop.screen().Width()-1,
                  0,
                  desktop.screen().Height()-1);
        }
    }


Mouse::~Mouse()
    {
    if (installed)    
        {
        Hide();
        }
    }


// ------ get the window to send mouse events
DFWindow *Mouse::MouseWindow(int mx, int my)
    {
    DFWindow *Mwnd = desktop.inWindow(mx, my);

    if (Mwnd == 0)
        {
        Mwnd = desktop.FocusCapture();
        }
    else if (desktop.FocusCapture() != 0)
        {
        if (!Mwnd->isDescendedFrom(desktop.FocusCapture()))
            {
            Mwnd = desktop.FocusCapture();
            }
        }
    return Mwnd;
    }

void Mouse::GetPosition(int &mx, int &my)
    {
    mx = currinfo.col;
    my = currinfo.row;
    }


void Mouse::SetPosition(int x, int y)
    {
    PTRLOC  loc;

    if (installed)    
        {
        loc.row = (USHORT)y;
        loc.col = (USHORT)x;
        MouSetPtrPos(&loc, hMou);
        }
    }


void Mouse::Show()
    {
    MouDrawPtr(hMou);
    }

void Mouse::Hide()
    {
    NOPTRRECT   rect;

    if (installed) 
        {
        rect.row = 0;
        rect.col = 0;
        rect.cRow = (USHORT)(desktop.screen().Height()-1);
        rect.cCol = (USHORT)(desktop.screen().Width()-1);
        MouRemovePtr(&rect, hMou);
        }
    }

void Mouse::SetTravel(int minx, int maxx, int miny, int maxy)
    {
    inclRect.col = (USHORT)minx;
    inclRect.row = (USHORT)miny;
    inclRect.cCol = (USHORT)maxx;
    inclRect.cRow = (USHORT)maxy;
    }


// -------- Event dispatch and handling -----

void Mouse::DispatchEvent()
    {
    MOUQUEINFO  qi;
    USHORT      wait = MOU_NOWAIT;

    if (installed)
        {
        MouGetNumQueEl(&qi, hMou);
    
        if(qi.cEvents != 0)
            {
            previnfo = currinfo;                      
            MouReadEventQue(&currinfo, &wait, hMou);


            // if moved: ensure coordinates are
            //           within inclusion rectangle
            if ((currinfo.row != previnfo.row) ||
                (currinfo.col != previnfo.col))
                {
                // handle inclusion rectangle
                if (currinfo.row < inclRect.row)
                    {
                    currinfo.row = inclRect.row;
                    }

                if (currinfo.col < inclRect.col)
                    {
                    currinfo.col = inclRect.col;
                    }

                if (currinfo.row > inclRect.cRow)
                    {
                    currinfo.row = inclRect.cRow;
                    }

                if (currinfo.col > inclRect.cCol)
                    {
                    currinfo.col = inclRect.cCol;
                    }

                HandleEvent(EVENT_MOUSE_MOVED);
                }

            // Handle changes in button state
            int DepressedNow    = ((currinfo.fs & Depression) != 0);
            int DepressedBefore = ((previnfo.fs & Depression) != 0);

            if (!DepressedNow && DepressedBefore) 
                {
                HandleEvent(EVENT_MOUSE_RELEASED);
                }

            if (DepressedNow && !DepressedBefore) 
                {
                HandleEvent(EVENT_MOUSE_DEPRESSED);
                }

            }

        // Handle timer events
        if (timerRunning && timer.TimedOut())
            {
            HandleEvent(EVENT_MOUSE_TIMEREXPIRED);
            timerRunning = False;
            }
        else if (!timerRunning && timer.TimerRunning())
            {
            timerRunning = True;
            }
        }
    return;
    }


void Mouse::HandleEvent(Event event)
    {
    DFWindow *Mwnd;

    if ((Mwnd = MouseWindow(currinfo.col, currinfo.row)) == NULL)
        {
        return;
        }

    switch(curState)
        {
        case STATE_MOUSE_IDLE:
            {
            switch(event)
                {
                case EVENT_MOUSE_MOVED:
                    {
                    // dispatch MouseMoved message to window
                    Mwnd->MouseMoved(currinfo.col, currinfo.row);
                    break;
                    }
                case EVENT_MOUSE_DEPRESSED:
                    {
                    // change to 'depressed' state
                    curState = STATE_MOUSE_DEPRESSED;
                    if (Mwnd->SetFocus())
                        {
                        Mwnd->LeftButton(currinfo.col, currinfo.row);
                        }
                    timer.SetTimer(delayticks);
                    break;
                    }
                case EVENT_MOUSE_RELEASED:
                    {
                    // Error - cannot happen !
                    DosBeep(1000, 200);
                    DosBeep(500, 500);
                    break;
                    }
                case EVENT_MOUSE_TIMEREXPIRED:
                    {
                    // Ignore
                    break;
                    }
                }
            break;
            }

        case STATE_MOUSE_DEPRESSED:
            {
            switch(event)
                {
                case EVENT_MOUSE_MOVED:
                    {
                    // dispatch MouseMoved message to window
                    Mwnd->MouseMoved(currinfo.col, currinfo.row);
                    break;
                    }
                case EVENT_MOUSE_DEPRESSED:
                    {
                    // Error - cannot happen !
                    DosBeep(1000, 500);
                    DosBeep(500, 500);
                    break;
                    }
                case EVENT_MOUSE_RELEASED:
                    {
                    // Possible double click
                    curState = STATE_MOUSE_RELEASED;
                    timer.SetTimer(doubleticks);
                    Mwnd->ButtonReleased(currinfo.col, currinfo.row);
                    break;
                    }
                case EVENT_MOUSE_TIMEREXPIRED:
                    {
                    // Held down a while; start typematic
                    curState = STATE_MOUSE_REPEAT;
                    timer.SetTimer(repeatticks);
                    Mwnd->LeftButton(currinfo.col, currinfo.row);
                    break;
                    }
                }
            break;
            }
    
        case STATE_MOUSE_RELEASED:
            {
            switch(event)
                {
                case EVENT_MOUSE_MOVED:
                    {
                    // dispatch MouseMoved message to window
                    curState=STATE_MOUSE_IDLE;
                    Mwnd->MouseMoved(currinfo.col, currinfo.row);
                    break;
                    }
                case EVENT_MOUSE_DEPRESSED:
                    {
                    // Probably a double-click
                    curState=STATE_MOUSE_DWAIT;
                    Mwnd->LeftButton(currinfo.col, currinfo.row);
                    break;
                    }
                case EVENT_MOUSE_RELEASED:
                    {
                    // Error - cannot happen !
                    DosBeep(500, 500);
                    DosBeep(1000, 500);
                    break;
                    }
                case EVENT_MOUSE_TIMEREXPIRED:
                    {
                    // OK, not a double click, so go to Idle state
                    curState=STATE_MOUSE_IDLE;
                    break;
                    }
                }
            break;
            }

        case STATE_MOUSE_DWAIT:
            {
            switch(event)
                {
                case EVENT_MOUSE_MOVED:
                    {
                    // dispatch MouseMoved message to window
                    curState = STATE_MOUSE_DEPRESSED;
                    Mwnd->MouseMoved(currinfo.col, currinfo.row);
                    // not a double-click, so go to depressed state
                    timer.SetTimer(delayticks);
                    break;
                    }
                case EVENT_MOUSE_DEPRESSED:
                    {
                    // Error - cannot happen !
                    DosBeep(500, 500);
                    DosBeep(1000, 500);
                    DosBeep(500, 500);
                    break;
                    }
                case EVENT_MOUSE_RELEASED:
                    {
                    curState = STATE_MOUSE_IDLE;
                    Mwnd->DoubleClick(currinfo.col, currinfo.row);

                    // test needed, or might post to nonexistent window
                    if ((Mwnd = MouseWindow(currinfo.col, currinfo.row)) 
                        != NULL)
                        {
                        Mwnd->ButtonReleased(currinfo.col, currinfo.row);
                        }
                    break;
                    }

                case EVENT_MOUSE_TIMEREXPIRED:
                    {
                    // not a double-click, so go to depressed state
                    curState = STATE_MOUSE_DEPRESSED;
                    timer.SetTimer(delayticks);
                    break;
                    }
                }
            break;
            }
    
        case STATE_MOUSE_REPEAT:
            {
            switch(event)
                {
                case EVENT_MOUSE_MOVED:
                    {
                    // no longer in typematic repeat mode
                    curState = STATE_MOUSE_DEPRESSED;
                    timer.SetTimer(repeatticks);
                    // dispatch MouseMoved message to window
                    Mwnd->MouseMoved(currinfo.col, currinfo.row);
    
                    break;
                    }
                case EVENT_MOUSE_DEPRESSED:
                    {
                    // Error - cannot happen !
                    DosBeep(1000, 500);
                    break;
                    }
                case EVENT_MOUSE_RELEASED:
                    {
                    // no longer in typematic repeat mode
                    curState=STATE_MOUSE_IDLE;
                    // test needed, or might post to nonexistent window
//                    if ((Mwnd = MouseWindow(currinfo.col, currinfo.row)) 
//                            != NULL)
//                        {
                        Mwnd->ButtonReleased(currinfo.col, currinfo.row);
//                        }
                    break;
                    }
                case EVENT_MOUSE_TIMEREXPIRED:
                    {
                    // dispatch click message & reset
                    timer.SetTimer(repeatticks);
                    Mwnd->LeftButton(currinfo.col, currinfo.row);
                    break;
                    }
                }
            break;
            }
        }
    }
