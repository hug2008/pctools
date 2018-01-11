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
#if defined(MMI_CN)
#include "resource.h"		// main symbols --> BizPhoneManager.rc
#else
#include "resource_en.h"		// main symbols  --> BizPhoneManager_en.rc
#endif
#include "MyHeader.h"

#if !defined(SYS_NAME)
#if defined(R16_ATC)
#define SYS_NAME  _T("BinPhoneManagerR16")
#else
#define SYS_NAME  _T("BinPhoneManagerR10")
#endif
#endif

#if !defined(VERSION_ID)
#if defined(FOR_CLIENT)
#define VERSION_ID   _T("Customer-D1.1.0")
#else
#define VERSION_ID   _T("Produce-D1.1.0")
#endif
#endif


#if !defined(VERSION_STATE)
#if defined(PAGE_CFG)
#if defined(NDEBUG)
#define VERSION_STATE   _T("Cfg Release")
#else
#define VERSION_STATE   _T("Cfg Debug")
#endif
#else
#if defined(NDEBUG)
#define VERSION_STATE   _T("Release")
#else
#define VERSION_STATE   _T("Debug")
#endif
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
	
#if defined(PAGE_CFG)
	/* 对一些特殊面页的处理 */
	PageEnable  m_pageEnable[PAGEENABLE_TOTAL];
#endif

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
#if defined(PAGE_CFG)
extern BOOL getPageEnableStatus(unsigned int  nPageId);
extern void setPageEnableStatus(unsigned int  nPageId, BOOL bEnable);
extern void getPageEnableStatusString(CString &szEnableStatusString);
extern void setPageEnableStatusString(CString szEnableStatusString);
extern unsigned int getPageCount(unsigned int  nPageId);
extern void setPageCount(unsigned int  nPageId, unsigned int  nCount);
#endif
// ==========================================================================
// we difined some important global val here
// ==========================================================================


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIZPHONEMANAGER_H__D882CE8F_DF5E_44A5_98B8_919122D4FF97__INCLUDED_)
