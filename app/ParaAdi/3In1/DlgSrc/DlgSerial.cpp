// DlgSerial.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "DlgSerial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSerial dialog


CDlgSerial::CDlgSerial(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSerial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSerial)
	m_strInputedSerial = _T("");
	m_strCustomID = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSerial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSerial)
	DDX_Text(pDX, IDC_SERIAL, m_strInputedSerial);
	DDX_Text(pDX, IDC_CUSTOM, m_strCustomID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSerial, CDialog)
	//{{AFX_MSG_MAP(CDlgSerial)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSerial message handlers
