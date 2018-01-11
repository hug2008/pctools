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
	COLORREF m_crLeftBrdColor;	// ÷вет левой рамки €чейки
	COLORREF m_crTopBrdColor;	// ÷вет верхней рамки
	COLORREF m_crRightBrdColor;	// ÷вет правой рамки €чейки
	COLORREF m_crBottomBrdColor;// ÷вет нижней рамки
	BYTE	 m_nOrderDraw;		// ѕор€док вывода линий рамки
	// цвет правой рамки €чейки должен соответствовать цвету левой рамки €чейки справа
	// цвет нижней рамки €чейки должен соответствовать цвету верхней рамки €чейки снизу
	// и наоборот

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
	COLORREF m_crBackgrnd;	// ÷вет фона €чейки
	COLORREF m_crText;		// ÷вет текста €чейки
} CELL_COLOR;


class CALXCellMetrics
{

public:
	CALXCellMetrics();
	~CALXCellMetrics();

	// —труктура определ€юща€ цвета €чейки по умолчанию
	CELL_COLOR m_DefaultCellColor;
	// —труктура определ€юща€ цвета активной €чейки
	CELL_COLOR m_ActiveCellColor;
	// —труктура определ€юща€ цвета выделенной €чейки
	CELL_COLOR m_SelectCellColor;
	//  ласс определ€ющий параметры рамки €чейки
	CALXCellBorders m_Borders;
	//  ласс определ€ющий параметры рамки "фокуса ввода"
	CALXFocusRect m_FocusRect;
	// ”казатель на шрифт активной €чейки
	CFont* pActiveFont;
	// ”казатель на шрифт по умолчанию
	CFont* pDefaultFont;

public:
	void FillDefault();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_CELLMETRICS_H_INCLUDED_)
