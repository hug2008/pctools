// TypeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TypeCtrl.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeCtrl

CTypeCtrl::CTypeCtrl()
{
}

CTypeCtrl::~CTypeCtrl()
{
}


BEGIN_MESSAGE_MAP(CTypeCtrl, CALXComboBoxCtrl)
	//{{AFX_MSG_MAP(CTypeCtrl)
	ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeCtrl message handlers

void CTypeCtrl::OnSelOK() 
{
	CWnd* pParent = GetParent();
	if(pParent != NULL)
		pParent->PostMessage(WM_COMMAND,IDC_CELLS_CTRL,0);
}
