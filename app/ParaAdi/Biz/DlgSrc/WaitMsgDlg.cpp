// WaitMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "WaitMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitMsgDlg dialog


CWaitMsgDlg::CWaitMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitMsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWaitMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitMsgDlg)
	DDX_Control(pDX, IDC_MSG_DLG_EDIT_TASK, m_ctrTask);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CWaitMsgDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitMsgDlg message handlers

void CWaitMsgDlg::OnOK() 
{
	// 使得对话框不会关闭
	// CDialog::OnOK();
}

void CWaitMsgDlg::OnCancel() 
{
	// 使得对话框不会关闭
	CDialog::OnCancel();
}

void CWaitMsgDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
