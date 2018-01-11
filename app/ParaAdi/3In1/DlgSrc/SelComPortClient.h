#pragma once
#include "buttonxp.h"
#include "comboboxxp.h"
#include "afxwin.h"

// CSelComPortClient �Ի���

class CSelComPortClient : public CDialog
{
	DECLARE_DYNAMIC(CSelComPortClient)

public:
	CSelComPortClient(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelComPortClient();

// �Ի�������
	enum { IDD = IDD_DLG_SELCOMPORT_CLIENT };

	CComboBoxXP	m_cbPort;
	CComboBoxXP	m_cbBaut;
	CString	    m_strBaut;
	CString	    m_ComPort;
	CButtonXP	m_btOK;
	CButtonXP	m_btCancel;

protected:
	HICON m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog(void);
	afx_msg void OnBnClickedClientOk();
	afx_msg void OnBnClickedClientCancel();
	DECLARE_MESSAGE_MAP()	
};
