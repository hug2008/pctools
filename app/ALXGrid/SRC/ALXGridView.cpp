// ALXGridView.cpp : implementation of the CALXGridView class
//
#include "stdafx.h"
#include "ALXGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CALXGridView, CView)

/////////////////////////////////////////////////////////////////////////////
// CALXGridView
BEGIN_MESSAGE_MAP(CALXGridView, CView)
	//{{AFX_MSG_MAP(CALXGridView)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateCellCopy)
	ON_COMMAND(ID_EDIT_COPY, OnCellCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateCellCut)
	ON_COMMAND(ID_EDIT_CUT, OnCellCut)
	ON_COMMAND(ID_EDIT_PASTE, OnCellPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateCellPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnCellUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateCellUndo)
	ON_WM_DESTROY()
	ON_MESSAGE(GWM_SETFOCUS, OnSetedFocus)
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CALXGridView construction/destruction

// Конструктор
CALXGridView::CALXGridView()
{
	SetGridWnd(this);
}

// Деструктор
CALXGridView::~CALXGridView()
{

}

/////////////////////////////////////////////////////////////////////////////
// CALXGridView drawing

void CALXGridView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	OnGridDraw(&dc);
}

/////////////////////////////////////////////////////////////////////////////
// CALXGridView message handlers

BOOL CALXGridView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// calc new x position
	int x = m_CurScrollPos.x;
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = m_sizeTotal.cx - m_sizePage.cx + 1;
		break;
	case SB_LINEUP:
		x -= m_sizeLine.cx;
		break;
	case SB_LINEDOWN:
		x += m_sizeLine.cx;
		break;
	case SB_PAGEUP:
		x -= m_sizePage.cx;
		break;
	case SB_PAGEDOWN:
		x += m_sizePage.cx;
		break;
	case SB_THUMBTRACK:
		x = (int)((double)nPos * m_nHScrollFactor);
		break;
	}
	if(x > m_sizeTotal.cx - m_sizePage.cx + 1)
		x = m_sizeTotal.cx - m_sizePage.cx + 1;
	if(x < 0)
		x = 0;
		
	// calc new y position
	int y = m_CurScrollPos.y;
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = m_sizeTotal.cy - m_sizePage.cy + 1;
		break;
	case SB_LINEUP:
		y -= m_sizeLine.cy;
		break;
	case SB_LINEDOWN:
		y += m_sizeLine.cy;
		break;
	case SB_PAGEUP:
		y -= m_sizePage.cy+GetHeaderRow()*m_nHeadRowHeight;
		break;
	case SB_PAGEDOWN:
		y += m_sizePage.cy-GetHeaderRow()*m_nHeadRowHeight;
		break;
	case SB_THUMBTRACK:
		y = (int)((double)nPos * m_nVScrollFactor);
		break;
	}
	if(y > m_sizeTotal.cy - m_sizePage.cy + 1)
		y = m_sizeTotal.cy - m_sizePage.cy + 1;
	if(y < 0)
		y = 0;

	BOOL bResult = CALXGrid::OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
	{
		UpdateWindow();
	
		CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
		if(pCellCtrl != NULL)
		{
			pCellCtrl->DeactivateCtrl();
//			SetFocus();
		}

		if(GetGridRowCount() > 0)
			RedrawFocusRect();
	}

	return bResult;
}
/*
BOOL CALXGridView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = m_CurScrollPos.x;
	int xMax = m_sizeTotal.cx;
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = m_CurScrollPos.y;
	int yMax = m_sizeTotal.cy;
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		CALXGridCore::ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
		{
			SetScrollPos(SB_HORZ, (int)((double)x/m_nHScrollFactor));
			m_CurScrollPos.x = x;
		}
		if (y != yOrig)
		{
			SetScrollPos(SB_VERT, (int)((double)y/m_nVScrollFactor));
			m_CurScrollPos.y = y;
		}
	}
	return TRUE;
}
*/
void CALXGridView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}

void CALXGridView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	UpdateScrollSizes();

	CALXCellCtrl* pCellCtrl = CALXGrid::GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL)
		CALXGrid::SetActiveCell(CALXGrid::GetActiveCol(),CALXGrid::GetActiveRow());
}

void CALXGridView::OnDraw(CDC* pDC) 
{
}

void CALXGridView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}

BOOL CALXGridView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CALXGridView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(!CALXGrid::OnKeyDown(nChar,nRepCnt,nFlags))
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CALXGridView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CALXGrid::OnMouseMove(nFlags, point);
	CView::OnMouseMove(nFlags, point);
}

void CALXGridView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CALXGrid::OnLButtonUp(nFlags, point);
	CView::OnLButtonUp(nFlags, point);
}

void CALXGridView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CALXGrid::OnLButtonDown(nFlags, point);
	CView::OnLButtonDown(nFlags, point);
}

BOOL CALXGridView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(!CALXGrid::OnSetCursor(pWnd, nHitTest, message))
		return CView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}

void CALXGridView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CALXGrid::OnLButtonDblClk(nFlags, point);
	CView::OnLButtonDblClk(nFlags, point);
}

void CALXGridView::OnSetFocus(CWnd* pOldWnd) 
{

	CView::OnSetFocus(pOldWnd);

	m_bSetFocus = true;

	PostMessage(GWM_SETFOCUS, (WPARAM)pOldWnd,0);
}

void CALXGridView::OnKillFocus(CWnd* pNewWnd) 
{
	m_bSetFocus = false;

	CView::OnKillFocus(pNewWnd);
}

long CALXGridView::OnSetedFocus(WPARAM wParam, LPARAM lParam) 
{
	CALXGrid::OnSetFocus((CWnd*)wParam);
	return 0;
}

void CALXGridView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	// Получаем указатель на элемент управления ячейки
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl == NULL)
	{
		// Создаем элемент управления ячейки
		pCellCtrl = CreateCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow),GetCellCtrlStyle(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow)));
		if(pCellCtrl != NULL)
		{
			SetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);
			SetDataCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);
		}
	}
	else
		SetDataCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);

	ActivateCtrl(m_nActiveCol, m_nActiveRow);
}

void CALXGridView::OnCellCopy() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Copy();
}

void CALXGridView::OnUpdateCellCut(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanCut());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridView::OnUpdateCellCopy(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanCopy());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridView::OnCellCut() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Cut();
}

void CALXGridView::OnCellPaste() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Paste();
}

void CALXGridView::OnUpdateCellPaste(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanPaste());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridView::OnCellUndo() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Undo();
}

void CALXGridView::OnUpdateCellUndo(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanUndo());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridView::OnDestroy() 
{
	CALXGrid::OnDestroy();

	CView::OnDestroy();	
}

CScrollBar* CALXGridView::GetScrollBarCtrl(int nBar) const
{
	ASSERT(nBar == SB_HORZ || nBar == SB_VERT);
	if (GetStyle() & ((nBar == SB_HORZ) ? WS_HSCROLL : WS_VSCROLL))
	{
		// it has a regular windows style scrollbar (no control)
		return NULL;
	}

	CWnd* pParent = GetParentSplitter(this, TRUE);
	if (pParent == NULL)
		return NULL;            // no splitter

	UINT nID = (UINT)(WORD)::GetDlgCtrlID(m_hWnd);
	if (nID < AFX_IDW_PANE_FIRST || nID > AFX_IDW_PANE_LAST)
		return NULL;            // not a standard pane ID

	// appropriate PANE id - look for sibling (splitter, or just frame)
	UINT nIDScroll;
	if (nBar == SB_HORZ)
		nIDScroll = AFX_IDW_HSCROLL_FIRST + (nID - AFX_IDW_PANE_FIRST) % 16;
	else
		nIDScroll = AFX_IDW_VSCROLL_FIRST + (nID - AFX_IDW_PANE_FIRST) / 16;

	// return shared scroll bars that are immediate children of splitter
	return (CScrollBar*)pParent->GetDlgItem(nIDScroll);
}


BOOL CALXGridView::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	// if the parent is a splitter, it will handle the message
	CALXSplitterWnd* pSplitter = (CALXSplitterWnd*)GetParent();
	if (pSplitter->IsKindOf(RUNTIME_CLASS(CALXSplitterWnd)))
		return pSplitter->OnMouseWheel(this, fFlags, zDelta, point);

	return CALXGrid::OnMouseWheel(fFlags, zDelta, point);
}
