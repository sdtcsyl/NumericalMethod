#pragma once
#include <afxwin.h>
#include "resource.h"

// Edge Detection Problem

// resource method

#define N 200

class CCode6B : public CFrameWnd
{
private:
	int **f;
	CSize sImage;
	CPoint Home;
	CBitmap bmp;
	double threshold;
public:
	CCode6B();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT, CPoint);
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};