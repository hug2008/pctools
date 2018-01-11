#if !defined(AFX_COLUMNSDLG_H__572AEAC1_024F_11D5_8A4A_000102CA073A__INCLUDED_)
#define AFX_COLUMNSDLG_H__572AEAC1_024F_11D5_8A4A_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColumnsDlg.h : header file
//
#include "ColListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CColumnsDlg dialog

class CColumnsDlg : public CDialog
{
// Construction
public:
	CColumnsDlg(CWnd* pParent = NULL);   // standard constructor
	~CColumnsDlg();

// Dialog Data
	CColListCtrl	m_wndCtrl;

	//{{AFX_DATA(CColumnsDlg)
	enum { IDD = IDD_DLG_COLUMNS };
	CButton	m_ctrlHide;
	CButton	m_ctrlShow;
	CButton	m_ctrlDown;
	CButton	m_ctrlUP;
	int		m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColumnsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColumnsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeWidth();
	afx_msg void OnSetColumn();
	afx_msg void OnSetColVisbl();
	afx_msg void OnBtnShow();
	afx_msg void OnBtnHide();
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnSetDef();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMNSDLG_H__572AEAC1_024F_11D5_8A4A_000102CA073A__INCLUDED_)
