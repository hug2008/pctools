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

// ��������� ���������� ���������� � �������
typedef struct ColInfo
{
	// ��������� ���������� ���������� � �����
	struct ColHeadInfo
	{
		CString	m_strHead; // ����� � ������ �����
		UINT	m_nFormat; // ������ ������
	} m_HeadInfo;
	// ��������� ���������� ����� ���������� � ������� �������
	struct ColCellsInfo
	{
		UINT	m_nFormat;			// ������ ������
		UINT	m_nTypeFrameCtrl;	// ��� �������� ��������� ����������
		UINT	m_nStateFrameCtrl;	// ��������� ��������� �������� ����������
		UINT	m_nTypeImage;		// ��� �����������
		CSize	m_sizeImage;		// ������ �����������
	} m_CellsInfo;
	// ��������� ���������� ����� ���������� �� �������� ���������� ��� ������ �������
	struct ColCtrlInfo
	{
		BYTE	m_nType;			// ��� �������� ����������
		DWORD	m_dwStyle;			// ����� 
		UINT	m_nID;				
		CALXCellCtrl* m_pColCtrl;	// ��������� �� ������� ����������
	} m_CtrlInfo;
	// ������ �������
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
// ��������� �������
	int AddCol(	int nWidth,				// ������ �������
				CString strHead,		// ��������� �����
				UINT nCellsFormat	= ACFF_LEFT,	// ������ ������� (��. CDC::DrawText(...))
				UINT nHeadFormat	= AHFF_CENTER,	// ������ ����� (��. CDC::DrawText(...))
				UINT nTypeDrawCtrl	= 0,// ��� ��������� �������� ���������� (��. CDC::DrawFrameControl(...))
				UINT nStateFrameCtrl= 0,// ��������� ��������� �������� ���������� (��. CDC::DrawFrameControl(...))
				UINT nID = NULL			// ������������� �������
				);
	int AddCol(COL_INFO srColInfo);
// ��������� �������
	int InsertCol( int nCol,				// ������ �������
				   int nWidth,				// ������ �������
				   CString strHead,			// ��������� �����
				   UINT nCellsFormat = ACFF_LEFT,	// ������ ������� (��. CDC::DrawText(...))
				   UINT nHeadFormat = AHFF_CENTER,	// ������ ����� (��. CDC::DrawText(...))
				   UINT nTypeDrawCtrl = 0,	// ��� ��������� �������� ���������� (��. CDC::DrawFrameControl(...))
				   UINT nStateFrameCtrl = 0,// ��������� ��������� �������� ���������� (��. CDC::DrawFrameControl(...))
				   UINT nID = NULL			// ������������� �������
				   );
// ������� ��� �������
	void RemoveAllCol();

// ��������� ������
	void InsertRows(int nRow, int nCount = 1);
// ������� ������
	void RemoveRows(int nRow, int nCount = 1);

// ���������� ������ ����������� ��� ������ �������
	BOOL DefineImage(int nCol, int cx, int cy, UINT nTypeImage = DIT_BITMAP);
// ���������� ������� ���������� ��� ������ �������
	BOOL DefineColCtrl(int nCol, BYTE CtrlType, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
// ��������� ������� ���������� ��� ������ �������
	virtual BOOL DestroyColCtrl(int nCol);

// ������ �����
	void OnGridDraw(CPaintDC* pDC);

// ��������� � ����������� ������ ������� ��� ������
	void ScrollAndSetActRow(int VPos, int nRow, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCol(int HPos, int nCol, BOOL RedrawCell = TRUE);
	void ScrollAndSetActCell(int nCol, int nRow, BOOL RedrawCell = TRUE);
// ��������� �� ������ ������� ��� ������
	void ScrollToRow(int VPos, int nRow);
	void ScrollToCol(int HPos, int nCol);

// Bo�������� ������� ������� 
	CPoint GetCurScrollPos();
// ��������� ������ �������, ������� ����� �������������
	void UpdateScrollSizes();
// ��������� ���������
	BOOL SaveModifyed(BOOL bSkipErrors = TRUE);
// ��������� ��������� � ������ ���������
	virtual void OnSavedCellData(int nCol, int nRow);

// �������� �������� ������, �������
	inline int GetActiveCol();
	inline int GetActiveRow();

// ������������ ������, ������, �������
	inline BOOL SetActiveCell(int nCol, int nRow);
	inline BOOL SetActiveNextCol( void );
	inline BOOL SetActivePrevCol( void );
	inline BOOL SetActiveNextRow( void );
	inline BOOL SetActivePrevRow( void );
	inline BOOL SetActiveCol(int nNewActiveCol);
	inline BOOL SetActiveRow(int nNewActiveRow);

// �������� ������
	// ������������� �������� ���������� �����
	void SetSelectRange(int nLeftCol, int nRightCol, int nTopRow, int nButtonRow);
	// �������� �������� ���������� �����
	void GetSelectRange(int& nLeftCol, int& nRightCol, int& nTopRow, int& nButtonRow);

// ��������� ������, ������
	void InvalidateCell(int nCol, int nRow);
	void InvalidateRow(int nRow);
	void InvalidateFrozenCell(int nCol, int nRow);

	// �������� ���������� ����� � �����
	virtual int GetGridRowCount();
	// ������������� ���������� ����� � �����
	void SetGridRowCount(int NewRowCount, BOOL bSendMsg = TRUE);
	// �������� ������ ������ �����
	int GetGridRowHeight();
	// ������������� ���������� �������� � ������
	void SetRowLineCount(int nNewLineCount);

	// �������� ���������� ����� � �����
	virtual int GetHeadRowCount();
	// ������������� ���������� ����� � �����
	void SetHeadRowCount(int NewRowCount, BOOL bRecalcMetrics = TRUE);

	// �������� ���������� ����������� ��������
	virtual int GetFrozenColCount();
	// ������������� ���������� ����������� ��������
	void SetFrozenColCount(int NewColCount, BOOL bRecalcMetrics = TRUE);

// ������ ����� � ������ ����������� �������
	// ������������� ������ ������ �����
	void SetHeadRowHeight(int NewHeadRowHeight, BOOL bRecalcMetrics = TRUE);
	// �������� ������ ������ �����
	int GetHeadRowHeight();
	// ������������� ������ ����������� ��������
	void SetFrozenColWidth(int NewFrozenColWidth, BOOL bRecalcMetrics = TRUE);
	// �������� ������ ����������� ��������
	int GetFrozenColWidth();

// ���������� �������� � �����
	// �������� ���������� �������� � �����
	virtual int GetHeadColCount(int nHeadRow = -1);

// ������ �������	
	// �������� ������ �������
	virtual int	 GetHeadColWidth(int nCol, int nHeadRow = -1);
	// ������������� ������ �������
	virtual void SetHeadColWidth(int nCol, int nRow, int nNewWidth);

// ����� ����� � �����
	// ������������� ����� �����
	void SetHeadFont(CFont* pFont, BOOL bRecalcMetrics = TRUE);
	// �������� ����� �����
	CFont* GetHeadFont();
	// ������������� ����� �����
	void SetGridFont(CFont* pFont, BOOL bRecalcMetrics = TRUE);
	// �������� ����� �����
	CFont* GetGridFont();

// ������ �������������� �������������� �������
	// ������������� ������ �������������� �������
	void SetExtAreaSize(CSize& sizeNewExtArea, BOOL bRecalcMetrics = TRUE);
	// �������� ������ �������������� �������
	CSize GetExtAreaSize();

// ���a�� ������ '�������� ��'
	virtual void OnSelAllButtonDown();
// ���a�� ������ �����
	virtual void OnHeadButtonDown(int nHeadCol, int nHeadRow);
// �������� ������ �����
	virtual void OnHeadButtonUp(int nHeadCol, int nHeadRow);
// ���a�� ������ ����������� �������
	virtual void OnFrozenButtonDown(int nFrozenCol, int nFrozenRow);
// �������� ������ ����������� �������
	virtual void OnFrozenButtonUp(int nFrozenCol, int nFrozenRow);
// ������� ������� ������ �����
	virtual void OnHeadButtonDblClk(int nHeadCol, int nHeadRow);
// ������� ������� ������ ����������� �������
	virtual void OnFrozenButtonDblClk(int nFrozenCol, int nFrozenRow);
// ���������� �����
	void SetSortCol(int nHeadCol, int nHeadRow, ALXSortOrder nSortOrder);
	void GetSortCol(int& nHeadCol, int& nHeadRow, ALXSortOrder& nSortOrder);

protected:

	bool m_bSetFocus; // ���������� �� �����

// ������� ��������� �������
	// ����������� �����
	BOOL OnMouseMove(UINT nFlags, CPoint point);
	// �������� �������� �����
	BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point, BOOL bSetFocus = TRUE);
	// ��������� ��������� �����
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	// ������� ������� �� ����� ������ �����
	BOOL OnLButtonDblClk(UINT nFlags, CPoint point);
	// ����� ������ ����� ������
	BOOL OnLButtonDown(UINT nFlags, CPoint point);
	// ����� ������ ����� ��������
	BOOL OnLButtonUp(UINT nFlags, CPoint point);
	// ������ �������
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// ���� ������������������
	void OnDestroy();
	// ���� �������� �����
	void OnSetFocus(CWnd* pOldWnd);

// ������� ���������
	BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll);

// ������� ���������� ���������� � �������
	COL_INFO GetColInfo(int nCol);

//*** ����������� ������� ���������� �� ������ CALXGridCore ***//

// ������ � ��������� ���������� ������
	// ������� ������� ���������� �������
	virtual CALXCellCtrl* CreateCellCtrl(int nCol, int nRow, DWORD dwStyle = WS_CHILD, UINT nID = NULL);
	// ������������� ������� ���������� ��� ������ ������
	virtual void SetCellCtrl(int nCol, int nRow, CALXCellCtrl* pCellCtrl);
	// �������� ������� ���������� �������
	virtual CALXCellCtrl* GetCellCtrl(int nCol, int nRow);
	// �������� ID �������� ���������� �������
	virtual UINT GetCellCtrlID(int nCol, int nRow);
	// �������� ����� �������� ���������� �������
	virtual DWORD GetCellCtrlStyle(int nCol, int nRow);

// ��������� ��������� ��������� � ������
	virtual BOOL OnSaveCellData(int nCol, int nRow);

// ����� ����������� � ������
	// ���������� ����� �� �������� ����������� � ������
	virtual BOOL IsImage(int nCol, int nRow);
	// ���������� ��� ����������� � ������
	virtual UINT GetTypeImage(int nCol, int nRow);
	// ���������� ������ ����������� � ������
	virtual BOOL GetSizeImage(int nCol, int nRow, CSize& sz);
	// �������� ����������� ��� ������
	virtual LPARAM GetImage(int nCol, int nRow, BOOL bSelected);
	// �������� ���������� � ����������� ��� ������
	virtual void GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected);

// �������� ���������� � ������ �����
	virtual CELL_INFO GetCellInfo(int nCol, int nRow);
	virtual CELL_DATA GetCellData(int nCol, int nRow);

// �������� ���������� � ������ �����
	virtual HEADCELL_INFO GetHeadCellInfo(int nCol, int nRow);

private:
// ��������� �����
	struct HeaderInfo
	{
		CArray < COL_INFO, COL_INFO& > m_Cols;
	} m_HeaderInfo;
// ��������� ���������� �������������� �����
	CRect m_rectSaveInv;
// ���������� ����� � �����
	int m_GridRowCount;
};
/////////////////////////////////////////////////////////////////////////////

#include "ALXGrid.inl"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_GRID_H_INCLUDED_)
