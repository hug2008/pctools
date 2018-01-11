// BizPhoneManager.h : main header file for the BIZPHONEMANAGE application
//

#if !defined(AFX_BIZPHONEMANAGER_H__D882CE8F_DF5E_44A5_98B8_919122D4FF97__INCLUDED_)
#define AFX_BIZPHONEMANAGER_H__D882CE8F_DF5E_44A5_98B8_919122D4FF97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MyHeader.h"
#include "IniFile.H"
#include <Ado.h>
/////////////////////////////////////////////////////////////////////////////
// CBizPhoneManagerApp:
// See BizPhoneManager.cpp for the implementation of this class
//

class CBizPhoneManagerApp : public CWinApp
{
public:
	CBizPhoneManagerApp();
public:
	CString  m_szLpt;

	CString  m_szShare;

	BOOL     m_bLockPage;

	CIniFile m_Ini;

	CString  m_szStatus;

	int      m_nMmiIndex;

	CAdoConnection   m_ado;

#ifdef SEC_ZPL_LOG
	FILE *m_fLog;
#endif//SEC_ZPL_LOG
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBizPhoneManagerApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	
	// Implementation
	
	//{{AFX_MSG(CBizPhoneManagerApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CBizPhoneManagerApp theApp;
// ==========================================================================
// we difined some important global val here
// ==========================================================================


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIZPHONEMANAGER_H__D882CE8F_DF5E_44A5_98B8_919122D4FF97__INCLUDED_)
