#include "code6B.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode6B* pFrame = new CCode6B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode6B, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CCode6B::CCode6B()
{
	f = new int *[N + 1];
	for (int i = 0; i <= N; i++)
		f[i] = new int[N + 1];
	Home = CPoint(10, 50);
	sImage = CSize(200, 100);
	Create(NULL, (LPCTSTR)"Edge detections using Sobel and Laplacian Methods",
		WS_OVERLAPPEDWINDOW, CRect(0, 0, 700, 600));
	bmp.LoadBitmapW(IDB_BITMAP1);
}

afx_msg void CCode6B::OnPaint()
{
	CPaintDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bmp);
	dc.BitBlt(Home.x, Home.y, 400, 300, &memDC, 0, 0, SRCCOPY);
	for(int j =0; j<= sImage.cx; j++)
		for (int i = 0; i <= sImage.cx; i++) {
			f[i][j] = dc.GetPixel(Home.x + i + 50, Home.y + j + 100);
			threshold += (double)f[i][j];
		}
	threshold /= (double)(N*N);
}

afx_msg void CCode6B::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	int Sy, Sx, S, L;
	
	dc.TextOutW(440, Home.y - 40, "Sobel Mtd");
	dc.TextOutW(440, Home.y - 10 + sImage.cy, "Laplacian 1 Mtd");
	dc.TextOutW(440, Home.y - 20 + 2* sImage.cy, "Laplacian 2 Mtd");

	for(int j =1; j<=sImage.cy-1; j++)
		for (int i = 1; i < +sImage.cx - 1; i++) 
		{
			//Sobel masking for detecting the edges
			Sy = f[i - 1][j + 1] - f[i + 1][j + 1] + 2 * f[i - 1][j] //horizontal kernel
				- 2 * f[i + 1][j] + f[i - 1][j - 1] - f[i + 1][j - 1];
			Sx = f[i - 1][j + 1] - 2 * f[i][j + 1] - f[i + 1][j + 1] //vertical kernel
				+ f[i - 1][j - 1] + 2 * f[i][j - 1] + f[i + 1][j - 1];
			S = abs(Sy) + abs(Sx);
			if (S <= (int)threshold)
				S = RGB(0, 0, 0);
			else
				S = RGB(255, 255, 255);
			dc.SetPixel(440 + i, Home.y - 25 + j, S);

			//Laplacian masking for detecting the edge
			L = -f[i][j + 1] - f[i - 1][j]
				+ 4 * f[i][j] - f[i + 1][j] - f[i][j - 1]; //Kernel 1

			L = abs(L);
			if (L <= (int)threshold)
				L = RGB(0, 0, 0);
			else
				L = RGB(255, 255, 255);
			dc.SetPixel(440 + i, Home.y + 5 + sImage.cy + j, L);

			L = -f[i - 1][j + 1] - f[i][j + 1] - f[i + 1][j + 1] // kernel 2
				- f[i - 1][j] + 8 * f[i][j] - f[i + 1][j]
				- f[i - 1][j - 1] - f[i][j - 1] - f[i + 1][j - 1];

			L = abs(L);
			if (L <= (int)threshold)
				L = RGB(0, 0, 0);
			else
				L = RGB(255, 255, 255);
			dc.SetPixel(440 + i, Home.y + 35 + 2 * sImage.cy + j, L);
		}
}
