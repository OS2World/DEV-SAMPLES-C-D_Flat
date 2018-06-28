// -------- dtimer.cpp
//
// OS/2 Version created to use the OS/2 Timer Services
// jw21sep93
//
// renamed from 'timer.cpp' to match header
//

#include <stdio.h>

#include "dtimer.h"

Timer::Timer()
    {
    // create semaphore
    ULONG SemAttr = DC_SEM_SHARED;      // needs to be shared
    disabled = True;                    // start in disabled state
    timer = 0;

    rc = DosCreateEventSem(NULL, &sem, SemAttr, 1);

    if(rc != NO_ERROR)
        {
        printf("DosCreateEventSem failed: rc = %d\n",rc);
        }

    }

Timer::~Timer()
    {
    rc = DosStopTimer(timer);
    rc = DosCloseEventSem(sem);
    }

void Timer::SetTimer(int ticks) 
    {
    ULONG ct;

    disabled = False;
    if (timer)
        {
        rc = DosStopTimer(timer);
        }

    rc = DosResetEventSem(sem, &ct);
    if(rc != NO_ERROR  && rc != ERROR_ALREADY_RESET)
        {
        printf("DosResetEventSem failed: rc = %d\n",rc);
        }

    rc = DosAsyncTimer(ticks*18, (HSEM)sem, &timer); 
    if(rc != NO_ERROR)
        {
        printf("DosAsyncTimer failed: rc = %d\n",rc);
        }
    }

void Timer::DisableTimer()
    {
    disabled = True;
    }

Bool Timer::TimedOut()
    {
    ULONG ct = 0L;

    if (disabled == False)
        {
        rc = DosQueryEventSem(sem, &ct);
        if(rc != NO_ERROR)
            {
            printf("DosQueryEventSem failed: rc = %d\n",rc);
            }
        }
    return (Bool) (ct != 0L);
    }


Bool Timer::TimerRunning()
    {
    ULONG ct;

    if (disabled == False)
        {
        rc = DosQueryEventSem(sem, &ct);
	    if(rc != NO_ERROR)
	        {
	        printf("DosQueryEventSem failed: rc = %d\n",rc);
	        }
        return (Bool) (ct == 0L);
        }

    return(False);
    }
