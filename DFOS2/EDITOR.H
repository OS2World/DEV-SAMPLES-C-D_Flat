// -------- editor.h 

#ifndef EDITOR_H
#define EDITOR_H

#include "editbox.h"

const unsigned char Ptab = '\t'+0x80; // pseudo tab expansion

class Editor : public EditBox	{
	void OpenWindow();
	int tabs;
	Bool wordwrapmode;
protected:
    int row;       // Current row
	virtual void Upward();
	virtual void Downward();
	virtual void Forward();
	virtual void Backward();
	virtual void DeleteCharacter();
	virtual void BeginDocument();
	virtual void EndDocument();
    virtual void PageUp();
    virtual void PageDown();
	virtual void InsertCharacter(int key);
	virtual void AdjustCursorTabs(int key = 0);
	void InsertTab();
	void AdjustTabInsert();
	void AdjustTabDelete();
	void WordWrap();
public:
	Editor(const char *ttl, int lf, int tp, int ht, int wd, DFWindow *par=0)
						: EditBox(ttl, lf, tp, ht, wd, par)
			{ OpenWindow(); }
	Editor(const char *ttl, int ht, int wd, DFWindow *par=0)
						: EditBox(ttl, ht, wd, par)
			{ OpenWindow(); }
	Editor(int lf, int tp, int ht, int wd, DFWindow *par=0)
						: EditBox(lf, tp, ht, wd, par)
			{ OpenWindow(); }
	Editor(int ht, int wd, DFWindow *par=0) : EditBox(ht, wd, par)
			{ OpenWindow(); }
	Editor(const char *ttl) : EditBox(ttl)
			{ OpenWindow(); }
	virtual void Keyboard(int key);
	virtual void ResetCursor();
	virtual unsigned char CurrentChar()
		{ return *(TextLine(row) + column); }
	virtual unsigned CurrentCharPosition()
		{ return (unsigned) ((const char *)(TextLine(row)+column) - (const char *) *text); }
	virtual Bool AtBufferStart()
		{ return (Bool) (column == 0 && row == 0); }
	virtual void PaintCurrentLine()
		{ WriteTextLine(row, colors.fg, colors.bg); }
	virtual void FormParagraph();
	int Tabs()
		{ return tabs; }
	void SetTabs(int t)
		{ tabs = t; }
	Bool WordWrapMode()
		{ return wordwrapmode; }
	void SetWordWrapMode(Bool wmode)
		{ wordwrapmode = wmode; }
    virtual void AddText(const char *txt);
	virtual const String GetText();
    virtual void WriteString(String &ln, int x, int y, int fg, int bg);
    virtual void LeftButton(int mx, int my);
	virtual void ClearText();
};


#endif
