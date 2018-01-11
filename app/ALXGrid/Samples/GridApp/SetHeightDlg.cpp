// SetHeight.cpp : implementation file
//

#include "stdafx.h"
#include "Grid.h"
#include "SetHeightDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetHeightDlg dialog


CSetHeightDlg::CSetHeightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetHeightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetHeightDlg)
	m_bHeight = 0;
	//}}AFX_DATA_INIT
}


void CSetHeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetHeightDlg)
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_bHeight);
	DDV_MinMaxByte(pDX, m_bHeight, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetHeightDlg, CDialog)
	//{{AFX_MSG_MAP(CSetHeightDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetHeightDlg message handlers
