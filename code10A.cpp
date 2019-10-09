#include "code10A.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode10A* pFrame = new CCode10A;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode10A, CFrameWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CCode10A::CCode10A()
{
	v = new CUSTOMER[N + 1];
	c = new COUNTER[C + 1];
	TopLeft = CPoint(0, 0); BottomRight = CPoint(900, 620);
	Create(NULL, (CString)"Multiserver system simulation with blocking", 
		WS_OVERLAPPEDWINDOW, CRect(TopLeft, BottomRight));
	t = 0; nbv = 0; nv = 0;
	for (int k = 1; k <= C; k++)
	{
		c[k].sta = 0;
		c[k].av[0] = 0;
	}
	wBar = 20; hBar = 20;
	::ZeroMemory(&lfTimes, sizeof(lfTimes));
	lfTimes.lfHeight = 80;
	fontTimes.CreatePointFontIndirect(&lfTimes);
}

CCode10A::~CCode10A()
{
	delete c, v;
}

void CCode10A::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	CString s;

	dc.SelectObject(fontTimes);
	dc.SetTextColor(RGB(255,255,255));
	for (int k = 1; k <= C; k++)
	{
		c[k].Home.x = 25; c[k].Home.y = 35 + (k - 1) * 30;
		rc = CRect(c[k].Home, CSize(wBar + 5, hBar));
		dc.FillSolidRect(&rc, RGB(150, 150, 150));
		s.Format((CString)"%d", k);
		dc.TextOutW(c[k].Home.x + 7, 7 + 30 * k, s);
	}
	dc.SetBkColor(RGB(255, 255, 255));
	dc.SetTextColor(RGB(100, 100, 100));
	s.Format((CString)"#Counters: %d  Max. #Customers: %d++", C, N);
	dc.TextOutW(30, 250, s);
	dc.Rectangle(rc);
	s.Format((CString)"Blocked Customers: ");
	dc.TextOutW(40, 420, s);
}

void CCode10A::OnKeyDown(UINT nChar, UINT nRep, UINT nFlags)
{
	CPaintDC dc(this);
	CRect rc;
	CString s;
	CBrush* pWhite = new CBrush(RGB(255, 255, 255));
	int aC, niv, nav;	//assignment, no. of incoming, no. of available customers

	dc.SelectObject(fontTimes);
	dc.SetTextColor(RGB(0, 0, 0));

	time_t seed = time(NULL);
	srand((unsigned)seed);
	if (nChar == VK_SPACE && nv <= N)
	{
		dc.FillRect(CRect(30, 390, 750, 410), pWhite);
		s.Format((CString)"t=%d", t++);
		dc.TextOutW(30, 290, s);
		nav = 0; 
		niv = rand() % mniv;
		for (int k = 1; k <= C; k++)
			if (c[k].av[0] >= 1)
			{
				int r = c[k].lv;
				if (v[r].act <= t)
					c[k].sta = 0;
			}

		dc.SetTextColor(RGB(150, 150, 150));
		for (int i = 1; i <= niv; i++)
		{
			nv++;
			if (nv > N)
				return;
			for (int k = 1; k <= C; k++) {
				v[nv].aC = 0;
				if (!c[k].sta)
				{
					aC = k; nav++;
					v[nv].aC = aC;
					v[nv].len = 1+rand()%mLen;
					v[nv].ast = t;
					v[nv].act = 1 + v[nv].len;
					c[aC].sta = 1;
					c[aC].lv = nv;
					int r = ++c[aC].av[0];
					c[aC].av[r] = nv;
					s.Format((CString)"%d,%d,%d", nv, v[nv].len, v[nv].aC);
					dc.TextOutW(240 + 65 * i, 390, s);
					v[nv].Home = CPoint(55 + t * wBar, 5 + 30 * aC);
					rc = CRect(v[nv].Home, CSize(wBar*v[nv].len, hBar));
					dc.Rectangle(rc);
					s.Format((CString)"%d", nv);
					dc.TextOutW(55 + t * wBar + 4, 5 + 30 * aC + 2, s);
					break;
				}
			}
			if (v[nv].aC == 0)
			{
				nbv++;
				int v = nbv / 14, w = nbv % 14;
				s.Format((CString)"%d", nv);
				dc.TextOutW(40 + 30 * w, 440 + v * 20, s);
			}
			dc.SetBkColor(RGB(255, 255, 255));
		}
		s.Format((CString)"#Assigned Customers = %d", nav, niv);
		dc.TextOutW(95, 390, s);
		s.Format((CString)"Last Customer Number : ", nv);
		dc.TextOutW(30, 370, s);
		s.Format((CString)"%d, %.21f", nbv, (double)nbv/nv);
		dc.TextOutW(140, 420, s);
	}
}