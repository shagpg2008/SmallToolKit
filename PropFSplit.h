#if !defined(AFX_PROPFSPLIT_H__E7B140E7_31F6_41FE_A9F2_C7CE579007D9__INCLUDED_)
#define AFX_PROPFSPLIT_H__E7B140E7_31F6_41FE_A9F2_C7CE579007D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropFSplit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropFSplit dialog

class CPropFSplit : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropFSplit)

// Construction
public:
	CPropFSplit();
	~CPropFSplit();

// Dialog Data
	//{{AFX_DATA(CPropFSplit)
	enum { IDD = IDD_PROPPAGE_FSPLIT };
	CListCtrl	m_lstFileList;
	CString	m_strDstFileName;
	CString	m_strFilePath;
	CString	m_strKeyValue;
	DWORD	m_strSize;
	CString	m_strSuffix;
	int		m_bSizeOrKeyValue;
	int		m_bSplitOrCombine;
	//}}AFX_DATA
	DWORD   m_dwFileDialogAttr;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropFSplit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	void EnableSomeWindows(BOOL bValue);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropFSplit)
	virtual void OnOK();
	afx_msg void OnRadioSplit();
	afx_msg void OnRadioCombine();
	afx_msg void OnRadioSize();
	afx_msg void OnRadioKeryvalue();
	afx_msg void OnButtonGetfiles();
	afx_msg void OnButtonDstfilename();
	afx_msg void OnCheckSuffix();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnDropFiles( HDROP hDropInfo );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPFSPLIT_H__E7B140E7_31F6_41FE_A9F2_C7CE579007D9__INCLUDED_)
