// XEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "XEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXEditCtrl

BEGIN_MESSAGE_MAP(CXEditCtrl, CALXEditCtrl)
	//{{AFX_MSG_MAP(CXEditCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Конструктор
CXEditCtrl::CXEditCtrl()
{
}

// Деструктор
CXEditCtrl::~CXEditCtrl()
{
}


BOOL CXEditCtrl::OnValidate()
{
	CString strVal;
	GetWindowText(strVal);
	double dbVal = atof(strVal);
	if(dbVal <= 0 || dbVal >= 1000)
	{
		EnableWindow(TRUE);
		SetFocus();
		AfxMessageBox("Input value may be more then 0 and less then 1000.");
		return FALSE;
	}
	return TRUE;
}



BOOL CXEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && LOWORD(pMsg->wParam) == VK_ESCAPE)
	{
		Undo();
		return TRUE;
	}

	
	return CALXEditCtrl::PreTranslateMessage(pMsg);
}
