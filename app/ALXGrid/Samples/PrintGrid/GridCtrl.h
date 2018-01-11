// GridCtrl.h : interface of the CGridCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCTRL_H_INCLUDED_)
#define AFX_GRIDCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Grid.h"
#include "XGridCtrl.h"

typedef struct Reestr
{
	COleCurrency m_Dedet;
	COleCurrency m_Credit;
} RECORD_INFO;

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl

class CGridCtrl : public CXGridCtrl
{
public:
// Construction/destruction
	DECLARE_DYNCREATE(CGridCtrl)

	CGridCtrl();
	~CGridCtrl();

	CArray < RECORD_INFO, RECORD_INFO& > m_RowInfo;

// Attributes
private:
    CSize       m_LogicalPageSize,      // Page size in gridctrl units.
                m_PaperSize;            // Page size in device units.

	int m_nRowInPage;
	int m_nPageWight;
	int m_nPageHight;

// Operations
private:
	// переопредел€ем виртуальные функции класса CXGridCtrl
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);

	// функции дл€ печати
	void PrintHead(CDC* pDC, CPrintInfo* pInfo);						// печать заголовка таблицы
	void PrintGridPage(CDC* pDC, CPrintInfo* pInfo);					// печать той части грида на странице 
	void PrintHeadCell(CDC* pDC, int nCol, int nRow, int x, int y);		// печать €чейки заголовка
	void PrintGridCell(CDC* pDC, int nCol, int nRow, int x, int y);		// печать €чейки сетки
	void PrintText(CDC* pDC, CString& str, CRect& rect, UINT nFormat );	// печать текста в €чейке
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Print();

protected:
	BOOL DoPreparePrinting(CPrintInfo* pInfo);

	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);
	// Generated message map functions
protected:

	//{{AFX_MSG(CGridCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCTRL_H_INCLUDED_)
