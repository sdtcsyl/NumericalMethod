#pragma once
#include <afxwin.h>
#include <math.h>

//Shortest path in a mesh network

#define Nx 10
#define Ny 5
#define N Nx*Ny
#define LinkRange 200
#define MaxPath 8

class CCode8B : public CFrameWnd
{
private:
	int Source, Dest;
	int nPath, nLink, TextGap;
	int ***alpha, LineFlag;
	typedef struct {
		CPoint Home;
		CRect rc;
	}NODE;
	NODE *v;
	typedef struct {
		int Wt, sd;
	}LINK;
	LINK **e;
	typedef struct {
		int b, e;
	}PATHLINK;
	typedef struct {
		int Distance, nPLink, st;
		PATHLINK PLink[N + 1];
	}PATH;
	PATH *Path;
	CPoint TextHome;
	CRect DrawArea, TextArea;
	CFont fontCourier, fontTimes;
	LOGFONT lfCourier, lfTimes;
public:
	CCode8B();
	~CCode8B();
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