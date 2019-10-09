#include "code2B.h"

CMyWinApp MyApplication;

BOOL CMyWinApp::InitInstance()
{
	CCode2B* pFrame = new CCode2B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCode2B, CFrameWnd)
	ON_BN_CLICKED(IDC_COMPUTE, OnCompute)
END_MESSAGE_MAP()

CCode2B::CCode2B()
{
	b = new double[N + 1];
	eb = new CEdit[N + 1];
	x = new double[N + 1];
	sx = new CStatic[N + 1];
	a = new double *[N + 1];
	ea = new CEdit *[N + 1];

	for (int i = 1; i <= N; i++) {
		a[i] = new double[N + 1];
		ea[i] = new CEdit[N + 1];
	}
	
	idc_ea = 200;
	idc_eb = 300;
	idc_sx = 400;

	Create(NULL, (LPCTSTR)"SLE: Gauss Elimination Method", WS_OVERLAPPEDWINDOW, CRect(0, 0, 500, 340));
	bCompute.Create((LPCTSTR)"Compute", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
		CRect(CPoint(380, 25), CSize(100, 25)), this, IDC_COMPUTE);

	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
			ea[i][j].Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
				CRect(CPoint(10 + (i - 1) * 70, 80 + (j - 1) * 30),
					CSize(60, 25)), this, idc_ea++);

		eb[i].Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
			CRect(CPoint(280, 80 + (i - 1) * 30), CSize(60, 25)),
			this, idc_eb++);
		sx[i].Create((LPCTSTR)"", WS_CHILD | WS_VISIBLE | SS_SUNKEN | SS_CENTER, CRect(CPoint(380, 80 + (i - 1) * 30),
				CSize(80, 25)), this, idc_sx++);
	}

	ea[1][1].SetFocus();	
}

CCode2B::~CCode2B() {
	for (int i = 1; i <= N; i++)
		delete a[i], ea[i];

	delete a, ea, b, x, sx;
}

void CCode2B::OnCompute()
{
	CString s;
	int i, j;
	for (i = 1; i < N; i++) {
		for (j = 1; j < +N; j++) {
			ea[i][j].GetWindowTextW(s);
			a[i][j] = atof(s);
		}
		eb[i].GetWindowTextW(s);
		b[i] = atof(s);
	}
	PGauss();
	for (i = 1; i <= N; i++) {
		s.Format("%lf", x[i]);
		sx[i].SetWindowTextW(s);
	}
}

void CCode2B::PGauss()
{
	int i, j, k;
	double m, Sum;
	for(k=1;k<=N-1; k++)
		for (i = k + 1; i <= N; i++)
		{
			m = a[i][k] / a[k][k];
			for (j = 1; j <= N; j++)
				a[i][j] = a[i][j] - m * a[k][j];
			b[i] = b[i] - m * b[k];
		}
	for (i = N; i >= 1; i--)
	{
		Sum = 0;
		x[i] = 0;
		for (j = i; j <= N; j++)
			Sum += a[i][j]*x[j];
		x[i] = (b[i] - Sum) / a[i][i];
	}
}