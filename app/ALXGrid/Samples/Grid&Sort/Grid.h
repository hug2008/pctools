// Grid.h : main header file for the GRID application
//

#if !defined(AFX_GRID_H__218AE4E3_1926_11D5_8A50_000102CA073A__INCLUDED_)
#define AFX_GRID_H__218AE4E3_1926_11D5_8A50_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGridApp:
// See Grid.cpp for the implementation of this class
//

#define GRIDCOLCOUNT (ID_COL_FOR_PAYMENT - ID_COL_DATE + 1)

typedef struct FieldColInfo
{
	BOOL m_bVisibl;
	int m_iField;
	int m_iCol;
	int m_nWidth;
} FLDCOL_INFO;

class CGridApp : public CWinApp
{
public:
	CGridApp();

	CString m_strDBName;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGridApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRID_H__218AE4E3_1926_11D5_8A50_000102CA073A__INCLUDED_)
