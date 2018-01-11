#if !defined(AFX_PROPPAGELIMITSMS_H__5F6FDB78_55FC_4282_B362_99867DFDB69__INCLUDED_)
#define AFX_PROPPAGELIMITSMS_H__5F6FDB78_55FC_4282_B362_99867DFDB69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLock.h : header file
//

#include "stackdialog.h"
#include "comboboxxp.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

#define   MAX_PAGELIMIT_LIMITITEM_COUNT        (40)
#define   MAX_LIMITNUM_LEN                     (16)
#define   BASE_PAGELIMIT_HEAD                  (14)

// CPropPageLimitSms dialog


class CPropPageLimitSms : public CStackPage
{

public:
	CPropPageLimitSms(CWnd* pParent = NULL);   // standard constructor

	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);

protected:
	void InitLimitNumTable(void);

protected:
	CBizPhoneSettingDlg *m_pParentWnd;

// Dialog Data
	//{{AFX_DATA(CPropPageLimitSms)
	enum { IDD = IDD_PROPPAGE_L_LIMIT };

	CReportCtrl	      m_listlimitnum;
	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPageLimitRadioYes();
	afx_msg void OnPageLimitRadioNo();

	DECLARE_MESSAGE_MAP()
};

#endif//AFX_PROPPAGELIMITSMS_H__5F6FDB78_55FC_4282_B362_99867DFDB69__INCLUDED_