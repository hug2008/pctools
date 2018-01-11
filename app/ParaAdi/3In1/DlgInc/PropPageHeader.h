#if !defined(PROP_PAGE_MOBILE_HEADER_H)
#define PROP_PAGE_MOBILE_HEADER_H

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"
#include "afxwin.h"

// CPropPageHeader dialog

class CPropPageHeader : public CStackPage
{
public:
	CPropPageHeader(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropPageHeader();

	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

// Dialog Data
	enum { IDD = IDD_PROPPAGE_MOBILE_HEADER };

protected:
	void InitMobileHeaderTable(void);

protected:
	CBizPhoneSettingDlg *m_pParentWnd;
	CReportCtrl	        m_rcMob;
	CStatic             m_stTips;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnMobileHeaderRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMobileHeaderRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()	
};
#endif/* PROP_PAGE_MOBILE_HEADER_H */