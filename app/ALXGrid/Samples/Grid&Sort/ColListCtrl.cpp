// GridCtrl.cpp : implementation of the CColListCtrl class
//

#include "stdafx.h"
#include "resource.h"
#include "Grid.h"
#include "ColListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColListCtrl

CColListCtrl::CColListCtrl()
{

	DefineColCtrl(AddCol(20, "", ACFF_CENTER, AHFF_CENTER, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT, ID_COL_VISIBL), GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT);
	DefineColCtrl(AddCol(178, "", ACFF_LEFT, AHFF_CENTER, 0, 0, ID_COL_HNAME), GA_CELLCTRL);

	m_MetricsCells.m_FocusRect.m_bSolid = FALSE;

	SetHeadRowCount(0,FALSE);
	SetFrozenColCount(0,FALSE);
}

CColListCtrl::~CColListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColListCtrl,CALXGridCtrl)
	//{{AFX_MSG_MAP(CColListCtrl)
	ON_BN_CLICKED(ID_COL_VISIBL, OnColVisibl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// CGridView data exchange

CELL_DATA CColListCtrl::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);


	switch(GetCellCtrlID(nCol,nRow))
	{
	case ID_COL_VISIBL:
		{
			CellData.m_dwTag = m_aFieldsColInfo[nRow].m_bVisibl;
			break;
		}
	case ID_COL_HNAME:
		{
			int i = m_aFieldsColInfo[nRow].m_iField;
			CellData.m_strText.LoadString(ID_COL_DATE+i);
			break;
		}
	}
	return CellData;
}

BOOL CColListCtrl::OnSaveCellData(int nCol, int nRow)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA Data = pCellCtrl->GetCellData();
		if(GetCellCtrlID(nCol,nRow) == ID_COL_VISIBL)
			m_aFieldsColInfo[nRow].m_bVisibl = Data.m_dwTag;

		CWnd* pParentWnd = GetParent();
		if(pParentWnd != NULL)
			pParentWnd->SendMessage(WM_COMMAND,ID_VIEW_COLUMNS,0);
	}
	return TRUE;
}

CALXCellMetrics* CColListCtrl::GetCellMetrics(int nCol, int nRow)
{
	m_MetricsCells.m_Borders.m_crLeftBrdColor		= ::GetSysColor(COLOR_WINDOW);
	m_MetricsCells.m_Borders.m_crRightBrdColor		= ::GetSysColor(COLOR_WINDOW);
	m_MetricsCells.m_Borders.m_crTopBrdColor		= ::GetSysColor(COLOR_WINDOW);
	m_MetricsCells.m_Borders.m_crBottomBrdColor		= ::GetSysColor(COLOR_WINDOW);
	if(nCol != 0)
	{
		m_MetricsCells.m_FocusRect.m_crBackColor = ::GetSysColor(COLOR_HIGHLIGHT);
		m_MetricsCells.m_FocusRect.m_crFaceColor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		m_MetricsCells.m_ActiveCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_HIGHLIGHT);
		m_MetricsCells.m_ActiveCellColor.m_crText		= ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		m_MetricsCells.m_SelectCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_HIGHLIGHT);
		m_MetricsCells.m_SelectCellColor.m_crText		= ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else
	{
		m_MetricsCells.m_ActiveCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_WINDOW);
		m_MetricsCells.m_ActiveCellColor.m_crText		= ::GetSysColor(COLOR_WINDOWTEXT);
		m_MetricsCells.m_SelectCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_WINDOW);
		m_MetricsCells.m_SelectCellColor.m_crText		= ::GetSysColor(COLOR_WINDOWTEXT);
	}
		m_MetricsCells.m_FocusRect.m_crBackColor = ::GetSysColor(COLOR_WINDOW);
		m_MetricsCells.m_FocusRect.m_crFaceColor = ::GetSysColor(COLOR_WINDOWTEXT);

	return &m_MetricsCells;
}

void CColListCtrl::OnSetActiveCell(int nCol, int nRow)
{
	if(m_pwndWidth != NULL)
	{
		CString strWidth;
		strWidth.Format("%u",m_aFieldsColInfo[nRow].m_nWidth < 0 ? 0 : m_aFieldsColInfo[nRow].m_nWidth);
		m_pwndWidth->SetWindowText(strWidth);
	}

	CWnd* pParentWnd = GetParent();
	if(pParentWnd != NULL)
		pParentWnd->PostMessage(WM_COMMAND,ID_VIEW_COLUMNS,0);

}

void CColListCtrl::OnColVisibl() 
{
	OnSaveCellData(GetActiveCol(), GetActiveRow());
}

void CColListCtrl::SetColVisibl(BOOL bVisbl)
{
	int nRow = GetActiveRow();
	m_aFieldsColInfo[nRow].m_bVisibl = bVisbl;
	CALXCellCtrl* pCellCtrl = GetCellCtrl(0,GetActiveRow());
	if(pCellCtrl != NULL)
	{
		CELL_DATA CellData;
		CellData.m_dwTag = m_aFieldsColInfo[nRow].m_bVisibl;
		pCellCtrl->SetData(CellData);
	}
	InvalidateRow(GetActiveRow());
}
