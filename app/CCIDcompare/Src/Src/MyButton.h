#if !defined(AFX_MYBUTTON_H__F56003FF_3CB5_4AFB_A601_A4097DA61B3E__INCLUDED_)
#define AFX_MYBUTTON_H__F56003FF_3CB5_4AFB_A601_A4097DA61B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

class CMyButton : public CButton
{
// Construction
public:
	CMyButton();

private:
	BOOL    b_Flag;			//按钮状态（false-正常，true-当前）
	BOOL    b_InRect;		//鼠标进入标志
	CString m_strText;		//按钮文字
	COLORREF m_ForeColor;	//文本颜色
	COLORREF m_BkColor;		//背景色
	COLORREF m_LockForeColor;	//锁定按钮的文字颜色
	CRect   m_ButRect;		//按钮尺寸
	CPoint  m_textPt;		//文字坐标（左上角）

	void   NormalButton(CDC *pDC);		//画正常的按钮
	void   PassButton(CDC *pDC);		//画鼠标经过时的按钮
	void   LockButton(CDC *pDC);		//画锁定的按钮

// Attributes
public:
	void  SetForeColor(COLORREF color);	//设置文本颜色
	void  SetBkColor(COLORREF color);	//设置背景颜色

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__F56003FF_3CB5_4AFB_A601_A4097DA61B3E__INCLUDED_)
