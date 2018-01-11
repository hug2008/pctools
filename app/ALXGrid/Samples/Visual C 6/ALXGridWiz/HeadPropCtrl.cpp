// HeadPropCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ALXGridwz.h"
#include "HeadPropCtrl.h"
#include "ALXGridaw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeadPropCtrl

IMPLEMENT_DYNCREATE(CHeadPropCtrl, CXGridCtrl)

BEGIN_MESSAGE_MAP(CHeadPropCtrl, CXGridCtrl)
	//{{AFX_MSG_MAP(CHeadPropCtrl)
	ON_COMMAND(IDC_CELLS_CTRL, OnSelCtrl)
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CHeadPropCtrl::CHeadPropCtrl()
{
	m_pPropSheet = NULL;

	m_pctrlTypeCtrl = NULL;
	m_pctrlEditStyle = NULL;
	m_pctrlCheckStyle = NULL;
	m_pctrlComboStyle = NULL;

	DefineColCtrl(AddCol(25,"N", ACFF_CENTER, AHFF_CENTER,0,0,IDC_NOM),GA_CELLCTRL);
	DefineColCtrl(AddCol(140,"Title", ACFF_LEFT, AHFF_LEFT,0,0,IDC_TITLE),GA_EDITCTRL,WS_CHILD | ES_LEFT | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(50,"Width", ACFF_CENTER, AHFF_CENTER,0,0,IDC_COL_WIDTH),GA_EDITCTRL,WS_CHILD | ES_CENTER | ES_AUTOHSCROLL);
	DefineColCtrl(AddCol(85,"Head Cell Align", ACFF_LEFT, AHFF_CENTER,0,0,IDC_HEAD_ALIGN),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL);
	DefineColCtrl(AddCol(85,"Column Cells Align", ACFF_LEFT, AHFF_CENTER,0,0,IDC_CELLS_ALIGN),GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL);
	DefineColCtrl(AddCol(100,"Column Cells Control", ACFF_LEFT, AHFF_CENTER,0,0,IDC_CELLS_CTRL),GA_CELLCTRL);//GA_COMBOBOXCTRL,WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL);
	DefineImage(5,19,15,DIT_IMGLIST);

	DefineColCtrl(AddCol(85,"Control Style", ACFF_LEFT, AHFF_CENTER,0,0,IDC_CELLS_CTRL_STYLE),GA_CELLCTRL);//,WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL);
	DefineColCtrl(AddCol(90,"ID", ACFF_LEFT, AHFF_CENTER,0,0,IDC_COL_ID),GA_EDITCTRL,WS_CHILD | ES_LEFT);

	DEFCOL_INFO ColInfo;
	ColInfo.m_strTitle		= "Head 1";
	ColInfo.m_nWidth		= 100;
	ColInfo.m_nHeadAlign	= 0;
	ColInfo.m_nCellAlign	= 0;
	ColInfo.m_nTypeControl	= 1;
	ColInfo.m_nControlStyle	= 0;
	ColInfo.m_strID			= "ID_COL_1";
	m_Cols.Add(ColInfo);

	m_ImageList.Create(IDB_TYPECTRL, 19, 10, (COLORREF)0x808000);

	SetFocusRect(::GetSysColor(COLOR_WINDOWTEXT),::GetSysColor(COLOR_WINDOW),FALSE);
	SetGridRowCount(m_Cols.GetSize());

	SaveInfo();
}

CHeadPropCtrl::~CHeadPropCtrl()
{
	if(m_pctrlTypeCtrl != NULL)
	{
		m_pctrlTypeCtrl->DestroyWindow();
		delete m_pctrlTypeCtrl;
	}
	if(m_pctrlEditStyle != NULL)
	{
		m_pctrlEditStyle->DestroyWindow();
		delete m_pctrlEditStyle;
	}
	if(m_pctrlCheckStyle != NULL)
	{
		m_pctrlCheckStyle->DestroyWindow();
		delete m_pctrlCheckStyle;
	}
	if(m_pctrlComboStyle != NULL)
	{
		m_pctrlComboStyle->DestroyWindow();
		delete m_pctrlComboStyle;
	}
}

CELL_DATA CHeadPropCtrl::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	switch(GetCellCtrlID(nCol,nRow))
	{
	case IDC_NOM:
			CellData.m_strText.Format("%u",nRow+1);
		break;
	case IDC_TITLE:
			CellData.m_strText = m_Cols[nRow].m_strTitle;
		break;
	case IDC_COL_WIDTH:
			CellData.m_strText.Format("%u",m_Cols[nRow].m_nWidth);
		break;
	case IDC_HEAD_ALIGN:
		{
			CellData.m_dwTag = m_Cols[nRow].m_nHeadAlign;
			switch(m_Cols[nRow].m_nHeadAlign)
			{
			case 1:
				CellData.m_strText = "RIGHT";
				break;
			case 2:
				CellData.m_strText = "CENTER";
				break;
			default:
				CellData.m_strText = "LEFT";
			}
		break;
		}
	case IDC_CELLS_ALIGN:
			CellData.m_dwTag = m_Cols[nRow].m_nCellAlign;
			switch(m_Cols[nRow].m_nCellAlign)
			{
			case 1:
				CellData.m_strText = "RIGHT";
				break;
			case 2:
				CellData.m_strText = "CENTER";
				break;
			default:
				CellData.m_strText = "LEFT";
			}
		break;
	case IDC_CELLS_CTRL:
			CellData.m_dwTag = m_Cols[nRow].m_nTypeControl;
			switch(m_Cols[nRow].m_nTypeControl)
			{
			case 1:
				CellData.m_strText = "EDITBOX";
				break;
			case 2:
				CellData.m_strText = "CHECKBOX";
				break;
			case 3:
				CellData.m_strText = "COMBOBOX";
				break;
			default:
				CellData.m_strText = "NONE";
			}
		break;
	case IDC_CELLS_CTRL_STYLE:
			CellData.m_dwTag = m_Cols[nRow].m_nControlStyle;
			switch(m_Cols[nRow].m_nTypeControl)
			{
			case 1:
				{
					if(m_Cols[nRow].m_nControlStyle == 0)
						CellData.m_strText = "Align LEFT";
					else if(m_Cols[nRow].m_nControlStyle == 1)
						CellData.m_strText = "Align RIGHT";
					else
						CellData.m_strText = "Align CENTER";
					break;
				}
			case 2:
				{
					if(m_Cols[nRow].m_nControlStyle == 0)
						CellData.m_strText = "2-STATE";
					else
						CellData.m_strText = "3-STATE";
					break;
				}
			case 3:
				{
					if(m_Cols[nRow].m_nControlStyle == 0)
						CellData.m_strText = "DROPDOWN";
					else
						CellData.m_strText = "DROP LIST";
					break;
				}
			default:
				{
						CellData.m_strText = "NONE";
					break;
				}
			}
		break;
	case IDC_COL_ID:
			CellData.m_strText = m_Cols[nRow].m_strID;
		break;
	}
	return CellData;
}

CALXCellCtrl* CHeadPropCtrl::CreateCellCtrl(int nCol, int nRow, DWORD dwStyle, UINT nID)
{
	CALXCellCtrl* pCellCtrl = CALXGrid::CreateCellCtrl(nCol, nRow, dwStyle);
	if(pCellCtrl != NULL)
	{
		switch(GetCellCtrlID(nCol,nRow))
		{
		case IDC_HEAD_ALIGN:
			{
				CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) pCellCtrl;
				if(pComboBoxCtrl->GetCount() <= 0)
				{
					pComboBoxCtrl->AddString("LEFT");
					pComboBoxCtrl->SetItemData(0,0);
					pComboBoxCtrl->AddString("RIGHT");
					pComboBoxCtrl->SetItemData(1,1);
					pComboBoxCtrl->AddString("CENTER");
					pComboBoxCtrl->SetItemData(2,2);
				}
				break;
			}
		case IDC_CELLS_ALIGN:
			{
				CALXComboBoxCtrl* pComboBoxCtrl = (CALXComboBoxCtrl*) pCellCtrl;
				if(pComboBoxCtrl->GetCount() <= 0)
				{
					pComboBoxCtrl->AddString("LEFT");
					pComboBoxCtrl->SetItemData(0,0);
					pComboBoxCtrl->AddString("RIGHT");
					pComboBoxCtrl->SetItemData(1,1);
					pComboBoxCtrl->AddString("CENTER");
					pComboBoxCtrl->SetItemData(2,2);
				}
				break;
			}
		}
	}
	return pCellCtrl;
}

CALXCellCtrl* CHeadPropCtrl::GetCellCtrl(int nCol, int nRow)
{
	if(GetCellCtrlID(nCol,nRow) == IDC_CELLS_CTRL)
	{
		if(m_pctrlTypeCtrl == NULL)
		{
			m_pctrlTypeCtrl = new CTypeCtrl;
			m_pctrlTypeCtrl->CreateCtrl(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,this,IDC_CELLS_CTRL_STYLE);
			m_pctrlTypeCtrl->EnableCtrl(FALSE);
			m_pctrlTypeCtrl->SetFontCtrl(&m_Font);

			m_pctrlTypeCtrl->AddString("NONE");
			m_pctrlTypeCtrl->SetItemData(0,0);
			m_pctrlTypeCtrl->AddString("EDITBOX");
			m_pctrlTypeCtrl->SetItemData(1,1);
			m_pctrlTypeCtrl->AddString("CHECKBOX");
			m_pctrlTypeCtrl->SetItemData(2,2);
			m_pctrlTypeCtrl->AddString("COMBOBOX");
			m_pctrlTypeCtrl->SetItemData(3,3);
		}
		return m_pctrlTypeCtrl;
	}

	if(GetCellCtrlID(nCol,nRow) != IDC_CELLS_CTRL_STYLE)
		return CXGridCtrl::GetCellCtrl(nCol,nRow);

	switch(m_Cols[nRow].m_nTypeControl)
	{
	case 1:
		{
			if(m_pctrlEditStyle == NULL)
			{
				m_pctrlEditStyle = new CALXComboBoxCtrl;
				m_pctrlEditStyle->CreateCtrl(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,this,IDC_CELLS_CTRL_STYLE);
				m_pctrlEditStyle->EnableCtrl(FALSE);
				m_pctrlEditStyle->SetFontCtrl(&m_Font);

				m_pctrlEditStyle->AddString("Align LEFT");
				m_pctrlEditStyle->SetItemData(0,0);
				m_pctrlEditStyle->AddString("Align RIGHT");
				m_pctrlEditStyle->SetItemData(1,1);
				m_pctrlEditStyle->AddString("Align CENTER");
				m_pctrlEditStyle->SetItemData(2,2);
			}
			return m_pctrlEditStyle;
		}
	case 2:
		{
			if(m_pctrlCheckStyle == NULL)
			{
				m_pctrlCheckStyle = new CALXComboBoxCtrl;
				m_pctrlCheckStyle->CreateCtrl(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,this,IDC_CELLS_CTRL_STYLE);
				m_pctrlCheckStyle->EnableCtrl(FALSE);
				m_pctrlCheckStyle->SetFontCtrl(&m_Font);

				m_pctrlCheckStyle->AddString("2-STATE");
				m_pctrlCheckStyle->SetItemData(0,0);
				m_pctrlCheckStyle->AddString("3-STATE");
				m_pctrlCheckStyle->SetItemData(1,1);
			}
			return m_pctrlCheckStyle;
		}
	case 3:
		{
			if(m_pctrlComboStyle == NULL)
			{
				m_pctrlComboStyle = new CALXComboBoxCtrl;
				m_pctrlComboStyle->CreateCtrl(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,this,IDC_CELLS_CTRL_STYLE);
				m_pctrlComboStyle->EnableCtrl(FALSE);
				m_pctrlComboStyle->SetFontCtrl(&m_Font);

				m_pctrlComboStyle->AddString("DROPDOWN");
				m_pctrlComboStyle->SetItemData(0,0);
				m_pctrlComboStyle->AddString("DROP LIST");
				m_pctrlComboStyle->SetItemData(1,1);
			}
			return m_pctrlComboStyle;
		}
	}
	return NULL;
}

BOOL CHeadPropCtrl::OnSaveCellData(int nCol, int nRow)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA CellData = pCellCtrl->GetCellData();
		switch(GetCellCtrlID(nCol,nRow))
		{
		case IDC_TITLE:
				m_Cols[nRow].m_strTitle = CellData.m_strText;
			break;
		case IDC_COL_WIDTH:
				m_Cols[nRow].m_nWidth = atoi(CellData.m_strText);
			break;
		case IDC_HEAD_ALIGN:
				m_Cols[nRow].m_nHeadAlign = CellData.m_dwTag;
			break;
		case IDC_CELLS_ALIGN:
				m_Cols[nRow].m_nCellAlign = CellData.m_dwTag;
			break;
		case IDC_CELLS_CTRL:
			{
				m_Cols[nRow].m_nTypeControl = CellData.m_dwTag;
				if(m_Cols[nRow].m_nTypeControl != 1 && m_Cols[nRow].m_nControlStyle > 1)
					m_Cols[nRow].m_nControlStyle = 0;
				InvalidateCell(6,GetActiveRow());
				break;
			}
		case IDC_CELLS_CTRL_STYLE:
				m_Cols[nRow].m_nControlStyle = CellData.m_dwTag;
			break;
		case IDC_COL_ID:
			{
				CellData.m_strText.Replace(" ","");
				CellData.m_strText.MakeUpper();
				m_Cols[nRow].m_strID = CellData.m_strText;
			break;
			}
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CHeadPropCtrl message handlers



void CHeadPropCtrl::OnSelCtrl() 
{
	OnSaveCellData(GetActiveCol(), GetActiveRow());
	InvalidateCell(5,GetActiveRow());
}

void CHeadPropCtrl::OnAddCol()
{
	m_pPropSheet = new CColPropSheet;

	m_pPropSheet->m_bEdit = FALSE;
	m_pPropSheet->m_nCol = m_Cols.GetSize()+1;
	m_pPropSheet->m_Page1.m_pWnd = this;
	m_pPropSheet->m_Page1.m_strTitle.Format("Head %u",m_Cols.GetSize()+1);
	m_pPropSheet->m_Page1.m_nWidth = 100;
	m_pPropSheet->m_Page1.m_strID.Format("ID_COL_%u",m_Cols.GetSize()+1);
	m_pPropSheet->m_Page2.m_pWnd = this;
	m_pPropSheet->m_Page2.m_nCAlign = 0;
	m_pPropSheet->m_Page2.m_nHAlign = 0;
	m_pPropSheet->m_Page3.m_pWnd = this;
	m_pPropSheet->m_Page3.m_nCtrl = 1;
	m_pPropSheet->m_Page3.m_nEAlign = 0;
	m_pPropSheet->m_Page3.m_nCHState = 0;
	m_pPropSheet->m_Page3.m_nCBDrop = 0;
	if(m_pPropSheet->DoModal() == IDOK)
	{
		if(m_pPropSheet->m_bEdit)
		{
			int IndCol = GetActiveRow();

			m_Cols[IndCol].m_strTitle		= m_pPropSheet->m_Page1.m_strTitle;
			m_Cols[IndCol].m_nWidth		= m_pPropSheet->m_Page1.m_nWidth;
			m_Cols[IndCol].m_strID			= m_pPropSheet->m_Page1.m_strID;
			m_Cols[IndCol].m_nHeadAlign	= m_pPropSheet->m_Page2.m_nHAlign;
			m_Cols[IndCol].m_nCellAlign	= m_pPropSheet->m_Page2.m_nCAlign;
			m_Cols[IndCol].m_nTypeControl	= m_pPropSheet->m_Page3.m_nCtrl;

			switch(m_Cols[IndCol].m_nTypeControl)
			{
			case 1:
				m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nEAlign;
				break;
			case 2:
				m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nCHState;
				break;
			case 3:
				m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nCBDrop;
				break;
			default:
				m_Cols[IndCol].m_nControlStyle = 0;
			}

			InvalidateRow(IndCol);
		}
		else
		{
			DEFCOL_INFO ColInfo;
			ColInfo.m_strTitle		= m_pPropSheet->m_Page1.m_strTitle;
			ColInfo.m_nWidth		= m_pPropSheet->m_Page1.m_nWidth;
			ColInfo.m_strID			= m_pPropSheet->m_Page1.m_strID;
			ColInfo.m_nHeadAlign	= m_pPropSheet->m_Page2.m_nHAlign;
			ColInfo.m_nCellAlign	= m_pPropSheet->m_Page2.m_nCAlign;
			ColInfo.m_nTypeControl	= m_pPropSheet->m_Page3.m_nCtrl;
			switch(ColInfo.m_nTypeControl)
			{
			case 1:
				ColInfo.m_nControlStyle = m_pPropSheet->m_Page3.m_nEAlign;
				break;
			case 2:
				ColInfo.m_nControlStyle = m_pPropSheet->m_Page3.m_nCHState;
				break;
			case 3:
				ColInfo.m_nControlStyle = m_pPropSheet->m_Page3.m_nCBDrop;
				break;
			default:
				ColInfo.m_nControlStyle = 0;
			}


			m_Cols.Add(ColInfo);
			InsertRows(m_Cols.GetSize()-1);
		}

	}

	delete m_pPropSheet;
	m_pPropSheet = NULL;
}

void CHeadPropCtrl::GetImageListDrawParams(int nCol, int nRow, IMAGELISTDRAWPARAMS* pimldp, BOOL bSelected)
{
	pimldp->himl = m_ImageList.m_hImageList;
//	if(GetActiveRow() == nRow && GetActiveCol() == nCol)
//		pimldp->fStyle = ILD_SELECTED;

	pimldp->i = m_Cols[nRow].m_nTypeControl;
}

void CHeadPropCtrl::SaveInfo()
{
	ALXAppWz.m_strGridConstr = "\tCString strTmp;\n";
	CString strTmp1,strTmp,strCAligh,strHAlign,strCtrl,strStyle;
	ALXAppWz.m_strGridStr = "STRINGTABLE DISCARDABLE\nBEGIN\n";
	ALXAppWz.m_strGridRes = "";

	for(int i = 0; i < m_Cols.GetSize(); i++)
	{
		switch(m_Cols[i].m_nCellAlign)
		{
		case 1:
			strCAligh = "ACFF_RIGHT";
			break;
		case 2:
			strCAligh = "ACFF_CENTER";
			break;
		default:
			strCAligh = "ACFF_LEFT";
			break;
		}
		
		switch(m_Cols[i].m_nHeadAlign)
		{
		case 1:
			strHAlign = "AHFF_RIGHT";
			break;
		case 2:
			strHAlign = "AHFF_CENTER";
			break;
		default:
			strHAlign = "AHFF_LEFT";
			break;
		}

		if(m_Cols[i].m_nTypeControl > 0)
		{
			switch(m_Cols[i].m_nTypeControl)
			{
			case 1:
				{
					strCtrl = "GA_EDITCTRL";
					switch(m_Cols[i].m_nControlStyle)
					{
					case 1:
						strStyle = "WS_CHILD | ES_RIGHT";
						break;
					case 2:
						strStyle = "WS_CHILD | ES_CENTER";
						break;
					default:
						strStyle = "WS_CHILD | ES_LEFT";
					}
					strTmp1.Format("\tstrTmp.LoadString(%s);\n",m_Cols[i].m_strID,strCtrl);
					strTmp.Format("\tDefineColCtrl(AddCol(%u, strTmp, %s, %s, 0, 0, %s), %s, %s);\n",m_Cols[i].m_nWidth,strCAligh,strHAlign,m_Cols[i].m_strID,strCtrl,strStyle);
					break;
				}
			case 2:
				{
					strCtrl = "GA_BUTTONCTRL";
					if(m_Cols[i].m_nControlStyle  == 1)
					{
						strStyle = "WS_CHILD | BS_AUTO3STATE | BS_FLAT";
						strTmp1.Format("\tstrTmp.LoadString(%s);\n",m_Cols[i].m_strID,strCtrl);
						strTmp.Format("\tDefineColCtrl(AddCol(%u, strTmp, %s, %s, DFC_BUTTON,DFCS_BUTTON3STATE | DFCS_FLAT, %s), %s, %s);\n",m_Cols[i].m_nWidth,strCAligh,strHAlign,m_Cols[i].m_strID,strCtrl,strStyle);
					}
					else
					{
						strStyle = "WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT";
						strTmp1.Format("\tstrTmp.LoadString(%s);\n",m_Cols[i].m_strID,strCtrl);
						strTmp.Format("\tDefineColCtrl(AddCol(%u, strTmp, %s, %s, DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_FLAT, %s), %s, %s);\n",m_Cols[i].m_nWidth,strCAligh,strHAlign,m_Cols[i].m_strID,strCtrl,strStyle);
					}
					break;
				}
			case 3:
				{
					strCtrl = "GA_COMBOBOXCTRL";
					if(m_Cols[i].m_nControlStyle  == 1)
						strStyle = "WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL";
					else
						strStyle = "WS_CHILD | CBS_DROPDOWN | WS_VSCROLL";
					strTmp1.Format("\tstrTmp.LoadString(%s);\n",m_Cols[i].m_strID,strCtrl);
					strTmp.Format("\tDefineColCtrl(AddCol(%u, strTmp, %s, %s, 0, 0, %s), %s, %s);\n",m_Cols[i].m_nWidth,strCAligh,strHAlign,m_Cols[i].m_strID,strCtrl,strStyle);
					break;
				}
			default:
				{
					strCtrl = "GA_CELLCTRL";
					strStyle = "WS_CHILD";
					strTmp1.Format("\tstrTmp.LoadString(%s);\n",m_Cols[i].m_strID,strCtrl);
					strTmp.Format("DefineColCtrl(AddCol(%u, strTmp, %s, %s, 0, 0, %s), %s, %s);\n",m_Cols[i].m_nWidth,strCAligh,strHAlign,m_Cols[i].m_strID,strCtrl,strStyle);
					break;
				}
			}
		}
		else
		{
			strTmp1.Format("\tstrTmp.LoadString(%s);\n",m_Cols[i].m_strID,strCtrl);
			strTmp.Format("\tAddCol(%u, strTmp, %s, %s,0,0,%s);\n",m_Cols[i].m_nWidth,strCAligh,strHAlign,m_Cols[i].m_strID);
		}

		ALXAppWz.m_strGridConstr = ALXAppWz.m_strGridConstr + strTmp1 + strTmp;
		strTmp.Format("%u",1000+i);
		ALXAppWz.m_strGridRes = ALXAppWz.m_strGridRes + "\n#define "+m_Cols[i].m_strID + "   "+strTmp;
		strTmp.Format("%u",1000+i);

		ALXAppWz.m_strGridStr = ALXAppWz.m_strGridStr + "   " + m_Cols[i].m_strID + "   \"" + m_Cols[i].m_strTitle + "\"\n";
	}
	ALXAppWz.m_strGridStr = ALXAppWz.m_strGridStr + "END\n";
}

void CHeadPropCtrl::OnDelCol()
{
	if(GetGridRowCount() > 1)
	{
		m_Cols.RemoveAt(GetActiveRow());
		RemoveRows(GetActiveRow());
	}
}

void CHeadPropCtrl::OnMoveColUp()
{
	if(GetGridRowCount() > 1 && GetActiveRow() > 0)
	{
		DEFCOL_INFO ColInfo;
		int IndCol = GetActiveRow();
		ColInfo = m_Cols[IndCol-1];
		m_Cols[IndCol-1] = m_Cols[IndCol];
		m_Cols[IndCol] = ColInfo;
		Invalidate();
		SetActivePrevRow();
	}
}

void CHeadPropCtrl::OnMoveColDown()
{
	if(GetActiveRow() < GetGridRowCount() - 1)
	{
		DEFCOL_INFO ColInfo;
		int IndCol = GetActiveRow();
		ColInfo = m_Cols[IndCol+1];
		m_Cols[IndCol+1] = m_Cols[IndCol];
		m_Cols[IndCol] = ColInfo;
		Invalidate();
		SetActiveNextRow();
	}
}

void CHeadPropCtrl::OnApplyNow() 
{
	if(m_pPropSheet == NULL)
		return;

	if(m_pPropSheet->m_bEdit)
	{
		int IndCol = GetActiveRow();

		m_Cols[IndCol].m_strTitle	= m_pPropSheet->m_Page1.m_strTitle;
		m_Cols[IndCol].m_nWidth		= m_pPropSheet->m_Page1.m_nWidth;
		m_Cols[IndCol].m_strID		= m_pPropSheet->m_Page1.m_strID;
		m_Cols[IndCol].m_nHeadAlign	= m_pPropSheet->m_Page2.m_nHAlign;
		m_Cols[IndCol].m_nCellAlign	= m_pPropSheet->m_Page2.m_nCAlign;
		m_Cols[IndCol].m_nTypeControl= m_pPropSheet->m_Page3.m_nCtrl;

		switch(m_Cols[IndCol].m_nTypeControl)
		{
		case 1:
			m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nEAlign;
			break;
		case 2:
			m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nCHState;
			break;
		case 3:
			m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nCBDrop;
			break;
		default:
			m_Cols[IndCol].m_nControlStyle = 0;
		}

		CALXCellCtrl* pCellCtrl = GetCellCtrl(GetActiveCol(),GetActiveRow());
		if(pCellCtrl != NULL)
		{
			pCellCtrl->SetData(GetCellData(GetActiveCol(),GetActiveRow()));
		}
		InvalidateRow(GetActiveRow());
	}
	else
	{
		DEFCOL_INFO ColInfo;
		ColInfo.m_strTitle		= m_pPropSheet->m_Page1.m_strTitle;
		ColInfo.m_nWidth		= m_pPropSheet->m_Page1.m_nWidth;
		ColInfo.m_strID			= m_pPropSheet->m_Page1.m_strID;
		ColInfo.m_nHeadAlign	= m_pPropSheet->m_Page2.m_nHAlign;
		ColInfo.m_nCellAlign	= m_pPropSheet->m_Page2.m_nCAlign;
		ColInfo.m_nTypeControl	= m_pPropSheet->m_Page3.m_nCtrl;

		switch(ColInfo.m_nTypeControl)
		{
		case 1:
			ColInfo.m_nControlStyle = m_pPropSheet->m_Page3.m_nEAlign;
			break;
		case 2:
			ColInfo.m_nControlStyle = m_pPropSheet->m_Page3.m_nCHState;
			break;
		case 3:
			ColInfo.m_nControlStyle = m_pPropSheet->m_Page3.m_nCBDrop;
			break;
		default:
			ColInfo.m_nControlStyle = 0;
		}
		m_Cols.Add(ColInfo);
		InsertRows(m_Cols.GetSize()-1);
	}

	m_pPropSheet->m_bEdit = TRUE;
}


void CHeadPropCtrl::OnEditCol()
{
	m_pPropSheet = new CColPropSheet;

	int IndCol = GetActiveRow();

	m_pPropSheet->m_bEdit = TRUE;
	m_pPropSheet->m_nCol  = IndCol + 1;
	m_pPropSheet->m_Page1.m_pWnd		= this;
	m_pPropSheet->m_Page1.m_strTitle	= m_Cols[IndCol].m_strTitle;
	m_pPropSheet->m_Page1.m_nWidth		= m_Cols[IndCol].m_nWidth;
	m_pPropSheet->m_Page1.m_strID		= m_Cols[IndCol].m_strID;
	m_pPropSheet->m_Page2.m_pWnd		= this;
	m_pPropSheet->m_Page2.m_nCAlign		= m_Cols[IndCol].m_nCellAlign;
	m_pPropSheet->m_Page2.m_nHAlign		= m_Cols[IndCol].m_nHeadAlign;
	m_pPropSheet->m_Page3.m_pWnd		= this;
	m_pPropSheet->m_Page3.m_nCtrl		= m_Cols[IndCol].m_nTypeControl;

	if(m_Cols[IndCol].m_nTypeControl == 1)
		m_pPropSheet->m_Page3.m_nEAlign = m_Cols[IndCol].m_nControlStyle;
	else
		m_pPropSheet->m_Page3.m_nEAlign = 0;
	if(m_Cols[IndCol].m_nTypeControl == 2)
		m_pPropSheet->m_Page3.m_nCHState = m_Cols[IndCol].m_nControlStyle;
	else
		m_pPropSheet->m_Page3.m_nCHState = 0;
	if(m_Cols[IndCol].m_nTypeControl == 3)
		m_pPropSheet->m_Page3.m_nCBDrop = m_Cols[IndCol].m_nControlStyle;
	else
		m_pPropSheet->m_Page3.m_nCBDrop = 0;

	if(m_pPropSheet->DoModal() == IDOK)
	{
		m_Cols[IndCol].m_strTitle		= m_pPropSheet->m_Page1.m_strTitle;
		m_Cols[IndCol].m_nWidth			= m_pPropSheet->m_Page1.m_nWidth;
		m_Cols[IndCol].m_strID			= m_pPropSheet->m_Page1.m_strID;
		m_Cols[IndCol].m_nHeadAlign		= m_pPropSheet->m_Page2.m_nHAlign;
		m_Cols[IndCol].m_nCellAlign		= m_pPropSheet->m_Page2.m_nCAlign;
		m_Cols[IndCol].m_nTypeControl	= m_pPropSheet->m_Page3.m_nCtrl;

		switch(m_Cols[IndCol].m_nTypeControl)
		{
		case 1:
			m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nEAlign;
			break;
		case 2:
			m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nCHState;
			break;
		case 3:
			m_Cols[IndCol].m_nControlStyle = m_pPropSheet->m_Page3.m_nCBDrop;
			break;
		default:
			m_Cols[IndCol].m_nControlStyle = 0;
		}

		InvalidateRow(IndCol);

	}

	delete m_pPropSheet;
	m_pPropSheet = NULL;
}
