#include "code9.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode9* pFrame = new CCode9;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode9, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_SCHEDULE, OnClickCalc)
END_MESSAGE_MAP()

CCode9::CCode9()
{
	TopLeft = CPoint(20, 10); BottomRight = CPoint(900, 32);
	Create(NULL, (CString)"Multiprocessor Task Scheduling", WS_OVERLAPPEDWINDOW, CRect(0, 0, 920, 630));
	TSbutton.Create((CString)"Task Scheduler", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 
		CRect(30,325,250,355), this, IDC_SCHEDULE);
	TaskInfoView.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
		CRect(BottomRight.x-200, TopLeft.y+10, BottomRight.x-10, BottomRight.y-10), this, IDC_TASKINFO);
	char* column[nFIELDS + 1] = { "Task","PE","Len","AST","CT","Pre","pmm","Order" };
	int columnWidth[nFIELDS + 1] = { 40,40,40,40,40,40,40,40 };
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 85;
	for (int i = 0; i <= nFIELDS; i++)
	{
		lvColumn.iSubItem = 0;
		lvColumn.pszText = column[i];
		TaskInfoView.InsertColumn(i, &lvColumn);
		TaskInfoView.SetColumnWidth(i, columnWidth[i]);
	}
	v = new NODE[N + 1];
	PE = new PROC[nPE + 1];

	nv = 0; RButtonFlag = 0;
	FontCourier.CreatePointFont(60, (CString)"Courier");
	BgColor = RGB(240, 240, 240);
	TextGap = 25; BoxSize = CSize(10, 10);
	TextColor = RGB(100, 100, 100);
	for (int k = 1; k <= nPE; k++) {
		PE[k].aTS[0] = 0;
		PE[k].prt = 0;
		PE[k].pel = 0;
		PE[k].Home = CPoint(20,360+(k-1)*30);
	}
}

void CCode9::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	CString s;
	CPen penBlue(PS_SOLID, 5, RGB(0, 0, 255));
	CBrush BgBrush(BgColor);
	GetClientRect(&rc);
	dc.FillRect(&rc, &BgBrush);

	CPen penDrawingBox(PS_SOLID, 4, RGB(100, 100, 100));
	dc.SelectObject(penDrawingBox);
	dc.SelectStockObject(HOLLOW_BRUSH);
	rc = CRect(TopLeft, BottomRight);
	dc.Rectangle(rc);

	dc.SelectObject(&penBlue);
	dc.SetTextColor(RGB(255, 255, 255));
	for (int k = 1; k <= nPE; k++)
	{
		rc = CRect(PE[k].Home, CSize(25, 25));
		dc.FillSolidRect(&rc, RGB(100, 100, 100));
		s.Format((CString)"%d", k);
		dc.TextOutW(PE[k].Home.x + 8, PE[k].Home.y + 5, s);
	}
}

void CCode9::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	CString s;
	CPen penGray(PS_SOLID, 2, TextColor);
	dc.SelectObject(penGray);

	dc.SelectObject(FontCourier);
	dc.SetTextColor(TextColor); dc.SetBkColor(BgColor);
	if (CRect(TopLeft, BottomRight).PtInRect(pt))
		if (nv <= N)
		{
			nv++;
			v[nv].Home = pt;
			v[nv].pre[0] = 0;
			v[nv].ast = 0;
			v[nv].act = 0;
			v[nv].sta = 0;	//Set node status to inactive
			v[nv].Box = CRect(CPoint(pt), CSize(BoxSize));
			dc.Rectangle(v[nv].Box);
			s.Format((CString)"%d", nv);
			dc.TextOutW(v[nv].Home.x, v[nv].Home.y - 15, s);
			v[nv].len = 1 + rand() % 5;
			s.Format((CString)"%d", v[nv].len);
			dc.TextOutW(v[nv].Home.x, v[nv].Home.y + 10, s);
		}
}


void CCode9::OnRButtonDown(UINT nFlags, CPoint pt)
{
	CClientDC dc(this);
	CString s;

	int i, u, w, r;
	CPen penGray(PS_SOLID, 1, TextColor);
	dc.SelectObject(penGray);
	time_t seed = time(NULL);
	srand((unsigned)seed);
	dc.SelectObject(FontCourier);
	dc.SetTextColor(BgColor);
	for (i = 1; i <= nv; i++) 
		if (v[i].Box.PtInRect(pt))
		{
			RButtonFlag++;
			if (RButtonFlag == 1)
				Pt1 = i;
			if (RButtonFlag == 2)
			{
				Pt2 = i;
				RButtonFlag = 0;
				r = ++v[Pt2].pre[0];
				v[Pt2].pre[r] = Pt1;
				v[Pt2].preCom[r] = 1 + rand() % 5;
				dc.MoveTo(v[Pt1].Home);
				dc.LineTo(v[Pt2].Home);
				u = (v[Pt1].Home.x + v[Pt2].Home.x) / 2;
				w = (v[Pt1].Home.y + v[Pt2].Home.y) / 2;
				s.Format((CString)"%d", v[Pt2].preCom[r]);
				dc.TextOutW(u, w, s);			
			}
		}
}

void CCode9::OnClickCalc()
{
	PMM();	//Sort the nodes according to their colevel values
	PreScheduler();		//initialize each PE with 1st Node
	Scheduler();	//the scheduler
	TaskInfo();		//display task information
}

void CCode9::TaskInfo()
{
	CString s;
	TaskInfoView.DeleteAllItems();
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	for (int i = 0; i <= nv; i++)
	{
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = "";
		TaskInfoView.InsertItem(&lvItem);
		if (i + 1 < nv && i + 1 <= N)
		{
			s.Format((CString)"%d", i + 1); TaskInfoView.SetItemText(i, 0, s);
			s.Format((CString)"%d", v[i + 1].aPE); TaskInfoView.SetItemText(i, 1, s);
			s.Format((CString)"%d", v[i + 1].len); TaskInfoView.SetItemText(i, 2, s);
			s.Format((CString)"%d", v[i + 1].ast); TaskInfoView.SetItemText(i, 3, s);
			s.Format((CString)"%d", v[i + 1].act); TaskInfoView.SetItemText(i, 4, s);
			s.Format((CString)"%d", v[i + 1].pre[0]); TaskInfoView.SetItemText(i, 5, s);
			s.Format((CString)"%d", v[i + 1].pmm); TaskInfoView.SetItemText(i, 6, s);
			s.Format((CString)"%d", v[i + 1].sort); TaskInfoView.SetItemText(i, 7, s);
		}
	}
}

void CCode9::PMM() //Sort the tasks according to levels
{
	int i, j, k, r, tmp;
	for (i = 1; i <= nv; i++) {
		v[i].sort = i;
		if (v[i].pre[0] == 0)
			v[i].pmm = 0;
		else
		{
			r = v[i].pre[1];
			tmp = v[r].len + v[i].preCom[1] + v[r].pmm;
			for (j = 1; j <= v[i].pre[0]; j++)
			{
				r = v[i].pre[j];
				tmp = (tmp < v[r].len + v[i].preCom[j] + v[r].pmm) ?
					v[r].len + v[i].preCom[j] + v[r].pmm : tmp;
			}
			v[i].pmm = tmp;
		}
		for (k = 1; k <= i - 1; k++)
		{
			r = v[k].sort;
			if (v[i].pmm < v[r].pmm) {
				for (j = i; j > k + 1; j--)
					v[j].sort = v[j - 1].sort;
				v[k].sort = i;
				break;
			}
		}
	}
}

void CCode9::PreScheduler()
{
	int i, j, u, k, AsPE, r, tmp;
	for (i = 1; i <= nv; i++) {
		u = v[i].sort;
		if (v[u].pre[0] == 0)
		{
			if (i <= nPE)
			{
				AsPE = i;
				v[u].aPE = AsPE;
				v[u].ast = 0;
				v[u].act = v[u].ast +v[u].len;
				v[u].sta = 1;
				PE[AsPE].aTS[0]++;
				PE[AsPE].aTS[1] = u;
				PE[AsPE].prt = v[u].act;
				PE[AsPE].pel += v[u].len;
			}
			else
			{
				tmp = PE[1].prt;
				AsPE = 1;
				for(k=1; k<=nPE;k++)
					if (tmp > PE[k].prt)
					{
						tmp = PE[k].prt;
						AsPE = k;
					}
				v[u].aPE = AsPE;
				v[u].ast = tmp;
				v[u].act = v[u].ast + v[u].len;
				v[u].sta = 1;
				r = ++PE[AsPE].aTS[0];
				PE[AsPE].aTS[r] = u;
				PE[AsPE].prt = v[u].act;
				PE[AsPE].pel += v[u].len;

			}
		}
		else
			if (i <= nPE)
			{
				AsPE = i;
				v[u].aPE = AsPE;
				r = v[u].pre[1];
				tmp = v[r].act + v[u].preCom[1];
				for (j = 1; j <= v[u].pre[0]; j++)
				{
					r = v[u].pre[j];
					if (tmp < v[r].act + v[u].preCom[j])
						tmp = v[r].act + v[u].preCom[i];
				}
				v[u].ast = tmp;
				v[u].act = v[u].ast + v[u].len;
				v[u].sta = 1;
				PE[AsPE].aTS[0]++;
				PE[AsPE].aTS[r] = u;
				PE[AsPE].prt = v[u].act;
				PE[AsPE].pel += v[u].len;
			}
	}
}

void CCode9::Scheduler()
{
	CClientDC dc(this);
	CString s;
	int u, i, j, k, r, m, AsPE;
	int HiDom, HiDomIndex, HiDomPE, LoDom, LoDomIndex, tmp;
	CPen penGray(PS_SOLID, 1, TextColor);

	dc.SelectObject(penGray);
	for (j = 1; j <= nv; j++)
	{
		u = v[j].sort;
		if (!v[u].sta)
		{
			//calculate hrt
			HiDomIndex = 1;
			HiDom = v[u].pre[HiDomIndex];
			v[u].hrt = v[HiDom].act + v[u].preCom[HiDomIndex];
			for (i = 1; i <= v[u].pre[0]; i++)
			{
				r = v[u].pre[i];
				if (v[u].hrt < v[r].act + v[u].preCom[i])
				{
					v[u].hrt = v[r].act + v[u].preCom[i];
					HiDom = r; HiDomIndex = i;
				}
			}

			//calculate lrt
			if (v[u].pre[0] == 1)
				v[u].lrt = v[u].hrt;
			else
			{
				LoDomIndex = ((HiDomIndex == 1) ? 2 : 1);
				LoDom = v[u].pre[LoDomIndex];
				v[u].lrt = v[LoDom].act + v[u].preCom[LoDomIndex];
				for (i = 1; i <= v[u].pre[0]; i++)
					if (i != HiDomIndex)
					{
						r = v[u].pre[i];
						if (v[u].lrt < v[r].act + v[u].preCom[i])
							v[u].lrt = v[r].act + v[u].preCom[i];
					}
			}

			HiDomPE = v[HiDom].aPE;
			if (PE[HiDomPE].prt <= v[u].hrt)
			{
				AsPE = HiDomPE;
				v[u].aPE = AsPE;
				if (v[u].pre[0] == 1)
					v[u].ast = PE[HiDom].prt;
				else
					v[u].ast = ((PE[HiDomPE].prt >= v[u].lrt) ? PE[HiDomPE].prt : v[u].lrt);
				v[u].act = v[u].ast + v[u].len;
				v[u].sta = 1;
				PE[AsPE].prt = v[u].act;
				r = ++PE[AsPE].aTS[0];
				PE[AsPE].aTS[r] = u;
				PE[AsPE].pel += v[u].len;
			}
			else
			{
				AsPE = ((HiDomPE == 1) ? 2 : 1);
				tmp = PE[AsPE].pel;
				for(k=1;k<=nPE;k++)
					if(k!=HiDomPE)
						if (tmp > PE[k].pel)
						{
							AsPE = k;
							tmp = PE[k].pel;
						}
				v[i].aPE = AsPE;
				v[u].ast = ((PE[AsPE].prt >= v[u].hrt) ? PE[AsPE].prt : v[u].hrt);
				v[u].act = v[u].ast + v[u].len;
				v[u].sta = 1;
				PE[AsPE].prt = v[u].act;
				r = ++PE[AsPE].aTS[0];
				PE[AsPE].aTS[r] = u;
				PE[AsPE].pel += v[u].len;
			}			
		}
	}

	dc.SelectObject(FontCourier);
	dc.SetTextColor(TextColor);
	for (i = 1; i <= nv; i++)
	{
		m = v[i].aPE;
		v[i].GHome = CPoint(30 + PE[m].Home.x + 20 * v[i].ast, PE[m].Home.y);
		v[i].GBox = CRect(v[i].GHome.x, v[i].GHome.y, v[i].GHome.x + 20 * v[i].len, v[i].GHome.y + 25);
		dc.Rectangle(&v[i].GBox);
		s.Format((CString)"%d", i); dc.TextOutW(2 + v[i].GHome.x, 5 + v[i].GHome.y, s);
	}
}