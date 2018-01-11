// PropPageLocalMobileSect.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageLocalMobileSect.h"

#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if defined(PAGE_CFG)
#define  TIP_LTEXT  _T("Phone number which has a local mobile number header is identified as local number. you must set the total first. ")
#else
#define  TIP_LTEXT  _T("Phone number which has a local mobile number header is identified as local number. At most %02d local mobile number header could be set. ")
#endif
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
	DDX_Control(pDX, IDC_EDIT_LMNH_COUNT, m_edCount);
	DDX_Control(pDX, IDC_ST_LMNH_COUNT_TIPS, m_stCountTip);
}


BEGIN_MESSAGE_MAP(CPropPageLocalMobileSect, CStackPage)
	//{{AFX_MSG_MAP(CPropPageLocalMobileSect)
	ON_BN_CLICKED(IDC_PAGE_5_BTN_IMPORT, OnPage5BtnImport)
	ON_BN_CLICKED(IDC_PAGE_5_BTN_EXPORT, OnPage5BtnExport)
#if defined(PAGE_CFG)
	ON_EN_CHANGE(IDC_EDIT_LMNH_COUNT, OnEnChangeEditLmnhCount)
#endif
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

#if defined(PAGE_CFG)
	m_stTips.SetWindowText(TIP_LTEXT);
	SetDlgItemInt(IDC_EDIT_LMNH_COUNT, INT_INIT_LOCAL_PHONE_ITEMNUM);
#else
	szInitStr.Format(TIP_LTEXT, INT_INIT_LOCAL_PHONE_ITEMNUM);
	m_stTips.SetWindowText(szInitStr);
	m_edCount.ShowWindow(SW_HIDE);
	m_stCountTip.ShowWindow(SW_HIDE);
#endif

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
						this
					);

	// �򿪵�·��Ϊ���������ڵ�·��
	char name[256];
	GetModuleFileName(NULL, name, 256);

	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// �ı���⣺Open - means ����ű��ļ�
	fDlg.m_ofn.lpstrTitle = _T("Import local mobile number header:");

	CString strFileName, sError;
	if ( IDOK==fDlg.DoModal() )
	{
		// ֻ�е���ʼ���룬��Ҫ��ս���
		m_rcLocalTable.DeleteAllItems();

		// ���ݵõ���·�����ļ���
		strFileName = fDlg.GetPathName();

		// ��������
		// ����ű��ļ�
		CFile inFile;
		if( !inFile.Open(strFileName,CFile::modeRead))
		{
			sError.Format(_T("Can't access the file %s!"), strFileName);
			m_pParentWnd->ShowStatus(sError, FALSE);
			//MessageBox(sError,SYS_NAME,MB_ICONSTOP);
		}
		else 
		{
			// ���txt�ļ����������ݶ��뵽�ڴ��е�CString������
			char pbuf[512];
			CString strAllSectString("");
			UINT nBytesRead;
			do
			{
				nBytesRead = inFile.Read( pbuf, 500 );
				if (nBytesRead>0)
				{
					pbuf[nBytesRead] = '\0';
					strAllSectString = strAllSectString + pbuf;
				}
			}
			while (nBytesRead>0);
			inFile.Close();

			int nSectStringLength = strAllSectString.GetLength();
			if (nSectStringLength == 0)
			{
				m_pParentWnd->ShowStatus(_T("There is nothing in this file!"), FALSE);
				return;
			}

			// �����
			CStringArray strLocalMobileSectArray;
			strLocalMobileSectArray.RemoveAll();

			// Ȼ�󽫶�������ݷֽ�ΪCStringArray�е�����
			int nPos(0), nNext(0);
			CString strTemp("");
			do
			{
				nNext = strAllSectString.Find("\r\n", nPos);
				if (nNext!=-1)
				{
					 CString string = strAllSectString.Mid(nPos, nNext-nPos);

					// �õ��Ĵ�����Ϊ��,���������һ��
					if (!string.IsEmpty())
					{
						// ȷ��ÿ�����ݵĳ��Ȳ�����16
						if (string.GetLength()>16)
						{
							m_pParentWnd->ShowStatus(_T("Err:local mobile number header is longer than 16!"), FALSE);
							return;
						}

						CPhoneNumber phone = string;
						if (!phone.IsPhoneNumber())
						{
							m_pParentWnd->ShowStatus(_T("Err:there is invalid char in this file!"), FALSE);
							return;
						}

						strLocalMobileSectArray.Add(string);
					}
					nPos = nNext+2;
				}
			}
			while(nNext!=-1);

			if (nPos < nSectStringLength)
			{
				// ����ĩβ�����һ�Σ����Լӵ�������ȥ 
				CString string = strAllSectString.Mid(nPos);

				// ---------------------------------------------------
				// ��ʹ�����һ�Σ�ҲҪ�������ݼ�飬���ܺ��� 2003.12.17 ����
				// ---------------------------------------------------

				// ȷ��ÿ�����ݵĳ��Ȳ�����16
				if (string.GetLength()>16)
				{
					m_pParentWnd->ShowStatus(_T("Err:local mobile number header is longer than 16!"), FALSE);
					return;
				}

				CPhoneNumber phone = string;
				if (!phone.IsPhoneNumber())
				{
					m_pParentWnd->ShowStatus(_T("Err:local mobile number header is longer than 16!"), FALSE);
					return;
				}

				strLocalMobileSectArray.Add(string);
			}
			
			if (strLocalMobileSectArray.GetSize() > (INT_PTR)INT_INIT_LOCAL_PHONE_ITEMNUM)
			{
				sError.Format(_T("total is more than %d!"),INT_INIT_LOCAL_PHONE_ITEMNUM);
				m_pParentWnd->ShowStatus(sError, FALSE);
				return;
			}

			m_pParentWnd->m_bDataChanged= TRUE;

			// Ȼ������������ݽ�������
			SortImportedString(strLocalMobileSectArray);

			// ��������������ʾ����
			DisplayLoadedData(strLocalMobileSectArray);

			CString strInfo;
			strInfo.Format(_T("successful import %d headers!"),strLocalMobileSectArray.GetSize());
			m_pParentWnd->ShowStatus(strInfo);
		} 
	}
}

void CPropPageLocalMobileSect::SortImportedString(CStringArray& strSortAry)
{
	// ��m_strLocalMobileSectArray�еĴ��淶�ɹ̶����ȷŵ�strTempAry��
	CStringArray strFixLengthAry;
	int nMaxSectLength = 16;		// �����ֻ��Ŷε���󳤶�
	BOOL bOperation;
	int i = 0;
	int nCount = strSortAry.GetSize();

	strFixLengthAry.RemoveAll();

	for (i=0; i< nCount; i++)
	{
		CString strOneSect = strSortAry[i];
		int nLen = strOneSect.GetLength();

		// ����16λ�ĳ���
		for (int j=0; j< 16-nLen; j++)
			strOneSect += " ";

		strFixLengthAry.Add(strOneSect);
	}
	ASSERT(strFixLengthAry.GetSize() == nCount);

	// ���濪ʼ��strFixLengthAry�е��ַ�����ʼ����
	
	do
	{
		bOperation = FALSE;
		for (i=0; i< nCount-1; i++)
		{
			CString stringI0 = strFixLengthAry[i];
			CString stringI1 = strFixLengthAry[i+1];
			if (strFixLengthAry[i]<strFixLengthAry[i+1])
			{
				// now SWAP this two string
				CString strTemp = strFixLengthAry[i];
				strFixLengthAry[i] = strFixLengthAry[i+1];
				strFixLengthAry[i+1] = strTemp;
				bOperation = TRUE;
			}
		}
	}
	while (bOperation == TRUE);

	// ȥ���ַ����еĿո�󣬸�ֵ��strSortAry
	strSortAry.RemoveAll();
	for (i=0; i< nCount; i++)
	{
		int nPos = strFixLengthAry[i].Find(' ');
		CString strTemp = strFixLengthAry[i].Left(nPos);
		strSortAry.Add(strTemp);
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
		_itoa( i+1, pszNum, 10 );
		m_rcLocalTable.InsertItem(i,pszNum);
		m_rcLocalTable.SetItemText(i,1,strSortAry[i]);
	}

	// �趨����Ϊ�Ѹ���
	m_pParentWnd->m_bDataChanged = TRUE;
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
	for (int i=0; i<nCount; i++)
	{
		strInput = m_rcLocalTable.GetItemText(i,1);
		if (!strInput.IsEmpty())
		{
			if (!strInput.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_5_LOCAL_MOBILESECT);
				m_rcLocalTable.SetFocus();
				m_rcLocalTable.SetSelection(i);

				sError.Format(_T("No.%d :[%s] is not a correct phone number string!") ,i+1, m_rcLocalTable.GetItemText(i,1));
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}

			if ( strInput.GetLength()>16)
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_5_LOCAL_MOBILESECT);
				m_rcLocalTable.SetFocus();
				m_rcLocalTable.SetSelection(i);

				sError.Format(_T("No.%d :header is longer than 16!"),i+1);
				MessageBox(sError,SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
		}
	}

	return TRUE;
}

void CPropPageLocalMobileSect::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	CStringArray* pstrAry = &m_pParentWnd->m_ArySECT_4400_strLocalTable;
	pstrAry->RemoveAll();

	// �õ������еı��غŶε�����
	int nItemNum = m_rcLocalTable.GetItemCount();
	CString strOneSect("");
	for (int i=0; i< nItemNum; i++)
	{
		strOneSect = m_rcLocalTable.GetItemText(i, 1);
		pstrAry->Add(strOneSect);
	}
}

void CPropPageLocalMobileSect::OnRefreshPage(void)
{
	// 1 -- the edit list ctrl
	m_rcLocalTable.DeleteAllItems();

	char pszNum[20];
	CString strLocalPhone("");
	int nCount = m_pParentWnd->m_ArySECT_4400_strLocalTable.GetSize();

#if defined(PAGE_CFG)
	SetDlgItemInt(IDC_EDIT_LMNH_COUNT, INT_INIT_LOCAL_PHONE_ITEMNUM);
	nCount = nCount>INT_INIT_LOCAL_PHONE_ITEMNUM?INT_INIT_LOCAL_PHONE_ITEMNUM:nCount;
#endif

	for (int i=0; i<nCount; i++)
	{
		strLocalPhone = m_pParentWnd->m_ArySECT_4400_strLocalTable[i];
		if (!strLocalPhone.IsEmpty())
		{
			// ���õ���stringд��listctrl��ȥ
			_itoa( i+1, pszNum, 10 );
			m_rcLocalTable.InsertItem(i,pszNum);
			m_rcLocalTable.SetItemText(i,1,strLocalPhone);
		}
	}
}

void CPropPageLocalMobileSect::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_5_BTN_IMPORT)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_5_BTN_EXPORT)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_5_REPORT_LOCALTABLE)->EnableWindow(bLock);
}

#if defined(PAGE_CFG)
void CPropPageLocalMobileSect::OnEnChangeEditLmnhCount()
{
	CDialog::UpdateData(TRUE);
	setPageCount(IDD_PROPPAGE_5_LOCAL_MOBILESECT, GetDlgItemInt(IDC_EDIT_LMNH_COUNT));
}
#endif
