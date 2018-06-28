// --------- ted.h

#ifndef TED_H
#define TED_H

#include "dflatpp.h"
#include "fileopen.h"
#include "tedtools.h"

#define Df void (DFWindow::*)()
#define Ap void (Application::*)()

extern MenuBarItem TedMenu[];
extern MenuSelection InsertCmd, WordWrapCmd;

// ------- Ted application definition
class Ted : public Application {
    MenuBar menubar;
    TedTools toolbar;
    String  fname;
    Editor editor;
protected:
    virtual void Move(int x, int y);
    virtual void Size(int x, int y);
    void BuildTitle();
    void TestChanged();
public:
    Ted();
	void OpenFile(String fn);
    // ----- menu command functions
    void CmNew();
    void CmOpen();
    void CmSave();
    void CmSaveAs();
    void CmInsert();
    void CmCut()    { /* not implemented */ }
    void CmCopy()   { /* not implemented */ }
    void CmPaste()  { /* not implemented */ }
    void CmPara();
    void CmExit()   { CloseWindow(); }
    virtual void CloseWindow();
};

#endif
