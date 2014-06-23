#if !defined(AFX_CWAITDIALOG_H__D887162B_FAFD_46DB_9B1A_BAA20265C1F9__INCLUDED_)
#define AFX_CWAITDIALOG_H__D887162B_FAFD_46DB_9B1A_BAA20265C1F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CWaitDialog.h : header file
//

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog

#define USR_SHOW_WAIT_INFO (WM_USER + 101)
#include <afxmt.h>
//#include "export.h"

class /*DLL_CONTROL_EXPORT*/ CWaitDlg : public CDialog
{
// Construction
public:
	CWaitDlg(CWnd* pParent = NULL);   // standard constructor
	~CWaitDlg();

	static void InitWaitDlgA(
		BOOL bNeedProgressCtrl ,//= FALSE, 
		int  ProgressRange     ,//= 128,
		float fYRate    ,//= 0.375,
		float fXRate    ,//= 0.5,
		COLORREF cBkRgb ,//= RGB(192,192,192), 
		COLORREF cTextColor
	);

	static void SetWaitDlgTextA(const char *tip);
	static void SetWaitDlgBkColorA(COLORREF rgb);
	static void SetWaitDlgTextColorA(COLORREF rgb);
	static void SetWaitDlgPosA(float XRate, float YRate);
	static void SetProgressStepA(int step);
	static void SetWaitDlgBkImageA(CBitmap *lpBitmap);

// Dialog Data
	//{{AFX_DATA(CWaitDlg)
	enum { IDD = IDD_WAIT_DIALOG };
	CProgressCtrl	m_Progress;			//默认是要进度条的
	CString		m_WaitText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT            m_Timer;
	static CSemaphore m_DlgInitOver;


	static CBrush    m_lpBkBrush;
	static COLORREF  m_TextColor;
	static float     m_XRate;
	static float     m_YRate;
	static char   m_SetTextInfo[256];
	static BOOL		 m_NeedProgress;
	static int		 m_ProgressStep;
	static int       m_ProgressRange;
	static CRect     m_WindowRect;
	// Generated message map functions
	//{{AFX_MSG(CWaitDlg)
	afx_msg void OnClose();
	afx_msg LRESULT OnShowWaitInfo(WPARAM wParam,LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWAITDIALOG_H__D887162B_FAFD_46DB_9B1A_BAA20265C1F9__INCLUDED_)
