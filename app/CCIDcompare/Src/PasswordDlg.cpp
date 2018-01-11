// PasswordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CCIDcompare.h"
#include "PasswordDlg.h"
#include ".\passworddlg.h"


// CPasswordDlg dialog

IMPLEMENT_DYNAMIC(CPasswordDlg, CDialog)
CPasswordDlg::CPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswordDlg::IDD, pParent)
	, m_strpw(_T(""))
{
}

CPasswordDlg::~CPasswordDlg()
{
}

void CPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strpw);
}


BEGIN_MESSAGE_MAP(CPasswordDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CPasswordDlg message handlers

BOOL CPasswordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->SetWindowText("«Î ‰»Î√‹¬Î");
	return TRUE;
}

void CPasswordDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	OnOK();
	CDialog::UpdateData(FALSE);
}
