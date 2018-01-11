#if !defined(AFX_GRIDHEADDLG_H__09AA4893_AFD0_11D4_8A40_005004555789__INCLUDED_)
#define AFX_GRIDHEADDLG_H__09AA4893_AFD0_11D4_8A40_005004555789__INCLUDED_

// GridHeadDlg.h : header file
//
#include "HeadPropCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGridHeadDlg dialog

class CGridHeadDlg : public CAppWizStepDlg
{
// Construction
public:
	CGridHeadDlg();
	virtual BOOL OnDismiss();

// Dialog Data
	//{{AFX_DATA(CGridHeadDlg)
	enum { IDD = IDD_GRIDHEAD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CToolBar		m_wndToolBar;
	CStatic			m_wndStatic;
	CHeadPropCtrl	m_wndHeadProp;

	CBitmap m_bmpGrid;
	CBitmap m_bmpFon;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridHeadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	// Generated message map functions
	//{{AFX_MSG(CGridHeadDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddcol();
	afx_msg void OnBtnDelcol();
	afx_msg void OnUpdateBtnDelcol(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnBtnPreviewGrid();
	afx_msg void OnBtnEditcol();
	afx_msg void OnBtnMoveUp();
	afx_msg void OnBtnMoveDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDHEADDLG_H__09AA4893_AFD0_11D4_8A40_005004555789__INCLUDED_)
