#if !defined(AFX_PROPPAGEPINSIM_H__81851616_BF0E_4A64_94DD_E7EBF8AE26C7__INCLUDED_)
#define AFX_PROPPAGEPINSIM_H__81851616_BF0E_4A64_94DD_E7EBF8AE26C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPagePinSim.h : header file
//

#include "stackdialog.h"
#include "comboboxxp.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPagePinSim dialog

class CPropPagePinSim : public CStackPage
{
// Construction
public:
	CPropPagePinSim(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:
	// 是否自动锁定PIN
	BOOL m_bAutoLockPin;

	// 是否使用随机PIN码
	BOOL m_bUseRandomPin;

	// 是否锁定SIM
	BOOL m_bLockSim;

public:
// Dialog Data
	//{{AFX_DATA(CPropPagePinSim)
	enum { IDD = IDD_PROPPAGE_8_PIN_SIM };
	CComboBoxXP	m_cbPinLength;
	CString	m_strInitPinCode;
	CString	m_strSimCode;
	int		m_nPinLength;
	CString	m_strFixedPinCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPagePinSim)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPagePinSim)
	virtual BOOL OnInitDialog();
	afx_msg void OnPage8RadioSimNo();
	afx_msg void OnPage8RadioSimYes();
	afx_msg void OnPage8RadioPinno();
	afx_msg void OnPage8RadioPinyes();
	afx_msg void OnSelchangePinlength();
	afx_msg void OnChangePin();
	afx_msg void OnChangeSim();
	afx_msg void OnPage8RadioUsepinyes();
	afx_msg void OnPage8RadioUsepinno();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEPINSIM_H__81851616_BF0E_4A64_94DD_E7EBF8AE26C7__INCLUDED_)
