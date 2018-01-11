// GridView.cpp : implementation of the CGridView class
//

#include "stdafx.h"
#include "Grid.h"

#include "PaymentSet.h"
#include "GridDoc.h"
#include "GridView.h"
#include "ColumnsDlg.h"

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
	ON_COMMAND(ID_VIEW_COLUMNS, OnViewColumns)
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
	m_pSet = NULL;

	for(int i = 0; i < GRIDCOLCOUNT; i++)
	{
		m_aFieldsColInfo[i].m_bVisibl = TRUE;
		m_aFieldsColInfo[i].m_iField = i;
		m_aFieldsColInfo[i].m_iCol = i;
		m_aFieldsColInfo[i].m_nWidth = -1;
	}

	m_aFieldsColInfo[0].m_nWidth = 70;
	m_aFieldsColInfo[1].m_nWidth = 200;
	m_aFieldsColInfo[2].m_nWidth = 100;
	m_aFieldsColInfo[3].m_nWidth = 100;
	m_aFieldsColInfo[4].m_nWidth = 100;

	DefineColCtrl(AddCol(0, "", ACFF_CENTER, AHFF_CENTER, 0, 0, 0), GA_CELLCTRL);
	SetFocusRect(::GetSysColor(COLOR_WINDOWTEXT),::GetSysColor(COLOR_WINDOW),FALSE);
}

CGridView::~CGridView()
{
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

//	cs.style |= AGS_FLAT;
	return CALXGridView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGridView data exchange

CELL_DATA CGridView::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	if(m_pSet == NULL || !m_pSet->IsOpen())
		return CellData;

	m_pSet->SetAbsolutePosition(nRow);

	switch(GetCellCtrlID(nCol,nRow))
	{
	case ID_COL_DATE:
		{
			if(m_pSet->m_DATE.GetStatus() == COleDateTime::valid)
				CellData.m_strText = m_pSet->m_DATE.Format(VAR_DATEVALUEONLY);
			break;
		}
	case ID_COL_NAME:
		{
			CellData.m_strText = m_pSet->m_NAME;
			break;
		}
	case ID_COL_PAYMENT_CHANGED:
		{
			CellData.m_strText = m_pSet->m_PAYMENT_CHANGED.Format();
			break;
		}
	case ID_COL_IS_PAID:
		{
			CellData.m_strText = m_pSet->m_IS_PAID.Format();
			break;
		}
	case ID_COL_FOR_PAYMENT:
		{
			COleCurrency curForPayment(0,0);

			if(m_pSet->m_PAYMENT_CHANGED.GetStatus() == COleCurrency::valid)
				curForPayment = m_pSet->m_PAYMENT_CHANGED;

			if(m_pSet->m_IS_PAID.GetStatus() == COleCurrency::valid)
				curForPayment = curForPayment - m_pSet->m_IS_PAID;

			CellData.m_strText = curForPayment.Format();
			break;
		}
	}

	return CellData;
}

BOOL CGridView::OnSaveCellData(int nCol, int nRow)
{
	CGridDoc* pDoc = GetDocument();
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA Data = pCellCtrl->GetCellData();

		m_pSet->SetAbsolutePosition(nRow);

		m_pSet->Edit();

		switch(GetCellCtrlID(nCol,nRow))
		{
		case ID_COL_DATE:
			{
			m_pSet->m_DATE.ParseDateTime(Data.m_strText);
			break;
			}
		case ID_COL_NAME:
			{
			m_pSet->m_NAME = Data.m_strText;
			break;
			}
		case ID_COL_PAYMENT_CHANGED:
			{
			m_pSet->m_PAYMENT_CHANGED.ParseCurrency(Data.m_strText); 

			for(int i = 0; i < GRIDCOLCOUNT; i ++)
				if(m_aFieldsColInfo[i].m_iField == 4 && m_aFieldsColInfo[i].m_iCol > 0)
					InvalidateCell(m_aFieldsColInfo[i].m_iCol,nRow);
			break;
			}
		case ID_COL_IS_PAID:
			{
			m_pSet->m_IS_PAID.ParseCurrency(Data.m_strText); 

			for(int i = 0; i < GRIDCOLCOUNT; i ++)
				if(m_aFieldsColInfo[i].m_iField == 4 && m_aFieldsColInfo[i].m_iCol > 0)
					InvalidateCell(m_aFieldsColInfo[i].m_iCol,nRow);
			break;
			}
		}
	}

	m_pSet->Update();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridView drawing

void CGridView::OnDraw(CDC* pDC)
{
	CGridDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CGridView::OnInitialUpdate()
{
	CALXGrid::SetSortCol(-1, -1, None);

	for(int i = 0; i < GetHeadColCount(); i++)
		DestroyColCtrl(i);

	RemoveAllCol();

	m_pSet = &GetDocument()->m_Set;

	int iCol, nWidth;
	CString strTmp;
	for(int ind = 0; ind < GRIDCOLCOUNT; ind++)
	{
		if(m_aFieldsColInfo[ind].m_bVisibl)
		{
			iCol = m_aFieldsColInfo[ind].m_iField;
			nWidth = m_aFieldsColInfo[ind].m_nWidth;

			switch(iCol)
			{
			case 0:
				{
					strTmp.LoadString(ID_COL_DATE);
					DefineColCtrl(AddCol(nWidth, strTmp, ACFF_CENTER, AHFF_CENTER, 0, 0, ID_COL_DATE), GA_EDITCTRL, WS_CHILD | ES_CENTER);
					break;
				}
			case 1:
				{
					strTmp.LoadString(ID_COL_NAME);
					DefineColCtrl(AddCol(nWidth, strTmp, ACFF_LEFT, AHFF_LEFT, 0, 0, ID_COL_NAME), GA_EDITCTRL, WS_CHILD | ES_LEFT);
					break;
				}
			case 2:
				{
					strTmp.LoadString(ID_COL_PAYMENT_CHANGED);
					DefineColCtrl(AddCol(nWidth, strTmp, ACFF_RIGHT, AHFF_RIGHT, 0, 0, ID_COL_PAYMENT_CHANGED), GA_EDITCTRL, WS_CHILD | ES_RIGHT);
					break;
				}
			case 3:
				{
					strTmp.LoadString(ID_COL_IS_PAID);
					DefineColCtrl(AddCol(nWidth, strTmp, ACFF_RIGHT, AHFF_RIGHT, 0, 0, ID_COL_IS_PAID), GA_EDITCTRL, WS_CHILD | ES_RIGHT);
					break;
				}
			case 4:
				{
					strTmp.LoadString(ID_COL_FOR_PAYMENT);
					DefineColCtrl(AddCol(nWidth, strTmp, ACFF_RIGHT, AHFF_RIGHT, 0, 0, ID_COL_FOR_PAYMENT), GA_CELLCTRL);
					break;
				}
			}
			m_aFieldsColInfo[ind].m_iCol = GetHeadColCount()-1;
		}
		else
			m_aFieldsColInfo[ind].m_iCol = -1;
	}

	CALXGridView::OnInitialUpdate();

	m_pSet->m_strSort = "";
	m_pSet->Requery();
	m_pSet->MoveLast();
	SetGridRowCount(m_pSet->GetRecordCount());
	UpdateScrollSizes();
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
}

void CGridView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
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


void CGridView::OnViewColumns() 
{
	SaveModifyed(FALSE);

	CColumnsDlg m_ColumnsDlg;

	for(int i = 0; i < GRIDCOLCOUNT; i++)
		m_ColumnsDlg.m_wndCtrl.m_aFieldsColInfo[i] = m_aFieldsColInfo[i];

	if(m_ColumnsDlg.DoModal() == IDOK)
	{
		for(int i = 0; i < GRIDCOLCOUNT; i++)
			m_aFieldsColInfo[i] =	m_ColumnsDlg.m_wndCtrl.m_aFieldsColInfo[i];

		SetActiveCell(0,0);
		OnInitialUpdate();
	}
}

void CGridView::SetHeadColWidth(int nCol, int nRow, int nNewWidth)
{
	CALXGrid::SetHeadColWidth(nCol,nRow,nNewWidth);
	int i = 0;
	while(m_aFieldsColInfo[i].m_iCol != nCol && i < GRIDCOLCOUNT-1)
		i++;

	if(m_aFieldsColInfo[i].m_iCol == nCol)
		m_aFieldsColInfo[i].m_nWidth = GetHeadColWidth(nCol,nRow);
}


void CGridView::OnHeadButtonDown(int nHeadCol, int nHeadRow)
{
	int nPrevSortCol, nPrevSortRow;
	ALXSortOrder nSortOrder;

	CALXGrid::GetSortCol(nPrevSortCol, nPrevSortRow, nSortOrder);

	if(nPrevSortCol == nHeadCol && nPrevSortRow == nHeadRow &&nSortOrder == Ascending)
		nSortOrder = Descending;
	else
		nSortOrder = Ascending;


	SaveModifyed(FALSE);

	m_pSet->SetAbsolutePosition(GetActiveRow());

	long nCode = m_pSet->m_CODE;

	switch(GetCellCtrlID(nHeadCol,-1))
	{
	case ID_COL_DATE:
		{
			m_pSet->m_strSort = "DATE";
			break;
		}
	case ID_COL_NAME:
		{
			m_pSet->m_strSort = "NAME";
			break;
		}
	case ID_COL_PAYMENT_CHANGED:
		{
			m_pSet->m_strSort = "PAYMENT_CHANGED";
			break;
		}
	case ID_COL_IS_PAID:
		{
			m_pSet->m_strSort = "IS_PAID";
			break;
		}
	default:
		{
			m_pSet->m_strSort = "";
			nSortOrder = None;
		}
	}

	if(nSortOrder == Descending)
		m_pSet->m_strSort = m_pSet->m_strSort + " DESC";

	CALXGrid::SetSortCol(nHeadCol, nHeadRow, nSortOrder);

	m_pSet->Requery();
	m_pSet->MoveLast();

	CString strFind;
	strFind.Format("CODE=%u",nCode);
	if(m_pSet->FindFirst(strFind))
		SetActiveCell(nHeadCol,m_pSet->GetAbsolutePosition());

	Invalidate();
	return;
}

