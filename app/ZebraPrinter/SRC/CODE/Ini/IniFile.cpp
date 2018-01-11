
#include "stdafx.h"

#include <AfxColl.h>





#include "IniFile.H"

CIniFile::CIniFile(void)
{
	char name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);

	m_SizeBufBytes = 16385;
}

CIniFile::CIniFile(CString m_FName)
{
	CString szCheck = m_FName;
	
	char name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);
	
	if (-1 != szCheck.Find(":\\"))
		m_Name = m_FName;
	else
		m_Name = m_Path + m_FName;
    
	m_SizeBufBytes = 16385;
}

void CIniFile::SetFilename(CString szNewfilename)
{
	char name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);

	if (-1 == szNewfilename.Find(":\\")){
		m_Name = m_Path + szNewfilename;
	}else
		m_Name = szNewfilename;
#ifdef _READ_WRITE_INI_ONLY_
	CString szCheck = m_Name;
	szCheck.MakeUpper();
	if (szCheck.Right(4) != _T(".INI")){
		m_Name = m_Name + _T(".ini");
	}
#endif//_READ_WRITE_INI_ONLY_
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

	char name[512];

	GetModuleFileName(NULL, name, 512);
	
	m_Path = name;

	m_Path = m_Path.Mid(0, m_Path.ReverseFind('\\')+1);

	if (-1 == szFileName.Find(":\\")){
		m_Name = m_Path + szFileName;
	}else
		m_Name = szFileName;
#ifdef _READ_WRITE_INI_ONLY_
	CString szCheck = m_Name;
	if (szFileName.IsEmpty())
		return FALSE;
	szCheck.MakeUpper();
	if (szCheck.Right(4) != _T(".INI")){
		m_Name += _T(".ini");
	}
#endif//_READ_WRITE_INI_ONLY_
	fp = fopen(m_Name, "w");
	if (NULL == fp)
	{
		return FALSE;
	}
	else
	{
		fprintf(fp, "[Version]\r");
		time_t now; 
        now = time((time_t *)NULL);         
		struct tm *l_time;        
		l_time = localtime(&now); 
		fprintf(fp, "CreateTime=%d%d%d%d%d", 
			l_time->tm_year+1900,(l_time->tm_mon+1)/10,
			(l_time->tm_mon+1)%10,(l_time->tm_mday)/10, (l_time->tm_mday)%10);
		fprintf(fp, "%d%d%d%d%d%d",
			l_time->tm_hour/10,l_time->tm_hour%10,
			l_time->tm_min/10, l_time->tm_min%10,
			l_time->tm_sec/10, l_time->tm_sec%10);
		fprintf(fp, "\rPathName=%s", m_Name); 
		fclose(fp);		
	}

	m_SizeBufBytes = 16385;
	
	return TRUE;
}



CString CIniFile::ReadString(CString m_Sec, CString m_Ident, CString m_Def)
{
	char Buffer[2048];
	
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
	
	if (GetPrivateProfileString(NULL, NULL, NULL, (char*) pvData, m_SizeBufBytes-1, m_Name))		
	{		
        char *P = (char*) pvData;
		
        while (*P != 0)			
		{
			
			m_Secs.Add(P);
			
			P += strlen(P) + 1;			
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
	
	if (GetPrivateProfileString(m_Sec, NULL, NULL, (char*) pvData, m_SizeBufBytes-1, m_Name))		
	{		
        char *P = (char*) pvData;		
        while (*P != 0)			
		{			
			m_Secs.Add(P);			
			P += strlen(P) + 1;			
		}		
	}	
	GlobalUnlock(hGlobal);
	
	GlobalFree(hGlobal);
	
	return m_Secs.GetSize() > 0;	
}



