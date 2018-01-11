/////////////////////////////////////////////////////////////////////////////
// WizUtil.cpp - Miscellaneous utility functions
// CG: This file was added by the AppWizard Tools component

#include "stdafx.h"
#include "ALXGridAw.h"
#include "WizUtil.h"

/////////////////////////////////////////////////////////////////////////////
// Miscellaneous utility functions

// These are generic macro-setting functions. They set macros or remove
// them from the dictionary.

void DefineMacro(LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	ALXAppWz.m_Dictionary[lpszKey] = lpszValue;
}

void UndefMacro(LPCTSTR lpszKey)
{
	ALXAppWz.m_Dictionary.RemoveKey(lpszKey);
}

// These are more specific macro-setting functions. They set macros
// depending on the "type", and work correctly in BEGINLOOP/ENDLOOP
// blocks.

static void MakeKey(CString& strKey, int iItem)
{
	CString strOriginal = strKey;
	strKey.Format(_T("%s_%d"), (LPCTSTR) strKey, iItem);
}

void DefineIntMacro(LPCTSTR pszKey, int iValue)
{
	CString strValue;
	strValue.Format(_T("%d"), iValue);
	DefineMacro(pszKey, strValue);
}

void DefineIntMacro(LPCTSTR pszKey, int iItem, int iValue)
{
	CString strKey(pszKey);
	MakeKey(strKey, iItem);
	DefineIntMacro(strKey, iValue);
}

void DefineBoolMacro(LPCTSTR pszKey, BOOL bValue)
{
	if (bValue)
		DefineMacro(pszKey, _T("1"));
	else
		UndefMacro(pszKey);
}

void DefineBoolMacro(LPCTSTR pszKey, int iItem, BOOL bValue)
{
	CString strKey(pszKey);
	MakeKey(strKey, iItem);
	DefineBoolMacro(strKey, bValue);
}

void DefineStringMacro(LPCTSTR pszKey, LPCTSTR pszValue)
{
	DefineMacro(pszKey, pszValue);
}

void DefineStringMacro(LPCTSTR pszKey, int iItem, LPCTSTR pszValue)
{
	CString strKey(pszKey);
	MakeKey(strKey, iItem);
	DefineStringMacro(strKey, pszValue);
}

BOOL IsMacroDefine(LPCTSTR pszKey)
{
	return ALXAppWz.m_Dictionary.Lookup(pszKey, CString(""));
}

BOOL IsValidFileName(LPCTSTR psz)
{
	if (*psz == _T('\0') || *psz == _T('.'))
		return FALSE;

	TCHAR ch;
	while (ch = *psz)
	{
		if (ch == _T('\\') || ch == _T('//') || ch == _T(':'))
			return FALSE;

		psz = _tcsinc(psz);
	}

	return TRUE;
}

BOOL IsValidSymbol(LPCTSTR psz)
{
	if (!_istalpha(*psz) && *psz != _T('_'))
	{
		return FALSE;
	}

	psz = _tcsinc(psz);
	while (*psz != NULL)
	{
		if (!_istalnum(*psz) && *psz != _T('_'))
			return FALSE;
		psz = _tcsinc(psz);
	}

	return TRUE;
}
