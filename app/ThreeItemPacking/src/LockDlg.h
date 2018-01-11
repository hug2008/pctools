#pragma once


// CLockDlg dialog

class CLockDlg : public CDialog
{
	DECLARE_DYNAMIC(CLockDlg)

public:
	CLockDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLockDlg();

// Dialog Data
	enum { IDD = IDD_LOCK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strpw;
	afx_msg void OnBnClickedOk();
};
