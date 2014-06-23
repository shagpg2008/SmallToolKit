#if !defined(AFX_PRIMETABLE_H__AF7B9504_43F0_4B05_82D7_EB1141D57930__INCLUDED_)
#define AFX_PRIMETABLE_H__AF7B9504_43F0_4B05_82D7_EB1141D57930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrimeTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrimeTable dialog

class CPrimeTable : public CDialog
{
// Construction
public:
	CPrimeTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrimeTable)
	enum { IDD = IDD_DIALOG_PRIME };
	DWORD	m_szNumber;
	CString	m_strPrimeTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrimeTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetPrimeTable(unsigned long num);

	// Generated message map functions
	//{{AFX_MSG(CPrimeTable)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRIMETABLE_H__AF7B9504_43F0_4B05_82D7_EB1141D57930__INCLUDED_)
