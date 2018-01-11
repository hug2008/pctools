// ALXComboBoxCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_COMBOBOXCTRL_H_INCLUDED_)
#define ALX_COMBOBOXCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXCellCtrl.h"

#define IDW_ALXCOMBOEDIT		1001
/////////////////////////////////////////////////////////////////////////////
// CALXComboEditCtrl window
class CALXComboEditCtrl : public CEdit
{
// Construction, Destruction
public:
	CALXComboEditCtrl();
	~CALXComboEditCtrl();
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXComboEditCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CALXComboEditCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CALXComboBoxCtrl window

class CALXComboBoxCtrl : public CComboBox, public CALXCellCtrl
{
// Construction, Destruction
public:
	CALXComboBoxCtrl();
	~CALXComboBoxCtrl();

// Attributes
private:
	CFont m_Font;		// Шрифт 
	BOOL  m_bInitCtrl;	// Определяет проинициализирован ли элемент управления
	CALXComboEditCtrl m_EditCtrl; // Окно редактирования
	int m_nFirstItemHeight;		  // Высота окна
	CString m_strText;			  // Текст (используется для определения того были ли изменения)

// Operations
	BOOL InitCtrl();
	BOOL SubclassEditCtrl();
	inline BOOL IsDroppedDown() { return (SendMessage(CB_GETDROPPEDSTATE) ? TRUE : FALSE); }
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXComboBoxCtrl)
	protected:
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
	virtual void SendLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL IsModifyed();
	virtual BOOL OnValidate();
	virtual BOOL IsActive();

// Функции для работы с буфером обмена
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
	//{{AFX_MSG(CALXComboBoxCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnCloseUp();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSelChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_COMBOBOXCTRL_H_INCLUDED_)
