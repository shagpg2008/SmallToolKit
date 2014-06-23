// PropChkSum.cpp : implementation file
//

#include "stdafx.h"
#include "SmallToolKit.h"
#include "PropChkSum.h"
//#include "WaitingDlg.h"
#include "CWaitDlgDll.h"
#include <boost/crc.hpp>
#include <cryptlib/sha.h>
#include <cryptlib/md5.h>
#include <cryptlib/sha3.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


enum {
	MessageDigest_CRC = 0,
	MessageDigest_MD5,
//	MessageDigest_SHA,
	MessageDigest_SHA1,
	MessageDigest_SHA2_224,
	MessageDigest_SHA2_256,
	MessageDigest_SHA2_384,
	MessageDigest_SHA2_512,
	MessageDigest_SHA3_224,
	MessageDigest_SHA3_256,
	MessageDigest_SHA3_384,
	MessageDigest_SHA3_512
};


extern "C" {
int convert_to_bin(char *buff, int maxlen, const char *data/*, int size*/, const char*infmt);
}

static DWORD WINAPI ThreadDoChksum(LPVOID lpArgs)
{
	CPropChkSum *lpChkSum = (CPropChkSum *)lpArgs;
	lpChkSum->OnCalc();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CPropChkSum property page

IMPLEMENT_DYNCREATE(CPropChkSum, CPropertyPage)

CPropChkSum::CPropChkSum() : CPropertyPage(CPropChkSum::IDD)
{
	//{{AFX_DATA_INIT(CPropChkSum)
	m_strCRC16 = _T("");
	m_strCRC32 = _T("");
	m_strCRC32C = _T("");
	m_strInitValue = _T("-1");
	m_strOut = _T("-1");
	m_strPoly = _T("");
	m_strCRC_CCITT = _T("");
	m_bInReflect = FALSE;
	m_bOutReflect = FALSE;
	m_dwDataWidth = 32;
	m_strSelfCRC = _T("");
	m_strData = _T("");
	//}}AFX_DATA_INIT

	m_szFileName = _T("");
}

CPropChkSum::~CPropChkSum()
{
}

void CPropChkSum::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropChkSum)
	DDX_Control(pDX, IDC_LIST_CHKSUM, m_mdList);
//	DDX_Text(pDX, IDC_EDIT_CRC16, m_strCRC16);
//	DDX_Text(pDX, IDC_EDIT_CRC32, m_strCRC32);
//	DDX_Text(pDX, IDC_EDIT_CRC32C, m_strCRC32C);
	DDX_Text(pDX, IDC_EDIT_INIT, m_strInitValue);
	DDX_Text(pDX, IDC_EDIT_OUTXOR, m_strOut);
	DDX_Text(pDX, IDC_EDIT_POLY, m_strPoly);
//	DDX_Text(pDX, IDC_EDIT_CRC_CCITT, m_strCRC_CCITT);
	DDX_Check(pDX, IDC_CHECK_IN, m_bInReflect);
	DDX_Check(pDX, IDC_CHECK_OUT, m_bOutReflect);
	DDX_Text(pDX, IDC_EDIT_CRC_WIDTH, m_dwDataWidth);
	DDV_MinMaxUInt(pDX, m_dwDataWidth, 4, 64);
	DDX_Text(pDX, IDC_EDIT_CRC, m_strSelfCRC);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strData);
	DDV_MaxChars(pDX, m_strData, 1024);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropChkSum, CPropertyPage)
	//{{AFX_MSG_MAP(CPropChkSum)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_CHECK_CUST, OnCheckCust)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropChkSum message handlers

BOOL CPropChkSum::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	CRect rect;
	m_mdList.GetClientRect(&rect);
	m_mdList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_mdList.InsertColumn(0, _T(""), LVCFMT_CENTER|LVCF_ORDER, rect.Width()-GetSystemMetrics(SM_CYHSCROLL));

	m_mdList.InsertItem(MessageDigest_CRC, _T("CRC(16/CCITT/XMDM/32): "));
	m_mdList.InsertItem(MessageDigest_MD5, _T("MD5: "));
//	m_mdList.InsertItem(MessageDigest_SHA, _T("SHA:"));
	m_mdList.InsertItem(MessageDigest_SHA1, _T("SHA1:"));
	m_mdList.InsertItem(MessageDigest_SHA2_224, _T("SHA2-224:"));
	m_mdList.InsertItem(MessageDigest_SHA2_256, _T("SHA2-256:"));
	m_mdList.InsertItem(MessageDigest_SHA2_384, _T("SHA2-384:"));
	m_mdList.InsertItem(MessageDigest_SHA2_512, _T("SHA2-512:"));
	m_mdList.InsertItem(MessageDigest_SHA3_224, _T("SHA3-224:"));
	m_mdList.InsertItem(MessageDigest_SHA3_256, _T("SHA3-256:"));
	m_mdList.InsertItem(MessageDigest_SHA3_384, _T("SHA3-384:"));
	m_mdList.InsertItem(MessageDigest_SHA3_512, _T("SHA3-512:"));
	m_mdList.SetCheck(MessageDigest_MD5, TRUE);
	m_mdList.SetCheck(MessageDigest_SHA1, TRUE);
	InitWaitDlg();
	return TRUE;
}

void CPropChkSum::OnCalc()
{
#define EACH_BLOCK_SIZE		(512<<20)
	CString strMessageDigest;
	HANDLE hFile = NULL;
	__int64 dwTotalFileSize = 0;
	__int64 dwSizeIndex = 0;
	DWORD dwFileSizeLow = 0;
	DWORD dwFileSizeHigh = 0;
	DWORD dwFileSize = 0;
	HANDLE hFileMap  = NULL;
	PVOID pvFile = NULL;
	char *pData  = NULL;
	char buff[512] = {0};

	CPropertyPage::OnOK();

	if((m_bIsFromFile && m_szFileName == _T("")) || (!m_bIsFromFile && m_strData == _T("") && m_szFileName != _T(""))) {
		AfxMessageBox(_T("Please select the file or fill the data in Hexical."));
		return ;
	}

//	bool m_bIsFromFile = m_szFileName != _T("");

	if(m_bIsFromFile) {
		hFile = CreateFile(m_szFileName, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile == INVALID_HANDLE_VALUE){
			CString str = m_szFileName + _T(" doesn't exist!");
			AfxMessageBox(str);
			return ;
		}

		dwFileSizeLow = GetFileSize(hFile, &dwFileSizeHigh);
		dwTotalFileSize = (( (__int64)dwFileSizeHigh)<<32) | dwFileSizeLow;
		hFileMap = CreateFileMapping(
							hFile, // 如果这值为INVALID_HANDLE_VALUE,是合法的，上步一定测试啊
							NULL,    // 默认安全性
							PAGE_READWRITE,    // 可读
							0, // 2个32位数示1个64位数，最大文件字节数，
							 // 高字节，文件大小小于4G时，高字节永远为0
							0,//dwFileSize, // 此为低字节，也就是最主要的参数，如果为0，取文件真实大小
							NULL);
		if (hFileMap == NULL)
		{
			CString str = _T("Mapping ") + m_szFileName + _T(" failed!");
			AfxMessageBox(str);
			CloseHandle(hFile);
			return ;
		}
/*
		dwSizeIndex = EACH_BLOCK_SIZE;
		if(dwTotalFileSize < EACH_BLOCK_SIZE) {
			dwSizeIndex = dwTotalFileSize;
		}

		pvFile = MapViewOfFile( //pvFile就是得到的指针，用它来直接操作文件
							hFileMap, 
							FILE_MAP_WRITE,     // 可读
							0, // 文件指针头位置 高字节
							0, // 文件指针头位置 低字节 必为分配粒度的整倍数,windows的粒度为64K
							dwSizeIndex);    // 要映射的文件尾，如果为0，则从指针头到真实文件尾
		if (pvFile == NULL)	{
			CString str = _T("MapViewOfFile ") + m_szFileName + _T(" failed, ERROR:!");
			str.Format(_T("MapViewOfFile %s failed, ERROR:%d!"), m_szFileName, GetLastError());
			AfxMessageBox(str);
			CloseHandle(hFileMap);
			CloseHandle(hFile);
			return ;
		}
		// 步骤4: 像操作内存一样操作文件,演示功能把整个文件倒序
		pData = (char*)pvFile;*/
	} else {
		dwTotalFileSize = dwFileSize = convert_to_bin(buff, sizeof(buff)-1, m_strData/*, m_szEditSize*/, "%02X ");
		pData = (char*)buff;
	}

#define DO_LOOP_CHKSUM(op) {	\
	if(m_bIsFromFile) {				\
		for(dwSizeIndex = 0; dwSizeIndex+EACH_BLOCK_SIZE < dwTotalFileSize; dwSizeIndex += EACH_BLOCK_SIZE) {	\
			pvFile = MapViewOfFile(hFileMap, FILE_MAP_WRITE, dwSizeIndex>>32, dwSizeIndex&0xffffffff, EACH_BLOCK_SIZE);	\
			if (pvFile == NULL)	{																		\
				CString str = _T("MapViewOfFile ") + m_szFileName + _T(" failed, ERROR:!");				\
				str.Format(_T("MapViewOfFile %s failed, ERROR:%d!"), m_szFileName, GetLastError());		\
				AfxMessageBox(str);		\
				CloseHandle(hFileMap);	\
				CloseHandle(hFile);		\
				if(dwTotalFileSize > (50<<20)) {	\
					ShowWaitTipWindow(NULL, false);	\
				}						\
				return ;				\
			}							\
			pData = (char*)pvFile;		\
			dwFileSize = EACH_BLOCK_SIZE;\
			op;							\
			UnmapViewOfFile(pvFile);	\
		}								\
		dwFileSize = dwTotalFileSize - dwSizeIndex;	\
		if(dwFileSize > 0) {						\
			pvFile = MapViewOfFile(hFileMap, FILE_MAP_WRITE, dwSizeIndex>>32, dwSizeIndex&0xffffffff, dwFileSize);\
			if (pvFile == NULL)	{																		\
				CString str = _T("MapViewOfFile ") + m_szFileName + _T(" failed, ERROR:!");				\
				str.Format(_T("MapViewOfFile %s failed, ERROR:%d!"), m_szFileName, GetLastError());		\
				AfxMessageBox(str);		\
				CloseHandle(hFileMap);	\
				CloseHandle(hFile);		\
				if(dwTotalFileSize > (50<<20)) {	\
					ShowWaitTipWindow(NULL, false);	\
				}						\
				return ;				\
			}							\
			pData = (char*)pvFile;		\
			op;							\
			UnmapViewOfFile(pvFile);	\
		}			\
	} else {		\
		op;			\
	}}				\

	if(dwTotalFileSize > (50<<20)) {
		ShowWaitTipWindow(GetSafeHwnd());
	}
	if(m_mdList.GetCheck(MessageDigest_CRC)) {
		boost::crc_16_type crc16t;
		boost::crc_ccitt_type crc_ccitt;
		boost::crc_xmodem_type  crc_xmdm;
		boost::crc_32_type crc32t;

		DO_LOOP_CHKSUM(
			(crc16t.process_bytes(pData, dwFileSize),
			crc_ccitt.process_bytes(pData, dwFileSize),
			crc_xmdm.process_bytes(pData, dwFileSize),
			crc32t.process_bytes(pData, dwFileSize))
		);

		unsigned short crc16 = crc16t.checksum();
		m_strCRC16.Format(_T("%04X"), crc16);

		crc16 = crc_ccitt.checksum();
		m_strCRC_CCITT.Format(_T("%04X"), crc16);

		crc16 = crc_xmdm.checksum();
		m_strCRC32C.Format(_T("%04X"), crc16);

		unsigned long crc32 = crc32t.checksum();
		m_strCRC32.Format(_T("%08X"), crc32);

		strMessageDigest = _T("CRC-16:") + m_strCRC16 + _T(";CCITT:") + m_strCRC_CCITT + 
			 _T(";XMDM:") + m_strCRC32C +  _T(";32:") + m_strCRC32; 
			m_mdList.SetItemText(MessageDigest_CRC, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_CRC, 0, _T("CRC(16/CCITT/XMDM/32): "));
	}

	unsigned char sha_md[120] = {0};
	int index = 0;

	if(m_mdList.GetCheck(MessageDigest_MD5)) {
		CryptoPP::Weak1::MD5 md5;
		DO_LOOP_CHKSUM(md5.Update((unsigned char *)pData, dwFileSize));
		md5.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"MD5: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15]
		);
		
		m_mdList.SetItemText(MessageDigest_MD5, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_MD5, 0, _T("MD5:"));
	}
/*
	if(m_mdList.GetCheck(MessageDigest_SHA)) {
		CryptoPP::SHA sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 
			sha_md[16], sha_md[17], sha_md[18], sha_md[19]
			);
		m_mdList.SetItemText(MessageDigest_SHA, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA, 0, _T("SHA:"));
	}
*/
	if(m_mdList.GetCheck(MessageDigest_SHA1)) {
		CryptoPP::SHA1 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA1: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 
			sha_md[16], sha_md[17], sha_md[18], sha_md[19]
			);
		m_mdList.SetItemText(MessageDigest_SHA1, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA1, 0, _T("SHA1:"));
	}

	if(m_mdList.GetCheck(MessageDigest_SHA2_224)) {
		CryptoPP::SHA224 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA2-224: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 
			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27]
			);
		m_mdList.SetItemText(MessageDigest_SHA2_224, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA2_224, 0, _T("SHA2-224:"));
	}
	
	if(m_mdList.GetCheck(MessageDigest_SHA2_256)) {
		CryptoPP::SHA256 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA2-256: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 
			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27],sha_md[28], sha_md[29], sha_md[30], sha_md[31]
			);
		m_mdList.SetItemText(MessageDigest_SHA2_256, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA2_256, 0, _T("SHA2-256:"));
	}
	
	if(m_mdList.GetCheck(MessageDigest_SHA2_384)) {
		CryptoPP::SHA384 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA2-384: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 

			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27],sha_md[28], sha_md[29], sha_md[30], sha_md[31],
			
			sha_md[32], sha_md[33], sha_md[34], sha_md[35], sha_md[36], sha_md[37], sha_md[38], sha_md[39],
			sha_md[40], sha_md[41], sha_md[42], sha_md[43],sha_md[44], sha_md[45], sha_md[46], sha_md[47]
			);
		m_mdList.SetItemText(MessageDigest_SHA2_384, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA2_384, 0, _T("SHA2-384:"));
	}

	if(m_mdList.GetCheck(MessageDigest_SHA2_512)) {
		CryptoPP::SHA512 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA2-512: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 

			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27],sha_md[28], sha_md[29], sha_md[30], sha_md[31],
			
			sha_md[32], sha_md[33], sha_md[34], sha_md[35], sha_md[36], sha_md[37], sha_md[38], sha_md[39],
			sha_md[40], sha_md[41], sha_md[42], sha_md[43],sha_md[44], sha_md[45], sha_md[46], sha_md[47],
			
			sha_md[48], sha_md[49], sha_md[50], sha_md[51], sha_md[52], sha_md[53], sha_md[54], sha_md[55],
			sha_md[56], sha_md[57], sha_md[58], sha_md[59],sha_md[60], sha_md[61], sha_md[62], sha_md[63]
			);
		m_mdList.SetItemText(MessageDigest_SHA2_512, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA2_512, 0, _T("SHA2-512:"));
	}

	if(m_mdList.GetCheck(MessageDigest_SHA3_224)) {
		CryptoPP::SHA3_224 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA3-224: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 

			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27]
			);
		m_mdList.SetItemText(MessageDigest_SHA3_224, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA3_224, 0, _T("SHA3-224:"));
	}

	if(m_mdList.GetCheck(MessageDigest_SHA3_256)) {
		CryptoPP::SHA3_256 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA3-256: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 
			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27],sha_md[28], sha_md[29], sha_md[30], sha_md[31]
			);
		m_mdList.SetItemText(MessageDigest_SHA3_256, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA3_256, 0, _T("SHA3-256:"));
	}

	if(m_mdList.GetCheck(MessageDigest_SHA3_384)) {
		CryptoPP::SHA3_384 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA3-384: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 

			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27],sha_md[28], sha_md[29], sha_md[30], sha_md[31],
			
			sha_md[32], sha_md[33], sha_md[34], sha_md[35], sha_md[36], sha_md[37], sha_md[38], sha_md[39],
			sha_md[40], sha_md[41], sha_md[42], sha_md[43],sha_md[44], sha_md[45], sha_md[46], sha_md[47]
			);
		m_mdList.SetItemText(MessageDigest_SHA3_384, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA3_384, 0, _T("SHA3-384:"));
	}

	if(m_mdList.GetCheck(MessageDigest_SHA3_512)) {
		CryptoPP::SHA3_512 sha;
		DO_LOOP_CHKSUM(sha.Update((unsigned char *)pData, dwFileSize));
		sha.Final((byte*)&sha_md);
		strMessageDigest.Format( 
			"SHA3-512: %02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X"
			"%02X%02X%02X%02X%02X%02X%02X%02X",
			sha_md[0], sha_md[1], sha_md[2], sha_md[3],sha_md[4], sha_md[5], sha_md[6], sha_md[7], 
			sha_md[8], sha_md[9], sha_md[10], sha_md[11],sha_md[12], sha_md[13], sha_md[14], sha_md[15], 

			sha_md[16], sha_md[17], sha_md[18], sha_md[19], sha_md[20], sha_md[21], sha_md[22], sha_md[23],
			sha_md[24], sha_md[25], sha_md[26], sha_md[27],sha_md[28], sha_md[29], sha_md[30], sha_md[31],
			
			sha_md[32], sha_md[33], sha_md[34], sha_md[35], sha_md[36], sha_md[37], sha_md[38], sha_md[39],
			sha_md[40], sha_md[41], sha_md[42], sha_md[43],sha_md[44], sha_md[45], sha_md[46], sha_md[47],
			
			sha_md[48], sha_md[49], sha_md[50], sha_md[51], sha_md[52], sha_md[53], sha_md[54], sha_md[55],
			sha_md[56], sha_md[57], sha_md[58], sha_md[59],sha_md[60], sha_md[61], sha_md[62], sha_md[63]
			);
		m_mdList.SetItemText(MessageDigest_SHA3_512, 0, strMessageDigest);
	} else {
	    m_mdList.SetItemText(MessageDigest_SHA3_512, 0, _T("SHA3-512:"));
	}
	
	//self define crc mode.
	if(m_strPoly != _T("") && m_dwDataWidth) {
		char *lptmp = NULL;
		const size_t bits = (size_t)m_dwDataWidth;
		const unsigned long poly = strtoul(m_strPoly, &lptmp, 16);
		const unsigned long init = strtoul(m_strInitValue, &lptmp, 16);
		const unsigned long out  = strtoul(m_strOut, &lptmp, 16);
		const bool inf = m_bInReflect?true:false;
		const bool outf = m_bOutReflect?true:false;
		unsigned long rem = 0;

		if(!((poly == ULONG_MAX && errno == ERANGE) || *lptmp != 0)) {
	//		boost::crc_optimal<bits, poly, init, out, inf, ino> crc_self;
			switch (bits) {
#define CALC_CRC_BITS(nbits)	\
			case nbits:  {		\
				boost::crc_basic<nbits> crc(poly, init, out, inf, outf);\
				DO_LOOP_CHKSUM(crc.process_bytes((unsigned char *)pData, dwFileSize));	\
				rem = crc.checksum();	\
				break;					\
				}
			CALC_CRC_BITS(1);	CALC_CRC_BITS(2);	CALC_CRC_BITS(3);
			CALC_CRC_BITS(4);	CALC_CRC_BITS(5);	CALC_CRC_BITS(6);
			CALC_CRC_BITS(7);	CALC_CRC_BITS(8);	CALC_CRC_BITS(9);
			CALC_CRC_BITS(10);	CALC_CRC_BITS(11);	CALC_CRC_BITS(12);
			CALC_CRC_BITS(13);	CALC_CRC_BITS(14);	CALC_CRC_BITS(15);
			CALC_CRC_BITS(16);	CALC_CRC_BITS(17);	CALC_CRC_BITS(18);
			CALC_CRC_BITS(19);	CALC_CRC_BITS(20);	CALC_CRC_BITS(21);
			CALC_CRC_BITS(22);	CALC_CRC_BITS(23);	CALC_CRC_BITS(24);
			CALC_CRC_BITS(25);	CALC_CRC_BITS(26);	CALC_CRC_BITS(27);
			CALC_CRC_BITS(28);	CALC_CRC_BITS(29);	CALC_CRC_BITS(30);
			CALC_CRC_BITS(31);	CALC_CRC_BITS(32);/*	CALC_CRC_BITS(33);
			CALC_CRC_BITS(34);	CALC_CRC_BITS(35);	CALC_CRC_BITS(36);
			CALC_CRC_BITS(37);	CALC_CRC_BITS(38);	CALC_CRC_BITS(39);
			CALC_CRC_BITS(40);	CALC_CRC_BITS(41);	CALC_CRC_BITS(42);
			CALC_CRC_BITS(43);	CALC_CRC_BITS(44);	CALC_CRC_BITS(45);
			CALC_CRC_BITS(46);	CALC_CRC_BITS(47);	CALC_CRC_BITS(48);
			CALC_CRC_BITS(49);	CALC_CRC_BITS(50);	CALC_CRC_BITS(51);
			CALC_CRC_BITS(52);	CALC_CRC_BITS(53);	CALC_CRC_BITS(54);
			CALC_CRC_BITS(55);	CALC_CRC_BITS(56);	CALC_CRC_BITS(57);
			CALC_CRC_BITS(58);	CALC_CRC_BITS(59);	CALC_CRC_BITS(60);
			CALC_CRC_BITS(61);	CALC_CRC_BITS(62);	CALC_CRC_BITS(63);
			CALC_CRC_BITS(64);	CALC_CRC_BITS(65);	CALC_CRC_BITS(66);
			CALC_CRC_BITS(67);	CALC_CRC_BITS(68);	CALC_CRC_BITS(69);*/
			default: break;
			}

			m_strSelfCRC.Format(_T("%%0%dX"), bits / 4);
			strMessageDigest.Format(m_strSelfCRC, rem);
			m_strSelfCRC = strMessageDigest;
#undef CALC_CRC_BITS
		}
	}
#undef DO_LOOP_CHKSUM

	if(dwTotalFileSize > (50<<20)) {
		ShowWaitTipWindow(NULL, false);
	}

	UpdateData(FALSE);
	if(m_bIsFromFile) {
		UnmapViewOfFile(pvFile);
		CloseHandle(hFileMap);
		CloseHandle(hFile);
	}
	return ;
}

void CPropChkSum::OnOK()
{
	UpdateData();
	//OnCalc(m_strData == _T(""));
	m_bIsFromFile = m_strData == _T("");
	CreateThread(NULL, 0, ThreadDoChksum, (LPVOID)this, NULL, NULL);
}

void CPropChkSum::OnButtonBrowse() 
{
	//CString strFileName;
	CFileDialog dlg(TRUE, NULL, NULL,
		0, 
		_T("All Files (*.*)|*.*||"));

	if(dlg.DoModal() != IDOK) {
		return ;
	}

	m_szFileName = dlg.GetPathName();
	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(m_szFileName);
	//OnCalc(true);
	m_bIsFromFile = true;
	CreateThread(NULL, 0, ThreadDoChksum, (LPVOID)this, NULL, NULL);
}


void CPropChkSum::OnDropFiles( HDROP hDropInfo )
{
	char szFile[MAX_PATH];
	DragQueryFile(hDropInfo, 0, szFile, sizeof(szFile));
	m_szFileName = szFile;
	GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(m_szFileName);
	UpdateData();
	m_bIsFromFile = true;
	CreateThread(NULL, 0, ThreadDoChksum, (LPVOID)this, NULL, NULL);
	//OnCalc();
}

void CPropChkSum::OnCheckCust() 
{
	BOOL bChecked = ((CButton*)GetDlgItem(IDC_CHECK_CUST))->GetCheck();
	GetDlgItem(IDC_CHECK_IN)->EnableWindow(bChecked);
	GetDlgItem(IDC_CHECK_OUT)->EnableWindow(bChecked);
	GetDlgItem(IDC_EDIT_CRC_WIDTH)->EnableWindow(bChecked);
	GetDlgItem(IDC_EDIT_POLY)->EnableWindow(bChecked);
	GetDlgItem(IDC_EDIT_INIT)->EnableWindow(bChecked);
	GetDlgItem(IDC_EDIT_OUTXOR)->EnableWindow(bChecked);
}
