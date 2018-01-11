// ALXGridCtrl.cpp : implementation of the CALXGridCtrl class
//
#include "stdafx.h"
#include "ALXGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CALXGridCtrl, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CALXGridCtrl
BEGIN_MESSAGE_MAP(CALXGridCtrl, CWnd)
	//{{AFX_MSG_MAP(CALXGridCtrl)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
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
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CALXGridCtrl construction/destruction

// Конструктор
CALXGridCtrl::CALXGridCtrl()
{
	SetGridWnd(this);
}

// Деструктор
CALXGridCtrl::~CALXGridCtrl()
{

}

////////////////////////////////////////////////////////////////////////////
// CALXGridCtrl drawing

void CALXGridCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	OnGridDraw(&dc);
}
/////////////////////////////////////////////////////////////////////////////
// CALXGridCtrl message handlers

BOOL CALXGridCtrl::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
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
	if(x>m_sizeTotal.cx - m_sizePage.cx + 1)
		x = m_sizeTotal.cx - m_sizePage.cx + 1;
	if(x<0)
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
		y -= m_sizePage.cy+m_CountHeaderRows*m_nHeadRowHeight;
		break;
	case SB_PAGEDOWN:
		y += m_sizePage.cy-m_CountHeaderRows*m_nHeadRowHeight;
		break;
	case SB_THUMBTRACK:
		y = (int)((double)nPos * m_nVScrollFactor);
		break;
	}
	if(y>m_sizeTotal.cy - m_sizePage.cy + 1)
		y = m_sizeTotal.cy - m_sizePage.cy + 1;
	if(y<0)
		y = 0;

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
	{
		UpdateWindow();
	
		CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
		if(pCellCtrl != NULL)
		{
			pCellCtrl->DeactivateCtrl();
			SetFocus();
		}

		if(GetGridRowCount() > 0)
			RedrawFocusRect();
	}

	return bResult;
}
/*
BOOL CALXGridCtrl::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
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
void CALXGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}

void CALXGridCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	UpdateScrollSizes();

	CALXCellCtrl* pCellCtrl = CALXGrid::GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL)
		CALXGrid::SetActiveCell(CALXGrid::GetActiveCol(),CALXGrid::GetActiveRow());
}

void CALXGridCtrl::OnDraw(CDC* pDC) 
{
}

void CALXGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;     // eat it

	// ignore scroll bar msgs from other controls
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}

BOOL CALXGridCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CALXGridCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CALXGrid::OnMouseMove(nFlags, point);
	CWnd::OnMouseMove(nFlags, point);
}

void CALXGridCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CALXGrid::OnLButtonUp(nFlags, point);
	CWnd::OnLButtonUp(nFlags, point);
}

void CALXGridCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CALXGrid::OnLButtonDown(nFlags, point);
	CWnd::OnLButtonDown(nFlags, point);
}

BOOL CALXGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(!CALXGrid::OnSetCursor(pWnd, nHitTest, message))
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	return TRUE;
}

void CALXGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CALXGrid::OnLButtonDblClk(nFlags, point);
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CALXGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	m_bSetFocus = true;

	CALXGrid::OnSetFocus(pOldWnd);
}

void CALXGridCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	m_bSetFocus = false;

	CWnd::OnKillFocus(pNewWnd);
}


BOOL CALXGridCtrl::Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	static CString strGridClass;
	if (strGridClass.IsEmpty())
	{
		strGridClass = AfxRegisterWndClass(CS_DBLCLKS);
	}
	
	BOOL bResult = CreateEx((dwExStyle == NULL ? WS_EX_CLIENTEDGE : dwExStyle),
					strGridClass,
                 	NULL,
     				dwStyle | WS_CHILD | WS_HSCROLL | WS_VSCROLL, 
	             	rect,
					pParentWnd,
                	nID,
                 	NULL);

	if(bResult)
	{
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

	return bResult;
}

BOOL CALXGridCtrl::PreTranslateMessage(MSG* pMsg) 
{	
	if(pMsg->message==WM_KEYDOWN)
	{	
		if(LOWORD(pMsg->wParam) != VK_CONTROL && GetKeyState(VK_CONTROL) < 0)
		{
			if(LOWORD(pMsg->wParam) == VK_TAB)
				pMsg->wParam = MAKEWPARAM(GetKeyState(VK_SHIFT) < 0 ? VK_UP : VK_DOWN,HIWORD(pMsg->wParam));
			return CWnd::PreTranslateMessage(pMsg);
		}
		if(pMsg->hwnd == m_hWnd || LOWORD(pMsg->wParam) == VK_TAB || LOWORD(pMsg->wParam) == VK_RETURN)
		{
			if(CALXGrid::OnKeyDown(LOWORD(pMsg->wParam),LOWORD(pMsg->lParam),HIWORD(pMsg->lParam)))
				return TRUE;
		}
		return CWnd::PreTranslateMessage(pMsg);
	}
	else
		return CWnd::PreTranslateMessage(pMsg);
}

void CALXGridCtrl::OnCellCopy() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Copy();
}

void CALXGridCtrl::OnUpdateCellCut(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanCut());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridCtrl::OnUpdateCellCopy(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanCopy());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridCtrl::OnCellCut() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Cut();
}

void CALXGridCtrl::OnCellPaste() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Paste();
}

void CALXGridCtrl::OnUpdateCellPaste(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanPaste());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridCtrl::OnCellUndo() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->Undo();
}

void CALXGridCtrl::OnUpdateCellUndo(CCmdUI* pCmdUI) 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCmdUI->Enable(pCellCtrl->IsCanUndo());
	else
		pCmdUI->Enable(FALSE);
}

void CALXGridCtrl::OnDestroy() 
{
	CALXGrid::OnDestroy();

	CWnd::OnDestroy();
}

BOOL CALXGridCtrl::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	return CALXGrid::OnMouseWheel(fFlags, zDelta, point);
}
