// SetWidth.cpp : implementation file
//

#include "stdafx.h"
#include "Grid.h"
#include "SetWidthDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetWidthDlg dialog


CSetWidthDlg::CSetWidthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetWidthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetWidthDlg)
	m_bWidth = 0;
	//}}AFX_DATA_INIT
}


void CSetWidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetWidthDlg)
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_bWidth);
	DDV_MinMaxByte(pDX, m_bWidth, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetWidthDlg, CDialog)
	//{{AFX_MSG_MAP(CSetWidthDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetWidthDlg message handlers
