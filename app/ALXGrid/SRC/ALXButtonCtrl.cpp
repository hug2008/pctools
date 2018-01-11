// ALXButtonCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ALXButtonCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXButtonCtrl
BEGIN_MESSAGE_MAP(CALXButtonCtrl, CButton)
	//{{AFX_MSG_MAP(CALXButtonCtrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Конструктор
CALXButtonCtrl::CALXButtonCtrl()
{
	m_brushCtrl.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
}

// Деструктор
CALXButtonCtrl::~CALXButtonCtrl()
{
}

BOOL CALXButtonCtrl::ActivateCtrl(int x, int y, int cx, int cy)
{
	if(m_Data.m_strText.GetLength()<=0)
	{
		DWORD dwStyle = GetStyle();
		if(dwStyle == (dwStyle | BS_CHECKBOX) || dwStyle == (dwStyle | BS_AUTOCHECKBOX) || dwStyle == (dwStyle | BS_3STATE) || dwStyle == (dwStyle | BS_AUTO3STATE) || dwStyle == (dwStyle | BS_RADIOBUTTON) || BS_AUTORADIOBUTTON)
			MoveWindow(x+2 + max(0,(cx-3-13)/2),y+2,min(cx-3,13),cy-3);
		else
			MoveWindow(x+2,y+2,cx-3,cy-3);
	}
	else
		MoveWindow(x+2,y+2,cx-3,cy-3);
	EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	CButton::SetFocus();
	return TRUE;
}

BOOL CALXButtonCtrl::DeactivateCtrl()
{
	EnableWindow(FALSE);
	ShowWindow(SW_HIDE);
	MoveWindow(0,0,0,0);
	return TRUE;
}

void CALXButtonCtrl::SetData(CELL_DATA CtrlData)
{
	m_Data = CtrlData;
	SetWindowText(CtrlData.m_strText);
	DWORD dwStyle = GetStyle();
	switch(CtrlData.m_dwTag)
	{
		case 0:
		{
			SetCheck(0);
			break;
		}
		case 1:
		{
			SetCheck(1);
			m_bState = 1;
			break;
		}
		case 2:
		{
			if(dwStyle == (dwStyle | BS_3STATE) || dwStyle == (dwStyle | BS_AUTO3STATE))
				SetCheck(2);
			else
				SetCheck(0);
			break;
		}
		default:
		{
				SetCheck(0);
		}
	}
	m_bState = GetCheck();
	return;
}

CELL_DATA CALXButtonCtrl::GetCellData()
{
	GetWindowText(m_Data.m_strText);
	DWORD dwStyle = GetStyle();
	switch(GetCheck())
	{
		case 1:
			{
				m_Data.m_dwTag = 1;
				break;
			}
		case 2:
			{
				m_Data.m_dwTag = 2;
				break;
			}
		default:
			m_Data.m_dwTag = 0;
	}
	return m_Data;
}

BOOL CALXButtonCtrl::OnValidate()
{
	m_bState = GetCheck();
	return TRUE;
}

BOOL CALXButtonCtrl::IsModifyed()
{
	return (m_bState != GetCheck());
}

BOOL CALXButtonCtrl::IsActive()
{
	return IsWindowEnabled();
}

BOOL CALXButtonCtrl::DestroyCtrl()
{
	return DestroyWindow();
}

BOOL CALXButtonCtrl::CreateCtrl(DWORD dwStyle, CWnd *pParentWnd, UINT nID)
{
	CRect rectCtrl(0,0,0,0);
	return	Create("",dwStyle,rectCtrl,pParentWnd,nID);
}

void CALXButtonCtrl::SetFontCtrl(CFont *pFont, BOOL bRedraw)
{
	SetFont(pFont,bRedraw);
}

BOOL CALXButtonCtrl::EnableCtrl(BOOL bEnable)
{
	return	EnableWindow(bEnable);
}

void CALXButtonCtrl::SetModify(BOOL bModify)
{
	if(bModify && m_bState == GetCheck())
	{
		if(GetCheck() > 0)
			m_bState = 1;
		else
			m_bState = 0;
	}
	else
		m_bState = GetCheck();
}

HBRUSH CALXButtonCtrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)m_brushCtrl;
}

void CALXButtonCtrl::SendLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_LBUTTONDOWN,nFlags,MAKELPARAM(point.x,point.y));
}

BOOL CALXButtonCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN)
	{
		switch (LOWORD(pMsg->wParam))
		{
			case VK_DOWN: case VK_UP: case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT: case VK_LEFT: case VK_RIGHT: case VK_RETURN: case VK_TAB:
				CALXButtonCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(LOWORD(pMsg->wParam),0),pMsg->lParam);
				return TRUE;
			default:
				return CButton::PreTranslateMessage(pMsg);
		}
	}
	else
		return CButton::PreTranslateMessage(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
}

void CALXButtonCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags,point);

	if(nFlags & MK_LBUTTON)
	{
		CRect rc;
		GetWindowRect(rc);

		if(point.x < 0 || point.x > rc.Width() || point.y < 0 || point.y > rc.Height())
			// Освобождаем мышь
			ReleaseCapture();
	}
}
