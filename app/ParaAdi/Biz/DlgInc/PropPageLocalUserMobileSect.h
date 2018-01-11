#if !defined(AFX_PROPPAGELOCALUSERMOBILESECT_H__B81B0CF3_B3AE_4851_A305_EC285C943EAE__INCLUDED_)
#define AFX_PROPPAGELOCALUSERMOBILESECT_H__B81B0CF3_B3AE_4851_A305_EC285C943EAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLocalUserMobileSect.h : header file
//

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalUserMobileSect dialog

class CPropPageLocalUserMobileSect : public CStackPage
{
// Construction
public:
	CPropPageLocalUserMobileSect(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:
	void InitLocalUserMobileTable();

public:
// Dialog Data
	//{{AFX_DATA(CPropPageLocalUserMobileSect)
	enum { IDD = IDD_PROPPAGE_6_LOCAL_USER_MOBILESECT };
	CReportCtrl	m_rcUserLocalTable;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageLocalUserMobileSect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageLocalUserMobileSect)
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGELOCALUSERMOBILESECT_H__B81B0CF3_B3AE_4851_A305_EC285C943EAE__INCLUDED_)
