#pragma once
#include <afxwin.h>
#include <math.h>
#include "resource.h"

// Channel Assignments to Mobile Users


#define L 19	//#cells


class CCode11B : public CFrameWnd
{
private:
	LOGFONT lfTimes; CFont fontTimes;
	CPoint TopLeft, TextArea;
	CBitmap h2;
	typedef struct {
		int f;	//allocated channels
		bool adj[L + 1];	//adjacency: 0=no, 1=yes
		bool coc[L + 1];	//co-channel: 0=no, 1=yes
		CPoint home;
	}CELL;
	CELL *Cell;
public:
	CCode11B();
	~CCode11B();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};
