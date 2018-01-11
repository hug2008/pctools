// ALXSpinCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_SPINCTRL_H__INCLUDED_)
#define ALX_SPINCTRL_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXEditSpinCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CALXSpinCtrl window

class CALXSpinCtrl : public CSpinButtonCtrl, public CALXCellCtrl
{
// Construction
public:
	CALXSpinCtrl();

// Attributes
public:
	CALXEditSpinCtrl m_EditCtrl;
	BOOL m_bModifyed;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXSpinCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CALXSpinCtrl();
	virtual void SendLButtonDown(CPoint point);
	virtual BOOL EnableCtrl(BOOL bEnable = TRUE);
	virtual void SetFontCtrl(CFont *pFont, BOOL bRedraw = TRUE);
	virtual void SetModify(BOOL bModify);
	virtual BOOL CreateCtrl(DWORD dwStyle, CWnd *pParentWnd, UINT nID);
	virtual BOOL DestroyCtrl();
	virtual BOOL ActivateCtrl(int x, int y, int cx, int cy);
	virtual BOOL DeactivateCtrl();
	virtual void SetData(CELL_DATA CtrlData);
	virtual CELL_DATA GetCellData();
	virtual BOOL IsActive();
	virtual BOOL IsModifyed();
	// Generated message map functions
protected:
	//{{AFX_MSG(CALXSpinCtrl)
	afx_msg void OnDeltaPos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEdit();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_SPINCTRL_H__INCLUDED_)
