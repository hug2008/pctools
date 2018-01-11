// PrevDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ALXGridwz.h"
#include "PrevDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrevDlg dialog


CPrevDlg::CPrevDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrevDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrevDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPrevDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrevDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrevDlg, CDialog)
	//{{AFX_MSG_MAP(CPrevDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrevDlg message handlers

BOOL CPrevDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rectFrame,rectParent,rect(0,0,100,100);
	GetClientRect(&rectParent);
	ClientToScreen(&rectParent);
	CWnd* pWnd = GetDlgItem(IDC_STATIC_FRAME);
	pWnd->GetWindowRect(&rectFrame);
	rect.left = rectFrame.left - rectParent.left;
	rect.top = rectFrame.top - rectParent.top;
	rect.right = rect.left + rectFrame.Width();
	rect.bottom = rect.top + rectFrame.Height();

	if (!m_wndGridPrev.Create(WS_EX_CLIENTEDGE,
							WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL| WS_TABSTOP, 
	             			rect,
							this,
                			0))
	{
		TRACE0("Failed to create grid control\n");
		return FALSE;
	}
	
	m_wndGridPrev.UpdateScrollSizes();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


