// ALXEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ALXEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXEditCtrl

BEGIN_MESSAGE_MAP(CALXEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CALXEditCtrl)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Конструктор
CALXEditCtrl::CALXEditCtrl()
{
}

// Деструктор
CALXEditCtrl::~CALXEditCtrl()
{
}



/////////////////////////////////////////////////////////////////////////////
// CALXEditCtrl message handlers

void CALXEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
//		case VK_HOME: case VK_END: case VK_DOWN: case VK_UP: case VK_PRIOR: case VK_NEXT:
//			break;
		case VK_TAB:
			CALXEditCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_ESCAPE:
			{
				if(CanUndo())
					Undo();
				break;
			}
		case VK_RETURN:
			CALXEditCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(VK_DOWN,0),MAKELPARAM(nRepCnt,nFlags));
			break;
//		case VK_LEFT:
//			break;
//		case VK_RIGHT:
//			break;
		default:
			CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CALXEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_HOME: case VK_END:
			if(GetStyle() & ES_XGRID)
			{
				CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_DOWN: case VK_UP: case VK_PRIOR: case VK_NEXT:
			CALXEditCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_TAB:
			break;
		case VK_RETURN:
			break;
		case VK_LEFT:
			{
				CString strEditString;
				GetWindowText(strEditString);
				if(strEditString.GetLength()<=0||GetCaretPos().x == PosFromChar(0).x)
					CALXEditCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
				else
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_RIGHT:
			{
				CString strEditString;
				GetWindowText(strEditString);
				if(strEditString.GetLength()==0||GetCaretPos().x > PosFromChar(strEditString.GetLength()-1).x)
					CALXEditCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
				else
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		default:
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CALXEditCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_HOME: case VK_END: case VK_DOWN: case VK_UP: case VK_PRIOR: case VK_NEXT:
			break;
		case VK_TAB:
			break;
		case VK_RETURN:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		default:
			CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}


BOOL CALXEditCtrl::ActivateCtrl(int x, int y, int cx, int cy)
{
	MoveWindow(x+2,y+2,cx-3,cy-3);
	EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	SetFocus();
	SetSel(0,-1);
	return TRUE;
}

BOOL CALXEditCtrl::DeactivateCtrl()
{
	EnableWindow(FALSE);
	ShowWindow(SW_HIDE);
	MoveWindow(0,0,0,0);
	return TRUE;
}

void CALXEditCtrl::SetData(CELL_DATA CtrlData)
{
	SetModify(FALSE);
	SetWindowText(CtrlData.m_strText);
//	SetSel(0,-1);
}

CELL_DATA CALXEditCtrl::GetCellData()
{
	GetWindowText(m_Data.m_strText);
	return m_Data;
}

BOOL CALXEditCtrl::OnValidate()
{
	return TRUE;
}

BOOL CALXEditCtrl::IsModifyed()
{
	return GetModify();
}

BOOL CALXEditCtrl::IsActive()
{
	return IsWindowEnabled();
}

BOOL CALXEditCtrl::DestroyCtrl()
{
	return DestroyWindow();
}

BOOL CALXEditCtrl::CreateCtrl(DWORD dwStyle, CWnd *pParentWnd, UINT nID)
{
	return	Create(dwStyle,CRect(0,0,0,0),pParentWnd,nID);
}

void CALXEditCtrl::SetFontCtrl(CFont *pFont, BOOL bRedraw)
{
	SetFont(pFont,bRedraw);
}
void CALXEditCtrl::SetModify(BOOL bModify)
{
	CEdit::SetModify(bModify);
}

BOOL CALXEditCtrl::EnableCtrl(BOOL bEnable)
{
	return	EnableWindow(bEnable);
}

BOOL CALXEditCtrl::IsCanCut()
{
	int nStartChar, nEndChar;
	CEdit::GetSel(nStartChar, nEndChar);
	return (nStartChar != nEndChar);
}

BOOL CALXEditCtrl::IsCanCopy()
{
	int nStartChar, nEndChar;
	CEdit::GetSel(nStartChar, nEndChar);
	return (nStartChar != nEndChar);
}

BOOL CALXEditCtrl::IsCanPaste()
{
	return (::IsClipboardFormatAvailable(CF_TEXT));
}

BOOL CALXEditCtrl::IsCanUndo()
{
	return CEdit::CanUndo();
}

void CALXEditCtrl::Cut()
{
	CEdit::Cut();
}

void CALXEditCtrl::Copy()
{
	CEdit::Copy();
}

void CALXEditCtrl::Paste()
{
	CEdit::Paste();
}

void CALXEditCtrl::Undo()
{
	CEdit::Undo();
}
