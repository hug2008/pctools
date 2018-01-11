// GridPrevCtrl.h : interface of the CGridPrevCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDPREVCTRL_H_INCLUDED_)
#define AFX_GRIDPREVCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventGrig.h : header file
//
#include <Afxtempl.h>
#include "XGridCtrl.h"
#include "TypeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CGridPrevCtrl

typedef struct ColRows
{
   CELL_DATA Rows[20];
} PREV_COLROWS;

class CGridPrevCtrl : public CXGridCtrl
{
// Construction
public:
	DECLARE_DYNCREATE(CGridPrevCtrl)
public:
	CGridPrevCtrl();
	virtual ~CGridPrevCtrl();

// Attributes
private:

// Operations
public:
	CArray < PREV_COLROWS, PREV_COLROWS& > m_Cols;

private:
// Overrides
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridPrevCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridPrevCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDPREVCTRL_H_INCLUDED_)
