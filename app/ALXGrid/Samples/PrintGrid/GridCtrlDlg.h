// GridCtrlDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCTRLDLG_H_INCLUDED_)
#define AFX_GRIDCTRLDLG_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGridCtrlDlg dialog

class CGridCtrlDlg : public CDialog
{
// Construction
public:
	CGridCtrlDlg(CWnd* pParent = NULL);   // standard constructor

	CGridCtrl m_GridCtrl;	// эл. управления список полей таблицы
// Dialog Data
	//{{AFX_DATA(CGridCtrlDlg)
	enum { IDD = IDD_GRID_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGridCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCTRLDLG_H_INCLUDED_)
