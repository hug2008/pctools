// $$wndview_hfile$$.h : interface of the $$WNDVIEW_CLASS$$ class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

$$IF(CXGridCtrl)
#include "XGridCtrl.h"
$$ENDIF

/////////////////////////////////////////////////////////////////////////////
// $$WNDVIEW_CLASS$$ window

class $$WNDVIEW_CLASS$$ : public $$WNDVIEW_BASE_CLASS$$
{
// Construction
public:
	$$WNDVIEW_CLASS$$();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$WNDVIEW_CLASS$$)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
$$IF(CXGridCtrl)
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
$$ENDIF //CXGridCtrl

// Implementation
public:
	virtual ~$$WNDVIEW_CLASS$$();

	// Generated message map functions
protected:
	//{{AFX_MSG($$WNDVIEW_CLASS$$)
$$IF(!CXGridCtrl)
	afx_msg void OnPaint();
$$ENDIF //!CXGridCtrl
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
