// ALXGrid.cpp : implementation of the CALXGrid class
//
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "ALXGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXGrid

// �����������
CALXGrid::CALXGrid()
	: CALXGridCore()
{
	m_GridRowCount = 0;
	m_bSetFocus = false;
}
// ����������
CALXGrid::~CALXGrid()
{
	RemoveAllCol();
}

// ����������� �������. ���������� ���������� �����
// � ������ ����� (��� ���������� �������� � �����).
// �������� nHeadRow - ������ ����� (�� ��������� -1).
int CALXGrid::GetHeadColCount(int nHeadRow/* = -1*/)
{
	return m_HeaderInfo.m_Cols.GetSize();
}

// ����������� �������. ���������� ������ ������
// � ������ ����� (��� ������ ������� � �����).
// �������� nHeadRow - ������ ����� (�� ��������� -1).
int CALXGrid::GetHeadColWidth(int nCol, int nHeadRow/* = -1*/)
{
	if(nCol >= 0)
	{
		ASSERT(m_HeaderInfo.m_Cols[nCol].m_nWidth >= 0);
		return m_HeaderInfo.m_Cols[nCol].m_nWidth;
	}
	else
		return 0;
}

// ����������� �������. ������������� ������ ������
// � ������ ����� ��� (������ ������� � �����).
// �������� nRow - ������ �����.
void CALXGrid::SetHeadColWidth(int nCol, int nRow, int nNewWidth)
{
	if(nCol >= 0)
		m_HeaderInfo.m_Cols[nCol].m_nWidth = nNewWidth;

	if(m_pGridWnd != NULL)
		m_pGridWnd->Invalidate();
}

// ������� ���������� ����.
void CALXGrid::OnGridDraw(CPaintDC *pDC)
{
	CALXGridCore::OnGridDraw(pDC);
}

// ��������� ������� � �����
int CALXGrid::AddCol(int		nWidth,
					 CString	strHead,
					 UINT		nCellsFormat,
					 UINT		nHeadFormat,
					 UINT		nTypeDrawCtrl,
					 UINT		nStateDrawCtrl,
					 UINT		nID
					 )
{
	return InsertCol(m_HeaderInfo.m_Cols.GetSize(), nWidth,	strHead, nCellsFormat, nHeadFormat,	nTypeDrawCtrl, nStateDrawCtrl, nID);
}

// ��������� ������� � �����
int CALXGrid::AddCol(COL_INFO ColInfo)
{
	int nCol = m_HeaderInfo.m_Cols.GetSize();
	m_HeaderInfo.m_Cols.InsertAt(nCol,ColInfo);
	return nCol;
}

// ��������� ������� � �����
int CALXGrid::InsertCol(int		nCol,
						int		nWidth,
						CString	strHead,
						UINT	nCellsFormat,
						UINT	nHeadFormat,
						UINT	nTypeDrawCtrl,
						UINT	nStateDrawCtrl,
						UINT	nID
						)
{
	ASSERT(nCol >= 0);
	COL_INFO ColInfo;

	ColInfo.m_nWidth				=	nWidth;
	ColInfo.m_HeadInfo.m_strHead	=	strHead;
	ColInfo.m_HeadInfo.m_nFormat	=	nHeadFormat;
	ColInfo.m_CellsInfo.m_nFormat	=	nCellsFormat;
	ColInfo.m_CellsInfo.m_nTypeFrameCtrl	= nTypeDrawCtrl;
	ColInfo.m_CellsInfo.m_nStateFrameCtrl	= nStateDrawCtrl;
	ColInfo.m_CellsInfo.m_nTypeImage		= NULL;

	ColInfo.m_CtrlInfo.m_pColCtrl	= NULL;
	ColInfo.m_CtrlInfo.m_nType		= GA_EDITCTRL;
	ColInfo.m_CtrlInfo.m_dwStyle	= WS_CHILD;
	ColInfo.m_CtrlInfo.m_nID		= nID;

	nCol = min(m_HeaderInfo.m_Cols.GetSize(), nCol);
	m_HeaderInfo.m_Cols.InsertAt(nCol,ColInfo);
	return nCol;
}

// ������� ��� �������
void CALXGrid::RemoveAllCol()
{
	for(int i = 0; i < m_HeaderInfo.m_Cols.GetSize(); i++)
		DestroyColCtrl(i);
	m_HeaderInfo.m_Cols.RemoveAll();
}

// ��������� ������
void CALXGrid::InsertRows(int nRow, int nCount)
{
	ASSERT(nRow >= 0);
	ASSERT(nCount > 0);

	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL && GetGridRowCount() > 0)
		SetDataCtrl(GetActiveCol(),GetActiveRow(), pCellCtrl);
	ScrollRow(nRow + m_CountHeaderRows, nCount*GetGridRowHeight());
	SetGridRowCount(GetGridRowCount() + nCount);
	UpdateScrollSizes();
	if(nRow < GetGridRowCount() - nCount)
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
	else
		SetActiveCell(GetActiveCol(),GetGridRowCount()-1);
}

// ������� ������
void CALXGrid::RemoveRows(int nRow, int nCount)
{
	ASSERT(nRow >= 0);
	ASSERT(nCount > 0);
	ASSERT(nRow + nCount <= GetGridRowCount());

	int nNewActiveRow = min(CALXGridCore::GetActiveRow(), GetGridRowCount() + GetHeadRowCount() - nCount - 1);
		m_nActiveRow = nNewActiveRow;
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL && GetGridRowCount() - nCount > 0)
		SetDataCtrl(GetActiveCol(),GetActiveRow(), pCellCtrl);
	ScrollRow(nRow + m_CountHeaderRows, -(nCount*GetGridRowHeight()));
	SetGridRowCount(GetGridRowCount() - nCount);
	UpdateScrollSizes();
	if(GetGridRowCount() > 0)
	{
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
		InvalidateRow(GetActiveRow());
	}
	else if(pCellCtrl != NULL)
	{
		pCellCtrl->SetModify(FALSE);
		pCellCtrl->DeactivateCtrl();
	}

}

// ������� ���������� ���������� � �������.
COL_INFO CALXGrid::GetColInfo(int nCol)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	return m_HeaderInfo.m_Cols[nCol];
}

// ����������� �������.
// ������� ���������� ���������� � ������ �����
HEADCELL_INFO CALXGrid::GetHeadCellInfo(int nCol, int nRow)
{
	HEADCELL_INFO HeadCellInfo;
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	HeadCellInfo.m_strText = m_HeaderInfo.m_Cols[nCol].m_HeadInfo.m_strHead;
	HeadCellInfo.m_nFormat = m_HeaderInfo.m_Cols[nCol].m_HeadInfo.m_nFormat;

	return HeadCellInfo;
}

// ������� ������������ ������� ����� ������ ����.
BOOL CALXGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	point.x += offsetXY;
	point.y += offsetXY;
	// ���� ���� �������� ������ �������
	if(m_EnableResizeCol)
	{
		m_BeginResizeCol = TRUE;
		// ����������� ����
		m_pGridWnd->SetCapture();

		// ������� �������������� �����
		CDC* pDC = m_pGridWnd->GetDC();
		m_pGridWnd->GetClientRect(&m_rectSaveInv);
		m_rectSaveInv.left  = point.x - offsetXY;
		m_rectSaveInv.right = m_rectSaveInv.left + 1;
		pDC->InvertRect(&m_rectSaveInv);
	}
	else 
	{
		// ���� ���������� ���� ��������� ��� ������� �����
		if(point.x > m_nRowWidth - m_CurScrollPos.x || point.y > GetHeaderRow() * m_nHeadRowHeight + GetGridRowCount() * m_nRowHeight - m_CurScrollPos.y)
			return FALSE;
		// ���� ������ ������ '�������� ��'
		if(point.x <= GetFrozenCol() * m_FrozenColWidth && point.y <= GetHeaderRow() * m_nHeadRowHeight)
		{
			m_bSelAllBtnPressed = TRUE;
			// �������� �������� ����������
			CDC* pDC = m_pGridWnd->GetDC();

			OnUpdateFrozenHeaderRect(pDC, CRect(1,1,m_FrozenColWidth * GetFrozenCol()+1,m_nHeadRowHeight * GetHeaderRow()+1));

			// ����������� �������� ����������
			m_pGridWnd->ReleaseDC(pDC);

			// ����������� ����
			m_pGridWnd->SetCapture();

			OnSelAllButtonDown();

			return TRUE;
		}
		// ���� ������ ������ �����
		else if(point.y <= GetHeaderRow() * m_nHeadRowHeight && point.x >= GetFrozenCol() * m_FrozenColWidth)
		{
			m_nHeaderRowPressed = YToRow(point.y);
			m_nHeaderColPressed = XToCol(point.x + m_CurScrollPos.x, m_nHeaderRowPressed);

			// �������� �������� ����������
			CDC* pDC = m_pGridWnd->GetDC();

			UpdateCell(pDC, m_nHeaderColPressed, m_nHeaderRowPressed);

			// ����������� �������� ����������
			m_pGridWnd->ReleaseDC(pDC);

			// ����������� ����
			m_pGridWnd->SetCapture();

			OnHeadButtonDown(m_nHeaderColPressed - m_CountFrozenCols, m_nHeaderRowPressed);

			return TRUE;
		}
		// ���� ������ ������ ����� ����������� �������
		else if(point.x <= GetFrozenCol() * m_FrozenColWidth && point.y >= GetHeaderRow() * m_nHeadRowHeight)
		{
			m_nFrozenColPressed = XToCol(point.x);
			m_nFrozenRowPressed = YToRow(point.y + m_CurScrollPos.y);

			// �������� �������� ����������
			CDC* pDC = m_pGridWnd->GetDC();

			UpdateCell(pDC, m_nFrozenColPressed, m_nFrozenRowPressed);

			// ����������� �������� ����������
			m_pGridWnd->ReleaseDC(pDC);
	
			// ����������� ����
			m_pGridWnd->SetCapture();

			OnFrozenButtonDown(m_nFrozenColPressed, m_nFrozenRowPressed - m_CountHeaderRows);

			return TRUE;
		}

		// ���� ��������� �����
		if(GetKeyState(VK_SHIFT) < 0)
			CALXGridCore::SetSelectCell(point);
		else if(CALXGridCore::SetActiveCell(point))
			SendLButtonDown(nFlags, point);

		// ����������� ����
		m_pGridWnd->SetCapture();
	}
	return TRUE;
}

// ������� ������������ ������� �������.
BOOL CALXGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_TAB:
		{
			if(GetKeyState(VK_SHIFT) < 0)
				SetActivePrevCol();
			else
				SetActiveNextCol();
			break;
		}
		case VK_HOME:
			SetActiveCol(0);
			break;
		case VK_END:
			SetActiveCol(GetHeadColCount() - 1);
			break;
		case VK_UP:
		{
			if(GetKeyState(VK_SHIFT) < 0)
				SetSelectCell(m_nActiveSelCol, m_nActiveSelRow - 1);
			else
				SetActivePrevRow();
				break;
		}
		case VK_DOWN:
		{
			if(GetKeyState(VK_SHIFT) < 0)
				SetSelectCell(m_nActiveSelCol, m_nActiveSelRow + 1);
			else
				SetActiveNextRow();
				break;
		}
		case VK_LEFT:
		{
			if(GetKeyState(VK_SHIFT) < 0)
				SetSelectCell(m_nActiveSelCol - 1, m_nActiveSelRow);
			else
				SetActivePrevCol();
				break;
		}
		case VK_RIGHT:
		{
			if(GetKeyState(VK_SHIFT) < 0)
				SetSelectCell(m_nActiveSelCol + 1, m_nActiveSelRow);
			else
				SetActiveNextCol();
				break;
		}
		case VK_PRIOR:
		{
			if(GetKeyState(VK_CONTROL) < 0)
				SetActiveRow(0);
			else
				SetActiveRow(max(0,GetActiveRow()-(m_sizePage.cy/m_nRowHeight-GetHeaderRow())));
			break;
		}
		case VK_NEXT:
		{
			if(GetKeyState(VK_CONTROL) < 0)
				SetActiveRow(GetGridRowCount() - 1);
			else
				SetActiveRow(min(GetGridRowCount() - 1,GetActiveRow()+(m_sizePage.cy/m_nRowHeight-GetHeaderRow())));
			break;
		}
		case VK_RETURN:
		{
			if(!SetActiveNextRow())
				SetActiveCell(GetActiveCol(),GetActiveRow());
			break;
		}
		case VK_CONTROL: case VK_SHIFT:
			break;
		default:
		{
			SetActiveCell(GetActiveCol(),GetActiveRow());
			return FALSE;
		}

	}

	return TRUE;
}

// ������� ������������ ������� ����� ������ ����.
BOOL CALXGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	point.x += offsetXY;
	point.y += offsetXY;
	// ���� ��� ������� ��������� ������ �������
	if(m_BeginResizeCol)
	{
		m_BeginResizeCol = FALSE;

		EndResizeCol(point.x);
		// ����������� ����
		ReleaseCapture();

		// ������� �������������� �����
		CDC* pDC = m_pGridWnd->GetDC();
		pDC->InvertRect(&m_rectSaveInv);
	}

	if(m_bSelAllBtnPressed)
	{
		m_bSelAllBtnPressed = FALSE;

		// �������� �������� ����������
		CDC* pDC = m_pGridWnd->GetDC();

		OnUpdateFrozenHeaderRect(pDC, CRect(1,1,m_FrozenColWidth * m_CountFrozenCols+1,m_nHeadRowHeight * m_CountHeaderRows+1));

		// ����������� �������� ����������
		m_pGridWnd->ReleaseDC(pDC);

		// ����������� ����
		ReleaseCapture();
	}
	if(m_nHeaderColPressed >= 0)
	{
		int nCol = m_nHeaderColPressed;
		int nRow = m_nHeaderRowPressed;
		m_nHeaderColPressed = -1;
		m_nHeaderRowPressed = -1;
		// �������� �������� ����������
		CDC* pDC = m_pGridWnd->GetDC();

		UpdateCell(pDC, nCol, nRow);

		// ����������� �������� ����������
		m_pGridWnd->ReleaseDC(pDC);
		// ����������� ����
		ReleaseCapture();
	
		OnHeadButtonUp(nCol - m_CountFrozenCols, nRow);
	}
	if(m_nFrozenRowPressed >= 0)
	{
		int nCol = m_nFrozenColPressed;
		int nRow = m_nFrozenRowPressed;
		m_nFrozenColPressed = -1;
		m_nFrozenRowPressed = -1;
		// �������� �������� ����������
		CDC* pDC = m_pGridWnd->GetDC();

		UpdateCell(pDC, nCol, nRow);

		// ����������� �������� ����������
		m_pGridWnd->ReleaseDC(pDC);
		// ����������� ����
		ReleaseCapture();

		OnFrozenButtonUp(nCol, nRow - m_CountHeaderRows);
	}

	// ����������� ����
	ReleaseCapture();

	return TRUE;
}

// ������� ������������ ����������� ����.
BOOL CALXGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	point.x += offsetXY;
	point.y += offsetXY;
	// �������� �������� ����������
	CDC* pDC = m_pGridWnd->GetDC();

	// ���� ���� ������� ��������� ������� �������
	if(m_BeginResizeCol)
	{
		CRect rect;
		m_pGridWnd->GetClientRect(&rect);
		// ���� ����� ������ ���� �� ������ /* ��� ����� ����� �� ������� ����� ����� ��� �� 5 ��������*/
		if(!(nFlags & MK_LBUTTON) || !m_EnableResizeCol/* || point.y < - 4 || point.y > rect.bottom + offsetXY */)
		{ 
			m_BeginResizeCol = FALSE;
			EndResizeCol(point.x);
			// ����������� ����
			ReleaseCapture();
			// ������� �������������� �����
			pDC->InvertRect(&m_rectSaveInv);
		}
		else
		{
			// ������� �������������� �����
			rect.left  = point.x - offsetXY;
			rect.right = rect.left + 1;
			pDC->InvertRect(&m_rectSaveInv);
			pDC->InvertRect(&rect);
			m_rectSaveInv = rect;
		}
	}
	// ���� ���� ������ ������ '�������� ��' � ���� ������������� �� ������� ������
	else if(m_bSelAllBtnPressed && (point.x < -3 || point.y < -3 || point.x > GetFrozenCol() * m_FrozenColWidth || point.y > GetHeaderRow() * m_nHeadRowHeight))
	{
		m_bSelAllBtnPressed = FALSE;
		// �������� �������� ����������
		CDC* pDC = m_pGridWnd->GetDC();

		OnUpdateFrozenHeaderRect(pDC, CRect(1,1,m_FrozenColWidth * GetFrozenCol()+1,m_nHeadRowHeight * GetHeaderRow()+1));

		// ����������� �������� ����������
		m_pGridWnd->ReleaseDC(pDC);

		// ����������� ����
		ReleaseCapture();

		return TRUE;
	}
	// ���� ���� ������ ������ ����� � ���� ������������� �� ������� ������
	else if(m_nHeaderColPressed >= 0 && (XToCol(point.x + m_CurScrollPos.x,YToRow(point.y)) != m_nHeaderColPressed || YToRow(point.y) != m_nHeaderRowPressed))
	{
		int nCol = m_nHeaderColPressed;
		int nRow = m_nHeaderRowPressed;
		m_nHeaderColPressed = -1;
		m_nHeaderRowPressed = -1;

		UpdateCell(pDC, nCol, nRow);

		// ����������� ����
		ReleaseCapture();
	}
	// ���� ���� ������ ����������� ������� � ���� ������������� �� ������� ������
	else if(m_nFrozenColPressed >= 0 && (XToCol(point.x) != m_nFrozenColPressed || YToRow(point.y  + m_CurScrollPos.y) != m_nFrozenRowPressed))
	{
		int nCol = m_nFrozenColPressed;
		int nRow = m_nFrozenRowPressed;
		m_nFrozenColPressed = -1;
		m_nFrozenRowPressed = -1;

		UpdateCell(pDC, nCol, nRow);

		// ����������� ����
		ReleaseCapture();
	}
	else if((nFlags & MK_LBUTTON) && m_nHeaderColPressed < 0 && m_nFrozenColPressed < 0)
	{
		int nCol = XToCol(max(0,point.x + m_CurScrollPos.x));
		int nRow = YToRow(max(0,point.y + m_CurScrollPos.y));

		if(nCol >= m_CountFrozenCols && nRow >= m_CountHeaderRows)
		{
			if(nCol != m_nActiveSelCol || nRow != m_nActiveSelRow)
				SetSelectCell(nCol, nRow);
		}
	}

	m_pGridWnd->ReleaseDC(pDC);
	return TRUE;
}

// ������� �������� �������� �����.
BOOL CALXGrid::OnMouseWheel(UINT fFlags, short zDelta, CPoint point, BOOL bSetFocus /*= TRUE */)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(), GetActiveRow());
	if(pCellCtrl != NULL)
		pCellCtrl->DeactivateCtrl();

	if(bSetFocus)
		m_pGridWnd->SetFocus();

	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(fFlags);

	// if we have a vertical scroll bar, the wheel scrolls that
	// if we have _only_ a horizontal scroll bar, the wheel scrolls that
	// otherwise, don't do any work at all

	DWORD dwStyle = m_pGridWnd->GetStyle();
	CScrollBar* pBar = m_pGridWnd->GetScrollBarCtrl(SB_VERT);
	BOOL bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_VSCROLL);

	pBar = m_pGridWnd->GetScrollBarCtrl(SB_HORZ);
	BOOL bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_HSCROLL);

	if (!bHasVertBar && !bHasHorzBar)
		return FALSE;

	BOOL bResult = FALSE;
	UINT uWheelScrollLines;// = _AfxGetMouseScrollLines();
	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uWheelScrollLines, 0);
	int nToScroll;
	int nDisplacement;

	if (bHasVertBar)
	{
		nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
		if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_sizePage.cy;//m_pageDev.cy;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_sizeLine.cy;//m_lineDev.cy;
			nDisplacement = min(nDisplacement, m_sizePage.cy/*m_pageDev.cy*/);
			nDisplacement = min(nDisplacement, m_sizeTotal.cy - m_sizePage.cy - m_CurScrollPos.y + 1);
		}

		bResult = OnScrollBy(CSize(0, nDisplacement), TRUE);
	}
	else if (bHasHorzBar)
	{
		nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
		if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_sizePage.cx;//m_pageDev.cx;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_sizeLine.cx;//m_lineDev.cx;
			nDisplacement = min(nDisplacement, m_sizePage.cx/*m_pageDev.cx*/);
			nDisplacement = min(nDisplacement, m_sizeTotal.cx - m_sizePage.cx - m_CurScrollPos.x + 1);
		}
		bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);
	}

//	if (bResult)
//		UpdateWindow();

	return bResult;
}

// ������� ������������ ��������� �� ��������� ������� ����.
BOOL CALXGrid::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message)
{
	m_EnableResizeCol = FALSE;
	if(nHitTest != HTCLIENT)
		return FALSE;
	else
	{
		CPoint point;
		::GetCursorPos(&point);
		point.x += offsetXY;
		point.y += offsetXY;
		m_pGridWnd->ScreenToClient(&point); // client coordinates of mouse position
		if(point.x >= GetFrozenCol() * m_FrozenColWidth - 2 &&  point.y <= GetHeaderRow() * m_nHeadRowHeight)
		{
			if(point.x + m_CurScrollPos.x > m_nRowWidth + 4)
				return FALSE;

			m_ResizeRow = point.y / GetHeadRowHeight();
			// ���� ������ ����� ���������
			if(m_BeginResizeCol || IsCursorBetweenCols(point.x, m_ResizeCol, m_ResizeRow))
			{
				m_EnableResizeCol = TRUE;
				if(m_curHResize != NULL)
					::SetCursor(m_curHResize);
				else
				{
					if(m_curHResize = ::LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_CURSOR_HRESIZE)))
						::SetCursor(m_curHResize);
					else
					{
						m_curHResize = NULL;
						::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
					}
				}
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}
	return TRUE;
}

// ������� ������������ ��������� �� ��������� ������.
void CALXGrid::OnSetFocus(CWnd* pOldWnd)
{
	if(!m_bSetFocus)
		return;
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL)
	{
//		if(pOldWnd != NULL && pOldWnd != (CWnd*)pCellCtrl)
//			SetActiveCell(GetActiveCol(),GetActiveRow());
		if(pOldWnd != NULL && pOldWnd != (CWnd*)pCellCtrl)
		{
			int nLeftCol, nRightCol, nTopRow, nButtonRow;
			GetSelectRange(nLeftCol, nRightCol, nTopRow, nButtonRow);
			if(nLeftCol == nRightCol && nTopRow == nButtonRow)
				SetActiveCell(GetActiveCol(),GetActiveRow());
		}
	}
	else
		SetSelectCell(m_nActiveSelCol,m_nActiveSelRow);
}


// ������� ������������ ������� ������� ����� ������ ����.
BOOL CALXGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	point.x += offsetXY;
	point.y += offsetXY;
	// ���� ���������� ���� ��������� ��� ������� �����
	if(point.x > m_nRowWidth - m_CurScrollPos.x || point.y > GetHeaderRow() * m_nHeadRowHeight + GetGridRowCount() * m_nRowHeight - m_CurScrollPos.y)
		return FALSE;
	// ���� ������ ������ �����
	if(point.y <= GetHeaderRow() * m_nHeadRowHeight && point.x >= GetFrozenCol() * m_FrozenColWidth)
	{
		m_nHeaderRowPressed = YToRow(point.y);
		m_nHeaderColPressed = XToCol(point.x + m_CurScrollPos.x, m_nHeaderRowPressed);

		// �������� �������� ����������
		CDC* pDC = m_pGridWnd->GetDC();

		UpdateCell(pDC, m_nHeaderColPressed, m_nHeaderRowPressed);

		// ����������� �������� ����������
		m_pGridWnd->ReleaseDC(pDC);

		// ����������� ����
		m_pGridWnd->SetCapture();

		OnHeadButtonDblClk(m_nHeaderColPressed - m_CountFrozenCols, m_nHeaderRowPressed);

		return TRUE;
	}

	// ���� ������ ������ ����� ����������� �������
	if(point.x <= GetFrozenCol() * m_FrozenColWidth  && point.y > GetHeaderRow() * m_nHeadRowHeight)
	{
		m_nFrozenColPressed = XToCol(point.x);
		m_nFrozenRowPressed = YToRow(point.y + m_CurScrollPos.x);

		// �������� �������� ����������
		CDC* pDC = m_pGridWnd->GetDC();

		UpdateCell(pDC, m_nFrozenColPressed, m_nFrozenRowPressed);

		// ����������� �������� ����������
		m_pGridWnd->ReleaseDC(pDC);

		// ����������� ����
		m_pGridWnd->SetCapture();

		OnFrozenButtonDblClk(m_nFrozenColPressed, m_nFrozenRowPressed - m_CountHeaderRows);

		return TRUE;
	}

	CALXGridCore::SetActiveCell(point);

	return TRUE;
}

// ���� ������������������.
void CALXGrid::OnDestroy()
{
	SaveModifyed();
}

// ����������� �������.
// ������ ������ '�������� ��'.
void CALXGrid::OnSelAllButtonDown()
{
	if(!SaveModifyed())
		return;
	if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
		// ������������� �������� ���������� �����
		CALXGridCore::SetSelectRange(m_CountFrozenCols, GetHeadColCount() - 1 + m_CountFrozenCols, m_CountHeaderRows, GetGridRowCount() - 1 + m_CountHeaderRows);
	return;
}

// ����������� �������.
// ������ ������ �����.
void CALXGrid::OnHeadButtonDown(int nHeadCol, int nHeadRow)
{
	if(!SaveModifyed())
		return;

	if(GetHeadRowCount() > 1)
		return;

	if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
	{
		int nFirstCol, nLastCol;
		nFirstCol = nHeadCol;
		nLastCol = nHeadCol;
		if(GetKeyState(VK_SHIFT) < 0)
		{
			int nLeftCol, nRightCol, nTopRow, nButtonRow;
			GetSelectRange(nLeftCol, nRightCol, nTopRow, nButtonRow);
			if(nLeftCol != nRightCol || nTopRow != nButtonRow)
			{
				nFirstCol = min(nLeftCol, nHeadCol);
				nLastCol = max(nRightCol, nHeadCol);
			}
		}

		int nActRow = GetActiveRow();

		CALXCellCtrl* pCellCtrl = GetCellCtrl(nHeadCol,nActRow);
		if(pCellCtrl != NULL)
			pCellCtrl->SetData(GetCellData(nHeadCol,nActRow));

		CALXGridCore::SetSelectRange(nFirstCol + m_CountFrozenCols, nLastCol + m_CountFrozenCols, m_CountHeaderRows, GetGridRowCount() - 1 + m_CountHeaderRows);
//		SetSelectRange(nFirstCol, nLastCol, 0, GetGridRowCount() - 1);
	}
	else
		SetActiveCol(nHeadCol);

	return;
}

// ����������� �������.
// �������� ������ �����.
void CALXGrid::OnHeadButtonUp(int nHeadCol, int nHeadRow)
{
	return;
}

// ����������� �������.
// ������ ������ ����������� �������.
void CALXGrid::OnFrozenButtonDown(int nFrozenCol, int nFrozenRow)
{
	if(!SaveModifyed())
		return;

	if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
	{
		int nFirstRow, nLastRow;
		nFirstRow = nFrozenRow;
		nLastRow = nFrozenRow;
		if(GetKeyState(VK_SHIFT) < 0)
		{
			int nLeftCol, nRightCol, nTopRow, nButtonRow;
			GetSelectRange(nLeftCol, nRightCol, nTopRow, nButtonRow);
			if(nLeftCol != nRightCol || nTopRow != nButtonRow)
			{
				nFirstRow = min(nTopRow, nFrozenRow);
				nLastRow = max(nButtonRow, nFrozenRow);
			}
		}

		int nActCol = GetActiveCol();
		int nActRow = GetActiveRow();

		CALXCellCtrl* pCellCtrl = GetCellCtrl(nActCol,nFrozenRow);
		if(pCellCtrl != NULL && (nActRow < nFirstRow || nActRow > nLastRow))
			pCellCtrl->SetData(GetCellData(nActCol,nFrozenRow));

		CALXGridCore::SetSelectRange(m_CountFrozenCols, GetHeadColCount() - 1 + m_CountFrozenCols, nFirstRow + m_CountHeaderRows, nLastRow + m_CountHeaderRows);
//		SetSelectRange(0, GetHeadColCount() - 1, nFirstRow, nLastRow);

		CDC* pDC = m_pGridWnd->GetDC();

		for(int i = 0; i < m_CountFrozenCols; i++)
			UpdateCell(pDC, i, nActRow + m_CountHeaderRows);

		nActRow = GetActiveRow();

		for(i = 0; i < m_CountFrozenCols; i++)
			UpdateCell(pDC, i, nActRow + m_CountHeaderRows);

		m_pGridWnd->ReleaseDC(pDC);
	}
	else
		SetActiveRow(nFrozenRow);

	return;
}

// ����������� �������.
// ������� ������� ������ ����������� �������.
void CALXGrid::OnFrozenButtonDblClk(int nFrozenCol, int nFrozenRow)
{
	OnFrozenButtonDown(nFrozenCol, nFrozenRow);
	return;
}

// ����������� �������.
// �������� ������ ����������� �������.
void CALXGrid::OnFrozenButtonUp(int nFrozenCol, int nFrozenRow)
{
	return;
}

// ����������� �������.
// ������� ������� ������ �����.
void CALXGrid::OnHeadButtonDblClk(int nHeadCol, int nHeadRow)
{
	OnHeadButtonDown(nHeadCol, nHeadRow);
	return;
}

// ����������� �������.
// ������� ���������� ��������� �� ������� ���������� �������.
CALXCellCtrl* CALXGrid::GetCellCtrl(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	return m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl;
}

// ����������� �������.
// ������� ���������� ID �������a ���������� �������.
UINT CALXGrid::GetCellCtrlID(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	return m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nID;
}

// ����������� �������.
// ������� ������������� ������� ���������� ��� ������.
void CALXGrid::SetCellCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl)
{
	m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl = pCellCtrl;
}

// ����������� �������. ��� ������ ���������� ������ ������.
// ��� ������� ������ ��������������� � �������� ������
// ����������� �� CALXGridView ��� CALXGridCtrl.
CELL_DATA CALXGrid::GetCellData(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	CELL_DATA CellData;
	CellData.m_dwTag  = -1;
	return CellData;
}

// ����������� �������.
// ������� ���������� ���������� � ����� ������.
CELL_INFO CALXGrid::GetCellInfo(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	CELL_INFO CellInfo;
	CellInfo.m_nFormat		= m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_nFormat;
	CellInfo.m_nTypeCtrl	= m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_nTypeFrameCtrl;
	CellInfo.m_nState		= m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_nStateFrameCtrl;
	CellInfo.m_nTypeImage	= m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_nTypeImage;
	return CellInfo;
}

// ����������� �������.
// ������� ���������� ���������� �
// ����� �������� ���������� �������.
DWORD CALXGrid::GetCellCtrlStyle(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	return m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_dwStyle;
}

// ����������� �������. ��� ������ ��������� ������ � ������.
// ��� ������� ������ ��������������� � �������� ������
// ����������� �� CALXGridView ��� CALXGridCtrl.
BOOL CALXGrid::OnSaveCellData(int nCol, int nRow)
{
	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if(pParentWnd != NULL)
		pParentWnd->SendMessage(GWM_SAVECHANGED,nCol,nRow);

	OnSavedCellData(nCol, nRow);

	return TRUE;
}

// ����������� �������. ��� ������ ���������� ����� ����������
// ������ � ������. ��� ������� ������ ��������������� � ��������
// ������ ����������� �� CALXGridView ��� CALXGridCtrl.
void CALXGrid::OnSavedCellData(int nCol, int nRow)
{
	return;
}

// ����������� �������.
// ������� ������� ������� ���������� ��� ������.
CALXCellCtrl* CALXGrid::CreateCellCtrl(int nCol, int nRow, DWORD dwStyle, UINT nID)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());

	if(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl != NULL)
		return m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl;

	nID = (nID == NULL ? m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nID : nID);
	m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nID = nID;

	CALXCellCtrl* pCellCtrl = NULL;

	switch(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nType)
	{
/*	case GA_CELLCTRL:
		{
			CALXCellCtrl* pCellCtrl = new CALXCellCtrl;
			break;
		}
*/	case GA_EDITCTRL:
		{
			CALXEditCtrl* pEditCtrl = new CALXEditCtrl;
			pCellCtrl = pEditCtrl;
			break;
		}
	case GA_BUTTONCTRL:
		{
			CALXButtonCtrl* pButnCtrl = new CALXButtonCtrl;
			pCellCtrl = pButnCtrl;
			break;
		}
	case GA_COMBOBOXCTRL:
		{
			CALXComboBoxCtrl* pComboCtrl = new CALXComboBoxCtrl;
			pCellCtrl = pComboCtrl;
			break;
		}
	}

	if(pCellCtrl != NULL)
	{
		pCellCtrl->CreateCtrl(dwStyle,m_pGridWnd,nID);
		pCellCtrl->EnableCtrl(FALSE);
		pCellCtrl->SetFocusRect(&m_MetricsCells.m_FocusRect);
		pCellCtrl->SetFontCtrl(&m_Font);
	}

	return pCellCtrl;
}

// ������� ���������� ��� � �����
// �������� ���������� ��� ������ �������.
BOOL CALXGrid::DefineColCtrl(int nCol, BYTE CtrlType, DWORD dwStyle, UINT nID)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	if(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl != NULL)
		return FALSE;


	m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nType = CtrlType;
	m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_dwStyle = dwStyle;
	if(nID == NULL)
		m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nID = (m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nID == NULL ? IDC_CELLCTRL+nCol : m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nID);
	else
		m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nID = nID;

	return TRUE;
}

// ������� ��������� ������� ���������� ��� ������ �������.
BOOL CALXGrid::DestroyColCtrl(int nCol)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	if(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl != NULL)
	{
		m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl->DestroyCtrl();
		switch(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nType)
		{
/*			case GA_CELLCTRL:
				delete (CALXCellCtrl*)m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl;
				break;
*/			case GA_EDITCTRL:
				delete (CALXEditCtrl*)m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl;
				m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl = NULL;
				break;
			case GA_BUTTONCTRL:
				delete (CALXButtonCtrl*)m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl;
				m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl = NULL;
				break;
			case GA_COMBOBOXCTRL:
				delete (CALXComboBoxCtrl*)m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl;
				m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl = NULL;
				break;
		}
		ASSERT(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_pColCtrl == NULL);
	}
	return TRUE;
}

// ������� ���������� ��� ����������� ��� ������ �������.
BOOL CALXGrid::DefineImage(int nCol, int cx , int cy, UINT nTypeImage)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_nTypeImage = nTypeImage;
	m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_sizeImage.cx = cx;
	m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_sizeImage.cy = cy;
	return TRUE;
}

// ������� ���������� "������" ���� 
// ��� ������ ��������� ��� �����������.
BOOL CALXGrid::IsImage(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	if(m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_nTypeImage != NULL)
		return TRUE;
	return FALSE;
}

// ������� ���������� ��� ����������� ��� ������ ������.
UINT CALXGrid::GetTypeImage(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	return m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_nTypeImage;
}

// ������� ���������� ������ ����������� ��� ������ ������.
BOOL CALXGrid::GetSizeImage(int nCol, int nRow, CSize& sz)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	sz = m_HeaderInfo.m_Cols[nCol].m_CellsInfo.m_sizeImage;
	return TRUE;
}

// ������� ���������� ��������� �� ����������� ��� ������ ������.
LPARAM CALXGrid::GetImage(int nCol, int nRow, BOOL bSelected)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	return NULL;
}

// ������� ���������� ��������� �� ���������
// ���������� ��������� ����������� ��� ������ ������.
void CALXGrid::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
}

// ����������� �������.
// ���������� ���������� ����� � ����� ��� ����� ����� �����.
int CALXGrid::GetGridRowCount()
{
	return m_GridRowCount;
}

// ������� ������ ���������� ����� � ����� ��� ����� ����� �����.
void CALXGrid::SetGridRowCount(int NewGridRowCount, BOOL bSendMsg /* = TRUE*/)
{
	if(m_GridRowCount == NewGridRowCount)
		return;

	m_GridRowCount = NewGridRowCount;

	if(!bSendMsg)
		return;

	if(m_pGridWnd != NULL && ::IsWindow(m_pGridWnd->m_hWnd))
	{
		CWnd* pParentWnd = m_pGridWnd->GetParent();
		if(pParentWnd != NULL && ::IsWindow(m_pGridWnd->m_hWnd))
			pParentWnd->PostMessage(GWM_GRIDROWCOUNTCHANGED,(WPARAM)m_pGridWnd,m_GridRowCount);
	}
}

// ������� ���������� ������ ������ �����.
int CALXGrid::GetGridRowHeight()
{
	return m_nRowHeight;
}

// ������������� ���������� �������� � ������
void CALXGrid::SetRowLineCount(int nNewLineCount)
{
	ASSERT(nNewLineCount > 0 && nNewLineCount <= 255);

	m_nRowLineCount = nNewLineCount;
}

// ������� ���������� ���������� ����� � �����.
int CALXGrid::GetHeadRowCount()
{
	return m_CountHeaderRows;
}

// ������� ������ ���������� ����� � �����.
void CALXGrid::SetHeadRowCount(int NewRowCount, BOOL bRecalcMetrics/* = TRUE*/)
{
	ASSERT(NewRowCount >= 0);
	m_nActiveRow = m_nActiveRow + NewRowCount - m_CountHeaderRows;
	m_CountHeaderRows = NewRowCount;
	if(bRecalcMetrics)
	{
		UpdateScrollSizes();
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
	}
}

// ������� ���������� ���������� ����������� ����� ��������.
int CALXGrid::GetFrozenColCount()
{
	return m_CountFrozenCols;
}

// ������� ������ ���������� ����������� ����� ��������.
void CALXGrid::SetFrozenColCount(int NewColCount, BOOL bRecalcMetrics/* = TRUE*/)
{
	ASSERT(NewColCount >= 0);
	m_nActiveCol = m_nActiveCol + NewColCount - m_CountFrozenCols;
	m_CountFrozenCols = NewColCount;
	if(bRecalcMetrics)
	{
		UpdateScrollSizes();
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
	}
}

// ������� ������ ������ ������ �����.
void CALXGrid::SetHeadRowHeight(int NewHeadRowHeight, BOOL bRecalcMetrics/* = TRUE*/)
{
	ASSERT(m_pGridWnd->GetStyle() & AGS_CUSTOMHEAD_HEIGHT);
	m_nHeadRowHeight = NewHeadRowHeight;
	if(bRecalcMetrics)
	{
		UpdateScrollSizes();
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
	}
}

// ������� ���������� ������ ������ �����.
int CALXGrid::GetHeadRowHeight()
{
	return m_nHeadRowHeight;
}

// ������� ������ ������ ����������� ��������.
void CALXGrid::SetFrozenColWidth(int NewFrozenColWidth, BOOL bRecalcMetrics/* = TRUE*/)
{
	m_FrozenColWidth = NewFrozenColWidth;
	if(bRecalcMetrics)
	{
		UpdateScrollSizes();
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
	}
}

// ������� ���������� ������ ����������� ��������.
int CALXGrid::GetFrozenColWidth()
{
	return m_FrozenColWidth;
}

// ������� ������������� ����� �����.
void CALXGrid::SetHeadFont(CFont* pFont, BOOL bRecalcMetrics/* = TRUE*/)
{
	m_pHeadFont = pFont;
	if(bRecalcMetrics)
	{
		UpdateScrollSizes();
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
	}
}

// ������� ���������� ������� ����� �����.
CFont* CALXGrid::GetHeadFont()
{
	return m_pHeadFont;
}

// ������� ������������� ����� �����.
void CALXGrid::SetGridFont(CFont* pFont, BOOL bRecalcMetrics/* = TRUE*/)
{
	LOGFONT	 logfont;
	memset(&logfont, 0, sizeof(logfont));
	pFont->GetLogFont(&logfont);
	m_Font.DeleteObject();
	if(!m_Font.CreateFontIndirect(&logfont))
	{
		TRACE0("Failed to create grid font\n");
		return;
	}
	for(int i = 0; i < m_HeaderInfo.m_Cols.GetSize(); i++)
	{
		if(m_HeaderInfo.m_Cols[i].m_CtrlInfo.m_pColCtrl != NULL)
			m_HeaderInfo.m_Cols[i].m_CtrlInfo.m_pColCtrl->SetFontCtrl(&m_Font);
	}

	if(bRecalcMetrics)
	{
		UpdateScrollSizes();
		ActivateCtrl(CALXGridCore::GetActiveCol(), CALXGridCore::GetActiveRow());
	}
}

// ������� ���������� ������� ����� �����.
CFont* CALXGrid::GetGridFont()
{
	return &m_Font;
}

// ������� ������������� ������ �������������� �������.
void CALXGrid::SetExtAreaSize(CSize& sizeNewExtArea, BOOL bRecalcMetrics/* = TRUE*/)
{
	m_sizeExtArea = sizeNewExtArea;
	if(bRecalcMetrics)
		UpdateScrollSizes();
}

// ������� ���������� ������ �������������� �������.
CSize CALXGrid::GetExtAreaSize()
{
	return m_sizeExtArea;
}

// ������� �������� ����, ��� ������� ������ ����� ������������.
void CALXGrid::InvalidateCell(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_HeaderInfo.m_Cols.GetSize());
	ASSERT(nRow >= 0);
	ASSERT(nRow < GetGridRowCount());
	
	CALXGridCore::InvalidateCell(GetFrozenColCount() + nCol, GetHeadRowCount() + nRow);
}

// ������� �������� ����, ��� ������� ������ ����� ������������.
void CALXGrid::InvalidateFrozenCell(int nCol, int nRow)
{
	ASSERT(nCol >= 0);
	ASSERT(nCol < m_CountFrozenCols);
	ASSERT(nRow >= 0);
	ASSERT(nRow < GetGridRowCount());
	
	CALXGridCore::InvalidateCell(nCol, GetHeadRowCount() + nRow);
}

// ������� �������� ����, ��� ������� ������ ����� ������������.
void CALXGrid::InvalidateRow(int nRow)
{
	ASSERT(nRow >= 0);
	ASSERT(nRow < GetGridRowCount());
	
	CALXGridCore::InvalidateRow(GetHeadRowCount() + nRow);
}

void CALXGrid::ScrollAndSetActCol(int HPos, int nCol, BOOL RedrawCell /*= TRUE*/)
{
	CALXGridCore::ScrollAndSetActCol(HPos, nCol, RedrawCell);
}

void CALXGrid::ScrollAndSetActRow(int VPos, int nRow, BOOL RedrawCell /*= TRUE*/)
{
	CALXGridCore::ScrollAndSetActRow(VPos, nRow, RedrawCell);
}

void CALXGrid::ScrollAndSetActCell(int nCol,int nRow, BOOL RedrawCell /*= TRUE*/)
{
	CALXGridCore::ScrollAndSetActCell(nCol, nRow, RedrawCell);
}

void CALXGrid::ScrollToCol(int HPos, int nCol)
{
	CALXGridCore::ScrollToCol(HPos, nCol);
}

void CALXGrid::ScrollToRow(int VPos, int nRow)
{
	CALXGridCore::ScrollAndSetActRow(VPos, nRow);
}

CPoint CALXGrid::GetCurScrollPos()
{
	return CALXGridCore::m_CurScrollPos;
}

// ������� ��������� ����� ����������� �������
// ���� ����� � ��������� �������� ���������.
void CALXGrid::UpdateScrollSizes()
{
	CALXGridCore::UpdateScrollSizes();
}

// ������� ��������� ��������� ���������
// � �������� �������� ���������� �������
// � �������������� ���� ������� ����������.
BOOL CALXGrid::SaveModifyed(BOOL bSkipErrors /*= TRUE*/)
{
	return CALXGridCore::SaveModifyed(bSkipErrors);
}

// ������� ������������� ������ �����,
// ������� ������� ���������� �� �������� ����������
void CALXGrid::SetSortCol(int nHeadCol, int nHeadRow, ALXSortOrder nSortOrder)
{
	m_nSortOrder = nSortOrder;
	m_nSortHeadCol = nHeadCol;
	m_nSortHeadRow = nHeadRow;
	return;
}

// ������� ������������ ��� ����������� �������,
// �� ������� ������������� ������.
void CALXGrid::GetSortCol(int& nHeadCol, int& nHeadRow, ALXSortOrder& nSortOrder)
{
	nSortOrder = m_nSortOrder;
	nHeadCol = m_nSortHeadCol;
	nHeadRow = m_nSortHeadRow;
	return;
}


void CALXGrid::SetSelectRange(int nLeftCol, int nRightCol, int nTopRow, int nButtonRow)
{
	ASSERT(nLeftCol <= nRightCol && nTopRow <= nButtonRow);
	ASSERT(nRightCol < m_HeaderInfo.m_Cols.GetSize());
	ASSERT(nButtonRow < m_GridRowCount);

	CALXGridCore::SetSelectRange(nLeftCol + m_CountFrozenCols, nRightCol + m_CountFrozenCols, nTopRow + m_CountHeaderRows, nButtonRow + m_CountHeaderRows);
	
	// ���� ���� ���������� ������� ����������
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL)
		CALXGrid::SetActiveCell(CALXGrid::GetActiveCol(),CALXGrid::GetActiveRow());
}

void CALXGrid::GetSelectRange(int& nLeftCol, int& nRightCol, int& nTopRow, int& nButtonRow)
{
	nLeftCol = m_SelRangeCell.m_LeftCol - m_CountFrozenCols;
	nRightCol = m_SelRangeCell.m_RightCol - m_CountFrozenCols;
	nTopRow = m_SelRangeCell.m_TopRow - m_CountHeaderRows;
	nButtonRow = m_SelRangeCell.m_BottomRow - m_CountHeaderRows;
}

// ������� ����������
BOOL CALXGrid::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = m_pGridWnd->GetStyle();
	pBar = m_pGridWnd->GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = m_pGridWnd->GetScrollBarCtrl(SB_HORZ);
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
			m_pGridWnd->SetScrollPos(SB_HORZ, (int)((double)x/m_nHScrollFactor));
			m_CurScrollPos.x = x;
		}
		if (y != yOrig)
		{
			m_pGridWnd->SetScrollPos(SB_VERT, (int)((double)y/m_nVScrollFactor));
			m_CurScrollPos.y = y;
		}
	}
	return TRUE;
}
