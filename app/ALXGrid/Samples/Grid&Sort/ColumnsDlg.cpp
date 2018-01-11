// ColumnsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Grid.h"
#include "ColumnsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CColumnsDlg dialog


CColumnsDlg::CColumnsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColumnsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumnsDlg)
	m_nWidth = 0;
	//}}AFX_DATA_INIT
}
CColumnsDlg::~CColumnsDlg()
{
}


void CColumnsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnsDlg)
	DDX_Control(pDX, IDC_BTN_HIDE, m_ctrlHide);
	DDX_Control(pDX, IDC_BTN_SHOW, m_ctrlShow);
	DDX_Control(pDX, IDC_BTN_DOWN, m_ctrlDown);
	DDX_Control(pDX, IDC_BTN_UP, m_ctrlUP);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 0, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnsDlg, CDialog)
	//{{AFX_MSG_MAP(CColumnsDlg)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnChangeWidth)
	ON_COMMAND(ID_VIEW_COLUMNS, OnSetColumn)
	ON_COMMAND(ID_COL_VISIBL, OnSetColVisbl)
	ON_BN_CLICKED(IDC_BTN_SHOW, OnBtnShow)
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	ON_BN_CLICKED(IDC_BTN_SETDEF, OnBtnSetDef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumnsDlg message handlers

BOOL CColumnsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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
							WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP | AGS_ROWSELECT, 
	             			rect,
							this,
                			0))
	{
		TRACE0("Failed to create grid control\n");
		return FALSE;
	}

	m_wndCtrl.SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOREDRAW); 

	m_wndCtrl.m_pwndWidth = GetDlgItem(IDC_EDIT_WIDTH);

	m_wndCtrl.SetGridRowCount(GRIDCOLCOUNT);

	m_wndCtrl.UpdateScrollSizes();

	CRect rc;
	m_wndCtrl.GetClientRect(rc);
	m_wndCtrl.SetHeadColWidth(1,0,rc.Width()-m_wndCtrl.GetHeadColWidth(0));

	m_wndCtrl.UpdateScrollSizes();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColumnsDlg::OnChangeWidth() 
{
	UpdateData();
	int nRow = m_wndCtrl.GetActiveRow();
	m_wndCtrl.m_aFieldsColInfo[nRow].m_nWidth = m_nWidth;
}

void CColumnsDlg::OnSetColumn() 
{
	int nActRow = m_wndCtrl.GetActiveRow();

	if(nActRow == 0)
		m_ctrlUP.EnableWindow(FALSE);
	else
		m_ctrlUP.EnableWindow(TRUE);

	if(nActRow == m_wndCtrl.GetGridRowCount() - 1)
		m_ctrlDown.EnableWindow(FALSE);
	else
		m_ctrlDown.EnableWindow(TRUE);

	if(m_wndCtrl.m_aFieldsColInfo[nActRow].m_bVisibl)
	{
		m_ctrlHide.EnableWindow(TRUE);
		m_ctrlShow.EnableWindow(FALSE);
	}
	else
	{
		m_ctrlHide.EnableWindow(FALSE);
		m_ctrlShow.EnableWindow(TRUE);
	}
}

void CColumnsDlg::OnSetColVisbl() 
{
	if(m_wndCtrl.m_aFieldsColInfo[m_wndCtrl.GetActiveRow()].m_bVisibl)
	{
		m_ctrlHide.EnableWindow(TRUE);
		m_ctrlShow.EnableWindow(FALSE);
	}
	else
	{
		m_ctrlHide.EnableWindow(FALSE);
		m_ctrlShow.EnableWindow(TRUE);
	}
}


void CColumnsDlg::OnBtnShow() 
{
	m_wndCtrl.SetColVisibl(TRUE);
	m_ctrlHide.EnableWindow(TRUE);
	m_ctrlShow.EnableWindow(FALSE);

}

void CColumnsDlg::OnBtnHide() 
{
	m_wndCtrl.SetColVisibl(FALSE);
	m_ctrlHide.EnableWindow(FALSE);
	m_ctrlShow.EnableWindow(TRUE);
}

void CColumnsDlg::OnBtnUp() 
{
	int nActRow = m_wndCtrl.GetActiveRow();
	m_wndCtrl.SaveModifyed(FALSE);

	FLDCOL_INFO fldColInfo;
	
	fldColInfo = m_wndCtrl.m_aFieldsColInfo[nActRow];
	m_wndCtrl.m_aFieldsColInfo[nActRow] = m_wndCtrl.m_aFieldsColInfo[nActRow - 1];
	m_wndCtrl.m_aFieldsColInfo[nActRow - 1] = fldColInfo;

	m_wndCtrl.SetActiveRow(nActRow - 1);
}

void CColumnsDlg::OnBtnDown() 
{
	int nActRow = m_wndCtrl.GetActiveRow();
	m_wndCtrl.SaveModifyed(FALSE);

	FLDCOL_INFO fldColInfo;
	
	fldColInfo = m_wndCtrl.m_aFieldsColInfo[nActRow];
	m_wndCtrl.m_aFieldsColInfo[nActRow] = m_wndCtrl.m_aFieldsColInfo[nActRow + 1];
	m_wndCtrl.m_aFieldsColInfo[nActRow + 1] = fldColInfo;

	m_wndCtrl.SetActiveRow(nActRow + 1);
}

void CColumnsDlg::OnBtnSetDef() 
{
	for(int i = 0; i < GRIDCOLCOUNT; i++)
	{
		m_wndCtrl.m_aFieldsColInfo[i].m_bVisibl = TRUE;
		m_wndCtrl.m_aFieldsColInfo[i].m_iField = i;
		m_wndCtrl.m_aFieldsColInfo[i].m_iCol = i;
		m_wndCtrl.m_aFieldsColInfo[i].m_nWidth = -1;
	}

	m_wndCtrl.m_aFieldsColInfo[0].m_nWidth = 70;
	m_wndCtrl.m_aFieldsColInfo[1].m_nWidth = 200;
	m_wndCtrl.m_aFieldsColInfo[2].m_nWidth = 100;
	m_wndCtrl.m_aFieldsColInfo[3].m_nWidth = 100;
	m_wndCtrl.m_aFieldsColInfo[4].m_nWidth = 100;

	CWnd* pwndWidth = GetDlgItem(IDC_EDIT_WIDTH);
	if(pwndWidth != NULL)
	{
		CString strWidth;
		strWidth.Format("%u",m_wndCtrl.m_aFieldsColInfo[m_wndCtrl.GetActiveRow()].m_nWidth);
		pwndWidth->SetWindowText(strWidth);
	}

	m_wndCtrl.SetColVisibl();
	m_wndCtrl.Invalidate();

}

void CColumnsDlg::OnOK() 
{
	int i = 0;
	while(i < GRIDCOLCOUNT && !m_wndCtrl.m_aFieldsColInfo[i].m_bVisibl)
		i++;
	if(i == GRIDCOLCOUNT)
		AfxMessageBox("More than one column should be reflect. Before that to continue, select one of listed columns.\n");
	else
		CDialog::OnOK();
}


