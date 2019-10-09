#include "code6A.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode6A* pFrame = new CCode6A;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode6A, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


CCode6A::CCode6A()
{
	f = new int *[N + 1];
	for (int i = 0; i <= N; i++)
		f[i] = new int[N + 1];
	Home = CPoint(10, 30);
	sImage = CSize(200, 100);
	Create(NULL, (LPCTSTR)"Manipulating RGB components through bit shifting", 
		WS_OVERLAPPEDWINDOW, CRect(0, 0, 700, 600));
	MyImage.LoadBitmapW(IDB_BITMAP1);
}

CCode6A::~CCode6A()
{
	for (int i = 0; i <= N; i++)
		delete f[i];
	delete f;
}

afx_msg void CCode6A::OnPaint()
{
	CPaintDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&MyImage);
	dc.BitBlt(Home.x, Home.y, 400, 320, &memDC, 0, 0, SRCCOPY);
	dc.TextOutW(440, 50, "press left button to see the rest");
}

afx_msg void CCode6A::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	int r, g, b, bw;
	dc.TextOutW(440, Home.y - 20, "extract from the original image");
	dc.TextOutW(440, 180, "gray scale by setting r=g=b");
	dc.TextOutW(20, 380, "gray scale by seting g=b=0");
	dc.TextOutW(230, 380, "gray scale by setting r=b=0");
	dc.TextOutW(440, 380, "gray scale by setting r=g=0");
	for(int j=0; j<=sImage.cx; j++)
		for (int i = 0; i <= sImage.cx; i++) {
			f[i][j] = dc.GetPixel(Home.x + i + 50, Home.y + j + 100);
			dc.SetPixel(440 + i, Home.y + j, f[i][j]);
			r = f[i][j] >> 16;
			dc.SetPixel(20 + i, 400 + j, r);
			g = r <<8;
			dc.SetPixel(230 + i, 400 + j, g);
			b = g >> 16;
			dc.SetPixel(440 + i, 400 + j, b);
			bw = r + g + b;
			dc.SetPixel(440 + i, 200 + j, bw);
		}
}