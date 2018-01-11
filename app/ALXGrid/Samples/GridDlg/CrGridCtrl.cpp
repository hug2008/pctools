// EventGrig.cpp : implementation file
//

#include "stdafx.h"
#include "Grid.h"
#include "CrGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEventGrig

IMPLEMENT_DYNCREATE(CCrGridCtrl, CALXGridCtrl)

BEGIN_MESSAGE_MAP(CCrGridCtrl, CALXGridCtrl)
	//{{AFX_MSG_MAP(CCrGridCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCrGridCtrl::CCrGridCtrl()
{
/*
	DefineColCtrl(AddCol(50,"¹", ACFF_RIGHT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL);
	DefineImage(0,7,7);
	DefineColCtrl(AddCol(180,"Text", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(75,"Data", ACFF_CENTER, AHFF_CENTER),GA_EDITCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(75,"Nom", ACFF_RIGHT, AHFF_RIGHT),GA_EDITCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(200,"Name1", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(200,"Name2", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	SetGridRowCount(100);
	m_BMP.LoadBitmap(IDB_BITMAP);
*/
	DefineColCtrl(AddCol(50,"¹", ACFF_RIGHT, AHFF_LEFT),GA_CELLCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL);
	DefineImage(0,7,7);
	DefineColCtrl(AddCol(180,"Text", ACFF_LEFT, AHFF_LEFT),GA_CELLCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(75,"Data", ACFF_CENTER, AHFF_CENTER),GA_CELLCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(75,"Nom", ACFF_RIGHT, AHFF_RIGHT),GA_CELLCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(200,"Name1", ACFF_LEFT, AHFF_LEFT),GA_CELLCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(200,"Name2", ACFF_LEFT, AHFF_LEFT),GA_CELLCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	SetGridRowCount(100);
	m_BMP.LoadBitmap(IDB_BITMAP);

}

CCrGridCtrl::~CCrGridCtrl()
{
}

CELL_DATA CCrGridCtrl::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	switch(nCol)
	{
	case 0:
		{
			CellData.m_strText.Format("%u",nRow+1866+nRow*5/3);
			break;
		}
	case 1:
		{
			CellData.m_strText = "Sampl";
			break;
		}
	case 2:
		{
			CellData.m_strText.Format("%u.%u.%u",11,11,2000);
			break;
		}
	case 3:
		{
			CellData.m_strText.Format("%u",nRow*nCol+44*nCol+56*nRow-nCol*33-nRow*3+500);
			break;
		}
	case 4:
		{
			CellData.m_strText = "Robson";
			break;
		}
	case 5:
		{
			CellData.m_strText = "Simpson";
			break;
		}
	default:
		CellData.m_strText.Format("%u - %u",nCol,nRow);
	}
	return CellData;
}

LPARAM CCrGridCtrl::GetImage(int nCol, int nRow, BOOL bSelected)
{
	return (LPARAM)(HBITMAP)m_BMP;
}
/////////////////////////////////////////////////////////////////////////////
// CCrGridCtrl message handlers
