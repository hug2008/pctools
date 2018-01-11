 /***************************************************************************
 *
 *                         <商务电话管理系统>
 *
 *               <Copyright 2003-2005 SIMCOM.COM 软件部>
 *
 *****************************************************************************
 *  模块名	    ：About Dialog
 *  文件名	    ：AboutDlg.h
 *  相关文件	：AboutDlg.CPP
 *  实现功能    ：
 *  作者		：陈胜
 *  版本		：V1.0
 *  时间		：2003/8/14
 *****************************************************************************
 *  修改记录:
 *
 *      日  期		版本		修改人		
 *      修改内容（包括修改的原因及函数）
 *
 *****************************************************************************/

#include "afxwin.h"
#if !defined(AFX_ABOUTDLG_H__B484EDF6_6866_49F0_80DA_0046337128D6__INCLUDED_)
#define AFX_ABOUTDLG_H__B484EDF6_6866_49F0_80DA_0046337128D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//
#include "MapHyperLink.h"
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
// Construction
public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_DLG_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	CStatic m_stSysName;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMapHyperLink m_lEmail;
	// Generated message map functions
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__B484EDF6_6866_49F0_80DA_0046337128D6__INCLUDED_)
