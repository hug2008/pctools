/////////////////////////////////////////////////////////////////////////////
// WizUtil.h - Miscellaneous utility functions
// CG: This file was added by the AppWizard Tools component

/////////////////////////////////////////////////////////////////////////////
// Helpers for putting entries into the dictionary

void DefineMacro(LPCTSTR lpszKey, LPCTSTR lpszValue);
void UndefMacro(LPCTSTR lpszKey);

// ...ints
void DefineIntMacro(LPCTSTR pszKey, int iValue);
void DefineIntMacro(LPCTSTR pszKey, int iItem, int iValue);

// ...BOOLS
void DefineBoolMacro(LPCTSTR pszKey, BOOL bValue);
void DefineBoolMacro(LPCTSTR pszKey, int iItem, BOOL bValue);

// ...strings
void DefineStringMacro(LPCTSTR pszKey, LPCTSTR pszValue);
void DefineStringMacro(LPCTSTR pszKey, int iItem, LPCTSTR pszValue);

// ...is define
BOOL IsMacroDefine(LPCTSTR pszKey);

/////////////////////////////////////////////////////////////////////////////
// Data Validation

BOOL IsValidFileName(LPCTSTR psz);
BOOL IsValidSymbol(LPCTSTR psz);
