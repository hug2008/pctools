// ThreeItemPacking.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ThreeItemPacking.h"
#include "ThreeItemPackingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CThreeItemPackingApp

BEGIN_MESSAGE_MAP(CThreeItemPackingApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CThreeItemPackingApp 构造

CThreeItemPackingApp::CThreeItemPackingApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CThreeItemPackingApp 对象

CThreeItemPackingApp theApp;


// CThreeItemPackingApp 初始化

BOOL CThreeItemPackingApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	HANDLE m_hMutex = CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, "程序已运行！", "提示", MB_OK|MB_ICONERROR);
		return FALSE; 
	}

	CFileFind file;
	if (FALSE == file.FindFile("PackConfig.ini", 0)){
		MessageBox(NULL,"找不到配置文件PackConfig.ini，程序无法运行！\r\n请检查文件PackConfig.ini是否在当前目录下。", 
			"错误", MB_OK|MB_ICONERROR);
		exit(0);
		return FALSE; 
	}

	if (FALSE == file.FindFile("LablePacking.mdb", 0)){
		MessageBox(NULL,"找不到数据库文件PackConfig.ini，程序无法运行！\r\n请检查文件LablePacking.mdb是否在当前目录下。", 
			"错误", MB_OK|MB_ICONERROR);
		exit(0);
		return FALSE; 
	}



	CThreeItemPackingDlg dlg;
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

int CThreeItemPackingApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}