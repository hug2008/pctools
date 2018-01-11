#if !defined(AFX_PROPPAGELOCK_H__5F6E8B78_55FC_4282_B362_9983C5DFDB69__INCLUDED_)
#define AFX_PROPPAGELOCK_H__5F6E8B78_55FC_4282_B362_9983C5DFDB69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLock.h : header file
//

#include "stackdialog.h"
#include "comboboxxp.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
#define   MAXREGIONNUM         (20)

/////////////////////////////////////////////////////////////////////////////
// CPropPageLock dialog

class CPropPageLock : public CStackPage
{
	// Construction
public:
	CPropPageLock(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);
	
protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	
protected:
	// switch for some lock service
	BOOL m_bLockProvider;
	BOOL m_bLockRegion;
	BOOL m_bAutoLockRegion;
	
	void InitLockRegionTable();
	
public:
	enum { IDD = IDD_PROPPAGE_9_LOCK };
	
	CReportCtrl       m_rcLockRegion;
	
	CEdit		      m_edtRegionNum;
	
	CSpinButtonCtrl   m_spnRegionNum;
	
	CString           m_strProvider;
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageLock)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CPropPageLock)
	virtual BOOL OnInitDialog();
	afx_msg void OnPage9RadioProviderNo();
	afx_msg void OnPage9RadioProviderYes();
	afx_msg void OnPage9RadioRegionYes();
	afx_msg void OnPage9RadioRegionNo();
	afx_msg void OnPage9RadioAutolockno();
	afx_msg void OnPage9RadioAutolockyes();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeRegionNum();
	afx_msg void OnChangeEditRegionNum();
	afx_msg void OnChangeEditProvider();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGELOCK_H__5F6E8B78_55FC_4282_B362_9983C5DFDB69__INCLUDED_)
