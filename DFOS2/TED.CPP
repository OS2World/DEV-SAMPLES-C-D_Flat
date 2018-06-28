// ------------- ted.cpp

#include <fstream.h>
#include "ted.h"

static char untitled[] = "(untitled)";

main(int argc, char *argv[])
{
    Ted ma;
	if (argc > 1)
		ma.OpenFile(argv[1]);
    ma.Execute();
    return 0;
}

// ------- editor colors
static Color col = {
	BLACK,				// fg
	LIGHTGRAY,			// bg
	LIGHTGRAY,			// selected fg
	BLACK,				// selected bg
	LIGHTGRAY,			// frame bg
	BLUE,				// frame fg
	LIGHTGRAY,			// highlighted fg
	BLUE				// highlighted bg
};

// ---- construct application
Ted::Ted() :  menubar(TedMenu, this),
              toolbar(this),
              editor(ClientLeft(),
                     ClientTop(),
                     ClientHeight(),
                     ClientWidth(),
                     this),
              fname(untitled)
                 
{
    SetAttribute(SIZEABLE | MOVEABLE);
	editor.SetAttribute(VSCROLLBAR | BORDER);
	editor.SetColor(col);
    SetClearChar(' ');
    BuildTitle();
    Show();
    editor.SetFocus();
}

// ---- builds the title with the current document name
void Ted::BuildTitle()
{
    SetTitle(String("TED: ") + fname);
    Title();
}

// ---- File/New Menu Command
void Ted::CmNew()
{
    TestChanged();
    editor.ClearText();
	fname = String(untitled);
	BuildTitle();
    editor.Paint();
}

// ---- Open and load a specified file
void Ted::OpenFile(String fn)
{
    editor.ClearText();
    fname = fn;
    BuildTitle();
    editor.ClearChanged();

    ifstream tfile(fname);
	if (tfile)	{
		// --- first get the file size
		tfile.seekg(0, ios::end);
		long sz = tfile.tellg();
		tfile.seekg(0, ios::beg);
		// --- buffer to read file into
    	char *ip = new char[sz+1];
		memset(ip, 0, sz+1);
		// --- read the file
		tfile.read(ip, sz);
		// --- put the text into the editor window
		editor.SetText(ip);
		// --- not needed any more
		delete [] ip;
    	editor.Paint();
	}
}

// ---- File/Open Menu Command
void Ted::CmOpen()
{
    TestChanged();
    FileOpen fo("*.txt");
    fo.Execute();
    if (fo.OKExit())
		OpenFile(fo.FileName());
}

// ---- File/Save Menu Command
void Ted::CmSave()
{
    if (fname == String(untitled))
        CmSaveAs();
    if (fname != String(untitled))    {
        editor.ClearChanged();
        ofstream tfile(fname);
        tfile.write(editor.GetText(), editor.TextLength());
    }
}

// ---- File/Save As Menu Command
void Ted::CmSaveAs()
{
    SaveAs sa;
    sa.Execute();
    if (sa.OKExit())    {
        fname = String(sa.FileName());
        BuildTitle();
        CmSave();
    }
}

// ---- Edit/Paragraph Menu Command
void Ted::CmPara()
{
	editor.FormParagraph();
}

// ---- Options/Insert Menu Command
void Ted::CmInsert()
{
    editor.SetInsertMode(InsertCmd.isToggled());
}

// ----- reset the editor focus when the application moves
void Ted::Move(int x, int y)
{
    Application::Move(x, y);
    editor.SetFocus();
}

// ----- resize the editor when the application resizes
void Ted::Size(int x, int y)
{
    editor.Hide();
    editor.Size(editor.Right()+(x-Right()),
                editor.Bottom()+(y-Bottom()));
    Application::Size(x, y);
    editor.SetFocus();
}

// ---- test for changes to the document before discarding
void Ted::TestChanged()
{
    if (editor.Changed())    {
        String msg(fname + " has changed. Save?");
        if (YesNo(msg))
            CmSave();
    }
}

// ---- test for changes before closing
void Ted::CloseWindow()
{
    TestChanged();
    Application::CloseWindow();
}

