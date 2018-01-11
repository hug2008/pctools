#if !defined(AFX_PROPPAGELOCALMOBILESECT_H__6B01BD74_BF48_4DD1_9C51_3693E4FA37F1__INCLUDED_)
#define AFX_PROPPAGELOCALMOBILESECT_H__6B01BD74_BF48_4DD1_9C51_3693E4FA37F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLocalMobileSect.h : header file
//

#include "buttonxp.h"
#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalMobileSect dialog

class CPropPageLocalMobileSect : public CStackPage
{
// Construction
public:
	CPropPageLocalMobileSect(CWnd* pParent = NULL);   // standard constructor

	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	// CStringArray m_strLocalMobileSectArray;
	// CStringArray m_strSortedLocalMobileSectArray;

	CStatic  m_stTips;
	CEdit    m_edCount;
	CStatic  m_stCountTip;

protected:
	void InitLocalMobileTable();
	void SortImportedString(CStringArray& strSortAry);
	void DisplayLoadedData(CStringArray& strSortAry);

public:
// Dialog Data
	//{{AFX_DATA(CPropPageLocalMobileSect)
	enum { IDD = IDD_PROPPAGE_5_LOCAL_MOBILESECT };
	CButtonXP	m_btImport;
	CButtonXP	m_btExport;
	CReportCtrl	m_rcLocalTable;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageLocalMobileSect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageLocalMobileSect)
	virtual BOOL OnInitDialog();
	afx_msg void OnPage5BtnImport();
	afx_msg void OnPage5BtnExport();
#if defined(PAGE_CFG)
	afx_msg void OnEnChangeEditLmnhCount();
#endif
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGELOCALMOBILESECT_H__6B01BD74_BF48_4DD1_9C51_3693E4FA37F1__INCLUDED_)
