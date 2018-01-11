// GridView.h : interface of the CGridView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDVIEW_H__218AE4EB_1926_11D5_8A50_000102CA073A__INCLUDED_)
#define AFX_GRIDVIEW_H__218AE4EB_1926_11D5_8A50_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridView.h"

class CPaymentSet;

class CGridView : public CALXGridView
{
protected: // create from serialization only
	CGridView();
	DECLARE_DYNCREATE(CGridView)

private:
	CPaymentSet* m_pSet;

	FLDCOL_INFO m_aFieldsColInfo[GRIDCOLCOUNT];


// Attributes
public:
	CGridDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
	virtual void SetHeadColWidth(int nCol, int nRow, int nNewWidth);


// Сортируем строки
	virtual void OnHeadButtonDown(int nHeadCol, int nHeadRow);

// Implementation
public:
	virtual ~CGridView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGridView)
	afx_msg void OnViewColumns();
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

#endif // !defined(AFX_GRIDVIEW_H__218AE4EB_1926_11D5_8A50_000102CA073A__INCLUDED_)
