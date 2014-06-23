// PropNet.cpp : implementation file
//

#include "stdafx.h"
#include "SmallToolKit.h"
#include "PropNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropNet property page

IMPLEMENT_DYNCREATE(CPropNet, CPropertyPage)

CPropNet::CPropNet() : CPropertyPage(CPropNet::IDD)
{
	//{{AFX_DATA_INIT(CPropNet)
	m_bIsClient = 0;
	m_dwPort = 0;
	m_strHost = _T("");
	m_bIsHdrIncl = FALSE;
	m_bIsDataString = FALSE;
	m_dtGroup1 = _T("");
	m_dtGroup2 = _T("");
	m_dtGroup3 = _T("");
	m_dtGroup4 = _T("");
	m_dtGroup5 = _T("");
	m_dtGroup6 = _T("");
	m_tpSocketType = 0;
	//}}AFX_DATA_INIT
	m_bServerRun = FALSE;
}

CPropNet::~CPropNet()
{
}

void CPropNet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropNet)
	DDX_Radio(pDX, IDC_RADIO_CLIENT, m_bIsClient);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwPort);
	DDV_MinMaxDWord(pDX, m_dwPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_HOST, m_strHost);
	DDX_Check(pDX, IDC_CHECK_HDRINCL, m_bIsHdrIncl);
	DDX_Check(pDX, IDC_CHECK_STRING, m_bIsDataString);
	DDX_Text(pDX, IDC_EDIT_GROUP1, m_dtGroup1);
	DDX_Text(pDX, IDC_EDIT_GROUP2, m_dtGroup2);
	DDX_Text(pDX, IDC_EDIT_GROUP3, m_dtGroup3);
	DDX_Text(pDX, IDC_EDIT_GROUP4, m_dtGroup4);
	DDX_Text(pDX, IDC_EDIT_GROUP5, m_dtGroup5);
	DDX_Text(pDX, IDC_EDIT_GROUP6, m_dtGroup6);
	DDX_Radio(pDX, IDC_RADIO_UDP, m_tpSocketType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropNet, CPropertyPage)
	//{{AFX_MSG_MAP(CPropNet)
	ON_BN_CLICKED(IDC_RADIO_RAW, OnRadioRaw)
	ON_BN_CLICKED(IDC_RADIO_TCP, OnRadioTcp)
	ON_BN_CLICKED(IDC_RADIO_UDP, OnRadioUdp)
	ON_BN_CLICKED(IDC_RADIO_SERVER, OnRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, OnRadioClient)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropNet message handlers

void CPropNet::OnRadioRaw() 
{
	((CComboBox*)GetDlgItem(IDC_CHECK_HDRINCL))->EnableWindow(TRUE);
}

void CPropNet::OnRadioTcp() 
{
	((CComboBox*)GetDlgItem(IDC_CHECK_HDRINCL))->EnableWindow(FALSE);
}

void CPropNet::OnRadioUdp() 
{
	((CComboBox*)GetDlgItem(IDC_CHECK_HDRINCL))->EnableWindow(FALSE);	
}

void CPropNet::OnRadioServer() 
{
	UpdateData();
	((CComboBox*)GetDlgItem(IDC_RADIO_RAW))->EnableWindow(FALSE);	
	((CComboBox*)GetDlgItem(IDC_CHECK_HDRINCL))->EnableWindow(FALSE);
	m_bIsHdrIncl = FALSE;
	if(m_tpSocketType > 1) {
		m_tpSocketType = 0;
	}
	UpdateData(FALSE);
}

void CPropNet::OnRadioClient() 
{
	((CComboBox*)GetDlgItem(IDC_RADIO_RAW))->EnableWindow(TRUE);
	((CComboBox*)GetDlgItem(IDC_CHECK_HDRINCL))->EnableWindow(IsDlgButtonChecked(IDC_RADIO_RAW));
}

extern "C" int parse_data_pair(const char *data, int isStr);
extern "C" int client(int sock_type, const void *host, unsigned short port, int isHdrIncl, int timeout);
extern "C" int server(int *isExit, unsigned short port, int type);

struct server_function_struc {
	int isExit;
	int type;
	unsigned short port;
};

UINT AFX_CDECL server_thread(LPVOID args)
{
	struct server_function_struc *lpargs = (struct server_function_struc*)args;
	return server(&lpargs->isExit, lpargs->port, lpargs->type);
}

void CPropNet::OnOK() 
{
	static struct server_function_struc args;
	int ret  = 0;	
	int sock_type = 2/*SOCK_DGRAM*/;
	UpdateData();

	if(m_bServerRun) {
		args.isExit = 1;
		m_bServerRun = FALSE;
		SetDlgItemText(IDOK, _T("ST&ART"));
		return;
	}

//	GetDlgItem(IDOK)->EnableWindow(FALSE);
	SetDlgItemText(IDOK, _T("ST&OP"));
	switch (m_tpSocketType) {
	case IDC_RADIO_TCP-IDC_RADIO_UDP: sock_type = 1; /*SOCK_STREAM;*/break;
	case IDC_RADIO_RAW-IDC_RADIO_UDP: sock_type = 3; /*SOCK_RAW*/; break;
	default: break;
	}

	parse_data_pair(NULL, 0);
	if(m_dtGroup1.Compare(_T("")) != 0) parse_data_pair((LPCTSTR)m_dtGroup1, m_bIsDataString);
	if(m_dtGroup2.Compare(_T("")) != 0) parse_data_pair((LPCTSTR)m_dtGroup2, m_bIsDataString);
	if(m_dtGroup3.Compare(_T("")) != 0) parse_data_pair((LPCTSTR)m_dtGroup3, m_bIsDataString);
	if(m_dtGroup4.Compare(_T("")) != 0) parse_data_pair((LPCTSTR)m_dtGroup4, m_bIsDataString);
	if(m_dtGroup5.Compare(_T("")) != 0) parse_data_pair((LPCTSTR)m_dtGroup5, m_bIsDataString);
	if(m_dtGroup6.Compare(_T("")) != 0) parse_data_pair((LPCTSTR)m_dtGroup6, m_bIsDataString);

	if(m_bIsClient == 1) {
		args.isExit = 0;
		args.port = (unsigned short)m_dwPort;
		args.type = sock_type;
		AfxBeginThread(server_thread, (LPVOID)&args);
		m_bServerRun = TRUE;
		return ;
	}
	else {
		if((ret=client(sock_type, (LPCTSTR)m_strHost, (unsigned short)m_dwPort, m_bIsHdrIncl, 5)) != 0) {
			TCHAR *lpszMessage[] = {
				_T("Something like host, port, wrong in configuration."),
				_T("Failed When to send or receive data group 1 ."),
				_T("Failed When to send or receive data group 2 ."),
				_T("Failed When to send or receive data group 3 ."),
				_T("Failed When to send or receive data group 4 ."),
				_T("Failed When to send or receive data group 5 ."),
				_T("Failed When to send or receive data group 6 ."),
			};

			if(ret < 0) {
				ret = 0;
			}

			AfxMessageBox(lpszMessage[ret]);
		}
	}

	CPropertyPage::OnOK();
	SetDlgItemText(IDOK, _T("ST&ART"));
//	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

