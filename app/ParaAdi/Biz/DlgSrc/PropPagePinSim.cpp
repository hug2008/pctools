// PropPagePinSim.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPagePinSim.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPagePinSim dialog


CPropPagePinSim::CPropPagePinSim(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPagePinSim::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPagePinSim)
	m_strInitPinCode = _T("1234");
	m_strSimCode = _T("");
	m_nPinLength = 0;
	m_strFixedPinCode = _T("1234");
	//}}AFX_DATA_INIT
	m_bAutoLockPin  = FALSE;
	m_bUseRandomPin = FALSE;
	m_bLockSim      = FALSE;
}


void CPropPagePinSim::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePinSim)
	DDX_Control(pDX, IDC_PAGE_8_COMBO_PINLENGTH, m_cbPinLength);
	DDX_Text(pDX, IDC_PAGE_8_EDIT_INITPIN, m_strInitPinCode);
	DDV_MaxChars(pDX, m_strInitPinCode, 8);
	DDX_Text(pDX, IDC_PAGE_8_EDIT_SIM, m_strSimCode);
	DDV_MaxChars(pDX, m_strSimCode, 19);
	DDX_CBIndex(pDX, IDC_PAGE_8_COMBO_PINLENGTH, m_nPinLength);
	DDX_Text(pDX, IDC_PAGE_8_EDIT_FIXEDPIN, m_strFixedPinCode);
	DDV_MaxChars(pDX, m_strFixedPinCode, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPagePinSim, CStackPage)
	//{{AFX_MSG_MAP(CPropPagePinSim)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_SIMNO, OnPage8RadioSimNo)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_SIMYES, OnPage8RadioSimYes)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_PINNO, OnPage8RadioPinno)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_PINYES, OnPage8RadioPinyes)
	ON_CBN_SELCHANGE(IDC_PAGE_8_COMBO_PINLENGTH, OnSelchangePinlength)
	ON_EN_CHANGE(IDC_PAGE_8_EDIT_INITPIN, OnChangePin)
	ON_EN_CHANGE(IDC_PAGE_8_EDIT_SIM, OnChangeSim)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_USEPINYES, OnPage8RadioUsepinyes)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_USEPINNO, OnPage8RadioUsepinno)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPagePinSim message handlers

void CPropPagePinSim::OnPage8RadioPinno() 
{
	// 防止两个同时被选中
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->SetCheck(0);

	// 初始PIN码
	(GetDlgItem(IDC_PAGE_8_EDIT_INITPIN ))->EnableWindow(FALSE);

	// 是否使用随机PIN码
	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->EnableWindow(FALSE);
	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO ))->EnableWindow(FALSE);

	// 随机PIN码长度
	m_cbPinLength.EnableWindow(FALSE);

	// 固定PIN码的值
	(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN ))->EnableWindow(FALSE);

	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPagePinSim::OnPage8RadioPinyes() 
{
	// 防止两个同时被选中
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINNO))->SetCheck(0);

	// 初始PIN码
	(GetDlgItem(IDC_PAGE_8_EDIT_INITPIN ))->EnableWindow(TRUE);

	// 是否使用随机PIN码
	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->EnableWindow(TRUE);
	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO ))->EnableWindow(TRUE);

	// 随机PIN码长度
	m_cbPinLength.EnableWindow(m_bUseRandomPin);

	// 固定PIN码的值
	(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN))->EnableWindow(!m_bUseRandomPin);

	m_pParentWnd->m_bDataChanged = TRUE;
}


void CPropPagePinSim::OnPage8RadioUsepinyes() 
{
	// 防止两个同时被选中
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO))->SetCheck(0);

	// 控制变量赋值
	m_bUseRandomPin = TRUE;

	// 随机PIN码长度
	m_cbPinLength.EnableWindow(m_bUseRandomPin);

	// 固定PIN码的值
	(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN))->EnableWindow(!m_bUseRandomPin);

	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPagePinSim::OnPage8RadioUsepinno() 
{
	// 防止两个同时被选中
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->SetCheck(0);

	// 控制变量赋值
	m_bUseRandomPin = FALSE;

	// 随机PIN码长度
	m_cbPinLength.EnableWindow(m_bUseRandomPin);

	// 固定PIN码的值
	(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN))->EnableWindow(!m_bUseRandomPin);

	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPagePinSim::OnSelchangePinlength() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPagePinSim::OnChangePin() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPagePinSim::OnChangeSim() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPagePinSim::OnPage8RadioSimNo() 
{
	// 当选择了NO SIM LOCK
	GetDlgItem(IDC_PAGE_8_EDIT_SIM)->EnableWindow(FALSE);
	//m_strSimCode = _T("");
	//GetDlgItem(IDC_PAGE_8_EDIT_SIM)->SetWindowText(m_strSimCode);

	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPagePinSim::OnPage8RadioSimYes() 
{
	// 当选择了SIM LOCK
	GetDlgItem(IDC_PAGE_8_EDIT_SIM)->EnableWindow(TRUE);

	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPagePinSim::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	// 设置锁PIN Radio
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->SetCheck(!m_bAutoLockPin);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINNO))->SetCheck(m_bAutoLockPin);

	// 根据是否自动锁PIN码，来设定后来的控件的状态
	if (m_bAutoLockPin)
	{
		// 防止两个同时被选中
		((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINNO))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->SetCheck(1);

		// 初始PIN码
		(GetDlgItem(IDC_PAGE_8_EDIT_INITPIN ))->EnableWindow(TRUE);

		// 是否使用随机PIN码
		(GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->EnableWindow(TRUE);
		(GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO ))->EnableWindow(TRUE);

		// 随机PIN码长度
		m_cbPinLength.EnableWindow(m_bUseRandomPin);

		// 固定PIN码的值
		(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN))->EnableWindow(!m_bUseRandomPin);
	}
	else
	{
		// 防止两个同时被选中
		((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINNO))->SetCheck(1);

		// 初始PIN码
		(GetDlgItem(IDC_PAGE_8_EDIT_INITPIN ))->EnableWindow(FALSE);

		// 是否使用随机PIN码
		(GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->EnableWindow(FALSE);
		(GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO ))->EnableWindow(FALSE);

		// 随机PIN码长度
		m_cbPinLength.EnableWindow(FALSE);

		// 固定PIN码的值
		(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN ))->EnableWindow(FALSE);
	}
	// 设置使用随机PIN码
	// if (m_bUseRandomPin)
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->SetCheck(m_bUseRandomPin);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO))->SetCheck(!m_bUseRandomPin);
	
	// 设置锁SIM Radio
	// if (m_bLockSim)
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMYES))->SetCheck(m_bLockSim);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMNO))->SetCheck(!m_bLockSim);

	((CEdit*)GetDlgItem(IDC_PAGE_8_EDIT_SIM))->EnableWindow(m_bLockSim);
/*
#ifdef DISABLE_INPUT
	(GetDlgItem(IDC_PAGE_8_EDIT_INITPIN ))->EnableWindow(FALSE);
	(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN ))->EnableWindow(FALSE);

	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->EnableWindow(FALSE);
	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO ))->EnableWindow(FALSE);
#endif
*/	
	return TRUE;
}

BOOL CPropPagePinSim::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the check button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINNO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
		MessageBox(_T("Please select if auto-lock PIN!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 1' -- the check button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
		MessageBox(_T("Please select if use random PIN code!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 2 -- the combo box
	if (m_nPinLength == -1)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
		m_cbPinLength.SetFocus();
		// 没有做选择
		MessageBox(_T("Please select the random PIN'length!"),SYS_NAME,MB_ICONSTOP);

		return FALSE;
	}

	// 3 -- the edit
	if (m_strInitPinCode.IsEmpty()||(m_strInitPinCode.GetLength()<4))// || (m_strInitPinCode.GetLength() != m_nPinLength+4))
	//if ((m_strInitPinCode.GetLength()<4)) || (m_strInitPinCode.GetLength() != m_nPinLength+4))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
		((CEdit*)GetDlgItem(IDC_PAGE_8_EDIT_INITPIN))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_8_EDIT_INITPIN)->SetFocus();

		MessageBox(_T("initial PIN code 4 chars at least!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if (m_strInitPinCode.GetLength() > 8)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
		((CEdit*)GetDlgItem(IDC_PAGE_8_EDIT_INITPIN))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_8_EDIT_INITPIN)->SetFocus();

		MessageBox(_T("initial PIN code no more than 8 chars!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 3' -- the edit
	if (((m_strFixedPinCode.GetLength()<4) || (m_strFixedPinCode.GetLength()>8))
		&&
		GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN)->IsWindowEnabled()
//		(((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->GetCheck())
//		&&
//		(((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO))->GetCheck())
		)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
		((CEdit*)GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN)->SetFocus();

		MessageBox(_T("fixed PIN length between 4 ~ 8 chars!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 4 -- the check button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMNO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
		MessageBox(_T("Please select if bind SIM card with phone!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 5 -- the edit
	if (((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMYES))->GetCheck() == TRUE)
	{
		// 此处如果，radio没有选中，就不用检查edit的数据了
		if (m_strSimCode.GetLength() > 19)
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
			((CEdit*)GetDlgItem(IDC_PAGE_8_EDIT_SIM))->SetSel(0,-1);
			GetDlgItem(IDC_PAGE_8_EDIT_SIM)->SetFocus();
			MessageBox(_T("Serial number no more than 19 chars!"),SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
#if defined(FORBID_IMSI_EMPTY)
		if (m_strSimCode.GetLength() == 0)
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_8_PIN_SIM);
			((CEdit*)GetDlgItem(IDC_PAGE_8_EDIT_SIM))->SetSel(0,-1);
			GetDlgItem(IDC_PAGE_8_EDIT_SIM)->SetFocus();
			MessageBox(_T("Serial number must bu setted!"),SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
#endif
	}
	return TRUE;
}

void CPropPagePinSim::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	m_pParentWnd->m_strPINSIM_6000_bAutoLockPin = 
		((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->GetCheck() ? "01" : "00";

	m_pParentWnd->m_strPINSIM_6005_bUseRandomPin = 
		((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->GetCheck() ? "01" : "00";

	m_pParentWnd->m_strPINSIM_6001_nPinLength.Format("0%d", m_nPinLength+4);
	m_pParentWnd->m_strPINSIM_6002_strOriginalPin = m_strInitPinCode;
	m_pParentWnd->m_strPINSIM_6004_strFixedPin    = m_strFixedPinCode;

	if (((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMYES))->GetCheck())
	{
		m_pParentWnd->m_strPINSIM_7000_bLockSim = "01";
		m_pParentWnd->m_strPINSIM_7001_strLockSimNum = m_strSimCode;
	}
	else
	{
		m_pParentWnd->m_strPINSIM_7000_bLockSim = "00";
		m_pParentWnd->m_strPINSIM_7001_strLockSimNum = "";
	}
}

void CPropPagePinSim::OnRefreshPage(void)
{
	CString strTemp;
	int bFlag;

	// 1 -- the check button
	m_bAutoLockPin = atoi(m_pParentWnd->m_strPINSIM_6000_bAutoLockPin);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINYES))->SetCheck(m_bAutoLockPin);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_PINNO ))->SetCheck(!m_bAutoLockPin);


	// 1' -- the check button
	m_bUseRandomPin = atoi(m_pParentWnd->m_strPINSIM_6005_bUseRandomPin);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->SetCheck(m_bUseRandomPin);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO ))->SetCheck(!m_bUseRandomPin);


	// 2 -- the combo box
	m_nPinLength = atoi(m_pParentWnd->m_strPINSIM_6001_nPinLength)-4;
	m_cbPinLength.EnableWindow(m_bUseRandomPin);
	//ASSERT((m_nPinLength>=0)&&(m_nPinLength<=4));

	// 3 -- the edit
	m_strInitPinCode  = m_pParentWnd->m_strPINSIM_6002_strOriginalPin;

	// 3' -- the edit
	m_strFixedPinCode = m_pParentWnd->m_strPINSIM_6004_strFixedPin;



	// 4 -- the check button
	bFlag = atoi(m_pParentWnd->m_strPINSIM_7000_bLockSim);
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMYES))->SetCheck(bFlag );
	((CButton*)GetDlgItem(IDC_PAGE_8_RADIO_SIMNO ))->SetCheck(!bFlag);
	GetDlgItem(IDC_PAGE_8_EDIT_SIM)->EnableWindow(bFlag);

	// 5 -- the edit
	m_strSimCode = bFlag ? m_pParentWnd->m_strPINSIM_7001_strLockSimNum: "" ;


	//==================================================================
	// 由于m_bAutoLockPin的值将在某种情况下决定Usepinyes/Usepinno的状态
	// 故把m_bUseRandomPin的处理放在m_bAutoLockPin之前
	//==================================================================
	
	// 根据是否使用随机PIN码，来设定后来的控件的状态
	if (m_bUseRandomPin)
		OnPage8RadioUsepinyes();
	else
		OnPage8RadioUsepinno();

	// 根据是否自动锁PIN码，来设定后来的控件的状态
	if (m_bAutoLockPin)
		OnPage8RadioPinyes();
	else
		OnPage8RadioPinno();
/*
#ifdef DISABLE_INPUT
	(GetDlgItem(IDC_PAGE_8_EDIT_INITPIN ))->EnableWindow(FALSE);
	(GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN ))->EnableWindow(FALSE);

	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES))->EnableWindow(FALSE);
	(GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO ))->EnableWindow(FALSE);
	
	m_cbPinLength.EnableWindow(FALSE);
	GetDlgItem(IDC_PAGE_8_EDIT_SIM)->EnableWindow(FALSE);
#endif
*/
	// Set the data to screen show
	UpdateData(FALSE);
}

/*
    ON_BN_CLICKED(IDC_PAGE_8_RADIO_SIMNO, OnPage8RadioSimNo)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_SIMYES, OnPage8RadioSimYes)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_PINNO, OnPage8RadioPinno)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_PINYES, OnPage8RadioPinyes)
	ON_CBN_SELCHANGE(IDC_PAGE_8_COMBO_PINLENGTH, OnSelchangePinlength)
	ON_EN_CHANGE(IDC_PAGE_8_EDIT_INITPIN, OnChangePin)
	ON_EN_CHANGE(IDC_PAGE_8_EDIT_SIM, OnChangeSim)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_USEPINYES, OnPage8RadioUsepinyes)
	ON_BN_CLICKED(IDC_PAGE_8_RADIO_USEPINNO, OnPage8RadioUsepinno)
*/
void CPropPagePinSim::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_8_RADIO_SIMNO)->EnableWindow(bLock);
    GetDlgItem(IDC_PAGE_8_RADIO_SIMYES)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_8_RADIO_PINNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_8_RADIO_PINYES)->EnableWindow(bLock);

	GetDlgItem(IDC_PAGE_8_COMBO_PINLENGTH)->EnableWindow(bLock);
    GetDlgItem(IDC_PAGE_8_EDIT_INITPIN)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_8_RADIO_USEPINYES)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_8_RADIO_USEPINNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_8_EDIT_FIXEDPIN)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_8_EDIT_SIM)->EnableWindow(bLock);	
}

