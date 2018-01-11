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

#include "resource_en.h"		// main symbols  --> BizPhoneManager_en.rc

#include "MyHeader.h"
#include "Para3In1Def.h"


#if !defined(VERSION_ID)
#if defined(FOR_CLIENT)
#define VERSION_ID   _T("Customer-D0.1.5")
#else
#define VERSION_ID   _T("Produce-D0.1.5")
#endif
#endif


#if !defined(VERSION_STATE)
#if defined(NDEBUG)
#define VERSION_STATE   _T("Release")
#else
#define VERSION_STATE   _T("Debug")
#endif
#endif

#define PAGEENABLE_TOTAL      (3)

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneManagerApp:
// See BizPhoneManager.cpp for the implementation of this class
//

typedef enum CompareIniStateTag
{
	CIS_NULL,
	CIS_READING,
	CIS_WRITING,
	CIS_SAME,
	CIS_DIF,

	CIS_NUM
}CompareIniState;

typedef enum UserModeTag
{ 
    UM_SAVE,
	UM_LOAD,
	UM_TOTAL
}UserMode;

class CBizPhoneManagerApp : public CWinApp
{
public:
	CBizPhoneManagerApp();
public:
	CString   m_bEnPassw;
	CString   m_szPassw;

	CString   m_szCurUser;
	BOOL      m_bEnablePage;
	BOOL      m_bIsAdmin;
	int       m_bUseMode;

	CString   m_szPort;
	CString   m_szBaut;

	CString   m_szIniInput;

	CString   m_szAppPath;
	
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
