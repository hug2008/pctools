// CCIDcompare.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������
#include "Src\\Comm.h"
#include "Barcode\\barcode.h"

// CCCIDcompareApp:
// �йش����ʵ�֣������ CCIDcompare.cpp
//

class CCCIDcompareApp : public CWinApp
{
public:
	CCCIDcompareApp();
private:
	void LoadSkin();
	HINSTANCE m_hDllSkin;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CString   M_STRBUYER;

	CString   M_STRCARDTYPE;

	CString   M_STRCHECTYPE;

	CString   M_STRPORT;
	CString   M_STRRATE;

	CString   M_STRDBSRC;

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCCIDcompareApp theApp;
