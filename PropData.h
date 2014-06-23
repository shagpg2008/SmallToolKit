#if !defined(AFX_PROPDATA_H__B53A8860_5754_4534_AB39_02DCAA4407EA__INCLUDED_)
#define AFX_PROPDATA_H__B53A8860_5754_4534_AB39_02DCAA4407EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropData dialog

class CPropData : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropData)

// Construction
public:
	CPropData();
	~CPropData();

// Dialog Data
	//{{AFX_DATA(CPropData)
	enum { IDD = IDD_PROPPAGE_DATA };
	CEdit	m_cEditFrom;
	CEdit	m_cEditTo;
	BOOL	m_bCheckFrom;
	BOOL	m_bCheckTo;
	CString	m_sEditFrom;
	CString	m_sEditInput;
	CString	m_sEditOutput;
	CString	m_sEditTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropData)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckFrom();
	afx_msg void OnCheckTo();
	virtual void OnOK();
	afx_msg void OnButtonFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPDATA_H__B53A8860_5754_4534_AB39_02DCAA4407EA__INCLUDED_)
