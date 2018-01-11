// CCIDcompare.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号
#include "Src\\Comm.h"
#include "Barcode\\barcode.h"

// CCCIDcompareApp:
// 有关此类的实现，请参阅 CCIDcompare.cpp
//

class CCCIDcompareApp : public CWinApp
{
public:
	CCCIDcompareApp();
private:
	void LoadSkin();
	HINSTANCE m_hDllSkin;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CString   M_STRBUYER;

	CString   M_STRCARDTYPE;

	CString   M_STRCHECTYPE;

	CString   M_STRPORT;
	CString   M_STRRATE;

	CString   M_STRDBSRC;

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCCIDcompareApp theApp;
