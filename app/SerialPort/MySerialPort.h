// MySerialPort.h : main header file for the MYSERIALPORT application
//

#if !defined(AFX_MYSERIALPORT_H__65C061A9_F370_421B_9C2A_65AFAA0390EB__INCLUDED_)
#define AFX_MYSERIALPORT_H__65C061A9_F370_421B_9C2A_65AFAA0390EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMySerialPortApp:
// See MySerialPort.cpp for the implementation of this class
//

class CMySerialPortApp : public CWinApp
{
public:
	CMySerialPortApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySerialPortApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMySerialPortApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSERIALPORT_H__65C061A9_F370_421B_9C2A_65AFAA0390EB__INCLUDED_)
