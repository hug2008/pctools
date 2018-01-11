#pragma once
#include "mydefine.h"

// CModifItemDlg dialog

class CModifItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CModifItemDlg)

public:
	CModifItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifItemDlg();

protected:
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_MODIFITEM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()	
public:
	CString     m_strSerial;
	CString     m_strTelnum;
	CString     m_strImei;
	CString     m_strCcid;
protected:
	CEditEx     m_ctrlSerial;
	CEditEx     m_ctrlTelnum;
	CEditEx     m_ctrlImei;
	CEditEx     m_ctrlCcid;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
