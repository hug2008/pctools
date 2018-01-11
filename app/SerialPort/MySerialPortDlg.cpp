// MySerialPortDlg.cpp : implementation file
// download by http://www.codefans.net

#include "stdafx.h"
#include "MySerialPort.h"
#include "MySerialPortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySerialPortDlg dialog

CMySerialPortDlg::CMySerialPortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySerialPortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMySerialPortDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMySerialPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMySerialPortDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMySerialPortDlg, CDialog)
	//{{AFX_MSG_MAP(CMySerialPortDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_TEST_BUTTON, OnTestButton)
	ON_BN_CLICKED(IDC_CLOSE_BUTTON, OnCloseButton)
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMM_MESSAGE,OnCommMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySerialPortDlg message handlers

BOOL CMySerialPortDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMySerialPortDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMySerialPortDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMySerialPortDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMySerialPortDlg::ShowMessage(CString strMessage)
{
	CString str;
	GetDlgItemText(IDC_EDIT1,str);
	str=strMessage+"\r\n"+str;
	SetDlgItemText(IDC_EDIT1,str);
}

void CMySerialPortDlg::OnTestButton() 
{
	BOOL bRet=serial.Open(GetSafeHwnd(),WM_COMM_MESSAGE,1,9600,8,0,1);

	if(bRet)
	{
		ShowMessage("成功打开串口！");
	}
	else
	{
		ShowMessage("打开串口失败！");
	}
}
LRESULT CMySerialPortDlg::OnCommMessage(WPARAM wParam,LPARAM lParam)
{
	int len=1024;
	static char szData[1024];
	switch(wParam)
	{
	case WM_COMM_READ:
		{
			if(serial.ReadData(szData,len))
			{
				ShowMessage(CString(szData));
			}
		}
		break;
	case WM_COMM_WRITE:
		{
			char *p=(char*)lParam;
			ShowMessage(CString(p));
		}
		break;
	default:
		{
		}
		break;
	}
	return 0L;
}


void CMySerialPortDlg::OnCloseButton() 
{
	BOOL bRet=serial.Close();

	if(bRet)
	{
		ShowMessage("成功关闭串口！");
	}
	else
	{
		ShowMessage("关闭串口失败！");
	}
}

void CMySerialPortDlg::OnSendButton() 
{
	int len=1024;
	char szData[1024]={0};

	len=GetDlgItemText(IDC_EDIT2,szData,len);
	serial.SendData(szData,len);	
}
