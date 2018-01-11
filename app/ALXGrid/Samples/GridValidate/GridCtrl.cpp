// GridCtrl.cpp : implementation of the CGridCtrl class
//

#include "stdafx.h"
#include "resource.h"
#include "GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl

CGridCtrl::CGridCtrl()
{
	CString strTmp;
	strTmp.LoadString(ID_COL_DEBIT);
	DefineColCtrl(AddCol(115, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_DEBIT), GA_CELLCTRL);
	strTmp.LoadString(ID_COL_CREDIT);
	DefineColCtrl(AddCol(115, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_CREDIT), GA_CELLCTRL);
	strTmp.LoadString(ID_COL_SALDO);
	DefineColCtrl(AddCol(120, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_SALDO), GA_CELLCTRL);

	// структура определ€юща€ шрифт, используемый сеткой
	LOGFONT	 logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfHeight = 12;
	logfont.lfWeight = FW_BOLD;
	lstrcpy(logfont.lfFaceName, _T("MS Sans Serif"));
	logfont.lfCharSet = RUSSIAN_CHARSET;
	logfont.lfOutPrecision = OUT_TT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = PROOF_QUALITY;
	logfont.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
	logfont.lfItalic = FALSE;

	if(!m_SumFont.CreateFontIndirect(&logfont))
	{
		TRACE0("Failed to create grid font\n");
		return;
	}

	m_MetricsCellsDebet.FillDefault();
	m_MetricsCellsDebet.m_FocusRect.m_bSolid = FALSE;
	m_MetricsCellsDebet.m_FocusRect.m_crBackColor = RGB(255,255,255);
	m_MetricsCellsDebet.m_FocusRect.m_crFaceColor = RGB(0,0,0);

	m_MetricsCellsCredit.FillDefault();
	m_MetricsCellsCredit.m_FocusRect.m_bSolid = FALSE;
	m_MetricsCellsCredit.m_FocusRect.m_crBackColor = RGB(255,255,255);
	m_MetricsCellsCredit.m_FocusRect.m_crFaceColor = RGB(0,0,0);
	m_MetricsCellsCredit.m_Borders.m_crRightBrdColor= ::GetSysColor(COLOR_3DSHADOW);

	m_MetricsCellsSaldo.FillDefault();
	m_MetricsCellsSaldo.m_FocusRect.m_crFaceColor = ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSaldo.m_DefaultCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_3DLIGHT);
	m_MetricsCellsSaldo.m_ActiveCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_BTNFACE);
	m_MetricsCellsSaldo.m_Borders.m_crLeftBrdColor		= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSaldo.m_Borders.m_crRightBrdColor		= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSaldo.m_Borders.m_crTopBrdColor		= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSaldo.m_Borders.m_crBottomBrdColor	= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSaldo.pActiveFont = &m_SumFont;

	for(int i = 0; i < 20; i++)
	{
		RECORD_INFO rc_Info;
		rc_Info.m_Dedet.SetCurrency((i+1)*20,0); 
		rc_Info.m_Credit.SetCurrency(i*10+10,0); 
		m_RowInfo.Add(rc_Info);
	}

	SetGridRowCount(m_RowInfo.GetSize());
}

CGridCtrl::~CGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CGridCtrl,CALXGridCtrl)
	//{{AFX_MSG_MAP(CGridCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridCtrl message handlers

BOOL CGridCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	return CALXGridCtrl::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CGridView data exchange

CELL_DATA CGridCtrl::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);
	
	switch(GetCellCtrlID(nCol,nRow))
	{
	case ID_COL_DEBIT:
		CellData.m_strText = m_RowInfo[nRow].m_Dedet.Format();
		break;
	case ID_COL_CREDIT:
		CellData.m_strText = m_RowInfo[nRow].m_Credit.Format();
		break;
	case ID_COL_SALDO:
		CellData.m_strText = (m_RowInfo[nRow].m_Dedet - m_RowInfo[nRow].m_Credit).Format();
		break;
	}
	return CellData;
}

BOOL CGridCtrl::OnSaveCellData(int nCol, int nRow)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA Data = pCellCtrl->GetCellData();
		switch(GetCellCtrlID(nCol,nRow))
		{
		case ID_COL_DEBIT:
			m_RowInfo[nRow].m_Dedet.ParseCurrency(Data.m_strText);
			break;
		case ID_COL_CREDIT:
			m_RowInfo[nRow].m_Credit.ParseCurrency(Data.m_strText);
			break;
		}
		InvalidateCell(2,nRow);
	}
	return TRUE;
}

CALXCellMetrics* CGridCtrl::GetCellMetrics(int nCol, int nRow)
{
	switch(GetCellCtrlID(nCol,nRow))
	{
	case ID_COL_DEBIT:
		return &m_MetricsCellsDebet;
	case ID_COL_CREDIT:
		return &m_MetricsCellsCredit;
	case ID_COL_SALDO:
		return &m_MetricsCellsSaldo;
	}
	return &m_MetricsCellsSaldo;
}

CALXCellCtrl* CGridCtrl::GetCellCtrl(int nCol, int nRow)
{
	switch(GetCellCtrlID(nCol,nRow))
	{
	case ID_COL_DEBIT:
		return &m_EditCtrl;
	case ID_COL_CREDIT:
		return &m_EditCtrl;
	}
	return NULL;
}

int CGridCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CALXGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_EditCtrl.CreateCtrl(WS_CHILD | ES_RIGHT | ES_AUTOHSCROLL,this,ID_COL_DEBIT);
	m_EditCtrl.SetFontCtrl(GetGridFont());

	m_EditCtrl.SetData(GetCellData(GetActiveCol(),GetActiveRow()));
	
	return 0;
}

void CGridCtrl::OnDestroy() 
{
	CWnd::OnDestroy();	
}

BOOL CGridCtrl::PreTranslateMessage(MSG* pMsg) 
{	
	if(pMsg->message==WM_KEYDOWN)
	{	
		if(LOWORD(pMsg->wParam) == VK_TAB && GetKeyState(VK_CONTROL) < 0)
			return CWnd::PreTranslateMessage(pMsg);
	}
	return CALXGridCtrl::PreTranslateMessage(pMsg);
}


