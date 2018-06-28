// -------- dflatdef.h

#ifndef DFLATDEF_H
#define DFLATDEF_H

// -------- window class types
enum WndType {
    DFlatWindow,
    ApplicationWindow,
    TextboxWindow,
	LabelWindow,
    FrameWindow,
    ScrollbarWindow,
    MenubarWindow,
    ListboxWindow,
    PopdownWindow,
    EditboxWindow,
    EditorWindow,
    DialogWindow,
    PushButtonWindow,
    RadioButtonWindow,
	CheckBoxWindow,
    ToolButtonWindow,
    ToolbarWindow,
    StatusbarWindow
};

enum Bool {False, True};

inline int max(int a, int b) { return a > b ? a : b; }
inline int min(int a, int b) { return a < b ? a : b; }


#endif

