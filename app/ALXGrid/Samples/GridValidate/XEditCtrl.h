// XEditCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_XEDITCTRL_H_INCLUDED_)
#define ALX_XEDITCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CXEditCtrl window

class CXEditCtrl : public CALXEditCtrl
{
// Construction, Destruction
public:
	CXEditCtrl();
	~CXEditCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL OnValidate();


	// Generated message map functions
protected:
	//{{AFX_MSG(CXEditCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_XEDITCTRL_H_INCLUDED_)
