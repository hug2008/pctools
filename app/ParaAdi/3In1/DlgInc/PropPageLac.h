#pragma once

#include "buttonxp.h"
#include "ComboBoxXP.h"
#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
#include "afxwin.h"
// CPropPageLac dialog

class CPropPageLac : public CStackPage
{
public:
	CPropPageLac(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropPageLac();

	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg *m_pParentWnd;
	CReportCtrl          m_rcLacTable;
	CButtonXP            m_btIm;
	CButtonXP            m_btEx;
	CEdit                m_eLacCount;

	void InitLacTable(void);
	void RefreshLacTable(CStringArray &szAryLac);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_LOCK_LAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButLacInport();
	afx_msg void OnBnClickedButLacExport();
	afx_msg void OnBnClickedRadioLockLacYes();
	afx_msg void OnBnClickedRadioLockLacNo();
	DECLARE_MESSAGE_MAP()	
};
