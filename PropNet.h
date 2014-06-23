#if !defined(AFX_PROPNET_H__5A4287DB_8F46_49CD_80E7_B7AA58201D37__INCLUDED_)
#define AFX_PROPNET_H__5A4287DB_8F46_49CD_80E7_B7AA58201D37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropNet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropNet dialog

class CPropNet : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropNet)

// Construction
public:
	CPropNet();
	~CPropNet();

// Dialog Data
	//{{AFX_DATA(CPropNet)
	enum { IDD = IDD_PROPPAGE_NET };
	int		m_bIsClient;
	DWORD	m_dwPort;
	CString	m_strHost;
	BOOL	m_bIsHdrIncl;
	BOOL	m_bIsDataString;
	CString	m_dtGroup1;
	CString	m_dtGroup2;
	CString	m_dtGroup3;
	CString	m_dtGroup4;
	CString	m_dtGroup5;
	CString	m_dtGroup6;
	int		m_tpSocketType;
	//}}AFX_DATA

	BOOL  m_bServerRun;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropNet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropNet)
	virtual void OnOK();
	afx_msg void OnRadioRaw();
	afx_msg void OnRadioTcp();
	afx_msg void OnRadioUdp();
	afx_msg void OnRadioServer();
	afx_msg void OnRadioClient();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPNET_H__5A4287DB_8F46_49CD_80E7_B7AA58201D37__INCLUDED_)
