// PropPageLocalMobileSect.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageLocalMobileSect.h"

#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define  TIP_LTEXT  _T("Phone number which has a local mobile number header is identified as local number. At most %02d local mobile number header could be set. ")

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalMobileSect dialog


CPropPageLocalMobileSect::CPropPageLocalMobileSect(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLocalMobileSect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageLocalMobileSect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageLocalMobileSect::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocalMobileSect)
	DDX_Control(pDX, IDC_PAGE_5_BTN_IMPORT, m_btImport);
	DDX_Control(pDX, IDC_PAGE_5_BTN_EXPORT, m_btExport);
	DDX_Control(pDX, IDC_PAGE_5_REPORT_LOCALTABLE, m_rcLocalTable);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ST_LMNH_TIPS, m_stTips);
}


BEGIN_MESSAGE_MAP(CPropPageLocalMobileSect, CStackPage)
	//{{AFX_MSG_MAP(CPropPageLocalMobileSect)
	ON_BN_CLICKED(IDC_PAGE_5_BTN_IMPORT, OnPage5BtnImport)
	ON_BN_CLICKED(IDC_PAGE_5_BTN_EXPORT, OnPage5BtnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalMobileSect message handlers

BOOL CPropPageLocalMobileSect::OnInitDialog() 
{
	CString  szInitStr;

	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	// Set the tooltip string of CButtonXP button
	m_btImport.SetToolTipText(IDS_PAGE_IMPORT_LOCAL_MOBILE);
	m_btExport.SetToolTipText(IDS_PAGE_EXPORT_LOCAL_MOBILE);

	// �������ߺ͵�ɫ
	m_rcLocalTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT  //|
//		RVS_FOCUSSUBITEMS
		);
	
	// ��ԴID��������Դ���ơ�����IDR_JPG_SPLASH����153
	// m_rcLocalTable.SetBkImage(IDB_SIM);
	InitLocalMobileTable();

	szInitStr.Format(TIP_LTEXT, NUM_LOCALPHONE_HEADER_TOTAL);
	m_stTips.SetWindowText(szInitStr);

	return TRUE;
}

void CPropPageLocalMobileSect::InitLocalMobileTable()
{
	// ���뱨ͷ
	m_rcLocalTable.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
//								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS,
								50);

	m_rcLocalTable.InsertColumn(1, _T("Local mobile number header"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								236);
	
}

void CPropPageLocalMobileSect::OnPage5BtnImport() 
{
	// �����˺���Ϣ
	CFileDialog fDlg(	TRUE,
						_T("txt"),
						NULL,
						OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
						_T("Number header file(*.txt)|*.txt|All File (*.*)|*.*||"),
						this);

	int Index = 0;
	// �򿪵�·��Ϊ���������ڵ�·��
	char name[256];
	GetModuleFileName(NULL, name, 256);

	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// �ı���⣺Open - means ����ű��ļ�
	fDlg.m_ofn.lpstrTitle = _T("Import local mobile number header:");

	CString strFileName, sError;
	CStringArray strLocalMobileSectArray;
	FILE *fp = NULL;
	if (IDOK == fDlg.DoModal())
	{
		// ���ݵõ���·�����ļ���
		strFileName = fDlg.GetPathName();

		fp = fopen(strFileName, "r");
		if (NULL == fp)
		{
			path.Format(_T("can't access %s!"),strFileName);
			m_pParentWnd->ShowStatus(path, FALSE);
			return;
		}
		// ֻ�е���ʼ���룬��Ҫ��ս���
		m_rcLocalTable.DeleteAllItems();
		// ���ԭ�е�����
		for (Index=0; Index<NUM_LOCALPHONE_HEADER_TOTAL; Index++)
		{
			m_pParentWnd->szLocalPhoneNumberHead[Index].Empty();
		}

		while (!feof(fp))
		{
			memset(name, 0x00, sizeof(name));
			fscanf(fp, "%s", name);
			if ((0 == strlen(name)) || (LOCALPHONE_HEADER_LEN < strlen(name)))
			{
				continue;
			}
			strLocalMobileSectArray.Add(name);
			if (NUM_LOCALPHONE_HEADER_TOTAL == strLocalMobileSectArray.GetSize())
			{
				break;
			}
		}

		fclose(fp);
		fp = NULL;


		// Ȼ������������ݽ�������
		SortImportedString(strLocalMobileSectArray, LOCALPHONE_HEADER_LEN);
		// ��������������ʾ����
		DisplayLoadedData(strLocalMobileSectArray);
		path.Format(_T("successful import %d headers!"),strLocalMobileSectArray.GetSize());
		m_pParentWnd->ShowStatus(path);
	}
}



void CPropPageLocalMobileSect::DisplayLoadedData(CStringArray& strSortAry)
{
	// ��������������ʾ����

	// �����б���
	char pszNum[20];
	int nCount = strSortAry.GetSize();
	int i = 0;

	for (i=0; i<nCount; i++)
	{
		// ���õ���stringд��listctrl��ȥ
		sprintf(pszNum, "%3.1d", i+1);
		m_rcLocalTable.InsertItem(i,pszNum);
		m_rcLocalTable.SetItemText(i,1,strSortAry[i]);
	}
}

void CPropPageLocalMobileSect::OnPage5BtnExport() 
{

	// �����ű��ļ�
	CFileDialog fDlg(	FALSE,
						_T("txt"),
						_T("config.txt"),
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						_T("Number header file(*.txt)|*.txt|All File (*.*)|*.*||"),
						this
					);


	// �򿪵�·��Ϊ���������ڵ�·��
	char name[256];
	int nItemNum = 0, i = 0;
	GetModuleFileName(NULL, name, 256);

	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// �ı���⣺Save - means �����ű��ļ�
	fDlg.m_ofn.lpstrTitle = _T("Export local mobile number header:");

	CString strFileName, sError;
	if (IDOK==fDlg.DoModal())
	{
		// ���ݵõ���·�����ļ���
		strFileName = fDlg.GetPathName();
		if (!m_pParentWnd->DeleteThisFile(strFileName))
		{
			sError.Format(_T("Fail to delete %s!"), strFileName);
			m_pParentWnd->ShowStatus(sError, FALSE);
		}


		// ��������������ʺ���Ϣ
		CFile outFile;
		if( !outFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite ))
		{
			sError.Format("",_T("Can't create %s!"),strFileName,_T(""));
			m_pParentWnd->ShowStatus(sError, FALSE);
		}
		else 
		{
			// �õ������еı��غŶε�����
			nItemNum = m_rcLocalTable.GetItemCount();
			CString strOneSect("");
			for (i=0; i< nItemNum; i++)
			{
				strOneSect = m_rcLocalTable.GetItemText(i, 1);
				outFile.Write( strOneSect, strOneSect.GetLength());
				outFile.Write( "\r\n", 2);
			}
			outFile.Close();

			CString strInfo;
			strInfo.Format(_T("successful export %d headers!"),nItemNum);
			m_pParentWnd->ShowStatus(strInfo);
		} 
	}
}

BOOL CPropPageLocalMobileSect::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);
	
	// 1 -- the list ctrl
	CPhoneNumber strInput(""),sError("");
	int nCount = m_rcLocalTable.GetItemCount();
	int i = 0;
	for (i=0; i<nCount; i++)
	{
		strInput = m_rcLocalTable.GetItemText(i,1);
		if (!strInput.IsEmpty())
		{
			if (!strInput.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_5_LOCAL_MOBILESECT));
				m_rcLocalTable.SetFocus();
				m_rcLocalTable.SetSelection(i);

				sError.Format(_T("No.%d :[%s] is not a correct phone number string!") ,i+1, m_rcLocalTable.GetItemText(i,1));
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}

			if ( strInput.GetLength()>LOCALPHONE_HEADER_LEN)
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_5_LOCAL_MOBILESECT));
				m_rcLocalTable.SetFocus();
				m_rcLocalTable.SetSelection(i);

				sError.Format(_T("No.%d :header is longer than %d!"),i+1, LOCALPHONE_HEADER_LEN);
				MessageBox(sError,SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
		}
	}

	/************************************************************************/
	/*           �����Ϻ󱣴�һ������,�Ա�Ʒ����͵ļ��                  */
	/************************************************************************/
	for (i=0; i<NUM_LOCALPHONE_HEADER_TOTAL; i++)
	{
		m_pParentWnd->szLocalPhoneNumberHead[i].Empty();
	}
	OnSaveData();

	return TRUE;
}

void CPropPageLocalMobileSect::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	// �õ������еı��غŶε�����
	int nItemNum = m_rcLocalTable.GetItemCount();
	CString strOneSect("");
	int i = 0;
	for (i=0; i< nItemNum; i++)
	{
		strOneSect = m_rcLocalTable.GetItemText(i, 1);
		m_pParentWnd->szLocalPhoneNumberHead[i] = strOneSect;
	}
}

void CPropPageLocalMobileSect::OnRefreshPage(void)
{
	// 1 -- the edit list ctrl
	m_rcLocalTable.DeleteAllItems();

	char pszNum[20];
	CString strLocalPhone("");
	int nCount = 0,
		Index  = 0;

	for (Index=0; Index<NUM_LOCALPHONE_HEADER_TOTAL; Index++)
	{
		strLocalPhone = m_pParentWnd->szLocalPhoneNumberHead[Index];
		if (strLocalPhone.IsEmpty())
		{
			continue;
		}
		sprintf(pszNum, "%3.1d", nCount+1);
		m_rcLocalTable.InsertItem(nCount, pszNum);
		m_rcLocalTable.SetItemText(nCount,1, strLocalPhone);
		nCount++;
	}
}

void CPropPageLocalMobileSect::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_5_BTN_IMPORT)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_5_BTN_EXPORT)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_5_REPORT_LOCALTABLE)->EnableWindow(bLock);
}

void SortImportedString(CStringArray& strSortAry, const int itemMaxLen)
{
	CStringArray strFixLengthAry;
	int nMaxSectLength = itemMaxLen;
	int i = 0, j = 0;
	int nCount = strSortAry.GetSize();
	CString   szTemp  = _T("");

	strFixLengthAry.RemoveAll();

	for (i=0; i< nCount; i++)
	{
		CString strOneSect = strSortAry[i];
		int nLen = strOneSect.GetLength();

		for (int j=0; j< nMaxSectLength-nLen; j++)
			strOneSect += " ";

		strFixLengthAry.Add(strOneSect);
	}
	ASSERT(strFixLengthAry.GetSize() == nCount);

	for (i=0; i< nCount-1; i++)
	{
		for (j=0; j<nCount-i-1; j++)
		{
			if (strFixLengthAry[j] > strFixLengthAry[j+1])
			{
				szTemp = strFixLengthAry[j];
				strFixLengthAry[j]   = strFixLengthAry[j+1];
				strFixLengthAry[j+1] = szTemp;
			}
		}
	}

	// ȥ���ַ����еĿո�󣬸�ֵ��strSortAry
	strSortAry.RemoveAll();
	for (i=0; i< nCount; i++)
	{
		int nPos = strFixLengthAry[i].Find(' ');
		CString strTemp = strFixLengthAry[i].Left(nPos);
		strSortAry.Add(strTemp);
	}
}