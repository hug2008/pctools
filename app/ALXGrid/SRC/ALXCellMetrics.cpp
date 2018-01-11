// CALXCellMetrics.cpp : implementation of the CALXCellMetrics class
//
#include "stdafx.h"
#include "ALXCellMetrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXCellBorders

void CALXCellBorders::DrawBorders(CDC *pDC, CRect rc)
{
	DrawBorders(pDC,rc,m_crLeftBrdColor,m_crTopBrdColor,m_crRightBrdColor,m_crBottomBrdColor,m_nOrderDraw);
}

void CALXCellBorders::DrawBorders(CDC *pDC, CRect rc, COLORREF crLeftBrdColor, COLORREF crTopBrdColor, COLORREF crRightBrdColor, COLORREF crBottomBrdColor, BYTE nOrderDraw)
{
	CPen PenLeft, PenTop, PenRight, PenBottom;
	if( 
		PenLeft.CreatePen(PS_SOLID,1,crLeftBrdColor) &&
		PenTop.CreatePen(PS_SOLID,1,crTopBrdColor) &&
		PenRight.CreatePen(PS_SOLID,1,crRightBrdColor) &&
		PenBottom.CreatePen(PS_SOLID,1,crBottomBrdColor)
		)
	{
		rc.right  -= 1;
		rc.bottom -= 1;

		CPen* pOldPen = pDC->SelectObject(&PenLeft);
		pDC->MoveTo(rc.left, rc.top);
		pDC->LineTo(rc.left, rc.bottom + (nOrderDraw == AOD_VERT_HOR ? 1 : 0));

		pDC->SelectObject(&PenRight);
		pDC->MoveTo(rc.right, rc.top);
		pDC->LineTo(rc.right, rc.bottom + (nOrderDraw == AOD_VERT_HOR ? 1 : 0));

		pDC->SelectObject(&PenTop);
		pDC->MoveTo(rc.left - (nOrderDraw == AOD_VERT_HOR ? 0 : 1), rc.top);
		pDC->LineTo(rc.right + (nOrderDraw == AOD_VERT_HOR ? 0 : 2), rc.top);

		pDC->SelectObject(&PenBottom);
		pDC->MoveTo(rc.left - (nOrderDraw == AOD_VERT_HOR ? 0 : 1), rc.bottom);
		pDC->LineTo(rc.right + (nOrderDraw == AOD_VERT_HOR ? 0 : 2), rc.bottom);

		pDC->SelectObject(pOldPen);
	}
	else
		pDC->Draw3dRect(&rc,crLeftBrdColor,crRightBrdColor);
}

/////////////////////////////////////////////////////////////////////////////
// CALXFocusRect

// Конструктор
CALXFocusRect::CALXFocusRect()
{
	m_crBackColor = ::GetSysColor(COLOR_WINDOW);
	m_crFaceColor = COLOR_SELECTCELLFRAME;
	m_bSolid = TRUE;
}

// Деструктор
CALXFocusRect::~CALXFocusRect()
{

}

void CALXFocusRect::DrawFocusRect(CDC *pDC, CRect& rc)
{
	if(m_bSolid)
		pDC->Draw3dRect(&rc,m_crFaceColor,m_crFaceColor);
	else
	{
		COLORREF crBkColor = pDC->GetBkColor();
		COLORREF crTxtColor = pDC->GetTextColor();
		pDC->Draw3dRect(&rc,m_crBackColor,m_crBackColor);
		pDC->SetBkColor(m_crBackColor);
		pDC->SetTextColor(m_crFaceColor);
		pDC->DrawFocusRect(rc);
		pDC->SetBkColor(crBkColor);
		pDC->SetTextColor(crTxtColor);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CALXCellMetrics

// Конструктор
CALXCellMetrics::CALXCellMetrics()
{
	pActiveFont = NULL;
	pDefaultFont = NULL;
}

// Деструктор
CALXCellMetrics::~CALXCellMetrics()
{

}

void CALXCellMetrics::FillDefault()
{
	// Определяем цвета ячейки по умолчанию
	m_DefaultCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_WINDOW);
	m_DefaultCellColor.m_crText		= ::GetSysColor(COLOR_WINDOWTEXT);
	// Определяем цвета активной ячейки
	m_ActiveCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_WINDOW);
	m_ActiveCellColor.m_crText		= ::GetSysColor(COLOR_WINDOWTEXT);
	// Определяем цвета выделенной ячейки
	m_SelectCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_SelectCellColor.m_crText		= ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	// Определяем параметры рамки ячейки
	m_Borders.m_crLeftBrdColor	= ::GetSysColor(COLOR_BTNFACE);
	m_Borders.m_crTopBrdColor	= ::GetSysColor(COLOR_BTNFACE);
	m_Borders.m_crRightBrdColor	= ::GetSysColor(COLOR_BTNFACE);
	m_Borders.m_crBottomBrdColor= ::GetSysColor(COLOR_BTNFACE);
	m_Borders.m_nOrderDraw = AOD_VERT_HOR;
	// Определяем параметры рамки "фокуса ввода"
	m_FocusRect.m_crBackColor = ::GetSysColor(COLOR_WINDOW);
	m_FocusRect.m_crFaceColor = COLOR_SELECTCELLFRAME;
	m_FocusRect.m_bSolid = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
