#pragma once


// CUserVerityDlg dialog

class CUserVerityDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserVerityDlg)

protected:
	HICON m_hIcon;

public:
	CUserVerityDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserVerityDlg();

// Dialog Data
	enum { IDD = IDD_USER_VERITY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(void);
    afx_msg void OnBnClickedBtVyManager(void);
	DECLARE_MESSAGE_MAP()
public:
	CString m_szUser;
	CString m_szPw;
};
