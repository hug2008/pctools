// PropPageMic.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageMic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageMic dialog


CPropPageMic::CPropPageMic(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageMic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageMic)
	m_nCMICAux = 2;
	m_nCMICMain = 2;
	m_strSidet0MainGain = _T("4096");
	m_strSidet1AuxGain  = _T("4096");
	m_strECHO0Vox = _T("0");
	m_strECHO0Min = _T("0");
	m_strECHO0Samp = _T("0");
	m_strECHO1Vox = _T("0");
	m_strECHO1Min = _T("0");
	m_strECHO1Samp = _T("0");
	//}}AFX_DATA_INIT
}


void CPropPageMic::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageMic)
	DDX_Control(pDX, IDC_PAGE_C_COMBO_CMIC_MAIN, m_cbCMICMain);
	DDX_Control(pDX, IDC_PAGE_C_COMBO_CMIC_AUX, m_cbCMICAux);
	DDX_CBIndex(pDX, IDC_PAGE_C_COMBO_CMIC_AUX, m_nCMICAux);
	DDX_CBIndex(pDX, IDC_PAGE_C_COMBO_CMIC_MAIN, m_nCMICMain);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_SIDET_GAIN, m_strSidet0MainGain);
	DDV_MaxChars(pDX, m_strSidet0MainGain, 5);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_SIDET_AUXGAIN, m_strSidet1AuxGain);
	DDV_MaxChars(pDX, m_strSidet1AuxGain, 5);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_ECHO0_VOX, m_strECHO0Vox);
	DDV_MaxChars(pDX, m_strECHO0Vox, 5);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_ECHO0_MIN, m_strECHO0Min);
	DDV_MaxChars(pDX, m_strECHO0Min, 5);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_ECHO0_SAMP, m_strECHO0Samp);
	DDV_MaxChars(pDX, m_strECHO0Samp, 5);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_ECHO1_VOX, m_strECHO1Vox);
	DDV_MaxChars(pDX, m_strECHO1Vox, 5);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_ECHO1_MIN, m_strECHO1Min);
	DDV_MaxChars(pDX, m_strECHO1Min, 5);
	DDX_Text(pDX, IDC_PAGE_C_EDIT_ECHO1_SAMP, m_strECHO1Samp);
	DDV_MaxChars(pDX, m_strECHO1Samp, 5);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CPropPageMic, CStackPage)
	//{{AFX_MSG_MAP(CPropPageMic)
	ON_CBN_SELCHANGE(IDC_PAGE_C_COMBO_CMIC_AUX, OnSelchangeCMICAux)
	ON_CBN_SELCHANGE(IDC_PAGE_C_COMBO_CMIC_MAIN, OnSelchangeCMICMain)
	ON_EN_CHANGE(IDC_PAGE_C_EDIT_ECHO0_MIN, OnChangeEditEcho0Min)
	ON_EN_CHANGE(IDC_PAGE_C_EDIT_ECHO0_SAMP, OnChangeEditEcho0Samp)
	ON_EN_CHANGE(IDC_PAGE_C_EDIT_ECHO0_VOX, OnChangeEditEcho0Vox)
	ON_EN_CHANGE(IDC_PAGE_C_EDIT_ECHO1_MIN, OnChangeEditEcho1Min)
	ON_EN_CHANGE(IDC_PAGE_C_EDIT_ECHO1_SAMP, OnChangeEditEcho1Samp)
	ON_EN_CHANGE(IDC_PAGE_C_EDIT_ECHO1_VOX, OnChangeEditEcho1Vox)
	ON_EN_CHANGE(IDC_PAGE_C_EDIT_SIDET_GAIN, OnChangeEditSidetGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageMic message handlers

void CPropPageMic::OnSelchangeCMICAux() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnSelchangeCMICMain() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnChangeEditEcho0Min() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnChangeEditEcho0Samp() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnChangeEditEcho0Vox() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnChangeEditEcho1Min() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnChangeEditEcho1Samp() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnChangeEditEcho1Vox() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageMic::OnChangeEditSidetGain() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageMic::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

#if !defined(R16_ATC)
	((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_SIDET_AUXGAIN))->EnableWindow(FALSE);
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPropPageMic::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the combo box Main
	if (m_nCMICMain == -1)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		m_cbCMICMain.SetFocus();
		// 没有做选择
		MessageBox(_T("Please select the main channel!"),SYS_NAME,MB_ICONSTOP);

		return FALSE;
	}

	// 2 -- the combo box Aux
	if (m_nCMICAux == -1)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		m_cbCMICAux.SetFocus();
		// 没有做选择
		MessageBox(_T("Please select the aux channel!"),SYS_NAME,MB_ICONSTOP);

		return FALSE;
	}

	int nTemp(0);

	// 3 -- the edit - SIDRET GainLevel
	nTemp = atoi(m_strSidet0MainGain);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY,  IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_SIDET_GAIN))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_SIDET_GAIN)->SetFocus();

		MessageBox(_T("Main level between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

#if defined(R16_ATC)
	nTemp = atoi(m_strSidet1AuxGain);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_SIDET_AUXGAIN))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_SIDET_AUXGAIN)->SetFocus();

		MessageBox(_T("Aux level between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
#endif

	// 4 -- the edit - ECHO0 - vox
	nTemp = atoi(m_strECHO0Vox);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_VOX))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_VOX)->SetFocus();

		MessageBox(_T("voxgain of main channel between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 5 -- the edit - ECHO0 - min
	nTemp = atoi(m_strECHO0Min);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_MIN))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_MIN)->SetFocus();

		MessageBox(_T("minMicEnergy of main channel between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 6 -- the edit - ECHO0 - samp
	nTemp = atoi(m_strECHO0Samp);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_SAMP))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_SAMP)->SetFocus();

		MessageBox(_T("sampSlncePrd of main channel between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 7 -- the edit - ECHO1 - vox
	nTemp = atoi(m_strECHO1Vox);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_VOX))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_VOX)->SetFocus();

		MessageBox(_T("voxgain of aux channel between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 8 -- the edit - ECHO1 - min
	nTemp = atoi(m_strECHO1Min);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_MIN))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_MIN)->SetFocus();

		MessageBox(_T("minMicEnergy of aux channel between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 9 -- the edit - ECHO1 - samp
	nTemp = atoi(m_strECHO1Samp);
	if ((nTemp<0)||(nTemp>32767))
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_C_MIC);
		((CEdit*)GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_SAMP))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_SAMP)->SetFocus();

		MessageBox(_T("sampSlncePrd of main channel between 0 and 32767!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

void CPropPageMic::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the combo box Main
	m_pParentWnd->m_strMIC_CMIC0_nCMICMain.Format("%d", m_nCMICMain);

	// 2 -- the combo box Aux
	m_pParentWnd->m_strMIC_CMIC1_nCMICAux.Format("%d", m_nCMICAux);

	// 3 -- the edit - SIDRET GainLevel
#if defined(R16_ATC)
	m_pParentWnd->m_strMIC_SIDET_MainGain = m_strSidet0MainGain;

	m_pParentWnd->m_strMIC_SIDET_AuxGain  = m_strSidet1AuxGain;
#else
	m_pParentWnd->m_strMIC_SIDET_Gain     = m_strSidet0MainGain;
#endif

	// 4 -- the edit - ECHO0 - vox
	m_pParentWnd->m_strMIC_ECHO0_Vox = m_strECHO0Vox;

	// 5 -- the edit - ECHO0 - min
	m_pParentWnd->m_strMIC_ECHO0_Min = m_strECHO0Min;

	// 6 -- the edit - ECHO0 - samp
	m_pParentWnd->m_strMIC_ECHO0_Samp = m_strECHO0Samp;

	// 7 -- the edit - ECHO1 - vox
	m_pParentWnd->m_strMIC_ECHO1_Vox = m_strECHO1Vox;

	// 8 -- the edit - ECHO1 - min
	m_pParentWnd->m_strMIC_ECHO1_Min = m_strECHO1Min;

	// 9 -- the edit - ECHO1 - samp
	m_pParentWnd->m_strMIC_ECHO1_Samp = m_strECHO1Samp;
}

void CPropPageMic::OnRefreshPage(void)
{
	// 1 -- the combo box Main
	m_nCMICMain = atoi(m_pParentWnd->m_strMIC_CMIC0_nCMICMain);

	// 2 -- the combo box Aux
	m_nCMICAux = atoi(m_pParentWnd->m_strMIC_CMIC1_nCMICAux);

	// 3 -- the edit - SIDRET GainLevel
#if defined(R16_ATC)
	m_strSidet0MainGain.Format("%d", atoi(m_pParentWnd->m_strMIC_SIDET_MainGain));
	m_strSidet1AuxGain.Format("%d", atoi(m_pParentWnd->m_strMIC_SIDET_AuxGain));
#else
	m_strSidet0MainGain.Format("%d", atoi(m_pParentWnd->m_strMIC_SIDET_Gain));
#endif

	// 4 -- the edit - ECHO0 - vox
	m_strECHO0Vox.Format("%d", atoi(m_pParentWnd->m_strMIC_ECHO0_Vox));

	// 5 -- the edit - ECHO0 - min
	m_strECHO0Min.Format("%d", atoi(m_pParentWnd->m_strMIC_ECHO0_Min));

	// 6 -- the edit - ECHO0 - samp
	m_strECHO0Samp.Format("%d", atoi(m_pParentWnd->m_strMIC_ECHO0_Samp));

	// 7 -- the edit - ECHO1 - vox
	m_strECHO1Vox.Format("%d", atoi(m_pParentWnd->m_strMIC_ECHO1_Vox));
 
	// 8 -- the edit - ECHO1 - min
	m_strECHO1Min.Format("%d", atoi(m_pParentWnd->m_strMIC_ECHO1_Min));

	// 9 -- the edit - ECHO1 - samp
	m_strECHO1Samp.Format("%d", atoi(m_pParentWnd->m_strMIC_ECHO1_Samp));

	// Set the data to screen show
	UpdateData(FALSE);
}

void CPropPageMic::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_C_COMBO_CMIC_MAIN)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_COMBO_CMIC_AUX)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_EDIT_SIDET_GAIN)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_VOX)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_MIN)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_EDIT_ECHO0_SAMP)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_VOX)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_MIN)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_C_EDIT_ECHO1_SAMP)->EnableWindow(bLock);
}

