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
	// Bitmap
	DefineColCtrl(AddCol(180,"Bitmap && LeftText", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	DefineImage(0,7,7,DIT_BITMAP);
	// Icon
	DefineColCtrl(AddCol(180,"ImageList && CenterText", ACFF_CENTER, AHFF_CENTER),GA_EDITCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL);
	DefineImage(1,15,15,DIT_IMGLIST);
	// ImageList
	DefineColCtrl(AddCol(180,"ImageList && RightText", ACFF_RIGHT, AHFF_RIGHT),GA_EDITCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL);
	DefineImage(2,15,15,DIT_IMGLIST);

	m_BMP.LoadBitmap(IDB_BITMAP);
	VERIFY(m_ImageList.Create(IDB_BITMAP_IMGLIST, 15, 1, (COLORREF)0x808000));
	SetGridRowCount(10000);
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
	switch(nCol)
	{
		case 4:
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
		case 5:
			{
				if(nRow%5 == 0) 
					CellData.m_dwTag = 0;
				else
					CellData.m_dwTag = 1;
				break;
			}
		case 6:
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
		case 7:
			{
				CellData.m_strText.Format("%u - %u",nCol,nRow);
				if(nRow%11 == 0) 
					CellData.m_dwTag = 0;
				else
						CellData.m_dwTag = 1;
				break;
			}
		case 8:
			{
				CellData.m_strText = "Раз";
				CellData.m_dwTag = 0;
				break;
			}
		case 9:
			{
				CellData.m_strText = "Первый";
				CellData.m_dwTag = 0;
				break;
			}
		case 10:
			{
				CellData.m_strText = "1";
				CellData.m_dwTag = 0;
				break;
			}
		case 11:
			{
				CellData.m_strText = "1-й";
				CellData.m_dwTag = 0;
				break;
			}
		default:
			CellData.m_strText.Format("%u - %u",nCol,nRow);
	}
	return CellData;
}

LPARAM CGridView::GetImage(int nCol, int nRow, BOOL bSelected)
{
	if(nCol == 0)
		return (LPARAM)(HBITMAP)m_BMP;
//	if(nCol == 1)
//		return (LPARAM)m_ImageList.ExtractIcon(bSelected ? 1 : 0);
	return NULL;
}
void CGridView::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{
	pimldp->himl = m_ImageList.m_hImageList;
	if(bSelected)
		pimldp->fStyle = ILD_SELECTED;
	pimldp->i = nCol;
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
