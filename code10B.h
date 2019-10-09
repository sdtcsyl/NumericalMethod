#pragma once
#include <afxwin.h>
#include <afxcmn.h>
#include <math.h>

// M/M/C Queueing without Blocking
#define IDC_vINFO 501
#define nFIELDS 6
#define C 10	// number of counters
#define N 60	// total number of customers
#define mLen 8	// max length of a customer
#define mniv 5	// max # incoming customers

class CCode10B : public CFrameWnd
{
private:
	LOGFONT lfTimes; CFont fontTimes;
	CPoint TopLeft, BottomRight;
	CListCtrl vInfoView;
	int wBar, hBar, t;	// width, height, time
	int nv, tnav;	// customer number, tot # assigned customers
	typedef struct
	{
		CPoint Home;
		bool sta;	// status
		int av[N + 50], lv;	// ith customer, last customer
	}COUNTER;
	COUNTER *c;
	typedef struct
	{
		CPoint Home;	// coordinates of customer
		int len;	// length
		int art;	// arrival time
		int ast, act;	// actual start, completion time
		int aC;		// assigned counter no.
		int sta;	// status, 0 = not arr., 1 = arr.assg., 2 = arr. not assg., 99 = dead.,
		int wait;	//waiting time in the queue
	}CUSTOMER;
	CUSTOMER *v;
public:
	CCode10B();
	~CCode10B();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRep, UINT nFlags);
	DECLARE_MESSAGE_MAP();
	void CustomerInfo(), Analysis();
	double factorial(double);
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};
