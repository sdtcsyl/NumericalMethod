#pragma once
#include <afxwin.h>
#include <afxcmn.h>
#include <math.h>

// Channel Assignments to Mobile Users

#define IDC_vINFO 501
#define nFIELDS 8
#define R 4		//#channels per cell	
#define L 7		//#cells
#define C R*L	//#channels
#define N 90	// total #calls
#define mniv 10	//max. #incoming calls/slot
#define mLen 7	//max. length of call

class CCode11A : public CFrameWnd
{
private:
	LOGFONT lfTimes; CFont fontTimes;
	CPoint TopLeft, TextArea;
	CListCtrl vInfoView;
	int wBar, hBar, t;	// width, height, time
	int nv, tnav;	// call#, total #assg calls
	typedef struct {
		CPoint Home;	
		int hCell;	// host cell
		bool sta;	// status
		int av[N + 50], lv;		//ith, last call
	}CHANNEL;
	CHANNEL *f;
	typedef struct {
		CPoint Home;	//coordinates of call
		int art;	// arrival time
		int len;	//length
		int ast, act;	//actual start, completion time
		int Sf, Df;	//assigned channel no.
		int sta;	//state of call. 0= doesn't exist yet, 1= in queue, 2= assigned, 3= terminated
		int awt;	//actual waiting time in the queue
		int SCell, DCell;	//host cell: S = source, D = dest
	}CALL;
	CALL *v;
	typedef struct {
		int f[R + 1];	//allocated channels
		bool sep[L + 1];	//adjacency: 0=no, 1=yes
	}CELL;
	CELL *Cell;
public:
	CCode11A();
	~CCode11A();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRep, UINT nFlags);
	DECLARE_MESSAGE_MAP();
	void CallInfo();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};
