// SmallToolKitDlg.h : header file
//

#if !defined(AFX_SMALLTOOLKITDLG_H__C7D9B638_28B3_445D_9FA5_15052787C30A__INCLUDED_)
#define AFX_SMALLTOOLKITDLG_H__C7D9B638_28B3_445D_9FA5_15052787C30A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PropData.h"
#include "PropNum.h"
#include "PropRadix.h"
#include "PropNet.h"
#include "PropFSplit.h"
#include "PropCode.h"
#include "PropChkSum.h"

/////////////////////////////////////////////////////////////////////////////
// CSmallToolKitDlg dialog

class CSmallToolKitDlg : public CDialog
{
// Construction
public:
	CSmallToolKitDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSmallToolKitDlg)
	enum { IDD = IDD_SMALLTOOLKIT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmallToolKitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnOK() ;
	virtual void OnCancel() ;
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CPropertySheet m_sheet;
	CPropData      m_data;
	CPropNum       m_num;
	CPropRadix     m_radix;
	CPropNet       m_net;
	CPropFSplit    m_split;
	CPropCode      m_code;
	CPropChkSum    m_sum;

	// Generated message map functions
	//{{AFX_MSG(CSmallToolKitDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMALLTOOLKITDLG_H__C7D9B638_28B3_445D_9FA5_15052787C30A__INCLUDED_)
