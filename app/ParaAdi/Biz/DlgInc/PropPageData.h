#if !defined(AFX_PROPPAGEDATA_H__E909844C_3EB2_41EC_A9AD_6BD3D792581E__INCLUDED_)
#define AFX_PROPPAGEDATA_H__E909844C_3EB2_41EC_A9AD_6BD3D792581E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageData.h : header file
//

#include "stackdialog.h"
#include "buttonxp.h"
#include "BizPhoneSettingDlg.h"
#include "afxwin.h"

#include "staticex.hxx"
/////////////////////////////////////////////////////////////////////////////
// CPropPageData dialog

class CPropPageData : public CStackPage
{
// Construction
public:
	CPropPageData(CWnd* pParent = NULL);   // standard constructor

	void OnLockPage(BOOL bLock);
	void OnRefreshPage(void);
	void OnShowPage(int nState);


protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	CStringArray m_strCommandAry;

	void InfoSystemLoadFile(CString strLoadFileName);
	void InfoSystemSaveFile(CString strSaveFileName);
	BOOL ValidatePassword(void);
	BOOL isAllPasswordEmpty(void);

	CStaticEx m_stState;
	UserMode  m_eUseMode;


public:
// Dialog Data
	//{{AFX_DATA(CPropPageData)
	enum { IDD = IDD_PROPPAGE_B_DATA };
	CButtonXP	m_btManager;
	CButtonXP	m_btChangePwd;
	//CButtonXP	m_btChgPwdAndSave;
	CButtonXP	m_btProduce;
	CButtonXP	m_btLoadIni;
	CButtonXP	m_btSaveIni;
	CButtonXP	m_btLoadPhone;
	//CButtonXP	m_btSavePhone;
	CButtonXP	m_btPhone2Default;
	CButtonXP	m_btDefault2Phone;
	CString	m_strOldPwd;
	CString	m_strNewPwd;
	CString	m_strCnfPwd;
	BOOL	m_bUseDefaultPwd;
	CString	m_strDefaultPwd;
	CString m_szIniInput;
	CStatic m_cStIniInput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageData)
	virtual BOOL OnInitDialog();
	afx_msg void OnPageCheckDefaultPwd();
	//afx_msg void OnPageChangePwd();
	//afx_msg void OnPageChangeAndSave();
	afx_msg void OnPageProduce();
	afx_msg void OnPageLoadFromIni();
	afx_msg void OnPageSaveToIni();
	afx_msg void OnPageLoadModuleSetting();
	afx_msg void OnPageSaveModuleSetting();
	afx_msg void OnPageSaveDefault();
	afx_msg void OnPageRecoverDefault();
	afx_msg void OnUserVerify(void);
	afx_msg void OnEnChangePageBEditDefaultpwd();
	//}}AFX_MSG
	afx_msg LRESULT OnEnableOperationButtom(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEDATA_H__E909844C_3EB2_41EC_A9AD_6BD3D792581E__INCLUDED_)
