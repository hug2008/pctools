// D:\Para\SRC\PAGE\PropPageOpnCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageOpnCfg.h"
#include "StringCheckEncode.h"

#define  INVALID_MCC_MNC_STR      _T("FFF")

BOOL strIsValidOpnCfg(CString strOpnCfg)
{
	CString szOpnCfg = strOpnCfg;
	// 1. must be hex string
	if (!IsHexString(szOpnCfg))
		return FALSE;

	// 2. len more than 4*3 chars
	if (13 > szOpnCfg.GetLength())
		return FALSE;

	// 3. network name must be ucs2 len
	if ((szOpnCfg.GetLength() - 12) % 4)
		return FALSE;

	// 4. too longer
	if (szOpnCfg.GetLength() > (12 + OPN_LEN*4))
		return FALSE;

	return TRUE;
}

void SwapString(CString &str1, CString &str2)
{
	CString sztemp = str1;
	str1 = str2;
	str2 = sztemp;
	return;
}

// CPropPageOpnCfg 对话框
CPropPageOpnCfg::CPropPageOpnCfg(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageOpnCfg::IDD, pParent)
{

}

CPropPageOpnCfg::~CPropPageOpnCfg()
{
}

void CPropPageOpnCfg::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_OPNCFG_LIST, m_rcOpnList);
	DDX_Control(pDX, IDC_CHECK_OPNCFG_ENABLE, m_ckEnable);
}


BEGIN_MESSAGE_MAP(CPropPageOpnCfg, CStackPage)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_OPNCFG_LIST, OnRvnEndItemEdit)
#if defined(PAGE_CFG)
	ON_BN_CLICKED(IDC_CHECK_OPNCFG_ENABLE, OnBnClickedCheckOpncfgEnable)
#endif
END_MESSAGE_MAP()


// CPropPageOpnCfg 消息处理程序
BOOL CPropPageOpnCfg::OnInitDialog(void)
{
	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	ASSERT(m_pParentWnd);

	m_rcOpnList.ModifyStyle(0,
		                    RVS_SHOWHGRID|
							RVS_SHOWVGRID|
							//		RVS_SHOWCOLORALTERNATE|
							RVS_SINGLESELECT|
							RVS_NOSORT
							//#ifndef DISABLE_INPUT
							| RVS_FOCUSSUBITEMS
							//#endif
							);

	InitOpnList();

#if defined(PAGE_CFG)
	m_ckEnable.SetCheck(ENABLE_OPN_CFG);
	m_rcOpnList.EnableWindow(ENABLE_OPN_CFG);
	if (!ENABLE_OPN_CFG)
	{
		resetOpnList(0);
	}
#else
	m_ckEnable.ShowWindow(SW_HIDE);
#endif

	return TRUE;
}

void CPropPageOpnCfg::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_pParentWnd->m_bDataChanged = TRUE;
}
#define MAX_FFF (3)
BOOL CPropPageOpnCfg::OnCloseTest(void)
{
	int nItem = 0;

	CString sError("");

	char szUcs2Opn[OPN_LEN*4*4 + 1] = _T("");

#if defined(PAGE_CFG)
	if (!ENABLE_OPN_CFG)
		return TRUE;
#endif

	getOpnCfgList();

	for (nItem=0; nItem<m_rcOpnList.GetItemCount(); nItem++)
	{
		memset(szUcs2Opn, '\0', sizeof(szUcs2Opn));
		// Mcc *********************************************************************************
		if (!IsEmptyOrHexString(m_OpCfg[nItem].szMcc))
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mcc must be hex string!", nItem+1);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}
		if (MAX_FFF < m_OpCfg[nItem].szMcc.GetLength())
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mcc no more than %d chars!", nItem+1, MAX_FFF);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		// Mnc1 *********************************************************************************
		if (!IsEmptyOrHexString(m_OpCfg[nItem].szMnc1))
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mnc1 must be hex string!", nItem+1);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}
		if (MAX_FFF < m_OpCfg[nItem].szMnc1.GetLength())
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mnc1 no more than %d chars!", nItem+1, MAX_FFF);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		// Mnc2 *********************************************************************************
		if (!IsEmptyOrHexString(m_OpCfg[nItem].szMnc2))
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mnc2 must be hex string!", nItem+1);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}
		if (MAX_FFF < m_OpCfg[nItem].szMnc2.GetLength())
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mnc2 no more than %d chars!", nItem+1, MAX_FFF);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		// Mnc3 *********************************************************************************
		if (!IsEmptyOrHexString(m_OpCfg[nItem].szMnc3))
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mnc3 must be hex string!", nItem+1);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}
		if (MAX_FFF < m_OpCfg[nItem].szMnc3.GetLength())
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("No.%02d:Mnc3 no more than %d chars!", nItem+1, MAX_FFF);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		// check if all mnc is empty
		if (m_OpCfg[nItem].szMnc1.IsEmpty() && m_OpCfg[nItem].szMnc2.IsEmpty() && m_OpCfg[nItem].szMnc1.IsEmpty())
		{// check if mcc is empty
			if (m_OpCfg[nItem].szMcc.IsEmpty())
				m_OpCfg[nItem].szMcc = INVALID_MCC_MNC_STR;			
		}
		else
		{
			if (m_OpCfg[nItem].szMcc.IsEmpty())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
				m_rcOpnList.SetFocus();
				m_rcOpnList.SetSelection(nItem);
				sError.Format(_T("No.%02d:Mcc must be set if the Mnc has been set!"), nItem+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);
				return FALSE;
			}
		}

		//one of mnc1, mnc2, mnc3 is empty,put it at the end and endow with INVALID_MCC_MNC_STR;
		if (m_OpCfg[nItem].szMnc1.IsEmpty())
		{
			if (m_OpCfg[nItem].szMnc2.IsEmpty())
			{
				if (m_OpCfg[nItem].szMnc3.IsEmpty())
				{// endow with INVALID_MCC_MNC_STR
					m_OpCfg[nItem].szMnc1 = INVALID_MCC_MNC_STR;
					m_OpCfg[nItem].szMnc2 = INVALID_MCC_MNC_STR;
					m_OpCfg[nItem].szMnc3 = INVALID_MCC_MNC_STR;
				}
				else
				{// Mnc1 <==> Mnc3
					SwapString(m_OpCfg[nItem].szMnc1, m_OpCfg[nItem].szMnc3);
					m_OpCfg[nItem].szMnc2 = INVALID_MCC_MNC_STR;
					m_OpCfg[nItem].szMnc3 = INVALID_MCC_MNC_STR;
				}
			}
			else if (m_OpCfg[nItem].szMnc3.IsEmpty())
			{// Mnc1 <==> Mnc2
				SwapString(m_OpCfg[nItem].szMnc1, m_OpCfg[nItem].szMnc2);
				m_OpCfg[nItem].szMnc2 = INVALID_MCC_MNC_STR;
				m_OpCfg[nItem].szMnc3 = INVALID_MCC_MNC_STR;
			}
			else
			{// Mnc1 <==> Mnc2  ->  Mnc2 <==> Mnc3
				SwapString(m_OpCfg[nItem].szMnc1, m_OpCfg[nItem].szMnc2);
				SwapString(m_OpCfg[nItem].szMnc2, m_OpCfg[nItem].szMnc3);
				m_OpCfg[nItem].szMnc3 = INVALID_MCC_MNC_STR;
			}
		}
		else if (m_OpCfg[nItem].szMnc2.IsEmpty())
		{
			if (m_OpCfg[nItem].szMnc3.IsEmpty())
			{// endow with INVALID_MCC_MNC_STR
				m_OpCfg[nItem].szMnc2 = INVALID_MCC_MNC_STR;
				m_OpCfg[nItem].szMnc3 = INVALID_MCC_MNC_STR;
			}
			else
			{// Mnc2 <==> Mnc3
				SwapString(m_OpCfg[nItem].szMnc2, m_OpCfg[nItem].szMnc3);
				m_OpCfg[nItem].szMnc3 = INVALID_MCC_MNC_STR;
			}
		}
		else
		{// endow with INVALID_MCC_MNC_STR
			m_OpCfg[nItem].szMnc3 = INVALID_MCC_MNC_STR;
		}

		//NetWork Name (18 Ucs2) ****************************************************************
		AscEncodeHexUcs2(m_OpCfg[nItem].szOpName, szUcs2Opn);
		if (OPN_LEN < strlen(szUcs2Opn)/4)
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_OPN_CFG);
			m_rcOpnList.SetFocus();
			m_rcOpnList.SetSelection(nItem);
			sError.Format("NetWork Name no more than 18 chars,(%dth)!", nItem+1);
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);
			return FALSE;
		}

		// 把编码后的数据保存
		MakeUpperString(szUcs2Opn);
		m_OpCfg[nItem].szOpName = szUcs2Opn;
	}

	return TRUE;
}
#undef MAX_FFF
void CPropPageOpnCfg::OnSaveData(void)
{
	int nItem = 0;

	char szMcc[4]  = "",
		 szMnc1[4] = "",
		 szMnc2[4] = "",
		 szMnc3[4] = "";

#if defined(PAGE_CFG)
	if (!ENABLE_OPN_CFG)
		return;
#endif

	if (!OnCloseTest())
		return;

	for (nItem=0; nItem<OPN_TOTAL; nItem++)
		m_pParentWnd->m_AryNetWorkName_2750_strNameList[nItem].Empty();

	for (nItem=0; nItem<m_rcOpnList.GetItemCount(); nItem++)
	{
		memset(szMcc,  '\0', sizeof(szMcc));
		strcpy(szMcc,  m_OpCfg[nItem].szMcc);

		memset(szMnc1, '\0', sizeof(szMnc1));
		strcpy(szMnc1, m_OpCfg[nItem].szMnc1);

		memset(szMnc2, '\0', sizeof(szMnc2));
		strcpy(szMnc2, m_OpCfg[nItem].szMnc2);

		memset(szMnc3, '\0', sizeof(szMnc3));
		strcpy(szMnc3, m_OpCfg[nItem].szMnc3);

		m_pParentWnd->m_AryNetWorkName_2750_strNameList[nItem].Format("%03X%03X%03X%03X%s", 
			                                                           HexstrToInt(szMcc),
																	   HexstrToInt(szMnc1),
																	   HexstrToInt(szMnc2),
																	   HexstrToInt(szMnc3),
																	   m_OpCfg[nItem].szOpName);
	}

}

void CPropPageOpnCfg::OnRefreshPage(void)
{
	int nItem = 0,
		index = 0;
	CString szItem = _T("");
	char szUcs2[OPN_LEN*8] = {'\0'};

#if defined(PAGE_CFG)
	BOOL bCheck = ENABLE_OPN_CFG;
	m_ckEnable.SetCheck(bCheck);
	// 1. clear the list
	if (bCheck)
		resetOpnList(OPN_TOTAL);
	else
		resetOpnList(0);

	// 2. draw the text in the list
	nItem = 0;
	for (index=0; index<m_rcOpnList.GetItemCount()/*OPN_TOTAL*/; index++)
#else

	// 1. clear the list
	resetOpnList(OPN_TOTAL);
	// 2. draw the text in the list
	nItem = 0;
	for (index=0; index<OPN_TOTAL; index++)
#endif
	{
		szItem = m_pParentWnd->m_AryNetWorkName_2750_strNameList[index];
		if (strIsValidOpnCfg(szItem))
		{
			m_rcOpnList.SetItemText(nItem, 1, szItem.Mid(0, 3));//Mcc
			m_rcOpnList.SetItemText(nItem, 2, szItem.Mid(3, 3));//Mnc1
			m_rcOpnList.SetItemText(nItem, 3, szItem.Mid(6, 3));//Mnc2
			m_rcOpnList.SetItemText(nItem, 4, szItem.Mid(9, 3));//Mnc3

			// ucs2 HexUcs2DecodeAsc
			memset(szUcs2, '\0', sizeof(szUcs2));
			HexUcs2DecodeAsc(szItem.Mid(12), szUcs2);			
			m_rcOpnList.SetItemText(nItem, 5, szUcs2);  //NetWork Name
			nItem++;
		}
	}
	return;
}

void CPropPageOpnCfg::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_OPNCFG_LIST)->EnableWindow(bLock);
}

void CPropPageOpnCfg::InitOpnList()
{
	// 加入报头
	m_rcOpnList.InsertColumn(0, _T("No."), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcOpnList.InsertColumn(1, _T(" MCC"),// LVCFMT_LEFT, 234);
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								40);

	m_rcOpnList.InsertColumn(2, _T("MNC1"),// LVCFMT_LEFT, 234);
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								40);

	m_rcOpnList.InsertColumn(3, _T("MNC2"),// LVCFMT_LEFT, 234);
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								40);

	m_rcOpnList.InsertColumn(4, _T("MNC3"),// LVCFMT_LEFT, 234);
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								40);

	m_rcOpnList.InsertColumn(5, _T("NetWork Name (18 chars)"),// LVCFMT_LEFT, 234);
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								150);

	resetOpnList(OPN_TOTAL);
}

void CPropPageOpnCfg::resetOpnList(int nItemCount)
{
	// 加入列表项
	char pszTemp[3];
	int i = 0;
	RVITEM rvi;

	if (nItemCount < 0)
		nItemCount = 0;

	m_rcOpnList.DeleteAllItems();

	for(i=1; i<=nItemCount/*OPN_TOTAL*/; i++)
	{
		sprintf(pszTemp, "%02d", i);
		m_rcOpnList.InsertItem(i-1, pszTemp);
	}

	// 更改列表项属性
	for(i=1; i<=nItemCount/*OPN_TOTAL*/; i++)
	{
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcOpnList.SetItem(&rvi);

		rvi.iSubItem = 2;
		rvi.lpszText = _T("");
		m_rcOpnList.SetItem(&rvi);

		rvi.iSubItem = 3;
		rvi.lpszText = _T("");
		m_rcOpnList.SetItem(&rvi);

		rvi.iSubItem = 4;
		rvi.lpszText = _T("");
		m_rcOpnList.SetItem(&rvi);

		rvi.iSubItem = 5;
		rvi.lpszText = _T("");
		m_rcOpnList.SetItem(&rvi);
	}

	if (0 == nItemCount)
		m_rcOpnList.EnableWindow(FALSE);
	else
		m_rcOpnList.EnableWindow(TRUE);
}

void CPropPageOpnCfg::getOpnCfgList(void)
{
	int nItem = 0;
	while (nItem < OPN_TOTAL)
	{
		m_OpCfg[nItem].szMcc.Empty();
		m_OpCfg[nItem].szMnc1.Empty();
		m_OpCfg[nItem].szMnc2.Empty();
		m_OpCfg[nItem].szMnc3.Empty();
		m_OpCfg[nItem].szOpName.Empty();
		nItem++;
	}
	nItem = 0;
	while (nItem<m_rcOpnList.GetItemCount())
	{
		m_OpCfg[nItem].szMcc    = m_rcOpnList.GetItemText(nItem, 1);
		
		m_OpCfg[nItem].szMnc1   = m_rcOpnList.GetItemText(nItem, 2);
		
		m_OpCfg[nItem].szMnc2   = m_rcOpnList.GetItemText(nItem, 3);
		
		m_OpCfg[nItem].szMnc3   = m_rcOpnList.GetItemText(nItem, 4);
		
		m_OpCfg[nItem].szOpName = m_rcOpnList.GetItemText(nItem, 5);

		nItem++;
	}
	return;
}
#if defined(PAGE_CFG)
void CPropPageOpnCfg::OnBnClickedCheckOpncfgEnable()
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_ckEnable.GetCheck();
	setPageEnableStatus(IDD_PROPPAGE_OPN_CFG, bCheck);

	if (bCheck)
		resetOpnList(OPN_TOTAL);
	else
		resetOpnList(0);
}
#endif
