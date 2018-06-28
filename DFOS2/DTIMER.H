// ----------- dtimer.h
//
// modified for OS/2 operation - jw21sep93
//
// uses OS/2 Timer services rather than doing
// own timer management.
//
// renamed to avoid collision with Borlands Timer.h 
//

#ifndef TIMER_H
#define TIMER_H

#define INCL_BASE
#define INCL_DOSDATETIME
#define INCL_NOPMAPI
#include <os2.h>

#include "dflatdef.h"


class Timer	
    {
    HEV     sem;
	HTIMER  timer;
    APIRET  rc;
    Bool    disabled;

public:
	Timer();
	~Timer();
	Bool    TimedOut();
	void    SetTimer(int ticks);
	void    DisableTimer(); 	 
	Bool    TimerRunning(); 	 
	void    Countdown() 		 { ; }
	Bool    TimerDisabled() 	 { return disabled; }
    };


#endif
