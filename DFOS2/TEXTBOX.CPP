// ------------- textbox.cpp
// Fixed bug where Mouse Click in Ted caused an Access Violation. -jr27nov93

#include "desktop.h"
#include "textbox.h"
#include "scrolbar.h"

static Color col = {
	BLACK,				// fg
	LIGHTGRAY,			// bg
	LIGHTGRAY,			// selected fg
	BLACK,				// selected bg
	LIGHTGRAY,			// frame fg
	BLACK,				// frame bg
	LIGHTGRAY,			// highlighted fg
	BLUE				// highlighted bg
};

// ----------- common constructor code
void TextBox::OpenWindow()
{
    windowtype = TextboxWindow;
    text = 0;
    hscrollbar = vscrollbar = 0;
    TextPointers = 0;
	colors = col;
    ClearText();
}

void TextBox::CloseWindow()
{
    ClearText();
    delete hscrollbar;
    delete vscrollbar;
    Control::CloseWindow();
}


// ------ show the textbox
void TextBox::Show()
{
    if( (attrib & HSCROLLBAR) && hscrollbar == 0 )
    {
        hscrollbar = new ScrollBar(HORIZONTAL, this);
        hscrollbar->SetAttribute(FRAMEWND);
    }

    if ((attrib & VSCROLLBAR) && vscrollbar == 0)
    {
        vscrollbar = new ScrollBar(VERTICAL, this);
        vscrollbar->SetAttribute(FRAMEWND);
    }
    Control::Show();
}

// ------------ build the text line pointers
void TextBox::BuildTextPointers()
{
    textwidth = wlines = 0;
    // ---- count the lines of text
	const char *cp = *text;
    while( *cp )
    {
        wlines++;
        while( *cp && *cp != '\n' )
        {
            cp++;
        }
        if( *cp )
        {
            cp++;
        }
    }
    // ----- build the pointer array
    delete TextPointers;
    TextPointers = new unsigned int[wlines+1];
    unsigned int off = 0;
    cp = *text;
    wlines = 0;
    while (*cp)    {
        *(TextPointers + wlines++) = off;
        const char *cp1 = cp;
        while (*cp && *cp != '\n')
            cp++;
        textwidth = max(textwidth, (unsigned int) (cp - cp1));
        if (*cp)
            cp++;
        off = (unsigned int) (cp - *text);
    }
    *(TextPointers + wlines) = off;
}

// --------- add a line of text to the textbox
void TextBox::AddText(const char *txt)
{
    if (text == 0)
        text = new String(txt);
	else 
	    *text += txt;
	if (txt[strlen(txt)-1] != '\n')
		*text += "\n";
    BuildTextPointers();
}

// --------- set the textbox's text buffer to new text
void TextBox::SetText(const char *txt)
{
    ClearText();
    AddText(txt);
}

// ------ set the length of the text buffer
void TextBox::SetTextLength(unsigned int len)
{
    if (text != 0)
        text->ChangeLength(len);
}

// --------- clear the text from the textbox
void TextBox::ClearText()
{
    delete text;
	text = 0;
    wlines = 0;
    textwidth = 0;
    wtop = wleft = 0;
    ClearTextBlock();
    delete TextPointers;
	TextPointers = 0;
}

// ------- extract a text line
String TextBox::ExtractTextLine(int lno)
{
    const char *lp = TextLine(lno);
    int offset = lp - (const char *) *text;
    for (int len = 0; *(lp+len) && *(lp+len) != '\n'; len++)
        ;
    return text->mid(len, offset);
}

// ---- display a line with a shortcut key character
void TextBox::WriteShortcutLine(int lno, int fg, int bg)
{
    if (!visible)
        return;
    String sc = ExtractTextLine(lno);
    int x = sc.Strlen();
    int y = lno-wtop;
    x -= DisplayShortcutField(sc, 0, y, fg, bg);
    // --------- pad the line
    int wd = ClientWidth() - x;
    if (wd > 0)
        WriteClientString(String(wd, ' '), x, y, fg, bg);
}

// ---- display a shortcut field character
int TextBox::DisplayShortcutField(String sc, int x, int y,
                                                int fg, int bg)
{
    int scs = 0;
    if (visible)	{
    	int off = sc.FindChar(SHORTCUTCHAR);
    	if (off != -1)    {
        	scs++;
        	if (off != 0)    {
            	String ls = sc.left(off);
            	WriteClientString(ls, x, y, fg, bg);
        	}
        	WriteClientChar(sc[off+1], x+off, y, shortcutfg, bg);
        	int len = sc.Strlen()-off-2;
        	if (len > 0)    {
            	String rs = sc.right(len);
            	scs += DisplayShortcutField(rs, x+off+1, y, fg, bg);
        	}
    	}
    	else
        	WriteClientString(sc, x, y, fg, bg);
	}
    return scs;
}

// ------- write a text line to the textbox
void TextBox::WriteTextLine(int lno, int fg, int bg)
{
    if (!visible || lno < wtop || lno >= wtop + ClientHeight())
        return;
    int wd = ClientWidth();
    String tl = ExtractTextLine(lno);
    String ln = tl.mid(wd, wleft);
    int dif = wd-ln.Strlen();
    if (dif > 0)
        ln = ln + String(dif, ' ');    // pad the line with spaces
    // ----- display the line
    WriteClientString(ln, 0, lno-wtop, fg, bg);
}

// ---------- paint the textbox
void TextBox::Paint()
{
    if (visible)    {
    	if (text == 0 || wlines == 0)
        	Control::Paint();
    	else    {
        	int ht = ClientHeight();
        	int wd = ClientWidth();
        	int fg = colors.fg;
        	int bg = colors.bg;
        	for (int i = 0; i < min(wlines-wtop,ht); i++)
            	WriteTextLine(wtop+i, fg, bg);
        	// ---- pad the bottom lines in the window
        	String line(wd, ' ');
        	while (i < ht)
            	WriteClientString(line, 0, i++, fg, bg);
        	if (resetscrollbox)
            	SetScrollBoxes();
        	resetscrollbox = False;
    	}
	}
}

// ------ process a textbox keystroke
void TextBox::Keyboard(int key)
{
    switch (key)    {
        case UP:
            if (ClientTop() == ClientBottom())
                break;
            ScrollDown();
            return;
        case DN:
            if (ClientTop() == ClientBottom())
                break;
            ScrollUp();
            return;
        case FWD:
            ScrollLeft();
            return;
        case BS:
            ScrollRight();
            return;
        case PGUP:
            PageUp();
            return;
        case PGDN:
            PageDown();
            return;
        case CTRL_PGUP:
            PageLeft();
            return;
        case CTRL_PGDN:
            PageRight();
            return;
        case HOME:
            Home();
            return;
        case END:
            End();
            return;
        default:
            break;
    }
    Control::Keyboard(key);
}

// ------- scroll up one line
void TextBox::ScrollUp()
{
    if (wtop < wlines-1)    {
        int fg = colors.fg;
        int bg = colors.bg;
        desktop.screen().Scroll(ClientRect(), 1, fg, bg);
        wtop++;
        int ln = wtop+ClientHeight()-1;
        if (ln < wlines)
            WriteTextLine(ln, fg, bg);
        SetScrollBoxes();
    }
}

// ------- scroll down one line
void TextBox::ScrollDown()
{
    if (wtop)    {
        int fg = colors.fg;
        int bg = colors.bg;
        desktop.screen().Scroll(ClientRect(), 0, fg, bg);
        --wtop;
        WriteTextLine(wtop, fg, bg);
        SetScrollBoxes();
    }
}

// ------- scroll left one character
void TextBox::ScrollLeft()
{
    if (wleft < textwidth-1)
        wleft++;
    Paint();
}

// ------- scroll right one character
void TextBox::ScrollRight()
{
    if (wleft > 0)
        --wleft;
    Paint();
}

// ------- page up one screenfull
void TextBox::PageUp()
{
    if (wtop)    {
        wtop -= ClientHeight();
        if (wtop < 0)
            wtop = 0;
        resetscrollbox = True;
        Paint();
    }
}

// ------- page down one screenfull
void TextBox::PageDown()
{
    if (wtop < wlines-1)    {
        wtop += ClientHeight();
        if (wlines < wtop)
            wtop = wlines-1;
        resetscrollbox = True;
        Paint();
    }
}

// ------- page right one screenwidth
void TextBox::PageRight()
{
    if (wleft < textwidth-1)    {
        wleft += ClientWidth();
        if (textwidth < wleft)
            wleft = textwidth-1;
        resetscrollbox = True;
        Paint();
    }
}

// ------- page left one screenwidth
void TextBox::PageLeft()
{
    if (wleft)    {
        wleft -= ClientWidth();
        if (wleft < 0)
            wleft = 0;
        resetscrollbox = True;
        Paint();
    }
}

// ----- move to the first line of the textbox
void TextBox::Home()
{
    wtop = 0;
    Paint();
}

// ----- move to the last line of the textbox
void TextBox::End()
{
    wtop = wlines-ClientHeight();
    if (wtop < 0)
        wtop = 0;
    Paint();
}

// ----- position the scroll boxes
void TextBox::SetScrollBoxes()
{
    if (vscrollbar != 0)
        vscrollbar->TextPosition(wlines ? (wtop*100)/wlines : 0);
    if (hscrollbar != 0)
        hscrollbar->TextPosition(textwidth ? (wleft*100)/textwidth : 0);
}

// ---- compute the horizontal page position
void TextBox::HorizontalPagePosition(int pct)
{
    wleft = (textwidth * pct) / 100;
    Paint();
}

// ---- compute the vertical page position
void TextBox::VerticalPagePosition(int pct)
{
    wtop = (wlines * pct) / 100;
    Paint();
}

void TextBox::SetScrollBars()
{
	if (LineCount() > ClientHeight())
		SetAttribute(VSCROLLBAR);
	else	{
		ClearAttribute(VSCROLLBAR);
		delete vscrollbar;
		vscrollbar = 0;
	}
	if (TextWidth() > ClientWidth())
		SetAttribute(HSCROLLBAR);
	else	{
		ClearAttribute(HSCROLLBAR);
		delete hscrollbar;
		hscrollbar = 0;
	}
}

// ---- determine the length of a line of text
int TextBox::LineLength(int lno)
{
	const char *tp = TextLine(lno);
	int len = 0;
	while( *(tp + len) && *(tp + len) != '\n' )
	{
    	len++;
    }
	return len;
}


const char *TextBox::TextLine( int line )
{
    if( line > LineCount() )
    {
        line = LineCount();
    }
    return( (const char *)(*text) + *(TextPointers+line) );
}
