#pragma once
#include <afxwin.h>
#include <math.h>

//Shortest path in a graph

#define N 20
#define LinkRange 200

class CCode8A : public CFrameWnd
{
private:
	int Source, Dest;
	int Color1, Color2, BgColor;
	int ***alpha, LineFlag;
	typedef struct {
		int Wt, sd;
		int via[10];
	}LINK;
	LINK **e;
	typedef struct {
		CPoint Home;
		CRect rc;
	}NODE;
	NODE *v;
	CPoint TopLeft, BottomRight;
	CFont fontArial, fontCourier;
	LOGFONT lfCourier, lfArial;
public:
	CCode8A();
	~CCode8A();
	void DisplayGraph();
	void ComputePath();
	void DrawPath();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};