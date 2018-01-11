#pragma once

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
#include "afxwin.h"

// CPropPageEmCallCfg 对话框

class CPropPageEmCallCfg : public CStackPage
{
public:
	CPropPageEmCallCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPropPageEmCallCfg();


public:
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

	CReportCtrl	m_rcEmCallList;
	void InitEmCallList(void);
	void resetEmcallList(int nItemCount);
	CButton m_ckEnable;

// 对话框数据
	enum { IDD = IDD_PROPPAGE_EMERGENCY_CALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog(void);
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
#if defined(PAGE_CFG)
	afx_msg void OnBnClickedCheckEmcallEnable();
#endif
	DECLARE_MESSAGE_MAP()
};
