// ------------- editbox.cpp

#include <ctype.h>
#include "desktop.h"
#include "editbox.h"

// ----------- common constructor code
void EditBox::OpenWindow()
{
	windowtype = EditboxWindow;
	column = 0;
	changed = False;
	text = new String(1);
	BuildTextPointers();
}

Bool EditBox::SetFocus()
{
	Bool rtn = TextBox::SetFocus();
	if (rtn)	{
		ResetCursor();
		desktop.cursor().Show();
	}
	return rtn;
}

void EditBox::ResetFocus()
{
	desktop.cursor().Hide();
	TextBox::ResetFocus();
}

// -------- process keystrokes
void EditBox::Keyboard(int key)
{
	int shift = desktop.keyboard().GetShift();
	if ((shift & ALTKEY) == 0)	{
    	switch (key)    {
        	case HOME:
            	Home();
				return;
        	case END:
            	End();
				return;
        	case CTRL_FWD:
            	NextWord();
				return;
        	case CTRL_BS:
            	PrevWord();
				return;
        	case FWD:
            	Forward();
				return;
        	case BS:
            	Backward();
				return;
        	case RUBOUT:
				if (CurrentCharPosition() == 0)
					break;
            	Backward();
				// --- fall through
        	case DEL:
            	DeleteCharacter();
				BuildTextPointers();
				PaintCurrentLine();
            	return;
        	default:
            	if (!isprint(key))
                	break;
            	// --- all printable keys get processed by editbox
				InsertCharacter(key);
				BuildTextPointers();
				PaintCurrentLine();
            	return;
    	}
	}
	TextBox::Keyboard(key);
}

// -------- paint the editbox
void EditBox::Paint()
{
	TextBox::Paint();
	ResetCursor();
}

// -------- move the editbox
void EditBox::Move(int x, int y)
{
	TextBox::Move(x, y);
	ResetCursor();
}

// --------- clear the text from the editbox
void EditBox::ClearText()
{
	TextBox::ClearText();
	OpenWindow();
	ResetCursor();
}

void EditBox::Home()
{
	column = 0;
	if (wleft)	{
		wleft = 0;
		Paint();
	}
	ResetCursor();
}

void EditBox::End()
{
	int ch;
	while ((ch = CurrentChar()) != '\0' && ch != '\n')
		column++;
	if (column - wleft >= ClientWidth())	{
		wleft = column - ClientWidth() + 1;
		Paint();
	}
	ResetCursor();
}

void EditBox::NextWord()
{
	while (!isWhite(CurrentChar()) && CurrentChar())
		Forward();
	while (isWhite(CurrentChar()))
		Forward();
}

void EditBox::PrevWord()
{
	Backward();
	while (isWhite(CurrentChar()) && !AtBufferStart())
		Backward();
	while (!isWhite(CurrentChar()) && !AtBufferStart())
		Backward();
	if (isWhite(CurrentChar()))
		Forward();
}

void EditBox::Forward()
{
	if (CurrentChar())	{
		column++;
		if (column-wleft == ClientWidth())
			ScrollLeft();
		ResetCursor();
	}
}

void EditBox::Backward()
{
	if (column)	{
		if (column == wleft)
			ScrollRight();
		--column;
		ResetCursor();
	}
}

void EditBox::InsertCharacter(int key)
{
	unsigned col = CurrentCharPosition();
	if (insertmode || CurrentChar() == '\0')	{
		// ---- shift the text to make room for new character
		String ls, rs;
		if (col)
			ls = text->left(col);
		int rt = text->Strlen()-col;
		if (rt > 0)
			rs = text->right(rt);
		*text = ls + " " + rs;
	}
	(*text)[col] = (char) key;
	if (key == '\n')
		BuildTextPointers();
	Forward();
	changed = True;
}

void EditBox::DeleteCharacter()
{
	if (CurrentChar())	{
		String ls, rs;
		unsigned col = CurrentCharPosition();
		if (col)
			ls = text->left(col);
		int rt = text->Strlen()-col-1;
		if (rt > 0)
			rs = text->right(rt);
		*text = ls + rs;
		changed = True;
	}
}

void EditBox::SetCursor(int x, int y)
{
	desktop.cursor().SetPosition(
		x+ClientLeft()-wleft, y+ClientTop()-wtop);
}

void EditBox::LeftButton(int mx, int my)
{
	if (ClientRect().Inside(mx, my))	{
		column = max(0, min(text->Strlen()-1, mx-ClientLeft()+wleft));
		ResetCursor();
	}
	else
		TextBox::LeftButton(mx, my);
}

void EditBox::SetCursorSize()
{
	if (insertmode)
	    desktop.cursor().BoxCursor();
	else 
	    desktop.cursor().NormalCursor();
}

void EditBox::ResetCursor()
{
	SetCursorSize();
	if (visible)
		SetCursor(column, 0);
}

