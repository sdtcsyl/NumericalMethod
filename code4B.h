#pragma once
#include <afxwin.h>

// Poisson Equation using the Finite Difference Method

#define M 3
#define N 4
#define R (M-1)*(N-1)
#define g(P,Q) ((double)10*P*Q)
#define IDC_COMPUTE 600

class CCode4B : public CFrameWnd
{
private:
	int idc_eu, idc_su;
	double h, k, **a, *b, *x, *y;
	CEdit **eu;
	CStatic **su;
	CButton bCompute;
	typedef struct
	{
		CPoint Home;
		double v;
	} NODE;
	NODE **u;

public:
	CCode4B();
	~CCode4B();
	afx_msg void OnPaint();
	afx_msg void OnCompute();
	DECLARE_MESSAGE_MAP();
	void PGauss();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};

