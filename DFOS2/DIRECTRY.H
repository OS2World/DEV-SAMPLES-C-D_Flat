// ---------- directry.h

#ifndef DRIVE_H
#define DRIVE_H

#include <dir.h>
#include <dos.h>
#include "listbox.h"
#include "label.h"

class PathNameLabel : public Label {
    const char *CurrentPath();
public:
    PathNameLabel(int x, int y, int wd, DFWindow *par = 0) :
        Label(CurrentPath(), x, y, wd, par) {}
    void FillLabel();
};

class DriveListBox : public ListBox    {
    unsigned currdrive;
public:
    DriveListBox(int lf, int tp, DFWindow *par = 0);
};

class DirectoryListBox : public ListBox    {
public:
    DirectoryListBox(int lf, int tp, DFWindow *par = 0);
    void FillList();
};

class FileNameListBox : public ListBox    {
public:
    FileNameListBox(const char *filespec, int lf, int tp, DFWindow *par = 0);
    void FillList(const char *filespec);
};

#endif
