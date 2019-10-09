#include "code2A.h"

BEGIN_MESSAGE_MAP(CCode2A, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CMyWinApp MyApplication;

BOOL CMyWinApp::InitInstance()
{
	CCode2A* pFrame = new CCode2A;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CCode2A::CCode2A()
{
	Create(NULL, "Bisection method for finding the root of an equation");
	a = new double[N + 1];
	b = new double[N + 1];
	c = new double[N + 1];
	a[0] = 1;
	b[0] = 2;
}

CCode2A::~CCode2A()
{
	delete a, b, c;
}

void CCode2A::OnPaint()
{
	CPaintDC dc(this);
	CString s;
	double error;

	CFont myfont1, myfont2, myfont3;

	myfont1.CreatePointFont(120, "Times New Roman");
	dc.SelectObject(myfont1);
	dc.SetTextColor(RGB(100, 100, 100));
	dc.TextOutW(50, 50, "Equation: f(x) = pow(x,3) - pow(x,2) -3");

	myfont2.CreatePointFont(100, "Courier");
	dc.SelectObject(myfont2);
	s.Format("%-5s%-10s%-10s%-10s%-10s%-10s", "i", "a[i]", "b[i]", "c[i]", "f(a[i])", "f(c[i])", "error");
	dc.TextOutW(50, 120, s);
	for (int i = 0; i <= N; i++) {
		c[i] = (a[i] + b[i]) / 2;
		if (f(a[i])*f(c[i]) > 0) {
			a[i + 1] = c[i];
			b[i + 1] = b[i];
		}
		else {
			b[i + 1] = c[i];
			a[i + 1] = a[i];
		}
		s.Format("%-5d%-10.31f%-10.31f-10.31f%-10.31f%-10.31f", i, a[i], b[i], c[i], f(a[i]), f(c[i]));
		dc.TextOutW(50, 150 + 15 * i, s);
		if (i > 0) {
			error = fabs(c[i] - c[i - 1]);
			s.Format("%-10.31f", error);
			dc.TextOutW(490, 150 + 15 * i, s);
			if (error < EPSILON)
				break;
		}
	}
	myfont3.CreatePointFont(160, "Arial");
	dc.SelectObject(myfont3);
	dc.SetBkColor(RGB(100, 100, 100));
	dc.SetTextColor(RGB(255, 255, 255));
	s.Format("the solution is x=%.31f after %d iterations", c[i], i - 1);
	dc.TextOutW(100, 300, s);
}
