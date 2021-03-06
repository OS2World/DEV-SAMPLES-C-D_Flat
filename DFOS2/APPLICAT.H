// -------------- applicat.h

#ifndef APPLICAT_H
#define APPLICAT_H

#include "toolbar.h"
#include "menubar.h"
#include "statbar.h"

const int ClearChar = 176;

class Application : public DFWindow    {
    MenuBar *menubar;            // points to menu bar
    ToolBar *toolbar;            // points to tool bar
    StatusBar *statusbar;        // points to status bar
    Bool takingfocus;            // true while taking focus
    void OpenWindow();
protected:
    // ------------- client window coordinate adjustments
    virtual void AdjustBorders();
public:
    Application(const char *ttl, int lf, int tp, int ht, int wd)
                : DFWindow(ttl, lf, tp, ht, wd, 0)
            { OpenWindow(); }
    Application(const char *ttl, int ht, int wd) : DFWindow(ttl, ht, wd, 0)
            { OpenWindow(); }
    Application(int lf, int tp, int ht, int wd) : DFWindow(lf, tp, ht, wd, 0)
            { OpenWindow(); }
    Application(int ht, int wd) : DFWindow(ht, wd, 0)
            { OpenWindow(); }
    Application(const char *ttl = 0) : DFWindow(ttl)
            { OpenWindow(); }
    // -------- API messages
    virtual void CloseWindow();
    virtual Bool SetFocus();
    virtual void Show();
    virtual void Keyboard(int key);
    virtual void ClockTick();
    void StatusMessage(String& Msg);
	void Execute();
};

#endif


