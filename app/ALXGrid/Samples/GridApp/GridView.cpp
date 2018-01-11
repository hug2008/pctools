// GridView.cpp : implementation of the CGridView class
//

#include "stdafx.h"
#include "Grid.h"

#include "GridDoc.h"
#include "GridView.h"
#include "SetCountDlg.h"
#include "SetHeightDlg.h"
#include "SetWidthDlg.h"

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
	ON_COMMAND(ID_GVIEW_HEADROW, OnSetHeadRowsCount)
	ON_COMMAND(ID_GVIEW_HEADHEIGHT, OnSetHeadHeight)
	ON_COMMAND(ID_GVIEW_FROZENWIDTH, OnSetFrozenColWidth)
	ON_COMMAND(ID_GVIEW_FROZENCOL, OnSetFrozenColsCount)
	ON_UPDATE_COMMAND_UI(ID_GSTYLE_ROWSELECT, OnUpdateGStyleRowselect)
	ON_COMMAND(ID_GSTYLE_ROWSELECT, OnGStyleRowselect)
	ON_UPDATE_COMMAND_UI(ID_GSTYLE_FLAT, OnUpdateGStyleFlat)
	ON_COMMAND(ID_GSTYLE_FLAT, OnGStyleFlat)
	ON_UPDATE_COMMAND_UI(ID_EDITROW_DEL, OnUpdateDelRow)
	ON_UPDATE_COMMAND_UI(ID_EDITROW_INS, OnUpdateInsRow)
	ON_COMMAND(ID_EDITROW_DEL, OnDelRow)
	ON_COMMAND(ID_EDITROW_INS, OnInsRow)
	ON_COMMAND(ID_EDITROW_ADD, OnAddRow)
	ON_COMMAND(ID_GSTYLE_NOGRIDLINES, OnGStyleNoGridLines)
	ON_UPDATE_COMMAND_UI(ID_GSTYLE_NOGRIDLINES, OnUpdateGStyleNoGridLines)
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
	CString strHeadTitle;
	// Text
	strHeadTitle.LoadString(ID_COL_LEFTTEXT);
	DefineColCtrl(AddCol(80, strHeadTitle, ACFF_LEFT, AHFF_LEFT, 0, 0, ID_COL_LEFTTEXT),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL, ID_COL_LEFTTEXT);

	strHeadTitle.LoadString(ID_COL_CENTERTEXT);
	DefineColCtrl(AddCol(80, strHeadTitle, ACFF_CENTER, AHFF_CENTER, 0, 0, ID_COL_CENTERTEXT),GA_EDITCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL, ID_COL_CENTERTEXT);

	strHeadTitle.LoadString(ID_COL_RIGHTTEXT);
	DefineColCtrl(AddCol(80, strHeadTitle, ACFF_RIGHT, AHFF_RIGHT, 0, 0, ID_COL_RIGHTTEXT),GA_EDITCTRL,WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL, ID_COL_RIGHTTEXT);

	// Bitmap
	strHeadTitle.LoadString(ID_COL_BITMAPTEXT);
	int nCol = AddCol(80, strHeadTitle, ACFF_LEFT, AHFF_LEFT, 0, 0, ID_COL_BITMAPTEXT);
	DefineColCtrl(nCol,GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL, ID_COL_BITMAPTEXT);
	DefineImage(nCol,7,7,DIT_IMGLIST);

	// CheckBox
	strHeadTitle.LoadString(ID_COL_CHECKBOX3STATE);
	DefineColCtrl(AddCol(90, strHeadTitle, ACFF_CENTER, AHFF_CENTER, DFC_BUTTON, DFCS_BUTTON3STATE | DFCS_FLAT, ID_COL_CHECKBOX3STATE),GA_BUTTONCTRL,WS_CHILD | BS_AUTO3STATE | BS_FLAT, ID_COL_CHECKBOX3STATE);

	strHeadTitle.LoadString(ID_COL_CHECKBOX);
	DefineColCtrl(AddCol(60, strHeadTitle, ACFF_CENTER, AHFF_CENTER, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT, ID_COL_CHECKBOX),GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT, ID_COL_CHECKBOX);

	strHeadTitle.LoadString(ID_COL_CHECKBOX3STATETEXT);
	DefineColCtrl(AddCol(130, strHeadTitle, ACFF_LEFT, AHFF_LEFT, DFC_BUTTON, DFCS_BUTTON3STATE | DFCS_FLAT, ID_COL_CHECKBOX3STATETEXT),GA_BUTTONCTRL,WS_CHILD | BS_AUTO3STATE | BS_FLAT, ID_COL_CHECKBOX3STATETEXT);

	strHeadTitle.LoadString(ID_COL_CHECKBOXTEXT);
	DefineColCtrl(AddCol(90,"CheckBox && Text", ACFF_LEFT, AHFF_LEFT,DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT, ID_COL_CHECKBOXTEXT),GA_BUTTONCTRL,WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT, ID_COL_CHECKBOXTEXT);

	// ComboBox
	strHeadTitle.LoadString(ID_COL_HDROPDOWNCOMBOBOX);
	DefineColCtrl(AddCol(150, strHeadTitle, ACFF_LEFT, AHFF_LEFT, 0, 0, ID_COL_HDROPDOWNCOMBOBOX),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWN | WS_VSCROLL, ID_COL_HDROPDOWNCOMBOBOX);

	strHeadTitle.LoadString(ID_COL_DROPDOWNCOMBOBOX);
	DefineColCtrl(AddCol(150, strHeadTitle, ACFF_LEFT, AHFF_LEFT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX | DFCS_FLAT, ID_COL_DROPDOWNCOMBOBOX),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWN | WS_VSCROLL, ID_COL_DROPDOWNCOMBOBOX);

	strHeadTitle.LoadString(ID_COL_HLISTCOMBOBOX);
	DefineColCtrl(AddCol(130, strHeadTitle, ACFF_LEFT, AHFF_LEFT, 0, 0, ID_COL_HLISTCOMBOBOX),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, ID_COL_HLISTCOMBOBOX);

	strHeadTitle.LoadString(ID_COL_LISTCOMBOBOX);
	DefineColCtrl(AddCol(130, strHeadTitle, ACFF_LEFT, ACFF_LEFT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX, ID_COL_LISTCOMBOBOX),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, ID_COL_LISTCOMBOBOX);

	VERIFY(m_ImageList.Create(IDB_BITMAP_IMGLIST, 7, 1, (COLORREF)0x808000));

	CSize sizeExArea(50,50);
	SetExtAreaSize(sizeExArea, FALSE);
	SetFocusRect(::GetSysColor(COLOR_WINDOWTEXT),::GetSysColor(COLOR_WINDOW),FALSE);
	SetHeadRowCount(2,FALSE);
//	SetGridRowCount(0);
}

CGridView::~CGridView()
{
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
//	cs.style = cs.style | AGS_CUSTOMFROZEN_WIDTH;
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
	CGridDoc* pDoc = GetDocument();
	
	switch(GetCellCtrlID(nCol,nRow))
	{
		case ID_COL_LEFTTEXT: 
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_LeftText;
				break;
			}
		case ID_COL_CENTERTEXT: 
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_CenterText;
				break;
			}
		case ID_COL_RIGHTTEXT: 
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_RightText;
				break;
			}
		case ID_COL_BITMAPTEXT: 
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_BitmapText;
				break;
			}
		case ID_COL_CHECKBOX3STATE:
			{
				CellData.m_dwTag = pDoc->m_Rows[nRow].m_CheckBox3State;
				break;
			}
		case ID_COL_CHECKBOX:
			{
				CellData.m_dwTag = pDoc->m_Rows[nRow].m_CheckBox;
				break;
			}
		case ID_COL_CHECKBOX3STATETEXT:
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_CheckBox3StateText.m_Text;
				CellData.m_dwTag = pDoc->m_Rows[nRow].m_CheckBox3StateText.m_State;
				break;
			}
		case ID_COL_CHECKBOXTEXT:
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_CheckBoxText.m_Text;
				CellData.m_dwTag = pDoc->m_Rows[nRow].m_CheckBoxText.m_State;
				break;
			}
		case ID_COL_HDROPDOWNCOMBOBOX:
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_HideDropDownComboBox;
				break;
			}
		case ID_COL_DROPDOWNCOMBOBOX:
			{
				CellData.m_strText = pDoc->m_Rows[nRow].m_DropDownComboBox;
				break;
			}
		case ID_COL_HLISTCOMBOBOX:
			{
				CellData.m_dwTag = pDoc->m_Rows[nRow].m_HideListComboBox;
				CellData.m_strText.Format("%u",pDoc->m_Rows[nRow].m_HideListComboBox);
				break;
			}
		case ID_COL_LISTCOMBOBOX:
			{
				CellData.m_dwTag = pDoc->m_Rows[nRow].m_ListComboBox;
				CellData.m_strText.Format("N-%u",pDoc->m_Rows[nRow].m_ListComboBox);
				break;
			}
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

void CGridView::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{
	pimldp->himl = m_ImageList.m_hImageList;
	if(bSelected)
		pimldp->fStyle = ILD_SELECTED;
	pimldp->i = 0;
}

CALXCellCtrl* CGridView::CreateCellCtrl(int nCol, int nRow, DWORD dwStyle, UINT nID)
{
	CALXCellCtrl* pCellCtrl = CALXGrid::CreateCellCtrl(nCol, nRow, dwStyle);
	if(pCellCtrl != NULL)
	{
		switch(GetCellCtrlID(nCol,nRow))
		{
		case ID_COL_HDROPDOWNCOMBOBOX:
			{
				CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) pCellCtrl;
				pComboBoxCtrl->AddString("One");
				pComboBoxCtrl->AddString("Two");
				pComboBoxCtrl->AddString("Three");
				break;
			}
		case ID_COL_DROPDOWNCOMBOBOX:
			{
				CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) pCellCtrl;
				pComboBoxCtrl->AddString("First");
				pComboBoxCtrl->AddString("Second");
				pComboBoxCtrl->AddString("Third");
				break;
			}
		case ID_COL_HLISTCOMBOBOX:
			{
				CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) pCellCtrl;
				pComboBoxCtrl->AddString("1");
				pComboBoxCtrl->SetItemData(0,1);
				pComboBoxCtrl->AddString("2");
				pComboBoxCtrl->SetItemData(1,2);
				pComboBoxCtrl->AddString("3");
				pComboBoxCtrl->SetItemData(2,3);
				break;
			}
		case ID_COL_LISTCOMBOBOX:
			{
				CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) pCellCtrl;
				pComboBoxCtrl->AddString("N-1");
				pComboBoxCtrl->SetItemData(0,1);
				pComboBoxCtrl->AddString("N-2");
				pComboBoxCtrl->SetItemData(1,2);
				pComboBoxCtrl->AddString("N-3");
				pComboBoxCtrl->SetItemData(2,3);
				break;
			}
		}
	}
	return pCellCtrl;
}

void CGridView::OnInitialUpdate() 
{
	CALXGridView::OnInitialUpdate();
	
	SetGridRowCount(GetDocument()->m_Rows.GetSize());
	UpdateScrollSizes();
	CGridView::SetFocus();

}

BOOL CGridView::OnSaveCellData(int nCol, int nRow)
{
	CGridDoc* pDoc = GetDocument();
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA Data = pCellCtrl->GetCellData();
		switch(GetCellCtrlID(nCol,nRow))
		{
			case ID_COL_LEFTTEXT: 
				{
					pDoc->m_Rows[nRow].m_LeftText = Data.m_strText;
					break;
				}
			case ID_COL_CENTERTEXT: 
				{
					pDoc->m_Rows[nRow].m_CenterText = Data.m_strText;
					break;
				}
			case ID_COL_RIGHTTEXT: 
				{
					pDoc->m_Rows[nRow].m_RightText = Data.m_strText;
					break;
				}
			case ID_COL_BITMAPTEXT: 
				{
					pDoc->m_Rows[nRow].m_BitmapText = Data.m_strText;
					break;
				}
			case ID_COL_CHECKBOX3STATE:
				{
					pDoc->m_Rows[nRow].m_CheckBox3State = (STATE)Data.m_dwTag;
					break;
				}
			case ID_COL_CHECKBOX:
				{
					pDoc->m_Rows[nRow].m_CheckBox = (STATE)Data.m_dwTag;
					break;
				}
			case ID_COL_CHECKBOX3STATETEXT:
				{
					pDoc->m_Rows[nRow].m_CheckBox3StateText.m_State = (STATE)Data.m_dwTag;
					break;
				}
			case ID_COL_CHECKBOXTEXT:
				{
					pDoc->m_Rows[nRow].m_CheckBoxText.m_State = (STATE)Data.m_dwTag;
					break;
				}
			case ID_COL_HDROPDOWNCOMBOBOX:
				{
					if(Data.m_dwTag == ULONG_MAX)
					{
						CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) GetCellCtrl(nCol, nRow);
						pComboBoxCtrl->AddString(Data.m_strText);
					}
					pDoc->m_Rows[nRow].m_HideDropDownComboBox = Data.m_strText;
					break;
				}
			case ID_COL_DROPDOWNCOMBOBOX:
				{
					if(Data.m_dwTag == ULONG_MAX)
					{
						CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) GetCellCtrl(nCol, nRow);
						pComboBoxCtrl->AddString(Data.m_strText);
					}
					pDoc->m_Rows[nRow].m_DropDownComboBox = Data.m_strText;
					break;
				}
			case ID_COL_HLISTCOMBOBOX:
				{
					pDoc->m_Rows[nRow].m_HideListComboBox = (ORDER)Data.m_dwTag;
					break;
				}
			case ID_COL_LISTCOMBOBOX:
				{
					pDoc->m_Rows[nRow].m_ListComboBox = (ORDER)Data.m_dwTag;
					break;
				}
		}
	}
	return TRUE;
}

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

void CGridView::OnSetHeadRowsCount() 
{
	CSetCountDlg dlgSetCount;
	dlgSetCount.m_bCount = GetHeadRowCount();
	if (dlgSetCount.DoModal() == IDOK) 
	{
		SetHeadRowCount(dlgSetCount.m_bCount);
		Invalidate();
	}
}


void CGridView::OnSetHeadHeight() 
{
	CSetHeightDlg dlgSetHeight;
	dlgSetHeight.m_bHeight = GetHeadRowHeight();
	if (dlgSetHeight.DoModal() == IDOK) 
	{
		CGridView::ModifyStyle(0,AGS_CUSTOMHEAD_HEIGHT);

		SetHeadRowHeight(dlgSetHeight.m_bHeight);
		Invalidate();
	}
}

void CGridView::OnSetFrozenColWidth() 
{
	CSetWidthDlg dlgSetWidth;
	dlgSetWidth.m_bWidth = GetFrozenColWidth();
	if (dlgSetWidth.DoModal() == IDOK) 
	{
		CGridView::ModifyStyle(0,AGS_CUSTOMFROZEN_WIDTH);

		SetFrozenColWidth(dlgSetWidth.m_bWidth);
		Invalidate();
	}
}

void CGridView::OnSetFrozenColsCount() 
{
	CSetCountDlg dlgSetCount;
	dlgSetCount.m_bCount = GetFrozenColCount();
	if (dlgSetCount.DoModal() == IDOK) 
	{
		SetFrozenColCount(dlgSetCount.m_bCount);
		Invalidate();
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

void CGridView::OnUpdateDelRow(CCmdUI* pCmdUI) 
{
	if(GetActiveRow() >= 0 && GetActiveRow() < GetGridRowCount())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CGridView::OnUpdateInsRow(CCmdUI* pCmdUI) 
{
	if(GetActiveRow() >= 0 && GetActiveRow() < GetGridRowCount())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CGridView::OnDelRow() 
{
	GetDocument()->m_Rows.RemoveAt(GetActiveRow());
	CGridView::RemoveRows(GetActiveRow());
}

void CGridView::OnInsRow() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL)
	{
		if(pCellCtrl->IsModifyed())
			if(pCellCtrl->OnValidate())
				OnSaveCellData(GetActiveCol(),GetActiveRow());
	}
	ROWINFO RowInfo;
	RowInfo.m_CheckBox3StateText.m_Text = " ";
	RowInfo.m_CheckBoxText.m_Text = " ";
	RowInfo.m_HideListComboBox = first;
	RowInfo.m_ListComboBox = first;
	GetDocument()->m_Rows.InsertAt(GetActiveRow(),RowInfo);
	CGridView::InsertRows(GetActiveRow());
}

void CGridView::OnAddRow() 
{
	ROWINFO RowInfo;
	RowInfo.m_CheckBox3StateText.m_Text = " ";
	RowInfo.m_CheckBoxText.m_Text = " ";
	RowInfo.m_HideListComboBox = first;
	RowInfo.m_ListComboBox = first;
	GetDocument()->m_Rows.Add(RowInfo);
	CGridView::InsertRows(GetDocument()->m_Rows.GetSize()-1);
/*	CGridView::SetGridRowCount(GetDocument()->m_Rows.GetSize());
	CGridView::UpdateScrollSizes();
	CGridView::SetActiveCell(GetActiveCol(),GetDocument()->m_Rows.GetSize()-1);
	CGridView::InvalidateRow(GetActiveRow());
*/
}

void CGridView::PrintHead(CDC* pDC, CPrintInfo* pInfo)
{
	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(GetHeadFont());
	int OldBkMode =	pDC->SetBkMode(TRANSPARENT);


	// Выводится каждая строка в области перирисовки,
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

	int y = GetHeadRowCount()*GetHeadRowHeight()+PRN_YMARGIN;

	int nFirstRow = (pInfo->m_nCurPage-1)*nRowInPage;
	int nLastRow = min(GetGridRowCount(),(int)(pInfo->m_nCurPage*nRowInPage));

	// Выводится каждая строка в области перерисовки,
	for (int nRow = nFirstRow; nRow < nLastRow; y += GetGridRowHeight(), nRow++)
		// Выводится каждая ячейка в области перерисовки
		for (int nCol = 0, x = PRN_XMARGIN; nCol < GetHeadColCount(); x = x + GetHeadColWidth(nCol), nCol++)
			PrintGridCell(pDC,nCol,nRow,x,y);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OldBkMode);
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

		DrawImage(pDC,nCol,nRow,pt,sz);

		// Смещаем левую границу вправо
		rectCell.left = min(rectCell.right,rectCell.left+sz.cx+2);
	}

	rectCell.DeflateRect(1,1,1,1); // Уменьшаем область рисования

	// Если надо рисовать элемент управления
	if(CellInfo.m_nTypeCtrl > 0)
		DrawCellCtrl(pDC,rectCell,CellInfo,CellData);
	else
		pDC->DrawText((LPCTSTR)CellData.m_strText, rectCell, CellInfo.m_nFormat);
}

void CGridView::DrawImage(CDC *pDC, int nCol, int nRow, CPoint& pt, CSize& sz)
{

	switch(GetTypeImage(nCol,nRow))
	{
	case DIT_ICON :
		{
			HICON hIcon = (HICON)GetImage(nCol,nRow,FALSE);
			if(hIcon != NULL)
				DrawIconEx(pDC->m_hDC,pt.x,pt.y,hIcon,sz.cx,sz.cy,NULL,NULL,DI_NORMAL | DI_DEFAULTSIZE);
			break;
		}
	case DIT_IMGLIST :
		{
			IMAGELISTDRAWPARAMS imldp;
			memset(&imldp, 0, sizeof(imldp));
			imldp.cbSize = sizeof(imldp);
			imldp.hdcDst = pDC->m_hDC;
			imldp.rgbBk = CLR_DEFAULT;
			imldp.rgbFg = CLR_DEFAULT;
			imldp.fStyle = ILD_TRANSPARENT;
			imldp.himl = NULL;
			imldp.cx = sz.cx;
			imldp.cy = sz.cy;
			imldp.x = pt.x;
			imldp.y = pt.y;

			GetImageListDrawParams(nCol,nRow,&imldp,FALSE);

			if(imldp.himl != NULL)
			{

				ImageList_DrawIndirect(&imldp);
			}

			break;
		}
	}
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

int CGridView::GetHeadColWidth(int nCol, int nHeadRow)
{
	if(nHeadRow >= 0 && nHeadRow == GetHeadRowCount()-2)
	{
		switch(nCol)
		{
		case 0:
			return CALXGrid::GetHeadColWidth(0) + CALXGrid::GetHeadColWidth(1) + CALXGrid::GetHeadColWidth(2);
		case 1:
			return CALXGrid::GetHeadColWidth(3);
		case 2:
			return CALXGrid::GetHeadColWidth(4) + CALXGrid::GetHeadColWidth(5);
		case 3:
			return CALXGrid::GetHeadColWidth(6) + CALXGrid::GetHeadColWidth(7);
		case 4:
			return CALXGrid::GetHeadColWidth(8) + CALXGrid::GetHeadColWidth(9);
		case 5:
			return CALXGrid::GetHeadColWidth(10) + CALXGrid::GetHeadColWidth(11);
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
		return 6;
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
				HeadCellInfo.m_strText = "Bitmap";
				HeadCellInfo.m_nFormat = AHFF_CENTER;
				break;
			}
		case 2:
			{
				HeadCellInfo.m_strText = "CheckBox";
				HeadCellInfo.m_nFormat = AHFF_CENTER;
				break;
			}
		case 3:
			{
				HeadCellInfo.m_strText = "CheckBox && Text";
				HeadCellInfo.m_nFormat = AHFF_CENTER;
				break;
			}
		case 4:
			{
				HeadCellInfo.m_strText = "DropDownComboBox";
				HeadCellInfo.m_nFormat = AHFF_CENTER;
				break;
			}
		case 5:
			{
				HeadCellInfo.m_strText = "ListComboBox";
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
				CALXGrid::SetHeadColWidth( 0, nRow, nNewWidth/3);
				CALXGrid::SetHeadColWidth( 1, nRow, nNewWidth/3);
				CALXGrid::SetHeadColWidth( 2, nRow, nNewWidth/3);
				break;
			}
		case 1:
			{
				CALXGrid::SetHeadColWidth( 3, nRow, nNewWidth);
				break;
			}
		case 2:
			{
				CALXGrid::SetHeadColWidth( 4, nRow, nNewWidth/2);
				CALXGrid::SetHeadColWidth( 5, nRow, nNewWidth/2);
				break;
			}
		case 3:
			{
				CALXGrid::SetHeadColWidth( 6, nRow, nNewWidth/2);
				CALXGrid::SetHeadColWidth( 7, nRow, nNewWidth/2);
				break;
			}
		case 4:
			{
				CALXGrid::SetHeadColWidth( 8, nRow, nNewWidth/2);
				CALXGrid::SetHeadColWidth( 9, nRow, nNewWidth/2);
				break;
			}
		case 5:
			{
				CALXGrid::SetHeadColWidth( 10, nRow, nNewWidth/2);
				CALXGrid::SetHeadColWidth( 11, nRow, nNewWidth/2);
				break;
			}
		}
	}
	else
		CALXGrid::SetHeadColWidth( nCol, nRow, nNewWidth);
}


void CGridView::OnGStyleNoGridLines() 
{
	if(GetStyle() & AGS_NOGRIDLINES)
		ModifyStyle(AGS_NOGRIDLINES,0);
	else
		ModifyStyle(0,AGS_NOGRIDLINES);
	Invalidate();
}

void CGridView::OnUpdateGStyleNoGridLines(CCmdUI* pCmdUI) 
{
	if(GetStyle() & AGS_NOGRIDLINES)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
}
