#pragma once
#include <afxwin.h>
#include <math.h>

//Natural Cubic Spline Curves

#define M 10
#define IDC_PUSHBUTTON 301

class CCode5B : public CFrameWnd
{
private:
	CPoint TopLeft, BottomRight, Origin;
	double *a, *b, *c, *d;
	double *X, *Y;
	int m, PushButtonClick;
	int Color1, Color2;
	CButton PushButton;
	CRect PushButtonRect;

public:
	CCode5B();
	~CCode5B() {};
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnClickCalc();
	void ComputeCSpline();
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};