// ------------- scrolbar.cpp

#include "desktop.h"
#include "textbox.h"
#include "scrolbar.h"

ScrollBar::ScrollBar(BarPlane Plane, TextBox *par)
						: DFWindow(1, 1, par)
{
	windowtype = ScrollbarWindow;
	if (par == 0)
		return;
	scrollbox = 1;
	sliding = False;
	SetClearChar(SCROLLBARCHAR);
	plane = Plane;
	if (plane == HORIZONTAL)	{
		rect.Left() = par->Left()+1;
		rect.Right() = par->Right()-1;
		rect.Top() = rect.Bottom() = par->Bottom();
	}
	else	{
		rect.Left() = rect.Right() = par->Right();
		rect.Top() = par->Top()+1;
		rect.Bottom() = par->Bottom()-1;
	}
	colors.fg = par ? par->FrameFG() : 0;
	colors.bg = par ? par->FrameBG() : 0;
}

Bool ScrollBar::SetFocus()
{
	if (Parent() != 0)
		Parent()->SetFocus();
	return True;
}

void ScrollBar::Paint()
{
	if (visible)	{
		int fg = colors.fg;
		int bg = colors.bg;
		DFWindow::Paint();
		if (plane == HORIZONTAL)	{
			WriteWindowChar(LEFTSCROLLBOX, 0, 0, fg, bg);
			WriteWindowChar(RIGHTSCROLLBOX, Width()-1, 0, fg, bg);
			WriteWindowChar(SCROLLBOXCHAR, scrollbox, 0, fg, bg);
		}
		else	{
			WriteWindowChar(UPSCROLLBOX, 0, 0, fg, bg);
			WriteWindowChar(DOWNSCROLLBOX, 0, Height()-1, fg, bg);
			WriteWindowChar(SCROLLBOXCHAR, 0, scrollbox, fg, bg);
		}
	}
}

void ScrollBar::LeftButton(int mx, int my)
{
	if (Parent() != 0 && !sliding)	{
		TextBox &Par = *(TextBox *)Parent();
		if (plane == VERTICAL)	{
			// -- test for hitting the vertical scroll buttons
			if (my == rect.Top())
				Par.ScrollDown();
			else if (my == rect.Bottom())
				Par.ScrollUp();
			// ------- test for hitting the vertical scroll box
			else if (my-rect.Top() == scrollbox)	{
				sliding = True;
				desktop.mouse().SetTravel(rect.Left(), rect.Right(),
								rect.Top()+1, rect.Bottom()-1);
			}
			else	{
				// ----- hit in the scroll bar
				if (my-rect.Top() < scrollbox)
					Par.PageUp();
				else
					Par.PageDown();
			}
		}
		else	{
			// -- test for hitting the horizontal scroll buttons
			if (mx == rect.Left())
				Par.ScrollRight();
			else if (mx == rect.Right())
				Par.ScrollLeft();
			// ------- test for hitting the horizontal scroll box
			else if (mx-rect.Left() == scrollbox)	{
				sliding = True;
				desktop.mouse().SetTravel(rect.Left()+1, rect.Right()-1,
								rect.Top(), rect.Bottom());
			}
			else	{
				// ----- hit in the scroll bar
				if (mx-rect.Left() < scrollbox)
					Par.PageLeft();
				else
					Par.PageRight();
			}
		}
	}
}

void ScrollBar::MouseMoved(int mx, int my)
{
	if (sliding)	{
		if (plane == VERTICAL)
			MoveScrollBox(my - rect.Top());
		else
			MoveScrollBox(mx - rect.Left());
	}
}

void ScrollBar::ButtonReleased(int, int)
{
	if (sliding && Parent() != 0)	{
		TextBox &Par = *(TextBox *)Parent();
		desktop.mouse().SetTravel(0, desktop.screen().Width()-1, 0, desktop.screen().Height()-1);
		int pct = (scrollbox-1)*100;
		if (plane == VERTICAL)	{
			pct /= (rect.Height()-2);
			Par.VerticalPagePosition(pct);
		}
		else	{
			pct /= (rect.Width()-2);
			Par.HorizontalPagePosition(pct);
		}
	}
	sliding = False;
}

void ScrollBar::ParentSized(int xdif, int ydif)
{
	if (plane == HORIZONTAL)	{
		Size(Right()+xdif, Top());
		Move(Left(), Bottom()+ydif);
	}
	else	{
		Size(Left(), Bottom()+ydif);
		Move(Right()+xdif, Top());
	}
}

void ScrollBar::TextPosition(int TxPct)
{
	int len = plane == HORIZONTAL ? Width()-2 : Height()-2;
	int sb = 1 + ((len * TxPct) / 100);
	MoveScrollBox(sb);
}

void ScrollBar::MoveScrollBox(int sb)
{
	if (sb != scrollbox)	{
		int fg = colors.fg;
		int bg = colors.bg;
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		if (plane == HORIZONTAL)
			x1 = sb, x2 = scrollbox;
		else
			y1 = sb, y2 = scrollbox;
		WriteWindowChar(SCROLLBOXCHAR, x1, y1, fg, bg);
		WriteWindowChar(SCROLLBARCHAR, x2, y2, fg, bg);
		scrollbox = sb;
	}
}

