// ALXSplitterWnd.cpp : implementation file
//

#include "stdafx.h"

#if _MSC_VER > 1200
#include <..\atlmfc\src\mfc\Afximpl.h>
#else
#include <..\SRC\Afximpl.h>
#endif // _MSC_VER > 1200

#include <Afxpriv.h>
#include "ALXSplitterWnd.h"

#include "ALXGridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXSplitterWnd

IMPLEMENT_DYNAMIC(CALXSplitterWnd, CSplitterWnd)

char const* _alxWndSplitt = _T("ALXSplitterWnd");
/////////////////////////////////////////////////////////////////////////////
// Визуальные атрибуты и другие константы

// HitTest возращаемые значения (значения и промежутки между значениями важны)
enum HitTestValue
{
	noHit                   = 0,
	vSplitterBox            = 1,
	hSplitterBox            = 2,
	bothSplitterBox         = 3,        // только для клавиатуры
	vSplitterBar1           = 101,
	vSplitterBar15          = 115,
	hSplitterBar1           = 201,
	hSplitterBar15          = 215,
	splitterIntersection1   = 301,
	splitterIntersection225 = 525
};

/////////////////////////////////////////////////////////////////////////////
// CALXSplitterWnd

BEGIN_MESSAGE_MAP(CALXSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CALXSplitterWnd)
	ON_MESSAGE(GWM_ACTCOLROWCHANGED, OnActGridColRowChanged)
	ON_MESSAGE(GWM_SELCOLROWRANGE, OnSelGridColRowRange)
	ON_MESSAGE(GWM_COLWIDTHCHANGED, OnGridColWidthChanged)
	ON_MESSAGE(GWM_SAVECHANGED, OnSaveChanged)
	ON_MESSAGE(GWM_GRIDROWCOUNTCHANGED, OnGridRowCountChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALXSplitterWnd construction/destruction

CALXSplitterWnd::CALXSplitterWnd()
{

}

CALXSplitterWnd::~CALXSplitterWnd()
{

}

/////////////////////////////////////////////////////////////////////////////
// CALXSplitterWnd заданное по умолчанию создание панелей

// Обычно окно разделителя невидимо при создании области окна
BOOL CALXSplitterWnd::CreateView(int row, int col,
	CRuntimeClass* pViewClass, SIZE sizeInit, CCreateContext* pContext)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(row >= 0 && row < m_nRows);
	ASSERT(col >= 0 && col < m_nCols);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	if (GetDlgItem(IdFromRowCol(row, col)) != NULL)
	{
		TRACE2("Error: CreateView - pane already exists for row %d, col %d.\n",
			row, col);
		ASSERT(FALSE);
		return FALSE;
	}
#endif

	// устанавливаем начальный размер для панелей
	m_pColInfo[col].nIdealSize = sizeInit.cx;
	m_pRowInfo[row].nIdealSize = sizeInit.cy;

	BOOL bSendInitialUpdate = FALSE;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected client if possible
		CView* pOldView = (CView*)GetActivePane();
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(contextT.m_pCurrentFrame == NULL);
			contextT.m_pLastView = pOldView;
			contextT.m_pCurrentDoc = pOldView->GetDocument();
			if (contextT.m_pCurrentDoc != NULL)
				contextT.m_pNewDocTemplate =
				  contextT.m_pCurrentDoc->GetDocTemplate();
		}
		else
		{
			CView* pView00 = (CView*)GetPane(0,0);
			if (pView00 != NULL && pView00->IsKindOf(RUNTIME_CLASS(CView)))
			{
				// set info about last pane
				ASSERT(contextT.m_pCurrentFrame == NULL);
				contextT.m_pLastView = pView00;
				contextT.m_pCurrentDoc = pView00->GetDocument();
				if (contextT.m_pCurrentDoc != NULL)
					contextT.m_pNewDocTemplate =
					  contextT.m_pCurrentDoc->GetDocTemplate();
			}
		}
		pContext = &contextT;
		bSendInitialUpdate = TRUE;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a splitter pane.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;

#if _MSC_VER > 1200
	dwStyle &= ~WS_BORDER;
#else
	if (afxData.bWin4)
		dwStyle &= ~WS_BORDER;
#endif // _MSC_VER > 1200

	// Create with the right size (wrong position)
	CRect rect(CPoint(0,0), sizeInit);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, this, IdFromRowCol(row, col), pContext))
	{
		TRACE0("Warning: couldn't create client pane for splitter.\n");
			// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	ASSERT((int)_AfxGetDlgCtrlID(pWnd->m_hWnd) == IdFromRowCol(row, col));

	// send initial notification message
	if (bSendInitialUpdate)
		pWnd->SendMessage(WM_INITIALUPDATE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CALXSplitterWnd client operations/overridables

void CALXSplitterWnd::DeleteView(int row, int col)
{
	ASSERT_VALID(this);

	// if active child is being deleted - activate next
	CWnd* pPane = GetPane(row, col);
	ASSERT_KINDOF(CView, pPane);
	if (GetActivePane() == pPane)
		ActivateNext(FALSE);

	// default implementation assumes view will auto delete in PostNcDestroy
	pPane->DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// Dynamic row/col split etc

AFX_STATIC int AFXAPI _ALXCanSplitRowCol(CSplitterWnd::CRowColInfo* pInfoBefore,
	int nBeforeSize, int nSizeSplitter)
	// request to split Before row at point nBeforeSize
	// returns size of new pane (nBeforeSize will be new size of Before pane)
	// return -1 if not big enough
{
	ASSERT(pInfoBefore->nCurSize > 0);
	ASSERT(pInfoBefore->nMinSize > 0);
	ASSERT(nBeforeSize <= pInfoBefore->nCurSize);

	// space gets take from before pane (weird UI for > 2 splits)
	if (nBeforeSize < pInfoBefore->nMinSize)
	{
		TRACE0("Warning: split too small to fit in a new pane.\n");
		return -1;
	}

	int nNewSize = pInfoBefore->nCurSize - nBeforeSize - nSizeSplitter;
	if (nBeforeSize < pInfoBefore->nMinSize)
	{
		TRACE0("Warning: split too small to shrink old pane.\n");
		return -1;
	}
	if (nNewSize < (pInfoBefore+1)->nMinSize)
	{
		TRACE0("Warning: split too small to create new pane.\n");
		return -1;
	}
	return nNewSize;
}


BOOL CALXSplitterWnd::SplitRow(int cyBefore)
{
	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);
	ASSERT(m_pDynamicViewClass != NULL);
	ASSERT(m_nRows < m_nMaxRows);

	int nRow,nCol;
	CALXGridView* pActiveView = (CALXGridView*)GetActivePane();
	if(pActiveView != NULL && IsChildPane(pActiveView, &nRow, &nCol))
		pActiveView->SaveModifyed();

	cyBefore -= m_cyBorder;
	int rowNew = m_nRows;
	int cyNew = _ALXCanSplitRowCol(&m_pRowInfo[rowNew-1], cyBefore, m_cySplitter);
	if (cyNew == -1)
		return FALSE;   // too small to split

	// create the scroll bar first (so new views can see that it is there)
	if (m_bHasVScroll &&
		!CreateScrollBarCtrl(SBS_VERT, AFX_IDW_VSCROLL_FIRST + rowNew))
	{
		TRACE0("Warning: SplitRow failed to create scroll bar.\n");
		return FALSE;
	}

	m_nRows++;  // bump count during view creation


	// create new views to fill the new row (RecalcLayout will position)
	for (int col = 0; col < m_nCols; col++)
	{
		CSize size(m_pColInfo[col].nCurSize, cyNew);
		if (!CreateView(rowNew, col, m_pDynamicViewClass, size, NULL))
		{
			TRACE0("Warning: SplitRow failed to create new row.\n");
			// delete anything we partially created 'col' = # columns created
			while (col > 0)
				DeleteView(rowNew, --col);
			if (m_bHasVScroll)
				GetDlgItem(AFX_IDW_VSCROLL_FIRST + rowNew)->DestroyWindow();
			m_nRows--;      // it didn't work out
			return FALSE;
		}

		int nHeadSortCol, nHeadSortRow;
		ALXSortOrder nSortOrder;

		CALXGridView* pNewView = (CALXGridView*)GetPane(rowNew, col);

		if(rowNew>0)
		{
			pNewView->SetHeadRowCount(0,FALSE);
			if(col>0)	// (1,1)
			{
				pNewView->SetFrozenColCount(0,FALSE);
				CALXGridView* pView01 = (CALXGridView*)GetPane(0, 1);
				pNewView->ScrollAndSetActCol(pView01->GetCurScrollPos().x,pView01->GetActiveCol(),FALSE);
				CALXGridView* pView10 = (CALXGridView*)GetPane(1, 0);
				pNewView->ScrollAndSetActRow(pView10->GetCurScrollPos().y,pView10->GetActiveRow(),FALSE);

				pView10->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
			else		// (1,0)
			{
				CALXGridView* pView00 = (CALXGridView*)GetPane(0, 0);
				pNewView->ScrollAndSetActCol(pView00->GetCurScrollPos().x,pView00->GetActiveCol()+pView00->GetFrozenColCount(),FALSE);
				pNewView->ScrollAndSetActRow(pView00->GetCurScrollPos().y,pView00->GetActiveRow(),FALSE);

				pView00->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
		}
		else
		{
			if(col>0)	// (0,1)
			{
				pNewView->SetFrozenColCount(0,FALSE);
				CALXGridView* pView00 = (CALXGridView*)GetPane(0, 0);
				pNewView->ScrollAndSetActCol(pView00->GetCurScrollPos().x,pView00->GetActiveCol(),FALSE);
				pNewView->ScrollAndSetActRow(pView00->GetCurScrollPos().y,pView00->GetActiveRow()+pView00->GetHeadRowCount(),FALSE);
	
				pView00->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
			else		// (0,0)
			{
				pNewView->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
		}

		pNewView->SetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
		pNewView->UpdateScrollSizes();
	}

	OnGridColWidthChanged((WPARAM)GetPane(0, 0),0);

	// создаются новые панели - меняются размеры и расположение
	m_pRowInfo[rowNew-1].nIdealSize = cyBefore;
	m_pRowInfo[rowNew].nIdealSize = cyNew;
	ASSERT(m_nRows == rowNew+1);
	RecalcLayout();

	return TRUE;
}

BOOL CALXSplitterWnd::SplitColumn(int cxBefore)
{

	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);
	ASSERT(m_pDynamicViewClass != NULL);
	ASSERT(m_nCols < m_nMaxCols);

	int nRow,nCol;
	CALXGridView* pActiveView = (CALXGridView*)GetActivePane();
	if(pActiveView != NULL && IsChildPane(pActiveView, &nRow, &nCol))
		pActiveView->SaveModifyed();

	cxBefore -= m_cxBorder;
	int colNew = m_nCols;
	int cxNew = _ALXCanSplitRowCol(&m_pColInfo[colNew-1], cxBefore, m_cxSplitter);
	if (cxNew == -1)
		return FALSE;   // too small to split

	// create the scroll bar first (so new views can see that it is there)
	if (m_bHasHScroll &&
		!CreateScrollBarCtrl(SBS_HORZ, AFX_IDW_HSCROLL_FIRST + colNew))
	{
		TRACE0("Warning: SplitRow failed to create scroll bar.\n");
		return FALSE;
	}

	m_nCols++;  // bump count during view creation


	// create new views to fill the new column (RecalcLayout will position)
	for (int row = 0; row < m_nRows; row++)
	{
		CSize size(cxNew, m_pRowInfo[row].nCurSize);
		if (!CreateView(row, colNew, m_pDynamicViewClass, size, NULL))
		{
			TRACE0("Warning: SplitColumn failed to create new column.\n");
			// delete anything we partially created 'col' = # columns created
			while (row > 0)
				DeleteView(--row, colNew);
			if (m_bHasHScroll)
				GetDlgItem(AFX_IDW_HSCROLL_FIRST + colNew)->DestroyWindow();
			m_nCols--;      // it didn't work out
			return FALSE;
		}

		int nHeadSortCol, nHeadSortRow;
		ALXSortOrder nSortOrder;

		CALXGridView* pNewView = (CALXGridView*)GetPane(row, colNew);
		if(colNew>0)
		{
			pNewView->SetFrozenColCount(0,FALSE);
			if(row>0)	// (1,1)
			{
				pNewView->SetHeadRowCount(0,FALSE);
				CALXGridView* pView10 = (CALXGridView*)GetPane(1, 0);
				pNewView->ScrollAndSetActRow(pView10->GetCurScrollPos().y,pView10->GetActiveRow(),FALSE);
				CALXGridView* pView01 = (CALXGridView*)GetPane(0, 1);
				pNewView->ScrollAndSetActCol(pView01->GetCurScrollPos().x,pView01->GetActiveCol(),FALSE);

				pView10->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
			else		// (0,1)
			{
				CALXGridView* pView00 = (CALXGridView*)GetPane(0, 0);
				pNewView->ScrollAndSetActCol(pView00->GetCurScrollPos().x,pView00->GetActiveCol(),FALSE);
				pNewView->ScrollAndSetActRow(pView00->GetCurScrollPos().y,pView00->GetActiveRow()+pView00->GetHeadRowCount(),FALSE);

				pView00->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
		}
		else
		{
			if(row>0)	// (1,0)
			{
				pNewView->SetHeadRowCount(0,FALSE);
				CALXGridView* pView00 = (CALXGridView*)GetPane(0, 0);
				pNewView->ScrollAndSetActCol(pView00->GetCurScrollPos().x,pView00->GetActiveCol()+pView00->GetFrozenColCount(),FALSE);
				pNewView->ScrollAndSetActRow(pView00->GetCurScrollPos().y,pView00->GetActiveRow(),FALSE);

				pView00->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
			else		// (0,0)
			{
				pNewView->GetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
			}
		}

		pNewView->SetSortCol(nHeadSortCol, nHeadSortRow, nSortOrder);
		pNewView->UpdateScrollSizes();
	}

	OnGridColWidthChanged((WPARAM)GetPane(0, 0),0);

	// создаются новые панели - меняются размеры и расположение
	m_pColInfo[colNew-1].nIdealSize = cxBefore;
	m_pColInfo[colNew].nIdealSize = cxNew;
	ASSERT(m_nCols == colNew+1);
	RecalcLayout();

	return TRUE;
}

void CALXSplitterWnd::DeleteRow(int rowDelete)
{
	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);

	ASSERT(m_nRows > 1);
	ASSERT(rowDelete < m_nRows);

	CALXGridView* pView = (CALXGridView*)GetPane(0,0);
	int nHeadRowCount = pView->GetHeadRowCount();

	int rowActive, colActive;
	if (GetActivePane(&rowActive, &colActive) != NULL && rowActive == rowDelete)
	{
		if (++rowActive >= m_nRows)
			rowActive = 0;
		SetActivePane(rowActive, colActive);
	}

	CWnd* pScrollDel = m_bHasVScroll ?
		GetDlgItem(AFX_IDW_VSCROLL_FIRST+rowDelete) : NULL;
	for (int col = 0; col < m_nCols; col++)
	{
		DeleteView(rowDelete, col);
		for (int row = rowDelete+1; row < m_nRows; row++)
		{
			CWnd* pPane = GetPane(row, col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(row-1, col));
			if (m_bHasVScroll && col == m_nCols-1)
			{
				CWnd* pScroll = GetDlgItem(AFX_IDW_VSCROLL_FIRST+row);
				if (pScroll != NULL)
					pScroll->SetDlgCtrlID(AFX_IDW_VSCROLL_FIRST+row-1);
			}
		}
	}
	m_nRows--;
	if (pScrollDel != NULL)
		pScrollDel->DestroyWindow();

	for ( col = 0; col < m_nCols; col++)
	{
		CALXGridView* pView = (CALXGridView*)GetPane(0,col);
		pView->SetHeadRowCount(nHeadRowCount);
	}

	RecalcLayout();     // переопределяем область
}

void CALXSplitterWnd::DeleteColumn(int colDelete)
{
	ASSERT_VALID(this);
	ASSERT(GetStyle() & SPLS_DYNAMIC_SPLIT);

	ASSERT(m_nCols > 1);
	ASSERT(colDelete < m_nCols);

	CALXGridView* pView = (CALXGridView*)GetPane(0,0);
	int nFrozenColCount = pView->GetFrozenColCount();

	int rowActive, colActive;
	if (GetActivePane(&rowActive, &colActive) != NULL && colActive == colDelete)
	{
		if (++colActive >= m_nCols)
			colActive = 0;
		SetActivePane(rowActive, colActive);
	}

	CWnd* pScrollDel = m_bHasHScroll ?
		GetDlgItem(AFX_IDW_HSCROLL_FIRST+colDelete) : NULL;
	for (int row = 0; row < m_nRows; row++)
	{
		DeleteView(row, colDelete);
		for (int col = colDelete+1; col < m_nCols; col++)
		{
			CWnd* pPane = GetPane(row, col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(row, col-1));
			if (m_bHasHScroll && row == m_nRows-1)
			{
				CWnd* pScroll = GetDlgItem(AFX_IDW_HSCROLL_FIRST+col);
				if (pScroll != NULL)
					pScroll->SetDlgCtrlID(AFX_IDW_HSCROLL_FIRST+col-1);
			}
		}
	}
	m_nCols--;
	if (pScrollDel != NULL)
		pScrollDel->DestroyWindow();

	for ( row = 0; row < m_nRows; row++)
	{
		CALXGridView* pView = (CALXGridView*)GetPane(row,0);
		pView->SetFrozenColCount(nFrozenColCount);
	}

	RecalcLayout();     // переопределяем область
}

LRESULT CALXSplitterWnd::OnActGridColRowChanged(WPARAM wParam, LPARAM lParam) 
{
	if(m_nCols < 2 && m_nRows < 2)
		return TRUE;

	CALXGridView* pView00 = (CALXGridView*)GetPane(0,0);

	// если сообщение из окна с коорд. (0,0)
	if(pView00->m_hWnd == (HWND)wParam)
	{
		// если 2 колонки
		if(m_nCols > 1)
		{
			CALXGridView* pView01 = (CALXGridView*)GetPane(0,1);
			pView01->ScrollAndSetActRow(pView00->GetCurScrollPos().y,pView00->GetActiveRow()+pView00->GetHeadRowCount());
		}
		// если 2 строки
		if(m_nRows > 1)
		{
			CALXGridView* pView10 = (CALXGridView*)GetPane(1,0);
			pView10->ScrollAndSetActCol(pView00->GetCurScrollPos().x,pView00->GetActiveCol()+pView00->GetFrozenColCount());
		}
	}

	// если 2 колонки
	if(m_nCols > 1)
	{
		CALXGridView* pView01 = (CALXGridView*)GetPane(0,1);

		// если сообщение из окна с коорд. (0,1)
		if(pView01->m_hWnd == (HWND)wParam)
		{
			pView00->ScrollAndSetActRow(pView01->GetCurScrollPos().y,pView01->GetActiveRow()+pView01->GetHeadRowCount());

			// если 2 строки
			if(m_nRows > 1)
			{
				CALXGridView* pView11 = (CALXGridView*)GetPane(1,1);
				pView11->ScrollAndSetActCol(pView01->GetCurScrollPos().x,pView01->GetActiveCol()+pView01->GetFrozenColCount());
			}
		}
	}

	// если 2 строки
	if(m_nRows > 1)
	{
		CALXGridView* pView10 = (CALXGridView*)GetPane(1,0);

		// если сообщение из окна с коорд. (1,0)
		if(pView10->m_hWnd == (HWND)wParam)
		{
			pView00->ScrollAndSetActCol(pView10->GetCurScrollPos().x,pView10->GetActiveCol()+pView10->GetFrozenColCount());

			// если 2 колонки
			if(m_nCols > 1)
			{
				CALXGridView* pView11 = (CALXGridView*)GetPane(1,1);
				pView11->ScrollAndSetActRow(pView10->GetCurScrollPos().y,pView10->GetActiveRow()+pView10->GetHeadRowCount());
			}
		}
	}
	// если 2 колонки и 2 строки
	if(m_nCols > 1 && m_nRows > 1)
	{
		CALXGridView* pView11 = (CALXGridView*)GetPane(1,1);

		// если сообщение из окна с коорд. (1,1)
		if(pView11->m_hWnd == (HWND)wParam)
		{
			CALXGridView* pView01 = (CALXGridView*)GetPane(0,1);
			pView01->ScrollAndSetActCol(pView11->GetCurScrollPos().x,pView11->GetActiveCol()+pView11->GetFrozenColCount());

			CALXGridView* pView10 = (CALXGridView*)GetPane(1,0);
			pView10->ScrollAndSetActRow(pView11->GetCurScrollPos().y,pView11->GetActiveRow()+pView11->GetHeadRowCount());
		}
	}
	return TRUE;
}

LRESULT CALXSplitterWnd::OnSelGridColRowRange(WPARAM wParam, LPARAM lParam) 
{
	if(m_nCols < 2 && m_nRows < 2)
		return TRUE;

	CALXGridView* pView00 = (CALXGridView*)GetPane(0,0);

	// если сообщение из окна с коорд. (0,0)
	if(pView00->m_hWnd == (HWND)wParam)
	{
		// если 2 колонки
		if(m_nCols > 1)
		{
			CALXGridView* pView01 = (CALXGridView*)GetPane(0,1);
			pView01->ScrollToRow(pView00->GetCurScrollPos().y,pView00->GetActiveRow()+pView00->GetHeadRowCount());
		}
		// если 2 строки
		if(m_nRows > 1)
		{
			CALXGridView* pView10 = (CALXGridView*)GetPane(1,0);
			pView10->ScrollToCol(pView00->GetCurScrollPos().x,pView00->GetActiveCol()+pView00->GetFrozenColCount());
		}
	}

	// если 2 колонки
	if(m_nCols > 1)
	{
		CALXGridView* pView01 = (CALXGridView*)GetPane(0,1);

		// если сообщение из окна с коорд. (0,1)
		if(pView01->m_hWnd == (HWND)wParam)
		{
			pView00->ScrollToRow(pView01->GetCurScrollPos().y,pView01->GetActiveRow()+pView01->GetHeadRowCount());

			// если 2 строки
			if(m_nRows > 1)
			{
				CALXGridView* pView11 = (CALXGridView*)GetPane(1,1);
				pView11->ScrollToCol(pView01->GetCurScrollPos().x,pView01->GetActiveCol()+pView01->GetFrozenColCount());
			}
		}
	}

	// если 2 строки
	if(m_nRows > 1)
	{
		CALXGridView* pView10 = (CALXGridView*)GetPane(1,0);

		// если сообщение из окна с коорд. (1,0)
		if(pView10->m_hWnd == (HWND)wParam)
		{
			pView00->ScrollToCol(pView10->GetCurScrollPos().x,pView10->GetActiveCol()+pView10->GetFrozenColCount());

			// если 2 колонки
			if(m_nCols > 1)
			{
				CALXGridView* pView11 = (CALXGridView*)GetPane(1,1);
				pView11->ScrollToRow(pView10->GetCurScrollPos().y,pView10->GetActiveRow()+pView10->GetHeadRowCount());
			}
		}
	}
	// если 2 колонки и 2 строки
	if(m_nCols > 1 && m_nRows > 1)
	{
		CALXGridView* pView11 = (CALXGridView*)GetPane(1,1);

		// если сообщение из окна с коорд. (1,1)
		if(pView11->m_hWnd == (HWND)wParam)
		{
			CALXGridView* pView01 = (CALXGridView*)GetPane(0,1);
			pView01->ScrollToCol(pView11->GetCurScrollPos().x,pView11->GetActiveCol()+pView11->GetFrozenColCount());

			CALXGridView* pView10 = (CALXGridView*)GetPane(1,0);
			pView10->ScrollToRow(pView11->GetCurScrollPos().y,pView11->GetActiveRow()+pView11->GetHeadRowCount());
		}
	}
	return TRUE;
}

LRESULT CALXSplitterWnd::OnGridColWidthChanged(WPARAM wParam, LPARAM lParam) 
{
	if(m_nCols < 2 && m_nRows < 2)
		return TRUE;
	int nCol,nRow;
	if(!IsChildPane((CWnd*)wParam, &nRow, &nCol))
		return TRUE;

	CALXGridView* pSendView = (CALXGridView*)GetPane(nRow,nCol);
	for(int col = 0; col < m_nCols; col++)
	{
		for(int row = 0; row < m_nRows; row++)
		{
			CALXGridView* pView = (CALXGridView*)GetPane(row,col);
			if(pSendView != pView)
			{
				for(int i = 0; i < pSendView->GetHeadColCount(); i++)
					pView->SetHeadColWidth(i,-1,pSendView->GetHeadColWidth(i));
				pView->SetFrozenColWidth(pSendView->GetFrozenColWidth(),FALSE);
				pView->UpdateScrollSizes();
			}
		}
	}
	return TRUE;
}

LRESULT CALXSplitterWnd::OnSaveChanged(WPARAM wParam, LPARAM lParam) 
{
	if(m_nCols < 2 && m_nRows < 2)
		return TRUE;

	for(int col = 0; col < m_nCols; col++)
	{
		for(int row = 0; row < m_nRows; row++)
		{
			CALXGridView* pView = (CALXGridView*)GetPane(row,col);
			{
				pView->InvalidateCell(wParam,lParam);
				pView->OnSavedCellData(wParam,lParam);
			}
		}
	}
	return TRUE;
}

LRESULT CALXSplitterWnd::OnGridRowCountChanged(WPARAM wParam, LPARAM lParam) 
{
	if(m_nCols < 2 && m_nRows < 2)
		return TRUE;

	for(int col = 0; col < m_nCols; col++)
	{
		for(int row = 0; row < m_nRows; row++)
		{
			CALXGridView* pView = (CALXGridView*)GetPane(row,col);
			if(pView->GetGridRowCount() != lParam)
			{
				pView->SetGridRowCount(lParam, FALSE);
				pView->UpdateScrollSizes();
			}
		}
	}
	return TRUE;
}

BOOL CALXSplitterWnd::OnMouseWheel(CView* pSendView, UINT fFlags, short zDelta, CPoint point)
{
	for(int col = 0; col < m_nCols; col++)
	{
		for(int row = 0; row < m_nRows; row++)
		{
			CALXGridView* pView = (CALXGridView*)GetPane(row,col);
			if(pSendView == pView)
			{
				if(col == 1)
				{
					CALXGridView* pLeftView = (CALXGridView*)GetPane(row,0);
					pView->CALXGrid::OnMouseWheel(fFlags, zDelta, point, FALSE);
					pLeftView->CALXGrid::OnMouseWheel(fFlags, zDelta, point, FALSE);
				}
				else if(m_nCols > 1)
				{
					CALXGridView* pRightView = (CALXGridView*)GetPane(row,1);
					pView->CALXGrid::OnMouseWheel(fFlags, zDelta, point, FALSE);
					pRightView->CALXGrid::OnMouseWheel(fFlags, zDelta, point, FALSE);
				}
				else
					pView->CALXGrid::OnMouseWheel(fFlags, zDelta, point, FALSE);

				pView->SetFocus();
			}
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CALXSplitterWnd diagnostics

#ifdef _DEBUG
void CALXSplitterWnd::AssertValid() const
{
	CWnd::AssertValid();
	ASSERT(m_nMaxRows >= 1);
	ASSERT(m_nMaxCols >= 1);
	ASSERT(m_nMaxCols > 1 || m_nMaxRows > 1);       // 1x1 is not permitted
	ASSERT(m_nRows >= 1);
	ASSERT(m_nCols >= 1);
	ASSERT(m_nRows <= m_nMaxRows);
	ASSERT(m_nCols <= m_nMaxCols);
}

void CALXSplitterWnd::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);

	if (m_pDynamicViewClass != NULL)
		dc << "m_pDynamicViewClass = " << m_pDynamicViewClass->m_lpszClassName;
	dc << "\nm_nMaxRows = " << m_nMaxRows;
	dc << "\nm_nMaxCols = " << m_nMaxCols;
	dc << "\nm_nRows = " << m_nRows;
	dc << "\nm_nCols = " << m_nCols;
	dc << "\nm_bHasHScroll = " << m_bHasHScroll;
	dc << "\nm_bHasVScroll = " << m_bHasVScroll;
	dc << "\nm_cxSplitter = " << m_cxSplitter;
	dc << "\nm_cySplitter = " << m_cySplitter;
	if (m_bTracking)
	{
		dc << "\nTRACKING: m_htTrack = " << m_htTrack;
		dc << "\nm_rectLimit = " << m_rectLimit;
		dc << "\nm_ptTrackOffset = " << m_ptTrackOffset;
		dc << "\nm_rectTracker = " << m_rectTracker;
		if (m_bTracking2)
			dc << "\nm_rectTracker2 = " << m_rectTracker2;
	}

	dc << "\n";
}
#endif

/////////////////////////////////////////////////////////////////////////////

