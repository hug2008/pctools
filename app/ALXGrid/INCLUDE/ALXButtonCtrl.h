// ALXButtonCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_BUTTONCTRL_H_INCLUDED_)
#define ALX_BUTTONCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXCellCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CALXButtonCtrl

class CALXButtonCtrl : public CButton, public CALXCellCtrl
{
// Конструктор, Деструктор
public:
	CALXButtonCtrl();
	~CALXButtonCtrl();

// Attributes
public:

private:
	CBrush	m_brushCtrl;
	BYTE	m_bState;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXButtonCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void SendLButtonDown(UINT nFlags, CPoint point);
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
	virtual BOOL OnValidate();

	// Generated message map functions
protected:
	//{{AFX_MSG(CALXButtonCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_BUTTONCTRL_H_INCLUDED_)
