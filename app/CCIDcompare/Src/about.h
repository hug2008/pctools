#include "stdafx.h"
#include "CCIDcompare.h"
#include "CCIDcompareDlg.h"
#include ".\ccidcomparedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

#define  STR_APP_NAME   _T("CCIDcompare Version ")
#define  STR_BUILD_TIME _T("Build On: ")

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	CString m_szVer;
	CString m_szBuildTime;

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};