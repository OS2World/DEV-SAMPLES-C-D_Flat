// ---------- fileopen.cpp

#include <io.h>
#include <dir.h>
#include "fileopen.h"
#include "notice.h"

// ----------------- File Open Dialog Box
FileOpen::FileOpen(const char *spec, const char *ttl) :
        Dialog(ttl, 19, 57),
        // ----- file name editbox
        filelabel ("~Filename:", 3, 2, 10, this),
        filename  (13, 2, 1, 40, this),
        // ----- drive:path display
        dirlabel  (3, 4, 50, this),
        // ----- files list box
        fileslabel("F~iles:", 3, 6, 7, this),
        files     (spec, 3, 7, this),
        // ----- directories list box
        dirslabel ("~Directories:", 19, 6, 13, this),
        dirs      (19, 7, this),
        // ----- drives list box
        disklabel ("Dri~ves:", 34, 6, 8, this),
        disks     (34, 7, this),
        // ----- command buttons
        ok        (46, 8, this),
        cancel    (46,11, this),
        help      (46,14, this),
        // ------ file open data members
        filespec(spec)
{
    filename.AddText(filespec);
}

// --- Get selected filename: files listbox->filename editbox
void FileOpen::SelectFileName()
{
    int sel = files.Selection();
    if (sel != -1)    {
        String fname = files.ExtractTextLine(sel);
        filename.SetText(fname);
        filename.Paint();
    }
}

// ---- called when user "selects" a control
//      e.g. changes the selection on a listbox
void FileOpen::ControlSelected(DFWindow *Wnd)
{
    if (Wnd == (DFWindow *) &files)
        // --- user selected a filename from list
        SelectFileName();
    else if (Wnd == (DFWindow *) &dirs ||
            Wnd == (DFWindow *) &disks)    {
        // --- user is selecting a different drive or directory
        filename.SetText(filespec);
        filename.Paint();
    }
}

// ---- called when user "chooses" a control
//      e.g. chooses the current selection on a listbox
void FileOpen::ControlChosen(DFWindow *Wnd)
{
    if (Wnd == (DFWindow *) &files)
        // --- user chose a filename from filename list
        OKFunction();
    else if (Wnd == (DFWindow *) &dirs)    {
        // --- user chose a directory from directory list
        int dr = dirs.Selection();
        String dir = dirs.ExtractTextLine(dr);
        int len = dir.Strlen();
        String direc = dir.mid(len-2,1);
        chdir(direc);
        ShowLists();
    }
    else if (Wnd == (DFWindow *) &disks)    {
        // --- user chose a drive from drive list
        int dr = disks.Selection();
        String drive = disks.ExtractTextLine(dr);
        setdisk(drive[0] - 'A');
        ShowLists();
    }
}

// ---- called when user chooses OK command
void FileOpen::OKFunction()
{
    String fname = filename.ExtractTextLine(0);
    if (access(fname, 0) == 0)    {
        filespec = fname;
        Dialog::OKFunction();
    }
    else if (fname.FindChar('*') != -1 ||
            fname.FindChar('?') != -1)    {
        filespec = fname;
        ShowLists();
    }
    else
        // ---- No file as specified
        ErrorMessage("File does not exist");
}

// ------ refresh the current directory display and 
//        the directories and files list after
//        user changes filespec, drive, or directory
void FileOpen::ShowLists()
{
    dirlabel.FillLabel();
    dirlabel.Show();
    dirs.FillList();
    dirs.Show();
    files.FillList(filespec);
    files.Show();
}

// ------- called just before a control gets the focus
void FileOpen::EnterFocus(DFWindow *Wnd)
{
    if (Wnd == (DFWindow *) &files)
        // --- The file name list box is getting the focus
        SelectFileName();
}

// ---- called when user chooses OK command
void SaveAs::OKFunction()
{
    String fname = filename.ExtractTextLine(0);
    if (access(fname, 0) != 0)    {
        // ---- chosen file does not exist
        if (fname.FindChar('*') != -1 ||
            fname.FindChar('?') != -1)    {
            // --- wild cards
            filespec = fname;
            ShowLists();
        }
        else    {
            filespec = fname;
            Dialog::OKFunction();
        }
    }
    else    {
        // ---- file exists
        String msg = fname + " already exists. Replace?";
        if (YesNo(msg))    {
            filespec = fname;
            Dialog::OKFunction();
        }
    }
}
