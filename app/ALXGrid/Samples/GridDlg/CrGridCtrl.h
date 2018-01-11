// GridCtrl.h : interface of the CCrGridCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRGRIDCTRL_H_INCLUDED_)
#define AFX_CRGRIDCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventGrig.h : header file
//
#include "ALXGridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CEventGrig window

class CCrGridCtrl : public CALXGridCtrl
{
// Construction
public:
	DECLARE_DYNCREATE(CCrGridCtrl)
public:
	CCrGridCtrl();
	virtual ~CCrGridCtrl();

// Attributes
private:
	CBitmap m_BMP;

// Operations
private:
	virtual LPARAM GetImage(int nCol, int nRow, BOOL bSelected);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrGridCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	//{{AFX_MSG(CCrGridCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRGRIDCTRL_H_INCLUDED_)
