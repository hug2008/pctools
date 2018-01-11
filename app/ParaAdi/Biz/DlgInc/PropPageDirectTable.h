#if !defined(AFX_CPropPageDirectTable_H__C4D0BDD9_8D53_483D_ADF6_6812BD3A285D__INCLUDED_)
#define AFX_CPropPageDirectTable_H__C4D0BDD9_8D53_483D_ADF6_6812BD3A285D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPropPageDirectTable.h : header file
//

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageDirectTable dialog

class CPropPageDirectTable : public CStackPage
{
// Construction
public:
	CPropPageDirectTable(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:
	void InitDirectTable();

public:
// Dialog Data
	//{{AFX_DATA(CPropPageDirectTable)
	enum { IDD = IDD_PROPPAGE_3_DIRECT_TABLE };
	CReportCtrl	m_rcDirectTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageDirectTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageDirectTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPropPageDirectTable_H__C4D0BDD9_8D53_483D_ADF6_6812BD3A285D__INCLUDED_)
