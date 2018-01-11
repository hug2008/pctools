// GridView.cpp : implementation of the CGridView class
//

#include "stdafx.h"
#include "XGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridView

IMPLEMENT_DYNCREATE(CXGridView, CALXGridView)

BEGIN_MESSAGE_MAP(CXGridView, CALXGridView)
	//{{AFX_MSG_MAP(CXGridView)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView construction/destruction

CXGridView::CXGridView()
{
	m_bEnableCellEdit = FALSE;

	// переопредел€ем цвета активной €чейки
	m_MetricsCells.m_ActiveCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_MetricsCells.m_ActiveCellColor.m_crText		= ::GetSysColor(COLOR_HIGHLIGHTTEXT);

	// переопредел€ем цвет и стиль рамки выделени€ активной €чейки
	SetFocusRect(::GetSysColor(COLOR_HIGHLIGHTTEXT),::GetSysColor(COLOR_HIGHLIGHT),FALSE);
}

CXGridView::~CXGridView()
{
}

// ¬иртуальна€ функци€ю. ‘ункци€ возвращает указатель на элемент управлени€ €чейкой.
CALXCellCtrl* CXGridView::GetCellCtrl(int nCol, int nRow)
{
	// можно входить в режим редактировани€
	if(m_bEnableCellEdit)
		return CALXGridView::GetCellCtrl(nCol, nRow);
	else
	{
		if(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nType == GA_BUTTONCTRL)
			return CALXGridView::GetCellCtrl(nCol, nRow);
	}

	return NULL;
}

void CXGridView::OnSetFocus(CWnd* pOldWnd) 
{
	SaveModifyed(); // сохран€ем изменени€

	m_bEnableCellEdit = FALSE;

	CALXGridView::OnSetFocus(pOldWnd);
}



void CXGridView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// нажатие вне области сетки: кнопка шапки или неподвижной колонки
	if(point.y <= GetHeadRowHeight() * GetHeadRowCount() || point.x <= GetFrozenColWidth() * GetFrozenColCount())
		CALXGridView::OnLButtonDblClk(nFlags, point);
	else
	{
		m_bEnableCellEdit = TRUE;
		CALXGrid::OnLButtonDown(nFlags, point);
	}
}

void CXGridView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// если нажата€ клавиша не имеет отношение к перемещению по €чейкам
	if( nChar == VK_TAB || nChar == VK_HOME || nChar == VK_END || nChar == VK_UP || nChar == VK_DOWN ||
		nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_PRIOR || nChar == VK_NEXT ||
		nChar == VK_CONTROL || nChar == VK_SHIFT )
	{
		if(m_bEnableCellEdit)
		{
			if(SaveModifyed(FALSE))
				CALXGridView::OnKeyDown(nChar, nRepCnt, nFlags);
		}
		else
			CALXGridView::OnKeyDown(nChar, nRepCnt, nFlags);

		return;
	}
	else
	{
		m_bEnableCellEdit = TRUE;

		CALXCellCtrl* pCellCtrl = m_HeaderInfo.m_Cols[GetActiveCol()].m_CtrlInfo.m_pColCtrl;
		if(pCellCtrl == NULL)
			return;

		// передаем данные от €чейки к эл. управлени€
		pCellCtrl->SetData(GetCellData(GetActiveCol(),GetActiveRow()));
		// активизируем эл. управлени€
		ActivateCtrl(m_nActiveCol, m_nActiveRow);

		if(nChar != VK_RETURN)
		{
			// передаем код нажатой клавиши в элемент управлени€
			switch(m_HeaderInfo.m_Cols[GetActiveCol()].m_CtrlInfo.m_nType)
			{
			case GA_EDITCTRL:
				{
					CALXEditCtrl* pEditCtrl = (CALXEditCtrl*)pCellCtrl;
					if(pEditCtrl != NULL)
						pEditCtrl->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
					break;
				}
			case GA_BUTTONCTRL:
				{
					CALXButtonCtrl* pButtonCtrl = (CALXButtonCtrl*)pCellCtrl;
					if(pButtonCtrl != NULL)
						pButtonCtrl->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
					break;
				}
			case GA_COMBOBOXCTRL:
				{
					CALXComboBoxCtrl* pComboCtrl = (CALXComboBoxCtrl*)pCellCtrl;
					if(pComboCtrl != NULL)
						pComboCtrl->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
					break;
				}
			}
		}
	}
}

