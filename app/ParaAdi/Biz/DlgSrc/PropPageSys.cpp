// PropPageSys.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageSys.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropPageSys dialog


CPropPageSys::CPropPageSys(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageSys::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageSys)
	m_strSMSCenter = _T("");
	m_nAutoDelay = 4;
	m_nLcdContrast = 32;
	//}}AFX_DATA_INIT
	m_bNotLocalAddZero = FALSE;
	m_bForbidLongDistanceCall = TRUE;
}


void CPropPageSys::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageSys)
	DDX_Control(pDX, IDC_PAGE_1_COMBO_AUTODELAY, m_cbAutoDelay);
	DDX_Text(pDX, IDC_PAGE_1_EDIT_SMSCENTER, m_strSMSCenter);
	DDV_MaxChars(pDX, m_strSMSCenter, 16);
	DDX_CBIndex(pDX, IDC_PAGE_1_COMBO_AUTODELAY, m_nAutoDelay);
	DDX_Text(pDX, IDC_PAGE_1_EDIT_LCD, m_nLcdContrast);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageSys, CStackPage)
	//{{AFX_MSG_MAP(CPropPageSys)
	ON_CBN_SELCHANGE(IDC_PAGE_1_COMBO_AUTODELAY, OnSelchangeAutoDelay)
	ON_EN_CHANGE(IDC_PAGE_1_EDIT_SMSCENTER, OnChangeSmsCenter)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_ADDYES, OnPage1RadioAddyes)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_ADDNO, OnPage1RadioAddno)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_FORBIDYES, OnPage1RadioForbidYes)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_FORBIDNO, OnPage1RadioForbidNo)
	ON_EN_CHANGE(IDC_PAGE_1_EDIT_LCD, OnChangeLcdContrast)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageSys message handlers
void CPropPageSys::OnSelchangeAutoDelay() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageSys::OnChangeSmsCenter() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageSys::OnChangeLcdContrast() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageSys::OnPage1RadioAddyes() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageSys::OnPage1RadioAddno() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageSys::OnPage1RadioForbidYes() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageSys::OnPage1RadioForbidNo() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageSys::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg *)GetParent();

	((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDYES))->SetCheck(m_bNotLocalAddZero);
	((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDNO))->SetCheck(!m_bNotLocalAddZero);
	
	((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDYES))->SetCheck(m_bForbidLongDistanceCall);
	((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDNO))->SetCheck(!m_bForbidLongDistanceCall);

	m_cbAutoDelay.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPropPageSys::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

	/* 1 -- the combo box  */
	if (m_nAutoDelay == -1)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_1_SYS);
		m_cbAutoDelay.SetFocus();
		// 没有做选择
		MessageBox(_T("Please select dialing time!"),SYS_NAME,MB_ICONSTOP);

		return FALSE;
	}

	/* 2 -- the edit  */
	if (!m_strSMSCenter.IsEmptyOrPhoneString())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_1_SYS);
		((CEdit*)GetDlgItem(IDC_PAGE_1_EDIT_SMSCENTER))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_1_EDIT_SMSCENTER)->SetFocus();

		MessageBox(_T("Center number is illeagal, try it again!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	/* 3 -- the check button */
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDNO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_1_SYS);
		MessageBox(_T("Please select wether add '0' at dial other city mobile number!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	/* 4 -- the check button  */
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDNO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_1_SYS);
		MessageBox(_T("Please select wether forbid domestic l.d.!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	/* 5 -- the edit of lcd */
	if ((m_nLcdContrast<10) ||(m_nLcdContrast>40))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_1_SYS);
		((CEdit*)GetDlgItem(IDC_PAGE_1_EDIT_LCD))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_1_EDIT_LCD)->SetFocus();

		MessageBox(_T("LCD contrast between 10 and 40!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

void CPropPageSys::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);
	// 1 -- the combo box
	m_pParentWnd->m_strSYS_0002_nAutoDely.Format("0%d", m_nAutoDelay);

	// 2 -- the edit
	m_pParentWnd->m_strSYS_0004_sSMSCenter = m_strSMSCenter;

	// 3 -- the check button
	m_pParentWnd->m_strSYS_4000_bNotLocalAddZero = 
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDYES))->GetCheck() ? "01" : "00";

	// 4 -- the check button
	m_pParentWnd->m_strSYS_0005_bForbidLongDistanceCall = 
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDYES))->GetCheck() ? "01" : "00";

	// 5 -- the edit of lcd
	m_pParentWnd->m_strSYS_LCD_Contrast.Format("%d", m_nLcdContrast);
}

void CPropPageSys::OnRefreshPage(void)
{

	// 1 -- the combo box
	m_nAutoDelay = atoi(m_pParentWnd->m_strSYS_0002_nAutoDely);

	// 2 -- the edit
	m_strSMSCenter = m_pParentWnd->m_strSYS_0004_sSMSCenter;

	// 3 -- the check button
	if (atoi(m_pParentWnd->m_strSYS_4000_bNotLocalAddZero))
	{
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDYES))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDNO ))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDYES))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_ADDNO ))->SetCheck(1);
	}

	// 4 -- the check button
	if (atoi(m_pParentWnd->m_strSYS_0005_bForbidLongDistanceCall))
	{
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDYES))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDNO ))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDYES))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_PAGE_1_RADIO_FORBIDNO ))->SetCheck(1);
	}

	// 5 -- the edit of lcd
	m_nLcdContrast = atoi(m_pParentWnd->m_strSYS_LCD_Contrast);

	// Set the data to screen show
	UpdateData(FALSE);
}
/*
    ON_CBN_SELCHANGE(IDC_PAGE_1_COMBO_AUTODELAY, OnSelchangeAutoDelay)
	ON_EN_CHANGE(IDC_PAGE_1_EDIT_SMSCENTER, OnChangeSmsCenter)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_ADDYES, OnPage1RadioAddyes)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_ADDNO, OnPage1RadioAddno)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_FORBIDYES, OnPage1RadioForbidYes)
	ON_BN_CLICKED(IDC_PAGE_1_RADIO_FORBIDNO, OnPage1RadioForbidNo)
	ON_EN_CHANGE(IDC_PAGE_1_EDIT_LCD, OnChangeLcdContrast)
*/
void CPropPageSys::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_1_COMBO_AUTODELAY)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_1_EDIT_SMSCENTER)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_1_RADIO_ADDYES)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_1_RADIO_ADDNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_1_RADIO_FORBIDYES)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_1_RADIO_FORBIDNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_1_EDIT_LCD)->EnableWindow(bLock);
}
