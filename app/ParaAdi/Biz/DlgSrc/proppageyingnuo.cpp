// proppageyingnuo.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "proppageyingnuo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageYingnuo dialog


CPropPageYingnuo::CPropPageYingnuo(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageYingnuo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageYingnuo)
	for (int i=1; i<6; i++)
		for (int j=1; j<17; j++)
			m_bSec[i][j] = 0;
	//}}AFX_DATA_INIT
}


void CPropPageYingnuo::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageYingnuo)
	DDX_Control(pDX, IDC_PAGE_E_BTN_IMPORT, m_btImport);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_1 , m_bSec[1][1]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_2 , m_bSec[1][2]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_3 , m_bSec[1][3]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_4 , m_bSec[1][4]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_5 , m_bSec[1][5]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_6 , m_bSec[1][6]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_7 , m_bSec[1][7]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_8 , m_bSec[1][8]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_9 , m_bSec[1][9]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_10, m_bSec[1][10]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_11, m_bSec[1][11]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_12, m_bSec[1][12]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_13, m_bSec[1][13]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_14, m_bSec[1][14]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_15, m_bSec[1][15]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_1_16, m_bSec[1][16]);

	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_1 , m_bSec[2][1]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_2 , m_bSec[2][2]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_3 , m_bSec[2][3]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_4 , m_bSec[2][4]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_5 , m_bSec[2][5]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_6 , m_bSec[2][6]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_7 , m_bSec[2][7]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_8 , m_bSec[2][8]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_9 , m_bSec[2][9]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_10, m_bSec[2][10]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_11, m_bSec[2][11]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_12, m_bSec[2][12]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_13, m_bSec[2][13]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_14, m_bSec[2][14]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_15, m_bSec[2][15]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_2_16, m_bSec[2][16]);

	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_1 , m_bSec[3][1]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_2 , m_bSec[3][2]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_3 , m_bSec[3][3]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_4 , m_bSec[3][4]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_5 , m_bSec[3][5]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_6 , m_bSec[3][6]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_7 , m_bSec[3][7]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_8 , m_bSec[3][8]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_9 , m_bSec[3][9]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_10, m_bSec[3][10]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_11, m_bSec[3][11]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_12, m_bSec[3][12]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_13, m_bSec[3][13]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_14, m_bSec[3][14]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_15, m_bSec[3][15]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_3_16, m_bSec[3][16]);

	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_1 , m_bSec[4][1]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_2 , m_bSec[4][2]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_3 , m_bSec[4][3]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_4 , m_bSec[4][4]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_5 , m_bSec[4][5]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_6 , m_bSec[4][6]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_7 , m_bSec[4][7]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_8 , m_bSec[4][8]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_9 , m_bSec[4][9]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_10, m_bSec[4][10]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_11, m_bSec[4][11]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_12, m_bSec[4][12]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_13, m_bSec[4][13]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_14, m_bSec[4][14]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_15, m_bSec[4][15]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_4_16, m_bSec[4][16]);

	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_1 , m_bSec[5][1]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_2 , m_bSec[5][2]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_3 , m_bSec[5][3]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_4 , m_bSec[5][4]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_5 , m_bSec[5][5]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_6 , m_bSec[5][6]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_7 , m_bSec[5][7]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_8 , m_bSec[5][8]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_9 , m_bSec[5][9]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_10, m_bSec[5][10]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_11, m_bSec[5][11]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_12, m_bSec[5][12]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_13, m_bSec[5][13]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_14, m_bSec[5][14]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_15, m_bSec[5][15]);
	DDX_Text(pDX, IDC_PAGE_E_EDIT_5_16, m_bSec[5][16]);

/*	for (int i=0; i<5; i++)
		for (int j=0; j<16; j++)
			DDV_MinMaxByte(pDX, m_bSec[i][j], 0, 255);
*/
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageYingnuo, CStackPage)
	//{{AFX_MSG_MAP(CPropPageYingnuo)
	ON_BN_CLICKED(IDC_PAGE_E_BTN_IMPORT, OnImportSecurity)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageYingnuo message handlers
BOOL CPropPageYingnuo::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPropPageYingnuo::OnCloseTest(void)
{
#ifdef  _YINGNUO_SECURITY_BUILD

	// Get Input
	UpdateData(TRUE);
/*
	// 1 -- the edit
	if (!m_strProdecerID.IsNumberAlpha())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_E_YINGNUO);
		((CEdit*)GetDlgItem(IDC_PAGE_D_EDIT_PRODUCERID))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_D_EDIT_PRODUCERID)->SetFocus();

		MessageBox(_T("请输入正确的生产商ID信息！"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
*/

#endif //_YINGNUO_SECURITY_BUILD
	
	return TRUE;
}

void CPropPageYingnuo::OnSaveData(void)
{
#ifdef  _YINGNUO_SECURITY_BUILD

	// Get Input
	UpdateData(TRUE);

	int j;
	CString strOneSecurity(""), strTemp("");

	for ( j=1; j<17; j++)
	{
		strTemp.Format("%.2X", m_bSec[1][j]);
		strOneSecurity += strTemp;
	}
	m_pParentWnd->m_strYINGNUO_0020_sSecurityNum0 = strOneSecurity;
	strOneSecurity = "";

	for ( j=1; j<17; j++)
	{
		strTemp.Format("%.2X", m_bSec[2][j]);
		strOneSecurity += strTemp;
	}
	m_pParentWnd->m_strYINGNUO_0021_sSecurityNum1 = strOneSecurity;
	strOneSecurity = "";

	for ( j=1; j<17; j++)
	{
		strTemp.Format("%.2X", m_bSec[3][j]);
		strOneSecurity += strTemp;
	}
	m_pParentWnd->m_strYINGNUO_0022_sSecurityNum2 = strOneSecurity;
	strOneSecurity = "";

	for ( j=1; j<17; j++)
	{
		strTemp.Format("%.2X", m_bSec[4][j]);
		strOneSecurity += strTemp;
	}
	m_pParentWnd->m_strYINGNUO_0023_sSecurityNum3 = strOneSecurity;
	strOneSecurity = "";

	for ( j=1; j<17; j++)
	{
		strTemp.Format("%.2X", m_bSec[5][j]);
		strOneSecurity += strTemp;
	}
	m_pParentWnd->m_strYINGNUO_0024_sSecurityNum4 = strOneSecurity;
	strOneSecurity = "";

#endif  //_YINGNUO_SECURITY_BUILD
}

void CPropPageYingnuo::OnRefreshPage(void)
{
#ifdef  _YINGNUO_SECURITY_BUILD

	int j;
	CString strOneSecurity(""), strTemp("");

	strOneSecurity = m_pParentWnd->m_strYINGNUO_0020_sSecurityNum0;
	for ( j=1; j<17; j++)
	{
		strTemp = strOneSecurity.Mid(j*2-2 ,2);
		m_bSec[1][j] = ConvertHexStringToInt8(strTemp[0], strTemp[1]);
	}

	strOneSecurity = m_pParentWnd->m_strYINGNUO_0021_sSecurityNum1;
	for ( j=1; j<17; j++)
	{
		strTemp = strOneSecurity.Mid(j*2-2 ,2);
		m_bSec[2][j] = ConvertHexStringToInt8(strTemp[0], strTemp[1]);
	}

	strOneSecurity = m_pParentWnd->m_strYINGNUO_0022_sSecurityNum2;
	for ( j=1; j<17; j++)
	{
		strTemp = strOneSecurity.Mid(j*2-2 ,2);
		m_bSec[3][j] = ConvertHexStringToInt8(strTemp[0], strTemp[1]);
	}

	strOneSecurity = m_pParentWnd->m_strYINGNUO_0023_sSecurityNum3;
	for ( j=1; j<17; j++)
	{
		strTemp = strOneSecurity.Mid(j*2-2 ,2);
		m_bSec[4][j] = ConvertHexStringToInt8(strTemp[0], strTemp[1]);
	}

	strOneSecurity = m_pParentWnd->m_strYINGNUO_0024_sSecurityNum4;
	for ( j=1; j<17; j++)
	{
		strTemp = strOneSecurity.Mid(j*2-2 ,2);
		m_bSec[5][j] = ConvertHexStringToInt8(strTemp[0], strTemp[1]);
	}

#endif  //_YINGNUO_SECURITY_BUILD
	// Set the data to screen show
	UpdateData(FALSE);
}

BYTE CPropPageYingnuo::ConvertHexStringToInt8(char ch1, char ch2)
{
	BYTE bt;
	char ch;

	ch = ch1;
	ASSERT(((ch>='0')&&(ch<='9'))||((ch>='A')&&(ch<='F')));
	if (ch<='9')
		bt = ch-'0';
	else
		bt = ch-'A'+10;

	ch = ch2;
	ASSERT(((ch>='0')&&(ch<='9'))||((ch>='A')&&(ch<='F')));
	if (ch<='9')
		bt = bt*16 + ch-'0';
	else
		bt = bt*16 + ch-'A'+10;

	return bt;
}

void CPropPageYingnuo::OnImportSecurity() 
{
#ifdef  _YINGNUO_SECURITY_BUILD

	// 导入账号信息
	CFileDialog fDlg(	TRUE,
						_T("bin"),
						NULL,
						OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
						_T("密钥文件 (*.bin)|*.bin||"),
						this
					);

	// 打开的路径为本程序所在的路径
	char name[256];
	GetModuleFileName(NULL, name, 256);

	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// 改变标题：Open - means 导入脚本文件
	fDlg.m_ofn.lpstrTitle = _T("导入密钥文件:");

	CString strFileName, sError;
	CString strAllSecuString("");
	if ( IDOK==fDlg.DoModal() )
	{
		// 依据得到的路径和文件名
		strFileName = fDlg.GetPathName();

		// 读出操作
		// 导入脚本文件
		CFile inFile;
		if( !inFile.Open(strFileName,CFile::modeRead))
		{
			sError.Format("",_T("密钥文件%s无法打开！"),strFileName,_T(""));
			m_pParentWnd->ShowStatus(_T("导入密钥文件错误！"), FALSE);
			MessageBox(sError,SYS_NAME,MB_ICONSTOP);
		}
		else 
		{
			// 获得txt文件，并将数据读入到内存中的CString变量中
			char pbuf[100];
			UINT nBytesRead;
			
			nBytesRead = inFile.Read( pbuf, 100 );
			if (nBytesRead!=80)
			{
				m_pParentWnd->ShowStatus(_T("密钥文件长度不正确，导入失败！"), FALSE);
				inFile.Close();
				return;
			}

			pbuf[80] = '\0';
			inFile.Close();
			ImportInnoString(pbuf);
		}
	}
#endif // _YINGNUO_SECURITY_BUILD
}

void CPropPageYingnuo::ImportInnoString(char* pBuffer)
{
	int i, j;
	BYTE bValue;

	for ( i=1; i<6; i++)
	{
		for ( j=1; j<17; j++)
		{
			bValue = *(pBuffer+(i-1)*16+(j-1));
			m_bSec[i][j] = bValue;
		}
	}
	UpdateData(FALSE);
}

