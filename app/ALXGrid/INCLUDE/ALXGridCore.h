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

// ������� ������ ������� ���������� � ������.
BOOL DrawCellCtrl(CDC* pDC, CRect& rect, CELL_INFO& CellInfo, CELL_DATA& CellData);

const BYTE offsetXY = 1; // ���������� �������� �������� ��������� ����� �����.
						 // ������������, ����� �� �������� ����� � ������� ����� �����
						 // �.�. � �������� ����� ������������ ����� ����.
						 // ��� ���� ��� ����� �������� ������� ���������� �������.

class CALXGridCore
{
public:
// Construction, Destruction
	CALXGridCore();
	~CALXGridCore();

// ��������� �� ���� CALXGridView ��� CALXGridCtrl
	// �������� ��������� �� ���� �����
	void SetGridWnd(CWnd* pGridWnd);
	// �������� ��������� �� ���� �����
	CWnd* GetGridWnd();

// ������������� ����� � ���� ����� "����� �����"
	void SetFocusRect(COLORREF crFaceColor, COLORREF crBkColor = ::GetSysColor(COLOR_WINDOW), BOOL bSolid = TRUE);

protected:

// ������ �����
	virtual void OnGridDraw(CDC* pDC);

// ������� ���������
	// ���������� ������ �� ����������� ����� ����� � ����� ����������� �������
	virtual void OnDrawFrozenHeaderBtn(CDC *pDC, CRect &rectCell);
	// ���������� ������ ����� 
	virtual void OnDrawGridRow(CDC* pDC, int nRow, int y, CRect& rectGrid);
	// ���������� ������ ����� 
	void OnDrawHeadRow(CDC* pDC, int nRow, int y, CRect& rectHead);
	// ���������� ������������ ����� �������
	void OnDrawFrozenRow(CDC* pDC, int nRow, int y, CRect& rectFrozen);
	// ���������� ������ ����� 
	virtual void OnDrawGridCell(CDC* pDC, int nCol, int nRow, int x, int y);
	// ���������� ������ ����� 
	virtual void OnDrawHeadCell(CDC* pDC, int nCol, int nRow, int x, int y);
	// ���������� ������ ������������� ����� ������� 
	virtual void OnDrawFrozenCell(CDC* pDC, int nCol, int nRow, int x, int y);
	// ������� ����������� � ������
	void DrawImage(CDC* pDC,  int nCol, int nRow, CPoint& pt, CSize& sz, BOOL bSelected);
	// ������� ����� ���������
	void RedrawFocusRect();
 // ������ ����������� ������
	void DrawFrozenBtn(CDC *pDC,  int x, int y, int cx, int cy, BOOL bCellPressed = FALSE, BOOL bDrawArrow = FALSE, BOOL bFlat = FALSE);
	// ��������� ������� �� ����������� �� � ����� �� � ������������ ����� ��������
	void OnUpdateFrozenHeaderRect(CDC *pDC, CRect &rectCell);

// ��������� ������ �������, ������� ����� �������������
	void UpdateScrollSizes();
// ��������� � ������������� ������� � ������ �������� ���������� �������
	void ActivateCtrl(int nCol, int nRow);

// ���������� ����� � ����� � ����������� �������� 
	// �������� ���������� ����������� ��������
	inline int GetFrozenCol();
	// �������� ���������� ����� � �����
	inline int GetHeaderRow();

// ���������� �����, �������� � �����
	// �������� ���������� ����� � �����
	virtual int GetGridRowCount();
	// �������� ���������� �������� � �����
	virtual int GetHeadColCount(int nHeadRow = -1);

// ������ �������	
	// �������� ������ �������
	int	 GetColWidth(int nCol, int nHeadRow = -1);
	// �������� ������ ������� �����
	virtual int	 GetHeadColWidth(int nCol, int nHeadRow = -1) = 0;
	// ������������� ������ �������
	virtual void SetHeadColWidth(int nCol, int nRow, int nNewWidth) = 0;

// ��������� ������ ������� � ������������� ������
	BOOL IsCursorBetweenCols(int x, int& ResizeCol, int& ResizeRow);
	void EndResizeCol(int x);

// ������ � ��������� ���������� �������
	// ������� ������� ���������� �������
	virtual CALXCellCtrl* CreateCellCtrl(int nCol, int nRow, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
	// ������������� ������� ���������� ��� ������ ������
	virtual void SetCellCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl);
	// �������� ������ �������� ���������� �������
	virtual void SetDataCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl);
	// �������� ��������� �� ������� ���������� �������
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);
	// �������� ����� �������� ���������� �������
	virtual DWORD GetCellCtrlStyle(int nCol, int nRow);

// ��������� ��������� ��������� � ������
	virtual BOOL OnSaveCellData(int nCol, int nRow) = 0;
	virtual void OnSavedCellData(int nCol, int nRow) = 0;

// ����� ����������� � ������
	// �������� ������� ������
	virtual CALXCellMetrics* GetCellMetrics(int nCol, int nRow);
	// ���������� ����� �� �������� ����������� � ������
	virtual BOOL IsImage(int nCol, int nRow);
	// ���������� ��� ����������� � ������
	virtual UINT GetTypeImage(int nCol, int nRow);
	// ���������� ������ ����������� � ������
	virtual BOOL GetSizeImage(int nCol, int nRow, CSize& sz);
	// �������� ��������� �� ����������� ��� ������
	virtual LPARAM GetImage(int nCol, int nRow, BOOL bSelected);
	// ��������� ��������� ��� ������ ����������� � ������
	virtual void GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected);

// ���������� ����� ������ ����������� ����������
	int YToRow(int y);
	int XToCol(int x, int nHeadRow = -1);

// ������������ ������
	BOOL SetActiveCell(int nCol, int nRow);
	BOOL SetActiveCell(CPoint point);
	virtual void OnSetActiveCell(int nCol, int nRow) {};
	// ���������� �������� ������
	virtual void OnChangeActiveRow(int nRow) {};
// ������������� ���������
	BOOL SetSelectCell(int nCol, int nRow);
	BOOL SetSelectCell(CPoint point);
	// ������������� �������� ���������� �����
	void SetSelectRange(int nLeftCol, int nRightCol, int nTopRow, int nButtonRow);
// ��������� ���������
	BOOL SaveModifyed(BOOL bSkipErrors = TRUE);

// �������� ������, ������� �����
	inline int GetGridRow(int nRow);
	inline int GetGridCol(int nCol);
// �������� �������� �������, �������
	inline int GetActiveCol();
	inline int GetActiveRow();

// �������� ���������� � ������ �����
	virtual CELL_INFO GetCellInfo(int nCol, int nRow);
	virtual CELL_DATA GetCellData(int nCol, int nRow);

// ��������� ������, ������
	void UpdateCell(CDC* pDC, int nCol, int nRow);
	void UpdateRow(CDC* pDC,int nRow);

	void InvalidateCell(int nCol, int nRow);
	void InvalidateRow(int nRow);

// �������� ���������� � ������ �����
	virtual HEADCELL_INFO GetHeadCellInfo(int nCol, int nRow) = 0;

// ��������� ���� �����
	void ScrollWindow( int xAmount, int yAmount);
	void ScrollCol( int nCol, int nHeadRow, int xAmount);
	void ScrollRow( int nRow, int yAmount);
// ��������� � ����������� ������ ������� ��� �������
	void ScrollAndSetActRow(int VPos, int nRow, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCol(int HPos, int nCol, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCell(int nCol, int nRow, BOOL RedrawCell = TRUE);
// ��������� �� ������ ������� ��� �������
	void ScrollToRow(int VPos, int nRow);
	void ScrollToCol(int HPos, int nCol);

	void SendLButtonDown(UINT nFlags, CPoint point);

//	m_pGridWnd ��������� �� ���� ������ CALXGridView ��� CALXGridCtrl
	CWnd*   m_pGridWnd;    

//	��������� ������ ������� � ������������� ������
	HCURSOR m_curHResize;		// ������ ��������� ������ �������
	BOOL	m_EnableResizeCol;	// ��������� ������ ������� ��������
	BOOL	m_BeginResizeCol;	// ���� ������� ��������� ������ �������
	int		m_ResizeCol;		// ����� ������� 
	int		m_ResizeRow;		// ����� ������ 

// ������ � �������
	int m_nHeadRowHeight;   // ������ ������ �����
	int m_nRowHeight;       // ������ ������
	int m_nRowWidth;        // ������ ������
	// ����� �������� � ������ �����
	int m_nRowLineCount;

	int m_nActiveCol;		// ����� ���������� �������
	int m_nActiveRow;		// ����� ��������� ������

	int m_nActiveSelCol;		// ����� ���������� ����������� �������
	int m_nActiveSelRow;		// ����� ��������� ���������� ������

// ���������� �����
	int m_nSortHeadCol;		// ������ ������� �����
	int m_nSortHeadRow;		// ������ ������ �����
	ALXSortOrder m_nSortOrder;	// �� �������� ��� �����������

// �����
	CFont	 m_Font;		// ����� ������� ������������ � �����
	CFont*	 m_pHeadFont;	// ��������� �� ����� �����

// ������� �����
	CSize m_sizeTotal;      // ������ �����
	CSize m_sizePage;       // ������ ��������
	CSize m_sizeLine;       // ������ ������
	CSize m_sizeExtArea;	// ������ �������������� �������	

// ���������
	BOOL	m_bInsideUpdate; // internal state for OnSize callback
	CPoint	m_CurScrollPos;	 // ������� ������� ������������ ����
	float	m_nVScrollFactor;// scrolling factor (if m_sizeTotal more than 32768 points, it is > 1)
	float	m_nHScrollFactor;// scrolling factor (if m_sizeTotal more than 32768 points, it is > 1)

// ����� � ����������� �������
	int m_CountHeaderRows;	// ���������� ����� � �����
	int m_CountFrozenCols;	// ���������� ������� ������������ �����
	int m_FrozenColWidth;	// ������ ������� ������������ �����

	int m_nHeaderColPressed;// ������ ������� ������� ������ � �����
	int m_nHeaderRowPressed;// ������ ������ ������� ������ � �����
	int m_nFrozenColPressed;// ������ ������� ������������ �������
	int m_nFrozenRowPressed;// ������ ������ ������� ������������ �������

	BOOL m_bSelAllBtnPressed;// ������ �� ������ '�������� ��'

// ����� � ��������� ����������� �����
	CALXCellMetrics m_MetricsCells;

// �������� ���������� �����
	struct ALXSelRangeCell
	{
		int m_LeftCol;
		int m_TopRow;
		int m_RightCol;
		int m_BottomRow;
		// �������
		inline BOOL InRange(int nCol, int nRow);	// ����������� �� ����������
		inline BOOL IsEmpty();						// �� ������ �� ��������
	} m_SelRangeCell;

private:
	// ��������� �� ������� ���������� �����
	CALXCellCtrl* m_pCellCtrl;

	// �������� ����� ���������� �����
	inline int GetAllRowCount();
	// �������� ����� ���������� ��������
	inline int GetAllColCount(int nHeadRow = -1);

	// �������� ������ ������
	CSize GetFontSize(CFont* pFont);
	// ��������� ������� �����
	void CalculateGridMetrics();

// ������� ���������� ������� � ���������
	// ������� ������� ������ � ����
	int TopWndPos(int nRow);
	// ����� ������� ������� � ����
	int LeftWndPos(int nCol, int nHeadRow = -1);
	// ������� ������� ������
	int TopRowPos(int nRow);
	// ����� ������� �������
	int LeftColPos(int nCol, int nHeadRow = -1);
	// ���������� �������� �����, ������� �������� � ������ �������
	void RectToRowRange(const CRect &GridRect, int &nFirstRow, int &nLastRow, BOOL bIncludePartiallyShownRows);
	// ���������� �������� ��������, ������� �������� � ������ �������
	void RectToColRange(const CRect &GridRect, int &nFirstCol, int &nLastCol, BOOL bIncludePartiallyShownCols, int nHeadRow = -1);

// ������� ��������� �������� ����� �� ������� ����
	// ����������� ������� ���� � ������� �����
	CRect GetGridRect(CRect& rectPaint);
	// ����������� ������� ���� � ������� �����
	CRect GetHeadRect(CRect& rectPaint);
	// ����������� ������� ���� � ������� ������������ ����� �������
	CRect GetFrozenRect(CRect& rectPaint);

// ������� ���������
	// ������������� ������ �������, ������� ����� �������������
	void SetScrollSizes(const SIZE &sizeTotal);
	// ���������� ��������� �� ������ ���������, � ����� �������� ������ ������� ������� � ������ ����� ���������
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	// ���������� ��������� ����� ���������
	void GetScrollBarState(CSize sizeClient, CSize& needSb, CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);
	// ���������� ������ ����� ���������
	void GetScrollBarSizes(CSize& sizeSb);
	// ��������� ������ ���������
	void UpdateBars();
	// ��������� � ������� ptPos
	void ScrollToPos(POINT ptPos);

// ������� ���������� ����
	// ��������� ������� ����
	void OnUpdateRect(CDC* pDC, CRect& rectClip);
	// ��������� ������� �����
	void OnUpdateHeaderRect(CDC *pDC, CRect &rectClip);
	// ��������� ������� ������������ ����� ��������
	void OnUpdateFrozenRect(CDC *pDC, CRect &rectClip);
	// ��������� ������� �����
	void OnUpdateGridRect(CDC *pDC, CRect &rectClip);
// ������ ������� ����������
	void DrawArrow(CDC *pDC, CPoint pos, CSize sz);
};
/////////////////////////////////////////////////////////////////////////////

#include "ALXGridCore.inl"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_GRIDCORE_H_INCLUDED_)
