// XGridCtrl.h : interface of the CXGridCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_XGRIDCTRL_H_INCLUDED_)
#define ALX_XGRIDCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridCtrl.h"

class CXGridCtrl : public CALXGridCtrl
{
protected: // create from serialization only
	CXGridCtrl();
	DECLARE_DYNCREATE(CXGridCtrl)

// Attributes
protected:
	BOOL	m_bEnableCellEdit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXGridCtrl)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);

// Implementation
public:
	virtual ~CXGridCtrl();

// Generated message map functions
protected:
	//{{AFX_MSG(CXGridCtrl)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_XGRIDCTRL_H_INCLUDED_)
