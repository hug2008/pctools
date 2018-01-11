// PropPageLocal.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageLocal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropPageLocal dialog

CPropPageLocal::CPropPageLocal(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLocal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageLocal)
	m_strLocalNum = _T("");
	m_nLocalMethod = 0;
	//}}AFX_DATA_INIT
}


void CPropPageLocal::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocal)
	DDX_Control(pDX, IDC_PAGE_2_COMBO_LOCALMETHOD, m_cbLocalMethod);
	DDX_Text(pDX, IDC_PAGE_2_EDIT_LOCALNUM, m_strLocalNum);
	DDV_MaxChars(pDX, m_strLocalNum, 6);
	DDX_CBIndex(pDX, IDC_PAGE_2_COMBO_LOCALMETHOD, m_nLocalMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageLocal, CStackPage)
	//{{AFX_MSG_MAP(CPropPageLocal)
	ON_CBN_SELCHANGE(IDC_PAGE_2_COMBO_LOCALMETHOD, OnSelchangeLocalMethod)
	ON_EN_CHANGE(IDC_PAGE_2_EDIT_LOCALNUM, OnChangePage2EditLocalnum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocal message handlers

void CPropPageLocal::OnSelchangeLocalMethod() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLocal::OnChangePage2EditLocalnum() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageLocal::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPropPageLocal::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the edit

	// ----------2003.08.20 - PengWei said, the Local zone number could be empty! --------
	// ---------- So i have to cut this sect ---------------------------------------------
	/*
	if (m_strLocalNum.IsEmpty())
	{
		MessageBox(_T("请输入正确的本地区号！"),SYS_NAME,MB_ICONSTOP);

		((CEdit*)GetDlgItem(IDC_PAGE_2_EDIT_LOCALNUM))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_2_EDIT_LOCALNUM)->SetFocus();
		return FALSE;
	}
	*/

	// 2 -- the combo box
	if (m_nLocalMethod == -1)
	{
		// 没有做选择
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_2_LOCAL);
		m_cbLocalMethod.SetFocus();

		MessageBox(_T("Please select the using method!"),SYS_NAME,MB_ICONSTOP);

		return FALSE;
	}

	return TRUE;
}

void CPropPageLocal::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	m_pParentWnd->m_strLOCAL_1000_strLocalZone = m_strLocalNum;
	m_pParentWnd->m_strLOCAL_0003_nLocalMethod.Format("0%d", m_nLocalMethod);
}

void CPropPageLocal::OnRefreshPage(void)
{
	CString strTemp;
	
	// 1 ---   m_strLocalNum
	m_strLocalNum = m_pParentWnd->m_strLOCAL_1000_strLocalZone;

	// 2 ---   m_strSMSCenter
	m_nLocalMethod = atoi(m_pParentWnd->m_strLOCAL_0003_nLocalMethod);

	// Set the data to screen show
	UpdateData(FALSE);
}

void CPropPageLocal::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_2_COMBO_LOCALMETHOD)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_2_EDIT_LOCALNUM)->EnableWindow(bLock);
}

