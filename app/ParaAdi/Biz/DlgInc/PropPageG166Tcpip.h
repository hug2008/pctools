#pragma once
#include "stackdialog.h"
#include "BizPhoneSettingDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

// CPropPageG166Tcpip dialog

class CPropPageG166Tcpip : public CStackPage
{
public:
	CPropPageG166Tcpip(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPropPageG166Tcpip();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_G166_TCPIP };

public:
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	CIPAddressCtrl       m_ipAr;
	CEdit                m_ePort;
	CString              m_strPort;
	CIPAddressCtrl       m_dns01;
	CIPAddressCtrl       m_dns02;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnIpnFieldchangedG166Ip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeG166Ipport();
	afx_msg void OnIpnFieldchangedG166Dns1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnIpnFieldchangedG166Dns2(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

// IP地址格式判断与格式转换
typedef enum ValidIpAddressTag{
	AR_Valid,	
	AR_Field0,
	AR_Field1,
	AR_Field2,
	AR_Field3,
	AR_Nfield = 4,
	AR_Short = 7,
	AR_Long = 15
}ValidIpAddress;


ValidIpAddress IsValidIpAddress(const char *szIpAddress);

int GetFieldfromValidIpAddress(BYTE& nField0, 
								BYTE& nField1, 
								BYTE& nField2, 
								BYTE& nField3, 
								const char *szValidIpAddress);
extern BOOL IsNumberString(CString &Src_string);
