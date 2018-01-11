// ALXGridCtrl.h : interface of the CALXGridCtrl class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_GRIDCTRL_H_INCLUDED_)
#define ALX_GRIDCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGrid.h"


#define DECLARE_REGISTER() \
public: \
	static LPCTSTR RegisterClass(); \
	static void UnregisterClass(); \
	static CMapPtrToWord _gxregmap; \
protected: \
	static LRESULT CALLBACK AFX_EXPORT WndProcHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); \
	virtual void PostNcDestroy();
// END DECLARE_REGISTER

#define IMPLEMENT_REGISTER(class_name, _style) \
	CMapPtrToWord class_name::_gxregmap; \
	LPCTSTR class_name::RegisterClass() \
	{ \
		WNDCLASS wndcls;  \
		LPCTSTR szName = _T(#class_name) ; \
		if (::GetClassInfo(AfxGetInstanceHandle(), szName, &wndcls)) \
			return szName; \
		wndcls.style = _style ; \
		wndcls.lpfnWndProc = class_name::WndProcHook ; \
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0 ; \
		wndcls.hInstance = AfxGetInstanceHandle() ; \
		wndcls.hIcon = NULL ; \
		wndcls.hCursor = NULL ; \
		wndcls.hbrBackground = NULL ; \
		wndcls.lpszMenuName = NULL ; \
		wndcls.lpszClassName = szName ; \
		if (!::RegisterClass(&wndcls)) \
			AfxThrowResourceException() ; \
		return szName ;  } \
	LRESULT CALLBACK AFX_EXPORT class_name::WndProcHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) \
	{ \
		class_name * pWnd = new class_name(); \
		pWnd->Attach(hWnd); \
		_gxregmap[pWnd] = 1; \
		::SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)AfxWndProc); \
		return ::CallWindowProc(AfxWndProc, hWnd, msg, wParam, lParam); } \
	void class_name::UnregisterClass() \
	{ \
		::UnregisterClass( _T(#class_name) , AfxGetInstanceHandle() );  } \
	void class_name::PostNcDestroy() \
	{ \
		WORD w; \
		if (_gxregmap.Lookup(this, w)) \
		{ \
			delete this; \
			_gxregmap.RemoveKey(this); \
		} \
	}
// END IMPLEMENT_REGISTER

/////////////////////////////////////////////////////////////////////////////
// CALXGridCtrl

class CALXGridCtrl : public CWnd, public CALXGrid
{
/////////////////////////////////////////////////////////////////////////////
//																			
	DECLARE_DYNCREATE(CALXGridCtrl)
//
/////////////////////////////////////////////////////////////////////////////
public:
// Construction, Destruction
	CALXGridCtrl();
	~CALXGridCtrl();

// Attributes:
public:
	BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
/////////////////////////////////////////////////////////////////////////////
// CView overridable member functions
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXGridCtrl)
	public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
//	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CALXGridCtrl)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdateCellCopy(CCmdUI* pCmdUI);
	afx_msg void OnCellCopy();
	afx_msg void OnUpdateCellCut(CCmdUI* pCmdUI);
	afx_msg void OnCellCut();
	afx_msg void OnCellPaste();
	afx_msg void OnUpdateCellPaste(CCmdUI* pCmdUI);
	afx_msg void OnCellUndo();
	afx_msg void OnUpdateCellUndo(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_GRIDCTRL_H_INCLUDED_)
