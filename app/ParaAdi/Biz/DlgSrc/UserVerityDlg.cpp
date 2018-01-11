// E:\CvsRoot\ParaDownload_Head\SRC\UserVerityDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\BizPhoneManager.h"
#include ".\userveritydlg.h"
#include "InputPassDlg2.h"
#include ".\VfManagerDlg.h"

// CUserVerityDlg dialog

IMPLEMENT_DYNAMIC(CUserVerityDlg, CDialog)
CUserVerityDlg::CUserVerityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserVerityDlg::IDD, pParent)
	, m_szUser(_T(""))
	, m_szPw(_T(""))
{
}

CUserVerityDlg::~CUserVerityDlg()
{
}

void CUserVerityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UV_USERNAME, m_szUser);
	DDX_Text(pDX, IDC_EDIT_UV_PASSW, m_szPw);
}


BEGIN_MESSAGE_MAP(CUserVerityDlg, CDialog)
	//ON_BN_CLICKED(IDC_BT_VY_VIEW, OnBnClickedBtVyView)
	//ON_BN_CLICKED(IDC_BT_VY_DEL, OnBnClickedBtVyDel)
	//ON_BN_CLICKED(IDC_BT_VY_ADD, OnBnClickedBtVyAdd)
	ON_BN_CLICKED(IDC_BT_VY_MANAGER, OnBnClickedBtVyManager)
END_MESSAGE_MAP()


// CUserVerityDlg message handlers

BOOL CUserVerityDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetWindowText(_T("User Verity"));

	GetDlgItem(IDC_BT_VY_MANAGER)->ShowWindow(theApp.m_bIsAdmin);

	return TRUE;
}

void CUserVerityDlg::OnBnClickedBtVyManager(void)
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CInputPassDlg2     PwDlg;
	CVfManagerDlg      ManagerDlg;
	CString  szSuperPassw = _T("");

	if (!theApp.m_bIsAdmin)
	{
		if (IDOK != PwDlg.DoModal()){
			return;
		}
		szSuperPassw.LoadString(IDS_STRING_SUPER_PASSW);
        
		if (0 != strcmp(szSuperPassw, PwDlg.m_sOldPass)){
			MessageBox(_T("password is no matching!"), _T("Err"), MB_OK|MB_ICONSTOP);
			return;
		}
	}

	ManagerDlg.DoModal();
}
