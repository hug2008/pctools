// PropPageTongze.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageTongze.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageTongze dialog


CPropPageTongze::CPropPageTongze(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageTongze::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageTongze)
	m_strProdecerID = _T("01230123012301230123012");
	//}}AFX_DATA_INIT
	m_bLocalSectEnabled = TRUE;
}


void CPropPageTongze::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageTongze)
	DDX_Text(pDX, IDC_PAGE_F_EDIT_PRODUCERID, m_strProdecerID);
	DDV_MaxChars(pDX, m_strProdecerID, 23);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageTongze, CStackPage)
	//{{AFX_MSG_MAP(CPropPageTongze)
	ON_EN_CHANGE(IDC_PAGE_F_EDIT_PRODUCERID, OnChangeProducerid)
	ON_BN_CLICKED(IDC_PAGE_F_RADIO_ENABLEYES, OnPageEnableYes)
	ON_BN_CLICKED(IDC_PAGE_F_RADIO_ENABLENO, OnPageEnableNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageTongze message handlers

void CPropPageTongze::OnChangeProducerid() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageTongze::OnPageEnableYes() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageTongze::OnPageEnableNo() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageTongze::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLEYES))->SetCheck(m_bLocalSectEnabled);
	((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLENO))->SetCheck(!m_bLocalSectEnabled);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPropPageTongze::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

#ifdef  _TONGZE_PARA_BUILD
	// 1 -- the edit
	if (!m_strProdecerID.IsNumberAlpha())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_F_TONGZE);
		((CEdit*)GetDlgItem(IDC_PAGE_F_EDIT_PRODUCERID))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_F_EDIT_PRODUCERID)->SetFocus();

		MessageBox(_T("请输入正确的生产商ID信息！"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 2 -- the RADIO button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLEYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLENO))->GetCheck()
	    )
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_F_TONGZE);
		MessageBox(_T("请选择正确的本地手机号段显示方式！"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
#endif //_TONGZE_PARA_BUILD
	
	return TRUE;
}

void CPropPageTongze::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

#ifdef  _TONGZE_PARA_BUILD
	// 1 -- the edit
	m_pParentWnd->m_strTONGZE_0009_sProducerID = m_strProdecerID;

	// 2 -- the radio button
	m_pParentWnd->m_strTONGZE_0011_bLocalSectEnabled = 
		((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLEYES))->GetCheck() ? "01" : "00";
#endif  //_TONGZE_PARA_BUILD
}

void CPropPageTongze::OnRefreshPage(void)
{
#ifdef  _TONGZE_PARA_BUILD
	// 1 -- the edit
	m_strProdecerID = m_pParentWnd->m_strTONGZE_0009_sProducerID;

	// 2 -- the radio button
	if (atoi(m_pParentWnd->m_strTONGZE_0011_bLocalSectEnabled))
	{
		((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLEYES))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLENO ))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLEYES))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_PAGE_F_RADIO_ENABLENO ))->SetCheck(1);
	}
#endif  //_TONGZE_PARA_BUILD
	// Set the data to screen show
	UpdateData(FALSE);
}

