#ifndef __CINIFILE_H__

#define __CINIFILE_H__



class CIniFile
{
public:
	
	CIniFile(void);
	
	CIniFile(CString m_FName);
	
	BOOL Create(CString szFileName);
	
	void SetFilename(CString szNewfilename);
	
	CString GetFilename(void);

	unsigned int GetSizeBytes(void);

	void  SetSizeBytes(unsigned int nBytes = 16385);
	
	CString ReadString(CString m_Sec, CString m_Ident, CString m_Def);
	
	BOOL WriteString(CString m_Sec, CString m_Ident, CString m_Val);
	
	BOOL ReadSections(CStringArray& m_Secs);
	
	BOOL ReadSection(CString m_Sec, CStringArray& m_Secs);
	
private:
	CString m_Path;
	CString m_Name;
	unsigned int m_SizeBufBytes;
};



#endif