// ColPropPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ColPropPages.h"
#include "WizUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGeneralPropPage, CPropertyPage)
IMPLEMENT_DYNCREATE(CAlignPropPage, CPropertyPage)
IMPLEMENT_DYNCREATE(CCtrlPropPage, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// CGeneralPropPage property page

CGeneralPropPage::CGeneralPropPage() : CPropertyPage(CGeneralPropPage::IDD)
{
	m_pWnd = NULL;

	//{{AFX_DATA_INIT(CGeneralPropPage)
	m_strTitle = _T("");
	m_strID = _T("");
	m_nWidth = 0;
	//}}AFX_DATA_INIT

	m_bValidate = TRUE;
}

CGeneralPropPage::~CGeneralPropPage()
{
}

void CGeneralPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralPropPage)
	DDX_Text(pDX, IDC_EDIT_TITLE, m_strTitle);
//	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 0, 10000);
	//}}AFX_DATA_MAP
	
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
	DDV_IDName(pDX, m_strID);
}


BEGIN_MESSAGE_MAP(CGeneralPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGeneralPropPage)
	ON_EN_CHANGE(IDC_EDIT_ID, OnModify)
	ON_EN_CHANGE(IDC_EDIT_TITLE, OnModify)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAlignPropPage property page

CAlignPropPage::CAlignPropPage() : CPropertyPage(CAlignPropPage::IDD)
{
	m_pWnd = NULL;

	//{{AFX_DATA_INIT(CAlignPropPage)
	m_nCAlign = -1;
	m_nHAlign = -1;
	//}}AFX_DATA_INIT
}

CAlignPropPage::~CAlignPropPage()
{
}

void CAlignPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlignPropPage)
	DDX_Radio(pDX, IDC_RADIO_CALIGN1, m_nCAlign);
	DDX_Radio(pDX, IDC_RADIO_HALIGN1, m_nHAlign);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlignPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAlignPropPage)
	ON_BN_CLICKED(IDC_RADIO_CALIGN1, OnModify)
	ON_BN_CLICKED(IDC_RADIO_CALIGN2, OnModify)
	ON_BN_CLICKED(IDC_RADIO_CALIGN3, OnModify)
	ON_BN_CLICKED(IDC_RADIO_HALIGN1, OnModify)
	ON_BN_CLICKED(IDC_RADIO_HALIGN2, OnModify)
	ON_BN_CLICKED(IDC_RADIO_HALIGN3, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCtrlPropPage property page

CCtrlPropPage::CCtrlPropPage() : CPropertyPage(CCtrlPropPage::IDD)
{
	m_pWnd = NULL;

	//{{AFX_DATA_INIT(CCtrlPropPage)
	m_nCtrl = -1;
	m_nEAlign = -1;
	m_nCHState = -1;
	m_nCBDrop = -1;
	//}}AFX_DATA_INIT
}

CCtrlPropPage::~CCtrlPropPage()
{
}

void CCtrlPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCtrlPropPage)
	DDX_Radio(pDX, IDC_RADIO_CTRL1, m_nCtrl);
	DDX_Radio(pDX, IDC_RADIO_EALIGN1, m_nEAlign);
	DDX_Radio(pDX, IDC_RADIO_CSTATE1, m_nCHState);
	DDX_Radio(pDX, IDC_RADIO_CBDROP1, m_nCBDrop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCtrlPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCtrlPropPage)
	ON_BN_CLICKED(IDC_RADIO_CTRL1, OnRadioCtrl1)
	ON_BN_CLICKED(IDC_RADIO_CTRL2, OnRadioCtrl2)
	ON_BN_CLICKED(IDC_RADIO_CTRL3, OnRadioCtrl3)
	ON_BN_CLICKED(IDC_RADIO_CTRL4, OnRadioCtrl4)
	ON_BN_CLICKED(IDC_RADIO_EALIGN1, OnModify)
	ON_BN_CLICKED(IDC_RADIO_EALIGN2, OnModify)
	ON_BN_CLICKED(IDC_RADIO_EALIGN3, OnModify)
	ON_BN_CLICKED(IDC_RADIO_CSTATE1, OnModify)
	ON_BN_CLICKED(IDC_RADIO_CSTATE2, OnModify)
	ON_BN_CLICKED(IDC_RADIO_CBDROP1, OnModify)
	ON_BN_CLICKED(IDC_RADIO_CBDROP2, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CCtrlPropPage::OnRadioCtrl1() 
{
	CWnd* pWnd = GetDlgItem(IDC_RADIO_EALIGN1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN3);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);

	SetModified(TRUE);
}

void CCtrlPropPage::OnRadioCtrl2() 
{
	CWnd* pWnd = GetDlgItem(IDC_RADIO_EALIGN1);
	if(pWnd != NULL)
		pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN2);
	if(pWnd != NULL)
		pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN3);
	if(pWnd != NULL)
		pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);

	SetModified(TRUE);
}

void CCtrlPropPage::OnRadioCtrl3() 
{
	CWnd* pWnd = GetDlgItem(IDC_RADIO_EALIGN1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN3);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE1);
	if(pWnd != NULL)
		pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE2);
	if(pWnd != NULL)
		pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);

	SetModified(TRUE);
}

void CCtrlPropPage::OnRadioCtrl4() 
{
	CWnd* pWnd = GetDlgItem(IDC_RADIO_EALIGN1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_EALIGN3);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE1);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CSTATE2);
	if(pWnd != NULL)
		pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP1);
	if(pWnd != NULL)
		pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_CBDROP2);
	if(pWnd != NULL)
		pWnd->EnableWindow(TRUE);

	SetModified(TRUE);
}


BOOL CCtrlPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	switch(m_nCtrl)
	{
	case 0:
		OnRadioCtrl1();
		break;
	case 1:
		OnRadioCtrl2();
		break;
	case 2:
		OnRadioCtrl3();
		break;
	case 3:
		OnRadioCtrl4();
		break;
	}
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCtrlPropPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}

BOOL CCtrlPropPage::OnApply() 
{
	m_pWnd->SendMessage(WM_COMMAND,ID_APPLY_NOW,0);
	
	return CPropertyPage::OnApply();
}

void CGeneralPropPage::OnModify() 
{
	m_bValidate = FALSE;
	UpdateData(TRUE);
	m_bValidate = TRUE;
	SetModified(TRUE);
}

BOOL CGeneralPropPage::OnApply() 
{

	m_pWnd->SendMessage(WM_COMMAND,ID_APPLY_NOW,0);

	return CPropertyPage::OnApply();
}

void CAlignPropPage::OnModify() 
{
	SetModified(TRUE);
}

BOOL CAlignPropPage::OnApply() 
{
	m_pWnd->SendMessage(WM_COMMAND,ID_APPLY_NOW,0);
	
	return CPropertyPage::OnApply();
}

void CCtrlPropPage::OnModify() 
{
	SetModified(TRUE);
}

void CGeneralPropPage::DDV_IDName(CDataExchange* pDX, CString& s)
{
	if (!pDX->m_bSaveAndValidate || !m_bValidate)
		return;

	if (!IsValidSymbol(s))
	{
		AfxMessageBox(CG_IDS_INVALID_CLASS);
		pDX->Fail();
	}
	else
		s.MakeUpper();
}


