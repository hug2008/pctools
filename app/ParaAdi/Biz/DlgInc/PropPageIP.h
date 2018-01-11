#if !defined(AFX_PROPPAGEIP_H__E9B6917B_7BC5_47E6_B16A_475A8E872D46__INCLUDED_)
#define AFX_PROPPAGEIP_H__E9B6917B_7BC5_47E6_B16A_475A8E872D46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageIP.h : header file
//

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageIP dialog

class CPropPageIP : public CStackPage
{
// Construction
public:
	CPropPageIP(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:
	void InitIPRuleTable();
	int	 GetDaoRuCodeLength(CString string);
	void DoIPRuleSort(CStringArray &StrAry);

public:

// Dialog Data
	//{{AFX_DATA(CPropPageIP)
	enum { IDD = IDD_PROPPAGE_A_IP };
	CReportCtrl	m_rcIPRuleTable;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageIP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageIP)
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEIP_H__E9B6917B_7BC5_47E6_B16A_475A8E872D46__INCLUDED_)
