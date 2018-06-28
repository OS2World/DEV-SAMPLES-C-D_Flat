// ----------- keyboard.cpp
//
// modified for OS/2 operation - jw21sep93

#include <stdio.h>
#include "desktop.h"

/* ----- table of alt keys for finding shortcut keys ----- */
static int altconvert[] = 
    {
    ALT_A,ALT_B,ALT_C,ALT_D,ALT_E,ALT_F,ALT_G,ALT_H,
    ALT_I,ALT_J,ALT_K,ALT_L,ALT_M,ALT_N,ALT_O,ALT_P,
    ALT_Q,ALT_R,ALT_S,ALT_T,ALT_U,ALT_V,ALT_W,ALT_X,
    ALT_Y,ALT_Z,ALT_0,ALT_1,ALT_2,ALT_3,ALT_4,ALT_5,
    ALT_6,ALT_7,ALT_8,ALT_9
    };


Keyboard::Keyboard()
    {
    hKbd = 0;

    KbdOpen(&hKbd);
    KbdGetFocus(IO_WAIT, hKbd);
    ki.cb = sizeof(ki);
    ki.fsMask = KEYBOARD_BINARY_MODE | KEYBOARD_SHIFT_REPORT;
    KbdSetStatus(&ki, hKbd);

    shift = GetShift();         // initialise shift state
    insert = True;
    }


/* ---- Test for keystroke ---- */
Bool Keyboard::KeyHit()
    {
    KbdPeek(&kki, hKbd);
    return (kki.fbStatus & ZEROFLAG) ? True : False;
    }

/* ---- Read a keystroke ---- */
int Keyboard::GetKey()
    {
    int c;

    KbdCharIn(&kki, IO_WAIT, hKbd);
    if (kki.chChar == 0x00 || kki.chChar == 0xE0)
        {
        c = kki.chScan | 0x1080;
        }
    else
        {
        c = kki.chChar & 0xff;
        }
    return c & 0x10ff;
    }

/* ---------- read the keyboard shift status --------- */

int Keyboard::GetShift()
    {
    register int os2State;
    register int state = 0;

    KbdGetStatus(&ki, hKbd);
    os2State = ki.fsState;

    if(os2State & KBDSTF_INSERT_ON) {state  = INSERTKEY; }
    if(os2State & KBDSTF_CAPSLOCK)  {state |= CAPSLOCK;  }
    if(os2State & KBDSTF_NUMLOCK)   {state |= NUMLOCK;   }
    if(os2State & KBDSTF_SCROLLLOCK){state |= SCROLLLOCK;}
    if(os2State & KBDSTF_ALT)       {state |= ALTKEY;    }
    if(os2State & KBDSTF_CONTROL)   {state |= CTRLKEY;   }
    if(os2State & KBDSTF_LEFTSHIFT) {state |= LEFTSHIFT; }
    if(os2State & KBDSTF_RIGHTSHIFT){state |= RIGHTSHIFT;}

    return state;
    }

/* ------ convert an Alt+ key to its letter equivalent ----- */
int Keyboard::AltConvert(int c)
    {
    int i = 0;
    int a = 0;

    for (i = 0; i < 36; i++)
        {
        if (c == altconvert[i])
            {
            break;
            }
        }

    if (i < 26)
        {
        a = 'a' + i;
        }
    else if (i < 36)
        {
        a = '0' + i - 26;
        }

    return a;
    }

Bool Keyboard::ShiftChanged()
    {
    int  sk  = GetShift();
    Bool rtn = (Bool) (sk != shift);
    shift    = sk;
    return rtn;
    }

// ------ dispatch keyboard events
void Keyboard::DispatchEvent()
    {
    DFWindow *Kwnd = desktop.InFocus();

    if (Kwnd == 0)
        {
        Kwnd = (DFWindow *) desktop.ApplWnd();
        }

    if (ShiftChanged())
        {
        // ---- the shift status changed
        Kwnd->ShiftChanged(GetShift());
        }

    if (KeyHit())    
        {
        // --- a key was pressed
        Kwnd->Keyboard(GetKey());
        Kwnd->KeyReleased();
        }
    }



