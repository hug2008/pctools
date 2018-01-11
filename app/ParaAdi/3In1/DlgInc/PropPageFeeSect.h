#if !defined(AFX_PROPPAGEFEESECT_H__8B0EDC1A_2741_41EC_9CEA_E8D088D12E20__INCLUDED_)
#define AFX_PROPPAGEFEESECT_H__8B0EDC1A_2741_41EC_9CEA_E8D088D12E20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageFeeSect.h : header file
//

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageFeeSect dialog

class CPropPageFeeSect : public CStackPage
{
// Construction
public:
	CPropPageFeeSect(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	void InitFeeSectTable();
	
public:
// Dialog Data
	//{{AFX_DATA(CPropPageFeeSect)
	enum { IDD = IDD_PROPPAGE_I_FEESECT };
	CReportCtrl	m_rcFeeSectTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageFeeSect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageFeeSect)
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEFEESECT_H__8B0EDC1A_2741_41EC_9CEA_E8D088D12E20__INCLUDED_)
