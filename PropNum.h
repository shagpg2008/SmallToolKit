#if !defined(AFX_PROPNUM_H__9E5E5618_7BE0_4783_A1C3_4E2AFF399AAE__INCLUDED_)
#define AFX_PROPNUM_H__9E5E5618_7BE0_4783_A1C3_4E2AFF399AAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropNum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropNum dialog

class CPropNum : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropNum)

// Construction
public:
	CPropNum();
	~CPropNum();

// Dialog Data
	//{{AFX_DATA(CPropNum)
	enum { IDD = IDD_PROPPAGE_Num };
	DWORD	m_dwNum1;
	DWORD	m_dwNum2;
	DWORD	m_dwNum3;
	int		m_RadiaNum;
	CString	m_strExpr;
	CString	m_strMath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropNum)
	virtual void OnOK();
	afx_msg void OnGetprimers();
	afx_msg void OnAscci();
	afx_msg void OnRadioRev();
	afx_msg void OnRadioMath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPNUM_H__9E5E5618_7BE0_4783_A1C3_4E2AFF399AAE__INCLUDED_)
