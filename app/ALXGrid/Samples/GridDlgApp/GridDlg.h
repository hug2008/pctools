// GridDlg.h : header file
//

#if !defined(AFX_GRIDDLG_H__E4C23EA5_EB85_11D4_8A41_000102CA073A__INCLUDED_)
#define AFX_GRIDDLG_H__E4C23EA5_EB85_11D4_8A41_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog

class CGridDlg : public CDialog
{
// Construction
public:
	CGridDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	CGridCtrl m_wndCtrl;
	//{{AFX_DATA(CGridDlg)
	enum { IDD = IDD_GRID_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGridDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDLG_H__E4C23EA5_EB85_11D4_8A41_000102CA073A__INCLUDED_)
