// GridCtrl.h : interface of the CRegGridCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGGRIDCTRL_H_INCLUDED_)
#define AFX_REGGRIDCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridCtrl.h"

class CRegGridCtrl : public CALXGridCtrl
{
public: // create from serialization only
	CRegGridCtrl();
	~CRegGridCtrl();
	DECLARE_DYNCREATE(CRegGridCtrl)
	DECLARE_REGISTER()

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegGridCtrl)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRegGridCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual HBITMAP GetBitmap(int nCol, int nRow, BOOL bSelected);
	CBitmap m_BMP;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGGRIDCTRL_H_INCLUDED_)
