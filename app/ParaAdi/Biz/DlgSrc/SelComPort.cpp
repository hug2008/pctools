// SelComPort.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "SelComPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelComPort dialog


CSelComPort::CSelComPort(CWnd* pParent /*=NULL*/)
	: CDialog(CSelComPort::IDD, pParent)
	, m_szEnterUser(_T(""))
	, m_szEnterPassW(_T(""))
	, m_cbCurMode(0)
{
	//{{AFX_DATA_INIT(CSelComPort)
	m_ComPort = _T("COM1");
	//}}AFX_DATA_INIT
#if defined (_XIAMEN_START_BUILD)
	m_strBaut = _T("57600");
#elif defined (_TONGZE_PARA_BUILD)
	m_strBaut = _T("115200");
#elif defined CARD_ENCRYPT
	m_strBaut = _T("115200");
#else
	m_strBaut = _T("19200");
#endif // _XIAMEN_START_BUILD
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CSelComPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelComPort)
	DDX_Control(pDX, IDC_COMPORT, m_cbPort);
	DDX_Control(pDX, IDC_COMBATU, m_cbBaut);
	DDX_CBString(pDX, IDC_COMBATU, m_strBaut);
	DDX_CBString(pDX, IDC_COMPORT, m_ComPort);
	DDV_MaxChars(pDX, m_ComPort, 15);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_ENTER_USER, m_szEnterUser);
	DDX_Text(pDX, IDC_EDIT_ENTER_PASSW, m_szEnterPassW);
	DDX_Control(pDX, IDC_COMBATU_MODE, m_cbMode);
	DDX_CBIndex(pDX, IDC_COMBATU_MODE, m_cbCurMode);
}


BEGIN_MESSAGE_MAP(CSelComPort, CDialog)
	//{{AFX_MSG_MAP(CSelComPort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelComPort message handlers

BOOL CSelComPort::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_btOK    .SetToolTipText(_T("Select and enter"));
	m_btCancel.SetToolTipText(_T("Exit System"));

	m_cbMode.AddString(_T("save"));
	m_cbMode.AddString(_T("load"));

	m_cbMode.SetCurSel(m_cbCurMode);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
