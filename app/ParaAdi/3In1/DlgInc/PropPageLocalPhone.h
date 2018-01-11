#if !defined(AFX_PROPPAGELOCALPHONE_H__3B0F5FD0_9B84_463A_BFD0_2E0CE8CB762F__INCLUDED_)
#define AFX_PROPPAGELOCALPHONE_H__3B0F5FD0_9B84_463A_BFD0_2E0CE8CB762F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLocalPhone.h : header file
//

#include "stackdialog.h"
#include "PhoneNumber.h"
#include "BizPhoneSettingDlg.h"
#include "afxwin.h"
#include "staticex.hxx"

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalPhone dialog

class CPropPageLocalPhone : public CStackPage
{
// Construction
public:
	CPropPageLocalPhone(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	BOOL m_bAIDial;
	BOOL m_bAutoLock;	
	BOOL m_bVoiceTip;	
	BOOL m_bSpecialApp;

public:
// Dialog Data
	//{{AFX_DATA(CPropPageLocalPhone)
	enum { IDD = IDD_PROPPAGE_G_LOCAL_PHONE };
//	CPhoneNumber	m_strAdValue;
	CPhoneNumber	m_strTime;
	CPhoneNumber	m_strID;
//	CPhoneNumber	m_strVerifyCode;
	CStaticEx       m_stRateVirityCode;
	CStaticEx       m_stFramFlag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageLocalPhone)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageLocalPhone)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePageId();
	afx_msg void OnChangePageTime();
//	afx_msg void OnChangePageVerify();
//	afx_msg void OnChangeEditAdscript();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BOOL IsValidFramFlag(const char *FramFlag_p);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGELOCALPHONE_H__3B0F5FD0_9B84_463A_BFD0_2E0CE8CB762F__INCLUDED_)
