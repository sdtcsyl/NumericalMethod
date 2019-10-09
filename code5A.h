#pragma once
#include <afxwin.h>
#include <math.h>

//Mathematical Curves

class CCode5A : public CFrameWnd
{
private:
	CPoint TopLeft, BottomRight, Origin;
	CPoint pt1, pt2, pt3;
	CRect DrawRc, rc1, rc2, rc3;
	int Color1, Color2;
	char MenuChoice;
public:
	CCode5A();
	~CCode5A() {};
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	void PolyNomial(), Lemniscate(), CreativeNet(), DrawAxes();
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};