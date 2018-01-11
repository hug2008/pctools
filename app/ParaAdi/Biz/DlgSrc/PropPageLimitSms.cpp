// E:\CvsRoot\bizphonemanage\SRC\PAGE\PropPageLimitSms.cpp : implementation file
//
#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageLimitSms.h"
#include "StringCheckEncode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPropPageLimitSms dialog
CPropPageLimitSms::CPropPageLimitSms(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLimitSms::IDD, pParent)
{
}


void CPropPageLimitSms::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PAGE_LM_LISTNUM, m_listlimitnum);	

}


BEGIN_MESSAGE_MAP(CPropPageLimitSms, CStackPage)

	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_LM_LISTNUM, OnRvnEndItemEdit)

	ON_BN_CLICKED(IDC_LM_LIMITYES, OnPageLimitRadioYes)

	ON_BN_CLICKED(IDC_LM_LIMITNO, OnPageLimitRadioNo)

END_MESSAGE_MAP()


// CPropPageLimitSms message handlers

BOOL CPropPageLimitSms::OnInitDialog()
{
	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();	

	m_listlimitnum.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
		RVS_SINGLESELECT|
		RVS_NOSORT|
		RVS_FOCUSSUBITEMS);

	InitLimitNumTable();


	return TRUE;
}

BOOL CPropPageLimitSms::OnCloseTest(void)
{
	// 检查列表的数据是否都是合法的电话号码
	// 长度是否在 16 字符以内

	BOOL  bLimit = FALSE;

	CString  szItemText  = _T("");

	CString  szErrmsg    = _T("");

	bLimit   = ((CButton*)GetDlgItem(IDC_LM_LIMITYES))->GetCheck();

	if (bLimit){// 选择了是才检测

		for (int i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){

			szItemText  =  m_listlimitnum.GetItemText(i, 1);

			if (!IsEmptyOrPhoneString(szItemText)){

				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_L_LIMIT);
				
				m_listlimitnum.SetFocus();
                
				m_listlimitnum.SetSelection(i);                

				szErrmsg.Format("No.%d: header[%s] must be phone number!", i+1, szItemText);

				MessageBox(szErrmsg, SYS_NAME,MB_ICONSTOP);
				
				return FALSE;

			}else{//if (!IsEmptyOrPhoneString(szItemText)){

				if (szItemText.GetLength() > MAX_LIMITNUM_LEN){

					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_L_LIMIT);
                    
					m_listlimitnum.SetFocus();
                    
					m_listlimitnum.SetSelection(i);

					szErrmsg.Format("No.%d: header[%s] is longer than %d!", i+1, szItemText, MAX_LIMITNUM_LEN);
                    
					MessageBox(szErrmsg, SYS_NAME,MB_ICONSTOP);
                    
					return FALSE;

				}//if (szItemText.GetLength() > MAX_LIMITNUM_LEN){

			}//}else{

		}// for (int i=0; i<40; i++){

	}// if (bLimit){// 选择了是才检测

	return TRUE;

}

void CPropPageLimitSms::OnSaveData(void)
{
	// 直接保存，不再判断
#if !defined(NO_NXT)
	int i = 0;

	CString  szItemText  = _T("");

	for (i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){

		szItemText  =  m_listlimitnum.GetItemText(i, 1);

		m_pParentWnd->m_strLimit_1020_MsgAd[i] = szItemText;

	}

#endif
	return;
}

void CPropPageLimitSms::OnRefreshPage(void)
{
#if !defined(NO_NXT)
	int i = 0;

	for (i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){

		m_listlimitnum.SetItemText(i, 1, _T(""));
	
	}

	for (i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){
        
		m_listlimitnum.SetItemText(i, 1, m_pParentWnd->m_strLimit_1020_MsgAd[i]);
        
	}
	

#endif
	return;
}

void CPropPageLimitSms::InitLimitNumTable(void)
{
	char pszTemp[10];

	int  address = 1020;

	RVITEM rvi;

	m_listlimitnum.InsertColumn(0, _T("Address"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_SUBITEM_NOFOCUS,
								70);

	m_listlimitnum.InsertColumn(1, _T("Limit header(max 16 chars)"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								130);

	m_listlimitnum.DeleteAllItems();
	

	while(address <= 1059){

		_itoa(address - 1020 + 1, pszTemp, 10);

		m_listlimitnum.InsertItem(address-1020, pszTemp);

		rvi.nMask = RVIM_TEXT;
		rvi.iItem = address - 1020;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_listlimitnum.SetItem(&rvi);

		address++;

	}

	return;
}

void CPropPageLimitSms::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
#if !defined(NO_NXT)
	int i = 0;
	for (i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){

		m_pParentWnd->m_strLimit_1020_MsgAd[i] = m_listlimitnum.GetItemText(i, 1);
	}
	m_pParentWnd->m_bDataChanged = TRUE;
#endif
}
void CPropPageLimitSms::OnPageLimitRadioYes()
{
#if !defined(NO_NXT)
	for (int i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){

		m_pParentWnd->m_strLimit_1020_MsgAd[i] = m_listlimitnum.GetItemText(i, 1);
	}

	m_pParentWnd->m_bDataChanged = TRUE;
#endif
}

void CPropPageLimitSms::OnPageLimitRadioNo()
{
#if !defined(NO_NXT)
	for (int i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){

		m_pParentWnd->m_strLimit_1020_MsgAd[i] = _T("");
	}
	m_pParentWnd->m_bDataChanged = TRUE;
#endif
}

void CPropPageLimitSms:: OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_LM_LISTNUM)->EnableWindow(bLock);
	//GetDlgItem(IDC_LM_LIMITYES)->EnableWindow(bLock);
	//GetDlgItem(IDC_LM_LIMITNO)->EnableWindow(bLock);
}