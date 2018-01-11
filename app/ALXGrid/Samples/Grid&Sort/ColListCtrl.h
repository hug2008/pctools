// ColListCtrl.h : interface of the CColListCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLLISTCTRL_H__E4C23EA7_EB85_11D4_8A41_000102CA073A__INCLUDED_)
#define AFX_COLLISTCTRL_H__E4C23EA7_EB85_11D4_8A41_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ALXGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CColListCtrl window

class CColListCtrl : public CALXGridCtrl
{
// Construction
public:
	CColListCtrl();

// Attributes
public:
	CWnd*	m_pwndWidth;

	FLDCOL_INFO m_aFieldsColInfo[GRIDCOLCOUNT];
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColListCtrl)
	protected:
	//}}AFX_VIRTUAL
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
	virtual CALXCellMetrics* GetCellMetrics(int nCol, int nRow);
	virtual void OnSetActiveCell(int nCol, int nRow);

// Implementation
public:
	void SetColVisibl(BOOL bVisbl = TRUE);
	virtual ~CColListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColListCtrl)
	afx_msg void OnColVisibl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLLISTCTRL_H__E4C23EA7_EB85_11D4_8A41_000102CA073A__INCLUDED_)
