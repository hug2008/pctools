#if !defined(AFX_PROPPAGELOCAL_H__E3AFE5A8_6CD0_457B_A2D6_5CF82E695B13__INCLUDED_)
#define AFX_PROPPAGELOCAL_H__E3AFE5A8_6CD0_457B_A2D6_5CF82E695B13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLocal.h : header file
//

#include "stackdialog.h"
#include "comboboxxp.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocal dialog

class CPropPageLocal : public CStackPage
{
// Construction
public:
	CPropPageLocal(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

public:
// Dialog Data
	//{{AFX_DATA(CPropPageLocal)
	enum { IDD = IDD_PROPPAGE_2_LOCAL };
	CComboBoxXP	m_cbLocalMethod;
	CString	m_strLocalNum;
	int		m_nLocalMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageLocal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageLocal)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLocalMethod();
	afx_msg void OnChangePage2EditLocalnum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGELOCAL_H__E3AFE5A8_6CD0_457B_A2D6_5CF82E695B13__INCLUDED_)
