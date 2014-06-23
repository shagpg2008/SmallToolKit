// PropCode.cpp : implementation file
//

#include "stdafx.h"
#include "SmallToolKit.h"
#include "PropCode.h"
#include <vector>

extern "C" int calc_code_lines(const char *filename, const char *lpcomment,  
					int *total, int *code, int *comment, int *blank);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GET_FILE_NAME(str) str.Right(str.GetLength()-str.ReverseFind(_T('\\'))-1)
#define GET_FILE_EXT(str)  str.Right(str.GetLength()-str.ReverseFind(_T('.'))-1)

/////////////////////////////////////////////////////////////////////////////
// CPropCode property page

IMPLEMENT_DYNCREATE(CPropCode, CPropertyPage)

CPropCode::CPropCode() : CPropertyPage(CPropCode::IDD)
{
	//{{AFX_DATA_INIT(CPropCode)
	m_bCheckSelf = FALSE;
	m_strSelf = _T("/*,*/ | //");
	//}}AFX_DATA_INIT
	m_dwIfCalced = 0;
}

CPropCode::~CPropCode()
{
}

void CPropCode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropCode)
	DDX_Control(pDX, IDC_LIST_FILE, m_strFileLst);
	DDX_Check(pDX, IDC_CHECK_SELF, m_bCheckSelf);
	DDX_Text(pDX, IDC_EDIT_SELF, m_strSelf);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropCode, CPropertyPage)
	//{{AFX_MSG_MAP(CPropCode)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_SELF, OnCheckSelf)
	ON_BN_CLICKED(ID_BUTTON_CALC, OnButtonCalc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropCode message handlers
BOOL CPropCode::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO: Add extra initialization here
	m_strFileLst.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	m_strFileLst.InsertColumn(0, _T("File Name"), LVCFMT_CENTER|LVCF_ORDER, 70);
	m_strFileLst.InsertColumn(1, _T("Total"), LVCFMT_CENTER|LVCF_ORDER, 45);
	m_strFileLst.InsertColumn(2, _T("Code"), LVCFMT_CENTER|LVCF_ORDER, 40);
	m_strFileLst.InsertColumn(3, _T("Comment"), LVCFMT_CENTER|LVCF_ORDER, 57);
	m_strFileLst.InsertColumn(4, _T("Blank"), LVCFMT_CENTER|LVCF_ORDER, 42);
	m_strFileLst.InsertColumn(5, _T("Rate"), LVCFMT_CENTER|LVCF_ORDER, 36);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPropCode::OnButtonOpen() 
{
#define MAXPATH 20480
	CString strFileName;
	CFileDialog dlg(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, 
		_T("All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrFile = new TCHAR[MAXPATH];
	memset(dlg.m_ofn.lpstrFile, 0x00, sizeof(TCHAR)*MAXPATH);
	dlg.m_ofn.nMaxFile=MAXPATH;
	if(dlg.DoModal() != IDOK) {
		delete []dlg.m_ofn.lpstrFile;
		return ;
	}

	UpdateData();

	if(m_dwIfCalced) {
		m_strFileLst.DeleteItem(m_strFileLst.GetItemCount()-1);
		m_dwIfCalced = 0;
	}

	strFileName = dlg.GetPathName();
	if(dlg.GetFileName() != _T("")) {
		m_vctPathName.push_back(strFileName);
		m_strFileLst.InsertItem(m_strFileLst.GetItemCount(), GET_FILE_NAME(strFileName));
		delete []dlg.m_ofn.lpstrFile;
		return ;
	}

	POSITION pos = dlg.GetStartPosition();
	while(pos != NULL) {
		strFileName = dlg.GetNextPathName(pos);
		m_vctPathName.push_back(strFileName);
		m_strFileLst.InsertItem(m_strFileLst.GetItemCount(), GET_FILE_NAME(strFileName));
	}

	delete []dlg.m_ofn.lpstrFile;
	((CButton*)GetDlgItem(IDOK))->SetFocus();
#undef MAXPATH
}

void CPropCode::OnButtonDelete() 
{	
	CString str;
	int index = 0;
	
	if(m_strFileLst.GetSelectedCount() == 0) {
		return ;
	}

	if(AfxMessageBox(_T("Are you sure to delete the file name list?"), MB_YESNO) == IDNO) {
		return ;
	}

	if(m_dwIfCalced) {
		m_strFileLst.DeleteItem(m_strFileLst.GetItemCount()-1);
		m_dwIfCalced = 0;
	}

	if(m_strFileLst.GetItemCount() == (int)m_strFileLst.GetSelectedCount()) {
		m_strFileLst.DeleteAllItems();
		m_vctPathName.clear();
		return;
	}

	for(index = m_strFileLst.GetItemCount()-1; index >= 0; index--) {
		if(m_strFileLst.GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED) {
			m_strFileLst.DeleteItem(index);
			m_vctPathName.erase(&m_vctPathName[index]);	
		}
	}
}

void CPropCode::OnOK() 
{
	OnButtonCalc();
	CPropertyPage::OnOK();
}

void CPropCode::OnCheckSelf() 
{
	CEdit *lpEdit = (CEdit*)GetDlgItem(IDC_EDIT_SELF);
	UpdateData();
	lpEdit->SetReadOnly(!m_bCheckSelf);
}

void CPropCode::OnButtonCalc() 
{
	int index = 0;
	int total = 0, codes = 0, comm = 0, blank = 0;
	int ttotal = 0, tcodes = 0, tcomm = 0, tblank = 0;
	char rate[64] = {0};
	char *lpFmt = NULL;
	struct lan_comments {
		char lan_dscb[40];
		char ext[60];
		char fmt[64];
	} comments[] = {
		{"C/C++/Objective-C/JavaScript/PHP", ".c, .h, .C, .hpp, .cpp, .cxx, .m, .sdl, .sst, .php", "/*,*/ | //"},
		{"Java", ".java", "// | /*,*/ | /**,*/"},
		{"Python", ".py", "# | ''','''"},
		{"Perl", ".pl", "# | =pod,=cut"},
		{"Lua",  ".lua", "-- | --[[,]]"},
		{"HTML", ".html, .htm", "<!--,-->"},
		{"C#", ".cs", "/*,*/ | // | /// | #region,#endregion"},
		{"Sed/Awk/Shell", ".sed, .awk, .sh", "#"},
		{"BAT", ".bat", "rem | REM"},
		{"VB/VBS/ASP", ".vbs, .frm", "<!--,--> | '"},
		{"Ruby", ".bat", "# | =begin,=end"},
		{"CSS",  ".css", "<!--,--> | /*,*/"},
		{"ASM/A38/P38",  ".S, .asm, .a38", ";"},
	};

	if(m_strFileLst.GetItemCount() <= 0) {
		AfxMessageBox(_T("Please open and add source files first!"));
		return ;
	}

	for(index = 0; index < m_strFileLst.GetItemCount()-m_dwIfCalced; index ++) {
		CString ext = GET_FILE_EXT(m_vctPathName[index]);
		for(int j = 0; j < sizeof(comments)/sizeof(comments[0]); j++) {
			if(strstr(comments[j].ext, ext) != NULL) {
				lpFmt = comments[j].fmt;
				break;
			}
		}

		if(j >= sizeof(comments)/sizeof(comments[0])) {
			if(!m_bCheckSelf) {
				continue;
			}

			strncpy(rate, m_strSelf, sizeof(rate));
			lpFmt = rate;
		}

		calc_code_lines(m_vctPathName[index], lpFmt, &total, &codes, &comm, &blank);
		
		ttotal += total;
		tcodes += codes;
		tcomm  += comm;
		tblank += blank;

		m_strFileLst.SetItemText(index, 1, itoa(total, rate, 10)); 
		m_strFileLst.SetItemText(index, 2, itoa(codes, rate, 10));
		m_strFileLst.SetItemText(index, 3, itoa(comm, rate, 10));
		m_strFileLst.SetItemText(index, 4, itoa(blank, rate, 10));
		sprintf(rate, "%2d%%", (comm*100) / total);
		m_strFileLst.SetItemText(index, 5, rate);
	}

	if(ttotal <= 0) {
		return ;
	}

	if(!m_dwIfCalced) {
		m_strFileLst.InsertItem(m_strFileLst.GetItemCount(), _T("Total"));
		m_dwIfCalced = 1;
	}

	m_strFileLst.SetItemText(index, 1, itoa(ttotal, rate, 10)); 
	m_strFileLst.SetItemText(index, 2, itoa(tcodes, rate, 10));
	m_strFileLst.SetItemText(index, 3, itoa(tcomm, rate, 10));
	m_strFileLst.SetItemText(index, 4, itoa(tblank, rate, 10));
	sprintf(rate, "%2d%%", ((tcomm*100)) / ttotal);
	m_strFileLst.SetItemText(index, 5, rate);
}

void CPropCode::OnDropFiles( HDROP hDropInfo )
{
	CString strFileName;
	char szFile[MAX_PATH];
	DWORD dwFiles = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	int index = 0;

	for(index = 0; index < dwFiles; index++) {
		DragQueryFile(hDropInfo, index, szFile, sizeof(szFile));
		strFileName = szFile;
		if(strFileName != _T("")) {
			m_vctPathName.push_back(strFileName);
			m_strFileLst.InsertItem(m_strFileLst.GetItemCount(), GET_FILE_NAME(strFileName));
		}
	}
	
	((CButton*)GetDlgItem(IDOK))->SetFocus();
}