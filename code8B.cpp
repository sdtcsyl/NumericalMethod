#include "code8B.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode8B* pFrame = new CCode8B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode8B, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CCode8B::CCode8B()
{
	int i, j, k;
	Create(NULL, (CString)"Shortest path in a mesh network", WS_OVERLAPPEDWINDOW, CRect(0, 0, 920, 600));

	//allocate memory
	v = new NODE[N + 1];
	e = new LINK *[N + 1];
	alpha = new int **[N + 1];
	Path = new PATH[N + 1];
	for (i = 0; i <= N; i++) {
		e[i] = new LINK[N + 1];
		alpha[i] = new int*[N + 1];
		for (j = 0; j <= N; j++)
			alpha[i][j] = new int[N + 1];
	}

	//initialize variables
	time_t seed = time(NULL);
	srand((unsigned)seed);
	nLink = 0; nPath = 0; LineFlag = 0;
	for (i = 1; i <= N; i++)
	{
		e[i][i].Wt = 99;
		for (j = i; j <= N; j++) {
			e[i][j].Wt = e[j][i].Wt = 99;
			if (( (j - i == 1 && j - i != Nx-1) || ( j-i == Nx && j-i != Nx*(Ny-1) )) && !( j-i == 1 && i % Nx == 0))
			{
				e[i][j].Wt = e[j][i].Wt = 1 + rand() % 9;
				nLink++;
			}
		}
	}

	for (i = 1; i <= N; i++)
		for (j = 1; j <= N; j++)
		{
			e[i][j].sd = e[i][j].Wt;
			for (k = 1; k <= N; k++)
				alpha[i][j][k] = 99;
		}

	//set fonts, colors, text and drawing area
	TextGap = 50; TextHome = CPoint(70, 520);
	DrawArea = CRect(10, 20, 900, 350);
	TextArea = CRect(10, 360, 900, 560);
	k = 0;
	for(j=1;j<=Ny;j++)
		for (i = 1; i <= Nx; i++)
		{
			k++;
			v[k].Home.x = 40 + DrawArea.left + 80 * (i - 1);
			v[k].Home.y = 40 + DrawArea.top + 60 * (i - 1);
		}
	::ZeroMemory(&lfCourier, sizeof(lfCourier));
	lfCourier.lfHeight = 60;
	fontCourier.CreatePointFontIndirect(&lfCourier);
	::ZeroMemory(&lfTimes, sizeof(lfTimes));
	lfTimes.lfHeight = 60;
	fontCourier.CreatePointFontIndirect(&lfTimes);

	// compute the shortest paths
	ComputePath();
}

CCode8B::~CCode8B()
{
	int i, j;
	for (i = 0; i <= N; i++)
	{
		e[i] = new LINK[N + 1];
		for (j = 0; j <= N; j++)
			delete alpha[i][j];
		delete e[i], alpha[i];
	}
	delete v, e, alpha, Path;
}

void CCode8B::OnPaint()
{
	CPaintDC dc(this);
	int i, j, k;
	CString s;
	CPen LinePen(PS_SOLID, 1, RGB(150, 150, 150));
	CPen TextBoxPen(PS_SOLID, 2, RGB(0, 50, 255));
	CBrush BgBrush(RGB(0,0,0));
	CRect rc = CRect(DrawArea.left, DrawArea.top, TextArea.right, TextArea.bottom);
	dc.FillRect(&rc, &BgBrush);
	rc = CRect(DrawArea.left+10, DrawArea.top + 10, TextArea.right-10, TextArea.bottom-10);
	dc.SelectObject(TextBoxPen);
	dc.MoveTo(TextArea.left + 10, TextArea.top + 10);
	dc.LineTo(TextArea.right - 20, TextArea.top + 10);
	dc.LineTo(TextArea.right - 20, TextArea.bottom - 20);
	dc.LineTo(TextArea.left - 20, TextArea.bottom - 20);
	dc.LineTo(TextArea.left - 20, TextArea.top + 10);
	dc.SelectObject(LinePen); dc.SelectObject(&fontTimes);
	dc.SetBkMode(TRANSPARENT); dc.SetTextColor(RGB(0, 0, 255));
	for(i=1;i<=N;i++)
		for (j = i; j <= N; j++)
			if (e[i][j].Wt != 99)
			{
				dc.MoveTo(v[i].Home); dc.LineTo(v[j].Home);
				s.Format((CString)"%d", e[i][j].Wt);
				if (i != j)
					dc.TextOutW(5+(v[i].Home.x + v[j].Home.x) / 2, (v[i].Home.y + v[j].Home.y) / 2, s);
			}
	dc.SetTextColor(RGB(0, 0, 0));
	for (int i = 1; i <= N; i++)	//draw the nodes
	{
		v[i].rc = CRect(v[i].Home.x-10, v[i].Home.y-10, v[i].Home.x + 10, v[i].Home.y + 10 );
		dc.FillSolidRect(&v[i].rc, RGB(250,250,0));
		s.Format((CString)"%d", i);
		dc.TextOutW(v[i].Home.x - 8, v[i].Home.y - 8, s);
	}
	dc.SelectObject(&fontTimes);
	dc.SetTextColor(RGB(200, 200, 200));
	dc.SetBkMode(TRANSPARENT);
	dc.TextOutW(20 + TextArea.left, TextArea.top + 20, "Path");
	for (k = 1; k <= 15; k++)
	{
		s.Format((CString)"%d", k);
		dc.TextOutW(50 + TextArea.left + TextGap * (k - 1), 20 + TextArea.top, s);
	}
}

void CCode8B::OnLButtonDown(UINT nFlags, CPoint pt)
{
	int i;
	if (nPath <= MaxPath -1)
		for (i = 1; i <= N; i++)
			if (v[i].rc.PtInRect(pt))
			{
				LineFlag++;
				if (LineFlag == 1)
					Source = i;
				if (LineFlag == 2)
				{
					Dest = i;
					LineFlag = 0;
					if (e[Source][Dest].sd != 99)
					{
						nPath++;
						Path[nPath].Distance = e[Source][Dest].sd;
						DrawPath();
					}
				}
			}
}

void CCode8B::ComputePath()
{
	int i, j, k, m, n;
	for (i = 1; i <= N; i++)
		for (j = 1; j <= N; j++)
			for (k = 1; k <= N; k++)
				if (e[j][i].sd != 99 || e[i][j].sd != 99 || e[j][k].sd != 99)
					if (e[j][i].sd + e[i][k].sd < e[j][k].sd)
					{
						n = 1;
						e[j][k].sd = e[j][i].sd + e[i][k].sd;
						for (m = 1; m <= N; m++)
							alpha[j][k][m] = 99;
						for (m = 1; m <= N; m++)
							if (alpha[j][i][m] != 99)
								alpha[j][k][n++] = alpha[j][i][m];
						alpha[j][k][n++] = i;
						for (m = 1; m <= N; m++)
							if (alpha[i][k][m] != 99)
								alpha[j][k][n++] = alpha[i][k][m];
					}
}


void CCode8B::DrawPath()
{
	CClientDC dc(this);
	CString s;

	int u, w, k, PathColor;
	int r = Path[nPath].st = 1;
	Path[nPath].PLink[r].b = Source;
	for (k = 1; k <= N; k++)
	{
		u = alpha[Source][Dest][k];
		if (u != 99)
		{
			Path[nPath].PLink[r].e = Path[nPath].PLink[r + 1].b = u;
			r++; 
		}
	}
	Path[nPath].nPLink = r;
	Path[nPath].PLink[r].e = Dest;

	time_t seed = time(NULL); srand((unsigned)seed);
	PathColor = RGB(rand() % 256, rand() % 256, rand() % 256);
	dc.SelectObject(&fontTimes);
	dc.SetTextColor(RGB(200, 200, 200));
	dc.SetBkMode(TRANSPARENT);
	s.Format((CString)"%d", nPath);
	dc.TextOutW(20 + TextArea.left, 20 + TextArea.top + 15 * nPath, s);
	CPen PathPen(PS_SOLID, 3, PathColor);
	dc.SelectObject(&PathPen);
	u = Path[nPath].PLink[1].b;
	dc.MoveTo(v[u].Home);
	dc.SetTextColor(PathColor);
	for (r = 1; r <= Path[nPath].nPLink; r++)
	{
		w = Path[nPath].PLink[r].e;
		dc.LineTo(v[w].Home);
		s.Format((CString)"%d", Path[nPath].PLink[r].b);
		dc.TextOutW(50 + TextArea.left + TextGap * (r - 1 + Path[nPath].st - 1), 20 + TextArea.top + 15 * nPath, s);
	}
	s.Format((CString)"%d", Path[nPath].PLink[r-1].e);
	dc.TextOutW(50 + TextArea.left + TextGap * (r - 1 + Path[nPath].st - 1), 20 + TextArea.top + 15 * nPath, s);
	s.Format((CString)"%d", Path[nPath].Distance);
	dc.TextOutW(TextArea.right -100, 20 + TextArea.top + 15 * nPath, s);
}


