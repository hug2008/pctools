// GridView.h : interface of the CGridView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDVIEW_H__542018CF_5037_11D4_B300_D23119F72142__INCLUDED_)
#define AFX_GRIDVIEW_H__542018CF_5037_11D4_B300_D23119F72142__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridView.h"

#define PRN_XMARGIN		5
#define PRN_YMARGIN		5

static int nRowInPage, PageWight, PageHight;

class CGridView : public CALXGridView
{
protected: // create from serialization only
	CGridView();
	DECLARE_DYNCREATE(CGridView)

	void PrintHead(CDC* pDC, CPrintInfo* pInfo);
	void PrintGridPage(CDC* pDC, CPrintInfo* pInfo);
	void PrintHeadCell(CDC* pDC, int nCol, int nRow, int x, int y);
	void PrintGridCell(CDC* pDC, int nCol, int nRow, int x, int y);
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
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridView();
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual void GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected);
	///
	// Получаем количество столбцов в сетке
	virtual int GetHeadColCount(int nHeadRow = -1);
	// Получаем ширину колонки
	virtual int	 GetHeadColWidth(int nCol, int nHeadRow = -1);
	// Получаем информацию о ячейке шапки
	virtual HEADCELL_INFO GetHeadCellInfo(int nCol, int nRow);
	// Устанавливаем ширину колонки
	virtual void SetHeadColWidth(int nCol, int nRow, int nNewWidth);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	CFont		m_HeadFont;
	CImageList	m_ImageList;

    CSize       m_PaperSize;	// Page size in device units.

// Generated message map functions
protected:
	//{{AFX_MSG(CGridView)
	afx_msg void OnSetGridFont();
	afx_msg void OnSetHeadFont();
	afx_msg void OnUpdateGStyleRowselect(CCmdUI* pCmdUI);
	afx_msg void OnGStyleRowselect();
	afx_msg void OnUpdateGStyleFlat(CCmdUI* pCmdUI);
	afx_msg void OnGStyleFlat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawText(CDC* pDC, CString& str, CRect& rect, UINT nFormat );
};

#ifndef _DEBUG  // debug version in GridView.cpp
inline CGridDoc* CGridView::GetDocument()
   { return (CGridDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDVIEW_H__542018CF_5037_11D4_B300_D23119F72142__INCLUDED_)
