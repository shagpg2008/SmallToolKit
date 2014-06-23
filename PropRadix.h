#if !defined(AFX_PROPRADIX_H__15B336A7_7911_44F7_B34E_582CD781E724__INCLUDED_)
#define AFX_PROPRADIX_H__15B336A7_7911_44F7_B34E_582CD781E724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropRadix.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropRadix dialog

class CPropRadix : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropRadix)

// Construction
public:
	CPropRadix();
	~CPropRadix();

// Dialog Data
	//{{AFX_DATA(CPropRadix)
	enum { IDD = IDD_PROPPAGE_RADIX };
	DWORD	m_dwOutputRadix;
	CString	m_strInput;
	DWORD	m_dwInputRadix;
	CString	m_strOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropRadix)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropRadix)
	virtual void OnOK();
	afx_msg void OnCheckOutputRadix();
	afx_msg void OnCheckRadix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPRADIX_H__15B336A7_7911_44F7_B34E_582CD781E724__INCLUDED_)
