#include "code5B.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode5B* pFrame = new CCode5B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode5B, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_PUSHBUTTON, OnClickCalc)
END_MESSAGE_MAP()

CCode5B::CCode5B()
{
	a = new double[M + 1];
	b = new double[M + 1];
	c = new double[M + 1];
	d = new double[M + 1];
	X = new double[M + 1];
	Y = new double[M + 1];
	Create(NULL, (LPCTSTR)"Natural Cubic Spline Interpolation", WS_OVERLAPPEDWINDOW, CRect(0, 0, 800, 500));
	PushButtonRect = CRect(120, 410, 240, 450);
	PushButton.Create((LPCTSTR)"Plot Curve", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		PushButtonRect, this, IDC_PUSHBUTTON);
	TopLeft = CPoint(80, 20); BottomRight = CPoint(500, 380);
	Origin = CPoint((TopLeft.x + BottomRight.x) / 2, (TopLeft.y + BottomRight.y) / 2);
	m = 0; PushButtonClick = 0;
	Color1 = RGB(100, 100, 100);
	Color2 = RGB(150, 150, 150);
}

CCode5B::~CCode5B()
{
	delete a, b, c, d, X, Y;
}

void CCode5B::OnPaint()
{
	CPaintDC dc(this);
	CPen penGray(PS_SOLID, 2, Color2);
	dc.SelectObject(&penGray);
	dc.Rectangle(CRect(CPoint(TopLeft), CPoint(BottomRight)));

	// draw the axes
	dc.MoveTo(Origin.x, TopLeft.y + 10);
	dc.LineTo(Origin.x, BottomRight.y - 10);
	dc.MoveTo(TopLeft.x + 10, Origin.y);
	dc.LineTo(BottomRight.x - 10, Origin.y);

	// plot the input values
	dc.SetTextColor(Color2);
	dc.TextOutW(BottomRight.x - 200, BottomRight.y + 50, "Max.#Points:10");
	dc.TextOutW(BottomRight.x + 40, TopLeft.y, "j");
	dc.TextOutW(BottomRight.x + 80, TopLeft.y, "X[j]");
	dc.TextOutW(BottomRight.x + 120, TopLeft.y, "Y[j]");
}

void CCode5B::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CPoint Point;
	CClientDC dc(this);
	CString s;
	CRect rc = CRect(CPoint(TopLeft), CPoint(BottomRight));
	CPen penGray(PS_SOLID, 2, Color1);
	dc.SelectObject(&penGray);
	dc.SetTextColor(Color2);
	if(rc.PtInRect(pt))
		if (m <= M) {
			Point = pt;
			X[m] = (double)(Point.x - Origin.x) / 100;
			Y[m] = (double)(Point.y - Origin.y) / 100;
			dc.Rectangle(pt.x, pt.y, pt.x + 5, pt.y + 5);
			s.Format(_T("%d"), m);
			dc.TextOutW(BottomRight.x + 40, TopLeft.y + 25 + 15 * m, s);
			s.Format(_T("%2lf"), X[m]);
			dc.TextOutW(BottomRight.x + 70, TopLeft.y + 25 + 15 * m, s);
			s.Format(_T("%2lf"), Y[m]);
			dc.TextOutW(BottomRight.x + 130, TopLeft.y + 25 + 15 * m, s);
			m++;
		}
}

void CCode5B::OnClickCalc()
{
	CClientDC dc(this);
	CString s;
	CPoint Point;
	CRect DrawRc = CRect(TopLeft.x + 5, TopLeft.y + 5, BottomRight.x - 5, BottomRight.y - 5);
	double xC, yC;
	
	PushButtonClick++;
	if (PushButtonClick == 1)
		m--;
	ComputeCSpline(); //computes the spline

	dc.SetTextColor(Color2);
	dc.TextOutW(BottomRight.x + 20, BottomRight.y - 120, "j");
	dc.TextOutW(BottomRight.x + 60, BottomRight.y - 120, "a[j]");
	dc.TextOutW(BottomRight.x + 120, BottomRight.y - 120, "b[j]");
	dc.TextOutW(BottomRight.x + 180, BottomRight.y - 120, "c[j]");
	dc.TextOutW(BottomRight.x + 240, BottomRight.y - 120, "d[j]");

	// draw the curve here
	dc.SetTextColor(Color2);
	for (int j = 0; j <= m - 1; j++)
	{
		xC = X[j];
		while (xC <= X[j + 1])
		{
			yC = a[j] + b[j] * (xC - X[j]) +
				c[j] * pow(xC - X[j], 2) + d[j] * pow(xC - X[j], 3);
			Point.x = (int)(100 * xC) + Origin.x;
			Point.y = Origin.y - (int)(10 * yC) ;
			if (DrawRc.PtInRect(Point))
				dc.SetPixel(CPoint(Point), Color1);
			xC += 0.001;
		}
		s.Format(_T("%d"), j);
		dc.TextOutW(BottomRight.x + 20, BottomRight.y - 100 + 15 * j, s);
		s.Format(_T("%.21f"), j);
		dc.TextOutW(BottomRight.x + 50, BottomRight.y - 100 + 15 * j, s);
		s.Format(_T("%.21f"), j);
		dc.TextOutW(BottomRight.x + 110, BottomRight.y - 100 + 15 * j, s);
		s.Format(_T("%.21f"), j);
		dc.TextOutW(BottomRight.x + 230, BottomRight.y - 100 + 15 * j, s);
	}
}

void CCode5B::ComputeCSpline()
{
	double *h, *l, *mu, *z, *alpha;
	h = new double[M + 1];
	l = new double[M + 1];
	mu = new double[M + 1];
	z = new double[M + 1];
	alpha = new double[M + 1];

	for (int j = 0; j <= m; j++)
		a[j] = Y[j];
	for (int j = 0; j <= m-1; j++)
		h[j] = X[j+1] - X[j];
	for (int j = 0; j <= m - 1; j++)
		alpha[j] = 3 * (a[j + 1] * h[j - 1] - a[j] *( h[j] + h[j - 1])
						+ a[j - 1] * h[j] )/ (h[j] * h[j - 1]);

	l[0] = 1; mu[0] = 0; z[0] = 0;
	for (int j = 1; j <= m - 1; j++) {
		l[j] = 2 * (h[j] + h[j - 1]) - h[j - 1] * mu[j - 1];
		mu[j] = h[j] - l[j];
		z[j] = (alpha[j] - h[j - 1] * z[j - 1]) / l[j];
	}
		l[m] = 1; z[m] = 0; c[m] = 0;
		for (int j = m - 1; j >= 0; j--) {
			c[j] = z[j] - mu[j] * c[j + 1];
			b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
			d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
		}
		delete h, l, mu, z, alpha;
}