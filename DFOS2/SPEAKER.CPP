// -------- speaker.cpp
//
// modified for OS/2 operation - jw21sep93

#include "speaker.h"
#include "dflatdef.h"


void Speaker::Beep()
    {
    DosBeep(1000, 250);
    }
