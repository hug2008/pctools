// D:\Exertion\ParaAdi\3In1\DlgSrc\PropPageLac.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageLac.h"

#include "PhoneNumber.h"
#include "proppagelac.h"
#include "PropPageLocalMobileSect.h"
#include "..\dlginc\proppagelac.h"

// CPropPageLac dialog
CPropPageLac::CPropPageLac(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLac::IDD, pParent)
{
}

CPropPageLac::~CPropPageLac()
{
}

void CPropPageLac::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_LOCK_LAC_TABLE, m_rcLacTable);
	DDX_Control(pDX, IDC_BUT_LAC_INPORT, m_btIm);
	DDX_Control(pDX, IDC_BUT_LAC_EXPORT, m_btEx);
	DDX_Control(pDX, IDC_EDIT_LOCK_LAC_COUNT, m_eLacCount);
}


BEGIN_MESSAGE_MAP(CPropPageLac, CStackPage)
	ON_BN_CLICKED(IDC_BUT_LAC_INPORT, OnBnClickedButLacInport)
	ON_BN_CLICKED(IDC_BUT_LAC_EXPORT, OnBnClickedButLacExport)
	ON_BN_CLICKED(IDC_RADIO_LOCK_LAC_YES, OnBnClickedRadioLockLacYes)
	ON_BN_CLICKED(IDC_RADIO_LOCK_LAC_NO, OnBnClickedRadioLockLacNo)
END_MESSAGE_MAP()


// CPropPageLac message handlers
BOOL CPropPageLac::OnInitDialog()
{
	CString  szInitStr;
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	ASSERT(m_pParentWnd);

	InitLacTable();

	((CButton*)GetDlgItem(IDC_RADIO_LOCK_LAC_YES))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_LOCK_LAC_NO))->SetCheck(TRUE);
	m_eLacCount.EnableWindow(FALSE);
	m_rcLacTable.EnableWindow(FALSE);
	m_btIm.EnableWindow(FALSE);

	return TRUE;
}


BOOL CPropPageLac::OnCloseTest(void)
{
#if defined(LOCK_LAC)
	CPhoneNumber   szItem = _T("");
	int nIndex            = 0;
	CString        szErr  = _T("");

	if  ( ((CButton*)GetDlgItem(IDC_RADIO_LOCK_LAC_YES))->GetCheck()
		==
		((CButton*)GetDlgItem(IDC_RADIO_LOCK_LAC_NO))->GetCheck()
		)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_LAC);
		MessageBox(_T("Please select lock or not lock lac!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	m_eLacCount.GetWindowText(szItem);
	if (atoi(szItem) > LAC_TOTAL)
	{
		szErr.Format(_T("the count of lac can not more than %d"), LAC_TOTAL);
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_LAC);
		m_eLacCount.SetFocus();
		m_eLacCount.SetSel(0, -1);
		MessageBox(szErr, SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}

	for (nIndex=0; nIndex<LAC_TOTAL; nIndex++)
	{
		szItem = m_rcLacTable.GetItemText(nIndex, 1);

		if (szItem.IsEmpty())
		{
			continue;
		}

		if (!szItem.IsHexNumber())
		{
			szErr.Format(_T("Lac Code must be hex', [%s] %dth Item!"), szItem, nIndex+1);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_LAC);
			m_rcLacTable.SetFocus();
			m_rcLacTable.SetSelection(nIndex);
			MessageBox(szErr, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		if (4 != szItem.GetLength())
		{
			szErr.Format(_T("Lac Code must be 4 chars, [%s] %dth Item!"), szItem, nIndex+1);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_LAC);
			m_rcLacTable.SetFocus();
			m_rcLacTable.SetSelection(nIndex);
			MessageBox(szErr, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}
	}
#endif
	return TRUE;
}
void CPropPageLac::OnSaveData(void)
{
#if defined(LOCK_LAC)
	CString   szAllItem = _T(""),
		      szItem    = _T("");
	int nIndex               = 0;


	/* 数量大于 0 则打开锁定的开关8004, 负责关闭8004 */
	m_pParentWnd->szIsLacLockEnable8004 = ((CButton*)GetDlgItem(IDC_RADIO_LOCK_LAC_YES))->GetCheck() ? _T("01") : _T("00");

	m_eLacCount.GetWindowText(szItem);
	nIndex = atoi(szItem);
	nIndex = nIndex > LAC_TOTAL ? LAC_TOTAL : nIndex;
	m_pParentWnd->szLockedLacNum8005.Format(_T("%d"), nIndex);


	for (nIndex=0; nIndex<LAC_TOTAL; nIndex++)
	{
		szItem = m_rcLacTable.GetItemText(nIndex, 1);
		if (szItem.IsEmpty())
		{
			continue;
		}

		szAllItem  += szItem;
	}

	if (szAllItem.GetLength() <= (LAC_DOZEN_ONE_AT_TOTAL*4) * 1)
	{
		m_pParentWnd->szLacInf8006[0] = szAllItem;
	}
	else if (szAllItem.GetLength() <= (LAC_DOZEN_ONE_AT_TOTAL*4) * 2)
	{
		m_pParentWnd->szLacInf8006[0] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*0,   LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[1] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*1);
	}
	else if (szAllItem.GetLength() <= (LAC_DOZEN_ONE_AT_TOTAL*4) * 3)
	{
		m_pParentWnd->szLacInf8006[0] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*0,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[1] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*1,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[2] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*2);
	}
	else if (szAllItem.GetLength() <= (LAC_DOZEN_ONE_AT_TOTAL*4) * 4)
	{
		m_pParentWnd->szLacInf8006[0] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*0,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[1] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*1,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[2] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*2,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[3] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*3);
	}
	else
	{
		/* 出错了 */
		ASSERT(FALSE);

		/* 出错处理  */
		szAllItem = szAllItem.Left((LAC_DOZEN_ONE_AT_TOTAL*4) * 4);
		m_pParentWnd->szLacInf8006[0] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*0,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[1] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*1,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[2] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*2,    LAC_DOZEN_ONE_AT_TOTAL*4);
		m_pParentWnd->szLacInf8006[3] = szAllItem.Mid(LAC_DOZEN_ONE_AT_TOTAL*4*3);
	}

#endif
}
void CPropPageLac::OnRefreshPage(void)
{
#if defined(LOCK_LAC)
	int nIndex         = 0;
	BOOL bCheck        = FALSE;
	CString szOneDozen = _T("");
	CStringArray szAryLac;
	szAryLac.RemoveAll();

	nIndex = atoi(m_pParentWnd->szIsLacLockEnable8004);
	bCheck = nIndex > 0 ? TRUE : FALSE;

	((CButton*)GetDlgItem(IDC_RADIO_LOCK_LAC_YES))->SetCheck(bCheck);
	((CButton*)GetDlgItem(IDC_RADIO_LOCK_LAC_NO))->SetCheck(!bCheck);

	m_eLacCount.SetWindowText(m_pParentWnd->szLockedLacNum8005);

	for (nIndex=0; nIndex<LAC_DOZEN_COUNT_AT_TOTAL; nIndex++)
	{
		szOneDozen = m_pParentWnd->szLacInf8006[nIndex];
		while (szOneDozen.GetLength() > 3)
		{
			szAryLac.Add(szOneDozen.Left(4));
			szOneDozen = szOneDozen.Mid(4);
		}
	}

	RefreshLacTable(szAryLac);

	if (bCheck)
	{
		OnBnClickedRadioLockLacYes();
	}
	else
	{
		OnBnClickedRadioLockLacNo();
	}
#endif
}
void CPropPageLac::OnLockPage(BOOL bLock)
{
	m_rcLacTable.EnableWindow(bLock);
	m_btIm.EnableWindow(bLock);
	m_btEx.EnableWindow(bLock);
}

void CPropPageLac::InitLacTable(void)
{
	int nIndex     = 0;
	CString szItem = _T("");
	RVITEM rvItem;

	m_rcLacTable.ModifyStyle(0, RVS_SHOWHGRID|RVS_SHOWVGRID|RVS_SINGLESELECT|RVS_NOSORT|RVS_FOCUSSUBITEMS);

	m_rcLacTable.InsertColumn(0, _T("No"),        RVCF_LEFT|RVCF_TEXT|RVCF_EX_FIXEDWIDTH|RVCF_SUBITEM_NOFOCUS, 50);

	m_rcLacTable.InsertColumn(1, _T("Lac Code"),  RVCF_LEFT|RVCF_TEXT|RVCF_EX_FIXEDWIDTH,  236);

	rvItem.nMask    = RVIM_TEXT;
	rvItem.iSubItem = 1;
	rvItem.lpszText = _T("");

	for (nIndex=0; nIndex<LAC_TOTAL; nIndex++)
	{
		rvItem.iItem = nIndex;

		szItem.Format(_T("%3.1d"), nIndex+1);
		m_rcLacTable.InsertItem(nIndex, szItem);
		m_rcLacTable.SetItem(&rvItem);
	}

	return;
}

void CPropPageLac::RefreshLacTable(CStringArray &szAryLac)
{

	int nIndex     = 0;
	RVITEM rvItem;

	rvItem.nMask    = RVIM_TEXT;
	rvItem.iSubItem = 1;
	rvItem.lpszText = _T("");

	for (nIndex=0; nIndex<LAC_TOTAL; nIndex++)
	{
		rvItem.iItem = nIndex;
		m_rcLacTable.SetItem(&rvItem);
	}

	for (nIndex=0; nIndex<szAryLac.GetSize(); nIndex++)
	{
		m_rcLacTable.SetItemText(nIndex, 1, szAryLac.GetAt(nIndex));
	}
}
void CPropPageLac::OnBnClickedButLacInport()
{
#if defined(LOCK_LAC)
	// 导入账号信息
	CFileDialog fDlg(TRUE,
                     _T("txt"),
					 NULL,
					 OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
					 _T("Lac code file(*.txt)|*.txt|All File (*.*)|*.*||"),
					 this);

	int Index = 0;
	FILE *fp = NULL;
	CStringArray szArray;
	CString strFileName, sError;

	// 打开的路径为本程序所在的路径
	char name[256] = _T("");
	GetModuleFileName(NULL, name, 256);

	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// 改变标题：Open - means 导入脚本文件
	fDlg.m_ofn.lpstrTitle = _T("Import Lac Code:");

	if (IDOK == fDlg.DoModal())
	{
		// 依据得到的路径和文件名
		strFileName = fDlg.GetPathName();

		fp = fopen(strFileName, "r");
		if (NULL == fp)
		{
			path.Format(_T("can't access %s!"),strFileName);
			m_pParentWnd->ShowStatus(path, FALSE);
			return;
		}

		/* 清空原有的屈居变量数据 */
		for (Index=0; Index<LAC_DOZEN_COUNT_AT_TOTAL; Index++)
		{
			m_pParentWnd->szLacInf8006[Index].Empty();
		}

		while (!feof(fp))
		{
			memset(name, 0x00, sizeof(name));
			fscanf(fp, "%s", name);
			if ((0 == strlen(name)) || (4 < strlen(name)))
			{
				continue;
			}
			szArray.Add(name);
			if (LAC_TOTAL == szArray.GetSize())
			{
				break;
			}
		}

		fclose(fp);
		fp = NULL;

		if (szArray.GetSize() > 0)
		{
			SortImportedString(szArray, 4);
		}

		RefreshLacTable(szArray);

		CString strInfo;
		strInfo.Format(_T("successful import %d Lac Code!"),szArray.GetSize());
		m_pParentWnd->ShowStatus(strInfo);

	}
#endif
}

void CPropPageLac::OnBnClickedButLacExport()
{
#if defined(LOCK_LAC)
	CFileDialog fDlg(FALSE,
                     _T("txt"),
					 _T("lacCode.txt"),
					 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					 _T("Number header file(*.txt)|*.txt|All File (*.*)|*.*||"),
					 this);

	FILE *fp = NULL;
	char name[256];
	int nItemNum = 0, i = 0;
	GetModuleFileName(NULL, name, 256);

	CString strFileName, sError;
	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;

	// 改变标题：Save - means 导出脚本文件
	fDlg.m_ofn.lpstrTitle = _T("Export Lac Code:");

	if (IDOK==fDlg.DoModal())
	{
		// 依据得到的路径和文件名
		strFileName = fDlg.GetPathName();

		fp = fopen(strFileName, "w");

		if (!fp)
		{
			sError.Format(_T("can't access %s!"),strFileName);
			m_pParentWnd->ShowStatus(path, FALSE);
			return;
		}

		nItemNum = 0;
		i = 0;
		while (i < LAC_TOTAL)
		{
			strFileName = m_rcLacTable.GetItemText(i, 1);
			if (!strFileName.IsEmpty())
			{
				fprintf(fp, "%s\r", strFileName);
				nItemNum++;
			}
		}
		fclose(fp);
		fp = NULL;

		CString strInfo;
		strInfo.Format(_T("successful export %d Lac Code!"),nItemNum);
		m_pParentWnd->ShowStatus(strInfo);
	}
#endif
}

void CPropPageLac::OnBnClickedRadioLockLacYes()
{
	m_eLacCount.EnableWindow(TRUE);
	m_rcLacTable.EnableWindow(TRUE);
	m_btIm.EnableWindow(TRUE);
}

void CPropPageLac::OnBnClickedRadioLockLacNo()
{
#if defined(LOCK_LAC)
	int nIndex = 0;
	for (nIndex=0; nIndex<LAC_DOZEN_COUNT_AT_TOTAL; nIndex++)
	{
		m_pParentWnd->szLacInf8006[nIndex].Empty();
	}
	m_eLacCount.SetWindowText(_T("0"));
	for (nIndex=0; nIndex<LAC_TOTAL; nIndex++)
	{
		m_rcLacTable.SetItemText(nIndex, 1, _T(""));
	}
#endif
	m_eLacCount.EnableWindow(FALSE);
	m_rcLacTable.EnableWindow(FALSE);
	m_btIm.EnableWindow(FALSE);
}
