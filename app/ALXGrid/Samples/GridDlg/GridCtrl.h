// GridCtrl.h : interface of the CGridCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCTRL_H__E4C23EA7_EB85_11D4_8A41_000102CA073A__INCLUDED_)
#define AFX_GRIDCTRL_H__E4C23EA7_EB85_11D4_8A41_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridCtrl.h"

typedef struct Reestr
{
	COleCurrency m_Dedet;
	COleCurrency m_Credit;
} RECORD_INFO;

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl window

class CGridCtrl : public CALXGridCtrl
{
// Construction
public:
	CGridCtrl();

	CArray < RECORD_INFO, RECORD_INFO& > m_RowInfo;

	CFont	 m_SumFont;

// Attributes
public:

private:
	CALXCellMetrics m_MetricsCellsDebet;
	CALXCellMetrics m_MetricsCellsCredit;
	CALXCellMetrics m_MetricsCellsSaldo;
	CALXCellMetrics m_MetricsCellsSum;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
	virtual CALXCellMetrics* GetCellMetrics(int nCol, int nRow);
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);

// Implementation
public:
	virtual ~CGridCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCTRL_H__E4C23EA7_EB85_11D4_8A41_000102CA073A__INCLUDED_)
