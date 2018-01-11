// ALXCellMetrics.h 
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_CELLMETRICS_H_INCLUDED_)
#define ALX_CELLMETRICS_H_INCLUDED_

#define COLOR_SELECTCELLFRAME	RGB(0,100,200)
/*
 * ALX Order Draw Borders
 */
#define AOD_VERT_HOR		0x00
#define AOD_HOR_VERT		0x01

class CALXCellBorders
{

public:
	COLORREF m_crLeftBrdColor;	// ���� ����� ����� ������
	COLORREF m_crTopBrdColor;	// ���� ������� �����
	COLORREF m_crRightBrdColor;	// ���� ������ ����� ������
	COLORREF m_crBottomBrdColor;// ���� ������ �����
	BYTE	 m_nOrderDraw;		// ������� ������ ����� �����
	// ���� ������ ����� ������ ������ ��������������� ����� ����� ����� ������ ������
	// ���� ������ ����� ������ ������ ��������������� ����� ������� ����� ������ �����
	// � ��������

	void DrawBorders(CDC *pDC, CRect rc);
	void DrawBorders(CDC *pDC, CRect rc,
				COLORREF crLeftBrdColor,
				COLORREF crTopBrdColor,
				COLORREF crRightBrdColor,
				COLORREF crBottomBrdColor,
				BYTE nOrderDraw);
};

class CALXFocusRect
{

public:
	CALXFocusRect();
	~CALXFocusRect();

	COLORREF	m_crBackColor;
	COLORREF	m_crFaceColor;
	BOOL		m_bSolid;

	void DrawFocusRect(CDC *pDC, CRect& rc);
};

typedef struct CellColor
{
	COLORREF m_crBackgrnd;	// ���� ���� ������
	COLORREF m_crText;		// ���� ������ ������
} CELL_COLOR;


class CALXCellMetrics
{

public:
	CALXCellMetrics();
	~CALXCellMetrics();

	// ��������� ������������ ����� ������ �� ���������
	CELL_COLOR m_DefaultCellColor;
	// ��������� ������������ ����� �������� ������
	CELL_COLOR m_ActiveCellColor;
	// ��������� ������������ ����� ���������� ������
	CELL_COLOR m_SelectCellColor;
	// ����� ������������ ��������� ����� ������
	CALXCellBorders m_Borders;
	// ����� ������������ ��������� ����� "������ �����"
	CALXFocusRect m_FocusRect;
	// ��������� �� ����� �������� ������
	CFont* pActiveFont;
	// ��������� �� ����� �� ���������
	CFont* pDefaultFont;

public:
	void FillDefault();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_CELLMETRICS_H_INCLUDED_)
