// RegGridDLG.cpp : implementation file
//

#include "stdafx.h"
#include "Grid.h"
#include "RegGridDLG.h"
#include "RegGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RegGridDLG dialog


CRegGridDLG::CRegGridDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CRegGridDLG::IDD, pParent)
{
	if (!CRegGridCtrl::RegisterClass())
	{
		TRACE0("Failed to register grid control\n");
		return;
	}
	//{{AFX_DATA_INIT(CRegGridDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CRegGridDLG::~CRegGridDLG()
{
	CRegGridCtrl::UnregisterClass();
}

void CRegGridDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegGridDLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegGridDLG, CDialog)
	//{{AFX_MSG_MAP(CRegGridDLG)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegGridDLG message handlers

