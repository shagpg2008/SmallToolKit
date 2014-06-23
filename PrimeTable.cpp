// PrimeTable.cpp : implementation file
//

#include "stdafx.h"
#include "SmallToolKit.h"
#include "PrimeTable.h"
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrimeTable dialog


CPrimeTable::CPrimeTable(CWnd* pParent /*=NULL*/)
	: CDialog(CPrimeTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrimeTable)
	m_szNumber = 1000;
	SetPrimeTable(m_szNumber);
	//}}AFX_DATA_INIT
}


void CPrimeTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrimeTable)
	DDX_Text(pDX, IDC_EDIT_MAX_NUM, m_szNumber);
	DDX_Text(pDX, IDC_STATIC_TABLE, m_strPrimeTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrimeTable, CDialog)
	//{{AFX_MSG_MAP(CPrimeTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrimeTable message handlers

void CPrimeTable::SetPrimeTable(unsigned long num)
{
#define BUF_SIZE (1<<24)
	void get_prime_between(vector<unsigned long> *plist, unsigned long start, unsigned long end);
	vector<unsigned long> prime;
	get_prime_between(&prime, 1, num);
	vector<unsigned long>::const_iterator it = prime.begin();
	char *buff = new char[BUF_SIZE];
	char *lpBuff = buff;
	char *lpBake = buff;
	int len = 0, remaind = BUF_SIZE-1;

	while(it != prime.end()) {
		len = _snprintf(lpBuff, remaind, "%d ", *it);
		lpBuff += len;
		remaind -= len;
		it++;
	}

	lpBuff--;
	*lpBuff = 0;
	m_strPrimeTable = buff;
	delete []buff;
}

BOOL CPrimeTable::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	((CComboBox *)GetDlgItem(IDC_COMBO_OPERATE))->SetCurSel(0);
	return TRUE;
}

void CPrimeTable::OnOK()
{
	int isprime(unsigned long num);
	char buff[64] = {0};
	int index = ((CComboBox *)GetDlgItem(IDC_COMBO_OPERATE))->GetCurSel();

	UpdateData();

	switch(index) {
	case 0: {
		sprintf(buff, "%d %s a prime number.", m_szNumber, isprime(m_szNumber)?"is":"isn't");
		AfxMessageBox(buff);
		break;
	}
	case 1: {
		unsigned long tmp = m_szNumber-1;
		if(m_szNumber <= 2) {
			sprintf(buff, "the number should be bigger than 2.");
			AfxMessageBox(buff);
			return ;
		}

		while(!isprime(tmp) && tmp > 1) tmp--;
		sprintf(buff, "the biggest prime less than %d is %d.", m_szNumber, tmp);
		AfxMessageBox(buff);
	} break;
	
	case 2: {
		unsigned long tmp = m_szNumber+1;
		if(m_szNumber >= (unsigned long)-1) {
			sprintf(buff, "the number should be less than ULONG_MAX.");
			AfxMessageBox(buff);
			return ;
		}

		while(!isprime(tmp) && tmp < (unsigned long)-1) tmp++;
		sprintf(buff, "the least prime bigger than %d is %d.", m_szNumber, tmp);
		AfxMessageBox(buff);
	} break;
	case 3: {
		SetPrimeTable(m_szNumber);
		break;
	}
	default: break;
	}

	UpdateData(FALSE);
	//CDialog::OnOK();
}
