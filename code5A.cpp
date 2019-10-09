#include "code5A.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode5A* pFrame = new CCode5A;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode5A, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CCode5A::CCode5A()
{
	Create(NULL, (LPCTSTR)"Mathematical Curves", WS_OVERLAPPEDWINDOW, CRect(20, 30, 600, 450));
	TopLeft = CPoint(130, 2);
	BottomRight = CPoint(550, 380);
	Origin.x = (TopLeft.x + BottomRight.x) / 2;
	Origin.y = (TopLeft.y + BottomRight.y) / 2;
	DrawRc = CRect(TopLeft.x + 3, TopLeft.y + 3, BottomRight.x - 3, BottomRight.y - 3);
	pt1 = CPoint(20, 70);
	rc1 = CRect(pt1, pt1 + CPoint(100, 25));
	pt2 = CPoint(20, 120);
	rc2 = CRect(pt2, pt2 + CPoint(100, 25));
	pt3 = CPoint(20, 170);
	rc3 = CRect(pt3, pt3 + CPoint(100, 25));
	Color1 = RGB(100, 100, 100);
	Color2 = RGB(170, 170, 170);
}

void CCode5A::OnPaint()
{
	CPaintDC dc(this);
	CPen penDark(PS_SOLID, 2, Color2);
	dc.SelectObject(&penDark);
	dc.Rectangle(CRect(CPoint(TopLeft), CPoint(BottomRight)));
	dc.FillSolidRect(&rc1, Color1);
	dc.FillSolidRect(&rc2, Color1);
	dc.FillSolidRect(&rc3, Color1);
	dc.SetTextColor(RGB(255, 255, 255));
	dc.SetBkColor(Color1);
	dc.TextOutW(pt1.x + 10, pt1.y + 5, "Polynomial");
	dc.TextOutW(pt2.x + 10, pt2.y + 5, "Lemniscate");
	dc.TextOutW(pt3.x + 10, pt3.y + 5, "Creative Net");
	switch (MenuChoice) {
	case 'P':
		CCode5A::PolyNomial(); break;
	case 'L':
		CCode5A::Lemniscate(); break;
	case 'C':
		CCode5A::CreativeNet(); break;
	}
}

void CCode5A::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if (rc1.PtInRect(pt))
	{
		MenuChoice = 'P';
		InvalidateRect(DrawRc);
	}
	if (rc1.PtInRect(pt))
	{
		MenuChoice = 'L';
		InvalidateRect(DrawRc);
	}
	if (rc1.PtInRect(pt))
	{
		MenuChoice = 'C';
		InvalidateRect(DrawRc);
	}
	if (!rc1.PtInRect(pt) && !rc2.PtInRect(pt) && !rc3.PtInRect(pt))
		MenuChoice = 'X';
}

void CCode5A::DrawAxes()
{
	CClientDC dc(this);
	CPen penGray(PS_SOLID, 2, Color2);
	dc.SelectObject(&penGray);
	dc.MoveTo(Origin.x, TopLeft.y + 10);
	dc.MoveTo(TopLeft.x + 10, Origin.y);
	dc.LineTo(BottomRight.x - 10, Origin.y);
}

void CCode5A::PolyNomial()
{
	CClientDC  dc(this);
	double X, Y;
	CPoint pt;
	DrawAxes();

	//draw the curve, where x is magnified 50 times, Y 20 times 
	X = -2;
	while (X <= 4)
	{
		Y = X * (X + 1)*(X - 1)*(X - 3);
		pt.x = (int)(50 * X);
		pt.y = (int)(20 * Y);
		pt.x += Origin.x; pt.y = Origin.y - pt.y;
		if (DrawRc.PtInRect(pt))
			dc.SetPixel(pt, Color1);
		X += 0.001;
	}
	dc.SetTextColor(Color1);
	dc.TextOutW(TopLeft.x + 20, BottomRight.y - 50, "f(x)=x(x+1)(x-1)(x-3)");
}

void CCode5A::Lemniscate()
{
	CClientDC dc(this);
	double r1, r2, t;
	CPoint pt;
	DrawAxes();

	// draw the curves where x, y are all magnified 150 times

	t = 0;
	while (t <= 8) {
		r1 = sin(3 * t);
		pt.x = (int)(150 * r1*cos(t)); pt.y = (int)(150 * r1*sin(t));
		pt.x += Origin.x; pt.y = Origin.y - pt.y;
		if (DrawRc.PtInRect(pt))
			dc.SetPixel(pt, Color2);
		t += .001;
	}

	dc.SetTextColor(Color1);
	dc.TextOutW(TopLeft.x + 20, BottomRight.y - 60, "r(t) = sin 3t");
	dc.SetTextColor(Color2);
	dc.TextOutW(TopLeft.x + 20, BottomRight.y - 30, "r(t) = cos 8t");
}


void CCode5A::CreativeNet()
{
	CClientDC dc(this);
	double X, Y, t;
	CPen penMark(PS_SOLID, 1, Color1);
	CPoint pt;

	dc.SetTextColor(Color1);
	dc.TextOutW(TopLeft.x + 200, TopLeft.y + 20, "Creative Net");
	dc.TextOutW(TopLeft.x + 20, TopLeft.y - 40, "X(t) = sin(0.99*t)+2*cos(3.01*t)");
	dc.TextOutW(TopLeft.x + 20, TopLeft.y - 20, "Y(t) = sin(1.01*t)+0.1*sin(15.03*t)");
	dc.SelectObject(&penMark);
	t = 150;
	X = sin(0.99*t) + 2 * cos(3.01*t); Y = sin(1.01*t) + 0.1*sin(15.03*t);
	pt.x = (int)(60 * X); pt.y = (int)(120 * Y);
	pt.x += Origin.x; pt.y = Origin.y - pt.y;
	
	dc.MoveTo(pt);
	while (t <= 150) {
		X = sin(0.99*t) + 2 * cos(3.01*t); 
		Y = sin(1.01*t) + 0.1*sin(15.03*t);
		pt.x = (int)(60 * X); 
		pt.y = (int)(120 * Y);
		pt.x += Origin.x; pt.y = Origin.y - pt.y;
		if (DrawRc.PtInRect(pt))
			dc.LineTo(pt.x, pt.y);
		::Sleep(0);
		t += 0.005;
	}
}