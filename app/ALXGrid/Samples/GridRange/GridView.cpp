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

	CString strHeadText;
	for(int i = 0; i < 1000; i++)
	{
		strHeadText.Format("%u",i);
		DefineColCtrl(AddCol(100,strHeadText, ACFF_CENTER, AHFF_CENTER),GA_CELLCTRL);
	}
//	m_BMP.LoadBitmap(IDB_BITMAP);
	SetGridRowCount(1000000);
}

CGridView::~CGridView()
{
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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

	CellData.m_strText.Format("%u - %u",nCol,nRow);

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
