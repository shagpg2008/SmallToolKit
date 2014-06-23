// CWaitDialog.cpp : implementation file
//

#define WAIT_DLG_IMPLEMENT

#include "stdafx.h"
#include "CWaitDialog.h"
#include "CWaitDlgDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog
CSemaphore CWaitDlg::m_DlgInitOver(1, 1);
CRect     CWaitDlg::m_WindowRect(0,0,0,0);
CBrush    CWaitDlg::m_lpBkBrush(RGB(192,192,192));
COLORREF  CWaitDlg::m_TextColor  = RGB(0,0,0);
float     CWaitDlg::m_XRate      = 0.5;
float     CWaitDlg::m_YRate      = 0.375;
char      CWaitDlg::m_SetTextInfo[256]= _T("Please wait...");
BOOL	  CWaitDlg::m_NeedProgress  = FALSE; //TRUE;//
int		  CWaitDlg::m_ProgressStep  = 0;
int       CWaitDlg::m_ProgressRange = 128;


CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitDlg)
	m_WaitText  = _T("Please wait...");
	//}}AFX_DATA_INIT
}

CWaitDlg::~CWaitDlg()
{
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_STATIC_TEXT, m_WaitText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
	//{{AFX_MSG_MAP(CWaitDlg)
	ON_WM_CLOSE()
	ON_MESSAGE(USR_SHOW_WAIT_INFO, OnShowWaitInfo)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg message handlers
void CWaitDlg::OnOK()
{
}

void CWaitDlg::OnCancel()
{
}

void CWaitDlg::OnClose() 
{
	CDialog::OnOK();
	m_DlgInitOver.Unlock();
}

HBRUSH CWaitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_lpBkBrush;
	}

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(m_TextColor);
		pDC->SetBkMode(TRANSPARENT);
		return m_lpBkBrush;
	}

	return hbr;
}

BOOL CWaitDlg::OnInitDialog() 
{
	m_WaitText = m_SetTextInfo;
	CDialog::OnInitDialog();
	
	RECT parentRect = {0};
	RECT Dlgect     = {0};
	CRect NewRect(0,0,0,0);

	GetWindowRect(&Dlgect);
	if(m_WindowRect.Width() != 0 && m_WindowRect.Height() != 0)
	{
		Dlgect = m_WindowRect;
	}

	CWnd *lpParent = GetParent();
	if(lpParent != NULL)
	{
		lpParent->GetWindowRect(&parentRect);
	}
	else{
		parentRect.right  = GetSystemMetrics(SM_CXSCREEN);
		parentRect.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
	
	int Prtx = parentRect.right - parentRect.left;
	int Prty = parentRect.bottom - parentRect.top;
	int Dlgx = Dlgect.right - Dlgect.left;
	int Dlgy = Dlgect.bottom- Dlgect.top;
	NewRect.left   = parentRect.left + (long)((Prtx-Dlgx) * m_XRate);
	NewRect.right  = NewRect.left + Dlgx;
	NewRect.top    = parentRect.top  + (long)((Prty-Dlgy) * m_YRate);
	NewRect.bottom = NewRect.top  + Dlgy;

	MoveWindow(&NewRect);
	m_Progress.SetRange(0, m_ProgressRange);

	//if there is no progress, then remove the text position.
	if(!m_NeedProgress)
	{
		GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT)->GetClientRect(&Dlgect);
		int textHigh = Dlgect.bottom - Dlgect.top;
		int addy = (Dlgy - textHigh) / 2;
		Dlgect.top    +=   addy;
		Dlgect.bottom = Dlgect.top  + textHigh;
		GetDlgItem(IDC_STATIC_TEXT)->MoveWindow(&Dlgect);
	}

	m_Timer = (UINT)SetTimer(1, 1000, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CWaitDlg::OnShowWaitInfo(WPARAM wParam,LPARAM lParam)
{
	m_WaitText = m_SetTextInfo;
	UpdateData(FALSE);
	return 0;
}

void CWaitDlg::InitWaitDlgA(
		BOOL bNeedProgressCtrl, // = FALSE, 
		int  ProgressRange    ,//= 128,
		float fYRate    , //= 0.375,
		float fXRate   , //= 0.5,
		COLORREF cBkRgb, // = RGB(192,192,192), 
		COLORREF cTextColor  //= RGB(0,0,0), 
	)
{
	m_ProgressRange = ProgressRange;
	m_NeedProgress = bNeedProgressCtrl;
	m_lpBkBrush.DeleteObject();
	m_lpBkBrush.CreateSolidBrush(cBkRgb);
	m_TextColor = cTextColor;
}

void CWaitDlg::SetWaitDlgTextA(const char *tip)
{
	strncpy(m_SetTextInfo, tip, 256);
}

void CWaitDlg::SetWaitDlgBkColorA(COLORREF rgb)
{
	m_lpBkBrush.DeleteObject();
	m_lpBkBrush.CreateSolidBrush(rgb);
}

void CWaitDlg::SetWaitDlgTextColorA(COLORREF rgb)
{
	m_TextColor = rgb;
}

void CWaitDlg::SetWaitDlgPosA(float xRate, float yRate)
{
	m_XRate = xRate;
	m_YRate = yRate;
}

void CWaitDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent  == 1)
	{
		if(m_WaitText != m_SetTextInfo)
		{
			m_WaitText = m_SetTextInfo;
			UpdateData(FALSE);
		}
		else
		{
			Invalidate();
		}

		if(m_NeedProgress){
			m_Progress.SetPos(m_ProgressStep);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CWaitDlg::SetProgressStepA(int step)
{
	m_ProgressStep = step;
}


void CWaitDlg::SetWaitDlgBkImageA(CBitmap *lpBitmap)
{
	m_lpBkBrush.DeleteObject();
	m_lpBkBrush.CreatePatternBrush(lpBitmap);
}




static CWaitDlg *lpWaitDlg = NULL;


//////////////////////////////////////////////////////////////
//下面是导出的函数实现
//////////////////////////////////////////////////////////////
static DWORD WINAPI ThreadUpdateIndex(LPVOID lpArgs);

void WINAPI InitWaitDlg(
		BOOL bNeedProgressCtrl, // = FALSE, 
		int  ProgressRange    ,//= 128,
		float fYRate    , //= 0.375,
		float fXRate   , //= 0.5,
		COLORREF cBkRgb, // = RGB(192,192,192), 
		COLORREF cTextColor
	)
{
	CWaitDlg::InitWaitDlgA(
		bNeedProgressCtrl, // = FALSE, 
		ProgressRange ,// = 128,
		fYRate    ,   //= 0.375,
		fXRate   ,   //= 0.5,
		cBkRgb,      // = RGB(192,192,192), 
		cTextColor
	);
}

void WINAPI SetWaitDlgBkImage(CBitmap *lpBitmap)
{
	if(lpBitmap != NULL)
	{
		CWaitDlg::SetWaitDlgBkImageA(lpBitmap);
	}
}

void WINAPI SetProgressStep(int step)
{
	CWaitDlg::SetProgressStepA(step);
}

void WINAPI SetWaitDlgPos(float yRate, float xRate)
{
	CWaitDlg::SetWaitDlgPosA(xRate, yRate);
}

void WINAPI SetWaitDlgBkColor(COLORREF rgb)
{
	CWaitDlg::SetWaitDlgBkColorA(rgb);
}

void WINAPI SetWaitDlgRect(int left, int top, int right, int bottom)
{
	CWaitDlg::m_WindowRect.left = left;
	CWaitDlg::m_WindowRect.top = top;
	CWaitDlg::m_WindowRect.right = right;
	CWaitDlg::m_WindowRect.bottom = bottom;
}

void WINAPI SetWaitDlgTextColor(COLORREF rgb)
{
	CWaitDlg::SetWaitDlgTextColorA(rgb);
}

void WINAPI SetWaitDlgText(const char *tip)
{
	CWaitDlg::SetWaitDlgTextA(tip);
}

void WINAPI ShowWaitTipWindow(HWND hMainWnd, bool show)
{
	if(show && lpWaitDlg == NULL)
	{
		DWORD dwThreadId = 0;
		ASSERT(CWaitDlg::m_DlgInitOver.Lock());
		HANDLE thread = CreateThread(NULL, 0, ThreadUpdateIndex, (LPVOID)hMainWnd, NULL, &dwThreadId);
		SetThreadPriority(thread, THREAD_PRIORITY_ABOVE_NORMAL);
		Sleep(100);
	}
	else if(lpWaitDlg != NULL)
	{
		PostMessage(lpWaitDlg->m_hWnd, WM_CLOSE,0,0);
	}

	return ;
}

static DWORD WINAPI ThreadUpdateIndex(LPVOID lpArgs)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HWND hMainWnd = (HWND)lpArgs;
	if(hMainWnd == NULL)
	{
		hMainWnd = ::GetDesktopWindow();
	}

	CWaitDlg dlg(CWnd::FromHandle(hMainWnd));
	lpWaitDlg = &dlg;
	dlg.DoModal();
	lpWaitDlg = NULL;
	return 0;
}
