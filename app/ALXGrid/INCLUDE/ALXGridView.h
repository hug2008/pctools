// ALXGridView.h : interface of the CALXGridView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_GRIDVIEW_H_INCLUDED_)
#define ALX_GRIDVIEW_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGrid.h"
#include "ALXSplitterWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CALXGridCtrl

class CALXGridView : public CView, public CALXGrid
{
/////////////////////////////////////////////////////////////////////////////
//																			
	DECLARE_DYNCREATE(CALXGridView)
//
/////////////////////////////////////////////////////////////////////////////
protected:
// Construction, Destruction
	CALXGridView();
	virtual ~CALXGridView();

/////////////////////////////////////////////////////////////////////////////
// CView overridable member functions
public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALXGridView)
	public:
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual void OnInitialUpdate();
	protected:
//	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CALXGridView)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg long OnSetedFocus(WPARAM wParam, LPARAM lParam);
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
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_GRIDVIEW_H_INCLUDED_)
