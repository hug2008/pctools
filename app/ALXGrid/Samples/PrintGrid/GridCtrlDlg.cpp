// GridCtrlDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCtrlDlg dialog

CGridCtrlDlg::CGridCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridCtrlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CGridCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridCtrlDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGridCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CGridCtrlDlg)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridCtrlDlg message handlers

BOOL CGridCtrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// вычисляем координаты окна по координатам окна рамки
	CRect rectFrame;		// координаты окна рамки
	CRect rectParent;		// координаты родительского окна эл. управления
	CRect rect(0,0,100,100);// координаты эл. управления m_GridCtrl
	GetClientRect(&rectParent);
	ClientToScreen(&rectParent);
	CWnd* pWnd = GetDlgItem(IDC_STATIC_FRAME);
	pWnd->GetWindowRect(&rectFrame);
	rect.left = rectFrame.left - rectParent.left;
	rect.top = rectFrame.top - rectParent.top;
	rect.right = rect.left + rectFrame.Width();
	rect.bottom = rect.top + rectFrame.Height();

	if (!m_GridCtrl.Create(WS_EX_CLIENTEDGE,
							WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL| WS_TABSTOP, 
	             			rect,
							this,
                			0))
	{
		TRACE0("Failed to create list box\n");
		return FALSE;
	}
	

	// устанавливаем Tab Order
	m_GridCtrl.SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOREDRAW); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridCtrlDlg::OnBtnPrint() 
{
	m_GridCtrl.SaveModifyed(TRUE);

	m_GridCtrl.Print();
}


