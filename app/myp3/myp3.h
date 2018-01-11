// myp3.h : main header file for the MYP3 application
//

#if !defined(AFX_MYP3_H__D24507C1_6611_4D1B_AE4D_72FD5793B7B6__INCLUDED_)
#define AFX_MYP3_H__D24507C1_6611_4D1B_AE4D_72FD5793B7B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMyp3App:
// See myp3.cpp for the implementation of this class
//

class CMyp3App : public CWinApp
{
public:
	CMyp3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyp3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMyp3App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYP3_H__D24507C1_6611_4D1B_AE4D_72FD5793B7B6__INCLUDED_)
