// ---------- directry.cpp
// 
// removed direct BIOS calls
// - to be replaced with native OS/2 code.
// 

#define INCL_NOPMAPI
#include <os2.h>
#include <direct.h>

#include "directry.h"

const char *PathNameLabel::CurrentPath()
    {
    static char path[CCHMAXPATH];
    _getdcwd(0, path, CCHMAXPATH);
    return path;
    }

void PathNameLabel::FillLabel()
    {
    SetText(CurrentPath());
    }


DriveListBox::DriveListBox(int lf, int tp, DFWindow *par)
                        : ListBox(lf, tp, 10, 10, par)
    {
    SetAttribute(BORDER);
    currdrive = getdisk();
    for (unsigned int dr = 0; dr < 26; dr++)    
        {
        setdisk(dr);
        if (getdisk() == dr)    {
            // ----- test for remapped B drive removed - jw
            
            String drname(" :");
            drname[0] = dr+'A';

            // ---- test for network or RAM disk removed - jw

            AddText(drname);
            }
        }
    setdisk(currdrive);
    SetScrollBars();
    }


DirectoryListBox::DirectoryListBox(int lf, int tp, DFWindow *par)
                        : ListBox(lf, tp, 10, 13, par)
    {
    SetAttribute(BORDER);
    FillList();
    }

void DirectoryListBox::FillList()
    {
    ClearText();
    int ax;
    struct ffblk ff;
    ax = findfirst("*.*", &ff, FA_DIREC);
    while (ax == 0)    
        {
        if ((ff.ff_attrib & FA_DIREC) != 0)    
            {
            if (strcmp(ff.ff_name, "."))    
                {
                String fname("[");
                fname += ff.ff_name;
                fname += "]";
                AddText(fname);
                }
            }
        ax = findnext(&ff);
        }
    SetScrollBars();
    }

FileNameListBox::FileNameListBox(const char *filespec,int lf,int tp, DFWindow *par)
                        : ListBox(lf, tp, 10, 14, par)
    {
    SetAttribute(BORDER);
    FillList(filespec);
    }

void FileNameListBox::FillList(const char *filespec)
    {
    ClearText();
    int ax;
    struct ffblk ff;
    ax = findfirst(*filespec ? filespec : "*.*", &ff, 0);
    while (ax == 0)    
        {
        AddText(ff.ff_name);
        ax = findnext(&ff);
        }
    SetScrollBars();
    }


