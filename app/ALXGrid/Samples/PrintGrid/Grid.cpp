// Grid.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Grid.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "GridDoc.h"
#include "GridView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridApp

BEGIN_MESSAGE_MAP(CGridApp, CWinApp)
	//{{AFX_MSG_MAP(CGridApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridApp construction

CGridApp::CGridApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGridApp object

CGridApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGridApp initialization

BOOL CGridApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_GRIDTYPE,
		RUNTIME_CLASS(CGridDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGridView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


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
		// No message handlers
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

// App command to run the dialog
void CGridApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CGridApp::GetPrinterDeviceDefaults(PRINTDLG *pPrintDlg)
{
	if(m_hDevNames == NULL)
	{

		UpdatePrinterSelection(m_hDevNames == NULL); //force default if no current
		if (m_hDevNames == NULL)
			return FALSE;               // no printer defaults

		LPDEVMODE lpDevDevMode = (LPDEVMODE)::GlobalLock(m_hDevMode);
		if(lpDevDevMode != NULL)
		{
			lpDevDevMode->dmFields = lpDevDevMode->dmFields | DM_ORIENTATION;
			lpDevDevMode->dmOrientation = DMORIENT_LANDSCAPE;
		}

		pPrintDlg->hDevNames = m_hDevNames;
		pPrintDlg->hDevMode = m_hDevMode;

		::GlobalUnlock(m_hDevNames);
		::GlobalUnlock(m_hDevMode);
	}
	return TRUE;
}

int CGridApp::GetPageOrientation()
{
	if(m_hDevMode==NULL)
		return 0;
	LPDEVMODE lpDevDevMode = (LPDEVMODE)::GlobalLock(m_hDevMode);
	if(lpDevDevMode == NULL)
		return 0;
	int iOrientation = lpDevDevMode->dmOrientation;

	::GlobalUnlock(m_hDevNames);
	::GlobalUnlock(m_hDevMode);
	return iOrientation;
}

/////////////////////////////////////////////////////////////////////////////
// CGridApp message handlers
