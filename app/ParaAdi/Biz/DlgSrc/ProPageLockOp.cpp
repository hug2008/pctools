// E:\CvsRoot\ParaDownload_H\SRC\PAGE\ProPageLockOp.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include ".\ProPageLockOp.h"
#include "StringCheckEncode.h"

// CPropPageLockOp dialog
CPropPageLockOp::CPropPageLockOp(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLockOp::IDD, pParent)
{
}

CPropPageLockOp::~CPropPageLockOp()
{
}

void CPropPageLockOp::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_LOCK_OP_LIST, m_list);
	DDX_Control(pDX, IDC_RADI_LOCKOP_YES, m_btLockYes);
	DDX_Control(pDX, IDC_RADI_LOCKOP_NO,  m_btLockNo);
}


BEGIN_MESSAGE_MAP(CPropPageLockOp, CStackPage)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_LOCK_OP_LIST, OnRvnEndItemEdit)
	ON_BN_CLICKED(IDC_RADI_LOCKOP_YES, OnPage9RadioLockProviderYes)
	ON_BN_CLICKED(IDC_RADI_LOCKOP_NO,  OnPage9RadioLockProviderNo)
END_MESSAGE_MAP()


// CPropPageLockOp message handlers
BOOL CPropPageLockOp::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	// 增加竖线和底色
	m_list.ModifyStyle(0,
                       RVS_SHOWHGRID|
					   RVS_SHOWVGRID|
					   //RVS_SHOWCOLORALTERNATE|
					   RVS_SINGLESELECT|
					   RVS_NOSORT
					   //fndef DISABLE_INPUT
					   | RVS_FOCUSSUBITEMS
					   //ndif
					   );

	m_btLockYes.SetCheck(FALSE);
	m_btLockNo.SetCheck(TRUE);
	m_list.DeleteAllItems();

	return TRUE;
}

BOOL CPropPageLockOp::OnCloseTest(void)
{
	int  i           = 0;
	CString  szItem1 = _T(""),
		     szItem3 = _T(""),
			 szErr   = _T("");
#if defined(LOCK_SERVICE_PROVIDER)
	if (m_btLockYes.GetCheck() == m_btLockNo.GetCheck())
	{
		szErr.Format(_T("yuo must check one of the two radio 'yes' and 'no'"));
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_OP);
		MessageBox(szErr,SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	if (m_btLockNo.GetCheck())
	{
		return TRUE;
	}

	for(i=0; i<SERVICE_PROVIDER_MAX/2; i++)
	{
		szItem1 = m_list.GetItemText(i, 1);
		szItem3 = m_list.GetItemText(i, 3);

		if (!IsEmptyOrNumberString(szItem1))
		{
			szErr.Format(_T("No.%02d[%s]: Op Code must be Number!"), i+1, szItem1);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_OP);
			MessageBox(szErr,SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
		if (szItem1.GetLength() > 6)
		{
			szErr.Format(_T("No.%02d[%s]: Op Code must be 5 or 6 numbers!"), i+1, szItem1);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_OP);
			MessageBox(szErr,SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
		if (szItem1.GetLength() && (szItem1.GetLength() < 5))
		{
			szErr.Format(_T("No.%02d[%s]: Op Code must be Number!"), i+1+SERVICE_PROVIDER_MAX/2, szItem1);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_OP);
			MessageBox(szErr,SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}

		if (!IsEmptyOrNumberString(szItem3))
		{
			szErr.Format(_T("No.%02d[%s]: Op Code must be Number!"), i+1+SERVICE_PROVIDER_MAX/2, szItem3);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_OP);
			MessageBox(szErr,SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
		if (szItem3.GetLength() > 6)
		{
			szErr.Format(_T("No.%02d[%s]: Op Code must be 5 or 6 numbers!"), i+1+SERVICE_PROVIDER_MAX/2, szItem3);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_OP);
			MessageBox(szErr,SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
		if (szItem3.GetLength() && (szItem3.GetLength() < 5))
		{
			szErr.Format(_T("No.%02d[%s]: Op Code must be Number!"), i+1+SERVICE_PROVIDER_MAX/2, szItem3);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_LOCK_OP);
			MessageBox(szErr,SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
	}
#endif
	return TRUE;
}


void CPropPageLockOp::OnSaveData(void)
{
	int  i           = 0;

	m_pParentWnd->m_strLOCK_7002_bLockProvider = m_btLockYes.GetCheck() ? _T("01") : _T("00");
#if defined(LOCK_SERVICE_PROVIDER)
	if (m_btLockYes.GetCheck())
	{

		for(i=0; i<SERVICE_PROVIDER_MAX/2; i++)
		{
			m_pParentWnd->m_AryLOCK_7003_nProvider[i]                        = m_list.GetItemText(i, 1);
			m_pParentWnd->m_AryLOCK_7003_nProvider[i+SERVICE_PROVIDER_MAX/2] = m_list.GetItemText(i, 3);
		}
	}
	else
	{
		for(i=0; i<SERVICE_PROVIDER_MAX/2; i++)
		{
			m_pParentWnd->m_AryLOCK_7003_nProvider[i]                        = _T("");
			m_pParentWnd->m_AryLOCK_7003_nProvider[i+SERVICE_PROVIDER_MAX/2] = _T("");
		}
	}
	
#endif
	return;
}

void CPropPageLockOp::OnRefreshPage(void)
{
	BOOL bLockProvider  = atoi((const char*)m_pParentWnd->m_strLOCK_7002_bLockProvider) > 0;

	m_btLockYes.SetCheck(bLockProvider);
	m_btLockNo.SetCheck(!bLockProvider);

	m_list.DeleteAllItems();

	if (!bLockProvider)
	{
		return;
	}

	InitListTable();	

	RefreshListUsingProviderAry();

	return;
}

void CPropPageLockOp::OnLockPage(BOOL bLock)
{
	m_btLockYes.EnableWindow(bLock);
	m_btLockNo.EnableWindow(bLock);
	m_list.EnableWindow(bLock);
	return;
}

void CPropPageLockOp::InitListTable(void)
{
	char pszTemp[10] = "";
	int  i           = 0;
	RVITEM rvi;

	m_list.InsertColumn(0, _T("No"), 
                          RVCF_LEFT|
						  RVCF_TEXT|
						  //RVCF_EX_AUTOWIDTH|
						  //RVCF_EX_FIXEDWIDTH|
						  RVCF_SUBITEM_NOFOCUS,
						  40);

	m_list.InsertColumn(1, _T("Op Code"),
                           RVCF_LEFT|
						   RVCF_TEXT|
						   RVCF_EX_FIXEDWIDTH,
						   80);

	m_list.InsertColumn(2, _T("No"), 
                           RVCF_LEFT|
						   RVCF_TEXT|
						   RVCF_SUBITEM_NOFOCUS,
						   40);

	m_list.InsertColumn(3, _T("Op Code"),
                           RVCF_LEFT|
						   RVCF_TEXT|
						   RVCF_EX_FIXEDWIDTH,
						   80);
#if defined(LOCK_SERVICE_PROVIDER)
	for (i=0; i<SERVICE_PROVIDER_MAX/2; i++)
	{
		sprintf(pszTemp,"%02d", i+1);
		m_list.InsertItem(i, pszTemp);

		sprintf(pszTemp,"%02d", i+1+SERVICE_PROVIDER_MAX/2);
		m_list.SetItemText(i, 2, pszTemp);
	}

	for(i=0; i<SERVICE_PROVIDER_MAX/2; i++)
	{
		rvi.iItem = i;
		rvi.nMask = RVIM_TEXT;

		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_list.SetItem(&rvi);

		rvi.iSubItem = 3;
		rvi.lpszText = _T("");
		m_list.SetItem(&rvi);
	}
#endif
}

void CPropPageLockOp::CleanListTable(void)
{
	char pszTemp[10] = "";
	int  i           = 0;
	RVITEM rvi;
#if defined(LOCK_SERVICE_PROVIDER)
	for(i=0; i<SERVICE_PROVIDER_MAX/2; i++)
	{
		rvi.iItem = i;
		rvi.nMask = RVIM_TEXT;

		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_list.SetItem(&rvi);

		rvi.iSubItem = 3;
		rvi.lpszText = _T("");
		m_list.SetItem(&rvi);
	}
#endif
	return;
}
void CPropPageLockOp::RefreshListUsingProviderAry(void)
{
	TCHAR pszTemp[10]     = "";
	TCHAR *str_null       = "";
	int  i                = 0;
	RVITEM rvi;
	CString   szItemText1 = _T(""),
		      szItemText3 = _T("");
#if defined(LOCK_SERVICE_PROVIDER)
	CleanListTable();

	for(i=0; i<SERVICE_PROVIDER_MAX/2; i++)
	{
		rvi.iItem = i;
		rvi.nMask = RVIM_TEXT;

		memset(pszTemp, 0x00, sizeof(pszTemp));
		strncpy(pszTemp, 
			    m_pParentWnd->m_AryLOCK_7003_nProvider[i], 
				m_pParentWnd->m_AryLOCK_7003_nProvider[i].GetLength());
		rvi.iSubItem = 1;
		if (0 == strcmp(_T("00000"), pszTemp))
		{
			rvi.lpszText = str_null;
		}
		else
		{
			rvi.lpszText = pszTemp;
		}
		m_list.SetItem(&rvi);

		memset(pszTemp, 0x00, sizeof(pszTemp));
		strncpy(pszTemp, 
			    m_pParentWnd->m_AryLOCK_7003_nProvider[i+SERVICE_PROVIDER_MAX/2], 
				m_pParentWnd->m_AryLOCK_7003_nProvider[i+SERVICE_PROVIDER_MAX/2].GetLength());
		rvi.iSubItem = 3;
		if (0 == strcmp(_T("00000"), pszTemp))
		{
			rvi.lpszText = str_null;
		}
		else
		{
			rvi.lpszText = pszTemp;
		}
		m_list.SetItem(&rvi);
	}
#endif
	return;
}

void CPropPageLockOp::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLockOp::OnPage9RadioLockProviderYes(void)
{
	m_list.DeleteAllItems();
	InitListTable();
	RefreshListUsingProviderAry();
	return;
}

void CPropPageLockOp::OnPage9RadioLockProviderNo(void)
{
	m_list.DeleteAllItems();
	return;
}