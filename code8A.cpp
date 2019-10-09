#include "code8A.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode8A* pFrame = new CCode8A;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode8A, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CCode8A::CCode8A()
{
	int i, j, k;
	Create(NULL, (CString)"Floyd-Warshall's Shortest Path Algorithm", WS_OVERLAPPEDWINDOW, CRect(0, 0, 800, 600));

	//allocate memory
	v = new NODE[N + 1];
	e = new LINK *[N + 1];
	alpha = new int **[N + 1];
	for (i = 0; i <= N; i++) {
		e[i] = new LINK[N + 1];
		alpha[i] = new int*[N + 1];
		for (j = 0; j <= N; j++)
			alpha[i][j] = new int[N + 1];
	}

	//initialize variables
	LineFlag = 0;
	TopLeft = CPoint(20, 20); BottomRight = CPoint(70, 450);
	time_t seed = time(NULL); srand((unsigned)seed);
	for (i = 1; i <= N; i++)
	{
		e[i][i].Wt = 0;
		for (j = i; j <= N; j++) {
			if (sqrt(pow(v[i].Home.x - v[j].Home.x, 2) + pow(v[i].Home.y - v[j].Home.y, 2)) <= LinkRange)
				e[i][j].Wt = e[j][i].Wt = 1 + rand() % 9;
			else
				e[i][j].Wt = e[j][i].Wt = 99;
		}
	}

	for(i=1;i<=N;i++)
		for (j = 1; j <= N; j++)
		{
			e[i][j].sd = e[i][j].Wt;
			for (k = 1; k <= N; k++)
				alpha[i][j][k] = 99;
		}

	//set fonts and colors
	Color1 = RGB(100, 100, 100);
	Color2 = RGB(170, 170, 170);
	BgColor = RGB(230, 230, 230);
	CFont fontArial;
	::ZeroMemory(&lfArial, sizeof(lfArial));
	lfArial.lfHeight = 60;
	fontArial.CreatePointFontIndirect(&lfArial);
	::ZeroMemory(&lfCourier, sizeof(lfCourier));
	lfCourier.lfHeight = 60;
	fontCourier.CreatePointFontIndirect(&lfCourier);

	// compute the shortest paths
	ComputePath();
}

CCode8A::~CCode8A()
{
	int i, j;
	for (i = 0; i <= N; i++)
	{
		e[i] = new LINK[N + 1];
		for (j = 0; j <= N; j++)
			delete alpha[i][j];
		delete e[i], alpha[i];
	}
	delete v, e, alpha;
}

void CCode8A::OnPaint()
{
	CPaintDC dc(this);
	CString s;
	CRect rc;

	//clear the window
	CBrush BgBrush(BgColor);
	GetClientRect(&rc);
	dc.FillRect(&rc, &BgBrush);

	//set the initial display
	rc = CRect(TopLeft, BottomRight);
	dc.Rectangle(&rc);
	DisplayGraph();
}

void CCode8A::DisplayGraph()
{
	CPaintDC dc(this);
	CString s;
	CPen penGray(PS_SOLID, 1, RGB(150, 150, 150));

	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(&fontCourier);
	dc.SetTextColor(RGB(255, 255, 255));
	dc.SelectObject(penGray);
	for (int i = 1; i <= N; i++)	//draw the nodes
	{
		v[i].rc = CRect(v[i].Home, v[i].Home+CPoint(12,12));
		dc.FillSolidRect(&v[i].rc, Color1);
		s.Format((CString)"%d", i);
		dc.TextOutW(v[i].Home.x + 2, v[i].Home.y + 2, s);
	}
	dc.SetTextColor(Color2);
	for (int i = 1; i <= N; i++)  //draw the links
		for (int j = i; j <= N; j++)
			if (e[i][j].Wt != 99)
			{
				dc.MoveTo(v[i].Home);
				dc.LineTo(v[j].Home);
				s.Format((CString)"%d", e[i][j].Wt);
				if (i != j)
					dc.TextOutW((v[i].Home.x + v[j].Home.x) / 2, (v[i].Home.y + v[j].Home.y) / 2, s);
			}
}

void CCode8A::ComputePath()
{
	int i, j, k, m, n;
	for(i=1;i<=N;i++)
		for(j=1;j<=N;j++)
			for(k=1;k<=N;k++)
				if(e[j][i].sd!=99 || e[i][j].sd!=99 || e[j][k].sd != 99)
					if (e[j][i].sd + e[i][k].sd < e[j][k].sd)
					{
						n = 1;
						e[j][k].sd = e[j][i].sd + e[i][k].sd;
						for (m = 1; m <= N; m++)
							alpha[j][k][m] = 99;
						for (m = 1; m <= N; m++)
							if (alpha[j][i][m] != 99)
								alpha[j][k][n++] = alpha[j][i][m];
						alpha[j][k][n++]=i;
						for (m = 1; m <= N; m++)
							if(alpha[i][k][m] != 99)
								alpha[j][k][n++] = alpha[i][k][m];
					}
}

void CCode8A::DrawPath()
{
	CClientDC dc(this);
	CString s;
	CRect rc;
	CPen penDark(PS_SOLID, 3, Color1);
	CBrush BgBrush(BgColor);
	int k, p, q, u, w, r;

	rc = CRect(150, 500, 800, 600);	//erase the text area
	dc.FillRect(&rc, &BgBrush);
	dc.SetTextColor(Color1);
	dc.SetBkColor(BgColor);
	p = 150; q = 120;
	r = 1; e[Source][Dest].via[r] = Source;
	dc.TextOutW(p - 100, 500, "Path:");
	dc.TextOutW(p - 100, 540, "Weight:");
	s.Format((CString)"%d", e[Source][Dest].via[r]);
	dc.TextOutW(p, 500, s);

	dc.SelectObject(&penDark);
	w = Source; dc.MoveTo(v[w].Home);
	for (k = 1; k <= N; k++)	// display the vias
	{
		u = alpha[Source][Dest][k];
		if (u != 99)
		{
			r++; e[Source][Dest].via[r] = u;
			dc.LineTo(v[u].Home);
			p += 60;
			s.Format((CString)"%d", e[Source][Dest].via[r]);
			dc.TextOutW(p, 500, s);
			q += 60;
			s.Format((CString)"%d", e[u][w].Wt);
			dc.TextOutW(q, 540, s);
			w = u;
		}
	}

	if (e[Source][w].sd != 99)	//display the destination
		dc.LineTo(v[Dest].Home);
	e[Source][Dest].via[r] = Dest;
	s.Format((CString)"%d", Dest);
	p += 60; q += 60;
	dc.TextOutW(p, 500, s);
	s.Format((CString)"%d", e[Dest][w].sd);
	dc.TextOutW(q, 540, s);
	s.Format((CString)"Total Weights : %d", e[Source][Dest].sd);
	dc.TextOutW(q + 60, 540, s);
}

void CCode8A::OnLButtonDown(UINT nFlags, CPoint pt)
{
	for(int i =1; i<=N; i++)
		if (v[i].rc.PtInRect(pt))
		{
			LineFlag++;
			if (LineFlag == 1)
				Source = i;
			if (LineFlag == 2)
			{
				Dest = i;
				DisplayGraph(); DrawPath();
				LineFlag = 0;
			}
		}
}