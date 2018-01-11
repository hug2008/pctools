#if !defined(AFX_PROPPAGEFEE_H__6C31FDD0_ABFB_4C4C_BC4F_E562370A6E35__INCLUDED_)
#define AFX_PROPPAGEFEE_H__6C31FDD0_ABFB_4C4C_BC4F_E562370A6E35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageFee.h : header file
//

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageFee dialog

class CPropPageFee : public CStackPage
{
// Construction
public:
	CPropPageFee(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	void InitFeeTable();
	
public:
	// Dialog Data
	//{{AFX_DATA(CPropPageFee)
	enum { IDD = IDD_PROPPAGE_H_FEE };
	CReportCtrl	m_rcFeeTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageFee)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageFee)
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEFEE_H__6C31FDD0_ABFB_4C4C_BC4F_E562370A6E35__INCLUDED_)
