 /***************************************************************************
 *
 *                         <����绰����ϵͳ>
 *
 *               <Copyright 2003-2005 SIMCOM.COM �����>
 *
 *****************************************************************************
 *  ģ����	    ��About Dialog
 *  �ļ���	    ��AboutDlg.CPP
 *  ����ļ�	��AboutDlg.h
 *  ʵ�ֹ���    ��
 *  ����		����ʤ
 *  �汾		��V1.0
  *  ʱ��		��2003/8/14
*****************************************************************************
 *  �޸ļ�¼:
 *
 *      ��  ��		�汾		�޸���		
 *      �޸����ݣ������޸ĵ�ԭ�򼰺�����
 *
 *****************************************************************************/// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ABOUTDLG_EMAIL, m_lEmail);
	DDX_Control(pDX, IDC_ST_SYSNAME, m_stSysName);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CString strVer("");

	CDialog::OnInitDialog();

	m_stSysName.SetWindowText(SYS_NAME);

	GetDlgItem(IDC_ABOUTDLG_VERSION)->GetWindowText(strVer);
	strVer += VERSION_ID;
	strVer += _T(" ");
	strVer += VERSION_STATE;
	GetDlgItem(IDC_ABOUTDLG_VERSION)->SetWindowText(strVer);

	strVer.Empty();
	strVer += _T("Build On: ");
	strVer += __DATE__;
	GetDlgItem(IDC_ST_COPYRIGHT)->SetWindowText(strVer);

#if defined(FOR_CLIENT)
	m_lEmail.SetURL("mailto:huangqr@desay.com");
#else
	m_lEmail.SetURL("http://10.1.1.10/");
#endif

	/*
	m_lEmail.ShowWindow(SW_HIDE);
	m_lEmail.EnableWindow(FALSE);
	*/

	SetWindowText("Sys Info");

	return TRUE;
}
