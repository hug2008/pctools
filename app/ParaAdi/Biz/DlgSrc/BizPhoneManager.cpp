// BizPhoneManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "BizPhoneSettingDlg.h"
#include "ParaDefine.H"
#include "SelComPort.h"
#include "MainContent.h"
#include "SplashWnd.h"
#include "UserVerify.h"
#if defined(FOR_CLIENT)
#include "SelComPortClient.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneManagerApp

BEGIN_MESSAGE_MAP(CBizPhoneManagerApp, CWinApp)
	//{{AFX_MSG_MAP(CBizPhoneManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneManagerApp construction

CBizPhoneManagerApp::CBizPhoneManagerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bEnablePage  = FALSE;
	m_bIsAdmin     = FALSE;
	m_bUseMode     = UM_SAVE;
	m_szIniInput   = _T("");
#if defined(PAGE_CFG)
	m_pageEnable[0].nPageId = IDD_PROPPAGE_EMERGENCY_CALL;
	m_pageEnable[1].nPageId = IDD_PROPPAGE_OPN_CFG;
	m_pageEnable[2].nPageId = IDD_PROPPAGE_5_LOCAL_MOBILESECT;
#endif
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBizPhoneManagerApp object

CBizPhoneManagerApp theApp;
extern CMainContent MainContent;

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneManagerApp initialization

BOOL CBizPhoneManagerApp::InitInstance()
{
	AfxEnableControlContainer();

	CString szPageEnable = _T("");
	char     szPath[MAX_PATH] = "";

	SetRegistryKey(_T("ParaDownload"));
	m_bEnPassw = GetProfileString("Settings", "EnPassw");
	m_szPassw = GetProfileString("Settings", "Passw");
	if(m_bEnPassw.IsEmpty())
	{
		m_bEnPassw = _T("FALSE");
	}

	m_szCurUser = GetProfileString("Settings", "CurUser", _T("Admin"));

	m_szPort    = GetProfileString("Settings", "port",    _T("COM1"));
	m_szBaut    = GetProfileString("Settings", "baut",    _T("115200"));
	m_bUseMode  = atoi(GetProfileString("Settings", "mode",    _T("00")));
	
#if defined(PAGE_CFG)
	szPageEnable = GetProfileString("Settings", STR_PAGECFG_ENABLE, _T(""));

	setPageEnableStatusString(szPageEnable);

	/*
	m_pageEnable[0].nPageId = IDD_PROPPAGE_EMERGENCY_CALL;
	m_pageEnable[1].nPageId = IDD_PROPPAGE_OPN_CFG;
	m_pageEnable[2].nPageId = IDD_PROPPAGE_5_LOCAL_MOBILESECT;
	*/
	m_pageEnable[0].nCount  = atoi(GetProfileString("Settings", STR_PAGECFG_COUNTEC, _T("00")));
	m_pageEnable[1].nCount  = atoi(GetProfileString("Settings", STR_PAGECFG_COUNTOC, _T("00")));
	m_pageEnable[2].nCount  = atoi(GetProfileString("Settings", STR_PAGECFG_COUNTLM, _T("400")));
#endif
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	OSVERSIONINFO os;

	// Get the Version of this Windows
	os.dwOSVersionInfoSize = sizeof(os);
	GetVersionEx(&os);
	
	if(os.dwMajorVersion < 4)
	{
		MessageBox(NULL,"this application will only run on Windows95 or WinNT 4.0 or greater","Error",MB_OK|MB_ICONINFORMATION);
		return 1;
	}
	
	if(os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		MainContent.bIfIsWinNt = FALSE;
	else if(os.dwPlatformId == VER_PLATFORM_WIN32_NT)
		MainContent.bIfIsWinNt = TRUE;
	else 
	{
		MessageBox(NULL,"there is an Error,the system is not supported","Error",MB_OK|MB_ICONINFORMATION);
		return 1;
	}

	// Start the com port select dlg
#if defined(FOR_CLIENT)
	CSelComPortClient m_selDlg;
	m_selDlg.m_ComPort     = m_szPort;
	m_selDlg.m_strBaut     = m_szBaut;
	if (IDOK == m_selDlg.DoModal())
	{
		MainContent.m_strBaudRate = m_selDlg.m_strBaut ;
		MainContent.m_SelComPort  = m_selDlg.m_ComPort;
		m_szPort                  = m_selDlg.m_ComPort;
		m_szBaut                  = m_selDlg.m_strBaut;

		m_bIsAdmin                = TRUE;
		m_bEnablePage             = TRUE;
	}
#else
	CSelComPort m_SelComPortDlg;
	m_SelComPortDlg.m_ComPort     = m_szPort;
	m_SelComPortDlg.m_strBaut     = m_szBaut;
	m_SelComPortDlg.m_szEnterUser = m_szCurUser;
	m_SelComPortDlg.m_cbCurMode   = m_bUseMode;
	if(m_SelComPortDlg.DoModal() == IDOK)
	{
		if(m_SelComPortDlg.m_ComPort.IsEmpty())
		{	
			MessageBox(NULL,"You must select a ComPort!","Error",MB_OK);
			return 1;
		}
		if((m_SelComPortDlg.m_ComPort.Find("COM")!=0)||(m_SelComPortDlg.m_ComPort.GetLength()<=3))
		{
			MessageBox(NULL,"The ComPort is incorrect!","Error",MB_OK);
			return 1;
		}
		if(m_SelComPortDlg.m_strBaut.IsEmpty())
		{
			MessageBox(NULL,"You must select a Baut!","Error",MB_OK);
			return 1;
		}

		// 用户名校验
		{
			char name[256]           = _T("");
			char szUser[NAME_SIZE]   = "";
			char szPassW[PASSW_SIZE] = "";
			CString path             = _T("");
			CString szSuperPassw     = _T("");
			CString szUserUp         = _T("");
			GetModuleFileName(NULL, name, 256);
			path = name;
			path = path.Mid(0, path.ReverseFind('\\')+1);
			path = path + VERIFY_FILE;
			szSuperPassw.LoadString(IDS_STRING_ADMIN_PASSW);

			strncpy(szUser,  m_SelComPortDlg.m_szEnterUser,  NAME_SIZE);
			strncpy(szPassW, m_SelComPortDlg.m_szEnterPassW, PASSW_SIZE);

			szUserUp = szUser; szUserUp.MakeUpper();

			if (0 == strcmp(szUserUp, _T("ADMIN")))
			{
				if (0 == strcmp(szPassW, szSuperPassw))
				{
					m_szCurUser   = szUser;
					m_bIsAdmin    = TRUE;
					m_bEnablePage = TRUE;
					m_bUseMode    = m_SelComPortDlg.m_cbCurMode;
				}
				else
				{
					if (strlen(szPassW) > 0)
						MessageBox(NULL, _T("password is not matching!"), _T("Err"), MB_OK|MB_ICONSTOP);
					else
						MessageBox(NULL, _T("Please input password!"), _T("Err"), MB_OK|MB_ICONSTOP);
					return 1;
				}
			}
			else
			{
				// 从userId.vf中查找这个用户
				if (VE_OK != isAvailableUserName(szUser, path)){
					path.Format("User [%s] is not in existence!", szUser);
					MessageBox(NULL, path, _T("Err"), MB_OK|MB_ICONSTOP);
					return 1;
			    }

				if (VE_OK != isAvailableNode(szUser, szPassW, path)){
					if (strlen(szPassW) > 0)
						MessageBox(NULL, _T("password is not matching!"), _T("Err"), MB_OK|MB_ICONSTOP);
					else
						MessageBox(NULL, _T("Please input password!"), _T("Err"), MB_OK|MB_ICONSTOP);
					return 1;
			    }

				m_szCurUser         = szUser;
				m_bIsAdmin          = FALSE;
				m_bEnablePage       = FALSE;
			}

		}        
		MainContent.m_strBaudRate = m_SelComPortDlg.m_strBaut ;
		MainContent.m_SelComPort  = m_SelComPortDlg.m_ComPort;
		m_szPort                  = m_SelComPortDlg.m_ComPort;
		m_szBaut                  = m_SelComPortDlg.m_strBaut;
		m_bUseMode                = m_SelComPortDlg.m_cbCurMode;
	}
#endif
	else 
	{
		return TRUE;
	}
#if defined(FOR_CLIENT)
	m_selDlg.DestroyWindow();
#else
	m_SelComPortDlg.DestroyWindow();
#endif


	GetModuleFileName(NULL, szPath, MAX_PATH);

	m_szAppPath   = szPath;
	m_szAppPath   = theApp.m_szAppPath.Mid(0, theApp.m_szAppPath.ReverseFind('\\')+1);
	m_szAppPath   = m_szAppPath + m_szPort + _T("-") + m_szBaut + _T("-");

	// Enable the splash screen component.
//#ifndef USE_SERIAL_CODE
	CSplashWnd::EnableSplashScreen(TRUE);
//#else
	//CSplashWnd::EnableSplashScreen(FALSE);
//#endif
	
	// Start the main dlg
	CBizPhoneSettingDlg dlg;
	m_pMainWnd = &dlg;

	//Associate a 'tag' with the window so we can locate it later
	// ::SetProp(m_pMainWnd->GetSafeHwnd(),m_pszExeName,(HANDLE)1);
	
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

// this function enable the SplashWnd response to user action such as mouse click
BOOL CBizPhoneManagerApp::PreTranslateMessage(MSG* pMsg) 
{
	// Route messages to the splash screen while it is visible
    if (CSplashWnd::PreTranslateAppMessage(pMsg)) {
        return TRUE;
    }
	
	return CWinApp::PreTranslateMessage(pMsg);
}
int CBizPhoneManagerApp::ExitInstance()
{
	CString szVal = _T("");
	int     index = 0;

	WriteProfileString("Settings", "EnPassw", m_bEnPassw);

	WriteProfileString("Settings", "Passw", m_szPassw);

	WriteProfileString("Settings", "CurUser", m_szCurUser);

	WriteProfileString("Settings", "port", m_szPort);

	WriteProfileString("Settings", "baut", m_szBaut);

	szVal.Format("%02d", m_bUseMode);
	WriteProfileString("Settings", "mode", szVal);
	
#if defined(PAGE_CFG)
	// save pageEnableStatus
	getPageEnableStatusString(szVal);
	WriteProfileString("Settings", STR_PAGECFG_ENABLE, szVal);

	/*
	m_pageEnable[0].nPageId = IDD_PROPPAGE_EMERGENCY_CALL;
	m_pageEnable[1].nPageId = IDD_PROPPAGE_OPN_CFG;
	m_pageEnable[2].nPageId = IDD_PROPPAGE_5_LOCAL_MOBILESECT;
	*/
	szVal.Format(_T("%02d"), m_pageEnable[0].nCount);
	WriteProfileString("Settings", STR_PAGECFG_COUNTEC, szVal);
	szVal.Format(_T("%02d"), m_pageEnable[1].nCount);
	WriteProfileString("Settings", STR_PAGECFG_COUNTOC, szVal);
	szVal.Format(_T("%02d"), m_pageEnable[2].nCount);
	WriteProfileString("Settings", STR_PAGECFG_COUNTLM, szVal);
#endif

	return CWinApp::ExitInstance();
}
#if defined(PAGE_CFG)
BOOL getPageEnableStatus(unsigned int  nPageId)
{
	int i = 0;

	for (i=0; i<PAGEENABLE_TOTAL; i++)
	{
		if (nPageId == theApp.m_pageEnable[i].nPageId)
			return theApp.m_pageEnable[i].bEnable;
	}

	// 默认使用
	return TRUE;
}

void setPageEnableStatus(unsigned int  nPageId, BOOL bEnable)
{
	int i = 0;
	for (i=0; i<PAGEENABLE_TOTAL; i++)
	{
		if (nPageId == theApp.m_pageEnable[i].nPageId)
		{
			theApp.m_pageEnable[i].bEnable = bEnable;
			break;
		}
	}
	return;
}

void getPageEnableStatusString(CString &szEnableStatusString)
{
	int index = 0;
	szEnableStatusString.Empty();
	for (index=0; index<PAGEENABLE_TOTAL; index++)
	{
		if (theApp.m_pageEnable[index].bEnable)
			szEnableStatusString += _T("1");
		else
			szEnableStatusString += _T("0");
	}
}

void setPageEnableStatusString(CString szEnableStatusString)
{
	int index = 0;
	if (szEnableStatusString.IsEmpty())
	{// 默认为可用
		szEnableStatusString.Format("%s", CString('1', PAGEENABLE_TOTAL));
	}
	else
	{
		if (PAGEENABLE_TOTAL > szEnableStatusString.GetLength())
		{
			szEnableStatusString.Format("%s%s", szEnableStatusString, CString('1', PAGEENABLE_TOTAL-szEnableStatusString.GetLength()));
		}
		else
		{
			szEnableStatusString = szEnableStatusString.Left(PAGEENABLE_TOTAL);
		}
	}

	ASSERT(PAGEENABLE_TOTAL == szEnableStatusString.GetLength());

	for (index=0; index<PAGEENABLE_TOTAL; index++)
	{
		if ('0' == szEnableStatusString.GetAt(index))
			theApp.m_pageEnable[index].bEnable  = FALSE;
		else
			theApp.m_pageEnable[index].bEnable  = TRUE;
	}
}

unsigned int getPageCount(unsigned int  nPageId)
{
    int i = 0;

	for (i=0; i<PAGEENABLE_TOTAL; i++)
	{
		if (nPageId == theApp.m_pageEnable[i].nPageId)
			return theApp.m_pageEnable[i].nCount;
	}
	return 0;
}

void setPageCount(unsigned int  nPageId, unsigned int  nCount)
{
    int i = 0;
	for (i=0; i<PAGEENABLE_TOTAL; i++)
	{
		if (nPageId == theApp.m_pageEnable[i].nPageId)
		{
			theApp.m_pageEnable[i].nCount = nCount;
			break;
		}
	}
	return;
}
#endif