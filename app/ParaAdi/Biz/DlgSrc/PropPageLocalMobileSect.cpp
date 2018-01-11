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

	// 增加竖线和底色
	m_rcLocalTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT  //|
//		RVS_FOCUSSUBITEMS
		);
	
	// 资源ID，或者资源名称。即：IDR_JPG_SPLASH或者153
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
	// 加入报头
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
	// 导入账号信息
	CFileDialog fDlg(	TRUE,
						_T("txt"),
						NULL,
						OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
						_T("Number header file(*.txt)|*.txt|All File (*.*)|*.*||"),
						this
					);

	// 打开的路径为本程序所在的路径
	char name[256];
	GetModuleFileName(NULL, name, 256);

	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// 改变标题：Open - means 导入脚本文件
	fDlg.m_ofn.lpstrTitle = _T("Import local mobile number header:");

	CString strFileName, sError;
	if ( IDOK==fDlg.DoModal() )
	{
		// 只有当开始导入，才要清空界面
		m_rcLocalTable.DeleteAllItems();

		// 依据得到的路径和文件名
		strFileName = fDlg.GetPathName();

		// 读出操作
		// 导入脚本文件
		CFile inFile;
		if( !inFile.Open(strFileName,CFile::modeRead))
		{
			sError.Format(_T("Can't access the file %s!"), strFileName);
			m_pParentWnd->ShowStatus(sError, FALSE);
			//MessageBox(sError,SYS_NAME,MB_ICONSTOP);
		}
		else 
		{
			// 获得txt文件，并将数据读入到内存中的CString变量中
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

			// 清空先
			CStringArray strLocalMobileSectArray;
			strLocalMobileSectArray.RemoveAll();

			// 然后将读入的数据分解为CStringArray中的数据
			int nPos(0), nNext(0);
			CString strTemp("");
			do
			{
				nNext = strAllSectString.Find("\r\n", nPos);
				if (nNext!=-1)
				{
					 CString string = strAllSectString.Mid(nPos, nNext-nPos);

					// 得到的串不能为空,否则忽略这一行
					if (!string.IsEmpty())
					{
						// 确保每个数据的长度不大于16
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
				// 还有末尾的最后一段，可以加到数据中去 
				CString string = strAllSectString.Mid(nPos);

				// ---------------------------------------------------
				// 即使是最后一段，也要进行数据检查，不能忽略 2003.12.17 修正
				// ---------------------------------------------------

				// 确保每个数据的长度不大于16
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

			// 然后对这样的数据进行排序
			SortImportedString(strLocalMobileSectArray);

			// 将排序后的数据显示出来
			DisplayLoadedData(strLocalMobileSectArray);

			CString strInfo;
			strInfo.Format(_T("successful import %d headers!"),strLocalMobileSectArray.GetSize());
			m_pParentWnd->ShowStatus(strInfo);
		} 
	}
}

void CPropPageLocalMobileSect::SortImportedString(CStringArray& strSortAry)
{
	// 将m_strLocalMobileSectArray中的串规范成固定长度放到strTempAry中
	CStringArray strFixLengthAry;
	int nMaxSectLength = 16;		// 本地手机号段的最大长度
	BOOL bOperation;
	int i = 0;
	int nCount = strSortAry.GetSize();

	strFixLengthAry.RemoveAll();

	for (i=0; i< nCount; i++)
	{
		CString strOneSect = strSortAry[i];
		int nLen = strOneSect.GetLength();

		// 补足16位的长度
		for (int j=0; j< 16-nLen; j++)
			strOneSect += " ";

		strFixLengthAry.Add(strOneSect);
	}
	ASSERT(strFixLengthAry.GetSize() == nCount);

	// 下面开始对strFixLengthAry中的字符串开始排序
	
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

	// 去除字符串中的空格后，赋值给strSortAry
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
	// 将排序后的数据显示出来

	// 加入列表项
	char pszNum[20];
	int nCount = strSortAry.GetSize();
	int i = 0;

	for (i=0; i<nCount; i++)
	{
		// 将得到的string写道listctrl中去
		_itoa( i+1, pszNum, 10 );
		m_rcLocalTable.InsertItem(i,pszNum);
		m_rcLocalTable.SetItemText(i,1,strSortAry[i]);
	}

	// 设定数据为已更改
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLocalMobileSect::OnPage5BtnExport() 
{

	// 导出脚本文件
	CFileDialog fDlg(	FALSE,
						_T("txt"),
						_T("config.txt"),
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						_T("Number header file(*.txt)|*.txt|All File (*.*)|*.*||"),
						this
					);


	// 打开的路径为本程序所在的路径
	char name[256];
	int nItemNum = 0, i = 0;
	GetModuleFileName(NULL, name, 256);

	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// 改变标题：Save - means 导出脚本文件
	fDlg.m_ofn.lpstrTitle = _T("Export local mobile number header:");

	CString strFileName, sError;
	if (IDOK==fDlg.DoModal())
	{
		// 依据得到的路径和文件名
		strFileName = fDlg.GetPathName();
		if (!m_pParentWnd->DeleteThisFile(strFileName))
		{
			sError.Format(_T("Fail to delete %s!"), strFileName);
			m_pParentWnd->ShowStatus(sError, FALSE);
		}


		// 保存操作：导出帐号信息
		CFile outFile;
		if( !outFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite ))
		{
			sError.Format("",_T("Can't create %s!"),strFileName,_T(""));
			m_pParentWnd->ShowStatus(sError, FALSE);
		}
		else 
		{
			// 得到界面中的本地号段的数量
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

	// 得到界面中的本地号段的数量
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
			// 将得到的string写道listctrl中去
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
