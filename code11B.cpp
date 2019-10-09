#include "code11B.h"
#include "stdafx.h"


BOOL CMyWinApp::InitInstance()
{
	CCode11B* pFrame = new CCode11B;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

CMyWinApp MyApplication;

BEGIN_MESSAGE_MAP(CCode11B, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

CCode11B::CCode11B()
{
	int i, j;
	::ZeroMemory(&lfTimes, sizeof(lfTimes));
	lfTimes.lfHeight = 80; fontTimes.CreatePointFontIndirect(&lfTimes);
	Cell = new CELL[L + 1];
	Create(NULL, (CString)"Channel Assignment Problem: One channel per cell", WS_OVERLAPPEDWINDOW, CRect(0, 0, 800, 600));
	h2.LoadBitmapW(IDB_BITMAP1);

	for (i = 1; i <= L; i++)
		Cell[i].f = 0;  // initialize the cells with channel 0

	// call assignment tables
	FILE *InFile1, *InFile2, *InFile3;
	InFile1 = fopen("Code11BAdj.in", "r");
	InFile2 = fopen("Code11BCoc.in", "r");
	InFile3 = fopen("Code11BHome.in", "r");
	for (i = 1; i <= L; i++)
	{
		for (j = 1; j <= i; j++)
		{
			fscanf(InFile1, "%d", &Cell[i].adj[j]); // read cell adjacency info
			Cell[j].adj[i] = Cell[i].adj[j];
		}
		for (j = 1; j <= i; j++)
		{
			fscanf(InFile2, "%d", &Cell[i].coc[j]); // read cell adjacency info
			Cell[j].coc[i] = Cell[i].coc[j];
		}
		// read cell home
		fscanf(InFile3, "%d %d", &Cell[i].home.x, &Cell[i].home.y);
	}
	fclose(InFile1);
	fclose(InFile2);
	fclose(InFile3);
}

CCode11B::~CCode11B()
{
	delete Cell;
}

void CCode11B::OnPaint()
{
	CPaintDC dc(this);
	CString s;
	int i, j, k, r;
	bool w[L + 1];

	//Draw the h2 hexagon
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&h2);
	dc.BitBlt(0, 50, 400, 320, &memDC, 0, 0, SRCCOPY);

	//Assign the channels
	Cell[1].f = 1;
	for (i = 2; i <= L; i++) {
		k = 1;
		while (k <= L)
		{
			w[k] = 0;
			for(j=1;j<=i-1;j++)
				if (Cell[i].adj[j] && abs(k - Cell[j].f < 2))
				{
					Cell[i].f = k + 1;
					w[k] = 1;
				}
			if (!w[k])
			{
				r = 0;
				for (j = 1; j <= i - 1; j++)
				{
					Cell[i].f = k;
					if (Cell[i].coc[j] && Cell[i].f == Cell[j].f)
					{
						Cell[i].f = k + 1;
						r++;
					}
				}
				if (r == 0)
					break;
			}
			k++;
		}
	}

	//display the output 
	dc.SelectObject(fontTimes);
	for (i = 1; i <= L; i++)
	{
		dc.TextOutW(380, 5, "Adjacency matrix");
		dc.TextOutW(380, 315, "Co-Channel matrix");
		for (j = 1; j <= L; j++)
		{
			s.Format((CString)"%d", Cell[i].adj[j]);
			dc.TextOutW(380 + (i - 1) * 20, 20+(j-1)*15, s);
			s.Format((CString)"%d", Cell[i].coc[j]);
			dc.TextOutW(380 + (i - 1) * 20, 330 + (j - 1) * 15, s);
		}
		s.Format((CString)"%d", Cell[i].f);
		dc.TextOutW(Cell[i].home.x , Cell[i].home.y, s);
	}
}