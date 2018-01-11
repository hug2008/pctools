// CrGridDLG.cpp : implementation file
//

#include "stdafx.h"
#include "Grid.h"
#include "CrGridDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrGridDLG dialog


CCrGridDLG::CCrGridDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CCrGridDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCrGridDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCrGridDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrGridDLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrGridDLG, CDialog)
	//{{AFX_MSG_MAP(CCrGridDLG)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrGridDLG message handlers

int CCrGridDLG::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_GridCtrl.Create( WS_EX_STATICEDGE,
							WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | AGS_FLAT, 
	             			CRect(7,7,410,220),
							this,
                			0))
	{
		TRACE0("Failed to create list box\n");
		return -1;
	}
	
	return 0;
}
