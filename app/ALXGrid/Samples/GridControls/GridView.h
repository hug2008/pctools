// GridView.h : interface of the CGridView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDVIEW_H__542018CF_5037_11D4_B300_D23119F72142__INCLUDED_)
#define AFX_GRIDVIEW_H__542018CF_5037_11D4_B300_D23119F72142__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridView.h"
#include "ALXSpinCtrl.h"

class CGridView : public CALXGridView
{
protected: // create from serialization only
	CGridView();
	DECLARE_DYNCREATE(CGridView)

// Attributes
public:
	CGridDoc* GetDocument();

// Operations
public:

// Overrides
	virtual CALXCellCtrl* CreateCellCtrl(int nCol, int nRow, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
	virtual BOOL DestroyColCtrl(int nCol);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	virtual void GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected);
	CImageList m_ImageList;

private:
	CALXSpinCtrl* m_pSpinCtrl;

// Generated message map functions
protected:
	//{{AFX_MSG(CGridView)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GridView.cpp
inline CGridDoc* CGridView::GetDocument()
   { return (CGridDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDVIEW_H__542018CF_5037_11D4_B300_D23119F72142__INCLUDED_)
