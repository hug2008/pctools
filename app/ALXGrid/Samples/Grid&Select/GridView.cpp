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

IMPLEMENT_DYNCREATE(CGridView, CALXGridView)

BEGIN_MESSAGE_MAP(CGridView, CALXGridView)
	//{{AFX_MSG_MAP(CGridView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CALXGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CALXGridView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CALXGridView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView construction/destruction

CGridView::CGridView()
{
	// Text
	DefineColCtrl(AddCol(80,"LeftText", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL,IDC_COL1);
	DefineColCtrl(AddCol(80,"CenterText", ACFF_CENTER, AHFF_CENTER),GA_EDITCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL,IDC_COL2);
	DefineColCtrl(AddCol(80,"RightText", ACFF_RIGHT, AHFF_RIGHT),GA_EDITCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL,IDC_COL3);
	DefineColCtrl(AddCol(80,"ChekBox", ACFF_CENTER, AHFF_CENTER, DFC_BUTTON, DFCS_BUTTON3STATE | DFCS_FLAT, IDC_COL4), GA_BUTTONCTRL, WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT,IDC_COL4);

	CSize sizeExArea(50,50);
	SetExtAreaSize(sizeExArea, FALSE);
	SetGridRowCount(100);

}

CGridView::~CGridView()
{
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= AGS_CUSTOMFROZEN_WIDTH | AGS_RANGESELECT;
	return CALXGridView::PreCreateWindow(cs);
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
		case IDC_COL1:
			CellData.m_strText.Format("%u",20+nRow);
			break;
		case IDC_COL2:
			CellData.m_strText.Format("%u",nCol+nRow);
			break;
		case IDC_COL3:
			CellData.m_strText.Format("%u",nCol*nRow+5);
			break;
	}
	return CellData;
}

/////////////////////////////////////////////////////////////////////////////
// CGridView diagnostics

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CALXGridView::AssertValid();
}

void CGridView::Dump(CDumpContext& dc) const
{
	CALXGridView::Dump(dc);
}

CGridDoc* CGridView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGridDoc)));
	return (CGridDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridView message handlers

void CGridView::OnInitialUpdate() 
{
	CALXGridView::OnInitialUpdate();
}


