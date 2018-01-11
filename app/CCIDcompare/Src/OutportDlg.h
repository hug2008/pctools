#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COutportDlg dialog

class COutportDlg : public CDialog
{
	DECLARE_DYNAMIC(COutportDlg)

public:
	COutportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutportDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_Outport_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	BOOL m_bTelnum;
	BOOL m_bImei;
	BOOL m_bCcid;
	BOOL m_bSerial;
	BOOL m_bArea;
	BOOL m_bBuyer;
	BOOL m_bcCheck;
	BOOL m_bcBuyer;
	BOOL m_bcArea;
	CString m_strbcBuyer;
	CString m_strbcArea;
	CComboBox m_CtrlBoxCheck;
	int m_iSelCheck;
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	CString m_strnewtablename;
};
