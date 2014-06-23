#if !defined(AFX_PROPCHKSUM_H__FD6589B2_B748_401E_A0B1_1E04096A956B__INCLUDED_)
#define AFX_PROPCHKSUM_H__FD6589B2_B748_401E_A0B1_1E04096A956B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropChkSum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropChkSum dialog

class CPropChkSum : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropChkSum)

// Construction
public:
	CPropChkSum();
	~CPropChkSum();

// Dialog Data
	//{{AFX_DATA(CPropChkSum)
	enum { IDD = IDD_PROPPAGE_CHKSUM };
	CListCtrl	m_mdList;
	CString	m_strCRC16;
	CString	m_strCRC32;
	CString	m_strCRC32C;
	CString	m_strInitValue;
	CString	m_strOut;
	CString	m_strPoly;
	CString	m_strCRC_CCITT;
	BOOL	m_bInReflect;
	BOOL	m_bOutReflect;
	UINT	m_dwDataWidth;
	CString	m_strSelfCRC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropChkSum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	CString m_szFileName;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropChkSum)
	virtual void OnOK();
	afx_msg void OnButtonBrowse();
	afx_msg void OnDropFiles( HDROP hDropInfo );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPCHKSUM_H__FD6589B2_B748_401E_A0B1_1E04096A956B__INCLUDED_)

