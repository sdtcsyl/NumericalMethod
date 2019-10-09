#pragma once
#include <afxwin.h>

//solving a system of linear equations

#define IDC_COMPUTE 500
#define N 3

class CCode2B : public CFrameWnd
{
private:
	int idc_ea, idc_eb, idc_sx;
	double **a, *x, *b;
	CEdit **ea, *eb;
	CStatic *sx;
	CButton bCompute;
public:
	CCode2B();
	~CCode2B();
	void PGauss();
	afx_msg void OnCompute();
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};