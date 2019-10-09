#include "code10B.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode10B* pFrame = new CCode10B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode10B, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CCode10B::CCode10B()
{
	v = new CUSTOMER[N + 1];
	c = new COUNTER[C + 1];
	TopLeft = CPoint(0, 0); 
	BottomRight = CPoint(900, 620);
	t = -1; tnav = 0; nv = 0;
	for (int k = 1; k <= C; k++)
	{
		c[k].sta = 0;
		c[k].av[0] = 0;
	}
	for (int i = 1; i <= N; i++)
		v[i].sta = 0;
	wBar = 20; hBar = 20;
	::ZeroMemory(&lfTimes, sizeof(lfTimes));
	lfTimes.lfHeight = 80;
	fontTimes.CreatePointFontIndirect(&lfTimes);

	Create(NULL, (CString)"m/m/c queueeing system simulation", WS_OVERLAPPEDWINDOW, CRect(TopLeft, BottomRight));
	vInfoView.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER,
		CRect(BottomRight.x - 200, TopLeft.y + 10, BottomRight.x - 10, BottomRight.y - 10), this, IDC_vINFO);
	char* column[nFIELDS + 1] = { "Cust","Ct","Len","Arr","Ast","Act","Sta" };
	int columnWidth[nFIELDS + 1] = { 40,40,40,40,40,40,40};
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
}

CCode10B::~CCode10B()
{
	delete c, v;
}

void CCode10B::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	CString s;

	dc.SelectObject(fontTimes);
	dc.SetTextColor(RGB(255, 255, 255));
	for (int k = 1; k <= C; k++)
	{
		c[k].Home.x = 25; c[k].Home.y = 35 + (k - 1) * 30;
		rc = CRect(c[k].Home, CSize(wBar + 5, hBar));
		dc.FillSolidRect(&rc, RGB(150, 150, 150));
		s.Format((CString)"%d", k);
		dc.TextOutW(c[k].Home.x+7, 7+30*k, s);
	}
	dc.SetBkColor(RGB(255, 255, 255));
	dc.SetTextColor(RGB(100, 100, 100));
	s.Format((CString)"#Counters: %d  Max. #Customers: %d", C, N);
	dc.TextOutW(30, 350, s);
}

void CCode10B::OnKeyDown(UINT nChar, UINT nRep, UINT nFlags)
{
	CPaintDC dc(this);
	CRect rc;
	CString s;
	CBrush* pWhite = new CBrush(RGB(255, 255, 255));
	int aC, niv, nav;	//assignment, no. of incoming, no. of available customers
	int i, k, r;

	time_t seed = time(NULL);
	srand((unsigned)seed);
	if (nChar == VK_SPACE && v[N].sta != 3)
	{
		dc.FillRect(CRect(30, 390, 750, 410), pWhite);
		dc.SelectObject(fontTimes);
		dc.SetBkColor(RGB(255, 255, 255));
		dc.SetTextColor(RGB(0, 0, 0));
		s.Format((CString)"t=%d", ++t);
		dc.TextOutW(30, 390, s);
		
		for (k = 1; k <= C; k++)
			if (c[k].av[0] >= 1)
			{
				int r = c[k].lv;
				if (v[r].act <= t)
					c[k].sta = 0;
			}
		nav = 0;
		niv = rand() % mniv;
		dc.SetTextColor(RGB(150, 150, 150));

		for (i = 1; i <= niv; i++)
		{
			nv++;
			if (nv <= N)
			{
				v[nv].art = t;
				v[nv].sta = 1;
			}
		}
		
		for (i = 1; i <= nv; i++)
		{
			if (v[i].sta == 1)
			{
				v[i].aC = 0;
				for(k=1; k<= C; k++)
					if (!c[k].sta)
					{
						aC = k; nav++, tnav++;
						v[nv].aC = aC;
						v[nv].len = 1 + rand() % mLen;
						v[nv].ast = t;
						v[nv].act = t + v[nv].len;
						v[i].wait = v[i].ast - v[i].art;
						v[i].Home = CPoint(55 + t * wBar, 5 + 30 * aC);
						c[aC].sta = 1;
						c[aC].lv = i;
						r = ++c[aC].av[0];
						c[aC].av[r] = i;
						rc = CRect(v[i].Home, CSize(wBar*v[i].len, hBar));
						dc.Rectangle(rc);
						s.Format((CString)"%d", i);
						dc.TextOutW(55 + t * wBar + 4, 5 + 30 * aC + 2, s);
						break;
					}
			}
			if (t >= v[i].act && v[i].sta == 2)
				v[i].sta = 3;
		}
		dc.SetTextColor(RGB(0, 0, 0));
		s.Format((CString)"#Assigned Customers = %d", nav, niv);
		dc.TextOutW(95, 390, s);
		s.Format((CString)"Last Customer Number : %d ", tnav);
		dc.TextOutW(30, 370, s);
		CustomerInfo();
	}
	if (v[N].sta == 2)
	{
		v[N].sta = 3;
		Analysis();
		return;
	}
}

void CCode10B::CustomerInfo()
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
			s.Format((CString)"%d", i + 1); 
			vInfoView.SetItemText(i, 0, s);
			s.Format((CString)"%d", v[i + 1].aC); 
			vInfoView.SetItemText(i, 1, s);
			s.Format((CString)"%d", v[i + 1].len); 
			vInfoView.SetItemText(i, 2, s);
			s.Format((CString)"%d", v[i + 1].art); 
			vInfoView.SetItemText(i, 3, s);
			s.Format((CString)"%d", v[i + 1].ast); 
			vInfoView.SetItemText(i, 4, s);
			s.Format((CString)"%d", v[i + 1].act); 
			vInfoView.SetItemText(i, 5, s);
			s.Format((CString)"%d", v[i + 1].sta); 
			vInfoView.SetItemText(i, 6, s);
		}
	}
}

void CCode10B::Analysis()
{
	CPaintDC dc(this);
	CString s;

	int k;
	double lambda, mu, rho, temp, tC;
	double p0;	//prob. of having no customers
	double pq, Nq, wq, Ts, Ns;
	tC = (double)C;
	lambda = (double)N / t;
	mu = 1 / lambda;
	rho = lambda / (tC*mu);
	temp = (pow(tC*rho, C)) / ((factorial(tC)*(1 - rho)));
	Nq = pq * rho / (1 - rho);
	wq = Nq / lambda;
	Ts = wq + 1 / mu;
	Ns = lambda * Ts;

	dc.SelectObject(fontTimes);
	dc.SetBkColor(RGB(255, 255, 255));
	dc.SetTextColor(RGB(100, 100, 100));

	s.Format((CString)"Arrival Rate, lambda: %1f", lambda);
	dc.TextOutW(450, 360, s);
	s.Format((CString)"Mean service time, mu: %1f", mu);
	dc.TextOutW(450, 385, s);
	s.Format((CString)"Counter utilization rate, rho: %1f", rho);
	dc.TextOutW(450, 410, s);

	s.Format((CString)"Prob. of having no customers, p0: %1f", p0);
	dc.TextOutW(450, 435, s);
	s.Format((CString)"Mean sojourn time, Ts: %1f", Ts);
	dc.TextOutW(450, 460, s);
	s.Format((CString)"Mean number of customers in the system, Ns: %1f", Ns);
	dc.TextOutW(450, 485, s);
	s.Format((CString)"Prob. of queueing (Erlang-C), pq: %1f", pq);
	dc.TextOutW(450, 510, s);
	s.Format((CString)"Expected number of customers in queue, Nq: %1f", Nq);
	dc.TextOutW(450, 535, s);
	s.Format((CString)"Mean waiting time in queue, wq: %1f", wq);
	dc.TextOutW(450, 560, s);
}

double CCode10B::factorial(double k)
{
	if (k == 0)
		return 1;
	return k * factorial(k - 1);
}