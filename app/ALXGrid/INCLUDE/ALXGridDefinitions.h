// ALXGridDefinitions.h 
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_GRIDDEFINITIONS_H_INCLUDED_)
#define ALX_GRIDDEFINITIONS_H_INCLUDED_

#include "ALXCellMetrics.h"

/*
 * ALX Cell Control Type
 */
#define GA_CELLCTRL		0x00
#define GA_EDITCTRL		0x01
#define GA_BUTTONCTRL	0x02
#define GA_COMBOBOXCTRL	0x03
#define GA_CHECKCTRL	0x04

typedef struct HeadCellInfo
{
	CString	m_strText;	// ����� ���������
	UINT	m_nFormat;	// ������
} HEADCELL_INFO;

typedef struct CellInfo
{
	UINT	m_nFormat;		// ������
	UINT	m_nTypeCtrl;	// ��� �������� ����������
	UINT	m_nState;		// ��������� �������� ����������
	UINT	m_nTypeImage;	// ��� �����������
} CELL_INFO;

typedef struct CellData
{
	CString	m_strText;	// ������ ������
	DWORD	m_dwTag;	// �������������� ��������
} CELL_DATA;

typedef enum ALXSortOrder
{
	Ascending = 0,
	Descending = 1,
	None = 2,
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_GRIDDEFINITIONS_H_INCLUDED_)
