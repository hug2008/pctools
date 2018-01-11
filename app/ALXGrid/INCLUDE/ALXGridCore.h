// ALXGridCore.h : interface of the CALXGridCore class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_GRIDCORE_H_INCLUDED_)
#define ALX_GRIDCORE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxpriv.h>

#include "ALXGridDefinitions.h"
#include "ALXCellCtrl.h"

#define GWM_ACTCOLROWCHANGED	(WM_USER+901)
#define GWM_SELCOLROWRANGE		(WM_USER+906)

#define GWM_COLWIDTHCHANGED		(WM_USER+902)
#define GWM_SAVECHANGED			(WM_USER+903)
#define GWM_GRIDROWCOUNTCHANGED	(WM_USER+904)
#define GWM_SETFOCUS			(WM_USER+905)

/*
 * ALX Grid Styles
 */

#define AGS_NOGRIDLINES			0x001
#define AGS_ROWSELECT			0x002
#define AGS_FLAT				0x004
#define AGS_CUSTOMHEAD_HEIGHT	0x008
#define AGS_CUSTOMFROZEN_WIDTH	0x010
#define AGS_RANGESELECT			0x020

/*
 * ALX DrawImage Types
 */
#define DIT_BITMAP				0x001
#define DIT_ICON				0x002
#define DIT_IMGLIST				0x004

// Функция рисует элемент управления в ячейке.
BOOL DrawCellCtrl(CDC* pDC, CRect& rect, CELL_INFO& CellInfo, CELL_DATA& CellData);

const BYTE offsetXY = 1; // Количество пикселов смещения координат влево вверх.
						 // Используется, чтобы не рисовать левую и верхнюю рамку сетки
						 // т.к. в качестве рамки используется рамка окна.
						 // Для окон без рамок смещение следует установить нулевое.

class CALXGridCore
{
public:
// Construction, Destruction
	CALXGridCore();
	~CALXGridCore();

// Указатель на окно CALXGridView или CALXGridCtrl
	// Передаем указатель на окно сетки
	void SetGridWnd(CWnd* pGridWnd);
	// Получаем указатель на окно сетки
	CWnd* GetGridWnd();

// Устанавливаем стиль и цвет рамки "фокус ввода"
	void SetFocusRect(COLORREF crFaceColor, COLORREF crBkColor = ::GetSysColor(COLOR_WINDOW), BOOL bSolid = TRUE);

protected:

// Рисуем сетку
	virtual void OnGridDraw(CDC* pDC);

// Функции рисования
	// Отображаем кнопку на пересечении ячеек шапки и ячеек неподвижных колонок
	virtual void OnDrawFrozenHeaderBtn(CDC *pDC, CRect &rectCell);
	// Отображаем строку сетки 
	virtual void OnDrawGridRow(CDC* pDC, int nRow, int y, CRect& rectGrid);
	// Отображаем строку шапки 
	void OnDrawHeadRow(CDC* pDC, int nRow, int y, CRect& rectHead);
	// Отображаем закрепленные слева столбцы
	void OnDrawFrozenRow(CDC* pDC, int nRow, int y, CRect& rectFrozen);
	// Отображаем ячейку сетки 
	virtual void OnDrawGridCell(CDC* pDC, int nCol, int nRow, int x, int y);
	// Отображаем ячейку шапки 
	virtual void OnDrawHeadCell(CDC* pDC, int nCol, int nRow, int x, int y);
	// Отображаем ячейку закрепленного слева столбца 
	virtual void OnDrawFrozenCell(CDC* pDC, int nCol, int nRow, int x, int y);
	// Выводим изображение в ячейке
	void DrawImage(CDC* pDC,  int nCol, int nRow, CPoint& pt, CSize& sz, BOOL bSelected);
	// Выводим рамку выделения
	void RedrawFocusRect();
 // Рисуем неподвижную кнопку
	void DrawFrozenBtn(CDC *pDC,  int x, int y, int cx, int cy, BOOL bCellPressed = FALSE, BOOL bDrawArrow = FALSE, BOOL bFlat = FALSE);
	// Обновляем область не относящуюся ни к шапки ни к закрепленным слева столбцам
	void OnUpdateFrozenHeaderRect(CDC *pDC, CRect &rectCell);

// Вычисляем размер области, которую нужно просматривать
	void UpdateScrollSizes();
// Вычисляем и устанавливает позицию и размер элемента управления ячейкой
	void ActivateCtrl(int nCol, int nRow);

// Количество строк в шапке и неподвижных столбцов 
	// Получаем количество неподвижных столбцов
	inline int GetFrozenCol();
	// Получаем количество строк в шапке
	inline int GetHeaderRow();

// Количество строк, столбцов в сетке
	// Получаем количество строк в сетке
	virtual int GetGridRowCount();
	// Получаем количество столбцов в сетке
	virtual int GetHeadColCount(int nHeadRow = -1);

// Ширина колонки	
	// Получаем ширину колонки
	int	 GetColWidth(int nCol, int nHeadRow = -1);
	// Получаем ширину колонки шапки
	virtual int	 GetHeadColWidth(int nCol, int nHeadRow = -1) = 0;
	// Устанавливаем ширину колонки
	virtual void SetHeadColWidth(int nCol, int nRow, int nNewWidth) = 0;

// Изменение ширины колонки в интерактивном режиме
	BOOL IsCursorBetweenCols(int x, int& ResizeCol, int& ResizeRow);
	void EndResizeCol(int x);

// Работа с элементом управления ячейкой
	// Создаем элемент управления ячейкой
	virtual CALXCellCtrl* CreateCellCtrl(int nCol, int nRow, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
	// Устанавливаем элемент управления для данной ячейки
	virtual void SetCellCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl);
	// Передаем данные элементу управления ячейкой
	virtual void SetDataCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl);
	// Получаем указатель на элемент управления ячейкой
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);
	// Получаем стиль элемента управления ячейкой
	virtual DWORD GetCellCtrlStyle(int nCol, int nRow);

// Сохраняем изменения сделанные в ячейке
	virtual BOOL OnSaveCellData(int nCol, int nRow) = 0;
	virtual void OnSavedCellData(int nCol, int nRow) = 0;

// Вывод изображения в ячейке
	// Получаем метрики ячейки
	virtual CALXCellMetrics* GetCellMetrics(int nCol, int nRow);
	// Определяем нужно ли рисовать изображения в ячейке
	virtual BOOL IsImage(int nCol, int nRow);
	// Определяем тип изображения в ячейке
	virtual UINT GetTypeImage(int nCol, int nRow);
	// Определяем размер изображения в ячейке
	virtual BOOL GetSizeImage(int nCol, int nRow, CSize& sz);
	// Получаем указатель на изображение для ячейки
	virtual LPARAM GetImage(int nCol, int nRow, BOOL bSelected);
	// Заполняем структуру для вывода изображения в ячейке
	virtual void GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected);

// Определяем какой ячейке принадлежат координаты
	int YToRow(int y);
	int XToCol(int x, int nHeadRow = -1);

// Активизируем ячейку
	BOOL SetActiveCell(int nCol, int nRow);
	BOOL SetActiveCell(CPoint point);
	virtual void OnSetActiveCell(int nCol, int nRow) {};
	// изменилась активная строка
	virtual void OnChangeActiveRow(int nRow) {};
// Устанавливаем выделение
	BOOL SetSelectCell(int nCol, int nRow);
	BOOL SetSelectCell(CPoint point);
	// Устанавливаем диапозон выделенных ячеек
	void SetSelectRange(int nLeftCol, int nRightCol, int nTopRow, int nButtonRow);
// Сохраняем изменения
	BOOL SaveModifyed(BOOL bSkipErrors = TRUE);

// Получаем строку, столбец сетки
	inline int GetGridRow(int nRow);
	inline int GetGridCol(int nCol);
// Получаем активную колонку, столбец
	inline int GetActiveCol();
	inline int GetActiveRow();

// Получаем информацию о ячейке сетки
	virtual CELL_INFO GetCellInfo(int nCol, int nRow);
	virtual CELL_DATA GetCellData(int nCol, int nRow);

// Обновляем ячейку, строку
	void UpdateCell(CDC* pDC, int nCol, int nRow);
	void UpdateRow(CDC* pDC,int nRow);

	void InvalidateCell(int nCol, int nRow);
	void InvalidateRow(int nRow);

// Получаем информацию о ячейке шапки
	virtual HEADCELL_INFO GetHeadCellInfo(int nCol, int nRow) = 0;

// Прокрутка окна сетки
	void ScrollWindow( int xAmount, int yAmount);
	void ScrollCol( int nCol, int nHeadRow, int xAmount);
	void ScrollRow( int nRow, int yAmount);
// Прокрутка и активизация нужной колонки или столбца
	void ScrollAndSetActRow(int VPos, int nRow, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCol(int HPos, int nCol, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCell(int nCol, int nRow, BOOL RedrawCell = TRUE);
// Прокрутка до нужной колонки или столбца
	void ScrollToRow(int VPos, int nRow);
	void ScrollToCol(int HPos, int nCol);

	void SendLButtonDown(UINT nFlags, CPoint point);

//	m_pGridWnd указатель на окно класса CALXGridView или CALXGridCtrl
	CWnd*   m_pGridWnd;    

//	Изменение ширины колонок в интерактивном режиме
	HCURSOR m_curHResize;		// Курсор изменения ширины колонки
	BOOL	m_EnableResizeCol;	// Изменение ширины колонки возможно
	BOOL	m_BeginResizeCol;	// Идет процесс изменения ширины колонки
	int		m_ResizeCol;		// Номер колонки 
	int		m_ResizeRow;		// Номер строки 

// Строки и столбцы
	int m_nHeadRowHeight;   // высота строки шапки
	int m_nRowHeight;       // высота строки
	int m_nRowWidth;        // ширина строки
	// число подстрок в строке сетки
	int m_nRowLineCount;

	int m_nActiveCol;		// номер выбранного столбца
	int m_nActiveRow;		// номер выбранной строки

	int m_nActiveSelCol;		// номер последнего выделенного столбца
	int m_nActiveSelRow;		// номер последней выделенной строки

// Сортировка строк
	int m_nSortHeadCol;		// индекс колонки шапки
	int m_nSortHeadRow;		// индекс строки шапки
	ALXSortOrder m_nSortOrder;	// по убыванию или возрастанию

// Шрифт
	CFont	 m_Font;		// шрифт который используется в сетке
	CFont*	 m_pHeadFont;	// указатель на шрифт шапки

// Метрики Сетки
	CSize m_sizeTotal;      // размер сетки
	CSize m_sizePage;       // размер страницы
	CSize m_sizeLine;       // размер строки
	CSize m_sizeExtArea;	// размер дополнительной области	

// Прокрутка
	BOOL	m_bInsideUpdate; // internal state for OnSize callback
	CPoint	m_CurScrollPos;	 // Текущая позиция виртуального окна
	float	m_nVScrollFactor;// scrolling factor (if m_sizeTotal more than 32768 points, it is > 1)
	float	m_nHScrollFactor;// scrolling factor (if m_sizeTotal more than 32768 points, it is > 1)

// Шапка и неподвижные колонки
	int m_CountHeaderRows;	// количество строк в шапке
	int m_CountFrozenCols;	// количество колонок закрепленных слева
	int m_FrozenColWidth;	// ширина колонок закрепленных слева

	int m_nHeaderColPressed;// индекс колонки нажатой кнопки в шапке
	int m_nHeaderRowPressed;// индекс строки нажатой кнопки в шапке
	int m_nFrozenColPressed;// индекс нажатой закрепленной колонки
	int m_nFrozenRowPressed;// индекс строки нажатой закрепленной колонки

	BOOL m_bSelAllBtnPressed;// нажата ли кнопка 'Выделить всё'

// Цвета и параметры отображения ячеек
	CALXCellMetrics m_MetricsCells;

// Доапозон выделенных ячеек
	struct ALXSelRangeCell
	{
		int m_LeftCol;
		int m_TopRow;
		int m_RightCol;
		int m_BottomRow;
		// функции
		inline BOOL InRange(int nCol, int nRow);	// принадлежит ли выделнению
		inline BOOL IsEmpty();						// не пустой ли диапозон
	} m_SelRangeCell;

private:
	// Указатель на элемент управления сетки
	CALXCellCtrl* m_pCellCtrl;

	// Получаем общее количество строк
	inline int GetAllRowCount();
	// Получаем общее количество столбцов
	inline int GetAllColCount(int nHeadRow = -1);

	// Получаем размер шрифта
	CSize GetFontSize(CFont* pFont);
	// Вычисляем метрики сетки
	void CalculateGridMetrics();

// Функции вычисления позиции и диапазона
	// Верхняя позиция строки в окне
	int TopWndPos(int nRow);
	// Левая позиция столбца в окне
	int LeftWndPos(int nCol, int nHeadRow = -1);
	// Верхняя позиция строки
	int TopRowPos(int nRow);
	// Левая позиция столбца
	int LeftColPos(int nCol, int nHeadRow = -1);
	// Определяем диапазон строк, которые попадают в данную область
	void RectToRowRange(const CRect &GridRect, int &nFirstRow, int &nLastRow, BOOL bIncludePartiallyShownRows);
	// Определяем диапазон столбцов, которые попадают в данную область
	void RectToColRange(const CRect &GridRect, int &nFirstCol, int &nLastCol, BOOL bIncludePartiallyShownCols, int nHeadRow = -1);

// Функции получения областей сетки из области окна
	// Преобразуем область окна в область сетки
	CRect GetGridRect(CRect& rectPaint);
	// Преобразуем область окна в область шапки
	CRect GetHeadRect(CRect& rectPaint);
	// Преобразуем область окна в область закрепленных слева колонок
	CRect GetFrozenRect(CRect& rectPaint);

// Функции прокрутки
	// Устанавливаем размер области, которую нужно просматривать
	void SetScrollSizes(const SIZE &sizeTotal);
	// Определяем требуются ли полосы прокрутки, а также получаем размер рабочей области и размер полос прокрутки
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	// Определяем состояние полос прокрутки
	void GetScrollBarState(CSize sizeClient, CSize& needSb, CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);
	// Определяем размер полос прокрутки
	void GetScrollBarSizes(CSize& sizeSb);
	// Обновляем полосы прокрутки
	void UpdateBars();
	// Переходим к позиции ptPos
	void ScrollToPos(POINT ptPos);

// Функции обновления окна
	// Обновляем область окна
	void OnUpdateRect(CDC* pDC, CRect& rectClip);
	// Обновляем область шапки
	void OnUpdateHeaderRect(CDC *pDC, CRect &rectClip);
	// Обновляем область закрепленных слева столбцов
	void OnUpdateFrozenRect(CDC *pDC, CRect &rectClip);
	// Обновляем область сетки
	void OnUpdateGridRect(CDC *pDC, CRect &rectClip);
// Рисуем стрелки сортировки
	void DrawArrow(CDC *pDC, CPoint pos, CSize sz);
};
/////////////////////////////////////////////////////////////////////////////

#include "ALXGridCore.inl"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_GRIDCORE_H_INCLUDED_)
