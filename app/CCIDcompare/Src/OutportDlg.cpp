// OutportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CCIDcompare.h"
#include "OutportDlg.h"
#include ".\outportdlg.h"


// COutportDlg dialog

IMPLEMENT_DYNAMIC(COutportDlg, CDialog)
COutportDlg::COutportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutportDlg::IDD, pParent)
	, m_bTelnum(TRUE)
	, m_bImei(TRUE)
	, m_bCcid(TRUE)
	, m_bSerial(TRUE)
	, m_bArea(TRUE)
	, m_bBuyer(TRUE)
	, m_bcCheck(TRUE)
	, m_bcBuyer(TRUE)
	, m_bcArea(TRUE)
	, m_strbcBuyer(_T("NC1234"))
	, m_strbcArea(_T("新都"))
	, m_iSelCheck(0)
	, m_strnewtablename(_T("新表的名字"))
{
}

COutportDlg::~COutportDlg()
{
}

void COutportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_bSerial);
	DDX_Check(pDX, IDC_CHECK2, m_bTelnum);
	DDX_Check(pDX, IDC_CHECK3, m_bImei);
	DDX_Check(pDX, IDC_CHECK4, m_bCcid);
	DDX_Check(pDX, IDC_CHECK5, m_bArea);
	DDX_Check(pDX, IDC_CHECK6, m_bBuyer);
	DDX_Check(pDX, IDC_CHECK7, m_bcCheck);
	DDX_Check(pDX, IDC_CHECK8, m_bcBuyer);
	DDX_Check(pDX, IDC_CHECK9, m_bcArea);
	DDX_Text(pDX, IDC_EDIT1, m_strbcBuyer);
	DDX_Text(pDX, IDC_EDIT4, m_strbcArea);
	DDX_Control(pDX, IDC_COMBO1, m_CtrlBoxCheck);
	DDX_CBIndex(pDX, IDC_COMBO1, m_iSelCheck);
	DDX_Text(pDX, IDC_EDIT5, m_strnewtablename);
}


BEGIN_MESSAGE_MAP(COutportDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK7, OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnBnClickedCheck9)
END_MESSAGE_MAP()


// COutportDlg message handlers
BOOL  COutportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CtrlBoxCheck.AddString("是");
	m_CtrlBoxCheck.AddString("否");
	m_CtrlBoxCheck.SetCurSel(1);
	m_CtrlBoxCheck.EnableWindow(m_bcCheck);

	this->GetDlgItem(IDC_EDIT1)->EnableWindow(m_bcBuyer);
	this->GetDlgItem(IDC_EDIT4)->EnableWindow(m_bcArea);
	this->GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
	return TRUE;
}
void COutportDlg::OnBnClickedCheck7()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	m_CtrlBoxCheck.EnableWindow(m_bcCheck);
	CDialog::UpdateData(FALSE);
}

void COutportDlg::OnBnClickedCheck8()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	this->GetDlgItem(IDC_EDIT1)->EnableWindow(m_bcBuyer);
	CDialog::UpdateData(FALSE);	
}

void COutportDlg::OnBnClickedCheck9()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	this->GetDlgItem(IDC_EDIT4)->EnableWindow(m_bcArea);
	CDialog::UpdateData(FALSE);	
}
