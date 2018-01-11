
// JPush.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "JPush.h"
#include "JPushDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

PushToneInfo::PushToneInfo()
{
	setToneType(TONE_DEFAULT);
	setToneName("default");
	setDisplay("Ĭ��");
}
PushToneInfo::PushToneInfo(PushToneType type, CString name, CString display){
	setToneType(type);
	setToneName(name);
	setDisplay(display);
}

PushToneInfo::~PushToneInfo()
{
}


void PushToneInfo::setToneType(PushToneType type){
	this->m_type = type;
}
void PushToneInfo::setToneName(CString name){
	this->m_name = name;
}
void PushToneInfo::setDisplay(CString display){
	this->m_sDisplay = display;
}

PushToneType PushToneInfo::getToneType(void){
	return this->m_type;
}
CString PushToneInfo::getToneName(void){
	return this->m_name;
}
CString PushToneInfo::getDisplay(void){
	return this->m_sDisplay;
}


// CJPushApp

BEGIN_MESSAGE_MAP(CJPushApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CJPushApp ����

CJPushApp::CJPushApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CJPushApp ����

CJPushApp theApp;


// CJPushApp ��ʼ��

BOOL CJPushApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CJPushDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


void CJPushApp::log(CString szInfo)
{
	CStdioFile file;

	do
	{
		if (!file.Open(LOG_FILE, CStdioFile::modeCreate | CStdioFile::modeReadWrite | CStdioFile::modeNoTruncate | CStdioFile::typeText))
		{
			break;
		}

		file.SeekToEnd();

		file.WriteString(szInfo);
		file.Flush();
		file.Close();
	} while (FALSE);
}

CStringW ConvertANSIToUTF16(__in const CHAR * pszTextANSI)
{
	if ((pszTextANSI == NULL) || (*pszTextANSI == '\0'))
	{
		return L"";
	}

	int cbANSI = strlen(pszTextANSI);

	int cchUTF16 = ::MultiByteToWideChar(
		CP_ACP,                 // convert from ANSI
		MB_ERR_INVALID_CHARS,   // error on invalid chars
		pszTextANSI,            // source UTF-8 string
		cbANSI,                 // total length of source UTF-8 string, // in CHAR's (= bytes), including end-of-string \0
		NULL,                   // unused - no conversion done in this step
		0                       // request size of destination buffer, in WCHAR's
		);

	CStringW strUTF16;
	WCHAR * pszUTF16 = strUTF16.GetBuffer(cchUTF16);

	int result = ::MultiByteToWideChar(
		CP_ACP,                // convert from ANSI
		MB_ERR_INVALID_CHARS,   // error on invalid chars
		pszTextANSI,            // source UTF-8 string
		cbANSI,                 // total length of source UTF-8 string, // in CHAR's (= bytes), including end-of-string \0
		pszUTF16,               // destination buffer
		cchUTF16                // size of destination buffer, in WCHAR's
		);

	strUTF16.ReleaseBuffer();

	return strUTF16;
}

CStringW ConvertUTF8ToUTF16(__in const CHAR * pszTextUTF8)
{
	if ((pszTextUTF8 == NULL) || (*pszTextUTF8 == '\0'))
	{
		return L"";
	}

	int cbUTF8 = strlen(pszTextUTF8);

	int cchUTF16 = ::MultiByteToWideChar(
		CP_UTF8,                // convert from UTF-8
		MB_ERR_INVALID_CHARS,   // error on invalid chars
		pszTextUTF8,            // source UTF-8 string
		cbUTF8,                 // total length of source UTF-8 string, // in CHAR's (= bytes), including end-of-string \0
		NULL,                   // unused - no conversion done in this step
		0                       // request size of destination buffer, in WCHAR's
		);

	CStringW strUTF16;
	WCHAR * pszUTF16 = strUTF16.GetBuffer(cchUTF16);

	int result = ::MultiByteToWideChar(
		CP_UTF8,                // convert from UTF-8
		MB_ERR_INVALID_CHARS,   // error on invalid chars
		pszTextUTF8,            // source UTF-8 string
		cbUTF8,                 // total length of source UTF-8 string, // in CHAR's (= bytes), including end-of-string \0
		pszUTF16,               // destination buffer
		cchUTF16                // size of destination buffer, in WCHAR's
		);

	strUTF16.ReleaseBuffer();

	return strUTF16;
}

CStringA ConvertUTF16ToUTF8(__in const WCHAR * pszTextUTF16)
{
	if ((pszTextUTF16 == NULL) || (*pszTextUTF16 == L'\0'))
	{
		return "";
	}

	int cchUTF16 = wcslen(pszTextUTF16);


	int cbUTF8 = ::WideCharToMultiByte(
		CP_UTF8,                // convert to UTF-8
		0,      // specify conversion behavior
		pszTextUTF16,           // source UTF-16 string
		cchUTF16,   // total source string length, in WCHAR's,      // including end-of-string \0
		NULL,                   // unused - no conversion required in this step
		0,                      // request buffer size
		NULL, NULL              // unused
		);

	CStringA strUTF8;
	CHAR * pszUTF8 = strUTF8.GetBuffer(cbUTF8);

	int result = ::WideCharToMultiByte(
		CP_UTF8,                // convert to UTF-8
		0,      // specify conversion behavior
		pszTextUTF16,           // source UTF-16 string
		cchUTF16,   // total source string length, in WCHAR's,  // including end-of-string \0
		pszUTF8,                // destination buffer
		cbUTF8,                 // destination buffer size, in bytes
		NULL, NULL              // unused
		);

	strUTF8.ReleaseBuffer();

	return strUTF8;
}

CStringA ConvertUTF16ToANSI(__in const WCHAR * pszTextUTF16)
{
	if ((pszTextUTF16 == NULL) || (*pszTextUTF16 == L'\0'))
	{
		return "";
	}

	int cchUTF16 = wcslen(pszTextUTF16);

	int cbANSI = ::WideCharToMultiByte(
		CP_ACP,                // convert to ANSI
		0,      // specify conversion behavior
		pszTextUTF16,           // source UTF-16 string
		cchUTF16,   // total source string length, in WCHAR's,      // including end-of-string \0
		NULL,                   // unused - no conversion required in this step
		0,                      // request buffer size
		NULL, NULL              // unused
		);

	CStringA strANSI;
	CHAR * pszANSI = strANSI.GetBuffer(cbANSI);

	int result = ::WideCharToMultiByte(
		CP_ACP,                // convert to ANSI
		0,      // specify conversion behavior
		pszTextUTF16,           // source UTF-16 string
		cchUTF16,   // total source string length, in WCHAR's,  // including end-of-string \0
		pszANSI,                // destination buffer
		cbANSI,                 // destination buffer size, in bytes
		NULL, NULL              // unused
		);

	strANSI.ReleaseBuffer();

	return strANSI;
}

CStringA ConvertUTF8ToANSI(__in const CHAR * pszTextUTF8)
{
	return ConvertUTF16ToANSI(ConvertUTF8ToUTF16(pszTextUTF8));
}

CStringA ConvertANSIToUTF8(__in const CHAR * pszTextANSI)
{
	return ConvertUTF16ToUTF8(ConvertANSIToUTF16(pszTextANSI));
}

PCHAR	WcharToChar(__out PCHAR outBuf, __in LPCWCH pWstr, __in size_t aBufSize)
{
	int nlength = wcslen(pWstr);

	int nbytes = WideCharToMultiByte(0, // specify the code page used to perform the conversion

		0,         // no special flags to handle unmapped characters

		pWstr,     // wide character string to convert

		nlength,   // the number of wide characters in that string

		NULL,      // no output buffer given, we just want to know how long it needs to be

		0,

		NULL,      // no replacement character given

		NULL);    // we don't want to know if a character didn't make it through the translation

	// make sure the buffer is big enough for this, making it larger if necessary

	if (nbytes > (int)aBufSize)
	{
		nbytes = aBufSize;
	}

	// ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�

	WideCharToMultiByte(0, // specify the code page used to perform the conversion

		0,         // no special flags to handle unmapped characters

		pWstr,   // wide character string to convert

		nlength,   // the number of wide characters in that string

		outBuf, // put the output ascii characters at the end of the buffer

		nbytes,  // there is at least this much space there

		NULL,      // no replacement character given

		NULL);

	return outBuf;
}

void		CharToWchar(__out LPWCH outBuf, size_t wBufSize, PCCH str)
{
	if (str)
	{

		size_t nu = strlen(str);

		size_t n = (size_t)MultiByteToWideChar(CP_ACP, 0, str, (int)nu, NULL, 0);

		if (n >= wBufSize)
		{
			n = wBufSize - 1;
		}

		MultiByteToWideChar(CP_ACP, 0, str, (int)nu, outBuf, (int)n);

		outBuf[n] = 0;
	}
}


int SplitString(const CString str, char split, CStringArray &strArray)  
{  
	strArray.RemoveAll();  
	CString strTemp = str;  
	int iIndex = 0;  
	while (1)  
	{  
		iIndex = strTemp.Find(split);  
		if(iIndex >= 0)  
		{  
			strArray.Add(strTemp.Left(iIndex));  
			strTemp = strTemp.Right(strTemp.GetLength()-iIndex-1);  
		}  
		else  
		{  
			break;  
		}  
	}  
	strArray.Add(strTemp);  

	return strArray.GetSize();  
}