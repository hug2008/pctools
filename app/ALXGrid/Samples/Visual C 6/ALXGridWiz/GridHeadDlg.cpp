// cstm1dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ALXGridwz.h"
#include "GridHeadDlg.h"
#include "ALXGridaw.h"
#include "PrevDlg.h"
#include "WizUtil.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridHeadDlg dialog


CGridHeadDlg::CGridHeadDlg()
	: CAppWizStepDlg(CGridHeadDlg::IDD)
{
	//{{AFX_DATA_INIT(CGridHeadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bmpGrid.LoadBitmap(IDB_GRID);
	m_bmpFon.LoadBitmap(IDB_FON);
}


void CGridHeadDlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridHeadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

// This is called whenever the user presses Next, Back, or Finish with this step
//  present.  Do all validation & data exchange from the dialog in this function.
BOOL CGridHeadDlg::OnDismiss()
{
	m_wndHeadProp.SaveModifyed(TRUE);
	m_wndHeadProp.SaveInfo();

	if (!UpdateData(TRUE))
		return FALSE;

	return TRUE;	// return FALSE if the dialog shouldn't be dismissed
}


BEGIN_MESSAGE_MAP(CGridHeadDlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CGridHeadDlg)
	ON_WM_CREATE()
	ON_COMMAND(ID_BTN_ADDCOL, OnBtnAddcol)
	ON_COMMAND(ID_BTN_DELCOL, OnBtnDelcol)
	ON_UPDATE_COMMAND_UI(ID_BTN_DELCOL, OnUpdateBtnDelcol)
	ON_WM_PAINT()
	ON_COMMAND(ID_BTN_PREVIEWGRID, OnBtnPreviewGrid)
	ON_COMMAND(ID_BTN_EDITCOL, OnBtnEditcol)
	ON_COMMAND(ID_BTN_MOVEUP, OnBtnMoveUp)
	ON_COMMAND(ID_BTN_MOVEDOWN, OnBtnMoveDown)
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridHeadDlg message handlers

int CGridHeadDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CAppWizStepDlg::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

BOOL CGridHeadDlg::OnInitDialog() 
{
	CAppWizStepDlg::OnInitDialog();

	CRect rectClient;
	GetClientRect(&rectClient);
	
	if (!m_wndToolBar.CreateEx(this, WS_CHILD | WS_VISIBLE | CBRS_TOP| CBRS_LEFT
		| CBRS_TOOLTIPS	| CBRS_BORDER_BOTTOM | CBRS_BORDER_TOP) ||
		!m_wndToolBar.LoadToolBar(IDR_GRIDHEAD))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;      // fail to create
	}
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	CRect rectFrame,rectParent,rect(0,0,100,100);
	GetClientRect(&rectParent);
	ClientToScreen(&rectParent);
	CWnd* pWnd = GetDlgItem(IDC_STATIC_FRAME);
	pWnd->GetWindowRect(&rectFrame);
	rect.left = rectFrame.left - rectParent.left;
	rect.top = rectFrame.top - rectParent.top;
	rect.right = rect.left + rectFrame.Width();
	rect.bottom = rect.top + rectFrame.Height();

	if (!m_wndHeadProp.Create(WS_EX_CLIENTEDGE,
							WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL| WS_TABSTOP, 
	             			rect,
							this,
                			0))
	{
		TRACE0("Failed to create grid control\n");
		return FALSE;
	}

	// We need to resize the dialog to make room for control bars.
	// First, figure out how big the control bars are.
	CRect rcClientNow;
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,
				   0, reposQuery, rcClientNow);

	// Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rectClient.left,
					rcClientNow.top - rectClient.top);

	CRect  rcChild;
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	// And position the control bars
	rectClient.top = 1;
	rectClient.left = rectClient.right - 112 - 50;
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,IDR_GRIDHEAD,CWnd::reposDefault,NULL, &rectClient,FALSE);

	TEXTMETRIC tm;
	CDC* pDC = GetDC();
	pDC->GetTextMetrics(&tm);

	m_wndHeadProp.SetHeadColWidth(0,0,4*tm.tmAveCharWidth);
	m_wndHeadProp.SetHeadColWidth(1,0,18*tm.tmAveCharWidth);
	m_wndHeadProp.SetHeadColWidth(2,0,9*tm.tmAveCharWidth);
	m_wndHeadProp.SetHeadColWidth(3,0,12*tm.tmAveCharWidth);
	m_wndHeadProp.SetHeadColWidth(4,0,14*tm.tmAveCharWidth);
	m_wndHeadProp.SetHeadColWidth(5,0,15*tm.tmAveCharWidth);
	m_wndHeadProp.SetHeadColWidth(6,0,15*tm.tmAveCharWidth);

	ReleaseDC(pDC);

	m_wndHeadProp.UpdateScrollSizes();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridHeadDlg::OnBtnAddcol() 
{
	m_wndHeadProp.SaveModifyed(TRUE);
	m_wndHeadProp.OnAddCol();
}

void CGridHeadDlg::OnBtnDelcol() 
{
	m_wndHeadProp.OnDelCol();
}

void CGridHeadDlg::OnUpdateBtnDelcol(CCmdUI* pCmdUI) 
{
	if(m_wndHeadProp.GetGridRowCount() > 1)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CGridHeadDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(rect);

	// Create a pattern brush from the bitmap.
	CBrush brush;
	CSize sizeGrid;
	BITMAP bmgrid;
	brush.CreatePatternBrush(&m_bmpFon);
	m_bmpGrid.GetBitmap(&bmgrid);
	sizeGrid.cx = bmgrid.bmWidth;
	sizeGrid.cy = bmgrid.bmHeight;

	// Select the brush into a device context, and draw.
	CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
	dc.FillRect(&CRect(0, 0, 180, rect.Height()),&brush); 	
	// Restore the original brush.
	dc.SelectObject(pOldBrush);


	CRect rect2(180, 0,180 + 2, rect.Height()+1);
	dc.Draw3dRect(rect2,::GetSysColor(COLOR_BTNSHADOW) , ::GetSysColor(COLOR_WINDOW));

	CPoint pt(16,16);
	dc.DrawState(pt,sizeGrid,m_bmpGrid,DSS_NORMAL);
}

void CGridHeadDlg::OnBtnPreviewGrid() 
{
	m_wndHeadProp.SaveModifyed(TRUE);
	PREV_COLROWS Col;
	CPrevDlg m_PrevDlg;

	for(int i = 0; i < 20; i++)
	{
		Col.Rows[0].m_dwTag = 0;
		Col.Rows[0].m_strText = "";
	}

	for(i = 0; i < m_wndHeadProp.m_Cols.GetSize(); i++)
	{
		UINT CellAlign, HeadAlign, DrawCtrl, StyleDrawCtrl;
		BYTE TypeCtrl;
		DWORD dwStyleCtrl;
		int IndCol;
		DrawCtrl = 0;
		StyleDrawCtrl = 0;
		switch(m_wndHeadProp.m_Cols[i].m_nCellAlign)
		{
		case 1:
			CellAlign = ACFF_RIGHT;
			break;
		case 2:
			CellAlign = ACFF_CENTER;
			break;
		default:
			CellAlign = ACFF_LEFT;
		}
		switch(m_wndHeadProp.m_Cols[i].m_nHeadAlign)
		{
		case 1:
			HeadAlign = AHFF_RIGHT;
			break;
		case 2:
			HeadAlign = AHFF_CENTER;
			break;
		default:
			HeadAlign = AHFF_LEFT;
		}
		switch(m_wndHeadProp.m_Cols[i].m_nTypeControl)
		{
		case 1:
			{
				TypeCtrl = GA_EDITCTRL;
				switch(m_wndHeadProp.m_Cols[i].m_nControlStyle)
				{
				case 1:
					dwStyleCtrl = WS_CHILD | ES_RIGHT;
						break;
				case 2:
					dwStyleCtrl = WS_CHILD | ES_CENTER;
					break;
				default:
					dwStyleCtrl = WS_CHILD | ES_LEFT;
				}
				break;
			}
		case 2:
			{
				TypeCtrl = GA_BUTTONCTRL;
				DrawCtrl = DFC_BUTTON;
				if(m_wndHeadProp.m_Cols[i].m_nControlStyle == 1)
				{
					dwStyleCtrl = WS_CHILD | BS_AUTO3STATE | BS_FLAT;
					StyleDrawCtrl = DFCS_BUTTON3STATE | DFCS_FLAT;
				}
				else
				{
					dwStyleCtrl = WS_CHILD | BS_AUTOCHECKBOX | BS_FLAT;
					StyleDrawCtrl = DFCS_BUTTONCHECK | DFCS_FLAT;
				}
				break;
			}
		case 3:
			TypeCtrl = GA_COMBOBOXCTRL;
				if(m_wndHeadProp.m_Cols[i].m_nControlStyle == 1)
					dwStyleCtrl = WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL;
				else
					dwStyleCtrl = WS_CHILD | CBS_DROPDOWN | WS_VSCROLL;
			break;
		default:
			TypeCtrl = GA_CELLCTRL;
		}

		switch(m_wndHeadProp.m_Cols[i].m_nTypeControl)
		{
		case 1:
			TypeCtrl = GA_EDITCTRL;
			break;
		case 2:
			TypeCtrl = GA_BUTTONCTRL;
			break;
		case 3:
			TypeCtrl = GA_COMBOBOXCTRL;
			break;
		default:
			TypeCtrl = GA_CELLCTRL;
		}

		IndCol = m_PrevDlg.m_wndGridPrev.AddCol(m_wndHeadProp.m_Cols[i].m_nWidth,m_wndHeadProp.m_Cols[i].m_strTitle,CellAlign,HeadAlign,DrawCtrl,StyleDrawCtrl);

		m_PrevDlg.m_wndGridPrev.DefineColCtrl(IndCol,TypeCtrl,dwStyleCtrl);

		m_PrevDlg.m_wndGridPrev.m_Cols.Add(Col);

	}

	if(m_PrevDlg.DoModal() == IDOK)
	{
		for(i = 0; i < m_wndHeadProp.m_Cols.GetSize(); i++)
			m_wndHeadProp.m_Cols[i].m_nWidth = m_PrevDlg.m_wndGridPrev.GetHeadColWidth(i);
		m_wndHeadProp.Invalidate();
	}
}

void CGridHeadDlg::OnBtnEditcol() 
{
	m_wndHeadProp.SaveModifyed(TRUE);
	m_wndHeadProp.OnEditCol();
}

void CGridHeadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CAppWizStepDlg::OnShowWindow(bShow, nStatus);
	if (!bShow)
		return;

	m_bmpGrid.DeleteObject(); 
	if(IsMacroDefine(_T("PROJTYPE_DLG")))
		m_bmpGrid.LoadBitmap(IDB_DLGGRID);
	else
		m_bmpGrid.LoadBitmap(IDB_GRID);

	CWnd* pParentWnd = GetParent();
	if(pParentWnd == NULL)
		return;

	int PropSheetButtons[] = { 0x1B9, IDOK, 0x1B8, IDCANCEL, ID_HELP };
	CWnd* pWndBtnHlp = pParentWnd->GetDlgItem(ID_HELP);
	CWnd* pWndBtnCnl = pParentWnd->GetDlgItem(IDCANCEL);

	if(pWndBtnHlp != NULL && pWndBtnCnl != NULL)
	{
		CRect rectHlp, rectBtn;
		pWndBtnHlp->GetWindowRect(rectHlp);
		pParentWnd->ScreenToClient(rectHlp); 
		pWndBtnHlp->ModifyStyle(WS_VISIBLE,WS_DISABLED);

		pWndBtnCnl->GetWindowRect(rectBtn);
		pParentWnd->ScreenToClient(rectBtn); 

		int dx = rectHlp.right - rectBtn.right;

		if(dx > 0)
		{
			pWndBtnCnl->MoveWindow(rectBtn.left + dx, rectBtn.top, rectBtn.Width(), rectBtn.Height(), FALSE);

			for(int i = 2; i >= 0; i--)
			{
				CWnd* pWndBtn = pParentWnd->GetDlgItem(PropSheetButtons[i]);
				if(pWndBtn != NULL)
				{
					pWndBtn->GetWindowRect(rectBtn);
					pParentWnd->ScreenToClient(rectBtn); 
					pWndBtn->MoveWindow(rectBtn.left + dx, rectBtn.top, rectBtn.Width(), rectBtn.Height(), FALSE);
				}
			}
		}
	}

	return;
}


void CGridHeadDlg::OnBtnMoveUp() 
{
	m_wndHeadProp.OnMoveColUp();
}

void CGridHeadDlg::OnBtnMoveDown() 
{
	m_wndHeadProp.OnMoveColDown();
}
