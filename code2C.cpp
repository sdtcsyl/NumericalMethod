#include "code2C.h"

CMyWinApp MyApplication;

BOOL CMyWinApp::InitInstance()
{
	AfxEnableControlContainer();
	Enable3dControlsStatic();
	CCode2C dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	return FALSE;
}

BEGIN_MESSAGE_MAP(CCode2C, CDialog)
	ON_BN_CLICKED(IDC_COMPUTE, OnCompute)
END_MESSAGE_MAP()

CCode2C::CCode2C(CWnd* pParent) : CDialog(CCode2C::IDD, pParent)
{
	b = new double[N + 1];
	sb = new CString[N + 1];
	x = new double[N + 1];
	sx = new CString[N + 1];
	a = new double *[N + 1];
	sa = new CString *[N + 1];
	
	for (int i = 1; i <= N; i++) {
		a[i] = new double[N + 1];
		sa[i] = new CString[N + 1];
	}
}

CCode2C::~CCode2C()
{
	for (int i = 1; i <= N; i++)
		delete a[i], sa[i];
	delete a, sa, b, x, sb, sx;
}

void CCode2C::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_A11, sa[1][1]);
	DDX_Text(pDX, IDC_A12, sa[1][2]);
	DDX_Text(pDX, IDC_A13, sa[1][3]);
	DDX_Text(pDX, IDC_A21, sa[2][1]);
	DDX_Text(pDX, IDC_A22, sa[2][2]);
	DDX_Text(pDX, IDC_A23, sa[2][3]);
	DDX_Text(pDX, IDC_A31, sa[3][1]);
	DDX_Text(pDX, IDC_A32, sa[3][2]);
	DDX_Text(pDX, IDC_A33, sa[3][3]);
	DDX_Text(pDX, IDC_b1, sb[1]);
	DDX_Text(pDX, IDC_b2, sb[2]);
	DDX_Text(pDX, IDC_b3, sb[3]);
	DDX_Text(pDX, IDC_x1, sx[1]);
	DDX_Text(pDX, IDC_x2, sx[2]);
	DDX_Text(pDX, IDC_x3, sx[3]);
}

void CCode2C::OnCompute()
{
	CString s;
	int i, j;
	UpdateData(TRUE);
	for (i = 1; i <= N; i++) {
		b[i] = atof(sb[i]);
		for (j = 1; j <= N; j++)
			a[i][j] = atof(sa[i][j]);
	}
	PGauss();
	for (i = 1; i <= N; i++)
		sx[i].Format("%lf", x[i]);
	UpdateData(FALSE);
}

void CCode2C::PGauss()
{
	int i, j, k;
	double m, Sum;
	for (k = 1; k <= N - 1; k++)
		for (i = k + 1; i <= N; i++)
		{
			m = a[i][k] / a[k][k];
			for (j = 1; j <= N; j++)
				a[i][j] = a[i][j] - m * a[k][j];
			b[i] = b[i] - m * b[k];
		}
	for (i = N; i >= 1; i--)
	{
		Sum = 0;
		x[i] = 0;
		for (j = i; j <= N; j++)
			Sum += a[i][j] * x[j];
		x[i] = (b[i] - Sum) / a[i][i];
	}
}