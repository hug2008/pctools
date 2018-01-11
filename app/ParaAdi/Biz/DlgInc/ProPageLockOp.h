#pragma once

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

// CPropPageLockOp dialog

class CPropPageLockOp : public CStackPage
{
public:
	CPropPageLockOp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropPageLockOp();

public:
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg  *m_pParentWnd;
	CReportCtrl          m_list;

	CButton              m_btLockYes;
	CButton              m_btLockNo;

	void InitListTable(void);
	void CleanListTable(void);
	void RefreshListUsingProviderAry(void);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_LOCK_OP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(void);
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPage9RadioLockProviderYes(void);
	afx_msg void OnPage9RadioLockProviderNo(void);
	DECLARE_MESSAGE_MAP()
};
