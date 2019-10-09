#pragma once
#include <afxcmn.h>
#include <math.h>

// Runge_Kutta Method for ODE

#define IDC_TABLEVIEW 300
#define nFIELDS 6
#define N 50
#define f(a,b) (pow(a,2)*cos(b)+1)

class CCode4A : public CFrameWnd
{
protected:
	double *x, *y, h;
	CString *sx, *sy;
	CString *sk1, *sk2, *sk3, *sk4;
public:
	CCode4A();
	~CCode4A();
	CListCtrl TableView;
	void CreateListColumns(), AddListItems(), RK4();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP();
};



