// StackDialog.h: interface for the CStackDialog class.
//
//////////////////////////////////////////////////////////////////////

/*#############################################################################
# STACKDIALOG.H
#
# SCA Software International S.A.
# http://www.scasoftware.com
# scaadmin@scasoftware.com
#
# Copyright (c) 1999 SCA Software International S.A.
#
# Date: 17.12.1999.
# Author: Zoran M.Todorovic
#
# This software is provided "AS IS", without a warranty of any kind.
# You are free to use/modify this code but leave this header intact.
#
#############################################################################*/

#if !defined(AFX_STACKDIALOG_H__C1ADDF75_CC4E_11D4_A156_00104B2E7509__INCLUDED_)
#define AFX_STACKDIALOG_H__C1ADDF75_CC4E_11D4_A156_00104B2E7509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

using namespace std;

class CStackPage;
class CStackDialog;

//=============================================================================
// Helper structure to maintain information about each page.
//=============================================================================

class CStackPageHelper 
{
private:
	BOOL			m_bCreated;				// True if m_pDialog is created
	BOOL			m_bActive;				// True if dialog is active (has focus)
	UINT			m_nDialogID;			// Resource ID of this page
	CStackPage*		m_pDialog;				// Child dialog of this page
	CStackDialog*	m_pParent;				// Parent dialog
	
public:
	DWORD m_dwHelpID;         // Help ID for this page
	DWORD m_dwData1;          // User defined data
	DWORD m_dwData2;          // User defined data
	
public:
	CStackPageHelper();
	CStackPageHelper(const CStackPageHelper& obj);
	~CStackPageHelper();
	CStackPageHelper& operator=(const CStackPageHelper& obj);
	
	CStackPage *GetPage(void);

	BOOL IsActive(void);
	void Activate(void);
	void Deactivate(void);
	
	friend class CStackDialog;
};

typedef list<CStackPageHelper> CStackPageHelperList;

//=============================================================================
// This is a stacked dialog page class declaration. 
//=============================================================================

class CStackPage : public CDialog 
{
public:
	CStackPage(LPCTSTR lpszTemplateName, CWnd* pParent = NULL);
	CStackPage(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CStackPage();
	
	virtual void OnSetActive(void);
	virtual void OnKillActive(void);
	virtual void OnCreatePage(void);
	virtual void OnDestroyPage(void);
	virtual BOOL OnCloseTest(void);
	virtual void OnSaveData(void);
	virtual void OnRefreshPage(void);
	virtual void OnLockPage(BOOL bLock);
	virtual void OnShowPage(int nState);

	//{{AFX_VIRTUAL(CStackPage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CStackPage)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//=============================================================================
// This is a stacked dialog class declaration. This is an abstract class.
// Must derive a class and implement CreatePage() member function.
//=============================================================================

class CStackDialog : public CDialog 
{
protected:
	UINT					m_nPlaceholderID;
	CStackPageHelperList	m_PageList;
	
private:
	void init(UINT placeholder);
	void flush(void);
	void destroyPageDialog(CStackPageHelper *pPage);

	CStackPageHelper *getFirst(void);
	CStackPageHelper *getNext(void);
	
protected:
	void activatePage(CStackPageHelper *pPage);
	void deactivatePage(CStackPageHelper *pPage);
	CStackPageHelper *getCurrent(void);
	
public:
	CStackDialog(UINT nId, LPCTSTR lpszTemplate, CWnd* pParent = NULL);
	CStackDialog(UINT nId, UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CStackDialog();
	
	CStackPageHelper *AddPage(UINT nId, DWORD dwHelpId = 0L,
							  DWORD dwData1 = 0, DWORD dwData2 = 0);
	void DelPage(CStackPageHelper *pPage);
	
	void SetPage(void);
	void SetPage(UINT nId);
	void SetFirstPage(void);
	void SetNextPage(void);
	
	CStackPageHelper *GetPage(void);
	CStackPageHelper *GetPage(UINT nId);
	
	virtual CStackPage *CreatePage(UINT nId) = 0;
	virtual void OnPageChanged(UINT nId, BOOL bActivated) = 0;
	
	//{{AFX_VIRTUAL(CStackDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CStackDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_STACKDIALOG_H__C1ADDF75_CC4E_11D4_A156_00104B2E7509__INCLUDED_)

/*#############################################################################
# End of file STACKDIALOG.H
#############################################################################*/