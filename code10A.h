#pragma once
#include <afxwin.h>

// Discrete-Event Simulation with Blocking

#define C 10	// number of counters
#define N 100	// total number of customers
#define mLen 8	// max length of a customer
#define mniv 7	// max # incoming customers

class CCode10A : public CFrameWnd
{
private:
	LOGFONT lfTimes; CFont fontTimes;
	CPoint TopLeft, BottomRight;
	int wBar, hBar, t;	// width, height, time
	int nv, nbv;	// customer number, # blocked customers
	typedef struct
	{
		CPoint Home;	
		bool sta;	// status
		int av[N + 1], lv;	// ith customer, last customer
	}COUNTER;
	COUNTER *c;
	typedef struct
	{
		CPoint Home;	// coordinates of customer
		int len;	// length
		int ast, act;	// actual start, completion time
		int aC;		// assigned counter no.
	}CUSTOMER;
	CUSTOMER *v;
public:
	CCode10A();
	~CCode10A();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRep, UINT nFlags);
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};
