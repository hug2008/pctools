// ALXEditSpinCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ALXEditSpinCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXEditSpinCtrl
CALXEditSpinCtrl::CALXEditSpinCtrl()
{
	m_hWndSpin = NULL;
}

CALXEditSpinCtrl::~CALXEditSpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CALXEditSpinCtrl, CALXEditCtrl)
	//{{AFX_MSG_MAP(CALXEditSpinCtrl)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALXEditSpinCtrl message handlers

BOOL CALXEditSpinCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{

   WORD wNotification;
#ifdef _WIN32
   wNotification = HIWORD(wParam);
#else
   wNotification = HIWORD(lParam);
#endif
   if (message == WM_COMMAND && wNotification == EN_CHANGE && m_hWndSpin != NULL)
   {
		::PostMessage(m_hWndSpin, WM_COMMAND, EN_CHANGE,0);
      //CEditView::OnEditChange();
      return TRUE;
   }
   else
	   return CALXEditCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

void CALXEditSpinCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar >= VK_0 && nChar <= VK_9)
		CALXEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}
