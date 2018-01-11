// ALXEditSpinCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////
#if !defined(ALX_EDITSPINCTRL_H_INCLUDED_)
#define ALX_EDITSPINCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXEditCtrl.h"

/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
#define VK_0			0x30
#define VK_9			0x39

/////////////////////////////////////////////////////////////////////////////
// CALXEditSpinCtrl window
class CALXEditSpinCtrl : public CALXEditCtrl
{
// Construction
public:
	CALXEditSpinCtrl();

// Attributes
public:
	HWND m_hWndSpin;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXEditSpinCtrl)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CALXEditSpinCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CALXEditSpinCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_EDITSPINCTRL_H_INCLUDED_)
