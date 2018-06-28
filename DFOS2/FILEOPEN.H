// --------- fileopen.h

#ifndef FILEOPEN_H
#define FILEOPEN_H

#include "dflatpp.h"
#include "directry.h"

// ------------ File Open dialog box
class FileOpen : public Dialog    {
    // -----File Open Dialog Box Controls:
protected:
    // ----- file name editbox
    Label filelabel;
    EditBox filename;
    // ----- drive:path display
    PathNameLabel dirlabel;
    // ----- files list box
    Label fileslabel;
    FileNameListBox files;
    // ----- directories list box
    Label dirslabel;
    DirectoryListBox dirs;
    // ----- drives list box
    Label disklabel;
    DriveListBox disks;
    // ----- command buttons
    OKButton ok;
    CancelButton cancel;
    HelpButton help;
    // ------ file open data members
    String filespec;
    // ------ private member functions
    void SelectFileName();
    void ShowLists();
    // --- functions inherited from DFWindow
    virtual void ControlSelected(DFWindow *Wnd);
    virtual void ControlChosen(DFWindow *Wnd);
    virtual void EnterFocus(DFWindow *Wnd);
    virtual void OKFunction();
public:
    FileOpen(const char *spec = "*.*", const char *ttl = "File Open");
    const String& FileName() { return filespec; }
};

class SaveAs : public FileOpen    {
    virtual void OKFunction();
public:
    SaveAs(const char *spec = "", const char *ttl = "Save As")
        : FileOpen(spec, ttl) { }
};

#endif

