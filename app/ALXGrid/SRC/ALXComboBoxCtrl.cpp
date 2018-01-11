// ALXEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ALXComboBoxCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXComboEditCtrl
BEGIN_MESSAGE_MAP(CALXComboEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CALXComboEditCtrl)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Конструктор
CALXComboEditCtrl::CALXComboEditCtrl()
{
}

// Деструктор
CALXComboEditCtrl::~CALXComboEditCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CALXComboEditCtrl обработчики сообщений

void CALXComboEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
			break;
		case VK_DOWN: case VK_UP:
			{
				if(GetKeyState(VK_SHIFT) < 0)
					CEdit::OnChar(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_TAB:
			GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_ESCAPE: 
			break;
		case VK_RETURN:
			GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(VK_DOWN,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		default:
			CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CALXComboEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
			GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_DOWN: case VK_UP:
			{
				if(GetKeyState(VK_SHIFT) >= 0)
					GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
				else
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_TAB:
			break;
		case VK_RETURN:
			break;
		case VK_LEFT:
			{
				CString strEditString;
				GetWindowText(strEditString);
				if(strEditString.GetLength()<=0||GetCaretPos().x == PosFromChar(0).x)
					GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
				else
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_RIGHT:
			{
				CString strEditString;
				GetWindowText(strEditString);
				if(strEditString.GetLength()==0||GetCaretPos().x > PosFromChar(strEditString.GetLength()-1).x)
					GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
				else
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		default:
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CALXComboEditCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
			break;
		case VK_DOWN: case VK_UP:
			{
				if(GetKeyState(VK_SHIFT) < 0)
					CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
				break;
			}
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

/////////////////////////////////////////////////////////////////////////////
// CALXComboBoxCtrl

BEGIN_MESSAGE_MAP(CALXComboBoxCtrl, CComboBox)
	//{{AFX_MSG_MAP(CALXComboBoxCtrl)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_PARENTNOTIFY()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseUp)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Конструктор
CALXComboBoxCtrl::CALXComboBoxCtrl()
{
	m_bInitCtrl=FALSE;
}

// Деструктор
CALXComboBoxCtrl::~CALXComboBoxCtrl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CALXComboBoxCtrl обработчики сообщений

void CALXComboBoxCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
			break;
		case VK_DOWN: case VK_UP:
			{
				if(GetKeyState(VK_SHIFT) < 0)
					CComboBox::OnChar(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_TAB:
			CALXComboBoxCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_ESCAPE: 
//			SetWindowText(m_strEditString);
			break;
		case VK_RETURN:
			CALXComboBoxCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(VK_DOWN,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		default:
			CComboBox::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CALXComboBoxCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
//	return;
	switch (nChar)
	{
		case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
			CALXComboBoxCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
			break;
		case VK_DOWN: case VK_UP:
			{
				if(GetKeyState(VK_SHIFT) >= 0)
					GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
				else
					CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_TAB:
			break;
		case VK_RETURN:
			break;
		case VK_LEFT:
			{
//				CString strEditString;
//				GetWindowText(strEditString);
//				if(strEditString.GetLength()<=0||GetCaretPos().x == PosFromChar(0).x)
					CALXComboBoxCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
//				else
//					CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_RIGHT:
			{
//				CString strEditString;
//				GetWindowText(strEditString);
//				if(strEditString.GetLength()==0||GetCaretPos().x > PosFromChar(strEditString.GetLength()-1).x)
					CALXComboBoxCtrl::GetParent()->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
//				else
//					CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
				break;
			}
		default:
		CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void CALXComboBoxCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
			break;
		case VK_DOWN: case VK_UP:
			{
				if(GetKeyState(VK_SHIFT) < 0)
					CComboBox::OnKeyUp(nChar, nRepCnt, nFlags);
				break;
			}
		case VK_TAB:
			break;
		case VK_RETURN:
			break;
		case VK_LEFT:
			break;
		case VK_RIGHT:
			break;
		default:
		CComboBox::OnKeyUp(nChar, nRepCnt, nFlags);
	}
}


BOOL CALXComboBoxCtrl::ActivateCtrl(int x, int y, int cx, int cy)
{
	m_nFirstItemHeight = cy-7;
	MoveWindow(x+1,y+1,cx-1,(cy)*7);
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
	{
		m_EditCtrl.MoveWindow(1,1,cx-cy,cy-3);
	}
	EnableWindow(TRUE);
	CComboBox::SetFocus();
	if(cx-2>0)
		ShowWindow(SW_SHOW);
	return TRUE;
}

BOOL CALXComboBoxCtrl::DeactivateCtrl()
{
	EnableWindow(FALSE);
	ShowWindow(SW_HIDE);
	MoveWindow(0,0,0,0);
	return TRUE;
}

void CALXComboBoxCtrl::SetData(CELL_DATA CtrlData)
{
	if(CtrlData.m_dwTag == -1)
		SetWindowText(CtrlData.m_strText);
	else
	{
		for(int i = 0; i < GetCount(); i++)
		{
			if(CtrlData.m_dwTag == GetItemData(i))
				SetCurSel(i);
		}
	}
	m_Data = CtrlData;
	GetWindowText(m_strText); 
}

CELL_DATA CALXComboBoxCtrl::GetCellData()
{
	GetWindowText(m_Data.m_strText);
	m_Data.m_dwTag = GetItemData(GetCurSel());
	return m_Data;
}

BOOL CALXComboBoxCtrl::OnValidate()
{
	return TRUE;
}

BOOL CALXComboBoxCtrl::IsModifyed()
{
	CString strText;
	GetWindowText(strText);
	if(m_strText == strText)
		return FALSE;
	else
		return TRUE;
}

BOOL CALXComboBoxCtrl::IsActive()
{
	return IsWindowEnabled();
}

BOOL CALXComboBoxCtrl::DestroyCtrl()
{
	return DestroyWindow();
}

BOOL CALXComboBoxCtrl::CreateCtrl(DWORD dwStyle, CWnd *pParentWnd, UINT nID)
{
	CRect rectCtrl(0,0,0,1000);
	if(!Create(dwStyle,rectCtrl,pParentWnd,nID))
		return FALSE;
	return	TRUE;
}

void CALXComboBoxCtrl::SetFontCtrl(CFont *pFont, BOOL bRedraw)
{
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	pFont->GetLogFont(&logfont);
	if(!m_Font.CreateFontIndirect(&logfont))
	{
		TRACE0("Failed to create CALXComboBoxCtrl font\n");
		return;
	}
	SetFont(pFont,bRedraw);
}

void CALXComboBoxCtrl::SetModify(BOOL bModify)
{
	CString strText;
	GetWindowText(strText);
	if(bModify && m_strText == strText)
	{
		if(m_strText != "")
			m_strText = "";
		else
			m_strText = "_";
	}
	else
		m_strText = strText;
}

BOOL CALXComboBoxCtrl::EnableCtrl(BOOL bEnable)
{
	return	EnableWindow(bEnable);
}

void CALXComboBoxCtrl::OnPaint() 
{
 	CPaintDC dc(this); // Контекст устройства для рисования
	CRect rect;
   	// Получаем область перерисовки.
	GetClientRect(&rect);

	//Рисуем рамку
	if(m_pFocusRect != NULL)
		m_pFocusRect->DrawFocusRect(&dc, rect);
	rect.DeflateRect(1,1,1,1);
	if(rect.Width()<=0 || rect.Height() <= 0)
		return;
	// Устанавливает цвет символа текста
	COLORREF clrOldForeground = dc.SetTextColor(::GetSysColor(::GetFocus()==GetSafeHwnd() ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT)); 
	COLORREF clrBackground = ::GetSysColor(::GetFocus()==GetSafeHwnd() ? COLOR_HIGHLIGHT : COLOR_WINDOW); 
	// Устанавливает цвет фона текста
	COLORREF clrOldBackground = dc.SetBkColor( clrBackground);
	dc.FillSolidRect(rect, clrBackground);
	// выбираем шрифт
	::SelectObject(dc.m_hDC,(HFONT)SendMessage(WM_GETFONT,0,0));

    int nItem=GetCurSel();
    CString strText(_T(""));
    if(nItem!=-1)
        GetLBText(nItem,strText);
	// сдвиг слева
//	rect.left++;
	// Выводим текст
	dc.DrawText(strText, rect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
	rect.left = max(rect.left,rect.right - rect.Height());
	if(!IsDroppedDown())
		dc.DrawFrameControl(&rect,DFC_SCROLL,DFCS_SCROLLCOMBOBOX);
	else
		dc.DrawFrameControl(&rect,DFC_SCROLL,DFCS_SCROLLCOMBOBOX | DFCS_PUSHED);

 	dc.SetTextColor(clrOldForeground); 
 	dc.SetBkColor(clrOldBackground); 
    return;
}

LRESULT CALXComboBoxCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	ASSERT_VALID(this);

	if(!m_bInitCtrl)
	{
		m_bInitCtrl=TRUE;
		InitCtrl();
	}
	
	switch(message) 
	{
		case WM_NCPAINT: case WM_PAINT:
		{
			OnPaint();
			return NULL;
			//return CComboBox::WindowProc(message,wParam,lParam);
		}
		case WM_NCDESTROY:
		{
			LRESULT lResult=CComboBox::WindowProc(message,wParam,lParam);
			m_bInitCtrl=FALSE;

			return lResult;
		}
	}

	return CComboBox::WindowProc(message, wParam, lParam);
}

BOOL CALXComboBoxCtrl::InitCtrl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(!SubclassEditCtrl())
		return FALSE;

	return TRUE;
}

BOOL CALXComboBoxCtrl::SubclassEditCtrl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	HWND hWnd;
	GetDlgItem(IDW_ALXCOMBOEDIT,&hWnd);
	if(hWnd!=NULL)
	{
		if(!m_EditCtrl.SubclassWindow(hWnd))
			return FALSE;

	}

	return TRUE;
}

void CALXComboBoxCtrl::OnParentNotify(UINT message, LPARAM lParam) 
{
	CComboBox::OnParentNotify(message, lParam);
	
	if(LOWORD(message)==WM_CREATE)	
	{
		if(HIWORD(message)==IDW_ALXCOMBOEDIT)
			SubclassEditCtrl();
	}
}

void CALXComboBoxCtrl::OnCloseUp() 
{
	Invalidate();
}

void CALXComboBoxCtrl::SendLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_LBUTTONDOWN,nFlags,MAKELPARAM(point.x,point.y));
}

void CALXComboBoxCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	Invalidate();
}

void CALXComboBoxCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CComboBox::OnSetFocus(pOldWnd);
	Invalidate();
}

void CALXComboBoxCtrl::OnSelChange() 
{
	Invalidate();
}


void CALXComboBoxCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CComboBox::OnSize(nType, cx, cy);
	SetItemHeight(-1,m_nFirstItemHeight);
}

BOOL CALXComboBoxCtrl::IsCanCut()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
	{
		int nStartChar, nEndChar;
		m_EditCtrl.GetSel(nStartChar, nEndChar);
		return (nStartChar != nEndChar);
	}
	else
		return FALSE;
}

BOOL CALXComboBoxCtrl::IsCanCopy()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
	{
		int nStartChar, nEndChar;
		m_EditCtrl.GetSel(nStartChar, nEndChar);
		return (nStartChar != nEndChar);
	}
	else
	{
		CString strText;
		CComboBox::GetWindowText(strText);
		return (strText.GetLength() > 0);
	}
}

BOOL CALXComboBoxCtrl::IsCanPaste()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
		return (::IsClipboardFormatAvailable(CF_TEXT));
	else
		return FALSE;

}

BOOL CALXComboBoxCtrl::IsCanUndo()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
		return m_EditCtrl.CanUndo();
	else
		return FALSE;
}

void CALXComboBoxCtrl::Cut()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
		m_EditCtrl.Cut();
}

void CALXComboBoxCtrl::Copy()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
		m_EditCtrl.Copy();
	else
	{
		CString strText;
		GetWindowText(strText);
		SetClipboardText(strText);
	}
}

void CALXComboBoxCtrl::Paste()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
		m_EditCtrl.Paste();

}

void CALXComboBoxCtrl::Undo()
{
	if(::IsWindow(m_EditCtrl.GetSafeHwnd()))
		m_EditCtrl.Undo();
}

