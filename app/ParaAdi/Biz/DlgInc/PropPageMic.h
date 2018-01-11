#if !defined(AFX_PROPPAGEMIC_H__3C725CC3_35BF_48A4_967D_94E3654AB750__INCLUDED_)
#define AFX_PROPPAGEMIC_H__3C725CC3_35BF_48A4_967D_94E3654AB750__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageMic.h : header file
//

#include "stackdialog.h"
#include "comboboxxp.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageMic dialog

class CPropPageMic : public CStackPage
{
// Construction
public:
	CPropPageMic(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

protected:

public:
// Dialog Data
	//{{AFX_DATA(CPropPageMic)
	enum { IDD = IDD_PROPPAGE_C_MIC };
	CComboBoxXP	m_cbCMICMain;
	CComboBoxXP	m_cbCMICAux;
	int		m_nCMICAux;
	int		m_nCMICMain;
	CString	m_strSidet0MainGain;
	CString m_strSidet1AuxGain;
	CString	m_strECHO0Vox;
	CString	m_strECHO0Min;
	CString	m_strECHO0Samp;
	CString	m_strECHO1Vox;
	CString	m_strECHO1Min;
	CString	m_strECHO1Samp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageMic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageMic)
	afx_msg void OnSelchangeCMICAux();
	afx_msg void OnSelchangeCMICMain();
	afx_msg void OnChangeEditEcho0Min();
	afx_msg void OnChangeEditEcho0Samp();
	afx_msg void OnChangeEditEcho0Vox();
	afx_msg void OnChangeEditEcho1Min();
	afx_msg void OnChangeEditEcho1Samp();
	afx_msg void OnChangeEditEcho1Vox();
	afx_msg void OnChangeEditSidetGain();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEMIC_H__3C725CC3_35BF_48A4_967D_94E3654AB750__INCLUDED_)
