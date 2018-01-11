// Grid.h : main header file for the GRID application
//

#if !defined(AFX_GRID_H__E4C23EA3_EB85_11D4_8A41_000102CA073A__INCLUDED_)
#define AFX_GRID_H__E4C23EA3_EB85_11D4_8A41_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGridApp:
// See Grid.cpp for the implementation of this class
//

class CGridApp : public CWinApp
{
public:
	CGridApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGridApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRID_H__E4C23EA3_EB85_11D4_8A41_000102CA073A__INCLUDED_)
