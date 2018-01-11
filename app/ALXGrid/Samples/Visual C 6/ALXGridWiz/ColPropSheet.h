// ColPropSheet.h : header file
//
// This class defines custom modal property sheet 
// CColPropSheet.
 
#ifndef __COLPROPSHEET_H__
#define __COLPROPSHEET_H__

#include "ColPropPages.h"

/////////////////////////////////////////////////////////////////////////////
// CColPropSheet

class CColPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CColPropSheet)

// Construction
public:
	CColPropSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CGeneralPropPage m_Page1;
	CAlignPropPage m_Page2;
	CCtrlPropPage m_Page3;

	BOOL m_bEdit;
	int m_nCol;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColPropSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColPropSheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CColPropSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __COLPROPSHEET_H__
