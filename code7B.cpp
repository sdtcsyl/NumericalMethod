#include "code7B.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode7B* pFrame = new CCode7B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode7B, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

CCode7B::CCode7B()
{
	int i, j;
	P = new int[N + 1];
	Q = new int[N + 1];
	v = new NODE[N + 1];
	e = new LINK *[N + 1];
	for (i = 0; i <= N; i++)
		e[i] = new LINK[N + 1];
	Create(NULL, (LPCTSTR)"Prim's Algorithm for computing the MST", WS_OVERLAPPEDWINDOW, CRect(0, 0, 800, 500));
	TopLeft = CPoint(20, 20); BottomRight = CPoint(450, 360);
	MSTbutton.Create((LPCTSTR)"Compute MST", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		CRect(500, 330, 650, 360), this, IDC_MST);
	MatText = CPoint(500, 30); InstText = CPoint(80, 730);
	nv = 0; nLink = 0; RButtonFlag = 0; idc_WtInput = 1001;
	for (i = 1; i <= N; i++)
		for(j=1; j<=N;j++)
		{
			e[i][j].Wt = e[j][i].Wt = 99;
			e[i][j].Flag = e[j][i].Flag = 0;
		}
	FontCourier.CreatePointFont(60, (LPCTSTR)"Courier");
	BgColor = RGB(240, 240, 240); TextGap = 25; BoxSize = CSize(10, 10);
	TextColor = RGB(100, 100, 100);
}

CCode7B::~CCode7B()
{
	for (int i = 0; i <= N; i++)
		delete e[i];
	delete v, e, P, Q;
}

void CCode7B::OnPaint()
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
}

void CCode7B::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	CString s;
	CPen penGray(PS_SOLID, 2, TextColor);
	dc.SelectObject(penGray);

	dc.SelectObject(FontCourier);
	dc.SetTextColor(TextColor);
	dc.SetBkColor(BgColor);
	if (CRect(TopLeft, BottomRight).PtInRect(pt))
		if (nv < N)
		{
			nv++;
			v[nv].Home = pt;
			v[nv].Box = CRect(CPoint(pt), CSize(BoxSize));
			dc.Rectangle(v[nv].Box);
			s.Format(_T("%d"), nv);
			dc.TextOutW(v[nv].Home.x - 10, v[nv].Home.y - 10, s);
		}
}

void CCode7B::OnRButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	int i, u, w;
	CPen penGray(PS_SOLID, 1, TextColor);
	dc.SelectObject(penGray);
	dc.SetBkColor(BgColor);

	for (i = 1; i <= nv; i++)
	{
		if (v[i].Box.PtInRect(pt))
		{
			RButtonFlag++;
			if (RButtonFlag == 1)
				Pt1 = i;
			if (RButtonFlag == 2)
			{
				Pt2 = i; nLink++;
				dc.MoveTo(v[Pt1].Home); dc.LineTo(v[Pt2].Home);
				u = (v[Pt1].Home.x + v[Pt2].Home.x) / 2;
				w = (v[Pt1].Home.y + v[Pt2].Home.y) / 2;
				e[Pt1][Pt2].InputBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(CPoint(u, w), CSize(12, 20)), this, idc_WtInput++);
				e[Pt1][Pt2].Flag = 1;
				RButtonFlag = 0;			
			}
		}
	}
}

void CCode7B::OnClickCalc()
{
	CClientDC dc(this);
	CString s;
	int i, j;

	dc.SelectObject(FontCourier);
	dc.SetTextColor(TextColor); dc.SetBkColor(BgColor);
	for (i = 1; i <= nv; i++)
	{
		P[i] = ((i == 1) ? 1 : 0);
		Q[i] = ((i == 1) ? 0 : i);

		for (j = 1; j <= nv; j++)
		{
			if (e[i][j].Flag)
			{
				dc.SetTextColor(TextColor);
				e[i][j].InputBox.GetWindowTextW(s);
				const char * foo = (LPCSTR)s.GetString();	//???
				e[i][j].Wt = e[j][i].Wt = atoi(foo);
				//e[i][j].Wt = e[j][i].Wt = atoi(s);
				s.Format(_T("%d"), e[i][j].Wt);
				dc.TextOutW(MatText.x + TextGap * (i - 1), MatText.y + TextGap + TextGap * (j - 1), s);
				dc.TextOutW(MatText.x + TextGap * (j - 1), MatText.y + TextGap + TextGap * (i - 1), s);
			}
			if (e[i][j].Wt == 99)
			{
				dc.SetTextColor(RGB(180, 180, 180));
				s.Format(_T("%d"), 0);
				dc.TextOutW(MatText.x + TextGap * (i - 1), MatText.y + TextGap + TextGap * (j - 1), s);
				dc.TextOutW(MatText.x + TextGap * (j - 1), MatText.y + TextGap + TextGap * (i - 1), s);
			}
		}
	}
	MSTcost = 0;
	for (i = 1; i <= nv - 1; i++)
		Prim();
	dc.SetTextColor(TextColor);
	s.Format(_T("COST=%d"), MSTcost);
	dc.TextOutW(600, 400, s);
}


void CCode7B::Prim()
{
	CClientDC dc(this);
	CPen penMSTpath(PS_SOLID, 3, RGB(100, 100, 100));
	int i, j, k, r, w, u, h;

	MinWt = 99;
	for (i = 1; i <= nv; i++) 
	{
		if (P[i] == 0)
			break;
		for(j=1; j<=nv;j++)
			if(Q[j]!=0)
				for(k=1;k<=nv;k++)
					if(Q[k]!=0)
						for(r=1;r<=nv; r++)
							if(Q[r]!=0)
								for(h=1;h<=nv;h++)
									if(Q[h]!=0)
										if (e[P[i]][Q[j]].Wt <= e[P[i]][Q[k]].Wt &&
											e[P[i]][Q[k]].Wt <= e[P[i]][Q[i]].Wt &&
											e[P[i]][Q[r]].Wt <= e[P[i]][Q[h]].Wt &&
											e[P[i]][Q[j]].Wt <= MinWt)
										{
											MinWt = e[P[i]][Q[j]].Wt;
											u = i; w = j;
										}
	}
	MSTcost += MinWt;
	dc.SelectObject(penMSTpath);
	dc.MoveTo(v[P[u]].Home); dc.LineTo(v[Q[w]].Home);
	for (i = 1; i <= nv; i++)
	{
		if (P[i] == 0)
		{
			P[i] = 2;
			break;
		}
	}
	for (i = 1; i <= nv; i++)
	{
		if (Q[i] == w)
		{
			Q[i] = 0;
			break;
		}
	}

}
