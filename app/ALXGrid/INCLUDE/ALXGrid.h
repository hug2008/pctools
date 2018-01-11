// ALXGrid.h : interface of the CALXGrid class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_GRID_H_INCLUDED_)
#define ALX_GRID_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>
#include "ALXGridCore.h"
#include "ALXEditCtrl.h"
#include "ALXButtonCtrl.h"
#include "ALXComboBoxCtrl.h"

/*
 * ALX Cell Format Flags
 */
#define ACFF_LEFT			DT_LEFT | DT_VCENTER | DT_NOPREFIX
#define ACFF_CENTER			DT_CENTER | DT_VCENTER | DT_NOPREFIX
#define ACFF_RIGHT			DT_RIGHT | DT_VCENTER | DT_NOPREFIX
#define ACFF_END_ELLIPSIS	DT_END_ELLIPSIS

/*
 * ALX Head Format Flags
 */
#define AHFF_LEFT		DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE
#define AHFF_CENTER		DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE
#define AHFF_RIGHT		DT_RIGHT | DT_VCENTER | DT_END_ELLIPSIS| DT_SINGLELINE

// Структура содержащая информацию о колонке
typedef struct ColInfo
{
	// Структура содержащая информацию о шапке
	struct ColHeadInfo
	{
		CString	m_strHead; // Текст в ячейке шапки
		UINT	m_nFormat; // Формат текста
	} m_HeadInfo;
	// Структура содержащая общую информацию о ячейках колонки
	struct ColCellsInfo
	{
		UINT	m_nFormat;			// Формат текста
		UINT	m_nTypeFrameCtrl;	// Тип элемента рисуемого управления
		UINT	m_nStateFrameCtrl;	// Состояние рисуемого элемента управления
		UINT	m_nTypeImage;		// Тип изображения
		CSize	m_sizeImage;		// Размер изображения
	} m_CellsInfo;
	// Структура содержащая общую информацию об элементе управления для данной колонки
	struct ColCtrlInfo
	{
		BYTE	m_nType;			// Тип элемента управления
		DWORD	m_dwStyle;			// Стиль 
		UINT	m_nID;				
		CALXCellCtrl* m_pColCtrl;	// Указатель на элемент управления
	} m_CtrlInfo;
	// Ширина колонки
	int		m_nWidth;
} COL_INFO;

class CALXGrid : protected CALXGridCore
{

friend class CXGridView;
friend class CXGridCtrl;
friend class CALXSplitterWnd;

public:
// Construction, Destruction
	CALXGrid();
	~CALXGrid();
// Добавляем колонку
	int AddCol(	int nWidth,				// ширина колонки
				CString strHead,		// заголовок шапки
				UINT nCellsFormat	= ACFF_LEFT,	// формат колонок (см. CDC::DrawText(...))
				UINT nHeadFormat	= AHFF_CENTER,	// формат шапки (см. CDC::DrawText(...))
				UINT nTypeDrawCtrl	= 0,// Тип рисуемого элемента управления (см. CDC::DrawFrameControl(...))
				UINT nStateFrameCtrl= 0,// Состояние рисуемого элемента управления (см. CDC::DrawFrameControl(...))
				UINT nID = NULL			// Идентификатор колонки
				);
	int AddCol(COL_INFO srColInfo);
// Вставляем колонку
	int InsertCol( int nCol,				// индекс колонки
				   int nWidth,				// ширина колонки
				   CString strHead,			// заголовок шапки
				   UINT nCellsFormat = ACFF_LEFT,	// формат колонок (см. CDC::DrawText(...))
				   UINT nHeadFormat = AHFF_CENTER,	// формат шапки (см. CDC::DrawText(...))
				   UINT nTypeDrawCtrl = 0,	// Тип рисуемого элемента управления (см. CDC::DrawFrameControl(...))
				   UINT nStateFrameCtrl = 0,// Состояние рисуемого элемента управления (см. CDC::DrawFrameControl(...))
				   UINT nID = NULL			// Идентификатор колонки
				   );
// Удаляем все колонки
	void RemoveAllCol();

// Вставляем строки
	void InsertRows(int nRow, int nCount = 1);
// Удаляем строки
	void RemoveRows(int nRow, int nCount = 1);

// Определяем размер изображения для данной колонки
	BOOL DefineImage(int nCol, int cx, int cy, UINT nTypeImage = DIT_BITMAP);
// Определяем элемент управления для данной колонки
	BOOL DefineColCtrl(int nCol, BYTE CtrlType, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
// Разрушает элемент управления для данной колонки
	virtual BOOL DestroyColCtrl(int nCol);

// Рисуем Сетку
	void OnGridDraw(CPaintDC* pDC);

// Прокрутка и активизация нужной колонки или строки
	void ScrollAndSetActRow(int VPos, int nRow, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCol(int HPos, int nCol, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCell(int nCol, int nRow, BOOL RedrawCell = TRUE);
// Прокрутка до нужной колонки или строки
	void ScrollToRow(int VPos, int nRow);
	void ScrollToCol(int HPos, int nCol);

// Boзвращает текущую позицию 
	CPoint GetCurScrollPos();
// Вычисляем размер области, которую нужно просматривать
	void UpdateScrollSizes();
// Сохраняем изменения
	BOOL SaveModifyed(BOOL bSkipErrors = TRUE);
// Изменения сделанные в ячейке сохранены
	virtual void OnSavedCellData(int nCol, int nRow);

// Получаем активную строку, столбец
	inline int GetActiveCol();
	inline int GetActiveRow();

// Активизируем ячейку, строку, столбец
	inline BOOL SetActiveCell(int nCol, int nRow);
	inline BOOL SetActiveNextCol( void );
	inline BOOL SetActivePrevCol( void );
	inline BOOL SetActiveNextRow( void );
	inline BOOL SetActivePrevRow( void );
	inline BOOL SetActiveCol(int nNewActiveCol);
	inline BOOL SetActiveRow(int nNewActiveRow);

// Выделяем ячейки
	// Устанавливаем диапозон выделенных ячеек
	void SetSelectRange(int nLeftCol, int nRightCol, int nTopRow, int nButtonRow);
	// Получаем диапозон выделенных ячеек
	void GetSelectRange(int& nLeftCol, int& nRightCol, int& nTopRow, int& nButtonRow);

// Обновляем ячейку, строку
	void InvalidateCell(int nCol, int nRow);
	void InvalidateRow(int nRow);
	void InvalidateFrozenCell(int nCol, int nRow);

	// Получаем количество строк в сетке
	virtual int GetGridRowCount();
	// Устанавливаем количество строк в сетке
	void SetGridRowCount(int NewRowCount, BOOL bSendMsg = TRUE);
	// Получаем высоту строки сетки
	int GetGridRowHeight();
	// Устанавливаем количество подстрок в строке
	void SetRowLineCount(int nNewLineCount);

	// Получаем количество строк в шапке
	virtual int GetHeadRowCount();
	// Устанавливаем количество строк в шапке
	void SetHeadRowCount(int NewRowCount, BOOL bRecalcMetrics = TRUE);

	// Получаем количество неподвижных столбцов
	virtual int GetFrozenColCount();
	// Устанавливаем количество неподвижных столбцов
	void SetFrozenColCount(int NewColCount, BOOL bRecalcMetrics = TRUE);

// Высота шапки и ширина неподвижных колонок
	// Устанавливаем высоту строки шапки
	void SetHeadRowHeight(int NewHeadRowHeight, BOOL bRecalcMetrics = TRUE);
	// Получаем высоту строки шапки
	int GetHeadRowHeight();
	// Устанавливаем ширину неподвижных столбцов
	void SetFrozenColWidth(int NewFrozenColWidth, BOOL bRecalcMetrics = TRUE);
	// Получаем ширину неподвижных столбцов
	int GetFrozenColWidth();

// Количество столбцов в сетке
	// Получаем количество столбцов в сетке
	virtual int GetHeadColCount(int nHeadRow = -1);

// Ширина колонки	
	// Получаем ширину колонки
	virtual int	 GetHeadColWidth(int nCol, int nHeadRow = -1);
	// Устанавливаем ширину колонки
	virtual void SetHeadColWidth(int nCol, int nRow, int nNewWidth);

// Шрифт сетки и шапки
	// Устанавливаем шрифт шапки
	void SetHeadFont(CFont* pFont, BOOL bRecalcMetrics = TRUE);
	// Получаем шрифт шапки
	CFont* GetHeadFont();
	// Устанавливаем шрифт сетки
	void SetGridFont(CFont* pFont, BOOL bRecalcMetrics = TRUE);
	// Получаем шрифт сетки
	CFont* GetGridFont();

// Размер дополнительной прокручиваемой области
	// Устанавливаем размер дополнительной области
	void SetExtAreaSize(CSize& sizeNewExtArea, BOOL bRecalcMetrics = TRUE);
	// Получаем размер дополнительной области
	CSize GetExtAreaSize();

// Нажaта кнопка 'Выделить всё'
	virtual void OnSelAllButtonDown();
// Нажaта кнопка шапки
	virtual void OnHeadButtonDown(int nHeadCol, int nHeadRow);
// Отпущена кнопка шапки
	virtual void OnHeadButtonUp(int nHeadCol, int nHeadRow);
// Нажaта кнопка неподвижной колонки
	virtual void OnFrozenButtonDown(int nFrozenCol, int nFrozenRow);
// Отпущена кнопка неподвижной колонки
	virtual void OnFrozenButtonUp(int nFrozenCol, int nFrozenRow);
// Двойное нажатие кнопки шапки
	virtual void OnHeadButtonDblClk(int nHeadCol, int nHeadRow);
// Двойное нажатие кнопки неподвижной колонки
	virtual void OnFrozenButtonDblClk(int nFrozenCol, int nFrozenRow);
// Сортировка строк
	void SetSortCol(int nHeadCol, int nHeadRow, ALXSortOrder nSortOrder);
	void GetSortCol(int& nHeadCol, int& nHeadRow, ALXSortOrder& nSortOrder);

protected:

	bool m_bSetFocus; // Установлен ли фокус

// Функции обработки событий
	// Перемещение мышки
	BOOL OnMouseMove(UINT nFlags, CPoint point);
	// Крутится колесико мышки
	BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point, BOOL bSetFocus = TRUE);
	// Установка указателя мышки
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	// Двойное нажатие на левую кнопку мышки
	BOOL OnLButtonDblClk(UINT nFlags, CPoint point);
	// Левая кнопка мышки нажата
	BOOL OnLButtonDown(UINT nFlags, CPoint point);
	// Левая кнопка мышки отпущена
	BOOL OnLButtonUp(UINT nFlags, CPoint point);
	// Нажата клавиша
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// Окно деинициализируется
	void OnDestroy();
	// Окно получило фокус
	void OnSetFocus(CWnd* pOldWnd);

// Функции прокрутки
	BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll);

// Функция возвращает информацию о колонке
	COL_INFO GetColInfo(int nCol);

//*** Виртуальные функции вызываемые из класса CALXGridCore ***//

// Работа с элементом управления ячейки
	// Создаем элемент управления ячейкой
	virtual CALXCellCtrl* CreateCellCtrl(int nCol, int nRow, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
	// Устанавливаем элемент управления для данной ячейки
	virtual void SetCellCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl);
	// Получаем элемент управления ячейкой
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);
	// Получаем ID элемента управления ячейкой
	virtual UINT GetCellCtrlID(int nCol, int nRow);
	// Получаем стиль элемента управления ячейкой
	virtual DWORD GetCellCtrlStyle(int nCol, int nRow);

// Сохраняем изменения сделанные в ячейке
	virtual BOOL OnSaveCellData(int nCol, int nRow);

// Вывод изображения в ячейке
	// Определяем нужно ли рисовать изображение в ячейке
	virtual BOOL IsImage(int nCol, int nRow);
	// Определяем тип изображения в ячейке
	virtual UINT GetTypeImage(int nCol, int nRow);
	// Определяем размер изображения в ячейке
	virtual BOOL GetSizeImage(int nCol, int nRow, CSize& sz);
	// Получаем изображение для ячейки
	virtual LPARAM GetImage(int nCol, int nRow, BOOL bSelected);
	// Получаем информацию о изображении для ячейки
	virtual void GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected);

// Получаем информацию о ячейке сетки
	virtual CELL_INFO GetCellInfo(int nCol, int nRow);
	virtual CELL_DATA GetCellData(int nCol, int nRow);

// Получаем информацию о ячейке шапки
	virtual HEADCELL_INFO GetHeadCellInfo(int nCol, int nRow);

private:
// Структура шапки
	struct HeaderInfo
	{
		CArray < COL_INFO, COL_INFO& > m_Cols;
	} m_HeaderInfo;
// Последние координаты разделительной линии
	CRect m_rectSaveInv;
// Количество строк в сетке
	int m_GridRowCount;
};
/////////////////////////////////////////////////////////////////////////////

#include "ALXGrid.inl"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_GRID_H_INCLUDED_)
