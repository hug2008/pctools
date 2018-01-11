// SetCountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Grid.h"
#include "SetCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCountDlg dialog


CSetCountDlg::CSetCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetCountDlg)
	m_bCount = 0;
	//}}AFX_DATA_INIT
}


void CSetCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCountDlg)
	DDX_Text(pDX, IDC_EDIT_COUNT, m_bCount);
	DDV_MinMaxByte(pDX, m_bCount, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCountDlg, CDialog)
	//{{AFX_MSG_MAP(CSetCountDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCountDlg message handlers
