// ------------- dialog.cpp

#include <ctype.h>
#include "dialog.h"
#include "desktop.h"

static Color col = {
	LIGHTGRAY,BLUE,
	LIGHTGRAY,BLUE,
	LIGHTGRAY,BLUE,
	LIGHTGRAY,BLUE
};

// ----------- common constructor code
void Dialog::OpenWindow()
{
    windowtype = DialogWindow;
    DblBorder = False;
    SetAttribute(BORDER | SAVESELF | CONTROLBOX |
        MOVEABLE | SHADOW);
	colors = col;
    isRunning = False;
    okexit = False;
}

void Dialog::CloseWindow()
{
    DFWindow::CloseWindow();
    isRunning = False;
}

void Dialog::Keyboard(int key)
{
    switch (key)    {
        case ESC:
            CancelFunction();
            break;
        case '\r':
            OKFunction();
            break;
        case ALT_F4:
            CloseWindow();
            break;
        case ' ':
            if ((desktop.keyboard().GetShift() & ALTKEY) == 0)
                break;
            // ---- fall through
        case ALT_F6:
            DFWindow::Keyboard(key);
            break;
        default:
            if ((desktop.keyboard().GetShift() & ALTKEY) != 0)
                TestShortcut(key);
            break;
    }
}

void Dialog::TestShortcut(int key)
{
    key = desktop.keyboard().AltConvert(key);
    key = tolower(key);
    Control *Ctl = (Control *)First();
    while (Ctl != 0)    {
        if (key == Ctl->Shortcut())    {
            Ctl->ShortcutSelect();
            break;
        }
        Ctl = (Control *) Ctl->Next();
    }
}

void Dialog::Execute()
{
    CaptureFocus();
    Control *Ctl = (Control *) First();
    while (Ctl != 0)    {
        if (Ctl->isEnabled())    {
            Ctl->SetFocus();
            break;
        }
        Ctl = (Control *) Ctl->Next();
    }

    // ---- modal dialog box
    isRunning = True;
    while (isRunning)
        desktop.DispatchEvents();
}

void Dialog::OKFunction()
{
    okexit = True;
    CloseWindow();
}

void Dialog::CancelFunction()
{
    CloseWindow();
}

void Dialog::HelpFunction()
{
}


