// PropPageXiamen.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageXiamen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageXiamen dialog


CPropPageXiamen::CPropPageXiamen(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageXiamen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageXiamen)
	m_strProdecerID = _T("01230123012301230123012");
	m_strMscReceiveNum = _T("");
	//}}AFX_DATA_INIT
	m_bShowOriginalPhoneNum = FALSE;
	m_bLocalSectEnabled = TRUE;
}


void CPropPageXiamen::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageXiamen)
	DDX_Text(pDX, IDC_PAGE_D_EDIT_PRODUCERID, m_strProdecerID);
	DDV_MaxChars(pDX, m_strProdecerID, 23);
	DDX_Text(pDX, IDC_PAGE_D_EDIT_MSCRECEIVENUM, m_strMscReceiveNum);
	DDV_MaxChars(pDX, m_strMscReceiveNum, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageXiamen, CStackPage)
	//{{AFX_MSG_MAP(CPropPageXiamen)
	ON_EN_CHANGE(IDC_PAGE_D_EDIT_MSCRECEIVENUM, OnChangeMscreceivenum)
	ON_EN_CHANGE(IDC_PAGE_D_EDIT_PRODUCERID, OnChangeProducerid)
	ON_BN_CLICKED(IDC_PAGE_D_RADIO_SHOWYES, OnPageRadioShowYes)
	ON_BN_CLICKED(IDC_PAGE_D_RADIO_SHOWNO, OnPageRadioShowNo)
	ON_BN_CLICKED(IDC_PAGE_D_RADIO_ENABLEYES, OnPageEnableYes)
	ON_BN_CLICKED(IDC_PAGE_D_RADIO_ENABLENO, OnPageEnableNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageXiamen message handlers

void CPropPageXiamen::OnChangeMscreceivenum() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageXiamen::OnChangeProducerid() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageXiamen::OnPageRadioShowYes() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageXiamen::OnPageRadioShowNo() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageXiamen::OnPageEnableYes() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageXiamen::OnPageEnableNo() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageXiamen::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWYES))->SetCheck(m_bShowOriginalPhoneNum);
	((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWNO))->SetCheck(!m_bShowOriginalPhoneNum);
	
	((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_ENABLEYES))->SetCheck(m_bLocalSectEnabled);
	((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_ENABLENO))->SetCheck(!m_bLocalSectEnabled);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPropPageXiamen::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

#ifdef  _XIAMEN_START_BUILD
	// 1 -- the edit
	if (!m_strProdecerID.IsNumberAlpha())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_D_XIAMEN_PARA);
		((CEdit*)GetDlgItem(IDC_PAGE_D_EDIT_PRODUCERID))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_D_EDIT_PRODUCERID)->SetFocus();

		MessageBox(_T("请输入正确的生产商ID信息！"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 2 -- the edit
	if (!m_strMscReceiveNum.IsEmptyOrPhoneString())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_D_XIAMEN_PARA);
		((CEdit*)GetDlgItem(IDC_PAGE_D_EDIT_MSCRECEIVENUM))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_D_EDIT_MSCRECEIVENUM)->SetFocus();

		MessageBox(_T("请输入正确的接收网管中心号码！"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 3 -- the RADIO button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWNO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_D_XIAMEN_PARA);
		MessageBox(_T("请选择正确的原始号码显示方式！"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
#endif //_XIAMEN_START_BUILD
	
	return TRUE;
}

void CPropPageXiamen::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

#ifdef  _XIAMEN_START_BUILD
	// 1 -- the edit
	m_pParentWnd->m_strXIAMEN_0009_sProducerID = m_strProdecerID;

	// 2 -- the edit
	m_pParentWnd->m_strXIAMEN_0006_sMscReceiveNum = m_strMscReceiveNum;

	// 3 -- the radio button
	m_pParentWnd->m_strXIAMEN_0007_bShowOriginalNum = 
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWYES))->GetCheck() ? "01" : "00";

	// 4 -- the radio button
	m_pParentWnd->m_strXIAMEN_0011_bLocalSectEnabled = 
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_ENABLEYES))->GetCheck() ? "01" : "00";
#endif  //_XIAMEN_START_BUILD
}

void CPropPageXiamen::OnRefreshPage(void)
{
#ifdef  _XIAMEN_START_BUILD
	// 1 -- the edit
	m_strProdecerID = m_pParentWnd->m_strXIAMEN_0009_sProducerID;

	// 2 -- the edit
	m_strMscReceiveNum = m_pParentWnd->m_strXIAMEN_0006_sMscReceiveNum;

	// 3 -- the radio button
	if (atoi(m_pParentWnd->m_strXIAMEN_0007_bShowOriginalNum))
	{
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWYES))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWNO ))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWYES))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_SHOWNO ))->SetCheck(1);
	}

	// 4 -- the radio button
	if (atoi(m_pParentWnd->m_strXIAMEN_0011_bLocalSectEnabled))
	{
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_ENABLEYES))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_ENABLENO ))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_ENABLEYES))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_PAGE_D_RADIO_ENABLENO ))->SetCheck(1);
	}
#endif  //_XIAMEN_START_BUILD
	// Set the data to screen show
	UpdateData(FALSE);
}

