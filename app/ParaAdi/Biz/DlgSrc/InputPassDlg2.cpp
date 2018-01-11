// InputPassDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "InputPassDlg2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputPassDlg2 dialog


CInputPassDlg2::CInputPassDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CInputPassDlg2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputPassDlg2)
	m_sOldPass = _T("");
	//}}AFX_DATA_INIT
}


void CInputPassDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPassDlg2)
	DDX_Text(pDX, IDC_OLDPASS, m_sOldPass);
	DDV_MaxChars(pDX, m_sOldPass, 8);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPassDlg2, CDialog)
	//{{AFX_MSG_MAP(CInputPassDlg2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPassDlg2 message handlers

void CInputPassDlg2::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_sOldPass==""){
		((CEdit*)GetDlgItem(IDC_OLDPASS))->SetSel(0,-1);
		GetDlgItem(IDC_OLDPASS)->SetFocus();
		MessageBox(_T("The password can't be empty!"),SYS_NAME,MB_ICONSTOP);
	}else
	CDialog::OnOK();
}

BOOL CInputPassDlg2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_OLDPASS)->SetFocus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
