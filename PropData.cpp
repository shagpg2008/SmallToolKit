// PropData.cpp : implementation file
//

#include "stdafx.h"
#include "SmallToolKit.h"
#include "PropData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
void convert_from_buff(char *buff, int maxlen, const char *data/*, int size*/, const char*infmt, const char *outfmt);
void convert_from_file(char *buff, int maxlen, FILE *fp, const char*infmt, const char *outfmt);
}


/////////////////////////////////////////////////////////////////////////////
// CPropData property page

IMPLEMENT_DYNCREATE(CPropData, CPropertyPage)

CPropData::CPropData() : CPropertyPage(CPropData::IDD)
{
	//{{AFX_DATA_INIT(CPropData)
	m_bCheckFrom = FALSE;
	m_bCheckTo = FALSE;
	m_sEditFrom = _T("%02x ");
	m_sEditInput = _T("");
	m_sEditOutput = _T("");
	m_sEditTo = _T("%c");
	//}}AFX_DATA_INIT

//	m_cEditFrom.SetReadOnly();
//	m_cEditTo.SetReadOnly();
//	m_cEditSize.SetReadOnly();
}

CPropData::~CPropData()
{
}

void CPropData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropData)
	DDX_Control(pDX, IDC_EDIT_FROM, m_cEditFrom);
	DDX_Control(pDX, IDC_EDIT_TO, m_cEditTo);
	DDX_Check(pDX, IDC_CHECK_FROM, m_bCheckFrom);
	DDX_Check(pDX, IDC_CHECK_TO, m_bCheckTo);
	DDX_Text(pDX, IDC_EDIT_FROM, m_sEditFrom);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_sEditInput);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_sEditOutput);
	DDX_Text(pDX, IDC_EDIT_TO, m_sEditTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropData, CPropertyPage)
	//{{AFX_MSG_MAP(CPropData)
	ON_BN_CLICKED(IDC_CHECK_FROM, OnCheckFrom)
	ON_BN_CLICKED(IDC_CHECK_TO, OnCheckTo)
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPropData::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO: Add extra initialization here
	m_cEditFrom.SetReadOnly();
	m_cEditTo.SetReadOnly();
//	m_cEditSize.SetReadOnly();
	return TRUE;  // return TRUE  unless you set the focus to a control
}


/////////////////////////////////////////////////////////////////////////////
// CPropData message handlers

void CPropData::OnCheckFrom() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cEditFrom.SetReadOnly(!m_bCheckFrom);
//	m_cEditSize.SetReadOnly(!m6_bCheckFrom);
}

void CPropData::OnCheckTo() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_cEditTo.SetReadOnly(!m_bCheckTo);
}

void CPropData::OnOK() 
{
	char buff[10240] = {0};
	
	UpdateData();
	char *ps = strchr(m_sEditFrom, '%');
	char *pe = strrchr(m_sEditFrom, '%');
	char *ts = strchr(m_sEditTo, '%');
	char *te = strrchr(m_sEditTo, '%');
	
	if(ps != pe || ts != te) {
		AfxMessageBox(_T("Only one parameter allowed in the input/output format"));
		return ;
	}

	convert_from_buff(buff, sizeof(buff)-1, m_sEditInput/*, m_szEditSize*/, m_sEditFrom, m_sEditTo);
	m_sEditOutput = buff;
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_INPUT)->SetFocus();
}

void CPropData::OnButtonFile() 
{
	char buff[10240] = {0};
	FILE *fp = NULL;
	CFileDialog dlg(TRUE, _T("Open File Data to convert..."), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("All Files (*.*)|*.*||"));

	UpdateData();
	if(dlg.DoModal() != IDOK) {
		m_sEditOutput = _T("");
		goto done;
	}

	if((fp = fopen(dlg.GetFileName(), "rb")) == NULL) {
		AfxMessageBox(_T("The file doesn't exist!"));
		m_sEditOutput = _T("");
		goto done;
	}

	convert_from_file(buff, sizeof(buff)-1, fp, m_sEditFrom, m_sEditTo);
	fclose(fp);
	m_sEditOutput = buff;

done:
	UpdateData(FALSE);
	GetDlgItem(IDC_EDIT_INPUT)->SetFocus();
}
