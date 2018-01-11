// CCIDcompare.cpp : ����Ӧ�ó��������Ϊ��
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


// CCCIDcompareApp ����

CCCIDcompareApp::CCCIDcompareApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCCIDcompareApp ����

CCCIDcompareApp theApp;

void CCCIDcompareApp::LoadSkin()
{
	//���ض�̬��
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
// CCCIDcompareApp ��ʼ��

BOOL CCCIDcompareApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//����Ƥ���ļ�
	//LoadSkin();
	

	// �˳���ֻ������һ��ʵ���ӣ��û��������жϳ����Ƿ�������
    HANDLE m_hMutex = CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS){ 
		MessageBox(NULL, "����������!", "����", MB_OK|MB_ICONERROR);
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
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CCCIDcompareApp::ExitInstance()
{
	CloseComm();
	//�˳�Ƥ����̬��
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