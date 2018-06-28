// ------------- frame.cpp

#include "desktop.h"
#include "frame.h"

static Color col = {
	GREEN,				// fg
	LIGHTGRAY,			// bg
	GREEN,				// selected fg
	LIGHTGRAY,			// selected bg
	GREEN,				// frame fg
	LIGHTGRAY,			// frame bg
	GREEN,				// highlighted fg
	LIGHTGRAY			// highlighted bg
};

Frame::Frame() : DFWindow(10, 10)
{
	windowtype = FrameWindow;
	SetAttribute(BORDER | SAVESELF);
	DblBorder = False;
	moving = False;
	Host = 0;
	colors = col;
}

void Frame::OpenFrame(DFWindow *Wnd, Bool Moving, int mx)
{
	moving = Moving;
	Host = Wnd;
	Move(Wnd->Left(), Wnd->Top());
	Size(Wnd->Right(), Wnd->Bottom());
	diff = moving ? mx-Left() : 0;
	CaptureFocus();
	minx = 0;
	miny = 0;
	maxx = desktop.screen().Width()-1;
	maxy = desktop.screen().Height()-1;

	if (!moving)	{
		minx = Wnd->Left()+10;
		miny = Wnd->Top()+10;
	}
	desktop.mouse().SetTravel(minx+diff, maxx, miny, maxy);
}

void Frame::CloseFrame(Bool doOp)
{
	desktop.mouse().SetTravel(0, desktop.screen().Width()-1, 0, desktop.screen().Height()-1);
	Hide();
	ReleaseFocus();
	if (doOp)	{
		if (moving)	{
			if (Host->Left() != Left() || Host->Top() != Top())
				Host->Move(Left(), Top());
		}
		else if (Host->Right() != Right() || Host->Bottom() != Bottom())
			Host->Size(Right(), Bottom());
	}
	Host = 0;
}

// ---------- display the frame
void Frame::Show()
{
	if (!visible)	{
		visible = True;
		videosave = new char[rect.Height() * rect.Width() * 2];
		desktop.screen().GetBuffer(rect, videosave);
		Border();
	}
}

// ----------- hide the frame
void Frame::Hide()
{
	if (visible)	{
		visible = False;
		desktop.screen().PutBuffer(rect, videosave);
		delete videosave;
		videosave = 0;
	}
}

void Frame::LeftButton(int, int)
{
	// --- intercept the left button
}

void Frame::ButtonReleased(int, int)
{
	CloseFrame(True);
}

void Frame::MouseMoved(int mx, int my)
{
	if (mx != prevmousecol || my != prevmouseline)	{
		if (moving)
			Move(mx-diff, my);
		else
			Size(mx, my);
	}
}

void Frame::Keyboard(int key)
{
	int lf = Left();
	int tp = Top();
	int rt = Right();
	int bt = Bottom();
	switch (key)	{
		case ESC:
			CloseFrame(False);
			break;
		case '\r':
			CloseFrame(True);
			break;
		case FWD:
			if (moving)	{
				if (lf < maxx)
					Move(lf+1, tp);
			}
			else
				if (rt < maxx)
					Size(rt+1, bt);
			break;
		case BS:
			if (moving)	{
				if (lf > minx)
					Move(lf-1, tp);
			}
			else if (rt > minx)
				Size(rt-1, bt);
			break;
		case UP:
			if (moving)	{
				if (tp > miny)
					Move(lf, tp-1);
			}
			else if (bt > miny)
				Size(rt, bt-1);
			break;
		case DN:
			if (moving)	{
				if (tp < maxy)
					Move(lf, tp+1);
			}
			else if (bt < maxy)
				Size(rt, bt+1);
			break;
		default:
			break;
	}
}


