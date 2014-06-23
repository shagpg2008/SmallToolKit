// PropRadix.cpp : implementation file
//

#include "stdafx.h"
#include "SmallToolKit.h"
#include "PropRadix.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropRadix property page

IMPLEMENT_DYNCREATE(CPropRadix, CPropertyPage)

CPropRadix::CPropRadix() : CPropertyPage(CPropRadix::IDD)
{
	//{{AFX_DATA_INIT(CPropRadix)
	m_dwOutputRadix = 16;
	m_strInput = _T("");
	m_dwInputRadix = 10;
	m_strOutput = _T("");
	//}}AFX_DATA_INIT
}

CPropRadix::~CPropRadix()
{
}

void CPropRadix::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropRadix)
	DDX_Text(pDX, IDC_EDIT_OUTPUT_RADIX, m_dwOutputRadix);
	DDX_Text(pDX, IDC_EDIT_INPUT_NUM, m_strInput);
	DDX_Text(pDX, IDC_EDIT_RADIX, m_dwInputRadix);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_NUM, m_strOutput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropRadix, CPropertyPage)
	//{{AFX_MSG_MAP(CPropRadix)
	ON_BN_CLICKED(IDC_CHECK_OUTPUT_RADIX, OnCheckOutputRadix)
	ON_BN_CLICKED(IDC_CHECK_RADIX, OnCheckRadix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropRadix message handlers

BOOL CPropRadix::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	((CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_RADIX))->SetReadOnly(TRUE);
	((CEdit*)GetDlgItem(IDC_EDIT_RADIX))->SetReadOnly(TRUE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPropRadix::OnOK() 
{
	DWORD temp = 0;
	char buff[1024] = {0};
	const char *start = NULL;
	char *rear = NULL;
	
	UpdateData();

	if(m_dwOutputRadix < 2 || m_dwInputRadix < 2) {
		AfxMessageBox("The minimum radix number is 2.");
		return ;
	}

	start = (LPCTSTR)m_strInput;
	rear = (char *)start;
	m_strOutput = _T("");

	while(*rear != 0) {
		temp = strtoul(start, &rear, (int)m_dwInputRadix);
		
		if(start < rear) {
			itoa(temp, buff, m_dwOutputRadix);
			m_strOutput += buff;
		}

		if(*rear != 0) {
			m_strOutput += *rear++;
			start = rear;
		}
	}

	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_INPUT_NUM)->SetFocus();
	CPropertyPage::OnOK();
}

void CPropRadix::OnCheckOutputRadix()
{
	((CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_RADIX))->SetReadOnly(!IsDlgButtonChecked(IDC_CHECK_OUTPUT_RADIX));
}

void CPropRadix::OnCheckRadix() 
{
	// TODO: Add your control notification handler code here
	((CEdit*)GetDlgItem(IDC_EDIT_RADIX))->SetReadOnly(!IsDlgButtonChecked(IDC_CHECK_RADIX));
}
