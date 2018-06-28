// ---------- toolbutt.cpp

#include "toolbar.h"
#include "desktop.h"

// ------- various color patterns
static Color EnabledColor = {
    LIGHTGRAY,            // fg
    ToolBarBG,            // bg
    WHITE,                // selected fg
    ToolBarBG,            // selected bg
    CYAN,                 // frame fg
    ToolBarBG,            // frame bg
    WHITE,                // highlighted fg
    ToolBarBG             // highlighted bg
};

static Color PressedColor = {
    BLACK,                // fg
    ToolBarBG,            // bg
    ToolBarBG,            // selected fg
    ToolBarBG,            // selected bg
    CYAN,                 // frame fg
    ToolBarBG,            // frame bg
    ToolBarBG,            // highlighted fg
    ToolBarBG             // highlighted bg
};

static Color DisabledColor = {
    BLACK,                // fg
    ToolBarBG,            // bg
    BLACK,                // selected fg
    ToolBarBG,            // selected bg
    BLACK,                // frame fg
    ToolBarBG,            // frame bg
    BLACK,                // highlighted fg
    ToolBarBG             // highlighted bg
};

// ---- pressed and unpressed frame corner characters
const int PRESSED_NE   = 184;
const int PRESSED_SW   = 211;
const int UNPRESSED_NE = 183;
const int UNPRESSED_SW = 212;

// ----- button frame when pressed
static BoxLines PressedBorder = {
    FOCUS_NW,
    FOCUS_LINE,
    PRESSED_NE,
    SIDE,
    SE,
    LINE,
    PRESSED_SW,
    FOCUS_SIDE,
};

// ----- button frame when not pressed
static BoxLines UnPressedBorder = {
    NW,
    LINE,
    UNPRESSED_NE,
    FOCUS_SIDE,
    FOCUS_SE,
    FOCUS_LINE,
    UNPRESSED_SW,
    SIDE
};

// --- common constructor code
void ToolButton::InitWindow(const char *lbl)
{
    oldFocus = 0;
    windowtype = ToolButtonWindow;
    Size(Left()+5, Top()+2);
    ClearAttribute(SHADOW);
    SetAttribute(BORDER);
    SetText(lbl);
    SetColors();
}

// --------- construct a Toolbar button specifying position
ToolButton::ToolButton(const char *lbl, int lf, int tp, DFWindow *par)
                : PushButton(lbl, lf, tp, par)
{
    InitWindow(lbl);
}

// ------ construct a Toolbar button, self-positioning
ToolButton::ToolButton(const char *lbl, DFWindow *par)
                : PushButton(lbl, 0, 0, par)
{
    InitWindow(lbl);
    if (par != 0 && par->WindowType() == ToolbarWindow)    {
        int btcount = 0;
        DFWindow *Wnd = par->First();
        while (Wnd != 0 && Wnd != this)    {
            if (Wnd->WindowType() == ToolButtonWindow)
                btcount++;
            Wnd = Wnd->Next();
        }
        Move(par->Left()+btcount*6, par->Top());
    }
}

// ----- draw the button's frame
void ToolButton::Border()
{
    if (visible)    {
        if (pressed)    {
            colors = PressedColor;
            DrawBorder(PressedBorder);
        }
        else    {
            SetColors();
            DrawBorder(UnPressedBorder);
        }
    }
}

// -------- set the button's colors
void ToolButton::SetColors()
{
    if (isEnabled())
        colors = EnabledColor;
    else
        colors = DisabledColor;
}

// ------- paint the button
void ToolButton::Paint()
{
    SetColors();
    TextBox::Paint();
}

// -------- the button was pressed
void ToolButton::ButtonCommand()
{
    PushButton::ButtonCommand();
    if (oldFocus != 0)
        oldFocus->SetFocus();
	oldFocus = 0;
}

// ---- remember who had the focus before the button got it
Bool ToolButton::SetFocus()
{
    if (oldFocus == 0)
        oldFocus = desktop.InFocus();
    return PushButton::SetFocus();
}

