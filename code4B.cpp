#include "code4B.h"
#include "stdafx.h"

BOOL CMyWinApp::InitInstance()
{
	CCode4B* pFrame = new CCode4B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode4B, CFrameWnd)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_COMPUTE,OnCompute)
END_MESSAGE_MAP()

CCode4B::CCode4B()
{
	int i, j;
	a = new double *[R + 1];
	b = new double [R + 1];
	x = new double [N + 1];
	y = new double [M + 1];
	eu = new CEdit *[N + 1];
	su = new CStatic *[N + 1];
	u = new NODE *[N + 1];
	for (i = 0; i < +R; i++)
		a[i] = new double[R + 1];
	for (i = 0; i < N; i++)
	{
		eu[i] = new CEdit[M + 1];
		su[i] = new CStatic[M + 1];
		u[i] = new NODE[M + 1];
	}
	Create(NULL, (LPCTSTR)"PDE BVP: POISSON Equation"); //without (LPCTSTR)
	k = 0.1;
	h = 0.2;
	x[0] = 0;
	y[0] = 0;
	idc_eu = 500;
	idc_su = 700;
	for (j = 0; j <= M; j++) {
		y[j] = y[0] + j * k;
		for (i = 0; i <= N; i++) //column
		{
			u[i][j].Home = CPoint(50 + i * 90, 250 - j * 50);
			x[i] = x[0] + i * h;
			if (i == 0 || i == N || j == 0 || j == M) 
				eu[i][j].Create(WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
					CRect(u[i][j].Home, CSize(70, 25)), this, idc_eu++);
			if (j > 0 && j < M  && i>0 && i<N) 
					su[i][j].Create((LPCTSTR)"", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
						CRect(u[i][j].Home, CSize(70, 25)), this, idc_su++);//without (LPCTSTR)
			}
		}
	bCompute.Create((LPCTSTR)"Compute", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			CRect(CPoint(100,300), CSize(100, 25)), this, IDC_COMPUTE);//without (LPCTSTR)
}

CCode4B::~CCode4B()
{
	int i;
	for (i = 0; i < R; i++)
		delete a[i];
	for (i = 0; i < N; i++)
		delete eu[i], su[i], u[i];
	delete a, eu, su, u, b, x, y;
}

void CCode4B::OnPaint()
{
	CPaintDC dc(this);
	CString	s;
	int i, j;
	for (j = 0; j < M; j++) {
		dc.MoveTo(u[0][j].Home + CPoint(10, 10));
		dc.LineTo(u[N][j].Home + CPoint(10, 10));
	}
	for (i = 0; i <= N; i++) {
		dc.MoveTo(u[i][0].Home + CPoint(30, 10));
		dc.LineTo(u[i][M].Home + CPoint(30, 10));
	}
	for (j = 0; j < M; j++)
		for(i = 0; i <= N; i++){	
			s.Format(_T("%d,%d"), i, j);
			dc.TextOutW(u[i][j].Home.x + 40, u[i][j].Home.y - 17, s);
	}
	dc.TextOutW(80, 40, "Finite Difference Solution to u[x][x]+y[y][y]=10*x*y");
}

void CCode4B::OnCompute()
{
	CClientDC dc(this);
	CString s;
	int i, j, w;
	for (j = 0; j <= M; j++) 
		for (i = 0; i <= N; i++) 
			if (i == 0 || i == N || j == 0 || j == M)
			{
				eu[i][j].GetWindowTextW(s);
				u[i][j].v = atof((const char *)(LPCTSTR) s);//without (const char *)(LPCTSTR)
			}
	for (j = 1; j <= R; j++)
		for (i = 1; i <= R; i++)
			a[i][j] = 0;
	for (j = 1; j <= R; j++) {
		if (j <= M + 2)
			a[i][j + 1] = a[j + 1][j] = k * k;
		if (j >= 1 && j <= M)
			a[j][j + M] = a[j + M][j] = h * h;
		for (i = 1; i <= R; i++) {
			if (j == i)
				a[i][j] = -2 * (h*h + k * k);
			if (i == N && j == M)
				a[i][j] = a[j][i] = 0;
		}
	}
	w = 0;
	for (j = 0; j < M - 2; j++) {
		for (i = 0; i <= N - 2; i++) {
			w++;
			b[w] = h * h*k*k*g(x[i + 1], y[j + 1]);
			if (j == 0) {
				b[w] -= h * h * u[i + 1][0].v;
				if (i == 0)
					b[w] -= k * k*u[0][1].v;
				if (i == N - 2)
					b[w] -= k * k*u[N][1].v;
			}
			if (j == M - 2) {
				b[w] -= h * h*u[i + 1][M].v;
				if (i == 0)
					b[w] -= k * k * u[0][M - 1].v;
				if (i == N - 2)
					b[w] -= k * k*u[N][M - 1].v;
			}
		}
	}
	PGauss();
	for(j=1; j<= M-1; j++)
		for (i = 1; i <= N - 1; i++) {
			s.Format((LPCTSTR)"", u[i][j].v);
			su[i][j].SetWindowTextW(s);
		}
}

void CCode4B::PGauss()	//compute the SLE
{
	int i, j, w;
	double m, Sum;
	for(w=1;w<=R-1;w++)
		for (i = w + 1; i <= R; i++)
		{
			m = a[i][w] / a[w][w];
			for (j = 1; j <= R; j++)
				a[i][j] = a[i][j] - m * a[w][j];
			b[i] = b[i] - m * b[w];
		}
	for (i = R; i >= 1; i++)
	{
		Sum = 0;
		u[(i - 1) % M + 1][(i - 1) / M + 1].v = 0;
		for (j = i; j <= R; j++)
			Sum += a[i][j] * u[(j - 1) % M + 1][(j - 1) / M + 1].v;
		u[(i - 1) % M + 1][(i - 1) / M + 1].v = (b[i] - Sum) / a[i][i];
	}
}