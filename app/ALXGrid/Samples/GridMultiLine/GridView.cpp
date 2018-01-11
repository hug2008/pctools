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
	SetRowLineCount(3);

	VERIFY(m_ImageList.Create(IDB_BITMAP, 28, 1, (COLORREF)0x808000));

	DefineColCtrl(AddCol(30,"Img", ACFF_LEFT, AHFF_CENTER),GA_CELLCTRL,0,IDC_COL1);
	DefineImage(0,28,28,DIT_IMGLIST);
	DefineColCtrl(AddCol(200,"Text", ACFF_LEFT | DT_WORDBREAK, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOVSCROLL | ES_MULTILINE,IDC_COL2);
	
	CSize sizeExArea(50,50);
	SetExtAreaSize(sizeExArea, FALSE);

	m_arstrRowsData.SetSize(100);
	m_arstrRowsData[0] = "It's sample multiline text. For edit double click left mouse button.";
	SetGridRowCount(m_arstrRowsData.GetSize());
}

CGridView::~CGridView()
{
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= AGS_CUSTOMHEAD_HEIGHT | AGS_CUSTOMFROZEN_WIDTH;
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
		case IDC_COL2:
			{
				CellData.m_strText = m_arstrRowsData[nRow];
				break;
			}
	}
	return CellData;
}

/* Получаем данные от элемента управления и сохраняем их  */
BOOL CGridView::OnSaveCellData(int nCol, int nRow)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	// если есть что сохранять
	if(pCellCtrl != NULL)
	{
		// получаем данные из эл. управления ячейкой
		CELL_DATA Data = pCellCtrl->GetCellData();
		switch(GetCellCtrlID(nCol,nRow))
		{
		case IDC_COL2:
			m_arstrRowsData[nRow] = Data.m_strText;
			break;
		}
 
	}
	return TRUE;
}

void CGridView::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{
	pimldp->himl = m_ImageList.m_hImageList;
//	if(bSelected)
//		pimldp->fStyle = ILD_SELECTED;
	pimldp->i = 0;
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

void CGridView::OnInitialUpdate() 
{
	CXGridView::OnInitialUpdate();
	
	SetHeadRowHeight(GetGridRowHeight()/2);

	SetActiveCell(1,0);
	
}
