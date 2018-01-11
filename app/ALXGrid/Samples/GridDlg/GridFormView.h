// GridFormView.h : interface of the CGridFormView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDFORMVIEW_H__BF17244D_282A_11D5_A8BF_9F636C7EFA25__INCLUDED_)
#define AFX_GRIDFORMVIEW_H__BF17244D_282A_11D5_A8BF_9F636C7EFA25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

class CGridFormView : public CFormView
{
protected: // create from serialization only
	CGridFormView();
	DECLARE_DYNCREATE(CGridFormView)

public:
	CGridCtrl m_wndCtrl;
	//{{AFX_DATA(CGridFormView)
	enum{ IDD = IDD_GRID_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CGridDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridFormView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGridFormView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GridFormView.cpp
inline CGridDoc* CGridFormView::GetDocument()
   { return (CGridDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDFORMVIEW_H__BF17244D_282A_11D5_A8BF_9F636C7EFA25__INCLUDED_)
