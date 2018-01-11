
// JPush.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "def.h"
// CJPushApp: 
// 有关此类的实现，请参阅 JPush.cpp
//

class PushToneInfo
{
public:
	PushToneInfo();
	PushToneInfo(PushToneType type, CString name, CString display);
	~PushToneInfo();


public:
	void setToneType(PushToneType type);
	void setToneName(CString name);
	void setDisplay(CString display);

	PushToneType getToneType(void);
	CString getToneName(void);
	CString getDisplay(void);

private:
	PushToneType	m_type;
	CString		m_name;
	CString		m_sDisplay;
};

class CJPushApp : public CWinApp
{
public:
	CJPushApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现
	void log(CString szInfo);

	DECLARE_MESSAGE_MAP()
};

extern CJPushApp theApp;

extern CStringA ConvertUTF8ToANSI(__in const CHAR * pszTextUTF8);
extern CStringA ConvertANSIToUTF8(__in const CHAR * pszTextANSI);
extern CStringW ConvertANSIToUTF16(__in const CHAR * pszTextANSI);
extern CStringA ConvertUTF16ToANSI(__in const WCHAR * pszTextUTF16);
extern CStringA ConvertUTF16ToUTF8(__in const WCHAR * pszTextUTF16);
extern CStringW ConvertUTF8ToUTF16(__in const CHAR * pszTextUTF8);
extern PCHAR	WcharToChar(__out PCHAR outBuf, __in LPCWCH pWstr, __in size_t aBufSize);
extern void		CharToWchar(__out LPWCH outBuf, __in size_t wBufSize, __in PCCH str);

extern int SplitString(const CString str, char split, CStringArray &strArray);