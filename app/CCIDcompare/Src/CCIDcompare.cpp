// CCIDcompare.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CCIDcompare.h"
#include "CCIDcompareDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCCIDcompareApp

BEGIN_MESSAGE_MAP(CCCIDcompareApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCCIDcompareApp 构造

CCCIDcompareApp::CCCIDcompareApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCCIDcompareApp 对象

CCCIDcompareApp theApp;

void CCCIDcompareApp::LoadSkin()
{
	//加载动态库
	m_hDllSkin = LoadLibrary("SkinLoad.dll");
	if(m_hDllSkin != NULL)
	{
		typedef	int		(* INITSKIN)(DWORD dwThreadId);
		//typedef	bool	(* LOADSKIN)(CString szPath);
		INITSKIN pInitSkin = (INITSKIN)GetProcAddress(m_hDllSkin,"InitSkinLoad");
		//LOADSKIN pLoadSkin = (LOADSKIN)GetProcAddress(m_hDllSkin,"LoadSkin");
		if(pInitSkin != NULL)
		{
			(*pInitSkin)(GetCurrentThreadId());
			//(*pLoadSkin)("xp_normal.skin");
		}
	}
	return;
}
// CCCIDcompareApp 初始化

BOOL CCCIDcompareApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//加载皮肤文件
	//LoadSkin();
	

	// 此程序只能运行一个实例子，用互斥量来判断程序是否已运行
    HANDLE m_hMutex = CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS){ 
		MessageBox(NULL, "程序已运行!", "错误", MB_OK|MB_ICONERROR);
		return FALSE;
	}

	SetRegistryKey(_T("DESAYELECCIDCOMPARE"));

	M_STRPORT = GetProfileString("Settings", "port", _T("COM1"));

	M_STRRATE = GetProfileString("Settings", "rate", _T("115200"));

	M_STRBUYER = GetProfileString("Settings", "Buyer", _T("NC1234"));

	M_STRCARDTYPE = GetProfileString("Settings", "Cardtype", _T("FALSE"));

	M_STRCHECTYPE = GetProfileString("Settings", "Checktype", _T("FALSE"));

	M_STRDBSRC    = GetProfileString("Settings", "DbSrc", _T("D:\\\\*.mdb"));

	CCCIDcompareDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。
	return FALSE;
}

int CCCIDcompareApp::ExitInstance()
{
	CloseComm();
	//退出皮肤动态库
	if(m_hDllSkin != NULL)
	{
		typedef bool (* EXITSKIN)();
		EXITSKIN pExitSkin = (EXITSKIN)GetProcAddress(m_hDllSkin,"ExitSkin");
		if(pExitSkin != NULL)
		{
			(*pExitSkin)();
		}
		FreeLibrary(m_hDllSkin);
	}

	WriteProfileString("Settings", "port", M_STRPORT);

	WriteProfileString("Settings", "rate", M_STRRATE);

	WriteProfileString("Settings", "Buyer", M_STRBUYER);

	WriteProfileString("Settings", "Cardtype", M_STRCARDTYPE);

	WriteProfileString("Settings", "Checktype", M_STRCHECTYPE);

	WriteProfileString("Settings", "DbSrc",  M_STRDBSRC);
	
	return CWinApp::ExitInstance();
}