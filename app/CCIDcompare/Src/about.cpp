#include "stdafx.h"
#include "CCIDcompare.h"
#include "CCIDcompareDlg.h"
#include ".\ccidcomparedlg.h"
#include "about.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_szVer(_T(""))
, m_szBuildTime(_T(""))
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_AB_VER, m_szVer);
	DDX_Text(pDX, IDC_STATIC_AB_BUILD, m_szBuildTime);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCCIDcompareDlg 对话框