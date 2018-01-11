// GridCtrl.h : interface of the CHeadPropCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRGRIDCTRL_H_INCLUDED_)
#define AFX_CRGRIDCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeadPropCtrl.h : header file
//
#include <Afxtempl.h>
#include "XGridCtrl.h"
#include "TypeCtrl.h"
#include "ColPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CHeadPropCtrl window
typedef struct DefColInfo
{
	CString m_strTitle;
	int		m_nWidth;
	int		m_nHeadAlign;
	int		m_nCellAlign;
	int		m_nTypeControl;
	int		m_nControlStyle;
	CString m_strID;
} DEFCOL_INFO;

class CHeadPropCtrl : public CXGridCtrl
{
// Construction
public:
	DECLARE_DYNCREATE(CHeadPropCtrl)
public:
	CHeadPropCtrl();
	virtual ~CHeadPropCtrl();

// Attributes
public:
	CArray < DEFCOL_INFO, DEFCOL_INFO& > m_Cols;

private:
	CTypeCtrl*			m_pctrlTypeCtrl;
	CALXComboBoxCtrl*	m_pctrlEditStyle;
	CALXComboBoxCtrl*	m_pctrlCheckStyle;
	CALXComboBoxCtrl*	m_pctrlComboStyle;

	CImageList m_ImageList;

	CColPropSheet*		m_pPropSheet;
// Operations
public:
void SaveInfo();

private:
// Overrides
	virtual CALXCellCtrl* CreateCellCtrl(int nCol, int nRow, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
	virtual void GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected);
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeadPropCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnEditCol();
	void OnDelCol();
	void OnAddCol();
	void OnMoveColUp();
	void OnMoveColDown();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeadPropCtrl)
	afx_msg void OnSelCtrl();
	afx_msg void OnApplyNow();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRGRIDCTRL_H_INCLUDED_)
