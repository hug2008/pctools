// ThreeItemPacking.cpp : ����Ӧ�ó��������Ϊ��
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


// CThreeItemPackingApp ����

CThreeItemPackingApp::CThreeItemPackingApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CThreeItemPackingApp ����

CThreeItemPackingApp theApp;


// CThreeItemPackingApp ��ʼ��

BOOL CThreeItemPackingApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	HANDLE m_hMutex = CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS) {
		MessageBox(NULL, "���������У�", "��ʾ", MB_OK|MB_ICONERROR);
		return FALSE; 
	}

	CFileFind file;
	if (FALSE == file.FindFile("PackConfig.ini", 0)){
		MessageBox(NULL,"�Ҳ��������ļ�PackConfig.ini�������޷����У�\r\n�����ļ�PackConfig.ini�Ƿ��ڵ�ǰĿ¼�¡�", 
			"����", MB_OK|MB_ICONERROR);
		exit(0);
		return FALSE; 
	}

	if (FALSE == file.FindFile("LablePacking.mdb", 0)){
		MessageBox(NULL,"�Ҳ������ݿ��ļ�PackConfig.ini�������޷����У�\r\n�����ļ�LablePacking.mdb�Ƿ��ڵ�ǰĿ¼�¡�", 
			"����", MB_OK|MB_ICONERROR);
		exit(0);
		return FALSE; 
	}



	CThreeItemPackingDlg dlg;
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

int CThreeItemPackingApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}