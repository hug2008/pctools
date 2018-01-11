// Names.cpp : implementation file
// CG: This file was added by the AppWizard Tools component

#include "stdafx.h"
#include "WizUtil.h"
#include "Names.h"
#include "ALXGridAw.h"


void AFXAPI ConstructElements(CNames* pElements, int nCount)
{
	for (int i = 0; i < nCount; i++, pElements++)
	{
		// call CNames default constructor directly
		new( pElements )CNames;
	}
}


////////////////////////////////////////////////////////////////////////////////
// CNames

void CNames::Init(LPCTSTR szMacroName, LPCTSTR szName, 
	LPCTSTR szBaseClass, BOOL bPrefixWithRoot /* = TRUE */, LPCTSTR szDefFile/* = ""*/)
{
	if (bPrefixWithRoot)
	{
		CString strRoot;

		// Base class name off of root
		VERIFY(ALXAppWz.m_Dictionary.Lookup(_T("safe_root"), strRoot));
		if (strRoot.IsEmpty())
			strRoot = _T("My");

		// Capitalize first letter
		strRoot.SetAt(0, toupper(strRoot[0]));

		m_strClass.Format(_T("C%s%s"), (LPCTSTR) strRoot, szName);
	}
	else
	{
		// Don't prefix with root; just use szName
		m_strClass.Format(_T("C%s"), szName);
	}

	m_strDefFile = szDefFile;

	// Do files
	UpdateFiles();

	// Remember other values for later
	m_strMacro = szMacroName;
	m_strBaseClass = szBaseClass;
}

void CNames::UpdateFiles()
{
	if (!m_bTrackIFile && !m_bTrackHFile)
		return;

	// Get filename from classname, minus leading 'C'
	CString strFile;
	strFile = (!m_strClass.IsEmpty() && m_strClass[0] == _T('C')) ? m_strClass.Mid(1) : m_strClass;

	if(!m_strDefFile.IsEmpty())
		strFile = m_strDefFile;


	// Update HFile if we should
	if (m_bTrackHFile)
		m_strHFile = strFile + _T(".h");

	// Update IFile if we should
	if (m_bTrackIFile)
		m_strIFile = strFile + _T(".cpp");
}

void CNames::DefineMacros()
{
	DefineStringMacro(m_strMacro + _T("_CLASS"), m_strClass);
//	if(_T("CXG") == m_strBaseClass.Left(3))
	if(m_strBaseClass == _T("CXGridCtrl") || m_strBaseClass == _T("CXGridView"))
	{
		DefineStringMacro(m_strMacro + _T("_BASE_CLASS"), m_strBaseClass);
		DefineBoolMacro(m_strBaseClass, TRUE);
	}
	CString strFileName;
	CString strMacro = m_strMacro;

	strMacro.MakeLower();

	strFileName = m_strHFile;
	strFileName.Replace(".h","");
	DefineStringMacro(m_strMacro + _T("_HFILE"), strFileName);
	DefineStringMacro(strMacro + _T("_hfile"), strFileName);

	strFileName = m_strIFile;
	strFileName.Replace(".cpp","");
	DefineStringMacro(m_strMacro + _T("_IFILE"), strFileName);
	DefineStringMacro(strMacro + _T("_ifile"), strFileName);
}
