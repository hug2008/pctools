// GridView.cpp : implementation of the CGridView class
//

#include "stdafx.h"
#include "Grid.h"

#include "GridDoc.h"
#include "GridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridView

IMPLEMENT_DYNCREATE(CGridView, CXGridView)

BEGIN_MESSAGE_MAP(CGridView, CXGridView)
	//{{AFX_MSG_MAP(CGridView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CXGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CXGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CXGridView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView construction/destruction

CGridView::CGridView()
{
	// Bitmap
	DefineColCtrl(AddCol(80,"Bitmap && Text", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,IDC_COL0);
	// Text
	DefineColCtrl(AddCol(80,"LeftText", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,IDC_COL1);
	DefineColCtrl(AddCol(80,"CenterText", ACFF_CENTER, AHFF_CENTER),GA_EDITCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,IDC_COL2);
	DefineColCtrl(AddCol(80,"RightText", ACFF_RIGHT, AHFF_RIGHT),GA_EDITCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL,IDC_COL3);
	// CheckBox
	DefineColCtrl(AddCol(100,"CheckBox3State", ACFF_CENTER, AHFF_CENTER, DFC_BUTTON, DFCS_BUTTON3STATE | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTO3STATE | BS_FLAT,IDC_COL4);
	DefineColCtrl(AddCol(60,"CheckBox", ACFF_CENTER, AHFF_CENTER,DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT,IDC_COL5);

	DefineColCtrl(AddCol(130,"CheckBox3State && Text", ACFF_LEFT, AHFF_LEFT, DFC_BUTTON,DFCS_BUTTON3STATE | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTO3STATE | BS_FLAT,IDC_COL6);
	DefineColCtrl(AddCol(90,"CheckBox && Text", ACFF_LEFT, AHFF_LEFT,DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT,IDC_COL7);

	SetFrozenColCount(2, FALSE);

	CSize sizeExArea(50,50);
	SetExtAreaSize(sizeExArea, FALSE);
	SetGridRowCount(1000);
}

CGridView::~CGridView()
{
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= AGS_CUSTOMFROZEN_WIDTH;
	return CXGridView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGridView drawing

void CGridView::OnDraw(CDC* pDC)
{
	CGridDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGridView printing

BOOL CGridView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGridView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGridView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

CELL_DATA CGridView::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	switch(GetCellCtrlID(nCol,nRow))
	{
		case IDC_COL4:
			{
				switch(nRow%3)
				{
				case 0:
					{
					CellData.m_dwTag = 2;
					break;
					}
				case 1:
					{
					CellData.m_dwTag = 1;
					break;
					}
				default:
					CellData.m_dwTag = 0;
				}
				break;
			}
		case IDC_COL5:
			{
				if(nRow%5 == 0) 
					CellData.m_dwTag = 0;
				else
					CellData.m_dwTag = 1;
				break;
			}
		case IDC_COL6:
			{
				CellData.m_strText.Format("%u - %u",nCol,nRow);
				if(nRow%7 == 0) 
					CellData.m_dwTag = 0;
				else
					if(nRow%4 == 0) 
						CellData.m_dwTag = 2;
					else
						CellData.m_dwTag = 1;
				break;
			}
		case IDC_COL7:
			{
				CellData.m_strText.Format("%u - %u",nCol,nRow);
				if(nRow%11 == 0) 
					CellData.m_dwTag = 0;
				else
						CellData.m_dwTag = 1;
				break;
			}
		default:
			CellData.m_strText.Format("%u - %u",nCol,nRow);
	}
	return CellData;
}

/////////////////////////////////////////////////////////////////////////////
// CGridView diagnostics

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CXGridView::AssertValid();
}

void CGridView::Dump(CDumpContext& dc) const
{
	CXGridView::Dump(dc);
}

CGridDoc* CGridView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGridDoc)));
	return (CGridDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridView message handlers
