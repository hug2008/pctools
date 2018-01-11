// D:\Para\SRC\PAGE\PropPageEmCallCfg.cpp : 实现文件
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageEmCallCfg.h"
#include "PhoneNumber.h"

// CPropPageEmCallCfg 对话框
CPropPageEmCallCfg::CPropPageEmCallCfg(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageEmCallCfg::IDD, pParent)
{

}

CPropPageEmCallCfg::~CPropPageEmCallCfg()
{
}

void CPropPageEmCallCfg::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_EMERGENCY_CALL_LIST, m_rcEmCallList);
	DDX_Control(pDX, IDC_CHECK_EMCALL_ENABLE, m_ckEnable);
}


BEGIN_MESSAGE_MAP(CPropPageEmCallCfg, CStackPage)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_EMERGENCY_CALL_LIST, OnRvnEndItemEdit)
#if defined(PAGE_CFG)
	ON_BN_CLICKED(IDC_CHECK_EMCALL_ENABLE, OnBnClickedCheckEmcallEnable)
#endif
END_MESSAGE_MAP()


// CPropPageEmCallCfg 消息处理程序
BOOL CPropPageEmCallCfg::OnInitDialog(void)
{
	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	ASSERT(m_pParentWnd);
	m_rcEmCallList.ModifyStyle(0, 
		                        RVS_SHOWHGRID|
								RVS_SHOWVGRID|
								//		RVS_SHOWCOLORALTERNATE|
								RVS_SINGLESELECT|
								RVS_NOSORT
								//#ifndef DISABLE_INPUT
								| RVS_FOCUSSUBITEMS
								//#endif
								);

	InitEmCallList();

#if defined(PAGE_CFG)
	m_ckEnable.SetCheck(ENABLE_EMERGENCY_CALL);
	m_rcEmCallList.EnableWindow(ENABLE_EMERGENCY_CALL);
	if (!ENABLE_EMERGENCY_CALL)
	{
		resetEmcallList(0);
	}
#else
	m_ckEnable.ShowWindow(SW_HIDE);
#endif

	return TRUE;
}

BOOL CPropPageEmCallCfg::OnCloseTest(void)
{
	int nItem = 0;

	CPhoneNumber szItem = _T("");
	CString sError("");

	for (nItem=0; nItem<m_rcEmCallList.GetItemCount(); nItem++)
	{
		szItem = m_rcEmCallList.GetItemText(nItem, 1);

		if (EMERGENCY_HEAD_LEN*2 < szItem.GetLength())
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_EMERGENCY_CALL);

			m_rcEmCallList.SetFocus();
			m_rcEmCallList.SetSelection(nItem);

			sError.Format("Call Number is not more than 6 chars,(%dth)!", nItem+1);
			MessageBox(sError, SYS_NAME,MB_ICONSTOP);

			return FALSE;
		}

		if (!szItem.IsEmptyOrPhoneString())
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_EMERGENCY_CALL);

			m_rcEmCallList.SetFocus();
			m_rcEmCallList.SetSelection(nItem);

			sError.Format("Number must be in the set {0,1,...,9,*,#},(%dth)!", nItem+1);
			MessageBox(sError, SYS_NAME,MB_ICONSTOP);

			return FALSE;
		}
	}

	return TRUE;
}

void CPropPageEmCallCfg::OnSaveData(void)
{
	int nItem = 0, nItemNoEmpty = 0;

	CString szItemVal = _T("");

	for (nItem=0; nItem<EMERGENCY_NUM_INT; nItem++)
		m_pParentWnd->m_AryEmCall_2610_strNumberList[nItem].Empty();

	for (nItem=0; nItem<m_rcEmCallList.GetItemCount(); nItem++)
	{
		szItemVal = m_rcEmCallList.GetItemText(nItem, 1);;
		if (szItemVal.IsEmpty())
			continue;

		m_pParentWnd->m_AryEmCall_2610_strNumberList[nItemNoEmpty] = szItemVal;
		nItemNoEmpty++;
	}
}

void CPropPageEmCallCfg::OnRefreshPage(void)
{
	int nItem = 0;
#if defined(PAGE_CFG)
	BOOL bCheck = ENABLE_EMERGENCY_CALL;
	m_ckEnable.SetCheck(bCheck);
	if (bCheck)
	{
		resetEmcallList(EMERGENCY_NUM_INT);
	}
	else
	{
		resetEmcallList(0);
	}
#endif
	for (nItem=0; nItem<m_rcEmCallList.GetItemCount(); nItem++)
	{
		m_rcEmCallList.SetItemText(nItem, 1, m_pParentWnd->m_AryEmCall_2610_strNumberList[nItem]);
	}
}

void CPropPageEmCallCfg::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_EMERGENCY_CALL_LIST)->EnableWindow(bLock);
}

void CPropPageEmCallCfg::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageEmCallCfg::InitEmCallList(void)
{
	// 加入报头
	m_rcEmCallList.InsertColumn(0, _T("No."), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS);

	m_rcEmCallList.InsertColumn(1, _T("Call Number (6 chars)"),// LVCFMT_LEFT, 234);
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								150);

	resetEmcallList(EMERGENCY_NUM_INT);
}
void CPropPageEmCallCfg::resetEmcallList(int nItemCount)
{
	// 加入列表项
	char pszTemp[3];
	int i = 0;
	RVITEM rvi;

	if (nItemCount < 0)
		nItemCount = 0;

	m_rcEmCallList.DeleteAllItems();

	for(i=1; i<=nItemCount; i++)
	{
		sprintf(pszTemp, "%02d", i);
		m_rcEmCallList.InsertItem(i-1, pszTemp);
	}

	// 更改列表项属性
	for(i=1; i<=nItemCount; i++)
	{
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcEmCallList.SetItem(&rvi);
	}
	if (0 == nItemCount)
	{
		m_rcEmCallList.EnableWindow(FALSE);
	}
	else
	{
		m_rcEmCallList.EnableWindow(TRUE);
	}
}

#if defined(PAGE_CFG)
void CPropPageEmCallCfg::OnBnClickedCheckEmcallEnable()
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = m_ckEnable.GetCheck();
	setPageEnableStatus(IDD_PROPPAGE_EMERGENCY_CALL, bCheck);

	if (bCheck)
		resetEmcallList(EMERGENCY_NUM_INT);
	else
		resetEmcallList(0);
}
#endif