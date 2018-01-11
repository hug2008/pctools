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
	ON_COMMAND(ID_GVIEW_GRIDFONT, OnSetGridFont)
	ON_COMMAND(ID_GVIEW_HEADFONT, OnSetHeadFont)
	ON_UPDATE_COMMAND_UI(ID_GSTYLE_ROWSELECT, OnUpdateGStyleRowselect)
	ON_COMMAND(ID_GSTYLE_ROWSELECT, OnGStyleRowselect)
	ON_UPDATE_COMMAND_UI(ID_GSTYLE_FLAT, OnUpdateGStyleFlat)
	ON_COMMAND(ID_GSTYLE_FLAT, OnGStyleFlat)
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
	DefineColCtrl(AddCol(90,"LeftText", ACFF_LEFT, AHFF_LEFT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	// Icon
	DefineColCtrl(AddCol(180,"ImageList && CenterText", ACFF_CENTER, AHFF_CENTER),GA_EDITCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL);
	DefineImage(1,15,15,DIT_IMGLIST);
	// ImageList
	DefineColCtrl(AddCol(180,"ImageList && RightText", ACFF_RIGHT, AHFF_RIGHT),GA_EDITCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL);
	DefineImage(2,15,15,DIT_IMGLIST);

	// CheckBox
	DefineColCtrl(AddCol(90,"CheckBox3State", ACFF_CENTER, AHFF_CENTER, DFC_BUTTON, DFCS_BUTTON3STATE | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTO3STATE | BS_FLAT);
	DefineColCtrl(AddCol(70,"CheckBox", ACFF_CENTER, AHFF_CENTER,DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT),GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT);

	VERIFY(m_ImageList.Create(IDB_BITMAP_IMGLIST, 15, 2, (COLORREF)0x808000));
	SetHeadRowCount(2,FALSE);
	SetGridRowCount(1000);
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
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;

	// don't prompt the user if we're doing print preview, printing directly,
	// or printing via IPrint and have been instructed not to ask

	CGridApp* pApp = (CGridApp*) AfxGetApp();
	if (!(pInfo->m_dwFlags & PRINTFLAG_PROMPTUSER))
	{
		if (pInfo->m_pPD->m_pd.hDC == NULL)
		{
			// if no printer set then, get default printer DC and create DC without calling
			//   print dialog.
			pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd);
		}
	}

	PageWight = PRN_XMARGIN * 2; // отступ справа 1 и слева 1
	// Определяем ширину сетки
	for(int i = 0; i < GetHeadColCount(); i++)
		PageWight = PageWight + GetHeadColWidth(i);
	if(pApp->GetPageOrientation() & DMORIENT_LANDSCAPE)
		PageHight = PageWight*210/297;
	else
		PageHight = PageWight*297/210;

	nRowInPage = (PageHight - PRN_YMARGIN*2 - GetHeadRowCount()*GetHeadRowHeight())/GetGridRowHeight();
	
	pInfo->SetMaxPage(GetGridRowCount()/nRowInPage + (GetGridRowCount()%nRowInPage > 0 ? 1 : 0));

	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGridView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
    if(!pCurrentDC) 
		return;

    // Get the page sizes (physical and logical)
    m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	
	PageHight = PageWight*m_PaperSize.cy/m_PaperSize.cx;

	nRowInPage = (PageHight - PRN_YMARGIN*2 - GetHeadRowCount()*GetHeadRowHeight())/GetGridRowHeight();

	pInfo->SetMaxPage(GetGridRowCount()/nRowInPage + (GetGridRowCount()%nRowInPage > 0 ? 1 : 0));

	ReleaseDC(pCurrentDC);
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
		case 3:
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
		case 4:
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

void CGridView::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{
	pimldp->himl = m_ImageList.m_hImageList;
	if(bSelected)
		pimldp->fStyle = ILD_SELECTED;
	pimldp->i = nCol-1;
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

void CGridView::OnSetGridFont()
{
	static LOGFONT	 logfont;
	memset(&logfont, 0, sizeof(logfont));
	CFont* fontGrid = GetGridFont();
	fontGrid->GetLogFont(&logfont);
	CFontDialog dlg(&logfont, CF_SCREENFONTS); 
	
	if(dlg.DoModal() == IDOK) 
	{
		CFont newFont;
		if(newFont.CreateFontIndirect(&logfont)) 
		{
			SetGridFont(&newFont);
			Invalidate();
		}
	}
}

void CGridView::OnSetHeadFont() 
{
	LOGFONT	 logfont;
	memset(&logfont, 0, sizeof(logfont));
	CFont* fontHead = GetHeadFont();
	fontHead->GetLogFont(&logfont);
	CFontDialog dlg(&logfont, CF_SCREENFONTS); 
	
	if (dlg.DoModal() == IDOK) 
	{
		m_HeadFont.DeleteObject();
		if(m_HeadFont.CreateFontIndirect(&logfont)) 
		{
			SetHeadFont(&m_HeadFont);
			Invalidate();
		}
	}
}


void CGridView::OnUpdateGStyleRowselect(CCmdUI* pCmdUI) 
{
	if(GetStyle() & AGS_ROWSELECT)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CGridView::OnGStyleRowselect() 
{
	if(GetStyle() & AGS_ROWSELECT)
		ModifyStyle(AGS_ROWSELECT,0);
	else
		ModifyStyle(0,AGS_ROWSELECT);
	Invalidate();
}

void CGridView::OnUpdateGStyleFlat(CCmdUI* pCmdUI) 
{
	if(GetStyle() & AGS_FLAT)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}

void CGridView::OnGStyleFlat() 
{
	if(GetStyle() & AGS_FLAT)
		ModifyStyle(AGS_FLAT,0);
	else
		ModifyStyle(0,AGS_FLAT);
	Invalidate();
}

void CGridView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(PageWight,PageHight);
	pDC->SetViewportExt(m_PaperSize);
	pDC->SetWindowOrg(0, 0);

	PrintHead(pDC, pInfo);
	PrintGridPage(pDC, pInfo);

	CALXGridView::OnPrint(pDC, pInfo);
}

void CGridView::PrintHead(CDC* pDC, CPrintInfo* pInfo)
{
	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(GetHeadFont());
	int OldBkMode =	pDC->SetBkMode(TRANSPARENT);


	// Выводится каждая строка в области перерисовки.
	for (int nRow = 0, y = PRN_YMARGIN; nRow < GetHeadRowCount(); y += GetHeadRowHeight(), nRow++)
		// Выводится каждый столбец в области перерисовки
		for (int nCol = 0, x = PRN_XMARGIN; nCol < GetHeadColCount(nRow); x = x + GetHeadColWidth(nCol,nRow), nCol++)
			PrintHeadCell(pDC,nCol,nRow,x,y);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OldBkMode);
}

void CGridView::PrintGridPage(CDC* pDC, CPrintInfo* pInfo)
{
	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(GetGridFont());
	int OldBkMode =	pDC->SetBkMode(TRANSPARENT);
	UINT OldAlign = pDC->GetTextAlign(); 
	pDC->SetTextAlign(TA_LEFT | TA_TOP | TA_NOUPDATECP);

	int y = GetHeadRowCount()*GetHeadRowHeight()+PRN_YMARGIN;

	int nFirstRow = (pInfo->m_nCurPage-1)*nRowInPage;
	int nLastRow = min(GetGridRowCount(),(int)(pInfo->m_nCurPage*nRowInPage));

	// Выводится каждая строка в области перерисовки
	for (int nRow = nFirstRow; nRow < nLastRow; y += GetGridRowHeight(), nRow++)
		// Выводится каждая ячейка в области перерисовки
		for (int nCol = 0, x = PRN_XMARGIN; nCol < GetHeadColCount(); x = x + GetHeadColWidth(nCol), nCol++)
			PrintGridCell(pDC,nCol,nRow,x,y);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OldBkMode);
	pDC->SetTextAlign(OldAlign);
}

void CGridView::PrintHeadCell(CDC* pDC, int nCol, int nRow, int x, int y)
{
	CRect rectCell(x,y,x+GetHeadColWidth(nCol,nRow)+1,y+GetHeadRowHeight()+1);
	COLORREF clrCellFrame = RGB(0,0,0);

	// выводим рамку
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,1,1);

	CBrush brushBackground;
	// закрашиваем ячейку
	if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
		pDC->FillRect(&rectCell, &brushBackground);
	rectCell.DeflateRect(2,2,1,0);
	// получаем текст и стиль 
	HEADCELL_INFO CellInfo = GetHeadCellInfo(nCol,nRow);
	// выводим текст
	pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
}

void CGridView::PrintGridCell(CDC* pDC, int nCol, int nRow, int x, int y)
{
	CELL_INFO CellInfo;
	CELL_DATA CellData;
	CRect rectCell(x,y,x+GetHeadColWidth(nCol)+1,y+GetHeadRowHeight()+1);
	COLORREF clrCellFrame = RGB(0,0,0);

	// выводим рамку
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,1,1);

	CBrush brushBackground;
	// закрашиваем ячейку
	if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)))
		pDC->FillRect(&rectCell, &brushBackground);
	rectCell.DeflateRect(2,2,1,0);

	// Получаем информацию о типе ячейки
	CellInfo = GetCellInfo(nCol,nRow);
	// Получаем информацию о содержании ячейки
	CellData = GetCellData(nCol,nRow);

	// Если ячейка с изображением
	if(IsImage(nCol,nRow))
	{
		CPoint pt;
		CSize sz;

		// Получаем размер изображения
		GetSizeImage(nCol,nRow,sz);
		// Уменьшаем размер изображения, если требуется
		sz.cx = min(rectCell.Width(),sz.cx);
		sz.cy = min(rectCell.Height(),sz.cy);

		pt.x = rectCell.left+1;
		// Смещаем изображение в центр ячейки по оси у
		pt.y = rectCell.top + (rectCell.Height() - sz.cy)/2;

		DrawImage(pDC,nCol,nRow,pt,sz,FALSE);

		// Смещаем левую границу вправо
		rectCell.left = min(rectCell.right,rectCell.left+sz.cx+2);
	}

	rectCell.DeflateRect(1,1,1,1); // Уменьшаем область рисования

	// Если надо рисовать элемент управления
	if(CellInfo.m_nTypeCtrl > 0)
		DrawCellCtrl(pDC,rectCell,CellInfo,CellData);
	else
//		pDC->DrawText((LPCTSTR)CellData.m_strText, rectCell, CellInfo.m_nFormat);
		DrawText(pDC,CellData.m_strText, rectCell, CellInfo.m_nFormat);
}

void CGridView::DrawText(CDC *pDC, CString &str, CRect& rect, UINT nFormat)
{
	if(!(nFormat & DT_RIGHT) || pDC->GetTextExtent(str).cx > rect.Width())
	{
		pDC->DrawText(str,rect,nFormat);
		return;
	}

	UINT OldAlign = pDC->GetTextAlign();

	pDC->SetTextAlign(TA_RIGHT | TA_TOP);

	pDC->TextOut(rect.right,rect.top,str);

	pDC->SetTextAlign(OldAlign);
}

int CGridView::GetHeadColWidth(int nCol, int nHeadRow)
{
	if(nHeadRow >= 0 && nHeadRow == GetHeadRowCount()-2)
	{
		switch(nCol)
		{
		case 0:
			return CALXGrid::GetHeadColWidth(0);
		case 1:
			return CALXGrid::GetHeadColWidth(1) + CALXGrid::GetHeadColWidth(2);
		case 2:
			return CALXGrid::GetHeadColWidth(3) + CALXGrid::GetHeadColWidth(4);
		default:
			return CALXGrid::GetHeadColWidth(nCol);
		}
	}
	else
		return CALXGrid::GetHeadColWidth(nCol);
}

int CGridView::GetHeadColCount(int nHeadRow)
{
	if(nHeadRow >= 0 && nHeadRow == GetHeadRowCount()-2)
		return 3;
	else
		return CALXGrid::GetHeadColCount();
}

HEADCELL_INFO CGridView::GetHeadCellInfo(int nCol, int nRow)
{
	HEADCELL_INFO HeadCellInfo;
	if(nRow >= 0 && nRow == GetHeadRowCount()-2)
	{
		switch(nCol)
		{
		case 0:
			{
				HeadCellInfo.m_strText = "Text";
				HeadCellInfo.m_nFormat = AHFF_CENTER;
				break;
			}
		case 1:
			{
				HeadCellInfo.m_strText = "Image";
				HeadCellInfo.m_nFormat = AHFF_CENTER;
				break;
			}
		case 2:
			{
				HeadCellInfo.m_strText = "CheckBox";
				HeadCellInfo.m_nFormat = AHFF_CENTER;
				break;
			}
		default:
			return CALXGrid::GetHeadCellInfo(nCol, nRow);
		}
		return HeadCellInfo;
	}
	else
		return CALXGrid::GetHeadCellInfo(nCol, nRow);
}

void CGridView::SetHeadColWidth(int nCol, int nRow, int nNewWidth)
{
	if(nRow >= 0 && nRow == GetHeadRowCount()-2)
	{
		switch(nCol)
		{
		case 0:
			{
				CALXGrid::SetHeadColWidth( 0, nRow, nNewWidth);
				break;
			}
		case 1:
			{
				CALXGrid::SetHeadColWidth( 1, nRow, nNewWidth/2);
				CALXGrid::SetHeadColWidth( 2, nRow, nNewWidth/2);
				break;
			}
		case 2:
			{
				CALXGrid::SetHeadColWidth( 3, nRow, nNewWidth/2);
				CALXGrid::SetHeadColWidth( 4, nRow, nNewWidth/2);
				break;
			}
		}
	}
	else
		CALXGrid::SetHeadColWidth( nCol, nRow, nNewWidth);
}


