// ALXEditCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_EDITCTRL_H_INCLUDED_)
#define ALX_EDITCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXCellCtrl.h"

#define ES_XGRID          0x8000L

/////////////////////////////////////////////////////////////////////////////
// CALXEditCtrl window

class CALXEditCtrl : public CEdit,public CALXCellCtrl
{
// Construction, Destruction
public:
	CALXEditCtrl();
	~CALXEditCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXEditCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL EnableCtrl(BOOL bEnable = TRUE);
	virtual void SetFontCtrl(CFont *pFont, BOOL bRedraw = TRUE);
	virtual void SetModify(BOOL bModify);
	virtual BOOL CreateCtrl(DWORD dwStyle, CWnd *pParentWnd, UINT nID);
	virtual BOOL DestroyCtrl();
	virtual BOOL ActivateCtrl(int x, int y, int cx, int cy);
	virtual BOOL DeactivateCtrl();
	virtual void SetData(CELL_DATA CtrlData);
	virtual CELL_DATA GetCellData();
	virtual BOOL IsModifyed();
	virtual BOOL OnValidate();
	virtual BOOL IsActive();

// ������� ��� ������ � ������� ������
	virtual BOOL IsCanCut();
	virtual BOOL IsCanCopy();
	virtual BOOL IsCanPaste();
	virtual BOOL IsCanUndo();
	virtual void Cut();
	virtual void Copy();
	virtual void Paste();
	virtual void Undo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CALXEditCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_EDITCTRL_H_INCLUDED_)
