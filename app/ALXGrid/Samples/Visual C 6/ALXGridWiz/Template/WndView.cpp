// $$wndview_ifile$$.cpp : implementation of the $$WNDVIEW_CLASS$$ class
//

#include "stdafx.h"
#include "resource.h"
#include "$$wndview_hfile$$.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// $$WNDVIEW_CLASS$$

$$WNDVIEW_CLASS$$::$$WNDVIEW_CLASS$$()
{
$$IF(CXGridCtrl)
$$GRID_CONSTR$$
	SetGridRowCount(20);
$$ENDIF //CXGridCtrl
}

$$WNDVIEW_CLASS$$::~$$WNDVIEW_CLASS$$()
{
}


BEGIN_MESSAGE_MAP($$WNDVIEW_CLASS$$,$$WNDVIEW_BASE_CLASS$$)
	//{{AFX_MSG_MAP($$WNDVIEW_CLASS$$)
$$IF(!CXGridCtrl)
	ON_WM_PAINT()
$$ENDIF //!CXGridCtrl
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// $$WNDVIEW_CLASS$$ message handlers

BOOL $$WNDVIEW_CLASS$$::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
$$IF(CXGridCtrl)
	return $$WNDVIEW_BASE_CLASS$$::PreCreateWindow(cs);
$$ELSE //!CXGridCtrl
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
$$ENDIF //CXGridCtrl
}

$$IF(!CXGridCtrl)
void $$WNDVIEW_CLASS$$::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}
$$ENDIF //!CXGridCtrl

/////////////////////////////////////////////////////////////////////////////
// $$VIEW_CLASS$$ data exchange

$$IF(CXGridCtrl)
CELL_DATA $$WNDVIEW_CLASS$$::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	return CellData;
}

BOOL $$WNDVIEW_CLASS$$::OnSaveCellData(int nCol, int nRow)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA Data = pCellCtrl->GetCellData();
	}
	return TRUE;
}
$$ENDIF //CXGridCtrl

