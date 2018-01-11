// LockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThreeItemPacking.h"
#include "LockDlg.h"
#include ".\lockdlg.h"


// CLockDlg dialog

IMPLEMENT_DYNAMIC(CLockDlg, CDialog)
CLockDlg::CLockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLockDlg::IDD, pParent)
	, m_strpw(_T(""))
{
}

CLockDlg::~CLockDlg()
{
}

void CLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strpw);
}


BEGIN_MESSAGE_MAP(CLockDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CLockDlg message handlers
BOOL CLockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText("«Î ‰»Î√‹¬Î");
	return TRUE;
}

void CLockDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	OnOK();
	CDialog::UpdateData(FALSE);
}
