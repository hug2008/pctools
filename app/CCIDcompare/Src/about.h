#include "stdafx.h"
#include "CCIDcompare.h"
#include "CCIDcompareDlg.h"
#include ".\ccidcomparedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

#define  STR_APP_NAME   _T("CCIDcompare Version ")
#define  STR_BUILD_TIME _T("Build On: ")

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	CString m_szVer;
	CString m_szBuildTime;

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};