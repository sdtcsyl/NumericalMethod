#pragma once

#include <afxwin.h>
#include<afxdisp.h>
#include "resource.h"

//Resource File Approach to the SLE Problem

// at first, users have to design the dialog in the .rc file in resource.files folder

#define N 3

class CCode2C : public CDialog
{
protected:
	double **a, *b, *x;
	CString **sa, *sb, *sx;

public:
	CCode2C(CWnd* pParent = NULL);
	~CCode2C();
	enum{ IDD = IDD_GAUSSDIALOG};
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnCompute();
	DECLARE_MESSAGE_MAP();
	void PGauss();
};

class CMyWinApp : public CWinApp
{
public:
	virtual BOOL InitInstance();
};