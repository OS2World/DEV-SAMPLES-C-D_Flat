// ------------ listbox.cpp

#include "listbox.h"
#include "keyboard.h"

static Color col = {
	BLACK,			// fg
	LIGHTGRAY,		// bg
	LIGHTGRAY,		// selected fg
	BLACK,			// selected bg
	BLACK,			// frame fg
	LIGHTGRAY,		// frame bg
	LIGHTGRAY,		// inactive fg
	BLACK			// inactive bg
};

// ----------- common constructor code
void ListBox::OpenWindow()
{
    windowtype = ListboxWindow;
    selection = -1;
    addmode = False;
    anchorpoint = -1;
    selectcount = 0;
	colors = col;
}

void ListBox::ClearSelection()
{
    if (selection != -1)
        WriteTextLine(selection, colors.fg, colors.bg);
}

void ListBox::ClearText()
{
	TextBox::ClearText();
	OpenWindow();
}

void ListBox::SetSelection(int sel)
{
    ClearSelection();
    if (sel >= 0 && sel < wlines)    {
        selection = sel;
        WriteTextLine(sel, colors.sfg, colors.sbg);
		Select();
    }
}

void ListBox::Keyboard(int key)
{
    int sel = selection; // (ClearSelection changes selection)
    switch (key)    {
        case UP:
            if (sel > 0)    {
                ClearSelection();
                if (sel == wtop)
                    ScrollDown();
                SetSelection(sel-1);
            }
            return;
        case DN:
            if (sel < wlines-1)    {
                ClearSelection();
                if (sel == wtop+ClientHeight()-1)
                    ScrollUp();
                SetSelection(sel+1);
            }
            return;
        case '\r':
            Choose();
            return;
        default:
            break;
    }
    TextBox::Keyboard(key);
}

// ---------- paint the listbox
void ListBox::Paint()
{
    TextBox::Paint();
    if (text != 0)
        WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::ScrollUp()
{
    TextBox::ScrollUp();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::ScrollDown()
{
    TextBox::ScrollDown();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::ScrollRight()
{
    TextBox::ScrollRight();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::ScrollLeft()
{
    TextBox::ScrollLeft();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::PageUp()
{
    TextBox::PageUp();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::PageDown()
{
    TextBox::PageDown();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::PageRight()
{
    TextBox::PageRight();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

void ListBox::PageLeft()
{
    TextBox::PageLeft();
    WriteTextLine(selection, colors.sfg, colors.sbg);
}

// ---------- Left mouse button was clicked
void ListBox::LeftButton(int mx, int my)
{
	if (my != prevmouseline)    {
		if (ClientRect().Inside(mx, my))    {
			int y = my - ClientTop();
            if (wlines && y < wlines-wtop)
                SetSelection(wtop+y);
        }
    }
    DFWindow::LeftButton(mx, my);
}

void ListBox::DoubleClick(int mx, int my)
{
    if (ClientRect().Inside(mx, my))    {
        my -= ClientTop();
        if (wlines && my < wlines-wtop)
            Choose();
    }
    DFWindow::DoubleClick(mx, my);
}

void ListBox::ButtonReleased(int, int)
{
    prevmouseline = -1;
}

