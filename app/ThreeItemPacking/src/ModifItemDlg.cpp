// ModifItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThreeItemPacking.h"
#include "ModifItemDlg.h"
#include ".\modifitemdlg.h"


// CModifItemDlg dialog

IMPLEMENT_DYNAMIC(CModifItemDlg, CDialog)
CModifItemDlg::CModifItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifItemDlg::IDD, pParent)
	, m_strSerial(_T(""))
	, m_strTelnum(_T(""))
	, m_strImei(_T(""))
	, m_strCcid(_T(""))
{
}

CModifItemDlg::~CModifItemDlg()
{
}

BOOL CModifItemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	{
		m_ctrlSerial.SubclassDlgItem(IDC_EDIT1, this);
		m_ctrlSerial.bkColor(RGB(195, 195, 195));
		m_ctrlSerial.textColor(RGB(255, 0, 0));
		m_ctrlSerial.setFont(20);
		m_ctrlSerial.LimitText(5);
		m_ctrlSerial.EnableWindow(FALSE);
		m_ctrlTelnum.SubclassDlgItem(IDC_EDIT5, this);
		m_ctrlTelnum.textColor(RGB(255, 0, 0));
		m_ctrlTelnum.setFont(20);
		m_ctrlTelnum.LimitText(11);
		m_ctrlImei.SubclassDlgItem(IDC_EDIT6, this);
		m_ctrlImei.textColor(RGB(255, 0, 0));
		m_ctrlImei.setFont(20);
		m_ctrlImei.LimitText(15);
		m_ctrlCcid.SubclassDlgItem(IDC_EDIT7,  this);
		m_ctrlCcid.textColor(RGB(255, 0, 0));
		m_ctrlCcid.setFont(20);
		m_ctrlCcid.LimitText(20);
		this->SetWindowText("列表数据修改");
	}
	return TRUE;
}
void CModifItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strSerial);
	DDX_Text(pDX, IDC_EDIT5, m_strTelnum);
	DDX_Text(pDX, IDC_EDIT6, m_strImei);
	DDX_Text(pDX, IDC_EDIT7, m_strCcid);
}


BEGIN_MESSAGE_MAP(CModifItemDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CModifItemDlg message handlers

void CModifItemDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	OnOK();
	CDialog::UpdateData(FALSE);
}

void CModifItemDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	OnCancel();
	CDialog::UpdateData(FALSE);
}
