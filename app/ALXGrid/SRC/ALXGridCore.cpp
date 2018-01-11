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

// Функция рисует элемент управления в ячейке
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

// Конструктор
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

	// указатель на класс окна
	m_pGridWnd = NULL;

	m_nRowHeight = 16;
	m_nHeadRowHeight = m_nRowHeight;
	m_FrozenColWidth = 8;

	// структура определяющая шрифт, используемый сеткой
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

	// Диапозон выделенных ячеек
	m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
	m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

	m_nActiveSelCol = m_nActiveCol;
	m_nActiveSelRow = m_nActiveRow;
}

// Деструктор
CALXGridCore::~CALXGridCore()
{
	if(m_pCellCtrl != NULL)
		delete m_pCellCtrl;
	if(m_curHResize != NULL)
		DestroyCursor(m_curHResize);
}

// Функция рисует содержание окна.
void CALXGridCore::OnGridDraw(CDC *pDC)
{
	CRect rectClip;
	// Получаем область перерисовки.
	pDC->GetClipBox(&rectClip); 
	// Окно необходимо перерисовать ?
	if (rectClip.IsRectEmpty())
		return;

	OnUpdateRect(pDC,rectClip);
}

// Функция преобразует область окна в область сетки.
CRect CALXGridCore::GetGridRect(CRect& rectPaint)
{
	CRect rectGrid = rectPaint;
	rectGrid.OffsetRect(m_CurScrollPos.x, m_CurScrollPos.y);
	return rectGrid;
}

// Функция преобразует область окна в область шапки.
CRect CALXGridCore::GetHeadRect(CRect& rectPaint)
{
	CRect rectHead = rectPaint;
	rectHead.OffsetRect(m_CurScrollPos.x,0); 
	return rectHead;
}

// Функция преобразует область окна в область неподвижных столбцов.
CRect CALXGridCore::GetFrozenRect(CRect& rectPaint)
{
	CRect rectFrozen = rectPaint;
	rectFrozen.OffsetRect(0, m_CurScrollPos.y); 
	return rectFrozen;
}

// Функция передает указатель на окно.
void CALXGridCore::SetGridWnd(CWnd *pGridWnd)
{
	m_pGridWnd = pGridWnd;
}

// Функция обновления области окна.
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
	// Определяется какие области должны быть обновлены

	// Если нужно перерисовывать шапку
	if(m_CountHeaderRows > 0 && rectClip.top <= m_nHeadRowHeight * m_CountHeaderRows - offsetXY )
	{
		// Если нужно перерисовывать закрепленные слева столбцы
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
		// Если нужно перерисовывать закрепленные слева столбцы
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

	// Если высота окна больше высоты сетки
	if(rectClip.bottom > nGridHeight)
	{
		// Если ширина окна больше ширины сетки
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
		// Если ширина окна больше ширины сетки
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

// Функция определяет диапазон строк попадающих в данную область.
// Параметр bIncludePartiallyShownRows определяет нужно ли включать
// строки частично попавшие в область.
void CALXGridCore::RectToRowRange(const CRect &GridRect, int &nFirstRow, int &nLastRow, BOOL bIncludePartiallyShownRows)
{
	CRect rect(GridRect);
	// смещаем координаты	
	rect.DeflateRect(offsetXY,offsetXY,-offsetXY,-offsetXY);
	// вычисляем диапазон
	int nRounding = bIncludePartiallyShownRows ? 0 : m_nRowHeight;
	// вычисляем высоту шапки
	int nHeadHeight = m_CountHeaderRows * m_nHeadRowHeight;

	// вычисляем диапазон ячеек
	if(rect.top <= nHeadHeight)
		nFirstRow = max(0,(rect.top + nRounding) / m_nHeadRowHeight);
	else
		nFirstRow = max(m_CountHeaderRows, min(m_CountHeaderRows + (rect.top - nHeadHeight + nRounding) / m_nRowHeight,GetAllRowCount() - 1));

	if(rect.bottom <= nHeadHeight)
		nLastRow = max(0, min((rect.bottom - nRounding) / m_nHeadRowHeight,m_CountHeaderRows - 1));
	else
		nLastRow = max(m_CountHeaderRows, min(m_CountHeaderRows + (rect.bottom - nHeadHeight - nRounding) / m_nRowHeight,GetAllRowCount() - 1));
}

// Функция определяет диапазон столбцов попадающих в данную область.
// Параметр bIncludePartiallyShownRows определяет нужно ли включать
// столбцы частично попавшие в область.
void CALXGridCore::RectToColRange(const CRect &GridRect, int &nFirstCol, int &nLastCol, BOOL bIncludePartiallyShownCols, int nHeadRow)
{
	CRect rect(GridRect);
	// смещаем координаты	
	rect.DeflateRect(offsetXY,offsetXY,-offsetXY,-offsetXY);
	// вычисляем диапазон
	nFirstCol = 0;
	int curwidth;
	// Выводит диапазон всех столбцов охваченных областью rect
	if(bIncludePartiallyShownCols)
	{
		curwidth = GetColWidth(nFirstCol, nHeadRow);
		while(rect.left >= curwidth && nFirstCol<GetAllColCount(nHeadRow)-1)
			curwidth = curwidth + GetColWidth(++nFirstCol, nHeadRow);

		nLastCol = nFirstCol;

		while(rect.right > curwidth && nLastCol<GetAllColCount(nHeadRow)-1)
			curwidth = curwidth + GetColWidth(++nLastCol, nHeadRow);
	}
	// Выводит диапазон столбцов, которые целиком охвачены областью rect
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

// Виртуальная функция. Она должна возвращать количество строк
// в сетке без учета строк шапки.
// Эта функция переопределяется в классе CALXGrid.
int CALXGridCore::GetGridRowCount()
{
	return 0;
}

// Виртуальная функция. Она должна возвращать количество столбцов
// в сетке без учета неподвижных столбцов.
// Эта функция переопределяется в классе CALXGrid.
int CALXGridCore::GetHeadColCount(int nHeadRow)
{
	return 0;
}

// Виртуальная функция. Она должна возвращать ширину колонки сетки.
// Параметр nCol должен передаваться с учетом неподвижных слева колонок.
// Эта функция переопределяется в классе CALXGrid.
int CALXGridCore::GetColWidth(int nCol, int nHeadRow)
{
	if(nCol >= m_CountFrozenCols)
		return GetHeadColWidth(GetGridCol(nCol), nHeadRow);
	else
		return ( nCol < 0 ? 0 : m_FrozenColWidth);
}

// Виртуальная функция. Выводит кнопку на
// пересечении ячеек шапки и ячеек неподвижных колонок.
// Кнопка - выделить все.
void CALXGridCore::OnDrawFrozenHeaderBtn(CDC *pDC, CRect &rectCell)
{
	BOOL bFlat = (m_pGridWnd->GetStyle() & AGS_FLAT);
	COLORREF clrCellFrame = ::GetSysColor(bFlat ? COLOR_BTNSHADOW : COLOR_WINDOWFRAME);

	// смещаем координаты ячейки	
	rectCell.DeflateRect(-offsetXY,-offsetXY,offsetXY,offsetXY);
	// выводим рамку
	rectCell.InflateRect(1,1,0,0);
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,0,0);
	// Если в оставшуюся область ячейки кнопка не влезет
	if(rectCell.Height() <= 2 || rectCell.Width() <= 2 || m_bSelAllBtnPressed)
	{
		CBrush brushBackground;
		// закрашиваем ячейку
		rectCell.DeflateRect(0,0,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
		return;
	}
	if(bFlat)
	{
		pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
		CBrush brushBackground;
		// закрашиваем ячейку
		rectCell.DeflateRect(1,1,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
	}
	else
		// рисуем кнопку
		pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
}

// Виртуальная функция. Выводит строку сетки.
// Параметр nRow передается с учетом количества строк шапки.
void CALXGridCore::OnDrawGridRow(CDC *pDC, int nRow, int y, CRect& rectGrid)
{
	int nFirstCol, nLastCol, x;
	// определяем номер первого и последнего перерисовываемого столбца
	RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
	nFirstCol = max(nFirstCol, m_CountFrozenCols);
	// определяем x координату самой левой ячейки
	x = LeftWndPos(nFirstCol);

	// выводится каждая ячейка в области перерисовки
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

// Функция выводит строку шапки.
void CALXGridCore::OnDrawHeadRow(CDC *pDC, int nRow, int y, CRect& rectHeader)
{
	int nFirstCol, nLastCol, x;
	// определяем номер первого и последнего перерисовываемого столбца
	RectToColRange(rectHeader, nFirstCol, nLastCol, TRUE, nRow);
	nFirstCol = max(nFirstCol, m_CountFrozenCols);
	// определяем x координату самой левой ячейки
	x = LeftWndPos(nFirstCol, nRow);

	// выводится каждая ячейка в области перерисовки
	for (int nCol = nFirstCol; nCol <= nLastCol; x = x + GetColWidth(nCol, nRow), nCol++)
		OnDrawHeadCell(pDC,nCol,nRow,x,y);
}

// Функция выводит строку неподвижных слева столбцов.
void CALXGridCore::OnDrawFrozenRow(CDC *pDC, int nRow, int y, CRect& rectFrozen)
{
	int nFirstCol, nLastCol, x;
	// определяем номер первого и последнего перерисовываемого столбца
	RectToColRange(rectFrozen, nFirstCol, nLastCol, TRUE);
	nFirstCol = min(nFirstCol, m_CountFrozenCols - 1);
	nLastCol  = min(nLastCol,  m_CountFrozenCols - 1);
	// определяем x координату самой левой ячейки
	x = LeftColPos(nFirstCol) - offsetXY;

	// выводится каждый столбец в области перерисовки
	for (int nCol = nFirstCol; nCol <= nLastCol; x = x + GetColWidth(nCol), nCol++)
		OnDrawFrozenCell(pDC,nCol,nRow,x,y);
}

// Функция вычисляет новые виртуальные размеры окна сетки
// и обновляет элементы прокрутки.
void CALXGridCore::UpdateScrollSizes()
{
	// вычисляем метрики сетки(ширину и высоту строки)
	CalculateGridMetrics();

	CSize sizeTotal(m_nRowWidth - offsetXY + m_sizeExtArea.cx,(m_nHeadRowHeight * m_CountHeaderRows + m_nRowHeight * GetGridRowCount()) - offsetXY + m_sizeExtArea.cy);

	sizeTotal.cx = max(0,sizeTotal.cx);
	sizeTotal.cy = max(0,sizeTotal.cy);

	// передаем размеры Scroll Bars
	SetScrollSizes(sizeTotal);
	// если нужно меняем текущую позицию
	if(m_CurScrollPos.y + m_sizePage.cy > m_sizeTotal.cy)
		m_CurScrollPos.y = max(0,m_sizeTotal.cy - m_sizePage.cy + 1);
	if(m_CurScrollPos.x + m_sizePage.cx > m_sizeTotal.cx)
		m_CurScrollPos.x = max(0,m_sizeTotal.cx - m_sizePage.cx + 1);
}

// Функция устанавливает новые виртуальные размеры для прокрутки.
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

// Функция обновляет элементы прокрутки
void CALXGridCore::UpdateBars()
{
	// UpdateBars может заставить изменить размеры окна - игнорировать эти изменения
	if (m_bInsideUpdate)
		return;         // Запрещает рекурсивные вызовы

	// Блокировка рекурсий
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// модифицируйте горизонталь, чтобы отразить действительность
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	// ВНИМАНИЕ: вкл./выкл. полос прокрутки приведет к повторным вызывам сообщения 'OnSize'
	ASSERT(m_sizeTotal.cx >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	// allow parent to do inside-out layout first
	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if (pParentWnd != NULL)
	{
		// если родительское окно отреагирует на это сообщение, используется клиентская
		// область вычисленная с учетом полосы прокрутки - иначе вся клиентская область
		if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0,
			(LPARAM)(LPCRECT)&rectClient) != 0)
		{
			// используйте rectClient вместо GetTrueClientSize для
			// вычисления размера клиентской области.
			bCalcClient = FALSE;
		}
	}

	CSize sizeClient;
	CSize sizeSb;
	CRect rect;

	if (bCalcClient)
	{
		// Определяем нужны ли полосы прокрутки и получаем размер рабочей области
		if (!GetTrueClientSize(sizeClient, sizeSb))
		{
			// нет места для полос прокрутки
			m_pGridWnd->GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// если клиентская область видима, то нужно использовать полосы прокрутки
				m_pGridWnd->EnableScrollBarCtrl(SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// родительское окно определяет клиентскую область
		GetScrollBarSizes(sizeSb);
		sizeClient.cx = rectClient.Width();
		sizeClient.cy = rectClient.Height();
	}

	// достаточно места, чтобы добавить полосы прокрутки
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;

	// получаем состояние полосы прокрутки в полученной области
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);

	// first scroll the window as needed
	// прокручиваем окно на сколько это необходимо
	ScrollToPos(ptMove); // также изменяем позицию

	// эта структура, необходима для обновления диапазона страниц полосы прокрутки
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// теперь модифицируем полосы как требуется
	m_pGridWnd->EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	m_pGridWnd->EnableScrollBarCtrl(SB_VERT, needSb.cy);
	// Получаем размер клиентской области окна
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

	// Удаляем блокировку рекурсий
	m_bInsideUpdate = FALSE;
}

// Функция возвращает размеры виртуального окна сетки.
void CALXGridCore::GetScrollBarSizes(CSize &sizeSb)
{
	sizeSb.cx = sizeSb.cy = 0;
	DWORD dwStyle = m_pGridWnd->GetStyle();

	if (m_pGridWnd->GetScrollBarCtrl(SB_VERT) == NULL)
		// по вертикали полосы прокрутки будут перекрывать клиентскую часть окна
		sizeSb.cx = ::GetSystemMetrics(SM_CXVSCROLL);//afxData.cxVScroll;
	if (m_pGridWnd->GetScrollBarCtrl(SB_HORZ) == NULL)
		// по горизонтали полосы прокрутки будут перекрывать клиентскую часть окна
		sizeSb.cy = ::GetSystemMetrics(SM_CXHSCROLL);//afxData.cyHScroll;
}

// Функция возвращает состояние элемента прокрутки.
void CALXGridCore::GetScrollBarState(CSize sizeClient, CSize &needSb, CSize &sizeRange, CPoint &ptMove, BOOL bInsideClient)
{
	// получаем размеры полос прокрутки (частично они находятся в клиентской области)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// достаточно места для полос прокрутки
	sizeRange.cx = (int)((double)(m_sizeTotal.cx + 1 - sizeClient.cx)/m_nHScrollFactor);
	sizeRange.cy = (int)((double)(m_sizeTotal.cy + 1 - sizeClient.cy)/m_nVScrollFactor);
		// > 0 => область должна прокручиваться
	ptMove = m_CurScrollPos;
		// новая позиция прокрутки (начало в текущей позиции)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // возврат к началу координат
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // место необходимое для прокрутки

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // возврат к началу координат
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // место необходимое для прокрутки
	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// нужна горизонтальная полоса прокрутки
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// если текущая позиция прокрутки превышает предел области прокрутки
	if (sizeRange.cx > 0 && ptMove.x/m_nHScrollFactor >= sizeRange.cx)
		ptMove.x = (int)(((double)sizeRange.cx)*m_nHScrollFactor);
	if (sizeRange.cy > 0 && ptMove.y/m_nVScrollFactor >= sizeRange.cy)
		ptMove.y = (int)(((double)sizeRange.cy)*m_nVScrollFactor);

	// теперь получаем окончательные размеры
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, и ptMove можно обновить
}

// Функция прокручивает виртуальное окно сетки до указанной позиции.
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

// Функция возвращает TRUE если нужны полосы прокрутки.
BOOL CALXGridCore::GetTrueClientSize(CSize &size, CSize &sizeSb)
{
	CRect rect;
	m_pGridWnd->GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = m_pGridWnd->GetStyle();

	// сначала получим размер полос прокрутки для этого окна
	GetScrollBarSizes(sizeSb);

	// сначала вычислим предпологаемый размер полосы прокрутки
	// (не учитывается, что полосы прокрутки могут быть вкл\выкл)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// по вертикали полосы прокрутки будут перекрывать клиентскую часть окна
		size.cx += sizeSb.cx;   // будет вкл. - скорректируем
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// по горизонтали полосы прокрутки будут перекрывать клиентскую часть окна
		size.cy += sizeSb.cy;   // будет вкл. - скорректируем
	}

	// возвращает TRUE если достаточно места
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

// Функция определяет верхнюю позицию данной строки.
// Параметр nRow должен передаваться с учетом колонок шапки.
int CALXGridCore::TopRowPos(int nRow)
{
	if(nRow > m_CountHeaderRows)
		return m_CountHeaderRows * m_nHeadRowHeight + (nRow - m_CountHeaderRows) * m_nRowHeight;
	else
		return nRow * m_nHeadRowHeight;
}

// Функция определяет левую позицию данной колонки.
// Параметр nCol должен передаваться с учетом неподвижных колонок слева.
int CALXGridCore::LeftColPos(int nCol, int nHeadRow)
{
	int iWidth = 0;
	for(int i = 0; i < nCol; i++)
		iWidth = iWidth + GetColWidth(i, nHeadRow);
	return iWidth;
}

// Функция определяет левую позицию данной колонки в окне.
// Параметр nCol должен передаваться с учетом неподвижных колонок слева.
int CALXGridCore::LeftWndPos(int nCol, int nHeadRow)
{
	return (LeftColPos(nCol, nHeadRow) - m_CurScrollPos.x - offsetXY);
}

// Функция определяет верхнюю позицию данной строки.
// Параметр nRow должен передаваться с учетом строк в шапке.
int CALXGridCore::TopWndPos(int nRow)
{
	return (TopRowPos(nRow) - m_CurScrollPos.y - offsetXY);
}

// Виртуальная функция. Выводит область сетки в окне.
void CALXGridCore::OnUpdateGridRect(CDC *pDC, CRect &rectClip)
{
	int nFirstRow, nLastRow, y;
	CRect rectGrid = GetGridRect(rectClip);
	// определяем номера первой и последней перерисовываемой строки
	RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, m_CountHeaderRows);
	nLastRow = min(nLastRow, GetGridRowCount() - 1 + m_CountHeaderRows);
	// определяем y координату самой верхней ячейки
	y = TopWndPos(nFirstRow);

	// выводится каждая строка в области перерисовки.
	for (int nRow = nFirstRow; nRow <= nLastRow; y += m_nRowHeight, nRow++)
		OnDrawGridRow(pDC,nRow,y,rectGrid);
}

// Функция обновления области окна принадлежащей неподвижным столбцам.
void CALXGridCore::OnUpdateFrozenRect(CDC *pDC, CRect &rectClip)
{
	int nFirstRow, nLastRow, y;
	CRect rectFrozen = GetFrozenRect(rectClip);
	// определяем номера первой и последней перерисовываемой строки
	RectToRowRange(rectFrozen, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, m_CountHeaderRows);
	nLastRow = min(nLastRow, GetGridRowCount() - 1 + m_CountHeaderRows);
	// определяем y координату верхней ячейки
	y = TopWndPos(nFirstRow);

	// выводится каждая строка в области перерисовки,
	for (int nRow = nFirstRow; nRow <= nLastRow; y += m_nRowHeight, nRow++)
		OnDrawFrozenRow(pDC,nRow,y,rectFrozen);
}

// Функция обновления(перерисовки) области окна
// не относящейся ни к шапке ни к неподвижным столбцам.
void CALXGridCore::OnUpdateFrozenHeaderRect(CDC *pDC, CRect &rectCell)
{
	OnDrawFrozenHeaderBtn(pDC, rectCell);
}

// Функция обновления области окна принадлежащей шапке.
void CALXGridCore::OnUpdateHeaderRect(CDC *pDC, CRect &rectClip)
{
	int nFirstRow, nLastRow, y;
	CRect rectHeader = GetHeadRect(rectClip);
	// определяем номера первой и последней перерисовываемой строки
	RectToRowRange(rectHeader, nFirstRow, nLastRow, TRUE);
	nFirstRow = min(nFirstRow,m_CountHeaderRows - 1);
	nLastRow = min(nLastRow, m_CountHeaderRows - 1);
	// определяем y координату самой верхней ячейки
	y = TopRowPos(nFirstRow) - offsetXY;

	CFont* pOldFont = NULL;
	if(m_pHeadFont != &m_Font)
		pOldFont = pDC->SelectObject(m_pHeadFont);

	// выводится каждая строка в области перерисовки,
	for (int nRow = nFirstRow; nRow <= nLastRow; y += m_nHeadRowHeight, nRow++)
		OnDrawHeadRow(pDC,nRow,y,rectHeader);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);
}

// Функция прокручивает окно сетки.
void CALXGridCore::ScrollWindow(int xAmount, int yAmount)
{
	CRect rectClient;
	m_pGridWnd->GetClientRect(&rectClient);
	// Если у сетки нет заголовка и неподвижных столбцов
	if(m_CountHeaderRows == 0 && m_CountFrozenCols == 0)
	{
		m_pGridWnd->ScrollWindow(xAmount, yAmount);
		return;
	}
	// Если сдвиг по оси y
	if(yAmount != 0 )
	{
		if(xAmount != 0)// если сдвиг по оси x и по оси у
		{
			rectClient.left	=	m_CountFrozenCols * m_FrozenColWidth - offsetXY + 1;
			m_pGridWnd->ScrollWindow(xAmount, 0,NULL,&rectClient);
			rectClient.left	=	0;
			rectClient.top	=	m_CountHeaderRows * m_nHeadRowHeight - offsetXY + 1;
			m_pGridWnd->ScrollWindow(0, yAmount,NULL,&rectClient);
		}
		else			// если сдвиг только по оси у
		{
			rectClient.top	=	m_CountHeaderRows * m_nHeadRowHeight - offsetXY + 1;
			m_pGridWnd->ScrollWindow(xAmount, yAmount,NULL,&rectClient);
		}
	}
	else
	{
		if(xAmount != 0)// если сдвиг только по оси x
		{
			rectClient.left	=	m_CountFrozenCols * m_FrozenColWidth - offsetXY + 1;
			m_pGridWnd->ScrollWindow(xAmount, yAmount,NULL,&rectClient);
		}
		else
			m_pGridWnd->ScrollWindow(xAmount, yAmount);
	}
}

// Функция прокручивает окно сетки.
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

// Функция прокручивает окно сетки.
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

// Функция рисует ячейку сетки.
void CALXGridCore::OnDrawGridCell(CDC *pDC, int nCol, int nRow, int x, int y)
{
	CELL_INFO CellInfo;
	CELL_DATA CellData;
	CALXCellCtrl* pCellCtrl;
	DWORD dwStyle = m_pGridWnd->GetStyle();

	int nGridCol = GetGridCol(nCol); // индекс колонки без учета неподвижных колонок
	int nGridRow = GetGridRow(nRow); // индекс строки без учета строк шапки

	// получаем метрики ячейки
	CALXCellMetrics* pCellMetrics = GetCellMetrics(nGridCol, nGridRow);

	ASSERT(pCellMetrics != NULL);

	// определяем активна ли ячейка
	BOOL bActiveCell = (nCol == m_nActiveCol && nRow == m_nActiveRow);
	// определяем выделена ли ячейка
	BOOL bSelCell = (dwStyle & AGS_ROWSELECT) && nRow == m_nActiveRow;
	bSelCell = bSelCell || ((dwStyle & AGS_RANGESELECT) && m_SelRangeCell.InRange(nCol, nRow));

	// определяем цвет фона
	COLORREF clrBackgrnd =  (bActiveCell ? pCellMetrics->m_ActiveCellColor.m_crBackgrnd : (bSelCell ? pCellMetrics->m_SelectCellColor.m_crBackgrnd : pCellMetrics->m_DefaultCellColor.m_crBackgrnd));
	// определяем цвет текста
	COLORREF clrText =  (bActiveCell ? pCellMetrics->m_ActiveCellColor.m_crText : (bSelCell ? pCellMetrics->m_SelectCellColor.m_crText : pCellMetrics->m_DefaultCellColor.m_crText));

	// получаем координаты и размер ячейки
	CRect rectCell(x,y,x+GetColWidth(nCol)+1,y+m_nRowHeight+1);
	// выводим рамку
	if(dwStyle & AGS_NOGRIDLINES)
	{
		COLORREF clrBackgrndHor =  (bSelCell || (bActiveCell && (dwStyle & AGS_ROWSELECT))) ? pCellMetrics->m_SelectCellColor.m_crBackgrnd : pCellMetrics->m_DefaultCellColor.m_crBackgrnd;
		COLORREF clrBackgrndVer =  pCellMetrics->m_DefaultCellColor.m_crBackgrnd;
		pCellMetrics->m_Borders.DrawBorders(pDC, rectCell,clrBackgrndHor,clrBackgrndVer,clrBackgrndHor,clrBackgrndVer,AOD_HOR_VERT);
	}
	else
		pCellMetrics->m_Borders.DrawBorders(pDC, rectCell);

	rectCell.DeflateRect(1,1,1,1); // уменьшаем область рисования
	
	if(rectCell.IsRectEmpty())
		return; // - если больше ничего рисовать не нужно

	// получаем информацию о типе ячейки
	CellInfo = GetCellInfo(nGridCol,nGridRow);

	// если ячейка активна
	if(bActiveCell)
	{
		// получаем указатель на элемент управления ячейкой
		pCellCtrl = GetCellCtrl(nGridCol,nGridRow);
		if(pCellCtrl != NULL)
			// получаем информацию о содержании ячейки из элемента управления
			CellData = pCellCtrl->GetCellData();
		else
			// получаем информацию о содержании ячейки из производного класса
			CellData = GetCellData(nGridCol,nGridRow);
	}
	else
		// получаем информацию о содержании ячейки из производного класса
		CellData = GetCellData(nGridCol,nGridRow);


	CBrush brushBackgrnd;
	// стираем ячейку
	if(brushBackgrnd.CreateSolidBrush(clrBackgrnd))
		pDC->FillRect(&rectCell, &brushBackgrnd);

	// если ячейка с изображением
	if(IsImage(nGridCol,nGridRow))
	{
		CPoint pt;
		CSize sz;

		// получаем размер изображения
		GetSizeImage(nGridCol,nGridRow,sz);
		// уменьшаем размер изображения, если требуется
		sz.cx = min(rectCell.Width(),sz.cx);
		sz.cy = min(rectCell.Height(),sz.cy);

		pt.x = rectCell.left+1;
		// смещаем изображение в центр ячейки по оси у
		pt.y = rectCell.top + (rectCell.Height() - sz.cy)/2;

		DrawImage(pDC,nGridCol,nGridRow,pt,sz,bActiveCell);

		// смещаем левую границу вправо
		rectCell.left = min(rectCell.right,rectCell.left+sz.cx+2);
	}

	// если эта ячейка активна
	if(bActiveCell && rectCell.Width() > 0 && rectCell.Height() > 0)
		// выводим рамку "фокуса ввода"
		pCellMetrics->m_FocusRect.DrawFocusRect(pDC, rectCell);

	rectCell.DeflateRect(1,1,1,1); // уменьшаем область рисования

	// устанавливаем цвет текста
	COLORREF clrOldText = pDC->SetTextColor(clrText);

	CFont* pOldFont = NULL;
	if(bActiveCell)
	{
		// если для ячейки определен шрифт
		if(pCellMetrics->pActiveFont != NULL)
			// устанавливаем шрифт
			pOldFont = pDC->SelectObject(pCellMetrics->pActiveFont);
	}	// если для ячейки определен шрифт
	else if(pCellMetrics->pDefaultFont != NULL)
			// устанавливаем шрифт
			pOldFont = pDC->SelectObject(pCellMetrics->pDefaultFont);
	

	if(CellInfo.m_nTypeCtrl > 0)
		// если надо, рисуем элемент управления
		DrawCellCtrl(pDC,rectCell,CellInfo,CellData);
	else
		// выводим текст
		pDC->DrawText(CellData.m_strText, rectCell, CellInfo.m_nFormat);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);

	pDC->SetTextColor(clrOldText);
}

// Виртуальная функция. Рисует ячейку шапки.
void CALXGridCore::OnDrawHeadCell(CDC *pDC, int nCol, int nRow, int x, int y)
{
	BOOL bFlat = (m_pGridWnd->GetStyle() & AGS_FLAT);
	CRect rectCell(x,y,x+GetColWidth(nCol, nRow)+1,y+m_nHeadRowHeight+1);
	COLORREF clrCellFrame = ::GetSysColor(bFlat ? COLOR_BTNSHADOW : COLOR_3DDKSHADOW);

	// выводим рамку
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);

	// Если в оставшуюся область ячейки кнопка не влезет или ячейка нажата
	if((rectCell.Height() <= 3 || rectCell.Width() <= 3) || (nCol == m_nHeaderColPressed && nRow == m_nHeaderRowPressed))
	{
		CBrush brushBackground;
		// закрашиваем ячейку
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
			// закрашиваем ячейку
			rectCell.DeflateRect(1,1,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
			rectCell.DeflateRect(2,0,2,1);
		}
		else
		{
			// рисуем кнопку
			pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
			rectCell.DeflateRect(3,1,3,2);
		}
	}


	if(!rectCell.IsRectEmpty())
	{
		// получаем текст и стиль 
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
			// если строка и стрелка полностью помещаются в ячейке
			if(rc.Width() + nGap + nArrowW + nArrowMargin < rectCell.Width())
			{
				// выводим текст
				pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
				// рисуем стрелку слева от текста
				DrawArrow(pDC,CPoint(rectCell.right - rc.Width() - nGap - nArrowW, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
			else
			{
				CRect rectClipCell;
				rectClipCell.top = rectCell.top;
				rectClipCell.bottom = rectCell.bottom;
				rectClipCell.left = rectCell.left + (nArrowMargin + nArrowW + nGap);
				rectClipCell.right = rectCell.right;
				// выводим текст
				pDC->DrawText(CellInfo.m_strText, rectClipCell, CellInfo.m_nFormat);
				// рисуем стрелку слева от текста
				DrawArrow(pDC,CPoint(rectCell.left + nArrowMargin, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
		}
		else if(CellInfo.m_nFormat & DT_CENTER)
		{
			// если строка и стрелка полностью помещаются в ячейке
			if((rectCell.Width() - rc.Width())/2 >  nGap + nArrowW + nArrowMargin)
			{
				// выводим текст
				pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
				// рисуем стрелку справа от текста
				DrawArrow(pDC,CPoint(rectCell.left + (rectCell.Width() - rc.Width())/2 + rc.Width() + nGap, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
			else
			{
				CRect rectClipCell;
				rectClipCell.top = rectCell.top;
				rectClipCell.bottom = rectCell.bottom;
				rectClipCell.left = rectCell.left;
				rectClipCell.right = rectCell.right - (nArrowMargin + nArrowW + nGap);
				// выводим текст
				pDC->DrawText(CellInfo.m_strText, rectClipCell, CellInfo.m_nFormat);
				// рисуем стрелку справа от текста
				DrawArrow(pDC,CPoint(rectCell.right - nArrowW - nArrowMargin, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
		}
		else
		{
			// если строка и стрелка полностью помещаются в ячейке
			if(rc.Width() + nGap + nArrowW + nArrowMargin < rectCell.Width())
			{
				// выводим текст
				pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
				// рисуем стрелку справа от текста
				DrawArrow(pDC,CPoint(rectCell.left + rc.Width() + nGap, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
			else
			{
				CRect rectClipCell;
				rectClipCell.top = rectCell.top;
				rectClipCell.bottom = rectCell.bottom;
				rectClipCell.left = rectCell.left;
				rectClipCell.right = rectCell.right - (nArrowMargin + nArrowW + nGap);
				// выводим текст
				pDC->DrawText(CellInfo.m_strText, rectClipCell, CellInfo.m_nFormat);
				// рисуем стрелку справа от текста
				DrawArrow(pDC,CPoint(rectCell.right - nArrowW - nArrowMargin, nArrowTopPos),CSize(nArrowW, nArrowH));
			}
		}
	}
}

// Виртуальная функция. Рисует левую неподвижную ячейку.
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

	// выводим рамку
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,0,0);

	// Если в оставшуюся область ячейки кнопка не влезет
	if(rectCell.Height() <= 2 || rectCell.Width() <= 2 )
	{
		CBrush brushBackground;
		// закрашиваем ячейку
		rectCell.DeflateRect(0,0,1,1);
		if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
			pDC->FillRect(&rectCell, &brushBackground);
		return;
	}

	// Если эта строка активна и это самая правая ячейка из неподвижных
	if(m_nActiveRow == nRow && nCol == m_CountFrozenCols - 1)
	{
		// Если ячейка не нажата
		if(nCol != m_nFrozenColPressed || nRow != m_nFrozenRowPressed)
		{
			if(bFlat)
			{
				pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
				CBrush brushBackground;
				// закрашиваем ячейку
				rectCell.DeflateRect(1,1,1,1);
				if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
					pDC->FillRect(&rectCell, &brushBackground);
				rectCell.DeflateRect(1,1,1,1);
			}
			else
			{
				// рисуем кнопку
				pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
				rectCell.DeflateRect(2,2,2,2);
			}
		}
		else
		{
			CBrush brushBackground;
			// закрашиваем ячейку
			rectCell.DeflateRect(0,0,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
			rectCell.DeflateRect(2,1,0,0);
		}
		// рисуем стрелку
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
		// Если ячейка не нажата
		if(nCol != m_nFrozenColPressed || nRow != m_nFrozenRowPressed)
		{
			if(bFlat)
			{
				pDC->Draw3dRect(rectCell,::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
				CBrush brushBackground;
				// закрашиваем ячейку
				rectCell.DeflateRect(1,1,1,1);
				if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
					pDC->FillRect(&rectCell, &brushBackground);
				rectCell.DeflateRect(1,1,1,1);
			}
			else
			{
				// рисуем кнопку
				pDC->DrawFrameControl(rectCell,DFC_BUTTON,DFCS_BUTTONPUSH);
				rectCell.DeflateRect(2,2,2,2);
			}
		}
		else
		{
			CBrush brushBackground;
			// закрашиваем ячейку
			rectCell.DeflateRect(0,0,1,1);
			if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
				pDC->FillRect(&rectCell, &brushBackground);
		}
	}
*/
}

// Функция вычисляет высоту и ширину строки
void CALXGridCore::CalculateGridMetrics()
{
	// Получаем стиль сетки
	DWORD dwStyle = m_pGridWnd->GetStyle();
	// Определяем размер шрифта
	CSize sizeFont = GetFontSize(&m_Font);
	// Определяем высоту строки сетки
	m_nRowHeight = sizeFont.cy * m_nRowLineCount + 3; // 2 lines of text

	// Если высота строки шапки должна вычислятся по размеру шрифта
	if((dwStyle & AGS_CUSTOMHEAD_HEIGHT) == 0)
	{
		// Если шрифты шапки и сетки совпадают
		if(m_pHeadFont != &m_Font)
		{
			ASSERT(m_pHeadFont != NULL);
			// Определяем размер шрифта
			sizeFont = GetFontSize(m_pHeadFont);
			// Определяем высоту строки шапки
			m_nHeadRowHeight = sizeFont.cy + 3;
		}
		else
		// Определяем высоту строки шапки
			m_nHeadRowHeight = m_nRowHeight;
	}

	// Если у сетки есть неподвижные столбцы, определяем ширину неподвижных столбцов
	if(m_CountFrozenCols > 0)
		// Если ширина неподвижных колонок должна вычислятся автоматически
		if((dwStyle & AGS_CUSTOMFROZEN_WIDTH) == 0)
				m_FrozenColWidth = m_nRowHeight/2;
	
	// Определяем ширину сетки
	m_nRowWidth = 0;
	for(int i = 0; i < GetAllColCount(); i++)
		m_nRowWidth = m_nRowWidth + GetColWidth(i);
}

// Функция возвращает размер шрифта.
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

// Функция устанавливает активную ячейку.
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


	// Получаем контекст устройства
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	if(nPrevActiveRow != m_nActiveRow)
		// Обновляем предыдущую активную ячейку неподвижной колонки
		for(int i = 0; i < m_CountFrozenCols; i++)
			UpdateCell(pDC, i, nPrevActiveRow);
	if(bRowSelect && nPrevActiveRow != m_nActiveRow)
		// Обновляем предыдущую строку
		UpdateRow(pDC,nPrevActiveRow);
	else
	{
		m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
		m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

		// если можно выделять диапозон ячеек
		if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
		{
			// снимаем выделение с предыдущего деапозона выделенных ячеек
			CRect rectGrid = GetGridRect(rectClient);
			// определяем диапозон видимых в окне столбцов
			int nFirstCol, nLastCol;
			// определяем номер первого и последнего перерисовываемого столбца
			RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
			nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
			nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
			// определяем диапозон видимых в окне строк
			int nFirstRow, nLastRow;
			// определяем номер первого и последнего перерисовываемого столбца
			RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
			nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
			nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
			// Снимаем выделение
			if(!mPrevSelRangeCell.IsEmpty())
				for(int i = nFirstRow; i <= nLastRow; i++)
					for(int j = nFirstCol; j <= nLastCol; j++)
						if(!m_SelRangeCell.InRange(j,i))
							UpdateCell(pDC, j, i);
		}

		// Обновляем предыдущую активную ячейку
		UpdateCell(pDC, nPrevActiveCol, nPrevActiveRow);
		if(m_CountHeaderRows > 0 && nPrevActiveCol != m_nActiveCol)
			// Обновляем предыдущую активную ячейку шапки
			UpdateCell(pDC, nPrevActiveCol, m_CountHeaderRows-1);
	}


	// Если необходимо, прокручиваем окно так,
	// что новая выбранная строка (столбец) были видны.
	CPoint pt(m_CurScrollPos);
	BOOL bNeedToVScroll = TRUE;
	BOOL bNeedToHScroll = TRUE;

	// Столбец полностью видим ?
	if(	LeftWndPos(m_nActiveCol) < m_CountFrozenCols * m_FrozenColWidth - offsetXY)
	{
		// Новый выбранный столбеца не видим или видим неполностью.
		// Надо прокрутить, так чтобы он был самым первым видимым столбцом.
		pt.x = LeftColPos(m_nActiveCol) - m_FrozenColWidth * m_CountFrozenCols;
	}
	else if ( LeftWndPos(m_nActiveCol + 1) - 1 > rectClient.right - offsetXY)
	{
		// Новый выбранный столбеца не видим или видим неполностью.
		// Надо прокрутить, так чтобы он был самым последним видимым столбцом.
		pt.x = min(LeftColPos(m_nActiveCol) - m_FrozenColWidth * m_CountFrozenCols, LeftColPos(m_nActiveCol + 1) - rectClient.Width()) - offsetXY + 1;
	}
	else
		bNeedToHScroll = FALSE;

	// Строка полностью видима ?
	if(	TopWndPos(m_nActiveRow) < m_CountHeaderRows * m_nHeadRowHeight - offsetXY)
	{
		// Новая выбранная строка не видима или видима неполностью.
		// Надо прокрутить, так чтобы она была самой первой видимой строкой.
		pt.y = TopRowPos(m_nActiveRow) - m_nHeadRowHeight * m_CountHeaderRows;
	}
	else if ( TopWndPos(m_nActiveRow + 1) - 1 > rectClient.bottom - offsetXY)
	{
		// Новая выбранная строка не видима или видима неполностью.
		// Надо прокрутить, так чтобы она была самой последней видимой строкой.
		pt.y = min(TopRowPos(m_nActiveRow) - m_nHeadRowHeight * m_CountHeaderRows, m_CountHeaderRows * m_nHeadRowHeight + (m_nActiveRow - m_CountHeaderRows + 1) * m_nRowHeight - rectClient.Height()) - offsetXY + 1;
	}
	else
		bNeedToVScroll = FALSE;

	// Если скроллинг необходим
	if(bNeedToHScroll || bNeedToVScroll)
		ScrollToPos(pt);
	
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
	else if(m_nActiveCol != nPrevActiveCol || m_nActiveRow != nPrevActiveRow)
		SetDataCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow), pCellCtrl);


	if(nPrevActiveRow != m_nActiveRow || !bNeedToVScroll)
		// Обновляем активную ячейку неподвижной колонки
		for(int i = 0; i < m_CountFrozenCols; i++)
			UpdateCell(pDC, i, m_nActiveRow);
	if(bRowSelect && nPrevActiveRow != m_nActiveRow)
		// Обновляем активную строку
		UpdateRow(pDC,m_nActiveRow);
	else
	{
		// Обновляем активную ячейку
		UpdateCell(pDC, m_nActiveCol, m_nActiveRow);
		if(m_CountHeaderRows > 0 && (nPrevActiveCol != m_nActiveCol || !bNeedToHScroll))
			// Обновляем предыдущую активную ячейку шапки
			UpdateCell(pDC, m_nActiveCol, m_CountHeaderRows-1);
	}

	// Освобождаем контекст устройства
	m_pGridWnd->ReleaseDC(pDC);

	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if(pParentWnd != NULL)
		pParentWnd->SendMessage(GWM_ACTCOLROWCHANGED,(WPARAM)m_pGridWnd->m_hWnd,0);

	ActivateCtrl(m_nActiveCol, m_nActiveRow);

	return TRUE;
}

// Функция устанавливает выделение ячейки.
BOOL CALXGridCore::SetSelectCell(int nCol, int nRow)
{
	if(nCol < m_CountFrozenCols || nRow < m_CountHeaderRows || nCol >= GetAllColCount() || nRow >= GetAllRowCount())
		return FALSE;
	// если нельзя выделять диапозон ячеек
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

	// Получаем контекст устройства
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// получаем диапозон выделенных ячеек
	if(!m_SelRangeCell.IsEmpty())
	{
		// получаем диапозон выделенных колонок
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
		// получаем диапозон выделенных строк
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
		// снимаем выделение с предыдущего деапозона выделенных ячеек
		CRect rectGrid = GetGridRect(rectClient);
		// определяем диапозон видимых в окне столбцов
		int nFirstCol, nLastCol;
		// определяем номер первого и последнего перерисовываемого столбца
		RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
		nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
		nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
		// определяем диапозон видимых в окне строк
		int nFirstRow, nLastRow;
		// определяем номер первого и последнего перерисовываемого столбца
		RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
		nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
		nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
		// Снимаем выделение
		if(!mPrevSelRangeCell.IsEmpty())
			for(int i = nFirstRow; i <= nLastRow; i++)
				for(int j = nFirstCol; j <= nLastCol; j++)
					if(!m_SelRangeCell.InRange(j,i))
						UpdateCell(pDC, j, i);

	// Обновляем предыдущую активную ячейку
	UpdateCell(pDC, nPrevActiveSelCol, nPrevActiveSelRow);
	if(m_CountHeaderRows > 0 && nPrevActiveSelCol != m_nActiveSelCol)
		// Обновляем предыдущую активную ячейку шапки
		UpdateCell(pDC, nPrevActiveSelCol, m_CountHeaderRows-1);

	// Если необходимо, прокручиваем окно так,
	// что новая выбранная строка (столбец) были видны.
	CPoint pt(m_CurScrollPos);
	BOOL bNeedToVScroll = TRUE;
	BOOL bNeedToHScroll = TRUE;

	// Столбец полностью видим ?
	if(	LeftWndPos(m_nActiveSelCol) < m_CountFrozenCols * m_FrozenColWidth - offsetXY)
	{
		// Новый выбранный столбеца не видим или видим неполностью.
		// Надо прокрутить, так чтобы он был самым первым видимым столбцом.
		pt.x = LeftColPos(m_nActiveSelCol) - m_FrozenColWidth * m_CountFrozenCols;
	}
	else if ( LeftWndPos(m_nActiveSelCol + 1) - 1 > rectClient.right - offsetXY)
	{
		// Новый выбранный столбеца не видим или видим неполностью.
		// Надо прокрутить, так чтобы он был самым последним видимым столбцом.
		pt.x = min(LeftColPos(m_nActiveSelCol) - m_FrozenColWidth * m_CountFrozenCols, LeftColPos(m_nActiveSelCol + 1) - rectClient.Width()) - offsetXY + 1;
	}
	else
		bNeedToHScroll = FALSE;

	// Строка полностью видима ?
	if(	TopWndPos(m_nActiveSelRow) < m_CountHeaderRows * m_nHeadRowHeight - offsetXY)
	{
		// Новая выбранная строка не видима или видима неполностью.
		// Надо прокрутить, так чтобы она была самой первой видимой строкой.
		pt.y = TopRowPos(m_nActiveSelRow) - m_nHeadRowHeight * m_CountHeaderRows;
	}
	else if ( TopWndPos(m_nActiveSelRow + 1) - 1 > rectClient.bottom - offsetXY)
	{
		// Новая выбранная строка не видима или видима неполностью.
		// Надо прокрутить, так чтобы она была самой последней видимой строкой.
		pt.y = min(TopRowPos(m_nActiveSelRow) - m_nHeadRowHeight * m_CountHeaderRows, m_CountHeaderRows * m_nHeadRowHeight + (m_nActiveSelRow - m_CountHeaderRows + 1) * m_nRowHeight - rectClient.Height()) - offsetXY + 1;
	}
	else
		bNeedToVScroll = FALSE;

	// Если скроллинг необходим
	if(bNeedToHScroll || bNeedToVScroll)
		ScrollToPos(pt);
	

	rectGrid = GetGridRect(rectClient);
	// определяем диапозон видимых в окне столбцов
	nFirstCol, nLastCol;
	// определяем номер первого и последнего перерисовываемого столбца
	RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
	nFirstCol = max(nFirstCol, m_SelRangeCell.m_LeftCol);
	nLastCol = min(nLastCol, m_SelRangeCell.m_RightCol);
	// определяем диапозон видимых в окне строк
	nFirstRow, nLastRow;
	// определяем номер первого и последнего перерисовываемого столбца
	RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, m_SelRangeCell.m_TopRow);
	nLastRow = min(nLastRow, m_SelRangeCell.m_BottomRow);
	// Обновляем выделенные ячейки
	for(int i = nFirstRow; i <= nLastRow; i++)
		for(int j = nFirstCol; j <= nLastCol; j++)
			UpdateCell(pDC, j, i);

	// Обновляем активную ячейку
	UpdateCell(pDC, m_nActiveSelCol, m_nActiveSelRow);
	if(m_CountHeaderRows > 0 && (nPrevActiveSelCol != m_nActiveSelCol || !bNeedToHScroll))
		// Обновляем предыдущую активную ячейку шапки
		UpdateCell(pDC, m_nActiveSelCol, m_CountHeaderRows-1);

	// Освобождаем контекст устройства
	m_pGridWnd->ReleaseDC(pDC);

	CWnd* pParentWnd = m_pGridWnd->GetParent();
	if(pParentWnd != NULL)
		pParentWnd->SendMessage(GWM_SELCOLROWRANGE,(WPARAM)m_pGridWnd->m_hWnd,0);

	return TRUE;
}

// Функция перерисовывает ячейку сетки.
void CALXGridCore::UpdateCell(CDC* pDC, int nCol, int nRow)
{
	// Определяем область которую нужно обновить
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

	// Если область видима и pDC не NULL
	if(rectCell.right > 0 && rectCell.bottom > 0 && pDC != NULL)
		OnUpdateRect(pDC,rectCell); // Обновляем область окна 
}

// Функция сообщает окну, что область ячейки нужно перерисовать.
void CALXGridCore::InvalidateCell(int nCol, int nRow)
{
	CRect rectCell;
	rectCell.left = LeftWndPos(nCol);
	rectCell.right = rectCell.left + GetColWidth(nCol);
	rectCell.top = TopWndPos(nRow);
	rectCell.bottom = rectCell.top + m_nRowHeight;
	m_pGridWnd->InvalidateRect(rectCell);
}

// Функция перерисовывает строки сетки.
void CALXGridCore::UpdateRow(CDC* pDC, int nRow)
{
	// Определяем область которую нужно обновить
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

	// Если область видима и pDC не NULL
	if(rectCell.right > 0 && rectCell.bottom > 0 && pDC != NULL)
		OnUpdateRect(pDC,rectCell); // Обновляем область окна 
}

// Функция сообщает окну, что область строки нужно перерисовать.
void CALXGridCore::InvalidateRow(int nRow)
{
	CRect rectRow;
	m_pGridWnd->GetClientRect(&rectRow);
	rectRow.top = TopWndPos(nRow);
	rectRow.bottom = rectRow.top + m_nRowHeight;
	m_pGridWnd->InvalidateRect(rectRow);
}

// Функция возвращает TRUE если курсор находится между колонками сетки.
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

// Функция вызывается при завершении изменения ширины колонки
void CALXGridCore::EndResizeCol(int x)
{
	// преобразуем коорд. окна в коорд. сетки
	x = x + m_CurScrollPos.x;

	// получаем новую ширину колонки
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
		// деактивируем активный элемент управления 
		pCellCtrl->DeactivateCtrl();
	}

	// обновляем Scroll Bars
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

// Функция устанавливает активную ячейку сетки.
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

// Функция устанавливает выделенную ячейку.
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

// Функция возвращает номер колонки которой принадлежит координата x.
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

// Функция возвращает номер строки которой принадлежит координата y.
int CALXGridCore::YToRow(int y)
{
	if(y < m_CountHeaderRows * m_nHeadRowHeight)
		return y / m_nHeadRowHeight;
	else
		return m_CountHeaderRows + (y - m_CountHeaderRows * m_nHeadRowHeight) / m_nRowHeight;
}

// Виртуальная функция. Возвращает указатель на элемент управления ячейки.
// Эта функция переопределяется в классе CALXGrid
CALXCellCtrl* CALXGridCore::GetCellCtrl(int nCol, int nRow)
{
	return m_pCellCtrl;
}

// Виртуальная функция. Устанавливает элемент управления для ячейки.
// Эта функция переопределяется в классе CALXGrid
void CALXGridCore::SetCellCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl)
{
}

// Виртуальная функция. Передает данные элементу управления ячейкой.
// Эту функцию можно переопределить в своем пользовательском классе.
void CALXGridCore::SetDataCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl)
{
	pCellCtrl->SetData(GetCellData(nCol,nRow));
}

// Виртуальная функция. Она должна создавать элемент управления для ячейки.
// Эта функция переопределяется в классе CALXGrid
CALXCellCtrl* CALXGridCore::CreateCellCtrl(int nCol, int nRow, DWORD dwStyle, UINT nID)
{
	CALXCellCtrl* pCellCtrl = new CALXCellCtrl;
	pCellCtrl->CreateCtrl(dwStyle,m_pGridWnd,IDC_CELLCTRL+nCol);
	pCellCtrl->EnableCtrl(FALSE);
	pCellCtrl->SetFontCtrl(&m_Font);
	return pCellCtrl;
}

// Виртуальная функция. Она должна возвращать данные ячейки.
// Эта функция должна переопределятся в дочернем классе порожденном от CALXGridView или CALXGridCtrl.
CELL_DATA CALXGridCore::GetCellData(int nCol, int nRow)
{
	CELL_DATA CtrlData;
	CtrlData.m_strText.Format("%u - %u",nCol,nRow);
	return CtrlData;
}

// Виртуальная функция. Она должна возвращать информацию о стиле ячейки.
// Эта функция переопределяется в классе CALXGrid.
CELL_INFO CALXGridCore::GetCellInfo(int nCol, int nRow)
{
	CELL_INFO CellInfo;
	CellInfo.m_nFormat = DT_CENTER | DT_VCENTER;
	CellInfo.m_nTypeCtrl = 0;
	return CellInfo;
}

// Виртуальная функция. Она должна возвращать информацию о стиле элемента управления ячейкой.
// Эта функция переопределяется в классе CALXGrid.
DWORD CALXGridCore::GetCellCtrlStyle(int nCol, int nRow)
{
	return WS_CHILD;
}

// Функция возвращает указатель на окно сетки.
CWnd* CALXGridCore::GetGridWnd()
{
	return m_pGridWnd;
}

// Виртуальная функция. Она определяет метрики ячейки.
// Эта функция может переопределятся в дочернем классе порожденном от CALXGridView или CALXGridCtrl.
CALXCellMetrics* CALXGridCore::GetCellMetrics(int nCol, int nRow)
{
	return &m_MetricsCells;
}

// Виртуальная функция. Она определяет существует ли для ячейки изображение.
// Эта функция переопределяется в классе CALXGrid.
BOOL CALXGridCore::IsImage(int nCol, int nRow)
{
	return FALSE;
}

// Виртуальная функция. Она определяет тип изображения для ячейки.
// Эта функция переопределяется в классе CALXGrid.
UINT CALXGridCore::GetTypeImage(int nCol, int nRow)
{
	return NULL;
}

// Виртуальная функция. Она должна возвращать размер изображение для данной ячейки.
// Эта функция переопределяется в классе CALXGrid.
BOOL CALXGridCore::GetSizeImage(int nCol, int nRow, CSize& sz)
{
	sz.cx = 0;
	sz.cy = 0;
	return FALSE;
}

// Виртуальная функция. Она должна возвращать указатель на изображение.
// Эта функция переопределяется в классе CALXGrid.
LPARAM CALXGridCore::GetImage(int nCol, int nRow, BOOL bSelected)
{
	return NULL;
}

// Виртуальная функция. Она должна возвращать информацию о изображении.
// Эта функция должна переопределятся в дочернем классе порожденном от CALXGridView или CALXGridCtrl.
void CALXGridCore::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{

}

// Устанавливаем стиль и цвет рамки "фокус ввода"
void CALXGridCore::SetFocusRect(COLORREF crFaceColor, COLORREF crBkColor/* = ::GetSysColor(COLOR_WINDOW)*/, BOOL bSolid/* = TRUE*/)
{
	m_MetricsCells.m_FocusRect.m_crFaceColor =	crFaceColor;
	m_MetricsCells.m_FocusRect.m_crBackColor = crBkColor;
	m_MetricsCells.m_FocusRect.m_bSolid = bSolid;
}

// Функция посылает сообщение элементу управления ячейкой о нажатии левой кнопки мыши.
// Используется при активизации элемента управления ячейкой.
void CALXGridCore::SendLButtonDown(UINT nFlags, CPoint point)
{
	CPoint pointClick;
	// преобразуем координаты окна в координаты элемента управления.
	pointClick.x = point.x + m_CurScrollPos.x - LeftColPos(m_nActiveCol);
	pointClick.y = point.y + m_CurScrollPos.y - TopRowPos(m_nActiveRow);
	if(pointClick.x < 0 || pointClick.y < 0)
		return;
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl != NULL)
		pCellCtrl->SendLButtonDown(nFlags, pointClick);
}

// Функция вычисляет и устанавливает позицию
// и размер элемента управления ячейкой.
void CALXGridCore::ActivateCtrl(int nCol, int nRow)
{
	if(nRow < m_CountHeaderRows)
		return;

	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(nCol),GetGridRow(nRow));
	if(pCellCtrl != NULL)
	{
		// Если ячейка с изображением
		if(IsImage(GetGridCol(nCol),GetGridRow(nRow)))
		{
			CSize sz;
			// Получаем размер изображения, чтобы сделать отступ вправо
			GetSizeImage(GetGridCol(nCol),GetGridRow(nRow),sz);
			sz.cx += 2;
			pCellCtrl->ActivateCtrl(LeftWndPos(nCol)+sz.cx,TopWndPos(nRow),GetColWidth(nCol)-sz.cx,m_nRowHeight);
		}
		else
			pCellCtrl->ActivateCtrl(LeftWndPos(nCol),TopWndPos(nRow),GetColWidth(nCol),m_nRowHeight);
	}
}

// Функция выводит рисунок в ячейке.
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

// Функция перерисовывает рамку выделения
void CALXGridCore::RedrawFocusRect()
{
	CRect rectCell,rectRow(0,0,0,0);
	CRect rectClient;
	CRgn rgnGrid;
	int nActGridCol = GetGridCol(m_nActiveCol); // индекс колонки без учета неподвижных колонок
	int nActGridRow = GetGridRow(m_nActiveRow); // индекс строки без учета строк шапки

	m_pGridWnd->GetClientRect(&rectClient);

	rectCell.left = LeftWndPos(m_nActiveCol) + 1;
	rectCell.right = rectCell.left + GetColWidth(m_nActiveCol) - 1;
	rectCell.top = TopWndPos(m_nActiveRow) + 1;
	rectCell.bottom = rectCell.top + m_nRowHeight - 1;

	// Если ячейка с изображением
	if(IsImage(nActGridCol,nActGridRow))
	{
		CSize sz;
		// Получаем размер изображения
		GetSizeImage(nActGridCol,nActGridRow,sz);
		// Уменьшаем размер изображения, если требуется
		sz.cx = min(rectCell.Width(),sz.cx);
		// Смещаем левую границу вправо
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

	// получаем метрики ячейки
	CALXCellMetrics* pCellMetrics = GetCellMetrics(nActGridCol, nActGridRow);

	ASSERT(pCellMetrics != NULL);

	// получаем контекст устройства
	CDC* pDC = m_pGridWnd->GetDC();

	pDC->SelectClipRgn(&rgnGrid);
		pCellMetrics->m_FocusRect.DrawFocusRect(pDC, rectCell);
		if(!rectRow.IsRectEmpty())
			m_MetricsCells.m_FocusRect.DrawFocusRect(pDC, rectRow);
	pDC->SelectClipRgn(NULL);

	// Освобождаем контекст устройства
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

	// Получаем контекст устройства
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// если нужно перерисовывать ячейки
	if(RedrawCells)
	{
		// если можно выделять диапозон ячеек
		if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
		{
			// снимаем выделение с предыдущего деапозона выделенных ячеек
			CRect rectGrid = GetGridRect(rectClient);
			// определяем диапозон видимых в окне столбцов
			int nFirstCol, nLastCol;
			// определяем номер первого и последнего перерисовываемого столбца
			RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
			nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
			nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
			// определяем диапозон видимых в окне строк
			int nFirstRow, nLastRow;
			// определяем номер первого и последнего перерисовываемого столбца
			RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
			nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
			nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
			// Снимаем выделение
			if(!mPrevSelRangeCell.IsEmpty())
				for(int i = nFirstRow; i <= nLastRow; i++)
					for(int j = nFirstCol; j <= nLastCol; j++)
						if(!m_SelRangeCell.InRange(j,i))
							UpdateCell(pDC, j, i);
		}

		// Обновляем предыдущую активную ячейку
		UpdateCell(pDC, nPrevActiveCol, nPrevActiveRow);
		if(m_CountHeaderRows > 0)
			// Обновляем предыдущую активную ячейку шапки
			UpdateCell(pDC, nPrevActiveCol, m_CountHeaderRows-1);
	}

	// Если необходимо, прокручиваем окно так, что новая выбранная
	// строка (столбец) были видны.
	
	// Если скроллинг необходим
	if(m_CurScrollPos.x != HPos)
	{
		CPoint pt(HPos,m_CurScrollPos.y);
		ScrollToPos(pt);
	}

	// если нужно перерисовывать ячейки
	if(RedrawCells)
	{

		if(!bRowSelect)
			// Обновляем активную ячейку
			UpdateCell(pDC, m_nActiveCol, m_nActiveRow);
		else
			// Обновляем активную строку
			UpdateRow(pDC,m_nActiveRow);

		if(m_CountHeaderRows > 0)
			// Обновляем активную ячейку шапки
			UpdateCell(pDC, m_nActiveCol, m_CountHeaderRows-1);
	}

	// Освобождаем контекст устройства
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

	// Получаем контекст устройства
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// если нужно перерисовывать ячейки
	if(RedrawCells)
	{
		if(bRowSelect)
			// Обновляем предыдущую строку
			UpdateRow(pDC,nPrevActiveRow);
		else
		{
			m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
			m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;

			// если можно выделять диапозон ячеек
			if(m_pGridWnd->GetStyle() & AGS_RANGESELECT)
			{
				// снимаем выделение с предыдущего деапозона выделенных ячеек
				CRect rectGrid = GetGridRect(rectClient);
				// определяем диапозон видимых в окне столбцов
				int nFirstCol, nLastCol;
				// определяем номер первого и последнего перерисовываемого столбца
				RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
				nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
				nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
				// определяем диапозон видимых в окне строк
				int nFirstRow, nLastRow;
				// определяем номер первого и последнего перерисовываемого столбца
				RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
				nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
				nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
				// Снимаем выделение
				if(!mPrevSelRangeCell.IsEmpty())
					for(int i = nFirstRow; i <= nLastRow; i++)
						for(int j = nFirstCol; j <= nLastCol; j++)
							if(!m_SelRangeCell.InRange(j,i))
								UpdateCell(pDC, j, i);
			}

			// Обновляем предыдущую активную ячейку
			UpdateCell(pDC, nPrevActiveCol, nPrevActiveRow);
			if(m_CountFrozenCols > 0)
				// Обновляем предыдущую активную ячейку неподвижной колонки
				for(int i = 0; i < m_CountFrozenCols; i++)
					UpdateCell(pDC, i, nPrevActiveRow);
		}
	}

	// Если необходимо, прокручиваем окно так, что новая выбранная
	// строка (столбец) были видны.

	// Если скроллинг необходим
	if(m_CurScrollPos.y != VPos)
	{
		CPoint pt(m_CurScrollPos.x,VPos);
		ScrollToPos(pt);
	}
	
	if(RedrawCells)
	{// если нужно перерисовывать ячейки

		if(bRowSelect)
			// Обновляем активную строку
			UpdateRow(pDC,m_nActiveRow);
		else
		{
			// Обновляем активную ячейку
			UpdateCell(pDC, m_nActiveCol, m_nActiveRow);
			if(m_CountFrozenCols > 0)
				// Обновляем активную ячейку неподвижной колонки
				UpdateCell(pDC, m_CountFrozenCols-1, m_nActiveRow);
		}
	}
	
	// Освобождаем контекст устройства
	m_pGridWnd->ReleaseDC(pDC);

	return;
}

void CALXGridCore::ScrollAndSetActCell(int nCol, int nRow, BOOL RedrawCells /*= TRUE*/)
{
	int nPrevActiveCol = m_nActiveCol;
	int nPrevActiveRow = m_nActiveRow;

	// Если необходимо, прокручиваем окно так,
	// что новая выбранная строка (столбец) были видны.
	CPoint pt(m_CurScrollPos);
	BOOL bNeedToVScroll = TRUE;
	BOOL bNeedToHScroll = TRUE;

	nCol += m_CountFrozenCols;
	nRow += m_CountHeaderRows;

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// Столбец полностью видим ?
	if(	LeftWndPos(nCol) < m_CountFrozenCols * m_FrozenColWidth - offsetXY)
	{
		// Новый выбранный столбеца не видим или видим неполностью.
		// Надо прокрутить, так чтобы он был самым первым видимым столбцом.
		pt.x = LeftColPos(nCol) - m_FrozenColWidth * m_CountFrozenCols;
	}
	else if ( LeftWndPos(nCol + 1) - 1 > rectClient.right - offsetXY)
	{
		// Новый выбранный столбеца не видим или видим неполностью.
		// Надо прокрутить, так чтобы он был самым последним видимым столбцом.
		pt.x = min(LeftColPos(nCol) - m_FrozenColWidth * m_CountFrozenCols, LeftColPos(nCol + 1) - rectClient.Width()) - offsetXY + 1;
	}
	else
		bNeedToHScroll = FALSE;

	// Строка полностью видима ?
	if(	TopWndPos(nRow) < m_CountHeaderRows * m_nHeadRowHeight - offsetXY)
	{
		// Новая выбранная строка не видима или видима неполностью.
		// Надо прокрутить, так чтобы она была самой первой видимой строкой.
		pt.y = TopRowPos(nRow) - m_nHeadRowHeight * m_CountHeaderRows;
	}
	else if ( TopWndPos(nRow + 1) - 1 > rectClient.bottom - offsetXY)
	{
		// Новая выбранная строка не видима или видима неполностью.
		// Надо прокрутить, так чтобы она была самой последней видимой строкой.
		pt.y = min(TopRowPos(nRow) - m_nHeadRowHeight * m_CountHeaderRows, m_CountHeaderRows * m_nHeadRowHeight + (nRow - m_CountHeaderRows + 1) * m_nRowHeight - rectClient.Height()) - offsetXY + 1;
	}
	else
		bNeedToVScroll = FALSE;

	// Если скроллинг необходим
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

	// Если необходимо, прокручиваем окно так, чтобы заданная
	// строка (столбец) была видна.
	
	// Если скроллинг необходим
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

	// Если необходимо, прокручиваем окно так, чтобы заданная
	// строка (столбец) была видна.
	
	// Если скроллинг необходим
	if(m_CurScrollPos.y != VPos)
	{
		CPoint pt(m_CurScrollPos.x,VPos);
		ScrollToPos(pt);
	}

	return;
}

// Функция сохраняет изменения сделанные в активном элементе управления
// ячейкой и деактивизирует этот элемент управления.
BOOL CALXGridCore::SaveModifyed(BOOL bSkipErrors /*= TRUE*/)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow));
	if(pCellCtrl != NULL)
	{
		if(pCellCtrl->IsModifyed())
		{// если были изменения
			if(pCellCtrl->OnValidate())
			{// если изменения корректны
				if(OnSaveCellData(GetGridCol(m_nActiveCol),GetGridRow(m_nActiveRow)))
				{// если изменения сохранены
					pCellCtrl->SetModify(FALSE);
					pCellCtrl->DeactivateCtrl();
					return TRUE;
				}
				else if(bSkipErrors)
				{// если можно проигнорировать ошибку
					pCellCtrl->SetModify(FALSE);
					pCellCtrl->DeactivateCtrl();
					return FALSE;
				}
				else
					return FALSE;
			}
			else if(bSkipErrors)
			{// если можно проигнорировать ошибку
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

// Функция рисует стрелку в ячейке шапки
// столбца по которому отсортированы строки.
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

// Функция рисует ячейку неподвижной колонки
void CALXGridCore::DrawFrozenBtn(CDC *pDC, int x, int y, int cx, int cy, BOOL bCellPressed, BOOL bDrawArrow, BOOL bFlat)
{
	// получаем облась рисования
	CRect rectCell(x,y,max(x,x+cx),max(y,y+cy));
	// получаем цвет рамки
	COLORREF clrCellFrame = ::GetSysColor(bFlat ? COLOR_BTNSHADOW : COLOR_3DDKSHADOW);

	// рисуем рамку
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
	// если нельзя выделять диапозон ячеек
	if(!(m_pGridWnd->GetStyle() & AGS_RANGESELECT))
		return;

	SaveModifyed(FALSE);

	ALXSelRangeCell mPrevSelRangeCell = m_SelRangeCell;

	m_pGridWnd->SetFocus();

	// Получаем контекст устройства
	CDC* pDC = m_pGridWnd->GetDC();

	CRect rectClient;
	m_pGridWnd->GetClientRect(rectClient);

	// новый диапозон
	m_SelRangeCell.m_LeftCol = nLeftCol;
	m_SelRangeCell.m_RightCol = nRightCol;
	m_SelRangeCell.m_TopRow = nTopRow;
	m_SelRangeCell.m_BottomRow =  nButtonRow;
	// если пустая область
	if(m_SelRangeCell.IsEmpty())
	{
		m_SelRangeCell.m_LeftCol = m_SelRangeCell.m_RightCol = m_nActiveCol;
		m_SelRangeCell.m_TopRow = m_SelRangeCell.m_BottomRow = m_nActiveRow;
	}

	m_nActiveSelCol = m_SelRangeCell.m_LeftCol;
	m_nActiveSelRow = m_SelRangeCell.m_TopRow;

	// новый активный столбец
	if(m_nActiveCol > m_SelRangeCell.m_RightCol)
		m_nActiveCol = m_SelRangeCell.m_RightCol;
	else if(m_nActiveCol < m_SelRangeCell.m_LeftCol)
		m_nActiveCol = m_SelRangeCell.m_LeftCol;
	// новая активная строка
	if(m_nActiveRow > m_SelRangeCell.m_BottomRow)
		m_nActiveRow = m_SelRangeCell.m_BottomRow;
	else if(m_nActiveRow < m_SelRangeCell.m_TopRow)
		m_nActiveRow = m_SelRangeCell.m_TopRow;

	// снимаем выделение с предыдущего деапозона выделенных ячеек
	CRect rectGrid = GetGridRect(rectClient);
	// определяем диапозон видимых в окне столбцов
	int nFirstCol, nLastCol;
	// определяем номер первого и последнего перерисовываемого столбца
	RectToColRange(rectGrid, nFirstCol, nLastCol, TRUE);
	nFirstCol = max(nFirstCol, mPrevSelRangeCell.m_LeftCol);
	nLastCol = min(nLastCol, mPrevSelRangeCell.m_RightCol);
	// определяем диапозон видимых в окне строк
	int nFirstRow, nLastRow;
	// определяем номер первого и последнего перерисовываемого столбца
	RectToRowRange(rectGrid, nFirstRow, nLastRow, TRUE);
	nFirstRow = max(nFirstRow, mPrevSelRangeCell.m_TopRow);
	nLastRow = min(nLastRow, mPrevSelRangeCell.m_BottomRow);
	// Снимаем выделение
	if(!mPrevSelRangeCell.IsEmpty())
		for(int i = nFirstRow; i <= nLastRow; i++)
			for(int j = nFirstCol; j <= nLastCol; j++)
				if(!m_SelRangeCell.InRange(j,i))
					UpdateCell(pDC, j, i);

// выделяем
	SetSelectCell(m_nActiveSelCol, m_nActiveSelRow);

//	m_pGridWnd->Invalidate();
	// Освобождаем контекст устройства
	m_pGridWnd->ReleaseDC(pDC);
}

