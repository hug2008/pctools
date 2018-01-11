#pragma once
#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
#include "afxwin.h"

// CPropPageOpnCfg 对话框

typedef struct OpNamePlmnTag
{
	CString szMcc;
	CString szMnc1;
	CString szMnc2;
	CString szMnc3;
	CString szOpName;
}OpNamePlmn;

BOOL strIsValidOpnCfg(CString strOpnCfg);

class CPropPageOpnCfg : public CStackPage
{
public:
	CPropPageOpnCfg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPropPageOpnCfg();
public:
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;

	OpNamePlmn  m_OpCfg[OPN_TOTAL];

	void getOpnCfgList(void);

	CReportCtrl	m_rcOpnList;
	void InitOpnList(void);
	void resetOpnList(int nItemCount);

	CButton m_ckEnable;

// 对话框数据
	enum { IDD = IDD_PROPPAGE_OPN_CFG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog(void);
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
#if defined(PAGE_CFG)
	afx_msg void OnBnClickedCheckOpncfgEnable();
#endif
	DECLARE_MESSAGE_MAP()
};
