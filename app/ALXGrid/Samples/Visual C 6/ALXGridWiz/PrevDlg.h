#if !defined(AFX_PREVDLG_H__04AACCC6_B623_11D4_8A40_005004555789__INCLUDED_)
#define AFX_PREVDLG_H__04AACCC6_B623_11D4_8A40_005004555789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrevDlg.h : header file
//
#include "GridPrevCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPrevDlg dialog

class CPrevDlg : public CDialog
{
// Construction
public:
	CPrevDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrevDlg)
	enum { IDD = IDD_GRIDPREV };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CGridPrevCtrl	m_wndGridPrev;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrevDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrevDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVDLG_H__04AACCC6_B623_11D4_8A40_005004555789__INCLUDED_)
