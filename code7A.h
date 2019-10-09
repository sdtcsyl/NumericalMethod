#pragma once
#include <afxwin.h>

//Drawing a graph

#define N 10

class CCode7A : public CFrameWnd
{
private:
	CPoint TopLeft, BottomRight;
	CPoint InstText, MatText;
	CFont FontCourier;
	CSize BoxSize;
	int nv, nLink, RButtonFlag, Pt1, Pt2;
	int TextGap, TextColor, BgColor;
	int **e;
	typedef struct {
		CPoint Home;
		CRect Box;
		int degree;
	} NODE;
	NODE *v;
public:
	CCode7A();
	~CCode7A() {};
	void Prim();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};