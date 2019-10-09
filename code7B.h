#pragma once
#include <afxwin.h>

//Minimum Spaning Tree

#define N 10
#define IDC_MST 500

class CCode7B : public CFrameWnd
{
private:
	CPoint TopLeft, BottomRight;
	CPoint InstText, MatText;
	CFont FontCourier;
	CSize BoxSize;
	CRect MSTbox;
	CButton MSTbutton;

	int nv, nLink, RButtonFlag, Pt1, Pt2, idc_WtInput;
	int *P, *Q, MSTcost, MinWt;
	int TextGap, TextColor, BgColor;
	typedef struct
	{
		int Wt;
		bool Flag;
		CEdit InputBox;
	} LINK;
	LINK **e;
	typedef struct {
		CPoint Home;
		CRect Box;
	} NODE;
	NODE *v;
public:
	CCode7B();
	~CCode7B() {};
	void Prim();
	afx_msg void OnClickCalc();
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