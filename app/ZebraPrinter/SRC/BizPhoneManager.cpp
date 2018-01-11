// BizPhoneManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "BizPhoneSettingDlg.h"
//#include <Shlwapi.h>
#include <io.h>
#include "SplashWnd.h"
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
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBizPhoneManagerApp object

CBizPhoneManagerApp theApp;

BOOL    bThereIsThreadRunning = FALSE;
/////////////////////////////////////////////////////////////////////////////
// CBizPhoneManagerApp initialization

BOOL CBizPhoneManagerApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CString   szError     = _T(""),
		      m_Path      = _T("");
	char      szPath[512] = _T("");

	BOOL      bIniExist   = FALSE;

	CFileFind filder;

	HANDLE m_hMutex = CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError() == ERROR_ALREADY_EXISTS) {
		szError.Format(_T("程序[%s]已运行！"), m_pszAppName);
		MessageBox(NULL, szError, "提示", MB_OK|MB_ICONERROR);
		exit(0);
	}

	//---------------取得数据库的绝对路径名---------------------
	GetModuleFileName(NULL, szPath, 512);	
	m_Path = szPath;
	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);
	//m_Path = m_Path + DBNAME;
	//MessageBox(NULL, m_dbPath, "Path", MB_OK);
	//----------------------------------------------------------


	if (!m_ado.ConnectAccess(m_Path + DBNAME, _T("DESAYELE"))){
		szError.Format(_T("连接数据库[%s]失败，程序无法运行！"), DBNAME);
		MessageBox(NULL,szError,"错误", MB_OK|MB_ICONERROR);
		exit(0);
	}

	if (!m_ado.IsOpen()){
		m_ado.Close();
		szError.Format(_T("打开数据库[%s]失败，程序无法运行！"), DBNAME);
		MessageBox(NULL,DBNAME,"错误", MB_OK|MB_ICONERROR);
		exit(0);
	}
		
	// Enable the splash screen component.
	//#ifndef USE_SERIAL_CODE
	CSplashWnd::EnableSplashScreen(TRUE);
	//#else
	//CSplashWnd::EnableSplashScreen(FALSE);
	//#endif
    

	// 检查配置文件"ZaberCnf.ini"是否在当前目录下
	bIniExist = filder.FindFile(m_Path + ININAME, TRUE);
	if(!bIniExist/*(_access(ININAME, 0x06) != 0)*/){
		m_Ini.Create(ININAME);
		m_szLpt      = _T("LPT1");
		m_szShare    = _T("01");
		m_bLockPage  = FALSE;
		m_nMmiIndex  = 0;
		m_Ini.WriteString(SEC_MAINSETTING, _T("Port"),     m_szLpt);
		m_Ini.WriteString(SEC_MAINSETTING, _T("Share"),    m_szShare);
		m_Ini.WriteString(SEC_MAINSETTING, _T("LockPage"), _T("00"));
		m_Ini.WriteString(SEC_MAINSETTING, _T("Mmi"),      _T("00"));
		//MessageBox(NULL, "No exist", _T("1111"), MB_OK);
	}else{
		m_Ini.SetFilename(ININAME);
		m_szLpt      =      m_Ini.ReadString(SEC_MAINSETTING, _T("Port"),     _T("LPT1"));
		m_szShare    =      m_Ini.ReadString(SEC_MAINSETTING, _T("Share"),    _T("01"));
		m_bLockPage  = atoi(m_Ini.ReadString(SEC_MAINSETTING, _T("LockPage"), _T("00")));
		m_nMmiIndex  = atoi(m_Ini.ReadString(SEC_MAINSETTING, _T("Mmi"),      _T("00")));
	}

#ifdef SEC_ZPL_LOG
	m_fLog = fopen(SEC_ZPL_LOG, "a+t");
#endif//SEC_ZPL_LOG
	
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
	m_Ini.WriteString(SEC_MAINSETTING, _T("Port"),     m_szLpt);

	m_Ini.WriteString(SEC_MAINSETTING, _T("Share"),    m_szShare);

	/* m_szShare */
	m_szShare.Format("%02d", m_bLockPage);
	m_Ini.WriteString(SEC_MAINSETTING, _T("LockPage"), m_szShare);
    /* m_nMmi */
	m_szShare.Format("%02d", m_nMmiIndex);
	m_Ini.WriteString(SEC_MAINSETTING, _T("Mmi"), m_szShare);

#ifdef SEC_ZPL_LOG
	if (m_fLog){
		fclose(m_fLog);
	}
#endif//SEC_ZPL_LOG

	return CWinApp::ExitInstance();
}