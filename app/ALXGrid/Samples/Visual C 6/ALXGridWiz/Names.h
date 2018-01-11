// Names.h : implementation file
// CG: This file was added by the AppWizard Tools component

#ifndef __NAMES_H__
#define __NAMES_H__

class CNames : public CObject	// Derive from CObject so we get the overloaded new
{
public:
	CNames() { m_bTrackHFile = m_bTrackIFile = TRUE; }

	CString m_strClass;
	CString m_strBaseClass;
	CString m_strHFile;
	CString m_strIFile;
	CString m_strMacro;
	CString m_strDefFile;

	// These remember if we should continue updating the fields
	BOOL m_bTrackHFile;
	BOOL m_bTrackIFile;

	void Init(LPCTSTR szMacroName, LPCTSTR szName,
		LPCTSTR szBaseClass, BOOL bPrefixWithRoot = TRUE,
		LPCTSTR szDefFile = "");
	void UpdateFiles();
	void DefineMacros();
};

typedef CArray< CNames, CNames& > CNamesArray;

template<> void AFXAPI ConstructElements<CNames>(CNames* pElements, int nCount);

#endif //__NAMES_H__
