#include "code11A.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode11A* pFrame = new CCode11A;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode11A, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CCode11A::CCode11A()
{	
	int i, j, k, r, w;
	TopLeft = CPoint(0, 0);
	TextArea = CPoint(750, 10);
	f = new CHANNEL[C + 1];
	v = new CALL[N + 1];
	Cell = new CELL[L + 1];

	Create(NULL, (CString)"Cellular telephone system simulation", WS_OVERLAPPEDWINDOW, CRect(0,0,1000,660));
	
	//channel and call initialization
	t = -1; tnav = 0; nv = 0;
	for (int k = 1; k <= C; k++)
	{
		f[k].sta = 0;
		f[k].av[0] = 0;
	}
	for (int i = 1; i <= N; i++)
		v[i].sta = 0;

	//call assignment table
	vInfoView.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
		CRect(TextArea.x, TextArea.y, TextArea.x + 230, TextArea.y + 330), this, IDC_vINFO);
	char* column[nFIELDS + 1] = { "Call","S","Sf","D","Dft","Arr","Ast","Act","Sta" };
	int columnWidth[nFIELDS + 1] = { 40,40,40,40,40,40,40,40,40 };
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_FMT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 85;
	for (int i = 0; i <= nFIELDS; i++)
	{
		lvColumn.iSubItem = 0;
		lvColumn.pszText = column[i];
		vInfoView.InsertColumn(i, &lvColumn);
		vInfoView.SetColumnWidth(i, columnWidth[i]);
	}

	FILE *InFile;
	InFile = fopen("Code11A.in", "r");
	for (i = 1; i <= L; i++)
	{
		for (j = 1; j <= i; j++)
		{
			fscanf(InFile, "%d", &Cell[i].sep[j]); // read cell adjacency info
			Cell[j].sep[i] = Cell[i].sep[j];
		}
		for (r = 1; r <= R; r++)
		{
			w = Cell[i].f[r] = i + 7 * (r - 1); // allocate channels to cells
			f[w].hCell = i;
		}
	}
	fclose(InFile);

	wBar = 18; hBar = 15;
	::ZeroMemory(&lfTimes, sizeof(lfTimes));
	lfTimes.lfHeight = 80;
	fontTimes.CreatePointFontIndirect(&lfTimes);

	
}

CCode11A::~CCode11A()
{
	delete f, v, Cell;
}


void CCode11A::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	CString s;

	dc.SelectObject(fontTimes);
	dc.SetTextColor(RGB(255, 255, 255));
	for (int k = 1; k <= C; k++)
	{
		f[k].Home.x = 25; f[k].Home.y = 35 + (k - 1) * (hBar+6);
		rc = CRect(f[k].Home, CSize(wBar + 5, hBar));
		dc.FillSolidRect(&rc, RGB(150, 150, 150));
		s.Format((CString)"%d", k);
		dc.TextOutW(f[k].Home.x + 7, f[k].Home.y + 2, s);
	}
	dc.SetBkColor(RGB(255, 255, 255));
	dc.SetTextColor(RGB(100, 100, 100));
	s.Format((CString)"Channels: %d  Max. #Calls: %d", C, N);
	dc.TextOutW(TextArea.x, TextArea.y+330, s);

	rc = CRect(TextArea.x, TextArea.y + 480, TextArea.x + 230, TextArea.y + 600);
	dc.Rectangle(&rc);
}

void CCode11A::OnKeyDown(UINT nChar, UINT nRep, UINT nFlags)
{
	CPaintDC dc(this);
	CRect rc;
	CString s;
	CBrush* pWhite = new CBrush(RGB(255, 255, 255));
	CPen SPen(PS_SOLID, 1, RGB(0, 0, 200));
	CPen DPen(PS_SOLID, 1, RGB(200, 0, 0));
	int niv, nav;	//#inc calls, #avail calls
	int i, j, k, p, q, r, u, w, z;

	j = 0;  // initialize #calls in Q
	time_t seed = time(NULL);	srand((unsigned)seed);

	if (nChar == VK_SPACE && v[N].sta != 3)
	{
		dc.SelectObject(fontTimes);
		dc.SetBkColor(RGB(255, 255, 255));
		dc.SetTextColor(RGB(0, 0, 0));
		s.Format((CString)"t=%d", ++t);
		dc.TextOutW(TextArea.x, TextArea.y, s);
		s.Format((CString)"%d", t);
		dc.TextOutW(TextArea.x+5, TextArea.y+350, s);
		rc = CRect(TextArea.x+5, TextArea.y + 490, TextArea.x + 225, TextArea.y + 590);
		dc.FillRect(&rc, pWhite);

		for (k = 1; k <= C; k++)
			if (f[k].av[0] >= 1)
			{
				int r = f[k].lv;
				if (v[r].act <= t)
					f[k].sta = 0;
			}

		nav = 0;
		niv = rand() % mniv;
		dc.SetTextColor(RGB(150, 150, 150));

		for (i = 1; i <= niv; i++)
		{
			nv++;
			if (nv > N)
				nv = N;
			v[nv].art = t;
			v[nv].sta = 1;
			v[nv].SCell = 1 + rand() % L;
			v[nv].DCell = 1 + rand() % L;
		}

		for (i = 1; i <= nv; i++)
		{
			u = w = 0;
			if (v[i].sta == 1)
			{
				v[i].Sf = 0; p = v[i].SCell;
				for (k = 1; k <= C; k++)
				{
					z = f[k].hCell;
					if (!f[k].sta && Cell[p].sep[z])
					{
						u = k;
						v[i].Sf = u;
						f[u].sta = 1;
						break;
					}
					if (k == C && !f[C].sta)
						u = 0;
				}
				v[i].Df = 0; q = v[i].DCell;
				if(f[u].sta && u>0)
					for (k = 1; k <= C; k++)
					{
						r = f[k].hCell;
						if (!f[k].sta && Cell[q].sep[r])
						{
							w = k; nav++; tnav++;
							v[i].Df = w;
							v[i].len = 1 + rand() % mLen;
							v[i].ast = t;
							v[i].act = t + v[i].len;
							v[i].sta = 2;
							v[i].awt = v[i].ast - v[i].art;
							f[w].sta = 1;
							f[w].lv = f[u].lv = i;

							z = ++f[w].av[0]; f[w].av[z] = i;
							v[i].Home = CPoint(f[w].Home.x + 30 + t * wBar, f[w].Home.y);
							rc = CRect(v[i].Home, CSize(wBar*v[i].len, hBar));
							dc.SelectObject(&DPen);
							dc.Rectangle(rc);
							s.Format((CString)"%d", i);
							dc.TextOutW(2 + v[i].Home.x+30+t*wBar, 1+v[i].Home.y, s);

							z = ++f[w].av[0]; f[u].av[z] = i;
							v[i].Home = CPoint(f[u].Home.x + 30 + t * wBar, f[u].Home.y);
							rc = CRect(v[i].Home, CSize(wBar*v[i].len, hBar));
							dc.SelectObject(&SPen);
							dc.Rectangle(rc);
							s.Format((CString)"%d", i);
							dc.TextOutW(2 + v[i].Home.x + 30 + t * wBar, 1 + v[i].Home.y, s);
							break;
						}
						if (k == C && !f[C].sta)
							w = 0;
					}
				if(u==0 || w==0)
				{
					j++;
					v[i].Sf = v[i].Df = 0;
					v[i].sta = 1;
					s.Format((CString)"%d", i);
					dc.TextOutW(TextArea.x + 20 +((j-1)/8)*30, TextArea.y + 510 + ((j - 1) / 8) * 10, s);

				}
			}
			if (t >= v[i].act && v[i].sta == 2)
				v[i].sta = 3;
		}
		dc.SetTextColor(RGB(0, 0, 0));
		s.Format((CString)"Last Assigned Call : %d", tnav);
		dc.TextOutW(TextArea.x, TextArea.y+370, s);
		s.Format((CString)"Total Arrivals : %d", nv);
		dc.TextOutW(TextArea.x, TextArea.y + 390, s);
		s.Format((CString)"#Arrivals at timeslot : %d", niv);
		dc.TextOutW(TextArea.x, TextArea.y + 410, s);
		s.Format((CString)"#Assigned at timeslot : %d", nav);
		dc.TextOutW(TextArea.x, TextArea.y + 430, s);
		s.Format((CString)"Calls in Q : %d", j);
		dc.TextOutW(TextArea.x, TextArea.y + 490, s);
		
		
		CallInfo();
	}
	if (v[N].sta == 2)
	{
		v[N].sta = 3;
		return;
	}
}

void CCode11A::CallInfo()
{
	CString s;
	vInfoView.DeleteAllItems();
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_STATE;
	lvItem.state = 0;
	lvItem.stateMask = 0;
	for (int i = 0; i <= tnav; i++)
	{
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.pszText = "";
		vInfoView.InsertItem(&lvItem);
		if (i + 1 < tnav && i + 1 <= N)
		{
			s.Format((CString)"%d", i + 1);	vInfoView.SetItemText(i, 0, s);
			s.Format((CString)"%d", v[i + 1].SCell); vInfoView.SetItemText(i, 1, s);
			s.Format((CString)"%d", v[i + 1].Sf);  vInfoView.SetItemText(i, 2, s);
			s.Format((CString)"%d", v[i + 1].DCell);  vInfoView.SetItemText(i, 3, s);
			s.Format((CString)"%d", v[i + 1].Df); 	vInfoView.SetItemText(i, 4, s);
			s.Format((CString)"%d", v[i + 1].art);	vInfoView.SetItemText(i, 5, s);
			s.Format((CString)"%d", v[i + 1].ast);	vInfoView.SetItemText(i, 6, s);
			s.Format((CString)"%d", v[i + 1].act);	vInfoView.SetItemText(i, 7, s);
			s.Format((CString)"%d", v[i + 1].sta);	vInfoView.SetItemText(i, 8, s);
		}
	}
}