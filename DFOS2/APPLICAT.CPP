// ------------ applicat.cpp

#include "dflatpp.h"

static Color col = {
	LIGHTGRAY, BLUE,
	LIGHTGRAY, BLUE,
	LIGHTGRAY, BLUE,
	LIGHTGRAY, BLUE
};

void Application::OpenWindow()
{
	colors = col;
    desktop.SetApplication(this);
    windowtype = ApplicationWindow;
    clearch = ClearChar;
    SetAttribute(BORDER | SAVESELF | CONTROLBOX | STATUSBAR);
    statusbar = new StatusBar(this);
    takingfocus = False;
}

void Application::CloseWindow()
{
    delete statusbar;
    desktop.SetApplication(0);
    DFWindow::CloseWindow();
}

void Application::AdjustBorders()
{
    DFWindow::AdjustBorders();
    if (attrib & MENUBAR)
        TopBorderAdj++;
    if (attrib & TOOLBAR)
        TopBorderAdj += 3;
    if (attrib & STATUSBAR)
        BottomBorderAdj = 1;
}

Bool Application::SetFocus()
{
    takingfocus = True;
    DFWindow::SetFocus();
    takingfocus = False;
    return True;
}

void Application::Show()
{
    if (!takingfocus || !isVisible())
        DFWindow::Show();
    else
        Border();
    desktop.mouse().Show();
}

void Application::Keyboard(int key)
{
	if (key == ' ')	// to catch Alt+Spacebar
		DFWindow::Keyboard(key);
	else	{
    	// ---- forward unprocessed keys to the menubar
		DFWindow *Wnd = First();
		while (Wnd != 0)	{
			if (Wnd->WindowType() == MenubarWindow)	{
				Wnd->Keyboard(key);
				break;
			}
			Wnd = Wnd->Next();
		}
	}
}

void Application::ClockTick()
{
    if (statusbar != 0)
        statusbar->ClockTick();
}

void Application::StatusMessage(String& Msg)
{
    if (statusbar != 0)
        statusbar->StatusMessage(Msg);
}

void Application::Execute(void)
{
    while (desktop.DispatchEvents())
        ;
}


