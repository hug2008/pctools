// GridView.cpp : implementation of the CGridView class
//

#include "stdafx.h"
#include "Grid.h"

#include "GridDoc.h"
#include "GridView.h"

#define GA_SPINCTRL		0x10

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
	ON_COMMAND(WM_CLOSE, OnClose)
	ON_WM_CREATE()
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
	VERIFY(m_ImageList.Create(IDB_BITMAP_IMGLIST, 7, 1, (COLORREF)0x808000));

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

	// Custom control
	m_pSpinCtrl = new CALXSpinCtrl;

	COL_INFO ColInfo;
	ColInfo.m_nWidth = 90;
	ColInfo.m_HeadInfo.m_strHead.LoadString(ID_COL_CUSTOMCTRL);
	ColInfo.m_HeadInfo.m_nFormat = AHFF_RIGHT;
	ColInfo.m_CellsInfo.m_nFormat = AHFF_RIGHT;
	ColInfo.m_CellsInfo.m_nStateFrameCtrl = 0;
	ColInfo.m_CellsInfo.m_nTypeFrameCtrl = 0;
	ColInfo.m_CellsInfo.m_nTypeImage = NULL;
	ColInfo.m_CtrlInfo.m_nType = GA_SPINCTRL;
	ColInfo.m_CtrlInfo.m_dwStyle = WS_CHILD | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_SETBUDDYINT;
	ColInfo.m_CtrlInfo.m_nID = ID_COL_CUSTOMCTRL;
	ColInfo.m_CtrlInfo.m_pColCtrl = m_pSpinCtrl;
	
	AddCol(ColInfo);

}

CGridView::~CGridView()
{
	RemoveAllCol();
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
		case ID_COL_CUSTOMCTRL:
			{
				CellData.m_dwTag = pDoc->m_Rows[nRow].m_nCustomCtrl;
				CellData.m_strText.Format("%u",pDoc->m_Rows[nRow].m_nCustomCtrl);
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
			case ID_COL_CUSTOMCTRL:
				{
					pDoc->m_Rows[nRow].m_nCustomCtrl = atoi(Data.m_strText);
					break;
				}
		}
	}
	return TRUE;
}



void CGridView::OnClose() 
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(),GetActiveRow());
	if(pCellCtrl != NULL)
	{
		if(pCellCtrl->IsModifyed())
			pCellCtrl->SetModify(FALSE);
	}
}

int CGridView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CALXGridView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pSpinCtrl->CreateCtrl(WS_CHILD | UDS_ARROWKEYS | UDS_ALIGNRIGHT | UDS_SETBUDDYINT,this,ID_COL_CUSTOMCTRL);
	m_pSpinCtrl->SetFontCtrl(GetGridFont());
	m_pSpinCtrl->SetRange(0, 255);
	m_pSpinCtrl->SetPos(0);
	
	return 0;
}

BOOL CGridView::DestroyColCtrl(int nCol)
{
	switch(GetCellCtrlID(nCol,0))
	{
	case ID_COL_CUSTOMCTRL:
		{
			delete m_pSpinCtrl;
			m_pSpinCtrl = NULL;
			break;
		}
	default:
		CALXGridView::DestroyColCtrl(nCol);
	}

	return TRUE;
}
