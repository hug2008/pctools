#if !defined(AFX_PROPPAGEFORBIDTABLE_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)
#define AFX_PROPPAGEFORBIDTABLE_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageForbidTable.h : header file
//

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageForbidTable dialog

class CPropPageForbidTable : public CStackPage
{
// Construction
public:
	CPropPageForbidTable(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:
	// ≥ı ºªØlistctrl
	void InitForbidTable();

public:
// Dialog Data
	//{{AFX_DATA(CPropPageForbidTable)
	enum { IDD = IDD_PROPPAGE_4_FORBID_TABLE };
	CReportCtrl	m_rcForbidTable;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageForbidTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageForbidTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEFORBIDTABLE_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)
