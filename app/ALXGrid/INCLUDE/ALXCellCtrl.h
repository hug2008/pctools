// ALXCellCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_CELLCTRL_H_INCLUDED_)
#define ALX_CELLCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridDefinitions.h"

// Функция передает в буфер обмена текст
BOOL SetClipboardText(LPCTSTR pcsz);
// Функция получает из буфера обмена текст
BOOL GetClipboardText(CString& s);

/////////////////////////////////////////////////////////////////////////////
// CALXCellCtrl

class CALXCellCtrl
{
// Construction, Destruction
public:
	CALXCellCtrl();
	~CALXCellCtrl();

	//Рамка
	CALXFocusRect* m_pFocusRect;

// Attributes
protected:
	CELL_DATA m_Data;

// Operations
public:
	// Посылается окном сетки при нажатии левой кнопки мыши
	virtual void SendLButtonDown(UINT nFlags, CPoint point);
	// Установка шрифта
	virtual void SetFontCtrl( CFont* pFont, BOOL bRedraw = TRUE );
	// Устанавливается флаг модификации
	virtual void SetModify(BOOL bModify);
	// Делает элемент управления доступным/недоступным
	virtual BOOL EnableCtrl(BOOL bEnable = TRUE);
	// Создает окно элемента управления
	virtual BOOL CreateCtrl(DWORD dwStyle, CWnd* pParentWnd, UINT nID);
	// Активизирует элемент управления
	virtual BOOL ActivateCtrl(int x, int y, int cx, int cy);
	// Деактивизирует элемент управления
	virtual BOOL DeactivateCtrl();
	// Вызывается окном сетки для передачи данных в ячейку
	virtual void SetData(CELL_DATA CtrlData);
	// Вызывается окном сетки для получения данных из ячейки
	virtual CELL_DATA GetCellData();
	// Определяет были ли сделаны изменения
	virtual BOOL IsModifyed();
	// Определяет корректно ли введены данные
	virtual BOOL OnValidate();
	// Разрушает окно элемента управления
	virtual BOOL DestroyCtrl();
	// Определяет активно ли окно элемента управления
	virtual BOOL IsActive();

	// Устанавливается указатель на параметры рамки "фокус ввода"
	void SetFocusRect(CALXFocusRect* pFocusRect);

// Функции для работы с буфером обмена
	virtual BOOL IsCanCut();
	virtual BOOL IsCanCopy();
	virtual BOOL IsCanPaste();
	virtual BOOL IsCanUndo();
	virtual void Cut();
	virtual void Copy();
	virtual void Paste();
	virtual void Undo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_CELLCTRL_H_INCLUDED_)
