#if !defined(AFX_TYPECTRL_H__6968D165_B228_11D4_8A40_005004555789__INCLUDED_)
#define AFX_TYPECTRL_H__6968D165_B228_11D4_8A40_005004555789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeCtrl.h : header file
//
#include "ALXComboBoxCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTypeCtrl window

class CTypeCtrl : public CALXComboBoxCtrl
{
// Construction
public:
	CTypeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTypeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTypeCtrl)
	afx_msg void OnSelOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPECTRL_H__6968D165_B228_11D4_8A40_005004555789__INCLUDED_)
