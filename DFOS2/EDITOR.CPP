// ----- editor.cpp

#include "editor.h"
#include "desktop.h"

// ----------- common constructor code
void Editor::OpenWindow()
{
	windowtype = EditorWindow;
	row = 0;
	tabs = 4;
	insertmode = desktop.keyboard().InsertMode();
	wordwrapmode = True;
	DblBorder = False;
}

// ---- keep the cursor out of tabbed space
void Editor::AdjustCursorTabs(int key)
{
	while (CurrentChar() == Ptab)
		key == FWD ? column++ : --column;
	ResetCursor();
}

// -------- process keystrokes
void Editor::Keyboard(int key)
{
	int svwtop = wtop;
	int svwleft = wleft;
    switch (key)    {
		case '\t':
			InsertTab();
			BuildTextPointers();
			PaintCurrentLine();
			ResetCursor();
			break;
		case ALT_P:
			FormParagraph();
			break;
		case UP:
			Upward();
			break;
		case DN:
			Downward();
			break;
		case CTRL_HOME:
			BeginDocument();
			break;
		case CTRL_END:
			EndDocument();
			break;
		case '\r':
			InsertCharacter('\n');
			BuildTextPointers();
			ResetCursor();
			Paint();
			break;
		case DEL:
		case RUBOUT:
			visible = False;
			EditBox::Keyboard(key);
			visible = True;
			BuildTextPointers();
			PaintCurrentLine();
			ResetCursor();
			break;
		default:
			EditBox::Keyboard(key);
			break;
	}
	if (svwtop != wtop || svwleft != wleft)
		Paint();
}

void Editor::Forward()
{
	if (CurrentChar())	{
		if (CurrentChar() == '\n')	{
			Home();
			Downward();
		}
		else
			EditBox::Forward();
		AdjustCursorTabs(FWD);
	}
}

void Editor::Backward()
{
	if (column)
		EditBox::Backward();
	else if (row)	{
		Upward();
		End();
	}
	AdjustCursorTabs();
}

void Editor::Upward()
{
	if (row)	{
		if (row == wtop)
			ScrollDown();
		--row;
		AdjustCursorTabs();
	}
}

void Editor::Downward()
{
	if (row < wlines)	{
		if (row == wtop + ClientHeight() - 1)
			ScrollUp();
		row++;
		AdjustCursorTabs();
	}
}

void Editor::BeginDocument()
{
	TextBox::Home();
	column = 0;
	row = 0;
	AdjustCursorTabs();
}

void Editor::EndDocument()
{
	TextBox::End();
	row = wlines-1;
	End();
	AdjustCursorTabs();
}

void Editor::ResetCursor()
{
	column = min(column, LineLength(row));
	row = min(row, wlines);
	SetCursorSize();
	if (visible)
		SetCursor(column, row);
}

// ------- page up one screenfull
void Editor::PageUp()
{
	row -= ClientHeight();
	if (row < 0)
		row = 0;
	EditBox::PageUp();
	AdjustCursorTabs();
}

// ------- page down one screenfull
void Editor::PageDown()
{
	row += ClientHeight();
	if (row >= wlines)
		row = wlines-1;
	EditBox::PageDown();
	AdjustCursorTabs();
}

void Editor::InsertTab()
{
	visible = False;
	if (insertmode)	{
		EditBox::InsertCharacter('\t');
		while ((column % tabs) != 0)
			EditBox::InsertCharacter(Ptab);
	}
	else
		do
			Forward();
		while ((column % tabs) != 0);
	visible = True;
}

// --- When inserting char, adjust next following tab, same line
void Editor::AdjustTabInsert()
{
	visible = False;
	// ---- test if there is a tab beyond this character
	int savecol = column;
	while (CurrentChar() && CurrentChar() != '\n')	{
		if (CurrentChar() == '\t')	{
			column++;
			if (CurrentChar() == Ptab)
				EditBox::DeleteCharacter();
			else
				for (int i = 0; i < tabs-1; i++)
					EditBox::InsertCharacter(Ptab);
			break;
		}
		column++;
	}
	column = savecol;
	visible = True;
}

// --- test for wrappable word and wrap it
void Editor::WordWrap()
{
	// --- test for word wrap
	int len = LineLength(row);
	int wd = ClientWidth()-1;
	if (len >= wd)	{
		const char *cp = TextLine(row);
		char ch = *(cp + wd);
		// --- test words beyond right margin
		if (len > wd || (ch && ch != ' ' && ch != '\n'))	{
			// --- test typing in last word in window's line
			const char *cw = cp + wd;
			cp += column;
			while (cw > cp)	{
				if (*cw == ' ')
					break;
				--cw;
			}
			int newcol = 0;
			if (cw <= cp)	{
				// --- user was typing last word on line
				// --- find beginning of the word
				const char *cp1 = TextLine(row);
				const char *cw1 = cw;
				while (*cw1 != ' ' && cw1 > cp1)
					--cw1, newcol++;
				wleft = 0;
			}
			FormParagraph();
			if (cw <= cp)	{
				// --- user was typing last word on line
				column = newcol;
				if (cw == cp)
					--column;
				row++;
				ResetCursor();
			}
		}
	}
}

void Editor::InsertCharacter(int key)
{
	if (insertmode)	{
		if (key != '\n')
			AdjustTabInsert();
	}
	else if (CurrentChar() == '\t')	{
		// --- overtyping a tab
		visible = False;
		column++;
		while (CurrentChar() == Ptab)
			EditBox::DeleteCharacter();
		--column;
	}
	visible = False;
	EditBox::InsertCharacter(key);
	visible = True;
	ResetCursor();
	if (wordwrapmode)
		WordWrap();
}

// --- When deleting char, adjust next following tab, same line
void Editor::AdjustTabDelete()
{
	visible = False;
	// ---- test if there is a tab beyond this character
	int savecol = column;
	while (CurrentChar() && CurrentChar() != '\n')	{
		if (CurrentChar() == '\t')	{
			column++;
			// --- count pseudo tabs
			int pct = 0;
			while (CurrentChar() == Ptab)
				pct++, column++;
			if (pct == tabs-1)	{
				column -= tabs-1;
				for (int i = 0; i < tabs-1; i++)
					EditBox::DeleteCharacter();
			}
			else
				EditBox::InsertCharacter(Ptab);
			break;
		}
		column++;
	}
	column = savecol;
	visible = True;
}

void Editor::DeleteCharacter()
{
	if (CurrentChar() == '\0')
		return;
	if (insertmode)
		AdjustTabDelete();
	if (CurrentChar() == '\t')	{
		// --- deleting a tab
		EditBox::DeleteCharacter();
		while (CurrentChar() == Ptab)
			EditBox::DeleteCharacter();
		return;
	}
	const char *cp = TextLine(row);
	const char *cw = cp + column;
	const Bool delnewline = (Bool) (*cw == '\n');
	const Bool reform = (Bool) (delnewline && *(cw+1) != '\n');
	const Bool lastnewline = (Bool) (delnewline && *(cw+1) == '\0');
	int newcol = 0;
	if (reform && !lastnewline)	{
		// --- user is deleting /n, find beginning of the last word
		while (*--cw != ' ' && cw > cp)
			newcol++;
	}
	EditBox::DeleteCharacter();
	if (lastnewline)
		return;
	if (delnewline && !reform)	{
		// --- user deleted a blank line
		visible = True;
		BuildTextPointers();
		Paint();
		return;
	}
	if (wordwrapmode && reform)	{
		// --- user deleted /n
		wleft = 0;
		FormParagraph();
		if (CurrentChar() == '\n')	{
			// ---- joined the last word with next line's
			//      first word and then wrapped the result
			column = newcol;
			row++;
		}
	}
}

// --- form a paragraph from the current cursor position
//    through one line before the next blank line or end of text
void Editor::FormParagraph()
{
    int BegCol, FirstLine;
    const char *BlkBegLine, *BlkEndLine, *BlkBeg;

    // ---- forming paragraph from cursor position
    FirstLine = wtop + row;
    BlkBegLine = BlkEndLine = TextLine(row);
    if ((BegCol = column) >= ClientWidth())
        BegCol = 0;
    // ---- locate the end of the paragraph
    while (*BlkEndLine)    {
        Bool blank = True;
        const char *BlankLine = BlkEndLine;
        // --- blank line marks end of paragraph
        while (*BlkEndLine && *BlkEndLine != '\n')    {
            if (*BlkEndLine != ' ')
                blank = False;
            BlkEndLine++;
        }
        if (blank)    {
            BlkEndLine = BlankLine;
            break;
        }
        if (*BlkEndLine)
            BlkEndLine++;
    }
    if (BlkEndLine == BlkBegLine)    {
		visible = True;
		Downward();
        return;
    }
    if (*BlkEndLine == '\0')
        --BlkEndLine;
    if (*BlkEndLine == '\n')
        --BlkEndLine;
    // --- change all newlines in block to spaces
	BlkBeg = BlkBegLine;
    while (BlkBeg < BlkEndLine)    {
        if (*BlkBeg == '\n')	{
            int off = BlkBeg - (const char *)*text;
            (*text)[off] = ' ';
		}
		BlkBeg++;
    }
    // ---- insert newlines at new margin boundaries
    BlkBeg = BlkBegLine;
    while (BlkBegLine < BlkEndLine)    {
        BlkBegLine++;
        if ((int)(BlkBegLine - BlkBeg) == ClientWidth()-1)    {
            while (*BlkBegLine != ' ' && BlkBegLine > BlkBeg)
                --BlkBegLine;
            if (*BlkBegLine != ' ')    {
                BlkBegLine = strchr(BlkBegLine, ' ');
                if (BlkBegLine == NULL || BlkBegLine >= BlkEndLine)
                    break;
            }
            int off = BlkBegLine - (const char *)*text;
            (*text)[off] = '\n';
            BlkBeg = BlkBegLine+1;
        }
    }
    BuildTextPointers();
	changed = True;
    // --- put cursor back at beginning
    column = BegCol;
    if (FirstLine < wtop)
        wtop = FirstLine;
    row = FirstLine - wtop;
	visible = True;
	Paint();
	ResetCursor();
}

// --------- add a line of text to the editor textbox
void Editor::AddText(const char *txt)
{
	// --- compute the buffer size based on tabs in the text
	const char *tp = txt;
	int x = 0;
	int sz = 0;
	while (*tp)	{
		if (*tp == '\t')	{
			// --- tab, adjust the buffer length
			int sps = Tabs() - (x % Tabs());
			sz += sps;
			x += sps;
		}
		else	{
			// --- not a tab, count the character
			sz++;
			x++;
		}
		if (*tp == '\n')
			x = 0;	// newline, reset x
		tp++;
	}
	// --- allocate a buffer
	char *ep = new char[sz];
	// --- detab the input file
	tp = txt;
	char *ttp = ep;
	x = 0;
	while (*tp)	{
		// --- put the character (\t, too) into the buffer
		*ttp++ = *tp;
		x++;
		// --- expand tab into one \t and pseudo-spaces (\t + 0x80)
		if (*tp == '\t')
			while ((x % Tabs()) != 0)
				*ttp++ = Ptab, x++;
		else if (*tp == '\n')
			x = 0;
		tp++;
	}
	*ttp = '\0';
	// ---- add the text to the editor window
	EditBox::AddText(ep);
}

// ------- retrieve editor text collapsing tabs
const String Editor::GetText()
{
	char *tx = new char[text->Strlen()+1];
	const char *tp = (const char *) *text;
	char *nt = tx;
	while (*tp)	{
		if (*(const unsigned char *)tp != Ptab)
			*tx++ = *tp;
		tp++;
	}
	*tx = '\0';
	String temp(nt);
	return nt;
}

void Editor::WriteString(String &ln, int x, int y, int fg, int bg)
{
	String nln(ln.Strlen());
	int ch;
	for (int i = 0; i < ln.Strlen(); i++)	{
		ch = ln[i];
		nln[i] = (ch & 0x7f) == '\t' ? ' ' : ch;
	}
	EditBox::WriteString(nln, x, y, fg, bg);
}

void Editor::LeftButton(int mx, int my)
{
	if (ClientRect().Inside(mx, my))	{
		column = mx-ClientLeft()+wleft;
		row = my-ClientTop()+wtop;
		if (row > wlines-1)
			row = wlines+1;
		ResetCursor();
	}
	else
		TextBox::LeftButton(mx, my);
}

// --------- clear the text from the editor window
void Editor::ClearText()
{
	OpenWindow();
	EditBox::ClearText();
}


