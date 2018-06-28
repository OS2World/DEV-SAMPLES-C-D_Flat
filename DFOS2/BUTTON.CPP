// ------------- button.cpp

#include "button.h"
#include "desktop.h"

Button::Button(const char *lbl, int lf, int tp, DFWindow *par)
                : TextBox(lf, tp, 1, strlen(lbl)+5, par)
{
    String lb("( ) ");
    lb += lbl;
    lb += " ";
    SetText(lb);
    setting = False;
    setchar = ' ';
    colors.fg =
    colors.sfg =
    colors.ffg =
    colors.hfg = par ? par->ClientFG() : 0; 
    colors.bg =
    colors.sbg =
    colors.fbg =
    colors.hbg = par ? par->ClientBG() : 0;
    shortcutfg = RED;
}

void Button::Paint()
{
    if (visible)    {
        (*text)[1] = setting ? setchar : ' ';
        if (isEnabled())
            WriteShortcutLine(0, colors.fg, colors.bg);
        else 
            WriteTextLine(0, colors.hfg, colors.hbg);
    }
}

Bool Button::SetFocus()
{
    TextBox::SetFocus();
    desktop.cursor().NormalCursor();
    desktop.cursor().SetPosition(Left()+1, Top());
    desktop.cursor().Show();
    return True;
}

void Button::ResetFocus()
{
    TextBox::ResetFocus();
    desktop.cursor().Hide();
}

void Button::Keyboard(int key)
{
    if (key == ' ')
        InvertButton();
    else 
        TextBox::Keyboard(key);
}

void Button::LeftButton(int mx, int my)
{
    if (ClientRect().Inside(mx,my))
        InvertButton();
}

void Button::InvertButton()
{
    if (setting)
        ReleaseButton();
    else
        PushButton();
}

void Button::PushButton()
{
    setting = True;
    Paint();
}

void Button::ReleaseButton()
{
    setting = False;
    Paint();
}






