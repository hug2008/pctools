// XGridView.h : interface of the CXGridView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_XGRIDVIEW_H_INCLUDED_)
#define ALX_XGRIDVIEW_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridView.h"

class CXGridView : public CALXGridView
{
protected: // create from serialization only
	CXGridView();
	DECLARE_DYNCREATE(CXGridView)

// Attributes
protected:
	BOOL	m_bEnableCellEdit;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXGridView)
	//}}AFX_VIRTUAL
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);

// Implementation
public:
	virtual ~CXGridView();

// Generated message map functions
protected:
	//{{AFX_MSG(CXGridView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_XGRIDVIEW_H_INCLUDED_)
