// PropPageEncrypt.h: interface for the CPropPageEncrypt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPPAGEENCRYPT_H__A9370733_B362_4FA7_A648_3CF914C14FC8__INCLUDED_)
#define AFX_PROPPAGEENCRYPT_H__A9370733_B362_4FA7_A648_3CF914C14FC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
#include "comboboxxp.h"
class CPropPageEncrypt : public CStackPage
{
public:
	CPropPageEncrypt(CWnd* pParent = NULL);  // Standard constructor


	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);
	
protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	
protected:

//	virtual ~CPropPageEncrypt();
public:
	// Dialog Data
	//{{AFX_DATA(CPropPageEncrypt)
	enum { IDD = IDD_PROPPAGE_16_ENCRYPTED_SOLUTION};
	CComboBoxXP	m_cbEncryptMethod;
	CComboBoxXP	m_cVendor;
	int      i_vendorNum;
	int      m_nEncryptSolution;
	CString  m_sVendor;
	CString  m_skeyword1;
	CString  m_skeyword2;
	CString  m_skeyword3;
	CString  m_skeyword4;
	CString  m_skeyword5;
	CString  m_skeyno;

	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageEncrypt)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CPropPageEncrypt)
	virtual BOOL OnInitDialog();

	afx_msg 	void OnSelchangeEncryption();
	afx_msg     void OnChangeEditKeyword5();
	afx_msg     void OnChangeEditKeyword4();
	afx_msg     void OnChangeEditKeyword3();
	afx_msg     void OnChangeEditKeyword2();
	afx_msg     void OnChangeEditKeyword1();
	afx_msg     void OnChangeEditVendorNo();
	afx_msg     void OnChangeEditKeyNo();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_PROPPAGEENCRYPT_H__A9370733_B362_4FA7_A648_3CF914C14FC8__INCLUDED_)
