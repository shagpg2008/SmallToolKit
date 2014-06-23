#if !defined(AFX_PROPCODE_H__C08271DE_8C5C_4A3D_B616_79B614975B99__INCLUDED_)
#define AFX_PROPCODE_H__C08271DE_8C5C_4A3D_B616_79B614975B99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropCode.h : header file
//
#include <vector>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CPropCode dialog

class CPropCode : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropCode)

// Construction
public:
	CPropCode();
	~CPropCode();

// Dialog Data
	//{{AFX_DATA(CPropCode)
	enum { IDD = IDD_PROPPAGE_CODE };
	CListCtrl	m_strFileLst;
	BOOL	m_bCheckSelf;
	CString	m_strSelf;
	//}}AFX_DATA
	vector<CString> m_vctPathName;
	int m_dwIfCalced;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropCode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropCode)
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonDelete();
	afx_msg void OnCheckSelf();
	afx_msg void OnButtonCalc();
	afx_msg void OnDropFiles( HDROP hDropInfo );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPCODE_H__C08271DE_8C5C_4A3D_B616_79B614975B99__INCLUDED_)
