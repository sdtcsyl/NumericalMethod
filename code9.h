#pragma once
#include <afxwin.h>
#include <afxcmn.h>
#include <math.h>

//Task Scheduling Using Four Processors

#define N 20	// Max #nodes
#define nPE 4	//#PE
#define IDC_SCHEDULE 500
#define IDC_TASKINFO 501
#define nFIELDS 7

class CCode9 : public CFrameWnd
{
private:
	CPoint TopLeft, BottomRight;
	CFont FontCourier;
	CSize BoxSize;
	CButton TSbutton;
	CListCtrl TaskInfoView;
	int nv, RButtonFlag, Pt1, Pt2;
	int TextGap, TextColor, BgColor;

	typedef struct {
		int len;	// length
		int aPE;	// assigned Processor
		int pmm;	// path max. magnitude
		int pre[5];	//pred task, pre[0]=#pred tasks
		int preCom[5]; //comm cost of pred tasks
		int ast, act;	//ast = actual start time, act= completion time
		int hrt, lrt;	//high ready time, low ready time
		int sort;	//sorted nodes according to colevels
		bool sta;	//status
		CPoint Home, GHome;	//node coordinates in text & graphic areas
		CRect Box, GBox;	//node representation as a box
	}NODE;
	NODE *v;
	typedef struct {
		int aTS[N + 1];	// task # (aTs[0] = #tasks in Pr)
		int prt, pel;	//proc ready time, execution length
		CPoint Home;
	}PROC;
	PROC *PE;
public:
	CCode9();
	void PreScheduler(), Scheduler();
	void TaskInfo(), PMM();
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