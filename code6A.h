#pragma once
#include <afxwin.h>
#include "resource.h"

//Working with color

// resource method

#define N 500

class CCode6A : public CFrameWnd
{
private:
	int **f;
	CSize sImage;
	CPoint Home;
	CBitmap MyImage;
public:
	CCode6A();
	~CCode6A() {};
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};