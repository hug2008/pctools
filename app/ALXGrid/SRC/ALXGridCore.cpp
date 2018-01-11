// ALXGridCore.cpp : implementation of the CALXGridCore class
//
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "ALXGridCore.h"

#if _MSC_VER > 1200
#include <atlbase.h>
#endif // _MSC_VER > 1200

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const DWORD maxScrollRange	=	SHRT_MAX;

// ������� ������ ������� ���������� � ������
BOOL DrawCellCtrl(CDC* pDC, CRect& rect, CELL_INFO& CellInfo, CELL_DATA& CellData)
{
	CRect rectText(rect), rectCtrl(rect);
	CString strText = CellData.m_strText;
	BOOL FirstDrawFrameCtrl = FALSE;
	UINT nState = CellInfo.m_nState;

	switch(CellInfo.m_nTypeCtrl)
	{
	case DFC_BUTTON:
		{
			// If it is the button DFCS_BUTTONPUSH
			if(nState & DFCS_BUTTONPUSH)
			{
				FirstDrawFrameCtrl = TRUE;
				rectCtrl.DeflateRect(1,0,0,1);
			}
			else
			{
				if(strText.GetLength()>0) // If it is necessary to output the text
				{
					strText = " " + strText;
					rectText.left	= min(rect.right,rect.left + 15);
				}
				else
				{
					rectText = CRect(0,0,0,0);
					rectCtrl.left	= max(rect.left,rect.left+(rect.Width()-13)/2);
				}
				rectCtrl.top	= max(rect.top,rect.top+(rect.Height()-13)/2);
				rectCtrl.right	= min(rect.right,rectCtrl.left + 13),
				rectCtrl.bottom = min(rect.bottom,rectCtrl.top + 13);

				switch(CellData.m_dwTag)
				{
				case 0:
					{
						nState =  nState & ~DFCS_CHECKED;
						break;
					}
				case 1:
					{
						if(nState == (nState | DFCS_BUTTON3STATE))
							nState =  (nState & ~DFCS_BUTTON3STATE) | DFCS_BUTTONCHECK | DFCS_CHECKED;
						else
							nState =  nState | DFCS_CHECKED;
						break;
					}
				case 2:
					{
						if(nState == (nState | DFCS_BUTTON3STATE))
							nState =  nState | DFCS_CHECKED;
						else
							nState =  nState & ~DFCS_CHECKED;
						break;
					}
				default:
						nState =  nState & ~DFCS_CHECKED;
				}
			}
			break;
		}
	case DFC_SCROLL:
		{
			if((nState & DFCS_SCROLLSIZEGRIPRIGHT) == DFCS_SCROLLSIZEGRIPRIGHT)
			{
				CRect rectCtrl1(rectCtrl), rectCtrl2(rectCtrl);
				rectCtrl1.left = max(rect.left,rect.right - ::GetSystemMetrics(SM_CXVSCROLL));
				rectCtrl1.bottom = rect.top + rect.Height()/2;
				rectCtrl2.top = rectCtrl1.bottom;
				rectCtrl2.left = rectCtrl1.left;
				rectText.right = rectCtrl.left;
				if(rectCtrl1.IsRectEmpty())
					return FALSE;
				return
					pDC->DrawText(strText, rectText, CellInfo.m_nFormat) &&
					pDC->DrawFrameControl(&rectCtrl1,CellInfo.m_nTypeCtrl,nState & ~ DFCS_SCROLLSIZEGRIPRIGHT | DFCS_SCROLLUP) &&
					pDC->DrawFrameControl(&rectCtrl2,CellInfo.m_nTypeCtrl,nState & ~ DFCS_SCROLLSIZEGRIPRIGHT | DFCS_SCROLLDOWN);
			}
			if((nState & DFCS_SCROLLSIZEGRIP) == DFCS_SCROLLSIZEGRIP)
			{
				FirstDrawFrameCtrl = TRUE;
				rectText.right = max(rect.left,rect.right - rect.Height()/2 - 1);
				break;
			}
			if((nState & DFCS_SCROLLCOMBOBOX) == DFCS_SCROLLCOMBOBOX)
			{
				rectCtrl.left = max(rect.left,rect.right - rect.Height());
				rectText.right = rectCtrl.left;
			}
			break;
		}
	case DFC_MENU:
		{
			FirstDrawFrameCtrl = TRUE;
			strText = " " + strText;
			rectCtrl.right	= min(rect.right,rect.left + 13);
			rectText.left	= min(rect.right,rectCtrl.right + 2); 
			break;
		}
	case DFC_CAPTION:
		{
			rectCtrl.left = max(rect.left,rect.right - rect.Height());
			rectText.right = rectCtrl.left; 
			break;
		}
	}

	if(rectCtrl.IsRectEmpty())
		return FALSE;
	if(FirstDrawFrameCtrl)
	{
		BOOL bResult =	pDC->DrawFrameControl(&rectCtrl,CellInfo.m_nTypeCtrl,nState);
						pDC->DrawText(strText, rectText, CellInfo.m_nFormat);

		return bResult;
/*		return
			pDC->DrawFrameControl(&rectCtrl,CellInfo.m_nTypeCtrl,nState) &&
			pDC->DrawText(strText, rectText, CellInfo.m_nFormat);
*/
	}
	else
	{
				pDC->DrawText(strText, rectText, CellInfo.m_nFormat);
		return	pDC->DrawFrameControl(&rectCtrl,CellInfo.m_nTypeCtrl,nState);
/*		return
			pDC->DrawText(strText, rectText, CellInfo.m_nFormat) &&
			pDC->DrawFrameControl(&rectCtrl,CellInfo.m_nTypeCtrl,nState);
*/	}
};

/////////////////////////////////////////////////////////////////////////////
// CALXGridCore

// �����������
CALXGridCore::CALXGridCore()
{
	m_pCellCtrl = NULL;

	m_bInsideUpdate = FALSE;

	m_curHResize = NULL;

	m_EnableResizeCol = FALSE;
	m_BeginResizeCol = FALSE;
		
	m_CountHeaderRows = 1;
	m_CountFrozenCols = 1;

	m_nHeaderColPressed = -1;
	m_nHeaderRowPressed = -1;

	m_nFrozenColPressed = -1;
	m_nFrozenRowPressed = -1;

	m_bSelAllBtnPressed = FALSE;

	m_nRowLineCount = 1;

	m_nActiveCol = m_CountFrozenCols;
	m_nActiveRow = m_CountHeaderRows;

	m_nSortOrder = None;
	m_nSortHeadCol = -1;
	m_nSortHeadRow = -1;

	m_nVScrollFactor = 1;
	m_nHScrollFactor = 1;

	m_CurScrollPos.x = 0;
	m_CurScrollPos.y = 0;

	m_sizeTotal = 0;
	m_sizePage = 0;
	m_sizeLine = 0;
	m_sizeExtArea = 0;

	// ��������� �� ����� ����
	m_pGridWnd = NULL;

	m_nRowHeight = 16;
	m_nHeadRowHeight = m_nRowHeight;
	m_FrozenColWidth = 8;

	// ��������� ������������ �����, ������������ ������
	LOGFONT	 logfont;
	memset(&logfont, 0, sizeof(logfont));
	CFont mFont;
	mFont.Attach(GetStockObject(DEFAULT_GUI_FONT));;
	mFont.GetLogFont(&logfont);
//	logfont.lfHeight = -11;
//	logfont.lfWeight = FW_NORMAL;
//	lstrcpy(logfont.lfFaceName, _T("Microsoft Sans Serif"));
//	logfont.lfCharSet = DEFAULT_CHARSET;//RUSSIAN_CHARSET;
//	logfont.lfOutPrecision = OUT_TT_PRECIS;
//	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
//	logfont.lfQuality = PROOF_QUALITY;
//	logfont.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
//	logfont.lfItalic = FALSE;

	m_pHeadFont = &m_Font;

	if(!m_Font.CreateFontIndirect(&logfont))
	{
		TRACE0("Failed to create grid font\n");
		return;
	}

	m_MetricsCells.FillDefault();

	// �������� ���������� �����
	m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
	m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

	m_nActiveSelCol = m_nActiveCol;
	m_nActiveSelRow = m_nActiveRow;
}

// ����������
CALXGridCore::~CALXGridCore()
{
	if(m_pCellCtrl != NULL)
		delete m_pCellCtrl;
	if(m_curHResize != NULL)
		DestroyCursor(m_curHResize);
}

// ������� ������ ���������� ����.
void CALXGridCore::OnGridDraw(CDC *pDC)
{
	CRect rectClip;
	// �������� ������� �����������.
	pDC->GetClipBox(&rectClip); 
	// ���� ���������� ������������ ?
	if (rectClip.IsRectEmpty())
		return;

	OnUpdateRect(pDC,rectClip);
}

// ������� ����������� ������� ���� � ������� �����.
CRect CALXGridCore::GetGridRect(CRect& rectPaint)
{
	CRect rectGrid = rectPaint;
	rectGrid.OffsetRect(m_CurScrollPos.x, m_CurScrollPos.y);
	return rectGrid;
}

// ������� ����������� ������� ���� � ������� �����.
CRect CALXGridCore::GetHeadRect(CRect& rectPaint)
{
	CRect rectHead = rectPaint;
	rectHead.OffsetRect(m_CurScrollPos.x,0); 
	return rectHead;
}

// ������� ����������� ������� ���� � ������� ����������� ��������.
CRect CALXGridCore::GetFrozenRect(CRect& rectPaint)
{
	CRect rectFrozen = rectPaint;
	rectFrozen.OffsetRect(0, m_CurScrollPos.y); 
	return rectFrozen;
}

// ������� �������� ��������� �� ����.
void CALXGridCore::SetGridWnd(CWnd *pGridWnd)
{
	m_pGridWnd = pGridWnd;
}

// ������� ���������� ������� ����.
void CALXGridCore::OnUpdateRect(CDC *pDC, CRect &rectClip)
{
	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(&m_Font);
	int OldBkMode =	pDC->SetBkMode(TRANSPARENT);

	COLORREF crOldText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	CRect rectClient;
	m_pGridWnd->GetClientRect(&rectClient);
	CRgn rgnGrid;
	rgnGrid.CreateRectRgn(  m_CountFrozenCols > 0 ? m_FrozenColWidth * m_CountFrozenCols - offsetXY + 1: 0,
							m_CountHeaderRows > 0 ? m_nHeadRowHeight * m_CountHeaderRows - offsetXY + 1: 0,
							rectClient.right, rectClient.bottom );
	// ������������ ����� ������� ������ ���� ���������

	// ���� ����� �������������� �����
	if(m_CountHeaderRows > 0 && rectClip.top <= m_nHeadRowHeight * m_CountHeaderRows - offsetXY )
	{
		// ���� ����� �������������� ������������ ����� �������
		if(m_CountFrozenCols > 0 && rectClip.left<= m_FrozenColWidth * m_CountFrozenCols - offsetXY)
		{
			pDC->SelectClipRgn(&rgnGrid);
			OnUpdateGridRect(pDC, CRect(m_FrozenColWidth * m_CountFrozenCols,m_nHeadRowHeight * m_CountHeaderRows - offsetXY ,rectClip.right,rectClip.bottom));
			pDC->SelectClipRgn(NULL);
			OnUpdateFrozenRect(pDC, CRect(rectClip.left,m_nHeadRowHeight * m_CountHeaderRows,m_FrozenColWidth * m_CountFrozenCols,rectClip.bottom));
			OnUpdateHeaderRect(pDC, CRect(m_FrozenColWidth * m_CountFrozenCols,rectClip.top,rectClip.right,m_nHeadRowHeight * m_CountHeaderRows));
			OnUpdateFrozenHeaderRect(pDC, CRect(1,1,m_FrozenColWidth * m_CountFrozenCols+1,m_nHeadRowHeight * m_CountHeaderRows+1));
		}
		else
		{
			pDC->SelectClipRgn(&rgnGrid);
			OnUpdateGridRect(pDC, CRect(rectClip.left,m_nHeadRowHeight * m_CountHeaderRows - offsetXY ,rectClip.right,rectClip.bottom));
			pDC->SelectClipRgn(NULL);
			OnUpdateHeaderRect(pDC, CRect(rectClip.left,rectClip.top,rectClip.right,m_nHeadRowHeight * m_CountHeaderRows));
		}
			
	}
	else
	{
		// ���� ����� �������������� ������������ ����� �������
		if(m_CountFrozenCols > 0 && rectClip.left <= m_FrozenColWidth * m_CountFrozenCols - offsetXY )
		{
			pDC->SelectClipRgn(&rgnGrid);
			OnUpdateGridRect(pDC, CRect(m_FrozenColWidth * m_CountFrozenCols,rectClip.top,rectClip.right,rectClip.bottom));
			pDC->SelectClipRgn(NULL);
			OnUpdateFrozenRect(pDC, CRect(rectClip.left,rectClip.top,m_FrozenColWidth * m_CountFrozenCols,rectClip.bottom));
		}
		else
			OnUpdateGridRect(pDC, rectClip);
	}

	int nGridHeight = max(m_nHeadRowHeight * m_CountHeaderRows - offsetXY + 1,TopWndPos(GetAllRowCount())+1);

	// ���� ������ ���� ������ ������ �����
	if(rectClip.bottom > nGridHeight)
	{
		// ���� ������ ���� ������ ������ �����
		if(rectClip.right > m_nRowWidth - m_CurScrollPos.x - offsetXY)
		{
			CBrush brushBackground1,brushBackground2;
			CRect rectBackgrnd1(m_nRowWidth - m_CurScrollPos.x - offsetXY + 1,rectClip.top - offsetXY,rectClip.right,rectClip.bottom);
			if(brushBackground1.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)))
				pDC->FillRect(&rectBackgrnd1, &brushBackground1);
			CRect rectBackgrnd2(rectClip.left,nGridHeight - offsetXY + 1,rectClip.right,rectClip.bottom);
			if(brushBackground2.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)))
				pDC->FillRect(&rectBackgrnd2, &brushBackground2);
		}
		else
		{
			CBrush brushBackground;
			CRect rectBackgrnd(rectClip.left,nGridHeight - offsetXY + 1,rectClip.right,rectClip.bottom);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)))
				pDC->FillRect(&rectBackgrnd, &brushBackground);
		}
	}
	else
	{
		// ���� ������ ���� ������ ������ �����
		if(rectClip.right > m_nRowWidth - m_CurScrollPos.x - offsetXY)
		{
			CBrush brushBackground;
			CRect rectBackgrnd(m_nRowWidth - m_CurScrollPos.x - offsetXY + 1,rectClip.top,rectClip.right,rectClip.bottom);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)))
				pDC->FillRect(&rectBackgrnd, &brushBackground);
		}
	}
	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);

	pDC->SetBkMode(OldBkMode);
	pDC->SetTextColor(crOldText);
}

// ������� ���������� �������� ����� ���������� � ������ �������.
// �������� bIncludePartiallyShownRows ���������� ����� �� ��������
// ������ �������� �������� � �������.
void CALXGridCore::RectToRowRange(const CRect &GridRect, int &nFirstRow, int &nLastRow, BOOL bIncludePartiallyShownRows)
{
	CRect rect(GridRect);
	// ������� ����������	
	rect.DeflateRect(offsetXY,offsetXY,-offsetXY,-offsetXY);
	// ��������� ��������
	int nRounding = bIncludePartiallyShownRows ? 0 : m_nRowHeight;
	// ��������� ������ �����
	int nHeadHeight = m_CountHeaderRows * m_nHeadRowHeight;

	// ��������� �������� �����
	if(rect.top <= nHeadHeight)
		nFirstRow = max(0,(rect.top + nRounding) / m_nHeadRowHeight);
	else
		nFirstRow = max(m_CountHeaderRows, min(m_CountHeaderRows + (rect.top - nHeadHeight + nRounding) / m_nRowHeight,GetAllRowCount() - 1));

	if(rect.bottom <= nHeadHeight)
		nLastRow = max(0, min((rect.bottom - nRounding) / m_nHeadRowHeight,m_CountHeaderRows - 1));
	else
		nLastRow = max(m_CountHeaderRows, min(m_CountHeaderRows + (rect.bottom - nHeadHeight - nRounding) / m_nRowHeight,GetAllRowCount() - 1));
}

// ������� ���������� �������� �������� ���������� � ������ �������.
// �������� bIncludePartiallyShownRows ���������� ����� �� ��������
// ������� �������� �������� � �������.
void CALXGridCore::RectToColRange(const CRect &GridRect, int &nFirstCol, int &nLastCol, BOOL bIncludePartiallyShownCols, int nHeadRow)
{
	CRect rect(GridRect);
	// ������� ����������	
	rect.DeflateRect(offsetXY,offsetXY,-offsetXY,-offsetXY);
	// ��������� ��������
	nFirstCol = 0;
	int curwidth;
	// ������� �������� ���� �������� ���������� �������� rect
	if(bIncludePartiallyShownCols)
	{
		curwidth = GetColWidth(nFirstCol, nHeadRow);
		while(rect.left >= curwidth && nFirstCol<GetAllColCount(nHeadRow)-1)
			curwidth = curwidth + GetColWidth(++nFirstCol, nHeadRow);

		nLastCol = nFirstCol;

		while(rect.right > curwidth && nLastCol<GetAllColCount(nHeadRow)-1)
			curwidth = curwidth + GetColWidth(++nLastCol, nHeadRow);
	}
	// ������� �������� ��������, ������� ������� �������� �������� rect
	else
	{
		curwidth = 0;
		while(rect.left + LeftColPos(m_CountFrozenCols, nHeadRow) > curwidth && nFirstCol<GetAllColCount(nHeadRow)-1)
			curwidth = curwidth + GetColWidth(nFirstCol++, nHeadRow);

		nLastCol = nFirstCol;

		if(nLastCol<=GetAllColCount(nHeadRow)-1)
			curwidth = curwidth + GetColWidth(nLastCol, nHeadRow);

		while(rect.right > curwidth && nLastCol<GetAllColCount(nHeadRow)-1)
		{
			nLastCol++;
			curwidth = curwidth + GetColWidth(nLastCol, nHeadRow);
			if(rect.right <= curwidth)
			{
				nLastCol--;
				return;
			}
		}
	}
}

// ����������� �������. ��� ������ ���������� ���������� �����
// � ����� ��� ����� ����� �����.
// ��� ������� ���������������� � ������ CALXGrid.
int CALXGridCore::GetGridRowCount()
{
	return 0;
}

// ����������� �������. ��� ������ ���������� ���������� ��������
// � ����� ��� ����� ����������� ��������.
// ��� ������� ���������������� � ������ CALXGrid.
int CALXGridCore::GetHeadColCount(int nHeadRow)
{
	return 0;
}

// ����������� �������. ��� ������ ���������� ������ ������� �����.
// �������� nCol ������ ������������ � ������ ����������� ����� �������.
// ��� ������� ���������������� � ������ CALXGrid.
int CALXGridCore::GetColWidth(int nCol, int nHeadRow)
{
	if(nCol >= m_CountFrozenCols)
		return GetHeadColWidth(GetGridCol(nCol), nHeadRow);
	else
		return ( nCol < 0 ? 0 : m_FrozenColWidth);
}

// ����������� �������. ������� ������ ��
// ����������� ����� ����� � ����� ����������� �������.
// ������ - �������� ���.
void CALXGridCore::OnDrawFrozenHeaderBtn(CDC *pDC, CRect &rectCell)
{
	BOOL bFlat = (m_pGridWnd->GetStyle() & AGS_FLAT);
	COLORREF clrCellFrame = ::GetSysColor(bFlat ? COLOR_BTNSHADOW : COLOR_WINDOWFRAME);

	// ������� ���������� ������	
	rectCell.DeflateRect(-offsetXY,-offsetXY,offsetXY,offsetXY);
	// ������� �����
	rectCell.InflateRect(1,1,0,0);
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,0,0);
	// ���� � ���������� ������� ������ ������ �� ������
	if(rectCell.Height() <= 2 || rectCell.Width() <= 2 || m_bSelAllBtnPressed)
	{
		CBrush brushBackground;
		// ����������� ������
		rectCell.DeflateRect(0,0,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
		return;
	}
	if(bFlat)
	{
		pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
		CBrush brushBackground;
		// ����������� ������
		rectCell.DeflateRect(1,1,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
	}
	else
		// ������ ������
		pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
}

// ����������� �������. ������� ������ �����.
// �������� nRow ���������� � ������ ���������� ����� �����.
void CALXGridCore::OnDrawGridRow(CDC *pDC, int nRow, int y, CRect& rectGrid)
{
	int nFirstCol, nLastCol, x;
	// ���������� ����� ������� � ���������� ����������������� �������
	RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
	nFirstCol = max(nFirstCol, m_CountFrozenCols);
	// ���������� x ���������� ����� ����� ������
	x = LeftWndPos(nFirstCol);

	// ��������� ������ ������ � ������� �����������
	for (int nCol = nFirstCol; nCol <= nLastCol; x = x + GetColWidth(nCol), nCol++)
		OnDrawGridCell(pDC,nCol,nRow,x,y);
	if(nRow == m_nActiveRow && 	m_pGridWnd->GetStyle() & AGS_ROWSELECT)
	{
		CRect rcFocus(LeftWndPos(m_CountFrozenCols) + 1, y + 1, LeftWndPos(m_CountFrozenCols) + (m_nRowWidth - m_CountFrozenCols * m_FrozenColWidth), y + m_nRowHeight);
		rcFocus.left = max(m_CountFrozenCols * m_FrozenColWidth - offsetXY, rcFocus.left);
		rcFocus.right = max(rcFocus.left, rcFocus.right);
		m_MetricsCells.m_FocusRect.DrawFocusRect(pDC, rcFocus);
	}
}

// ������� ������� ������ �����.
void CALXGridCore::OnDrawHeadRow(CDC *pDC, int nRow, int y, CRect& rectHeader)
{
	int nFirstCol, nLastCol, x;
	// ���������� ����� ������� � ���������� ����������������� �������
	RectToColRange(rectHeader, nFirstCol, nLastCol, TRUE, nRow);
	nFirstCol = max(nFirstCol, m_CountFrozenCols);
	// ���������� x ���������� ����� ����� ������
	x = LeftWndPos(nFirstCol, nRow);

	// ��������� ������ ������ � ������� �����������
	for (int nCol = nFirstCol; nCol <= nLastCol; x = x + GetColWidth(nCol, nRow), nCol++)
		OnDrawHeadCell(pDC,nCol,nRow,x,y);
}

// ������� ������� ������ ����������� ����� ��������.
void CALXGridCore::OnDrawFrozenRow(CDC *pDC, int nRow, int y, CRect& rectFrozen)
{
	int nFirstCol, nLastCol, x;
	// ���������� ����� ������� � ���������� ����������������� �������
	RectToColRange(rectFrozen, nFirstCol, nLastCol, TRUE);
	nFirstCol = min(nFirstCol, m_CountFrozenCols - 1);
	nLastCol  = min(nLastCol,  m_CountFrozenCols - 1);
	// ���������� x ���������� ����� ����� ������
	x = LeftColPos(nFirstCol) - offsetXY;

	// ��������� ������ ������� � ������� �����������
	for (int nCol = nFirstCol; nCol <= nLastCol; x = x + GetColWidth(nCol), nCol++)
		OnDrawFrozenCell(pDC,nCol,nRow,x,y);
}

// ������� ��������� ����� ����������� ������� ���� �����
// � ��������� �������� ���������.
void CALXGridCore::UpdateScrollSizes()
{
	// ��������� ������� �����(������ � ������ ������)
	CalculateGridMetrics();

	CSize sizeTotal(m_nRowWidth - offsetXY + m_sizeExtArea.cx,(m_nHeadRowHeight * m_CountHeaderRows + m_nRowHeight * GetGridRowCount()) - offsetXY + m_sizeExtArea.cy);

	sizeTotal.cx = max(0,sizeTotal.cx);
	sizeTotal.cy = max(0,sizeTotal.cy);

	// �������� ������� Scroll Bars
	SetScrollSizes(sizeTotal);
	// ���� ����� ������ ������� �������
	if(m_CurScrollPos.y + m_sizePage.cy > m_sizeTotal.cy)
		m_CurScrollPos.y = max(0,m_sizeTotal.cy - m_sizePage.cy + 1);
	if(m_CurScrollPos.x + m_sizePage.cx > m_sizeTotal.cx)
		m_CurScrollPos.x = max(0,m_sizeTotal.cx - m_sizePage.cx + 1);
}

// ������� ������������� ����� ����������� ������� ��� ���������.
void CALXGridCore::SetScrollSizes(const SIZE &sizeTotal)
{
	ASSERT(sizeTotal.cx >= 0 && sizeTotal.cy >= 0);

	m_sizeTotal = sizeTotal;

	if(m_sizeTotal.cx < maxScrollRange)
		m_nHScrollFactor = 1;
	else
		m_nHScrollFactor = (float)((double)m_sizeTotal.cx/(double)maxScrollRange + 1);

	if(m_sizeTotal.cy < maxScrollRange)
		m_nVScrollFactor = 1;
	else
		m_nVScrollFactor = (float)((double)m_sizeTotal.cy/(double)maxScrollRange + 1);

	if (m_pGridWnd != NULL)
		UpdateBars();
}

// ������� ��������� �������� ���������
void CALXGridCore::UpdateBars()
{
	// UpdateBars ����� ��������� �������� ������� ���� - ������������ ��� ���������
	if (m_bInsideUpdate)
		return;         // ��������� ����������� ������

	// ���������� ��������
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// ������������� �����������, ����� �������� ����������������
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	// ��������: ���./����. ����� ��������� �������� � ��������� ������� ��������� 'OnSize'
	ASSERT(m_sizeTotal.cx >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	// allow parent to do inside-out layout first
	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if (pParentWnd != NULL)
	{
		// ���� ������������ ���� ����������� �� ��� ���������, ������������ ����������
		// ������� ����������� � ������ ������ ��������� - ����� ��� ���������� �������
		if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0,
			(LPARAM)(LPCRECT)&rectClient) != 0)
		{
			// ����������� rectClient ������ GetTrueClientSize ���
			// ���������� ������� ���������� �������.
			bCalcClient = FALSE;
		}
	}

	CSize sizeClient;
	CSize sizeSb;
	CRect rect;

	if (bCalcClient)
	{
		// ���������� ����� �� ������ ��������� � �������� ������ ������� �������
		if (!GetTrueClientSize(sizeClient, sizeSb))
		{
			// ��� ����� ��� ����� ���������
			m_pGridWnd->GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// ���� ���������� ������� ������, �� ����� ������������ ������ ���������
				m_pGridWnd->EnableScrollBarCtrl(SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// ������������ ���� ���������� ���������� �������
		GetScrollBarSizes(sizeSb);
		sizeClient.cx = rectClient.Width();
		sizeClient.cy = rectClient.Height();
	}

	// ���������� �����, ����� �������� ������ ���������
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// �������� ��������� ������ ��������� � ���������� �������
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);

	// first scroll the window as needed
	// ������������ ���� �� ������� ��� ����������
	ScrollToPos(ptMove); // ����� �������� �������

	// ��� ���������, ���������� ��� ���������� ��������� ������� ������ ���������
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// ������ ������������ ������ ��� ���������
	m_pGridWnd->EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	m_pGridWnd->EnableScrollBarCtrl(SB_VERT, needSb.cy);
	// �������� ������ ���������� ������� ����
	m_pGridWnd->GetClientRect(&rect);
	sizeClient.cx = rect.Width();
	sizeClient.cy = rect.Height();
	if (needSb.cx)
	{
		info.nPage = (int)((double)sizeClient.cx/m_nHScrollFactor);
		if(info.nPage<=0)
			info.nPage = 1;
		m_sizePage.cx = sizeClient.cx;
		m_sizeLine.cx = sizeClient.cx / 10;
		info.nMax = (int)((double)m_sizeTotal.cx/m_nHScrollFactor);
		if (!m_pGridWnd->SetScrollInfo(SB_HORZ, &info, TRUE))
			m_pGridWnd->SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	if (needSb.cy)
	{
		info.nPage = (int)((double)sizeClient.cy/m_nVScrollFactor);
		if(info.nPage<=0)
			info.nPage = 1;
		m_sizePage.cy = sizeClient.cy;
		m_sizeLine.cy = m_nRowHeight;
		info.nMax = (int)((double)m_sizeTotal.cy/m_nVScrollFactor);
		if (!m_pGridWnd->SetScrollInfo(SB_VERT, &info, TRUE))
			m_pGridWnd->SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// ������� ���������� ��������
	m_bInsideUpdate = FALSE;
}

// ������� ���������� ������� ������������ ���� �����.
void CALXGridCore::GetScrollBarSizes(CSize &sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = m_pGridWnd->GetStyle();

	if (m_pGridWnd->GetScrollBarCtrl(SB_VERT) == NULL)
		// �� ��������� ������ ��������� ����� ����������� ���������� ����� ����
		sizeSb.cx = ::GetSystemMetrics(SM_CXVSCROLL);//afxData.cxVScroll;
	if (m_pGridWnd->GetScrollBarCtrl(SB_HORZ) == NULL)
		// �� ����������� ������ ��������� ����� ����������� ���������� ����� ����
		sizeSb.cy = ::GetSystemMetrics(SM_CXHSCROLL);//afxData.cyHScroll;
}

// ������� ���������� ��������� �������� ���������.
void CALXGridCore::GetScrollBarState(CSize sizeClient, CSize &needSb, CSize &sizeRange, CPoint &ptMove, BOOL bInsideClient)
{
	// �������� ������� ����� ��������� (�������� ��� ��������� � ���������� �������)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// ���������� ����� ��� ����� ���������
	sizeRange.cx = (int)((double)(m_sizeTotal.cx + 1 - sizeClient.cx)/m_nHScrollFactor);
	sizeRange.cy = (int)((double)(m_sizeTotal.cy + 1 - sizeClient.cy)/m_nVScrollFactor);
		// > 0 => ������� ������ ��������������
	ptMove = m_CurScrollPos;
		// ����� ������� ��������� (������ � ������� �������)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // ������� � ������ ���������
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // ����� ����������� ��� ���������

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // ������� � ������ ���������
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // ����� ����������� ��� ���������
	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// ����� �������������� ������ ���������
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// ���� ������� ������� ��������� ��������� ������ ������� ���������
	if (sizeRange.cx > 0 && ptMove.x/m_nHScrollFactor >= sizeRange.cx)
		ptMove.x = (int)(((double)sizeRange.cx)*m_nHScrollFactor);
	if (sizeRange.cy > 0 && ptMove.y/m_nVScrollFactor >= sizeRange.cy)
		ptMove.y = (int)(((double)sizeRange.cy)*m_nVScrollFactor);

	// ������ �������� ������������� �������
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, � ptMove ����� ��������
}

// ������� ������������ ����������� ���� ����� �� ��������� �������.
void CALXGridCore::ScrollToPos(POINT ptPos)
{
	ASSERT(ptPos.x >= 0);
	ASSERT(ptPos.y >= 0);

	int xOrig = m_CurScrollPos.x;
	m_pGridWnd->SetScrollPos(SB_HORZ, (int)(((double)ptPos.x)/m_nHScrollFactor));
	int yOrig = m_CurScrollPos.y;
	m_pGridWnd->SetScrollPos(SB_VERT, (int)(((double)ptPos.y)/m_nVScrollFactor));
	
	ScrollWindow(xOrig - ptPos.x, yOrig - ptPos.y);

	m_CurScrollPos.x = ptPos.x;
	m_CurScrollPos.y = ptPos.y;
}

// ������� ���������� TRUE ���� ����� ������ ���������.
BOOL CALXGridCore::GetTrueClientSize(CSize &size, CSize &sizeSb)
{
	CRect rect;
	m_pGridWnd->GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = m_pGridWnd->GetStyle();

	// ������� ������� ������ ����� ��������� ��� ����� ����
	GetScrollBarSizes(sizeSb);

	// ������� �������� �������������� ������ ������ ���������
	// (�� �����������, ��� ������ ��������� ����� ���� ���\����)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// �� ��������� ������ ��������� ����� ����������� ���������� ����� ����
		size.cx += sizeSb.cx;   // ����� ���. - �������������
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// �� ����������� ������ ��������� ����� ����������� ���������� ����� ����
		size.cy += sizeSb.cy;   // ����� ���. - �������������
	}

	// ���������� TRUE ���� ���������� �����
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

// ������� ���������� ������� ������� ������ ������.
// �������� nRow ������ ������������ � ������ ������� �����.
int CALXGridCore::TopRowPos(int nRow)
{
	if(nRow > m_CountHeaderRows)
		return m_CountHeaderRows * m_nHeadRowHeight + (nRow - m_CountHeaderRows) * m_nRowHeight;
	else
		return nRow * m_nHeadRowHeight;
}

// ������� ���������� ����� ������� ������ �������.
// �������� nCol ������ ������������ � ������ ����������� ������� �����.
int CALXGridCore::LeftColPos(int nCol, int nHeadRow)
{
	int iWidth = 0;
	for(int i = 0; i < nCol; i++)
		iWidth = iWidth + GetColWidth(i, nHeadRow);
	return iWidth;
}

// ������� ���������� ����� ������� ������ ������� � ����.
// �������� nCol ������ ������������ � ������ ����������� ������� �����.
int CALXGridCore::LeftWndPos(int nCol, int nHeadRow)
{
	return (LeftColPos(nCol, nHeadRow) - m_CurScrollPos.x - offsetXY);
}

// ������� ���������� ������� ������� ������ ������.
// �������� nRow ������ ������������ � ������ ����� � �����.
int CALXGridCore::TopWndPos(int nRow)
{
	return (TopRowPos(nRow) - m_CurScrollPos.y - offsetXY);
}

// ����������� �������. ������� ������� ����� � ����.
void CALXGridCore::OnUpdateGridRect(CDC *pDC, CRect &rectClip)
{
	int nFirstRow, nLastRow, y;
	CRect rectGrid = GetGridRect(rectClip);
	// ���������� ������ ������ � ��������� ���������������� ������
	RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, m_CountHeaderRows);
	nLastRow = min(nLastRow, GetGridRowCount() - 1 + m_CountHeaderRows);
	// ���������� y ���������� ����� ������� ������
	y = TopWndPos(nFirstRow);

	// ��������� ������ ������ � ������� �����������.
	for (int nRow = nFirstRow; nRow <= nLastRow; y += m_nRowHeight, nRow++)
		OnDrawGridRow(pDC,nRow,y,rectGrid);
}

// ������� ���������� ������� ���� ������������� ����������� ��������.
void CALXGridCore::OnUpdateFrozenRect(CDC *pDC, CRect &rectClip)
{
	int nFirstRow, nLastRow, y;
	CRect rectFrozen = GetFrozenRect(rectClip);
	// ���������� ������ ������ � ��������� ���������������� ������
	RectToRowRange(rectFrozen, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, m_CountHeaderRows);
	nLastRow = min(nLastRow, GetGridRowCount() - 1 + m_CountHeaderRows);
	// ���������� y ���������� ������� ������
	y = TopWndPos(nFirstRow);

	// ��������� ������ ������ � ������� �����������,
	for (int nRow = nFirstRow; nRow <= nLastRow; y += m_nRowHeight, nRow++)
		OnDrawFrozenRow(pDC,nRow,y,rectFrozen);
}

// ������� ����������(�����������) ������� ����
// �� ����������� �� � ����� �� � ����������� ��������.
void CALXGridCore::OnUpdateFrozenHeaderRect(CDC *pDC, CRect &rectCell)
{
	OnDrawFrozenHeaderBtn(pDC, rectCell);
}

// ������� ���������� ������� ���� ������������� �����.
void CALXGridCore::OnUpdateHeaderRect(CDC *pDC, CRect &rectClip)
{
	int nFirstRow, nLastRow, y;
	CRect rectHeader = GetHeadRect(rectClip);
	// ���������� ������ ������ � ��������� ���������������� ������
	RectToRowRange(rectHeader, nFirstRow, nLastRow, TRUE);
	nFirstRow = min(nFirstRow,m_CountHeaderRows - 1);
	nLastRow = min(nLastRow, m_CountHeaderRows - 1);
	// ���������� y ���������� ����� ������� ������
	y = TopRowPos(nFirstRow) - offsetXY;

	CFont* pOldFont = NULL;
	if(m_pHeadFont != &m_Font)
		pOldFont = pDC->SelectObject(m_pHeadFont);

	// ��������� ������ ������ � ������� �����������,
	for (int nRow = nFirstRow; nRow <= nLastRow; y += m_nHeadRowHeight, nRow++)
		OnDrawHeadRow(pDC,nRow,y,rectHeader);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);
}

// ������� ������������ ���� �����.
void CALXGridCore::ScrollWindow(int xAmount, int yAmount)
{
	CRect rectClient;
	m_pGridWnd->GetClientRect(&rectClient);
	// ���� � ����� ��� ��������� � ����������� ��������
	if(m_CountHeaderRows == 0 && m_CountFrozenCols == 0)
	{
		m_pGridWnd->ScrollWindow(xAmount, yAmount);
		return;
	}
	// ���� ����� �� ��� y
	if(yAmount != 0 )
	{
		if(xAmount != 0)// ���� ����� �� ��� x � �� ��� �
		{
			rectClient.left	=	m_CountFrozenCols * m_FrozenColWidth - offsetXY + 1;
			m_pGridWnd->ScrollWindow(xAmount, 0,NULL,&rectClient);
			rectClient.left	=	0;
			rectClient.top	=	m_CountHeaderRows * m_nHeadRowHeight - offsetXY + 1;
			m_pGridWnd->ScrollWindow(0, yAmount,NULL,&rectClient);
		}
		else			// ���� ����� ������ �� ��� �
		{
			rectClient.top	=	m_CountHeaderRows * m_nHeadRowHeight - offsetXY + 1;
			m_pGridWnd->ScrollWindow(xAmount, yAmount,NULL,&rectClient);
		}
	}
	else
	{
		if(xAmount != 0)// ���� ����� ������ �� ��� x
		{
			rectClient.left	=	m_CountFrozenCols * m_FrozenColWidth - offsetXY + 1;
			m_pGridWnd->ScrollWindow(xAmount, yAmount,NULL,&rectClient);
		}
		else
			m_pGridWnd->ScrollWindow(xAmount, yAmount);
	}
}

// ������� ������������ ���� �����.
void CALXGridCore::ScrollRow(int nRow, int yAmount)
{
	CRect rectScroll;
	m_pGridWnd->GetClientRect(&rectScroll);
	rectScroll.top = TopWndPos(nRow);
	if(m_CountHeaderRows * m_nHeadRowHeight - offsetXY > rectScroll.top)
	{
		m_pGridWnd->InvalidateRect(CRect(rectScroll.left,m_CountHeaderRows * m_nHeadRowHeight - offsetXY + 1,rectScroll.right,rectScroll.bottom));
		SetActiveCell(m_nActiveCol,m_nActiveRow);
		return;
	}
	if(rectScroll.IsRectEmpty())
	{
		SetActiveCell(m_nActiveCol,m_nActiveRow);
	}
	else
	{
		m_pGridWnd->ScrollWindow(0, yAmount,NULL,&rectScroll);
		if(yAmount >= 0)
			rectScroll.top += m_nRowHeight;
		m_pGridWnd->InvalidateRect(CRect(rectScroll.left,rectScroll.top,rectScroll.right,rectScroll.top + m_nRowHeight));
	}
}

// ������� ������������ ���� �����.
void CALXGridCore::ScrollCol(int nCol, int nHeadRow, int xAmount)
{
	CRect rectScroll;
	m_pGridWnd->GetClientRect(&rectScroll);
	rectScroll.left = LeftWndPos(nCol, nHeadRow);
	if(m_CountFrozenCols * m_FrozenColWidth - offsetXY > rectScroll.left)
		xAmount = m_CountFrozenCols * m_FrozenColWidth - offsetXY - rectScroll.left;
	if(!rectScroll.IsRectEmpty())
		m_pGridWnd->ScrollWindow(xAmount, 0,NULL,&rectScroll);
}

// ������� ������ ������ �����.
void CALXGridCore::OnDrawGridCell(CDC *pDC, int nCol, int nRow, int x, int y)
{
	CELL_INFO CellInfo;
	CELL_DATA CellData;
	CALXCellCtrl* pCellCtrl;
	DWORD dwStyle = m_pGridWnd->GetStyle();

	int nGridCol = GetGridCol(nCol); // ������ ������� ��� ����� ����������� �������
	int nGridRow = GetGridRow(nRow); // ������ ������ ��� ����� ����� �����

	// �������� ������� ������
	CALXCellMetrics* pCellMetrics = GetCellMetrics(nGridCol, nGridRow);

	ASSERT(pCellMetrics != NULL);

	// ���������� ������� �� ������
	BOOL bActiveCell = (nCol == m_nActiveCol && nRow == m_nActiveRow);
	// ���������� �������� �� ������
	BOOL bSelCell = (dwStyle & AGS_ROWSELECT) && nRow == m_nActiveRow;
	bSelCell = bSelCell || ((dwStyle & AGS_RANGESELECT) && m_SelRangeCell.InRange(nCol, nRow));

	// ���������� ���� ����
	COLORREF clrBackgrnd =  (bActiveCell ? pCellMetrics->m_ActiveCellColor.m_crBackgrnd : (bSelCell ? pCellMetrics->m_SelectCellColor.m_crBackgrnd : pCellMetrics->m_DefaultCellColor.m_crBackgrnd));
	// ���������� ���� ������
	COLORREF clrText =  (bActiveCell ? pCellMetrics->m_ActiveCellColor.m_crText : (bSelCell ? pCellMetrics->m_SelectCellColor.m_crText : pCellMetrics->m_DefaultCellColor.m_crText));

	// �������� ���������� � ������ ������
	CRect rectCell(x,y,x+GetColWidth(nCol)+1,y+m_nRowHeight+1);
	// ������� �����
	if(dwStyle & AGS_NOGRIDLINES)
	{
		COLORREF clrBackgrndHor =  (bSelCell || (bActiveCell && (dwStyle & AGS_ROWSELECT))) ? pCellMetrics->m_SelectCellColor.m_crBackgrnd : pCellMetrics->m_DefaultCellColor.m_crBackgrnd;
		COLORREF clrBackgrndVer =  pCellMetrics->m_DefaultCellColor.m_crBackgrnd;
		pCellMetrics->m_Borders.DrawBorders(pDC, rectCell,clrBackgrndHor,clrBackgrndVer,clrBackgrndHor,clrBackgrndVer,AOD_HOR_VERT);
	}
	else
		pCellMetrics->m_Borders.DrawBorders(pDC, rectCell);

	rectCell.DeflateRect(1,1,1,1); // ��������� ������� ���������
	
	if(rectCell.IsRectEmpty())
		return; // - ���� ������ ������ �������� �� �����

	// �������� ���������� � ���� ������
	CellInfo = GetCellInfo(nGridCol,nGridRow);

	// ���� ������ �������
	if(bActiveCell)
	{
		// �������� ��������� �� ������� ���������� �������
		pCellCtrl = GetCellCtrl(nGridCol,nGridRow);
		if(pCellCtrl != NULL)
			// �������� ���������� � ���������� ������ �� �������� ����������
			CellData = pCellCtrl->GetCellData();
		else
			// �������� ���������� � ���������� ������ �� ������������ ������
			CellData = GetCellData(nGridCol,nGridRow);
	}
	else
		// �������� ���������� � ���������� ������ �� ������������ ������
		CellData = GetCellData(nGridCol,nGridRow);


	CBrush brushBackgrnd;
	// ������� ������
	if(brushBackgrnd.CreateSolidBrush(clrBackgrnd))
		pDC->FillRect(&rectCell, &brushBackgrnd);

	// ���� ������ � ������������
	if(IsImage(nGridCol,nGridRow))
	{
		CPoint pt;
		CSize sz;

		// �������� ������ �����������
		GetSizeImage(nGridCol,nGridRow,sz);
		// ��������� ������ �����������, ���� ���������
		sz.cx = min(rectCell.Width(),sz.cx);
		sz.cy = min(rectCell.Height(),sz.cy);

		pt.x = rectCell.left+1;
		// ������� ����������� � ����� ������ �� ��� �
		pt.y = rectCell.top + (rectCell.Height() - sz.cy)/2;

		DrawImage(pDC,nGridCol,nGridRow,pt,sz,bActiveCell);

		// ������� ����� ������� ������
		rectCell.left = min(rectCell.right,rectCell.left+sz.cx+2);
	}

	// ���� ��� ������ �������
	if(bActiveCell && rectCell.Width() > 0 && rectCell.Height() > 0)
		// ������� ����� "������ �����"
		pCellMetrics->m_FocusRect.DrawFocusRect(pDC, rectCell);

	rectCell.DeflateRect(1,1,1,1); // ��������� ������� ���������

	// ������������� ���� ������
	COLORREF clrOldText = pDC->SetTextColor(clrText);

	CFont* pOldFont = NULL;
	if(bActiveCell)
	{
		// ���� ��� ������ ��������� �����
		if(pCellMetrics->pActiveFont != NULL)
			// ������������� �����
			pOldFont = pDC->SelectObject(pCellMetrics->pActiveFont);
	}	// ���� ��� ������ ��������� �����
	else if(pCellMetrics->pDefaultFont != NULL)
			// ������������� �����
			pOldFont = pDC->SelectObject(pCellMetrics->pDefaultFont);
	

	if(CellInfo.m_nTypeCtrl > 0)
		// ���� ����, ������ ������� ����������
		DrawCellCtrl(pDC,rectCell,CellInfo,CellData);
	else
		// ������� �����
		pDC->DrawText(CellData.m_strText, rectCell, CellInfo.m_nFormat);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);

	pDC->SetTextColor(clrOldText);
}

// ����������� �������. ������ ������ �����.
void CALXGridCore::OnDrawHeadCell(CDC *pDC, int nCol, int nRow, int x, int y)
{
	BOOL bFlat = (m_pGridWnd->GetStyle() & AGS_FLAT);
	CRect rectCell(x,y,x+GetColWidth(nCol, nRow)+1,y+m_nHeadRowHeight+1);
	COLORREF clrCellFrame = ::GetSysColor(bFlat ? COLOR_BTNSHADOW : COLOR_3DDKSHADOW);

	// ������� �����
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);

	// ���� � ���������� ������� ������ ������ �� ������ ��� ������ ������
	if((rectCell.Height() <= 3 || rectCell.Width() <= 3) || (nCol == m_nHeaderColPressed && nRow == m_nHeaderRowPressed))
	{
		CBrush brushBackground;
		// ����������� ������
		rectCell.DeflateRect(1,1,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
		rectCell.DeflateRect(4,2,1,0);
	}
	else
	{
		rectCell.DeflateRect(1,1,0,0);
		if(bFlat)
		{
			pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
			CBrush brushBackground;
			// ����������� ������
			rectCell.DeflateRect(1,1,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
			rectCell.DeflateRect(2,0,2,1);
		}
		else
		{
			// ������ ������
			pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
			rectCell.DeflateRect(3,1,3,2);
		}
	}


	if(!rectCell.IsRectEmpty())
	{
		// �������� ����� � ����� 
		HEADCELL_INFO CellInfo = GetHeadCellInfo(GetGridCol(nCol),nRow);

		if(m_nSortHeadCol + m_CountFrozenCols != nCol || m_nSortHeadRow != nRow || m_nSortOrder == None)
		{
			pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
			return;
		}

		int nArrowW = min(7,rectCell.Width());
		int nArrowH = min(7,rectCell.Height());
		int nArrowTopPos = rectCell.top + (rectCell.Height()-nArrowH)/2;
		int nArrowMargin = min(5,(rectCell.Width() - nArrowW)/2);
		int nGap = pDC->GetTextExtent(" ").cx * 3;

		if(rectCell.Width() <= nGap + nArrowW + nArrowMargin)
		{
			DrawArrow(pDC,CPoint(rectCell.left+(rectCell.Width()-nArrowW)/2,nArrowTopPos),CSize(nArrowW, nArrowH));
			return;
		}

		CRect rc(rectCell);
		pDC->DrawText(CellInfo.m_strText,rc,CellInfo.m_nFormat| DT_CALCRECT);

		if(CellInfo.m_nFormat & DT_RIGHT)
		{
			// ���� ������ � ������� ��������� ���������� � ������
			if(rc.Width() + nGap + nArrowW + nArrowMargin < rectCell.Width())
			{
				// ������� �����
				pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
				// ������ ������� ����� �� ������
				DrawArrow(pDC,CPoint(rectCell.right - rc.Width() - nGap - nArrowW, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
			else
			{
				CRect rectClipCell;
				rectClipCell.top = rectCell.top;
				rectClipCell.bottom = rectCell.bottom;
				rectClipCell.left = rectCell.left + (nArrowMargin + nArrowW + nGap);
				rectClipCell.right = rectCell.right;
				// ������� �����
				pDC->DrawText(CellInfo.m_strText, rectClipCell, CellInfo.m_nFormat);
				// ������ ������� ����� �� ������
				DrawArrow(pDC,CPoint(rectCell.left + nArrowMargin, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
		}
		else if(CellInfo.m_nFormat & DT_CENTER)
		{
			// ���� ������ � ������� ��������� ���������� � ������
			if((rectCell.Width() - rc.Width())/2 >  nGap + nArrowW + nArrowMargin)
			{
				// ������� �����
				pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
				// ������ ������� ������ �� ������
				DrawArrow(pDC,CPoint(rectCell.left + (rectCell.Width() - rc.Width())/2 + rc.Width() + nGap, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
			else
			{
				CRect rectClipCell;
				rectClipCell.top = rectCell.top;
				rectClipCell.bottom = rectCell.bottom;
				rectClipCell.left = rectCell.left;
				rectClipCell.right = rectCell.right - (nArrowMargin + nArrowW + nGap);
				// ������� �����
				pDC->DrawText(CellInfo.m_strText, rectClipCell, CellInfo.m_nFormat);
				// ������ ������� ������ �� ������
				DrawArrow(pDC,CPoint(rectCell.right - nArrowW - nArrowMargin, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
		}
		else
		{
			// ���� ������ � ������� ��������� ���������� � ������
			if(rc.Width() + nGap + nArrowW + nArrowMargin < rectCell.Width())
			{
				// ������� �����
				pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
				// ������ ������� ������ �� ������
				DrawArrow(pDC,CPoint(rectCell.left + rc.Width() + nGap, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
			else
			{
				CRect rectClipCell;
				rectClipCell.top = rectCell.top;
				rectClipCell.bottom = rectCell.bottom;
				rectClipCell.left = rectCell.left;
				rectClipCell.right = rectCell.right - (nArrowMargin + nArrowW + nGap);
				// ������� �����
				pDC->DrawText(CellInfo.m_strText, rectClipCell, CellInfo.m_nFormat);
				// ������ ������� ������ �� ������
				DrawArrow(pDC,CPoint(rectCell.right - nArrowW - nArrowMargin, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
		}
	}
}

// ����������� �������. ������ ����� ����������� ������.
void CALXGridCore::OnDrawFrozenCell(CDC *pDC, int nCol, int nRow, int x, int y)
{
	int cx = GetColWidth(nCol) + 1;
	int cy = m_nRowHeight + 1;
	BOOL bPressed = (nCol == m_nFrozenColPressed && nRow == m_nFrozenRowPressed);
	BOOL bDrawArrow = (nCol == m_CountFrozenCols - 1) && (m_nActiveRow == nRow || (GetKeyState(VK_SHIFT) >= 0 && nRow == m_nFrozenRowPressed));
	BOOL bFlat = (m_pGridWnd->GetStyle() & AGS_FLAT);

	DrawFrozenBtn(pDC, x, y, cx, cy, bPressed, bDrawArrow, bFlat);

/*	BOOL bFlat = (m_pGridWnd->GetStyle() & AGS_FLAT);
	CRect rectCell(x,y,x+GetColWidth(nCol)+1,y+m_nRowHeight+1);
	COLORREF clrCellFrame = ::GetSysColor(bFlat ? COLOR_BTNSHADOW : COLOR_3DDKSHADOW);

	// ������� �����
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,0,0);

	// ���� � ���������� ������� ������ ������ �� ������
	if(rectCell.Height() <= 2 || rectCell.Width() <= 2 )
	{
		CBrush brushBackground;
		// ����������� ������
		rectCell.DeflateRect(0,0,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
		return;
	}

	// ���� ��� ������ ������� � ��� ����� ������ ������ �� �����������
	if(m_nActiveRow == nRow && nCol == m_CountFrozenCols - 1)
	{
		// ���� ������ �� ������
		if(nCol != m_nFrozenColPressed || nRow != m_nFrozenRowPressed)
		{
			if(bFlat)
			{
				pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
				CBrush brushBackground;
				// ����������� ������
				rectCell.DeflateRect(1,1,1,1);
				if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
					pDC->FillRect(&rectCell, &brushBackground);
				rectCell.DeflateRect(1,1,1,1);
			}
			else
			{
				// ������ ������
				pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
				rectCell.DeflateRect(2,2,2,2);
			}
		}
		else
		{
			CBrush brushBackground;
			// ����������� ������
			rectCell.DeflateRect(0,0,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
			rectCell.DeflateRect(2,1,0,0);
		}
		// ������ �������
		CBrush brushBlack;
		if(brushBlack.CreateSolidBrush(::GetSysColor(COLOR_WINDOWFRAME)))
		{
			POINT ptTriangle[4];
			CRgn rgnTriangle;
			ptTriangle[0].x = rectCell.left + (rectCell.Width()-4)/2;
			ptTriangle[0].y = rectCell.top + (rectCell.Height()-7)/2 - 1;
			ptTriangle[1].x = ptTriangle[0].x+4;
			ptTriangle[1].y = ptTriangle[0].y+4;
			ptTriangle[2].x = ptTriangle[0].x;
			ptTriangle[2].y = ptTriangle[0].y+8;
			rgnTriangle.CreatePolygonRgn(ptTriangle,3,WINDING);
			pDC->FillRgn(&rgnTriangle,&brushBlack);
		}
	}
	else
	{
		// ���� ������ �� ������
		if(nCol != m_nFrozenColPressed || nRow != m_nFrozenRowPressed)
		{
			if(bFlat)
			{
				pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
				CBrush brushBackground;
				// ����������� ������
				rectCell.DeflateRect(1,1,1,1);
				if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
					pDC->FillRect(&rectCell, &brushBackground);
				rectCell.DeflateRect(1,1,1,1);
			}
			else
			{
				// ������ ������
				pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
				rectCell.DeflateRect(2,2,2,2);
			}
		}
		else
		{
			CBrush brushBackground;
			// ����������� ������
			rectCell.DeflateRect(0,0,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
		}
	}
*/
}

// ������� ��������� ������ � ������ ������
void CALXGridCore::CalculateGridMetrics()
{
	// �������� ����� �����
	DWORD dwStyle = m_pGridWnd->GetStyle();
	// ���������� ������ ������
	CSize sizeFont = GetFontSize(&m_Font);
	// ���������� ������ ������ �����
	m_nRowHeight = sizeFont.cy * m_nRowLineCount + 3; // 2 lines of text

	// ���� ������ ������ ����� ������ ���������� �� ������� ������
	if((dwStyle & AGS_CUSTOMHEAD_HEIGHT) == 0)
	{
		// ���� ������ ����� � ����� ���������
		if(m_pHeadFont != &m_Font)
		{
			ASSERT(m_pHeadFont != NULL);
			// ���������� ������ ������
			sizeFont = GetFontSize(m_pHeadFont);
			// ���������� ������ ������ �����
			m_nHeadRowHeight = sizeFont.cy + 3;
		}
		else
		// ���������� ������ ������ �����
			m_nHeadRowHeight = m_nRowHeight;
	}

	// ���� � ����� ���� ����������� �������, ���������� ������ ����������� ��������
	if(m_CountFrozenCols > 0)
		// ���� ������ ����������� ������� ������ ���������� �������������
		if((dwStyle & AGS_CUSTOMFROZEN_WIDTH) == 0)
				m_FrozenColWidth = m_nRowHeight/2;
	
	// ���������� ������ �����
	m_nRowWidth = 0;
	for(int i = 0; i < GetAllColCount(); i++)
		m_nRowWidth = m_nRowWidth + GetColWidth(i);
}

// ������� ���������� ������ ������.
CSize CALXGridCore::GetFontSize(CFont* pFont)
{
	CSize sizeFont;

	CFont* pOldFont = NULL;
	CClientDC dc(m_pGridWnd);

	pOldFont = dc.SelectObject(pFont);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	sizeFont.cx = tm.tmWeight;
	sizeFont.cy = tm.tmHeight;

	if(pOldFont != NULL)
		dc.SelectObject(pOldFont);

	return sizeFont;
}

// ������� ������������� �������� ������.
BOOL CALXGridCore::SetActiveCell(int nCol, int nRow)
{
	if(nCol < m_CountFrozenCols || nRow < m_CountHeaderRows || nCol >= GetAllColCount() || nRow >= GetAllRowCount())
		return FALSE;

	int nPrevActiveCol = m_nActiveCol;
	int nPrevActiveRow = m_nActiveRow;
	BOOL bRowSelect = (m_pGridWnd->GetStyle() & AGS_ROWSELECT);

	ALXSelRangeCell mPrevSelRangeCell = m_SelRangeCell;

	if(SaveModifyed(FALSE))
	{
		m_nActiveCol = nCol;
		m_nActiveRow = nRow;

		m_nActiveSelCol = m_nActiveCol;
		m_nActiveSelRow = m_nActiveRow;

		if(m_nActiveRow != nPrevActiveRow)
			OnChangeActiveRow(GetGridRow(m_nActiveRow));

		m_pGridWnd->SetFocus();
	}

	OnSetActiveCell(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));


	// �������� �������� ����������
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	if(nPrevActiveRow != m_nActiveRow)
		// ��������� ���������� �������� ������ ����������� �������
		for(int i = 0; i < m_CountFrozenCols; i++)
			UpdateCell(pDC, i, nPrevActiveRow);
	if(bRowSelect && nPrevActiveRow != m_nActiveRow)
		// ��������� ���������� ������
		UpdateRow(pDC,nPrevActiveRow);
	else
	{
		m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
		m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

		// ���� ����� �������� �������� �����
		if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
		{
			// ������� ��������� � ����������� ��������� ���������� �����
			CRect rectGrid = GetGridRect(rectClient);
			// ���������� �������� ������� � ���� ��������
			int nFirstCol, nLastCol;
			// ���������� ����� ������� � ���������� ����������������� �������
			RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
			nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
			nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
			// ���������� �������� ������� � ���� �����
			int nFirstRow, nLastRow;
			// ���������� ����� ������� � ���������� ����������������� �������
			RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
			nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
			nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
			// ������� ���������
			if(!mPrevSelRangeCell.IsEmpty())
				for(int i = nFirstRow; i <= nLastRow; i++)
					for(int j = nFirstCol; j <= nLastCol; j++)
						if(!m_SelRangeCell.InRange(j,i))
							UpdateCell(pDC, j, i);
		}

		// ��������� ���������� �������� ������
		UpdateCell(pDC, nPrevActiveCol, nPrevActiveRow);
		if(m_CountHeaderRows > 0 && nPrevActiveCol != m_nActiveCol)
			// ��������� ���������� �������� ������ �����
			UpdateCell(pDC, nPrevActiveCol, m_CountHeaderRows-1);
	}


	// ���� ����������, ������������ ���� ���,
	// ��� ����� ��������� ������ (�������) ���� �����.
	CPoint pt(m_CurScrollPos);
	BOOL bNeedToVScroll = TRUE;
	BOOL bNeedToHScroll = TRUE;

	// ������� ��������� ����� ?
	if(	LeftWndPos(m_nActiveCol) < m_CountFrozenCols * m_FrozenColWidth - offsetXY)
	{
		// ����� ��������� �������� �� ����� ��� ����� �����������.
		// ���� ����������, ��� ����� �� ��� ����� ������ ������� ��������.
		pt.x = LeftColPos(m_nActiveCol) - m_FrozenColWidth * m_CountFrozenCols;
	}
	else if ( LeftWndPos(m_nActiveCol + 1) - 1 > rectClient.right - offsetXY)
	{
		// ����� ��������� �������� �� ����� ��� ����� �����������.
		// ���� ����������, ��� ����� �� ��� ����� ��������� ������� ��������.
		pt.x = min(LeftColPos(m_nActiveCol) - m_FrozenColWidth * m_CountFrozenCols, LeftColPos(m_nActiveCol + 1) - rectClient.Width()) - offsetXY + 1;
	}
	else
		bNeedToHScroll = FALSE;

	// ������ ��������� ������ ?
	if(	TopWndPos(m_nActiveRow) < m_CountHeaderRows * m_nHeadRowHeight - offsetXY)
	{
		// ����� ��������� ������ �� ������ ��� ������ �����������.
		// ���� ����������, ��� ����� ��� ���� ����� ������ ������� �������.
		pt.y = TopRowPos(m_nActiveRow) - m_nHeadRowHeight * m_CountHeaderRows;
	}
	else if ( TopWndPos(m_nActiveRow + 1) - 1 > rectClient.bottom - offsetXY)
	{
		// ����� ��������� ������ �� ������ ��� ������ �����������.
		// ���� ����������, ��� ����� ��� ���� ����� ��������� ������� �������.
		pt.y = min(TopRowPos(m_nActiveRow) - m_nHeadRowHeight * m_CountHeaderRows, m_CountHeaderRows * m_nHeadRowHeight + (m_nActiveRow - m_CountHeaderRows + 1) * m_nRowHeight - rectClient.Height()) - offsetXY + 1;
	}
	else
		bNeedToVScroll = FALSE;

	// ���� ��������� ���������
	if(bNeedToHScroll || bNeedToVScroll)
		ScrollToPos(pt);
	
	// �������� ��������� �� ������� ���������� ������
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl == NULL)
	{
		// ������� ������� ���������� ������
		pCellCtrl = CreateCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow),GetCellCtrlStyle(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow)));
		if(pCellCtrl != NULL)
		{
			SetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);
			SetDataCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);
		}
	}
	else if(m_nActiveCol != nPrevActiveCol || m_nActiveRow != nPrevActiveRow)
		SetDataCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);


	if(nPrevActiveRow != m_nActiveRow || !bNeedToVScroll)
		// ��������� �������� ������ ����������� �������
		for(int i = 0; i < m_CountFrozenCols; i++)
			UpdateCell(pDC, i, m_nActiveRow);
	if(bRowSelect && nPrevActiveRow != m_nActiveRow)
		// ��������� �������� ������
		UpdateRow(pDC,m_nActiveRow);
	else
	{
		// ��������� �������� ������
		UpdateCell(pDC, m_nActiveCol, m_nActiveRow);
		if(m_CountHeaderRows > 0 && (nPrevActiveCol != m_nActiveCol || !bNeedToHScroll))
			// ��������� ���������� �������� ������ �����
			UpdateCell(pDC, m_nActiveCol, m_CountHeaderRows-1);
	}

	// ����������� �������� ����������
	m_pGridWnd->ReleaseDC(pDC);

	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if(pParentWnd != NULL)
		pParentWnd->SendMessage(GWM_ACTCOLROWCHANGED,(WPARAM)m_pGridWnd->m_hWnd,0);

	ActivateCtrl(m_nActiveCol, m_nActiveRow);

	return TRUE;
}

// ������� ������������� ��������� ������.
BOOL CALXGridCore::SetSelectCell(int nCol, int nRow)
{
	if(nCol < m_CountFrozenCols || nRow < m_CountHeaderRows || nCol >= GetAllColCount() || nRow >= GetAllRowCount())
		return FALSE;
	// ���� ������ �������� �������� �����
	if(!(m_pGridWnd->GetStyle() & AGS_RANGESELECT))
		return SetActiveCell(nCol, nRow);

	int nPrevActiveSelCol = m_nActiveSelCol;
	int nPrevActiveSelRow = m_nActiveSelRow;
	BOOL bRowSelect = (m_pGridWnd->GetStyle() & AGS_ROWSELECT);

	ALXSelRangeCell mPrevSelRangeCell = m_SelRangeCell;

	if(SaveModifyed(FALSE))
	{
		m_nActiveSelCol = nCol;
		m_nActiveSelRow = nRow;
	}

	m_pGridWnd->SetFocus();

	// �������� �������� ����������
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// �������� �������� ���������� �����
	if(!m_SelRangeCell.IsEmpty())
	{
		// �������� �������� ���������� �������
		if(m_SelRangeCell.m_LeftCol > m_nActiveSelCol)
		{
			m_SelRangeCell.m_LeftCol = m_nActiveSelCol;
			m_SelRangeCell.m_RightCol = m_nActiveCol;
		}
		else if(m_SelRangeCell.m_RightCol < m_nActiveSelCol)
		{
			m_SelRangeCell.m_LeftCol = m_nActiveCol;
			m_SelRangeCell.m_RightCol = m_nActiveSelCol;
		}
		else if(m_nActiveSelCol >= m_SelRangeCell.m_LeftCol && m_nActiveSelCol <= m_SelRangeCell.m_RightCol)
		{
			if(m_nActiveSelCol > nPrevActiveSelCol)
				if(m_nActiveSelCol < m_nActiveCol)
					m_SelRangeCell.m_LeftCol = m_nActiveSelCol;
				else
					m_SelRangeCell.m_LeftCol = m_nActiveCol;
			else if(m_nActiveSelCol < nPrevActiveSelCol)
				if(m_nActiveSelCol > m_nActiveCol)
					m_SelRangeCell.m_RightCol = m_nActiveSelCol;
				else
					m_SelRangeCell.m_RightCol = m_nActiveCol;
		}
		// �������� �������� ���������� �����
		if(m_SelRangeCell.m_TopRow > m_nActiveSelRow)
		{
			m_SelRangeCell.m_TopRow = m_nActiveSelRow;
			m_SelRangeCell.m_BottomRow = m_nActiveRow;
		}
		else if(m_SelRangeCell.m_BottomRow < m_nActiveSelRow)
		{
			m_SelRangeCell.m_TopRow = m_nActiveRow;
			m_SelRangeCell.m_BottomRow = m_nActiveSelRow;
		}
		else if(m_nActiveSelRow >= m_SelRangeCell.m_TopRow && m_nActiveSelRow <= m_SelRangeCell.m_BottomRow)
		{
			if(m_nActiveSelRow > nPrevActiveSelRow)
				if(m_nActiveSelRow < m_nActiveRow)
					m_SelRangeCell.m_TopRow = m_nActiveSelRow;
				else
					m_SelRangeCell.m_TopRow = m_nActiveRow;
			else if(m_nActiveSelRow < nPrevActiveSelRow)
				if(m_nActiveSelRow > m_nActiveRow)
					m_SelRangeCell.m_BottomRow = m_nActiveSelRow;
				else
					m_SelRangeCell.m_BottomRow = m_nActiveRow;
		}
	}
	else
	{
		m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveSelCol;
		m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveSelRow;
	}
		// ������� ��������� � ����������� ��������� ���������� �����
		CRect rectGrid = GetGridRect(rectClient);
		// ���������� �������� ������� � ���� ��������
		int nFirstCol, nLastCol;
		// ���������� ����� ������� � ���������� ����������������� �������
		RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
		nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
		nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
		// ���������� �������� ������� � ���� �����
		int nFirstRow, nLastRow;
		// ���������� ����� ������� � ���������� ����������������� �������
		RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
		nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
		nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
		// ������� ���������
		if(!mPrevSelRangeCell.IsEmpty())
			for(int i = nFirstRow; i <= nLastRow; i++)
				for(int j = nFirstCol; j <= nLastCol; j++)
					if(!m_SelRangeCell.InRange(j,i))
						UpdateCell(pDC, j, i);

	// ��������� ���������� �������� ������
	UpdateCell(pDC, nPrevActiveSelCol, nPrevActiveSelRow);
	if(m_CountHeaderRows > 0 && nPrevActiveSelCol != m_nActiveSelCol)
		// ��������� ���������� �������� ������ �����
		UpdateCell(pDC, nPrevActiveSelCol, m_CountHeaderRows-1);

	// ���� ����������, ������������ ���� ���,
	// ��� ����� ��������� ������ (�������) ���� �����.
	CPoint pt(m_CurScrollPos);
	BOOL bNeedToVScroll = TRUE;
	BOOL bNeedToHScroll = TRUE;

	// ������� ��������� ����� ?
	if(	LeftWndPos(m_nActiveSelCol) < m_CountFrozenCols * m_FrozenColWidth - offsetXY)
	{
		// ����� ��������� �������� �� ����� ��� ����� �����������.
		// ���� ����������, ��� ����� �� ��� ����� ������ ������� ��������.
		pt.x = LeftColPos(m_nActiveSelCol) - m_FrozenColWidth * m_CountFrozenCols;
	}
	else if ( LeftWndPos(m_nActiveSelCol + 1) - 1 > rectClient.right - offsetXY)
	{
		// ����� ��������� �������� �� ����� ��� ����� �����������.
		// ���� ����������, ��� ����� �� ��� ����� ��������� ������� ��������.
		pt.x = min(LeftColPos(m_nActiveSelCol) - m_FrozenColWidth * m_CountFrozenCols, LeftColPos(m_nActiveSelCol + 1) - rectClient.Width()) - offsetXY + 1;
	}
	else
		bNeedToHScroll = FALSE;

	// ������ ��������� ������ ?
	if(	TopWndPos(m_nActiveSelRow) < m_CountHeaderRows * m_nHeadRowHeight - offsetXY)
	{
		// ����� ��������� ������ �� ������ ��� ������ �����������.
		// ���� ����������, ��� ����� ��� ���� ����� ������ ������� �������.
		pt.y = TopRowPos(m_nActiveSelRow) - m_nHeadRowHeight * m_CountHeaderRows;
	}
	else if ( TopWndPos(m_nActiveSelRow + 1) - 1 > rectClient.bottom - offsetXY)
	{
		// ����� ��������� ������ �� ������ ��� ������ �����������.
		// ���� ����������, ��� ����� ��� ���� ����� ��������� ������� �������.
		pt.y = min(TopRowPos(m_nActiveSelRow) - m_nHeadRowHeight * m_CountHeaderRows, m_CountHeaderRows * m_nHeadRowHeight + (m_nActiveSelRow - m_CountHeaderRows + 1) * m_nRowHeight - rectClient.Height()) - offsetXY + 1;
	}
	else
		bNeedToVScroll = FALSE;

	// ���� ��������� ���������
	if(bNeedToHScroll || bNeedToVScroll)
		ScrollToPos(pt);
	

	rectGrid = GetGridRect(rectClient);
	// ���������� �������� ������� � ���� ��������
	nFirstCol, nLastCol;
	// ���������� ����� ������� � ���������� ����������������� �������
	RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
	nFirstCol = max(nFirstCol, m_SelRangeCell.m_LeftCol);
	nLastCol = min(nLastCol, m_SelRangeCell.m_RightCol);
	// ���������� �������� ������� � ���� �����
	nFirstRow, nLastRow;
	// ���������� ����� ������� � ���������� ����������������� �������
	RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, m_SelRangeCell.m_TopRow);
	nLastRow = min(nLastRow, m_SelRangeCell.m_BottomRow);
	// ��������� ���������� ������
	for(int i = nFirstRow; i <= nLastRow; i++)
		for(int j = nFirstCol; j <= nLastCol; j++)
			UpdateCell(pDC, j, i);

	// ��������� �������� ������
	UpdateCell(pDC, m_nActiveSelCol, m_nActiveSelRow);
	if(m_CountHeaderRows > 0 && (nPrevActiveSelCol != m_nActiveSelCol || !bNeedToHScroll))
		// ��������� ���������� �������� ������ �����
		UpdateCell(pDC, m_nActiveSelCol, m_CountHeaderRows-1);

	// ����������� �������� ����������
	m_pGridWnd->ReleaseDC(pDC);

	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if(pParentWnd != NULL)
		pParentWnd->SendMessage(GWM_SELCOLROWRANGE,(WPARAM)m_pGridWnd->m_hWnd,0);

	return TRUE;
}

// ������� �������������� ������ �����.
void CALXGridCore::UpdateCell(CDC* pDC, int nCol, int nRow)
{
	// ���������� ������� ������� ����� ��������
	CRect rectCell;
	if(nCol < m_CountFrozenCols)
	{
		rectCell.left = m_FrozenColWidth * nCol - offsetXY;
		rectCell.right = m_FrozenColWidth * (nCol + 1) - offsetXY - 1;
	}
	else
	{
		rectCell.left = LeftWndPos(nCol, nRow < m_CountHeaderRows ? nRow : -1);
		rectCell.right = LeftWndPos(nCol+1, nRow < m_CountHeaderRows ? nRow : -1)-1;
	}
	if(nRow < m_CountHeaderRows)
	{
		rectCell.top = m_nHeadRowHeight * nRow - offsetXY;
		rectCell.bottom = m_nHeadRowHeight * (nRow + 1) - offsetXY - 1;
	}
	else
	{
		rectCell.top = TopWndPos(nRow);
		rectCell.bottom = TopWndPos(nRow+1)-1;
	}

	// ���� ������� ������ � pDC �� NULL
	if(rectCell.right > 0 && rectCell.bottom > 0 && pDC != NULL)
		OnUpdateRect(pDC,rectCell); // ��������� ������� ���� 
}

// ������� �������� ����, ��� ������� ������ ����� ������������.
void CALXGridCore::InvalidateCell(int nCol, int nRow)
{
	CRect rectCell;
	rectCell.left = LeftWndPos(nCol);
	rectCell.right = rectCell.left + GetColWidth(nCol);
	rectCell.top = TopWndPos(nRow);
	rectCell.bottom = rectCell.top + m_nRowHeight;
	m_pGridWnd->InvalidateRect(rectCell);
}

// ������� �������������� ������ �����.
void CALXGridCore::UpdateRow(CDC* pDC, int nRow)
{
	// ���������� ������� ������� ����� ��������
	CRect rectCell;
	m_pGridWnd->GetClientRect(&rectCell);
	rectCell.left = 0;

	if(nRow < m_CountHeaderRows)
	{
		rectCell.top = m_nHeadRowHeight * nRow - offsetXY;
		rectCell.bottom = m_nHeadRowHeight * (nRow + 1) - offsetXY - 1;
	}
	else
	{
		rectCell.top = TopWndPos(nRow);
		rectCell.bottom = TopWndPos(nRow+1)-1;
	}

	// ���� ������� ������ � pDC �� NULL
	if(rectCell.right > 0 && rectCell.bottom > 0 && pDC != NULL)
		OnUpdateRect(pDC,rectCell); // ��������� ������� ���� 
}

// ������� �������� ����, ��� ������� ������ ����� ������������.
void CALXGridCore::InvalidateRow(int nRow)
{
	CRect rectRow;
	m_pGridWnd->GetClientRect(&rectRow);
	rectRow.top = TopWndPos(nRow);
	rectRow.bottom = rectRow.top + m_nRowHeight;
	m_pGridWnd->InvalidateRect(rectRow);
}

// ������� ���������� TRUE ���� ������ ��������� ����� ��������� �����.
BOOL CALXGridCore::IsCursorBetweenCols(int x, int& ResizeCol, int& ResizeRow)
{
	x = x + m_CurScrollPos.x;
	int i = 0;
	int iPrevWidth = 0, iNextWidth = GetColWidth(i,ResizeRow);
	while( (x < iPrevWidth  || x > iNextWidth) && i < GetAllColCount(ResizeRow)-1)
	{
		i++;
		iPrevWidth = iNextWidth;
		iNextWidth = iNextWidth + GetColWidth(i,ResizeRow);
	}

	if(x <= iPrevWidth + 2 || x >= iNextWidth - 2)
	{
		if(x <= iPrevWidth + 2)
			ResizeCol = i-1;
		else
			ResizeCol = i;
		if(ResizeCol < m_CountFrozenCols && !(m_pGridWnd->GetStyle() & AGS_CUSTOMFROZEN_WIDTH))
			return FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

// ������� ���������� ��� ���������� ��������� ������ �������
void CALXGridCore::EndResizeCol(int x)
{
	// ����������� �����. ���� � �����. �����
	x = x + m_CurScrollPos.x;

	// �������� ����� ������ �������
	int nNewWidth = x - LeftColPos(m_ResizeCol,m_ResizeRow);
	int nOldWidth = GetColWidth(m_ResizeCol,m_ResizeRow);
	if(nNewWidth < 0)
		nNewWidth = 0;

	if(m_ResizeCol >= m_CountFrozenCols)
	{
		SetHeadColWidth(GetGridCol(m_ResizeCol), m_ResizeRow, nNewWidth);
		ScrollCol(m_ResizeCol, m_ResizeRow, nOldWidth - nNewWidth);
	}
	else
		m_FrozenColWidth = nNewWidth;

	BOOL bActiveCtrl = FALSE;
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl != NULL)
	{
		bActiveCtrl = pCellCtrl->IsActive();
		// ������������ �������� ������� ���������� 
		pCellCtrl->DeactivateCtrl();
	}

	// ��������� Scroll Bars
	UpdateScrollSizes();

	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if(pParentWnd != NULL)
		pParentWnd->SendMessage(GWM_COLWIDTHCHANGED,(WPARAM)m_pGridWnd,0);

	m_pGridWnd->SetFocus();

	if(m_ResizeCol < m_CountFrozenCols)
		m_pGridWnd->Invalidate();

	if(bActiveCtrl)
		ActivateCtrl(m_nActiveCol, m_nActiveRow);
}

// ������� ������������� �������� ������ �����.
BOOL CALXGridCore::SetActiveCell(CPoint point)
{
	int nCol = 0, nRow = 0;
	if(point.x <= m_CountFrozenCols*m_FrozenColWidth)
		nCol = m_nActiveCol;
	else
		nCol = XToCol(point.x + m_CurScrollPos.x);

	if(point.y <= m_CountHeaderRows*m_nHeadRowHeight)
		nRow = m_nActiveRow;
	else
		nRow = YToRow(point.y + m_CurScrollPos.y);

	return SetActiveCell(nCol, nRow);
}

// ������� ������������� ���������� ������.
BOOL CALXGridCore::SetSelectCell(CPoint point)
{
	int nCol = 0, nRow = 0;
	if(point.x <= m_CountFrozenCols*m_FrozenColWidth)
		nCol = m_nActiveSelCol;
	else
		nCol = XToCol(point.x + m_CurScrollPos.x);

	if(point.y <= m_CountHeaderRows*m_nHeadRowHeight)
		nRow = m_nActiveSelRow;
	else
		nRow = YToRow(point.y + m_CurScrollPos.y);

	return SetSelectCell(nCol, nRow);
}

// ������� ���������� ����� ������� ������� ����������� ���������� x.
int CALXGridCore::XToCol(int x, int nHeadRow)
{
	int nCol = 0;
	int iPrevWidth = 0, iNextWidth = GetColWidth(nCol, nHeadRow);
	while( (x < iPrevWidth  || x > iNextWidth) && nCol < GetAllColCount(nHeadRow)-1)
	{
		nCol++;
		iPrevWidth = iNextWidth;
		iNextWidth = iNextWidth + GetColWidth(nCol,nHeadRow);
	}
	return (x > iNextWidth) ? ++nCol : nCol;
}

// ������� ���������� ����� ������ ������� ����������� ���������� y.
int CALXGridCore::YToRow(int y)
{
	if(y < m_CountHeaderRows * m_nHeadRowHeight)
		return y / m_nHeadRowHeight;
	else
		return m_CountHeaderRows + (y - m_CountHeaderRows * m_nHeadRowHeight) / m_nRowHeight;
}

// ����������� �������. ���������� ��������� �� ������� ���������� ������.
// ��� ������� ���������������� � ������ CALXGrid
CALXCellCtrl* CALXGridCore::GetCellCtrl(int nCol, int nRow)
{
	return m_pCellCtrl;
}

// ����������� �������. ������������� ������� ���������� ��� ������.
// ��� ������� ���������������� � ������ CALXGrid
void CALXGridCore::SetCellCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl)
{
}

// ����������� �������. �������� ������ �������� ���������� �������.
// ��� ������� ����� �������������� � ����� ���������������� ������.
void CALXGridCore::SetDataCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl)
{
	pCellCtrl->SetData(GetCellData(nCol,nRow));
}

// ����������� �������. ��� ������ ��������� ������� ���������� ��� ������.
// ��� ������� ���������������� � ������ CALXGrid
CALXCellCtrl* CALXGridCore::CreateCellCtrl(int nCol, int nRow, DWORD dwStyle, UINT nID)
{
	CALXCellCtrl* pCellCtrl = new CALXCellCtrl;
	pCellCtrl->CreateCtrl(dwStyle,m_pGridWnd,IDC_CELLCTRL+nCol);
	pCellCtrl->EnableCtrl(FALSE);
	pCellCtrl->SetFontCtrl(&m_Font);
	return pCellCtrl;
}

// ����������� �������. ��� ������ ���������� ������ ������.
// ��� ������� ������ ��������������� � �������� ������ ����������� �� CALXGridView ��� CALXGridCtrl.
CELL_DATA CALXGridCore::GetCellData(int nCol, int nRow)
{
	CELL_DATA CtrlData;
	CtrlData.m_strText.Format("%u - %u",nCol,nRow);
	return CtrlData;
}

// ����������� �������. ��� ������ ���������� ���������� � ����� ������.
// ��� ������� ���������������� � ������ CALXGrid.
CELL_INFO CALXGridCore::GetCellInfo(int nCol, int nRow)
{
	CELL_INFO CellInfo;
	CellInfo.m_nFormat = DT_CENTER | DT_VCENTER;
	CellInfo.m_nTypeCtrl = 0;
	return CellInfo;
}

// ����������� �������. ��� ������ ���������� ���������� � ����� �������� ���������� �������.
// ��� ������� ���������������� � ������ CALXGrid.
DWORD CALXGridCore::GetCellCtrlStyle(int nCol, int nRow)
{
	return WS_CHILD;
}

// ������� ���������� ��������� �� ���� �����.
CWnd* CALXGridCore::GetGridWnd()
{
	return m_pGridWnd;
}

// ����������� �������. ��� ���������� ������� ������.
// ��� ������� ����� ��������������� � �������� ������ ����������� �� CALXGridView ��� CALXGridCtrl.
CALXCellMetrics* CALXGridCore::GetCellMetrics(int nCol, int nRow)
{
	return &m_MetricsCells;
}

// ����������� �������. ��� ���������� ���������� �� ��� ������ �����������.
// ��� ������� ���������������� � ������ CALXGrid.
BOOL CALXGridCore::IsImage(int nCol, int nRow)
{
	return FALSE;
}

// ����������� �������. ��� ���������� ��� ����������� ��� ������.
// ��� ������� ���������������� � ������ CALXGrid.
UINT CALXGridCore::GetTypeImage(int nCol, int nRow)
{
	return NULL;
}

// ����������� �������. ��� ������ ���������� ������ ����������� ��� ������ ������.
// ��� ������� ���������������� � ������ CALXGrid.
BOOL CALXGridCore::GetSizeImage(int nCol, int nRow, CSize& sz)
{
	sz.cx = 0;
	sz.cy = 0;
	return FALSE;
}

// ����������� �������. ��� ������ ���������� ��������� �� �����������.
// ��� ������� ���������������� � ������ CALXGrid.
LPARAM CALXGridCore::GetImage(int nCol, int nRow, BOOL bSelected)
{
	return NULL;
}

// ����������� �������. ��� ������ ���������� ���������� � �����������.
// ��� ������� ������ ��������������� � �������� ������ ����������� �� CALXGridView ��� CALXGridCtrl.
void CALXGridCore::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{

}

// ������������� ����� � ���� ����� "����� �����"
void CALXGridCore::SetFocusRect(COLORREF crFaceColor, COLORREF crBkColor/* = ::GetSysColor(COLOR_WINDOW)*/, BOOL bSolid/* = TRUE*/)
{
	m_MetricsCells.m_FocusRect.m_crFaceColor =	crFaceColor;
	m_MetricsCells.m_FocusRect.m_crBackColor = crBkColor;
	m_MetricsCells.m_FocusRect.m_bSolid = bSolid;
}

// ������� �������� ��������� �������� ���������� ������� � ������� ����� ������ ����.
// ������������ ��� ����������� �������� ���������� �������.
void CALXGridCore::SendLButtonDown(UINT nFlags, CPoint point)
{
	CPoint pointClick;
	// ����������� ���������� ���� � ���������� �������� ����������.
	pointClick.x = point.x + m_CurScrollPos.x - LeftColPos(m_nActiveCol);
	pointClick.y = point.y + m_CurScrollPos.y - TopRowPos(m_nActiveRow);
	if(pointClick.x < 0 || pointClick.y < 0)
		return;
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl != NULL)
		pCellCtrl->SendLButtonDown(nFlags, pointClick);
}

// ������� ��������� � ������������� �������
// � ������ �������� ���������� �������.
void CALXGridCore::ActivateCtrl(int nCol, int nRow)
{
	if(nRow < m_CountHeaderRows)
		return;

	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(nCol),GetGridRow(nRow));
	if(pCellCtrl != NULL)
	{
		// ���� ������ � ������������
		if(IsImage(GetGridCol(nCol),GetGridRow(nRow)))
		{
			CSize sz;
			// �������� ������ �����������, ����� ������� ������ ������
			GetSizeImage(GetGridCol(nCol),GetGridRow(nRow),sz);
			sz.cx += 2;
			pCellCtrl->ActivateCtrl(LeftWndPos(nCol)+sz.cx,TopWndPos(nRow),GetColWidth(nCol)-sz.cx,m_nRowHeight);
		}
		else
			pCellCtrl->ActivateCtrl(LeftWndPos(nCol),TopWndPos(nRow),GetColWidth(nCol),m_nRowHeight);
	}
}

// ������� ������� ������� � ������.
void CALXGridCore::DrawImage(CDC *pDC, int nCol, int nRow, CPoint& pt, CSize& sz, BOOL bSelected)
{

	switch(GetTypeImage(nCol,nRow))
	{
	case DIT_BITMAP :
		{
			LPARAM hBitmap = GetImage(nCol,nRow,bSelected);
			if(hBitmap != NULL)
				DrawState(pDC->m_hDC,NULL,NULL, hBitmap,0,pt.x,pt.y,sz.cx,sz.cy,DST_BITMAP);
			break;
		}
	case DIT_ICON :
		{
			HICON hIcon = (HICON)GetImage(nCol,nRow,bSelected);
			if(hIcon != NULL)
				DrawIconEx(pDC->m_hDC,pt.x,pt.y,hIcon,sz.cx,sz.cy,NULL,NULL,DI_NORMAL | DI_DEFAULTSIZE);
			break;
		}
	case DIT_IMGLIST :
		{
			IMAGELISTDRAWPARAMS imldp;
			memset(&imldp, 0, sizeof(imldp));
			imldp.cbSize = sizeof(imldp);
			imldp.hdcDst = pDC->m_hDC;
			imldp.rgbBk = CLR_DEFAULT;
			imldp.rgbFg = CLR_DEFAULT;
			imldp.fStyle = ILD_TRANSPARENT;
			imldp.himl = NULL;
			imldp.cx = sz.cx;
			imldp.cy = sz.cy;
			imldp.x = pt.x;
			imldp.y = pt.y;

#if _MSC_VER > 1200
			DWORD dwMajor = 0, dwMinor = 0;
			AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
			if (dwMajor < 6)
				imldp.cbSize = IMAGELISTDRAWPARAMS_V3_SIZE;
#endif // _MSC_VER > 1200

			GetImageListDrawParams(nCol,nRow,&imldp,bSelected);

			if(imldp.himl != NULL)
				ImageList_DrawIndirect(&imldp);

			break;
		}
	}
}

// ������� �������������� ����� ���������
void CALXGridCore::RedrawFocusRect()
{
	CRect rectCell,rectRow(0,0,0,0);
	CRect rectClient;
	CRgn rgnGrid;
	int nActGridCol = GetGridCol(m_nActiveCol); // ������ ������� ��� ����� ����������� �������
	int nActGridRow = GetGridRow(m_nActiveRow); // ������ ������ ��� ����� ����� �����

	m_pGridWnd->GetClientRect(&rectClient);

	rectCell.left = LeftWndPos(m_nActiveCol) + 1;
	rectCell.right = rectCell.left + GetColWidth(m_nActiveCol) - 1;
	rectCell.top = TopWndPos(m_nActiveRow) + 1;
	rectCell.bottom = rectCell.top + m_nRowHeight - 1;

	// ���� ������ � ������������
	if(IsImage(nActGridCol,nActGridRow))
	{
		CSize sz;
		// �������� ������ �����������
		GetSizeImage(nActGridCol,nActGridRow,sz);
		// ��������� ������ �����������, ���� ���������
		sz.cx = min(rectCell.Width(),sz.cx);
		// ������� ����� ������� ������
		rectCell.left = min(rectCell.right,rectCell.left+sz.cx+2);
	}
	if(m_pGridWnd->GetStyle() & AGS_ROWSELECT)
	{
		rectRow.left = LeftWndPos(m_CountFrozenCols) + 1;
		rectRow.right = rectRow.left + (m_nRowWidth - m_CountFrozenCols * m_FrozenColWidth) - 1;
		rectRow.top = rectCell.top;
		rectRow.bottom = rectCell.bottom;
	}
	else
	{
		if(rectCell.IsRectEmpty())
			return;
	}

	rgnGrid.CreateRectRgn(  m_CountFrozenCols > 0 ? m_FrozenColWidth * m_CountFrozenCols - offsetXY + 1: 0,
							m_CountHeaderRows > 0 ? m_nHeadRowHeight * m_CountHeaderRows - offsetXY + 1: 0,
							rectClient.right,rectClient.bottom  );

	// �������� ������� ������
	CALXCellMetrics* pCellMetrics = GetCellMetrics(nActGridCol, nActGridRow);

	ASSERT(pCellMetrics != NULL);

	// �������� �������� ����������
	CDC* pDC = m_pGridWnd->GetDC();

	pDC->SelectClipRgn(&rgnGrid);
		pCellMetrics->m_FocusRect.DrawFocusRect(pDC, rectCell);
		if(!rectRow.IsRectEmpty())
			m_MetricsCells.m_FocusRect.DrawFocusRect(pDC, rectRow);
	pDC->SelectClipRgn(NULL);

	// ����������� �������� ����������
	m_pGridWnd->ReleaseDC(pDC);
}

void CALXGridCore::ScrollAndSetActCol(int HPos, int nCol, BOOL RedrawCells /* = TRUE*/)
{
	if(nCol < m_CountFrozenCols || nCol >= GetAllColCount())
		return;

	HPos = min(HPos,m_sizeTotal.cx - m_sizePage.cx + 1);
	HPos = HPos > 0 ? HPos : 0;

	int nPrevActiveCol = m_nActiveCol;
	int nPrevActiveRow = m_nActiveRow;
	BOOL bRowSelect = (m_pGridWnd->GetStyle() & AGS_ROWSELECT);

	ALXSelRangeCell mPrevSelRangeCell = m_SelRangeCell;

	SaveModifyed();

	m_nActiveCol = nCol;

	m_nActiveSelCol = m_nActiveCol;
	m_nActiveSelRow = m_nActiveRow;
	
	m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
	m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl != NULL)
		SetDataCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);

	// �������� �������� ����������
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// ���� ����� �������������� ������
	if(RedrawCells)
	{
		// ���� ����� �������� �������� �����
		if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
		{
			// ������� ��������� � ����������� ��������� ���������� �����
			CRect rectGrid = GetGridRect(rectClient);
			// ���������� �������� ������� � ���� ��������
			int nFirstCol, nLastCol;
			// ���������� ����� ������� � ���������� ����������������� �������
			RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
			nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
			nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
			// ���������� �������� ������� � ���� �����
			int nFirstRow, nLastRow;
			// ���������� ����� ������� � ���������� ����������������� �������
			RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
			nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
			nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
			// ������� ���������
			if(!mPrevSelRangeCell.IsEmpty())
				for(int i = nFirstRow; i <= nLastRow; i++)
					for(int j = nFirstCol; j <= nLastCol; j++)
						if(!m_SelRangeCell.InRange(j,i))
							UpdateCell(pDC, j, i);
		}

		// ��������� ���������� �������� ������
		UpdateCell(pDC, nPrevActiveCol, nPrevActiveRow);
		if(m_CountHeaderRows > 0)
			// ��������� ���������� �������� ������ �����
			UpdateCell(pDC, nPrevActiveCol, m_CountHeaderRows-1);
	}

	// ���� ����������, ������������ ���� ���, ��� ����� ���������
	// ������ (�������) ���� �����.
	
	// ���� ��������� ���������
	if(m_CurScrollPos.x != HPos)
	{
		CPoint pt(HPos,m_CurScrollPos.y);
		ScrollToPos(pt);
	}

	// ���� ����� �������������� ������
	if(RedrawCells)
	{

		if(!bRowSelect)
			// ��������� �������� ������
			UpdateCell(pDC, m_nActiveCol, m_nActiveRow);
		else
			// ��������� �������� ������
			UpdateRow(pDC,m_nActiveRow);

		if(m_CountHeaderRows > 0)
			// ��������� �������� ������ �����
			UpdateCell(pDC, m_nActiveCol, m_CountHeaderRows-1);
	}

	// ����������� �������� ����������
	m_pGridWnd->ReleaseDC(pDC);

	return;
}

void CALXGridCore::ScrollAndSetActRow(int VPos, int nRow, BOOL RedrawCells /*= TRUE*/)
{
	if(nRow < m_CountHeaderRows || nRow >= GetAllRowCount())
		return;
	VPos = min(VPos,m_sizeTotal.cy - m_sizePage.cy + 1);
	VPos = VPos > 0 ? VPos : 0;

	int nPrevActiveCol = m_nActiveCol;
	int nPrevActiveRow = m_nActiveRow;
	BOOL bRowSelect = (m_pGridWnd->GetStyle() & AGS_ROWSELECT);

	ALXSelRangeCell mPrevSelRangeCell = m_SelRangeCell;

	SaveModifyed();

	m_nActiveRow = nRow;

	m_nActiveSelCol = m_nActiveCol;
	m_nActiveSelRow = m_nActiveRow;
	
	m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
	m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl != NULL)
		SetDataCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);

	// �������� �������� ����������
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// ���� ����� �������������� ������
	if(RedrawCells)
	{
		if(bRowSelect)
			// ��������� ���������� ������
			UpdateRow(pDC,nPrevActiveRow);
		else
		{
			m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
			m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

			// ���� ����� �������� �������� �����
			if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
			{
				// ������� ��������� � ����������� ��������� ���������� �����
				CRect rectGrid = GetGridRect(rectClient);
				// ���������� �������� ������� � ���� ��������
				int nFirstCol, nLastCol;
				// ���������� ����� ������� � ���������� ����������������� �������
				RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
				nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
				nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
				// ���������� �������� ������� � ���� �����
				int nFirstRow, nLastRow;
				// ���������� ����� ������� � ���������� ����������������� �������
				RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
				nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
				nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
				// ������� ���������
				if(!mPrevSelRangeCell.IsEmpty())
					for(int i = nFirstRow; i <= nLastRow; i++)
						for(int j = nFirstCol; j <= nLastCol; j++)
							if(!m_SelRangeCell.InRange(j,i))
								UpdateCell(pDC, j, i);
			}

			// ��������� ���������� �������� ������
			UpdateCell(pDC, nPrevActiveCol, nPrevActiveRow);
			if(m_CountFrozenCols > 0)
				// ��������� ���������� �������� ������ ����������� �������
				for(int i = 0; i < m_CountFrozenCols; i++)
					UpdateCell(pDC, i, nPrevActiveRow);
		}
	}

	// ���� ����������, ������������ ���� ���, ��� ����� ���������
	// ������ (�������) ���� �����.

	// ���� ��������� ���������
	if(m_CurScrollPos.y != VPos)
	{
		CPoint pt(m_CurScrollPos.x,VPos);
		ScrollToPos(pt);
	}
	
	if(RedrawCells)
	{// ���� ����� �������������� ������

		if(bRowSelect)
			// ��������� �������� ������
			UpdateRow(pDC,m_nActiveRow);
		else
		{
			// ��������� �������� ������
			UpdateCell(pDC, m_nActiveCol, m_nActiveRow);
			if(m_CountFrozenCols > 0)
				// ��������� �������� ������ ����������� �������
				UpdateCell(pDC, m_CountFrozenCols-1, m_nActiveRow);
		}
	}
	
	// ����������� �������� ����������
	m_pGridWnd->ReleaseDC(pDC);

	return;
}

void CALXGridCore::ScrollAndSetActCell(int nCol, int nRow, BOOL RedrawCells /*= TRUE*/)
{
	int nPrevActiveCol = m_nActiveCol;
	int nPrevActiveRow = m_nActiveRow;

	// ���� ����������, ������������ ���� ���,
	// ��� ����� ��������� ������ (�������) ���� �����.
	CPoint pt(m_CurScrollPos);
	BOOL bNeedToVScroll = TRUE;
	BOOL bNeedToHScroll = TRUE;

	nCol += m_CountFrozenCols;
	nRow += m_CountHeaderRows;

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// ������� ��������� ����� ?
	if(	LeftWndPos(nCol) < m_CountFrozenCols * m_FrozenColWidth - offsetXY)
	{
		// ����� ��������� �������� �� ����� ��� ����� �����������.
		// ���� ����������, ��� ����� �� ��� ����� ������ ������� ��������.
		pt.x = LeftColPos(nCol) - m_FrozenColWidth * m_CountFrozenCols;
	}
	else if ( LeftWndPos(nCol + 1) - 1 > rectClient.right - offsetXY)
	{
		// ����� ��������� �������� �� ����� ��� ����� �����������.
		// ���� ����������, ��� ����� �� ��� ����� ��������� ������� ��������.
		pt.x = min(LeftColPos(nCol) - m_FrozenColWidth * m_CountFrozenCols, LeftColPos(nCol + 1) - rectClient.Width()) - offsetXY + 1;
	}
	else
		bNeedToHScroll = FALSE;

	// ������ ��������� ������ ?
	if(	TopWndPos(nRow) < m_CountHeaderRows * m_nHeadRowHeight - offsetXY)
	{
		// ����� ��������� ������ �� ������ ��� ������ �����������.
		// ���� ����������, ��� ����� ��� ���� ����� ������ ������� �������.
		pt.y = TopRowPos(nRow) - m_nHeadRowHeight * m_CountHeaderRows;
	}
	else if ( TopWndPos(nRow + 1) - 1 > rectClient.bottom - offsetXY)
	{
		// ����� ��������� ������ �� ������ ��� ������ �����������.
		// ���� ����������, ��� ����� ��� ���� ����� ��������� ������� �������.
		pt.y = min(TopRowPos(nRow) - m_nHeadRowHeight * m_CountHeaderRows, m_CountHeaderRows * m_nHeadRowHeight + (nRow - m_CountHeaderRows + 1) * m_nRowHeight - rectClient.Height()) - offsetXY + 1;
	}
	else
		bNeedToVScroll = FALSE;

	// ���� ��������� ���������
	if(bNeedToHScroll || bNeedToVScroll)
		ScrollToPos(pt);

	ScrollAndSetActCol(pt.x, nCol, RedrawCells);
	ScrollAndSetActRow(pt.y, nRow, RedrawCells);

	OnSetActiveCell(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));

	if(m_nActiveRow != nPrevActiveRow)
		OnChangeActiveRow(GetGridRow(m_nActiveRow));
}

void CALXGridCore::ScrollToCol(int HPos, int nCol)
{
	if(nCol < m_CountFrozenCols || nCol >= GetAllColCount())
		return;

	HPos = min(HPos,m_sizeTotal.cx - m_sizePage.cx + 1);
	HPos = HPos > 0 ? HPos : 0;

	// ���� ����������, ������������ ���� ���, ����� ��������
	// ������ (�������) ���� �����.
	
	// ���� ��������� ���������
	if(m_CurScrollPos.x != HPos)
	{
		CPoint pt(HPos,m_CurScrollPos.y);
		ScrollToPos(pt);
	}

	return;
}

void CALXGridCore::ScrollToRow(int VPos, int nRow)
{
	if(nRow < m_CountHeaderRows || nRow >= GetAllRowCount())
		return;
	VPos = min(VPos,m_sizeTotal.cy - m_sizePage.cy + 1);
	VPos = VPos > 0 ? VPos : 0;

	// ���� ����������, ������������ ���� ���, ����� ��������
	// ������ (�������) ���� �����.
	
	// ���� ��������� ���������
	if(m_CurScrollPos.y != VPos)
	{
		CPoint pt(m_CurScrollPos.x,VPos);
		ScrollToPos(pt);
	}

	return;
}

// ������� ��������� ��������� ��������� � �������� �������� ����������
// ������� � �������������� ���� ������� ����������.
BOOL CALXGridCore::SaveModifyed(BOOL bSkipErrors /*= TRUE*/)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl != NULL)
	{
		if(pCellCtrl->IsModifyed())
		{// ���� ���� ���������
			if(pCellCtrl->OnValidate())
			{// ���� ��������� ���������
				if(OnSaveCellData(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow)))
				{// ���� ��������� ���������
					pCellCtrl->SetModify(FALSE);
					pCellCtrl->DeactivateCtrl();
					return TRUE;
				}
				else if(bSkipErrors)
				{// ���� ����� ��������������� ������
					pCellCtrl->SetModify(FALSE);
					pCellCtrl->DeactivateCtrl();
					return FALSE;
				}
				else
					return FALSE;
			}
			else if(bSkipErrors)
			{// ���� ����� ��������������� ������
				pCellCtrl->SetModify(FALSE);
				pCellCtrl->DeactivateCtrl();
				return FALSE;
			}
			else
				return FALSE;
		}
		pCellCtrl->DeactivateCtrl();
	}
	return TRUE;
}

// ������� ������ ������� � ������ �����
// ������� �� �������� ������������� ������.
void CALXGridCore::DrawArrow(CDC *pDC, CPoint pos, CSize sz)
{
	if(sz.cx <= 3 || sz.cy <= 3)
		return;

	CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	CPen* pOldPen;


	if(m_nSortOrder == Descending)		
	{
		// Draw triangle pointing downwords
		pOldPen = pDC->SelectObject(&penShadow);
		pDC->MoveTo(pos.x + sz.cx,	pos.y);
		pDC->LineTo(pos.x, pos.y);
		pDC->LineTo(pos.x + sz.cx/2, pos.y + sz.cy);
		pDC->SelectObject(&penLight);
		pDC->MoveTo(pos.x + sz.cx/2 + 1, pos.y + sz.cy - 1);
		pDC->LineTo(pos.x + sz.cx, pos.y);
	}
	else		
	{
		// Draw triangle pointing upwards
		pOldPen = pDC->SelectObject(&penLight);
		pDC->MoveTo(pos.x,	pos.y + sz.cy);
		pDC->LineTo(pos.x + sz.cx, pos.y + sz.cy);
		pDC->LineTo(pos.x + sz.cx/2 + 1, pos.y);		
		pDC->SelectObject(&penShadow);
		pDC->MoveTo(pos.x,	pos.y + sz.cy - 1);
		pDC->LineTo(pos.x + sz.cx/2,pos.y);
	}
	// Restore the pen
	pDC->SelectObject( pOldPen );	
}

// ������� ������ ������ ����������� �������
void CALXGridCore::DrawFrozenBtn(CDC *pDC, int x, int y, int cx, int cy, BOOL bCellPressed, BOOL bDrawArrow, BOOL bFlat)
{
	// �������� ������ ���������
	CRect rectCell(x,y,max(x,x+cx),max(y,y+cy));
	// �������� ���� �����
	COLORREF clrCellFrame = ::GetSysColor(bFlat ? COLOR_BTNSHADOW : COLOR_3DDKSHADOW);

	// ������ �����
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,0,0);

	//
	if(rectCell.Height() <= 2 || rectCell.Width() <= 2 )
	{
		CBrush brushBackground;
		// fill cell
		rectCell.DeflateRect(0,0,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
	}	 // if active row && first from right frozen cell
	else if(bDrawArrow) 
	{
		// if not press
		if(!bCellPressed)
		{
			if(bFlat)
			{
				pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
				CBrush brushBackground;
				// fill cell
				rectCell.DeflateRect(1,1,1,1);
				if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
					pDC->FillRect(&rectCell, &brushBackground);
				rectCell.DeflateRect(1,1,1,1);
			}
			else
			{
				// draw button
				pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
				rectCell.DeflateRect(2,2,2,2);
			}
		}
		else
		{
			CBrush brushBackground;
			// fill cell
			rectCell.DeflateRect(0,0,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
			rectCell.DeflateRect(2,1,0,0);
		}
		// draw arrow
		CBrush brushBlack;
		if(brushBlack.CreateSolidBrush(::GetSysColor(COLOR_WINDOWFRAME)))
		{
			POINT ptTriangle[4];
			CRgn rgnTriangle;
			ptTriangle[0].x = rectCell.left + (rectCell.Width()-4)/2;
			ptTriangle[0].y = rectCell.top+(rectCell.Height()-9)/2;
			ptTriangle[1].x = ptTriangle[0].x+4;
			ptTriangle[1].y = ptTriangle[0].y+4;
			ptTriangle[2].x = ptTriangle[0].x;
			ptTriangle[2].y = ptTriangle[0].y+8;
			rgnTriangle.CreatePolygonRgn(ptTriangle,3,WINDING);
			pDC->FillRgn(&rgnTriangle,&brushBlack);
		}
	}
	else
	{
		// if not press
		if(!bCellPressed)
		{
			if(bFlat)
			{
				pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
				CBrush brushBackground;
				// fill cell
				rectCell.DeflateRect(1,1,1,1);
				if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
					pDC->FillRect(&rectCell, &brushBackground);
				rectCell.DeflateRect(1,1,1,1);
			}
			else
			{
				// draw button
				pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
				rectCell.DeflateRect(2,2,2,2);
			}
		}
		else
		{
			CBrush brushBackground;
			// fill cell
			rectCell.DeflateRect(0,0,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
		}
	}
}

void CALXGridCore::SetSelectRange(int nLeftCol, int nRightCol, int nTopRow, int nButtonRow)
{
	// ���� ������ �������� �������� �����
	if(!(m_pGridWnd->GetStyle() & AGS_RANGESELECT))
		return;

	SaveModifyed(FALSE);

	ALXSelRangeCell mPrevSelRangeCell = m_SelRangeCell;

	m_pGridWnd->SetFocus();

	// �������� �������� ����������
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// ����� ��������
	m_SelRangeCell.m_LeftCol = nLeftCol;
	m_SelRangeCell.m_RightCol = nRightCol;
	m_SelRangeCell.m_TopRow = nTopRow;
	m_SelRangeCell.m_BottomRow =  nButtonRow;
	// ���� ������ �������
	if(m_SelRangeCell.IsEmpty())
	{
		m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
		m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;
	}

	m_nActiveSelCol = m_SelRangeCell.m_LeftCol;
	m_nActiveSelRow = m_SelRangeCell.m_TopRow;

	// ����� �������� �������
	if(m_nActiveCol > m_SelRangeCell.m_RightCol)
		m_nActiveCol = m_SelRangeCell.m_RightCol;
	else if(m_nActiveCol < m_SelRangeCell.m_LeftCol)
		m_nActiveCol = m_SelRangeCell.m_LeftCol;
	// ����� �������� ������
	if(m_nActiveRow > m_SelRangeCell.m_BottomRow)
		m_nActiveRow = m_SelRangeCell.m_BottomRow;
	else if(m_nActiveRow < m_SelRangeCell.m_TopRow)
		m_nActiveRow = m_SelRangeCell.m_TopRow;

	// ������� ��������� � ����������� ��������� ���������� �����
	CRect rectGrid = GetGridRect(rectClient);
	// ���������� �������� ������� � ���� ��������
	int nFirstCol, nLastCol;
	// ���������� ����� ������� � ���������� ����������������� �������
	RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
	nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
	nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
	// ���������� �������� ������� � ���� �����
	int nFirstRow, nLastRow;
	// ���������� ����� ������� � ���������� ����������������� �������
	RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
	nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
	// ������� ���������
	if(!mPrevSelRangeCell.IsEmpty())
		for(int i = nFirstRow; i <= nLastRow; i++)
			for(int j = nFirstCol; j <= nLastCol; j++)
				if(!m_SelRangeCell.InRange(j,i))
					UpdateCell(pDC, j, i);

// ��������
	SetSelectCell(m_nActiveSelCol, m_nActiveSelRow);

//	m_pGridWnd->Invalidate();
	// ����������� �������� ����������
	m_pGridWnd->ReleaseDC(pDC);
}

