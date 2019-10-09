#include "code7A.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode7A* pFrame = new CCode7A;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode7A, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

CCode7A::CCode7A()
{
	int i, j;
	v = new NODE[N + 1];
	e = new int *[N + 1];
	for (i = 0; i <= N; i++)
		e[i] = new int[N + 1];
	Create(NULL, (LPCTSTR)"Drawing a graph", WS_OVERLAPPEDWINDOW, CRect(0, 0, 800, 500));
	TopLeft = CPoint(20, 20); BottomRight = CPoint(450, 360);
	MatText = CPoint(500, 30); InstText = CPoint(80, 730);
	nv = 0; nLink = 0; RButtonFlag = 0;
	for (i = 1; i <= N; i++)
	{
		v[i].degree = 0;
		for (j = 1; j <= N; j++)
			e[i][j] = e[j][i] = 99;
	}
	FontCourier.CreatePointFont(60, (LPCTSTR)"Courier");
	BgColor = RGB(240, 240, 240); TextGap = 20; BoxSize = CSize(10, 10);
	TextColor = RGB(100, 100, 100);
}

CCode7A::~CCode7A()
{
	for (int i = 0; i <= N; i++)
		delete e[i];
	delete v, e;
}

void CCode7A::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	CBrush BgBrush(BgColor);
	GetClientRect(&rc);
	dc.FillRect(&rc, &BgBrush);

	CPen penDrawingBox(PS_SOLID, 4, RGB(100, 100, 100));
	dc.SelectObject(penDrawingBox);
	dc.SelectStockObject(HOLLOW_BRUSH);
	rc = CRect(TopLeft, BottomRight);
	dc.Rectangle(rc);

	dc.SelectObject(FontCourier);
	dc.SetTextColor(TextColor); dc.SetBkColor(BgColor);
	dc.TextOutW(InstText.x, InstText.y, "Instructions:");
	dc.TextOutW(InstText.x + 30, InstText.y + 25, "Left mouse to click points");
	dc.TextOutW(InstText.x + 30, InstText.y + 45, "Right mouse to draw a link between two points");
	dc.TextOutW(MatText.x , MatText.y, "Adjacency Matrix");
	dc.TextOutW(MatText.x + TextGap*N, MatText.y, "Degree");
}

void CCode7A::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	CString s;
	CPen penGray(PS_SOLID, 2, TextColor);
	dc.SelectObject(penGray);

	dc.SelectObject(FontCourier);
	dc.SetTextColor(TextColor);
	dc.SetBkColor(BgColor);
	if(CRect(TopLeft, BottomRight).PtInRect(pt))
		if (nv <= N)
		{
			nv++;
			v[nv].Home = pt;
			v[nv].Box = CRect(CPoint(pt), CSize(BoxSize));
			dc.Rectangle(v[nv].Box);
			s.Format(_T("%d"), nv);
			dc.TextOutW(v[nv].Home.x - 10, v[nv].Home.y - 10, s);
			for (int i = 1; i <= nv; i++) {
				dc.TextOutW(MatText.x + TextGap * (i - 1), MatText.y + TextGap + TextGap * (nv - 1), "0");
				dc.TextOutW(MatText.x + TextGap * (nv - 1), MatText.y + TextGap + TextGap * (i - 1), "0");
				s.Format(_T("%d"), v[nv].degree);
				dc.TextOutW(MatText.x + TextGap + TextGap * N, MatText.y + TextGap + TextGap * (nv - 1), s);
			}
		}
}

void CCode7A::OnRButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	CString s;
	CPen penGray(PS_SOLID, 1, TextColor);
	dc.SelectObject(penGray);
	dc.SetBkColor(BgColor);

	for (int i = 1; i <= nv; i++)
	{
		if (v[i].Box.PtInRect(pt))
		{
			RButtonFlag++;
			if (RButtonFlag == 1)
				Pt1 = i;
			if (RButtonFlag == 2)
			{
				Pt2 = i; e[Pt1][Pt2] = 1; nLink++;
			}
		}
		if (RButtonFlag == 2)
		{
			dc.MoveTo(v[Pt1].Home); dc.LineTo(v[Pt2].Home);
			s.Format(_T("%d"), e[Pt1][Pt2]);
			dc.TextOutW(MatText.x + TextGap * (Pt1 - 1), MatText.y + TextGap + TextGap * (Pt2 - 1), s);
			dc.TextOutW(MatText.x + TextGap * (Pt2 - 1), MatText.y + TextGap + TextGap * (Pt1 - 1), s);
			s.Format(_T("%d"), ++v[Pt1].degree);
			dc.TextOutW(MatText.x + TextGap + TextGap * N, MatText.y + TextGap + TextGap * (Pt1 - 1), s);
			s.Format(_T("%d"), ++v[Pt2].degree);
			dc.TextOutW(MatText.x + TextGap + TextGap * N, MatText.y + TextGap + TextGap * (Pt2 - 1), s);
			RButtonFlag = 0;
		}
	}
}