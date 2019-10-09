#pragma once
#include <afxcmn.h>
#include <math.h>
#include <stdafx.h>

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


BEGIN_MESSAGE_MAP(CCode4A, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CCode4A::CCode4A()
{
	x = new double[N + 1];
	y = new double[N + 1];
	sx = new CString[N + 1];
	sy = new CString[N + 1];
	sk1 = new CString[N + 1];
	sk2 = new CString[N + 1];
	sk3 = new CString[N + 1];
	sk4 = new CString[N + 1];
	x[0] = 0;
	y[0] = -1;
	h = 0.1;
	Create(NULL, (LPCTSTR)"Runge-Kutta Mehod of Order 4");
	TableView.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER, CRect(30, 50, 550, 400), this, IDC_TABLEVIEW);
	CreateListColumns();
	RK4();
	AddListItems();
}

CCode4A::~CCode4A()
{
	delete x, y, sx, sy, sk1, sk2, sk3, sk4;
}

void CCode4A::OnPaint()
{
	CPaintDC dc(this);
	dc.TextOutW(30, 20, (CString)"Solutions to the problem dy/dx=pow(x,2)cos(y)+1,y(0)=-1");	//TextOut
}

void CCode4A::CreateListColumns()
{
	char* column[nFIELDS + 1] = { "i","x","y","k1","k2","k3","k4" };
	int columnWidth[nFIELDS + 1] = { 40, 40, 80, 80, 80, 80, 80 };
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 85;
	for (int i = 0; i <= nFIELDS; i++) {
		lvColumn.iSubItem = 0;
		lvColumn.pszText = (LPWSTR)column[i]; //without LPWSTR
		TableView.InsertColumn(i, &lvColumn);
		TableView.SetColumnWidth(i, columnWidth[i]);
	}
}

void CCode4A::AddListItems()
{
	CString iteration;
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	for (int i = 0; i <= N; i++)
	{
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = (LPWSTR) "";//without LPWSTR
		iteration.Format(_T("%d"), i); //without _T()
		TableView.InsertItem(&lvItem);
		TableView.InsertItem(&lvItem);
		TableView.SetItemText(i, 0, iteration);
		TableView.SetItemText(i, 1, sx[i]);
		TableView.SetItemText(i, 2, sy[i]);
		TableView.SetItemText(i, 3, sk1[i]);
		TableView.SetItemText(i, 4, sk2[i]);
		TableView.SetItemText(i, 5, sk3[i]);
		TableView.SetItemText(i, 6, sk4[i]);
	}
}

void CCode4A::RK4()
{
	int i;
	double k1, k2, k3, k4;
	for (i = 0; i < N; i++)
	{
		k1 = h * f(x[i], y[i]);
		k2 = h * f(x[i] + h / 2, y[i] + k1 / 2);
		k3 = h * f(x[i] + h / 2, y[i] + k2 / 2);
		k4 = h * f(x[i] + h, y[i] + k3);
		y[i + 1] = y[i] + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		x[i + 1] = x[0] + (double)(i + 1)*h;	//increment for x
		//convert the values to strings
		sx[i].Format(_T("%.1lf"), x[i]);	//without _T()
		sy[i].Format(_T("%.lf"), y[i]);		//without _T()
		sk1[i].Format(_T("%.lf"), k1);		//without _T()
		sk2[i].Format(_T("%.lf"), k2);		//without _T()
		sk3[i].Format(_T("%.lf"), k3);		//without _T()
		sk4[i].Format(_T("%.lf"), k4);		//without _T()
	}
}

