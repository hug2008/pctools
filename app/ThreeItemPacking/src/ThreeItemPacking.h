// ThreeItemPacking.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CThreeItemPackingApp:
// �йش����ʵ�֣������ ThreeItemPacking.cpp
//

class CThreeItemPackingApp : public CWinApp
{
public:
	CThreeItemPackingApp();

// ��д
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CThreeItemPackingApp theApp;
