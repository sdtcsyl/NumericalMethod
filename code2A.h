#pragma once
#include <afxwin.h>
#include <math.h>

//Bisection Method

#define N 10;	//maximum number of iterations
#define f(x) (pow(x,3)-pow(x,2)-2)
#define EPSILON 0.005

class CCode2A : public CFrameWnd
{
private:
	double *a, *b, *c;
public:
	CCode2A();
	~CCode2A();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};