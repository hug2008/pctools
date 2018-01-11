// PropPageLocalPhone.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageLocalPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STR_FRAM_FLAG _T("00000000000000000000000")
const char FramFlagSet[] = {'0', '5', 'a', 'A', 0x00};
BOOL GetFramFlag(char *dst_p, const char *FramFlag_p);
/////////////////////////////////////////////////////////////////////////////

CPropPageLocalPhone::CPropPageLocalPhone(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLocalPhone::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageLocalPhone)
//	m_strAdValue = _T("");
	m_strID = _T("0");
	m_strTime = _T("0");
//	m_strVerifyCode = _T("");
	//}}AFX_DATA_INIT
	m_bAIDial		= FALSE;
	m_bAutoLock		= FALSE;	
	m_bVoiceTip		= FALSE;	
	m_bSpecialApp	= FALSE;
}


void CPropPageLocalPhone::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocalPhone)
	//	DDX_Text(pDX, IDC_EDIT_ADSCRIPT, m_strAdValue);
	//	DDV_MaxChars(pDX, m_strAdValue, 6);
	DDX_Text(pDX, IDC_PAGE_G_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_PAGE_G_EDIT_TIME, m_strTime);
	//	DDX_Text(pDX, IDC_PAGE_G_EDIT_VERIFY, m_strVerifyCode);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ST_RATE_VERIFY_CODE, m_stRateVirityCode);
	DDX_Control(pDX, IDC_ST_RATE_FRAM_FLAG, m_stFramFlag);
}


BEGIN_MESSAGE_MAP(CPropPageLocalPhone, CStackPage)
	//{{AFX_MSG_MAP(CPropPageLocalPhone)
	ON_EN_CHANGE(IDC_PAGE_G_EDIT_ID, OnChangePageId)
	ON_EN_CHANGE(IDC_PAGE_G_EDIT_TIME, OnChangePageTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalPhone message handlers
void CPropPageLocalPhone::OnChangePageId() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLocalPhone::OnChangePageTime() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}



BOOL CPropPageLocalPhone::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	((CButton*)GetDlgItem(IDC_RADIO_AUTOLOCK_YES))->	SetCheck(m_bAutoLock);
	((CButton*)GetDlgItem(IDC_RADIO_AUTOLOCK_NO))->		SetCheck(!m_bAutoLock);

	((CButton*)GetDlgItem(IDC_RADIO_VOICETIP_YES))->	SetCheck(m_bVoiceTip);
	((CButton*)GetDlgItem(IDC_RADIO_VOICETIP_NO))->		SetCheck(!m_bVoiceTip);

	((CButton*)GetDlgItem(IDC_RADIO_AIDIAL_YES))->   	SetCheck(m_bAIDial);
	((CButton*)GetDlgItem(IDC_RADIO_AIDIAL_NO))->		SetCheck(!m_bAIDial);

	((CButton*)GetDlgItem(IDC_RADIO_SPECIALAPP_YES))->  SetCheck(m_bSpecialApp);
	((CButton*)GetDlgItem(IDC_RADIO_SPECIALAPP_NO))->	SetCheck(!m_bSpecialApp);


	m_stRateVirityCode.textColor(RGB(255, 0, 0));

	m_stRateVirityCode.setFont(-18);

	m_stRateVirityCode.SetWindowText(_T("00000000"));

	m_stFramFlag.textColor(RGB(0, 0, 255));

	m_stFramFlag.setFont(-14);

	m_stFramFlag.SetWindowText(STR_FRAM_FLAG);
	
	return TRUE; 
}

BOOL CPropPageLocalPhone::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the radio button
	if  ( ((CButton*)GetDlgItem(IDC_RADIO_AIDIAL_YES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_RADIO_AIDIAL_NO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		MessageBox(_T("请选择正确的智能拨号状态！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}

	// 2 -- the radio button
	if  ( ((CButton*)GetDlgItem(IDC_RADIO_AUTOLOCK_YES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_RADIO_AUTOLOCK_NO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		MessageBox(_T("请选择正确的自动锁定状态！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}

	// 3 -- the radio button
	if  ( ((CButton*)GetDlgItem(IDC_RADIO_VOICETIP_YES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_RADIO_VOICETIP_NO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		MessageBox(_T("请选择正确的收费提示音状态！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}

	// 3 -- the radio button
	if  ( ((CButton*)GetDlgItem(IDC_RADIO_SPECIALAPP_YES))->GetCheck()
		==
		((CButton*)GetDlgItem(IDC_RADIO_SPECIALAPP_NO))->GetCheck()
		)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		MessageBox(_T("请选择正确的特殊应用状态！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}


	// 1 -- the edit
	if (!m_strTime.IsHexNumber())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		((CEdit*)GetDlgItem(IDC_PAGE_G_EDIT_TIME))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_G_EDIT_TIME)->SetFocus();

		MessageBox(_T("请输入正确的起跳时间！"), SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}

	if (atoi(m_strTime) >9 )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		((CEdit*)GetDlgItem(IDC_PAGE_G_EDIT_TIME))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_G_EDIT_TIME)->SetFocus();

		MessageBox(_T("输入的起跳时间不能大于9！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}

	// 2 -- the edit
	if (!m_strID.IsHexNumber())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		((CEdit*)GetDlgItem(IDC_PAGE_G_EDIT_ID))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_G_EDIT_ID)->SetFocus();

		MessageBox(_T("请输入正确的费率表ID！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}

	if (atoi(m_strID) >255 )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		((CEdit*)GetDlgItem(IDC_PAGE_G_EDIT_ID))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_G_EDIT_ID)->SetFocus();

		MessageBox(_T("输入的费率表ID不能大于255！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}
	/*
	if (1 > atoi(m_strID))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_G_LOCAL_PHONE));
		((CEdit*)GetDlgItem(IDC_PAGE_G_EDIT_ID))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_G_EDIT_ID)->SetFocus();

		MessageBox(_T("输入的费率表ID不能小于1！"),SYS_NAME, MB_ICONSTOP);
		return FALSE;
	}
	*/
	
	return TRUE;
}

void CPropPageLocalPhone::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);
	
	// 1 -- the check button
	m_pParentWnd->szIntelligentDial26 = 
		((CButton*)GetDlgItem(IDC_RADIO_AIDIAL_YES))->GetCheck() ? "11" : "00";

	// 2 -- the check button
 	m_pParentWnd->szAutoLock27 = 
		((CButton*)GetDlgItem(IDC_RADIO_AUTOLOCK_YES))->GetCheck() ? "11" : "00";

	// 3 -- the check button
 	m_pParentWnd->szTollIndiction28 = 
		((CButton*)GetDlgItem(IDC_RADIO_VOICETIP_YES))->GetCheck() ? "11" : "00";

	// 4 -- the check button
	m_pParentWnd->szSpecialApplication29 = 
		((CButton*)GetDlgItem(IDC_RADIO_SPECIALAPP_YES))->GetCheck() ? "11" : "00";

//	// 5 -- the edit
// 	m_pParentWnd->m_strLOCALPHONE_0030_strAdscript = m_strAdValue;

//	// 1 -- the edit
	m_pParentWnd->szMajorRateFramStartTime12026 = m_strTime;

	// 2 -- the edit
 	m_pParentWnd->szMajorRateFramID12027 = m_strID;

	// 3 -- the edit
// 	m_pParentWnd->m_AryFeeDown_12800_strVerifyCode = m_strVerifyCode;
}

void CPropPageLocalPhone::OnRefreshPage(void)
{
	char FramFlag[NUM_RATE_FRAM_HEADER_TOTAL+1] = "";
	// 1 -- the check button
	m_bAIDial = atoi(m_pParentWnd->szIntelligentDial26);
	((CButton*)GetDlgItem(IDC_RADIO_AIDIAL_YES))->		SetCheck(m_bAIDial);
	((CButton*)GetDlgItem(IDC_RADIO_AIDIAL_NO))->		SetCheck(!m_bAIDial);

	// 2 -- the check button
	m_bAutoLock = (atoi(m_pParentWnd->szAutoLock27) != 0);
	((CButton*)GetDlgItem(IDC_RADIO_AUTOLOCK_YES))->	SetCheck(m_bAutoLock);
	((CButton*)GetDlgItem(IDC_RADIO_AUTOLOCK_NO))->		SetCheck(!m_bAutoLock);

	// 3 -- the check button
	m_bVoiceTip = atoi(m_pParentWnd->szTollIndiction28);
	((CButton*)GetDlgItem(IDC_RADIO_VOICETIP_YES))->	SetCheck(m_bVoiceTip);
	((CButton*)GetDlgItem(IDC_RADIO_VOICETIP_NO))->		SetCheck(!m_bVoiceTip);

	// 4 -- the check button
	m_bSpecialApp = atoi(m_pParentWnd->szSpecialApplication29);
	((CButton*)GetDlgItem(IDC_RADIO_SPECIALAPP_YES))->	SetCheck(m_bSpecialApp);
	((CButton*)GetDlgItem(IDC_RADIO_SPECIALAPP_NO))->	SetCheck(!m_bSpecialApp);



//	// 1 -- the edit
	m_strTime = m_pParentWnd->szMajorRateFramStartTime12026;

//	// 1 -- the edit
	m_strID = m_pParentWnd->szMajorRateFramID12027;

	if (m_pParentWnd->szMajorRateFramRateVerify12800.IsEmpty())
	{
		m_pParentWnd->szMajorRateFramRateVerify12800 = _T("00000000");
	}
	m_stRateVirityCode.SetWindowText(m_pParentWnd->szMajorRateFramRateVerify12800);

	/* 46位两两重复的数据,没两位取一个字符即可 */
	if (!IsValidFramFlag(m_pParentWnd->szMajorRateFramFramFlag12030))
	{
		m_stFramFlag.SetWindowText(STR_FRAM_FLAG);
	}
	else
	{
		GetFramFlag(FramFlag, m_pParentWnd->szMajorRateFramFramFlag12030);
		m_stFramFlag.SetWindowText(FramFlag);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}

BOOL IsCharInSet(const char cChar, const char *Set_p)
{
	while (*Set_p)
	{
		if (cChar == *Set_p++)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsValidFramFlag(const char *FramFlag_p)
{
	ASSERT(FramFlag_p);
	int Index = 0;
	if (NULL == FramFlag_p)
	{
		return FALSE;
	}
	if (NUM_RATE_FRAM_HEADER_TOTAL*2 != strlen(FramFlag_p))
	{
		return FALSE;
	}

	while(*FramFlag_p)
	{
		if (!IsCharInSet(*FramFlag_p, FramFlagSet))
		{
			return FALSE;
		}
		FramFlag_p++;
	}

	return TRUE;
}
BOOL GetFramFlag(char *dst_p, const char *FramFlag_p)
{
	ASSERT(dst_p && FramFlag_p);
	if (!dst_p || !FramFlag_p)
	{
		return FALSE;
	}
	while (*dst_p++ = *FramFlag_p++)
	{
		FramFlag_p++;
	}
	return TRUE;
}