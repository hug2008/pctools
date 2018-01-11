///////////////////////////////////////////////////////////////////////////////
//
// ButtonXP.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "XPTools.h"

///////////////////////////////////////////////////////////////////////////////
class CButtonXP : public CButton
{
// Construction
public:
    CButtonXP();
	void SetToolTipText(CString sText, BOOL bActivate = TRUE);
	void SetToolTipText(UINT nStrId, BOOL bActivate = TRUE);

protected:
	void InitToolTip();

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CButtonXP)
    public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CButtonXP();

protected:
    CMouseMgr m_MouseMgr;

	// ToolTip
	CToolTipCtrl m_ToolTip;

    // Generated message map functions
protected:
    //{{AFX_MSG(CButtonXP)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseOut();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
