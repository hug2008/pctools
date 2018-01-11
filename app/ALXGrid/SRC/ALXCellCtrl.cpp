// ALXCellCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ALXCellCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Функция передает в буфер обмена текст
BOOL SetClipboardText(LPCTSTR pcsz)
{
	static UINT uiFmt = (sizeof(TCHAR) == sizeof(char)) ? CF_TEXT : CF_UNICODETEXT;

	if (pcsz != NULL && *pcsz != _T('\0'))
	{
		int iLength = (lstrlen(pcsz) + 1) * sizeof(TCHAR);
		HANDLE h = GlobalAlloc(GHND | GMEM_SHARE, iLength);
		LPTSTR p = (LPTSTR)GlobalLock(h);
		if (p != NULL)
		{
			lstrcpy(p, pcsz);
			GlobalUnlock(h);

			if (OpenClipboard(NULL))
			{
				if (EmptyClipboard())
				{
					SetClipboardData(uiFmt, h);
					h = NULL;
				}

				CloseClipboard();
				if (h != NULL)
					GlobalFree(h);
				else
					return TRUE;
			}
		}
	}

	return FALSE;
}

// Функция получает из буфера обмена текст
BOOL GetClipboardText(CString& s)
{
	static UINT uiFmt = (sizeof(TCHAR) == sizeof(char)) ? CF_TEXT : CF_UNICODETEXT;

	if (IsClipboardFormatAvailable(uiFmt))
	{
		if (OpenClipboard(NULL))
		{
			HANDLE h = GetClipboardData(uiFmt);
			LPCTSTR p = (LPCTSTR)GlobalLock(h);
			s = p;
			GlobalUnlock(h);
			CloseClipboard();
			return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CALXCellCtrl

// Конструктор
CALXCellCtrl::CALXCellCtrl()
{
	m_pFocusRect = NULL;
}

// Деструктор
CALXCellCtrl::~CALXCellCtrl()
{

}

/////////////////////////////////////////////////////////////////////////////

BOOL CALXCellCtrl::ActivateCtrl(int x, int y, int cx, int cy)
{
	return TRUE;
}

BOOL CALXCellCtrl::DeactivateCtrl()
{
	return TRUE;
}

void CALXCellCtrl::SetData(CELL_DATA CtrlData)
{
	m_Data = CtrlData;
}

BOOL CALXCellCtrl::OnValidate()
{
	return TRUE;
}

BOOL CALXCellCtrl::IsModifyed()
{
	return FALSE;
}

BOOL CALXCellCtrl::DestroyCtrl()
{

	return TRUE;
}

BOOL CALXCellCtrl::CreateCtrl(DWORD dwStyle, CWnd* pParentWnd, UINT nID)
{
	return TRUE;
}

BOOL CALXCellCtrl::EnableCtrl(BOOL bEnable)
{
	return TRUE;
}

void CALXCellCtrl::SetFontCtrl(CFont *pFont, BOOL bRedraw)
{

}

void CALXCellCtrl::SetModify(BOOL bModify)
{

}

// Устанавливается указатель на параметры рамки "фокус ввода"
void CALXCellCtrl::SetFocusRect(CALXFocusRect* pFocusRect)
{
	m_pFocusRect = pFocusRect;
}

CELL_DATA CALXCellCtrl::GetCellData()
{
	return m_Data;
}

void CALXCellCtrl::SendLButtonDown(UINT nFlags, CPoint point)
{

}

BOOL CALXCellCtrl::IsActive()
{
	return FALSE;
}

BOOL CALXCellCtrl::IsCanCut()
{
	return FALSE;
}

BOOL CALXCellCtrl::IsCanCopy()
{
	return FALSE;
}

BOOL CALXCellCtrl::IsCanPaste()
{
	return FALSE;
}

BOOL CALXCellCtrl::IsCanUndo()
{
	return FALSE;
}

void CALXCellCtrl::Cut()
{

}

void CALXCellCtrl::Copy()
{

}

void CALXCellCtrl::Paste()
{

}

void CALXCellCtrl::Undo()
{

}
