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
	
}





BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

//{{AFX_MSG_MAP(CAboutDlg)

//}}AFX_MSG_MAP

END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////

// CAboutDlg message handlers



BOOL CAboutDlg::OnInitDialog() 

{
	
	CDialog::OnInitDialog();
	
	
	
#ifdef _XIAMEN_START_BUILD
	
	CString strVer("");
	
	GetDlgItem(IDC_ABOUTDLG_VERSION)->GetWindowText(strVer);
	
	strVer += _T(" ����ʵ���ر��");
	
	GetDlgItem(IDC_ABOUTDLG_VERSION)->SetWindowText(strVer);
	
#endif // _XIAMEN_START_BUILD
	
	
	
#ifdef _YINGNUO_SECURITY_BUILD
	
#ifndef _YINGNUO_GENERAL_BUILD	
	
	CString strVer("");
	
	GetDlgItem(IDC_ABOUTDLG_VERSION)->GetWindowText(strVer);
	
	strVer += _T(" Ӣŵ�ر��");
	
	GetDlgItem(IDC_ABOUTDLG_VERSION)->SetWindowText(strVer);
	
#endif
	
#endif // _YINGNUO_SECURITY_BUILD
	
	
	
#ifdef _TONGZE_PARA_BUILD
	
	CString strVer("");
	
	GetDlgItem(IDC_ABOUTDLG_VERSION)->GetWindowText(strVer);
	
	strVer += _T(" ͨ���ر��");
	
	GetDlgItem(IDC_ABOUTDLG_VERSION)->SetWindowText(strVer);
	
#endif // _TONGZE_PARA_BUILD
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	
	// EXCEPTION: OCX Property Pages should return FALSE
	
}

