#if !defined(AFX_PROPPAGELOCKLAC_H__88A98257_FCF2_4CCB_B3DD_9944DBC3EFB1__INCLUDED_)
#define AFX_PROPPAGELOCKLAC_H__88A98257_FCF2_4CCB_B3DD_9944DBC3EFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLockLac.h : header file
//
#include "stackdialog.h"

#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CPropPageLockLac dialog

class CPropPageLockLac : public CStackPage
{
// Construction
public:
	CPropPageLockLac(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:
	// switch for some lock service
	
	BOOL m_bLockLac;
	

	void InitLockRegionTable();


// Dialog Data
	//{{AFX_DATA(CPropPageLockLac)
	enum { IDD = IDD_PROPPAGE_10_LOCK_LAC};
	CReportCtrl	m_rcLockLac;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageLockLac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageLockLac)
	afx_msg void OnPage10RadioLacyes();
	afx_msg void OnPage10RadioLacno();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGELOCKLAC_H__88A98257_FCF2_4CCB_B3DD_9944DBC3EFB1__INCLUDED_)
