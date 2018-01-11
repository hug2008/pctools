// GridPrevCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ALXGridwz.h"
#include "GridPrevCtrl.h"
#include "ALXGridaw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridPrevCtrl

IMPLEMENT_DYNCREATE(CGridPrevCtrl, CXGridCtrl)

BEGIN_MESSAGE_MAP(CGridPrevCtrl, CXGridCtrl)
	//{{AFX_MSG_MAP(CGridPrevCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGridPrevCtrl::CGridPrevCtrl()
{
	SetGridRowCount(20);
}

CGridPrevCtrl::~CGridPrevCtrl()
{
}

CELL_DATA CGridPrevCtrl::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	if(nCol < m_Cols.GetSize() && nRow < 20)
	{
		CellData = m_Cols[nCol].Rows[nRow];
	}
	return CellData;
}

BOOL CGridPrevCtrl::OnSaveCellData(int nCol, int nRow)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA CellData = pCellCtrl->GetCellData();
		if(nCol < m_Cols.GetSize() && nRow < 20)
			m_Cols[nCol].Rows[nRow] = CellData;
	}

	return TRUE;
}






/////////////////////////////////////////////////////////////////////////////
// CGridPrevCtrl message handlers




