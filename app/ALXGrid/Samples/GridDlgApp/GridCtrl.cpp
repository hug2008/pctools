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
	DefineColCtrl(AddCol(115, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_DEBIT), GA_EDITCTRL, WS_CHILD | ES_RIGHT);
	strTmp.LoadString(ID_COL_CREDIT);
	DefineColCtrl(AddCol(115, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_CREDIT), GA_EDITCTRL, WS_CHILD | ES_RIGHT);
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

	m_MetricsCellsSum.FillDefault();
	m_MetricsCellsSum.m_FocusRect.m_crFaceColor = RGB(0,0,0);
	m_MetricsCellsSum.m_DefaultCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_BTNFACE);
	m_MetricsCellsSum.m_ActiveCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_BTNFACE);
	m_MetricsCellsSum.m_Borders.m_crLeftBrdColor		= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSum.m_Borders.m_crRightBrdColor		= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSum.m_Borders.m_crTopBrdColor			= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSum.m_Borders.m_crBottomBrdColor		= ::GetSysColor(COLOR_3DSHADOW);
	m_MetricsCellsSum.pDefaultFont = &m_SumFont;
	m_MetricsCellsSum.pActiveFont = &m_SumFont;

	for(int i = 0; i < 20; i++)
	{
		RECORD_INFO rc_Info;
		rc_Info.m_Dedet.SetCurrency(i*20,0); 
		rc_Info.m_Credit.SetCurrency(i*10,0); 
		m_RowInfo.Add(rc_Info);
	}

	SetGridRowCount(m_RowInfo.GetSize()+1);
}

CGridCtrl::~CGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CGridCtrl,CALXGridCtrl)
	//{{AFX_MSG_MAP(CGridCtrl)
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
	
	if(nRow == GetGridRowCount()-1)
	{
		switch(GetCellCtrlID(nCol,nRow))
		{
		case ID_COL_DEBIT:
			{
				COleCurrency SumDeb(0,0);
				for(int i = 0; i < m_RowInfo.GetSize(); i++)
					SumDeb += m_RowInfo[i].m_Dedet;
				CellData.m_strText = SumDeb.Format();
				break;
			}
		case ID_COL_CREDIT:
			{
				COleCurrency SumCrd(0,0);
				for(int i = 0; i < m_RowInfo.GetSize(); i++)
					SumCrd += m_RowInfo[i].m_Credit;
				CellData.m_strText = SumCrd.Format();
				break;
			}
		case ID_COL_SALDO:
			{
				COleCurrency SumSld(0,0);
				for(int i = 0; i < m_RowInfo.GetSize(); i++)
					SumSld += m_RowInfo[i].m_Dedet - m_RowInfo[i].m_Credit;
				CellData.m_strText = SumSld.Format();
				break;
			}
		}
		return CellData;
	}

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
		InvalidateRow(m_RowInfo.GetSize());
	}
	return TRUE;
}

CALXCellMetrics* CGridCtrl::GetCellMetrics(int nCol, int nRow)
{
	if(nRow == GetGridRowCount()-1)
		return &m_MetricsCellsSum;

	switch(GetCellCtrlID(nCol,nRow))
	{
	case ID_COL_DEBIT:
		{
			if(nRow == GetGridRowCount()-2)
				m_MetricsCellsDebet.m_Borders.m_crBottomBrdColor = ::GetSysColor(COLOR_3DSHADOW);
			else
				m_MetricsCellsDebet.m_Borders.m_crBottomBrdColor = ::GetSysColor(COLOR_BTNFACE);;

			return &m_MetricsCellsDebet;
		}
	case ID_COL_CREDIT:
		{
			if(nRow == GetGridRowCount()-2)
				m_MetricsCellsCredit.m_Borders.m_crBottomBrdColor = ::GetSysColor(COLOR_3DSHADOW);
			else
				m_MetricsCellsCredit.m_Borders.m_crBottomBrdColor = ::GetSysColor(COLOR_BTNFACE);;

			return &m_MetricsCellsCredit;
		}
	case ID_COL_SALDO:
		return &m_MetricsCellsSaldo;
	}
	return &m_MetricsCellsSaldo;
}

CALXCellCtrl* CGridCtrl::GetCellCtrl(int nCol, int nRow)
{
	if(nRow == GetGridRowCount()-1)
		return NULL;

	return CALXGrid::GetCellCtrl(nCol, nRow);
}

