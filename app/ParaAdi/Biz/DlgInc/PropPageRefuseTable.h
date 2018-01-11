#if !defined(AFX_PROPPAGEREFUSETABLE_H__696A2793_46B8_45C7_818E_715367CFF5AA__INCLUDED_)
#define AFX_PROPPAGEREFUSETABLE_H__696A2793_46B8_45C7_818E_715367CFF5AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageRefuseTable.h : header file
//

#include "stackdialog.h"
#include "comboboxxp.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageRefuseTable dialog

class CPropPageRefuseTable : public CStackPage
{
// Construction
public:
	CPropPageRefuseTable(CWnd* pParent = NULL);   // standard constructor

	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:
	void InitRefuseTable();

public:
// Dialog Data
	//{{AFX_DATA(CPropPageRefuseTable)
	enum { IDD = IDD_PROPPAGE_7_REFUSE_TABLE };
	CComboBoxXP	m_cbRefuseType;
	int			m_nRefuseType;
	CReportCtrl	m_rcRefuseTable;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageRefuseTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageRefuseTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeRefuseType();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEREFUSETABLE_H__696A2793_46B8_45C7_818E_715367CFF5AA__INCLUDED_)
