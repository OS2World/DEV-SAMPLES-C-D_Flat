// ---------- tedmenu.cpp

#include "dflatpp.h"
#include "ted.h"

// --------- MenuSelection objects
MenuSelection
    NewCmd      ("~New",           (Ap) &Ted::CmNew ),
    OpenCmd     ("~Open...",       (Ap) &Ted::CmOpen ),
    SaveCmd     ("~Save",          (Ap) &Ted::CmSave ),
    SaveAsCmd   ("Save ~as...",    (Ap) &Ted::CmSaveAs ),
    ExitCmd     ("E~xit      Alt+F4", (Ap) &Ted::CmExit,  ALT_F4 ),
    CutCmd      ("~Cut       Ctrl+X", (Ap) &Ted::CmCut,   CTRL_X ),
    CopyCmd     ("C~opy      Ctrl+C", (Ap) &Ted::CmCopy,  CTRL_C ),
    PasteCmd    ("~Paste     Ctrl+V", (Ap) &Ted::CmPaste, CTRL_V ),
	ParaCmd     ("P~aragraph Alt+P",  (Ap) &Ted::CmPara,  ALT_P  ),
    InsertCmd   ("~Insert Ins",  (Ap) &Ted::CmInsert, On, INS ),
    WordWrapCmd ("~Word wrap",     On );

// --------- File menu definition
MenuSelection *File[] = {
    &NewCmd,
    &OpenCmd,
    &SaveCmd,
    &SaveAsCmd,
    &SelectionSeparator,
    &ExitCmd,
    0
};

MenuSelection *Edit[] = {
    &CutCmd,
    &CopyCmd,
    &PasteCmd,
    &SelectionSeparator,
	&ParaCmd,
    0
};

MenuSelection *Options[] = {
    &InsertCmd,
    &WordWrapCmd,
    0
};

// --------- menu bar definition
MenuBarItem TedMenu[] = {
    MenuBarItem( "~File",    File    ),
    MenuBarItem( "~Edit",    Edit    ),
    MenuBarItem( "~Options", Options ),
    MenuBarItem( 0 )
};
