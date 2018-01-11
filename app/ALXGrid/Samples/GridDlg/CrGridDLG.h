#if !defined(AFX_CRGRIDDLG_H__4BDAB1A2_570A_11D4_8A40_005004555789__INCLUDED_)
#define AFX_CRGRIDDLG_H__4BDAB1A2_570A_11D4_8A40_005004555789__INCLUDED_


#include "CrGridCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrGridDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrGridDLG dialog

class CCrGridDLG : public CDialog
{
// Construction
public:
	CCrGridDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	CCrGridCtrl m_GridCtrl;
	//{{AFX_DATA(CCrGridDLG)
	enum { IDD = IDD_CRGRID_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrGridDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCrGridDLG)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRGRIDDLG_H__4BDAB1A2_570A_11D4_8A40_005004555789__INCLUDED_)
