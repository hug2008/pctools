#if !defined(AFX_PROPPAGESYS_H__08135F10_6369_4564_B902_13721B5E5949__INCLUDED_)
#define AFX_PROPPAGESYS_H__08135F10_6369_4564_B902_13721B5E5949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageSys.h : header file
//

#include "stackdialog.h"
#include "comboboxxp.h"
#include "PhoneNumber.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageSys dialog

class CPropPageSys : public CStackPage
{
// Construction
public:
	CPropPageSys(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	BOOL m_bNotLocalAddZero;
	BOOL m_bForbidLongDistanceCall;

public:
// Dialog Data
	//{{AFX_DATA(CPropPageSys)
	enum { IDD = IDD_PROPPAGE_1_SYS };
	CComboBoxXP		m_cbAutoDelay;
	CPhoneNumber	m_strSMSCenter;
	int				m_nAutoDelay;
	int				m_nLcdContrast;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageSys)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageSys)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeAutoDelay();
	afx_msg void OnChangeSmsCenter();
	afx_msg void OnPage1RadioAddyes();
	afx_msg void OnPage1RadioAddno();
	afx_msg void OnPage1RadioForbidYes();
	afx_msg void OnPage1RadioForbidNo();
	afx_msg void OnChangeLcdContrast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGESYS_H__08135F10_6369_4564_B902_13721B5E5949__INCLUDED_)
