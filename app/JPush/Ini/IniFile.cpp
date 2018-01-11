
#include "stdafx.h"

#include <AfxColl.h>





#include "IniFile.h"

CIniFile::CIniFile(void)
{
	TCHAR name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);

	m_SizeBufBytes = 16385;
}

CIniFile::CIniFile(CString m_FName)
{
	CString szCheck = m_FName;
	
	TCHAR name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);
	
	if (-1 != szCheck.Find(_T(":\\")))
		m_Name = m_FName;
	else
		m_Name = m_Path + m_FName;

	Create(m_Name);
    
	m_SizeBufBytes = 16385;
}

void CIniFile::SetFilename(CString szNewfilename)
{
	TCHAR name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);

	if (-1 == szNewfilename.Find(_T(":\\"))){
		m_Name = m_Path + szNewfilename;
	}else
		m_Name = szNewfilename;

	Create(m_Name);
}

CString CIniFile::GetFilename(void)
{
	return m_Name;
}

unsigned int CIniFile::GetSizeBytes(void)
{
	return m_SizeBufBytes;
}

void CIniFile::SetSizeBytes(unsigned int nBytes)
{
	if (nBytes<100) {return;}
	m_SizeBufBytes = nBytes;
}

BOOL CIniFile::Create(CString szFileName)
{
	FILE *fp = NULL;

	TCHAR name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);

	if (-1 == szFileName.Find(_T(":\\")))
	{
		m_Name = m_Path + szFileName;
	}
	else
	{
		m_Name = szFileName;
	}

	DWORD fAttr = GetFileAttributes(m_Name);
	if (INVALID_FILE_ATTRIBUTES != fAttr)
	{
		return TRUE;
	}

	CStdioFile file;

	if (!file.Open(m_Name, CStdioFile::modeCreate | CStdioFile::modeReadWrite | CStdioFile::modeNoTruncate | CStdioFile::typeText))
	{
		return FALSE;
	}

	time_t now;
	struct tm l_time = { 0 };

	now = time((time_t *)NULL);

	localtime_s(&l_time, &now);

	file.SeekToEnd();
	file.Flush();
	file.Close();


	CString szInfo = _T("");
	szInfo.Format(_T("%04d%02d%02d-%02d:%02d:%02d"), 
		l_time.tm_year + 1900, 
		l_time.tm_mon+1,
		l_time.tm_mday,
		l_time.tm_hour,
		l_time.tm_min,
		l_time.tm_sec);

	WriteString(_T("Version"), _T("Time"), szInfo);
	WriteString(_T("Version"), _T("Path"), m_Name);

	m_SizeBufBytes = 16385;
	
	return TRUE;
}



CString CIniFile::ReadString(CString m_Sec, CString m_Ident, CString m_Def)
{
	TCHAR Buffer[2048];
	
	GetPrivateProfileString(m_Sec, m_Ident, m_Def, Buffer, sizeof(Buffer), m_Name);
	
	return Buffer;
}



BOOL CIniFile::WriteString(CString m_Sec, CString m_Ident, CString m_Val)
{
	return WritePrivateProfileString(m_Sec, m_Ident, m_Val, m_Name);
}



BOOL CIniFile::ReadSections(CStringArray& m_Secs)
{
	
	LPVOID pvData = NULL;
	
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_SizeBufBytes);
	
	_ASSERTE(NULL != hGlobal);	
	
	pvData = GlobalLock(hGlobal);
	
	_ASSERTE(NULL != pvData);	
	
	m_Secs.RemoveAll();	
	
	if (GetPrivateProfileString(NULL, NULL, NULL, (TCHAR*) pvData, m_SizeBufBytes-1, m_Name))		
	{		
		TCHAR *P = (TCHAR*)pvData;
		
        while (*P != 0)			
		{
			
			m_Secs.Add(P);
			
			P += lstrlen(P) + 1;
		}		
	}
	
	GlobalUnlock(hGlobal);
	
	GlobalFree(hGlobal);
	
	return m_Secs.GetSize() > 0;
	
}



BOOL CIniFile::ReadSection(CString m_Sec, CStringArray& m_Secs)
{
	
	LPVOID pvData = NULL;
	
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_SizeBufBytes);
	
	_ASSERTE(NULL != hGlobal);	
	
	pvData = GlobalLock(hGlobal);
	
	_ASSERTE(NULL != pvData);	
	
	m_Secs.RemoveAll();	
	
	if (GetPrivateProfileString(m_Sec, NULL, NULL, (TCHAR*) pvData, m_SizeBufBytes-1, m_Name))		
	{		
		TCHAR *P = (TCHAR*)pvData;
        while (*P != 0)			
		{			
			m_Secs.Add(P);			
			P += lstrlen(P) + 1;			
		}		
	}	
	GlobalUnlock(hGlobal);
	
	GlobalFree(hGlobal);
	
	return m_Secs.GetSize() > 0;	
}

UINT CIniFile::ReadInt(CString szSec, CString szIdent, UINT iDef)
{
	return GetPrivateProfileInt(szSec, szIdent, iDef, m_Name);
}

BOOL CIniFile::WriteInt(CString szSec, CString szIdent, UINT iVal)
{
	CString szVal = _T("");
	szVal.Format(_T("%d"), iVal);
	return WriteString(szSec, szIdent, szVal);
}

BOOL CIniFile::DeleteSection(LPCTSTR lpszSectionName)  
{  
	return ::WritePrivateProfileSection(lpszSectionName, NULL, m_Name);  
}  

BOOL CIniFile::DeleteKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName)  
{  
	return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, NULL, m_Name);  
} 

