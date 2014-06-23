// PropFSplit.cpp : implementation file
//

#include <vector>
#include "stdafx.h"
#include "SmallToolKit.h"
#include "PropFSplit.h"
#include "./core/file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern "C" int convert_to_bin(char *buff, int maxlen, const char *data/*, int size*/, const char*infmt);
/////////////////////////////////////////////////////////////////////////////
// CPropFSplit property page

IMPLEMENT_DYNCREATE(CPropFSplit, CPropertyPage)

CPropFSplit::CPropFSplit() : CPropertyPage(CPropFSplit::IDD)
{
	//{{AFX_DATA_INIT(CPropFSplit)
	m_strDstFileName = _T("");
	m_strFilePath = _T("");
	m_strKeyValue = _T("");
	m_strSize = 2048;
	m_strSuffix = _T(".%03d");
	m_bSizeOrKeyValue = 0;
	m_bSplitOrCombine = 0;
	//}}AFX_DATA_INIT

	m_dwFileDialogAttr = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
}

CPropFSplit::~CPropFSplit()
{
}

void CPropFSplit::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropFSplit)
	DDX_Control(pDX, IDC_LIST_FILE_LIST, m_lstFileList);
	DDX_Text(pDX, IDC_EDIT_DST_FILE, m_strDstFileName);
	DDX_Text(pDX, IDC_EDIT_KEY_VALUE, m_strKeyValue);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_strSize);
	DDX_Text(pDX, IDC_EDIT_SUFFIX, m_strSuffix);
	DDX_Radio(pDX, IDC_RADIO_SIZE, m_bSizeOrKeyValue);
	DDX_Radio(pDX, IDC_RADIO_SPLIT, m_bSplitOrCombine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropFSplit, CPropertyPage)
	//{{AFX_MSG_MAP(CPropFSplit)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RADIO_SPLIT, OnRadioSplit)
	ON_BN_CLICKED(IDC_RADIO_COMBINE, OnRadioCombine)
	ON_BN_CLICKED(IDC_RADIO_SIZE, OnRadioSize)
	ON_BN_CLICKED(IDC_RADIO_KERYVALUE, OnRadioKeryvalue)
	ON_BN_CLICKED(IDC_BUTTON_GETFILES, OnButtonGetfiles)
	ON_BN_CLICKED(IDC_BUTTON_DSTFILENAME, OnButtonDstfilename)
	ON_BN_CLICKED(IDC_CHECK_SUFFIX, OnCheckSuffix)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPropFSplit::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CRect rect;
	m_lstFileList.GetClientRect(&rect);
	//m_lstFileList.InsertColumn(0, _T("File Path Name"), LVCFMT_LEFT, rect.Width());
	m_lstFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_lstFileList.InsertColumn(0, _T(""), LVCFMT_LEFT, rect.Width());
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPropFSplit message handlers

void CPropFSplit::EnableSomeWindows(BOOL bValue)
{
	GetDlgItem(IDC_CHECK_SUFFIX)->EnableWindow(bValue);
	GetDlgItem(IDC_RADIO_SIZE)->EnableWindow(bValue);
	GetDlgItem(IDC_RADIO_KERYVALUE)->EnableWindow(bValue);
	((CEdit*)GetDlgItem(IDC_EDIT_SIZE))->EnableWindow(bValue);
	((CEdit*)GetDlgItem(IDC_EDIT_KEY_VALUE))->EnableWindow(bValue);
	((CEdit*)GetDlgItem(IDC_EDIT_SUFFIX))->EnableWindow(bValue);
}

void CPropFSplit::OnRadioSplit() 
{
	SetDlgItemText(IDOK, _T("&Split"));
	EnableSomeWindows(TRUE);
	m_dwFileDialogAttr = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
}

void CPropFSplit::OnRadioCombine() 
{
	SetDlgItemText(IDOK, _T("&Merge"));
	EnableSomeWindows(FALSE);
	m_dwFileDialogAttr = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
}

void CPropFSplit::OnRadioSize() 
{
	BOOL bValue = TRUE;
	((CEdit*)GetDlgItem(IDC_EDIT_SIZE))->SetReadOnly(!bValue);
	((CEdit*)GetDlgItem(IDC_EDIT_KEY_VALUE))->SetReadOnly(bValue);
}

void CPropFSplit::OnRadioKeryvalue() 
{
	BOOL bValue = TRUE;
	((CEdit*)GetDlgItem(IDC_EDIT_SIZE))->SetReadOnly(bValue);
	((CEdit*)GetDlgItem(IDC_EDIT_KEY_VALUE))->SetReadOnly(!bValue);
}

void CPropFSplit::OnCheckSuffix() 
{
	((CEdit*)GetDlgItem(IDC_EDIT_SUFFIX))->SetReadOnly(!IsDlgButtonChecked(IDC_CHECK_SUFFIX));
}

void CPropFSplit::OnButtonGetfiles() 
{
	CString strFileName;
	CFileDialog dlg(TRUE, NULL, NULL,
		m_dwFileDialogAttr, 
		_T("All Files (*.*)|*.*||"));

	if(dlg.DoModal() != IDOK) {
		return ;
	}

	UpdateData();

	if(m_bSplitOrCombine == 0) {
		m_lstFileList.DeleteAllItems();
	}

	strFileName = dlg.GetPathName();
	if(dlg.GetFileName() != _T("")) {
		m_lstFileList.InsertItem(0, strFileName);
		return ;
	}

	POSITION pos = dlg.GetStartPosition();
	while(pos != NULL) {
		m_lstFileList.InsertItem(m_lstFileList.GetItemCount(), dlg.GetNextPathName(pos));
	}
}

void CPropFSplit::OnButtonDstfilename() 
{
	CFileDialog dlg(FALSE, NULL, _T("comb.bin"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("All Files (*.*)|*.*||"));

	if(dlg.DoModal() != IDOK) {
		return ;
	}

	SetDlgItemText(IDC_EDIT_DST_FILE, dlg.GetPathName());
}

void CPropFSplit::OnOK() 
{	
	TCHAR filename[100][MAX_PATH] = {0};
	LPCTSTR lpfile[100] = {NULL};
	int index = 0;

	CPropertyPage::OnOK();
	UpdateData();

	while(m_lstFileList.GetItemText(index, 0, filename[index], MAX_PATH)) {
		if(index > 100) {
			AfxMessageBox(_T("The maximum file number is 100!"));
			return;
		}

		lpfile[index] = filename[index];
		index++;
		TRACE("index=%d, file:%s\n", index, filename[index]);
	}

	if(m_strDstFileName.GetLength() == 0) {
		AfxMessageBox(_T("Please select saved file!"));
		return ;
	}

	if(m_bSplitOrCombine == 0) { //split
		int ret = 0;
		if(m_bSizeOrKeyValue == 0) { //by size.
			ret = fsplit_by_size(filename[0], m_strSize, m_strDstFileName, m_strSuffix);
		} else { // by key value
			char key[100] = {0};
			int len = 0;

			if(m_strKeyValue.GetLength() == 0) {
				AfxMessageBox(_T("Please fill Key Value!"));
				return;
			}

			if((len = convert_to_bin(key, sizeof(key), m_strKeyValue/*, 3*/, "%02X ")) <= 0) {
				AfxMessageBox(_T("No Key value or the format is wrong!"));
				return ;
			}


			CString ext = m_strDstFileName.Right(m_strDstFileName.GetLength() - m_strDstFileName.ReverseFind('.'));
			m_strDstFileName = m_strDstFileName.Left(m_strDstFileName.ReverseFind('.')) + m_strSuffix + ext;

			ret = fsplit_by_key(filename[0], key, len, m_strDstFileName);
		}
		
		if(ret != 0) {
			AfxMessageBox(_T("Split failed!"));
		} else {
			AfxMessageBox(_T("Split successfully!"));
		}
	} else { //combine;
		if(merge_files(m_strDstFileName, lpfile) != 0) {
			AfxMessageBox(_T("Combine failed!"));
		} else {
			AfxMessageBox(_T("Combine successfully!"));
		}
	}
}

void CPropFSplit::OnButtonDelete() 
{
	int index = 0;
	
	if(m_lstFileList.GetSelectedCount() == 0) {
		return ;
	}

	if(AfxMessageBox(_T("Are you sure to delete the file name list?"), MB_YESNO) == IDNO) {
		return ;
	}

	if(m_lstFileList.GetItemCount() == (int)m_lstFileList.GetSelectedCount()) {
		m_lstFileList.DeleteAllItems();
		return;
	}

	for(index = m_lstFileList.GetItemCount()-1; index >= 0; index--) {
		if(m_lstFileList.GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED) {
			m_lstFileList.DeleteItem(index);
		}
	}
}

void CPropFSplit::OnButtonUp() 
{
	int index = 0;

	if(m_lstFileList.GetSelectedCount() == 0 || m_lstFileList.GetItemCount() <= 1) {
		return ;
	}

	while(m_lstFileList.GetItemState(index++, LVIS_SELECTED) == LVIS_SELECTED);

	for(; index < m_lstFileList.GetItemCount(); index ++) {
		if(m_lstFileList.GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED) {
			CString text = m_lstFileList.GetItemText(index, 0);
			m_lstFileList.DeleteItem(index);
			m_lstFileList.InsertItem(index-1, text);
			m_lstFileList.SetItemState(index-1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}

	m_lstFileList.SetFocus();
}

void CPropFSplit::OnButtonDown() 
{
	int index = m_lstFileList.GetItemCount() - 1;

	if(m_lstFileList.GetSelectedCount() == 0 || m_lstFileList.GetItemCount() <= 1) {
		return ;
	}
	
	while(m_lstFileList.GetItemState(index--, LVIS_SELECTED) == LVIS_SELECTED);

	for(; index >= 0; index --) {
		if(m_lstFileList.GetItemState(index, LVIS_SELECTED) == LVIS_SELECTED) {
			CString text = m_lstFileList.GetItemText(index, 0);
			m_lstFileList.DeleteItem(index);
			m_lstFileList.InsertItem(index+1, text);
			m_lstFileList.SetItemState(index+1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}

	m_lstFileList.SetFocus();
}


void CPropFSplit::OnDropFiles( HDROP hDropInfo )
{
	CString strFileName;
	char szFile[MAX_PATH];
	DWORD dwFiles = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	int index = 0;

	for(index = 0; index < dwFiles; index++) {
		DragQueryFile(hDropInfo, index, szFile, sizeof(szFile));
		strFileName = szFile;
		if(strFileName != _T("")) {
			m_lstFileList.InsertItem(m_lstFileList.GetItemCount(), strFileName);
		}
	}	
}
