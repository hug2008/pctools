// D:\Exertion\ParaAdi\3In1\DlgSrc\PropPageHeader.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageHeader.h"
#include "StringCheckEncode.h"

// CPropPageHeader dialog

#define  TIP_MOBILE_LTEXT  _T("Phone number which has a mobile header is identified as a mobile phone number. At most %02d mobile header could be set. ")

CPropPageHeader::CPropPageHeader(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageHeader::IDD, pParent)
{
}

CPropPageHeader::~CPropPageHeader()
{
}

void CPropPageHeader::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_MOBILE_HEADER_TAB, m_rcMob);
	DDX_Control(pDX, IDC_ST_MOBILE_HEADER_TIPS, m_stTips);
}


BEGIN_MESSAGE_MAP(CPropPageHeader, CStackPage)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_MOBILE_HEADER_TAB, OnMobileHeaderRvnEndItemEdit)
	ON_NOTIFY(RVN_ITEMCLICK,   IDC_PAGE_MOBILE_HEADER_TAB, OnMobileHeaderRvnItemClick)
END_MESSAGE_MAP()


// CPropPageHeader message handlers
BOOL CPropPageHeader::OnInitDialog() 
{
	CString  szTips = _T("");
	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	m_rcMob.ModifyStyle(0, RVS_SHOWHGRID|RVS_SHOWVGRID|RVS_SINGLESELECT|RVS_NOSORT|RVS_FOCUSSUBITEMS);

	InitMobileHeaderTable();
#if defined(ENABLE_MOBILE_HEAD)
	szTips.Format(TIP_MOBILE_LTEXT, MAX_MOBILE_HEADER_TOTAL);
#endif
	m_stTips.SetWindowText(szTips);

	return TRUE;
}

BOOL CPropPageHeader::OnCloseTest(void)
{
#if defined(ENABLE_MOBILE_HEAD)
	int Index           = 0;
	CString   szItem1   = _T(""),
		      szItem3   = _T(""),
		      szErrMsg  = _T("");

	UpdateData(TRUE);

	for (Index=0; Index<MAX_MOBILE_HEADER_TOTAL/2; Index++)
	{
		szItem1 = m_rcMob.GetItemText(Index, 1);
		szItem3 = m_rcMob.GetItemText(Index, 3);

		if (szItem1.GetLength() > MOBILE_HEADER_ITEM_LEN)
		{
			szErrMsg.Format(_T("The header no more than %d numbers, (No:%d)"), MOBILE_HEADER_ITEM_LEN, Index*2 + 1);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_MOBILE_HEADER);
			m_rcMob.SetFocus();
			m_rcMob.SetSelection(Index);
			MessageBox(szErrMsg, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		if (szItem3.GetLength() > MOBILE_HEADER_ITEM_LEN)
		{
			szErrMsg.Format(_T("The header no more than %d numbers, (No:%d)"), MOBILE_HEADER_ITEM_LEN, Index*2 + 2);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_MOBILE_HEADER);
			m_rcMob.SetFocus();
			m_rcMob.SetSelection(Index);
			MessageBox(szErrMsg, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		if (!IsEmptyOrNumberString(szItem1))
		{
			szErrMsg.Format(_T("The header must be numbers, (No:%d)"), Index*2 + 1);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_MOBILE_HEADER);
			m_rcMob.SetFocus();
			m_rcMob.SetSelection(Index);
			MessageBox(szErrMsg, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		if (!IsEmptyOrNumberString(szItem3))
		{
			szErrMsg.Format(_T("The header must be numbers, (No:%d)"), Index*2 + 2);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_MOBILE_HEADER);
			m_rcMob.SetFocus();
			m_rcMob.SetSelection(Index);
			MessageBox(szErrMsg, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}
	}

#endif
	return TRUE;
}

void CPropPageHeader::OnSaveData(void)
{
#if defined(ENABLE_MOBILE_HEAD)
	int Index           = 0;
	CString   szItem1   = _T(""),
		      szItem3   = _T("");

	CStringArray szAry;
	szAry.RemoveAll();

	UpdateData(TRUE);

	for (Index=0; Index<MAX_MOBILE_HEADER_TOTAL/2; Index++)
	{
		szItem1 = m_rcMob.GetItemText(Index, 1);
		szItem3 = m_rcMob.GetItemText(Index, 3);
		if (!szItem1.IsEmpty())
		{
			szAry.Add(szItem1);
		}
		if (!szItem3.IsEmpty())
		{
			szAry.Add(szItem3);
		}
	}

	for (Index=0; Index<MAX_MOBILE_HEADER_TOTAL; Index++)
	{
		if (Index < szAry.GetSize())
		{
			m_pParentWnd->m_strMobileHeader[Index] = szAry.GetAt(Index);
		}
		else
		{
			m_pParentWnd->m_strMobileHeader[Index].Empty();
		}
	}
#endif
}

void CPropPageHeader::OnRefreshPage(void)
{
#if defined(ENABLE_MOBILE_HEAD)
	int Index           = 0,
		nHalfSize       = 0;
	CString   szItem1   = _T(""),
		      szItem3   = _T("");

	CStringArray szAry;
	szAry.RemoveAll();

	for (Index=0; Index<MAX_MOBILE_HEADER_TOTAL; Index++)
	{
		if (!m_pParentWnd->m_strMobileHeader[Index].IsEmpty())
		{
			szAry.Add(m_pParentWnd->m_strMobileHeader[Index]);
		}

		if (Index < MAX_MOBILE_HEADER_TOTAL/2)
		{
			m_rcMob.SetItemText(Index, 1, _T(""));
			m_rcMob.SetItemText(Index, 3, _T(""));
		}
	}

	nHalfSize = szAry.GetSize()/2;
	for (Index=0; Index<nHalfSize; Index++)
	{
		m_rcMob.SetItemText(Index, 1, szAry[Index]);
		m_rcMob.SetItemText(Index, 3, szAry[Index + nHalfSize]);
	}

	if (szAry.GetSize()%2)
	{
		m_rcMob.SetItemText(Index, 1, szAry[nHalfSize*2]);
	}
#endif
}

void CPropPageHeader::OnLockPage(BOOL bLock)
{
	m_rcMob.EnableWindow(bLock);
}

void CPropPageHeader::InitMobileHeaderTable(void)
{
#if defined(ENABLE_MOBILE_HEAD)
	int Index           = 0;
	RVITEM   rvi;
	char     szIndex[5] = "";

	m_rcMob.InsertColumn(0,   _T("No."),       RVCF_LEFT|RVCF_TEXT|RVCF_EX_FIXEDWIDTH|RVCF_SUBITEM_NOFOCUS,  45);
	m_rcMob.InsertColumn(1,   _T("header"),    RVCF_LEFT|RVCF_TEXT|RVCF_EX_FIXEDWIDTH,  80);
	m_rcMob.InsertColumn(2,   _T("No."),       RVCF_LEFT|RVCF_TEXT|RVCF_EX_FIXEDWIDTH|RVCF_SUBITEM_NOFOCUS,  45);
	m_rcMob.InsertColumn(3,   _T("header"),    RVCF_LEFT|RVCF_TEXT|RVCF_EX_FIXEDWIDTH,  80);
	m_rcMob.DeleteAllItems();

	rvi.nMask    = RVIM_TEXT;
	rvi.lpszText = _T("");
	for (Index=0; Index<MAX_MOBILE_HEADER_TOTAL/2; Index++)
	{
		rvi.iItem     = Index;

		sprintf(szIndex, "%2.1d", Index*2+1);
		m_rcMob.InsertItem(Index,  szIndex);

		sprintf(szIndex, "%2.1d", Index*2+2);
		m_rcMob.SetItemText(Index, 2, szIndex);

		rvi.iSubItem = 1;
		m_rcMob.SetItem(&rvi);

		rvi.iSubItem = 3;
		m_rcMob.SetItem(&rvi);
	}

#endif
}

void CPropPageHeader::OnMobileHeaderRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMREPORTVIEW lpnmrv = (LPNMREPORTVIEW)pNMHDR;
	/*
	CString szSubItem = _T("");
	if (2 == lpnmrv->iSubItem)
	{
		szSubItem.Format(_T("%2.1d"), lpnmrv->iItem*2 + 2);
		m_rcMob.SetItemText(lpnmrv->iItem, 2, szSubItem);
	}
	*/
	m_pParentWnd->m_bDataChanged = TRUE;
	*pResult = 0;
}

void CPropPageHeader::OnMobileHeaderRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMREPORTVIEW lpnmrv = (LPNMREPORTVIEW)pNMHDR;
	/*
	CString szSubItem = _T("");
	if (2 == lpnmrv->iSubItem)
	{
		szSubItem.Format(_T("%2.1d"), lpnmrv->iItem*2 + 2);
		m_rcMob.SetItemText(lpnmrv->iItem, 2, szSubItem);
	}
	*/
	m_pParentWnd->m_bDataChanged = TRUE;
	*pResult = 0;
}