// GridFormView.cpp : implementation of the CGridFormView class
//

#include "stdafx.h"
#include "Grid.h"

#include "GridDoc.h"
#include "GridFormView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridFormView

IMPLEMENT_DYNCREATE(CGridFormView, CFormView)

BEGIN_MESSAGE_MAP(CGridFormView, CFormView)
	//{{AFX_MSG_MAP(CGridFormView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridFormView construction/destruction

CGridFormView::CGridFormView()
	: CFormView(CGridFormView::IDD)
{
	//{{AFX_DATA_INIT(CGridFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CGridFormView::~CGridFormView()
{
}

void CGridFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CGridFormView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CGridFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CGridFormView printing

BOOL CGridFormView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGridFormView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGridFormView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGridFormView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CGridFormView diagnostics

#ifdef _DEBUG
void CGridFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGridFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGridDoc* CGridFormView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGridDoc)));
	return (CGridDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridFormView message handlers

BOOL CGridFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rectView, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	if(!CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rectView, pParentWnd, nID, pContext))
		return FALSE;

	// Create GridCtrl
	CRect rectClient;
	GetClientRect(&rectClient);
	
	CRect rectFrame,rectParent,rect(0,0,100,100);
	GetClientRect(&rectParent);
	ClientToScreen(&rectParent);
	CWnd* pWnd = GetDlgItem(IDC_STATIC_FRAME);
	pWnd->GetWindowRect(&rectFrame);
	rect.left = rectFrame.left - rectParent.left;
	rect.top = rectFrame.top - rectParent.top;
	rect.right = rect.left + rectFrame.Width();
	rect.bottom = rect.top + rectFrame.Height();

	if (!m_wndCtrl.Create(WS_EX_CLIENTEDGE,
							WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL| WS_TABSTOP, 
	             			rect,
							this,
                			0))
	{
		TRACE0("Failed to create grid control\n");
		return FALSE;
	}

	m_wndCtrl.SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOREDRAW);

	return TRUE;
}
