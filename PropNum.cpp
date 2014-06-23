// PropNum.cpp : implementation file
//

//#pragma warning( disable :C4003)
//#pragma warning(push,3)
#include "stdafx.h"
#include "SmallToolKit.h"
#include "PropNum.h"
#include "PrimeTable.h"
//#include "AscciiCodeTable.h"
#include "./core/math.h"
extern "C" double eval(const char *s);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropNum property page

IMPLEMENT_DYNCREATE(CPropNum, CPropertyPage)

CPropNum::CPropNum() : CPropertyPage(CPropNum::IDD)
{
	//{{AFX_DATA_INIT(CPropNum)
	m_dwNum1 = 0;
	m_dwNum2 = 0;
	m_dwNum3 = 0;
	m_RadiaNum = 0;
	m_strExpr = _T("");
	m_strMath = _T("");
	//}}AFX_DATA_INIT
}

CPropNum::~CPropNum()
{
}

void CPropNum::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropNum)
	DDX_Text(pDX, IDC_EDIT_NUM1, m_dwNum1);
	DDX_Text(pDX, IDC_EDIT_NUM2, m_dwNum2);
	DDX_Text(pDX, IDC_EDIT_NUM3, m_dwNum3);
	DDX_Radio(pDX, IDC_RADIO_GCD, m_RadiaNum);
	DDX_Text(pDX, IDC_EDIT_EXPR, m_strExpr);
	DDX_Text(pDX, IDC_EDIT_MATH, m_strMath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropNum, CPropertyPage)
	//{{AFX_MSG_MAP(CPropNum)
	ON_BN_CLICKED(IDGETPRIMERS, OnGetprimers)
	ON_BN_CLICKED(ID_ASCCI, OnAscci)
	ON_BN_CLICKED(IDC_RADIO_REV, OnRadioRev)
	ON_BN_CLICKED(IDC_RADIO_MATH, OnRadioMath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropNum message handlers
void CPropNum::OnOK() 
{
	int index = IDC_RADIO_GCD;
	// TODO: Add extra validation here
	//CDialog::OnOK();
	UpdateData();

	if(((CButton*)GetDlgItem(IDC_RADIO_MATH))->GetCheck()) {
		char buff[100] = {0};
		int index = 0;
		sprintf(buff, "%f", eval(m_strExpr));
		index = strlen(buff) - 1;
		while(buff[index] == '0') {
			index--;
		}

		if(buff[index] == '.') {
			buff[index] = 0;
		} else {
			buff[index+1] = 0;
		}

		m_strMath = buff;

		UpdateData(FALSE);
		return;
	}

	index += m_RadiaNum;

	if((m_dwNum1 == 0 || m_dwNum2 == 0) && index < IDC_RADIO_XOR
		|| (m_dwNum2 == 0 && index == IDC_RADIO_DIV)) {
		AfxMessageBox(_T("Number shouldn't be zero!"));
		return ;
	}

	switch(index) {
	case IDC_RADIO_GCD: m_dwNum3 = (DWORD)gcd((DWORD)m_dwNum1, (DWORD)m_dwNum2);break;
	case IDC_RADIO_LCM: m_dwNum3 = (DWORD)lcm((DWORD)m_dwNum1, (DWORD)m_dwNum2);break;
	case IDC_RADIO_LIM: m_dwNum3 = (DWORD)lim((DWORD)m_dwNum1, (DWORD)m_dwNum2);break;
	case IDC_RADIO_MOD: m_dwNum3 = (DWORD)mod((DWORD)m_dwNum1, (DWORD)m_dwNum2);break;
	case IDC_RADIO_XOR: m_dwNum3 = (DWORD)m_dwNum1 ^ (DWORD)m_dwNum2;break;
	case IDC_RADIO_OR:  m_dwNum3 = (DWORD)m_dwNum1 | (DWORD)m_dwNum2;break;
	case IDC_RADIO_AND: m_dwNum3 = (DWORD)m_dwNum1 & (DWORD)m_dwNum2;break;
	case IDC_RADIO_ADD: m_dwNum3 = (DWORD)m_dwNum1 + (DWORD)m_dwNum2;break;
	case IDC_RADIO_SUB: m_dwNum3 = (DWORD)m_dwNum1 - (DWORD)m_dwNum2;break;
	case IDC_RADIO_MUL: m_dwNum3 = (DWORD)m_dwNum1 * (DWORD)m_dwNum2;break;
	case IDC_RADIO_DIV: m_dwNum3 = (DWORD)m_dwNum1 / (DWORD)m_dwNum2;break;
	case IDC_RADIO_REV: m_dwNum3 = ~((DWORD)m_dwNum1);break;
	default :break;
	}

	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_NUM1)->SetFocus();
}

void CPropNum::OnGetprimers() 
{
	CPrimeTable	dlg;
	dlg.DoModal();
}

void CPropNum::OnAscci() 
{
//	CAscciiCodeTable dlg;
	//dlg.DoModal();
	HINSTANCE result = ShellExecute(NULL, _T("open"), _T("rundll32.exe"), 
								_T("url.dll,FileProtocolHandler http://www.asciitable.com/"), 
								NULL, SW_SHOWNORMAL);
}

void CPropNum::OnRadioRev() 
{
	
}

void CPropNum::OnRadioMath() 
{
/*	if(((CButton*)GetDlgItem(IDC_RADIO_MATH))->GetCheck()) {
		GetDlgItem(IDC_EDIT_EXPR)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_EDIT_EXPR)->EnableWindow(FALSE);
	}
*/}
