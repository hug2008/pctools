#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CVfManagerDlg dialog

class CVfManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CVfManagerDlg)
protected:
	HICON m_hIcon;
	CListCtrl  m_list;
	CEdit      m_eUser;
	CString    m_sUser;
	CEdit      m_ePassw;
	CString    m_sPassw;
	CString    m_sVfFile;

	CStringArray m_userArray;
	CStringArray m_passwArray;

public:
	CVfManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVfManagerDlg();
	void  loadVyFile(CString szFile);
	void  RefreshList(void);
	void  InitList(void);

// Dialog Data
	enum { IDD = IDD_VY_MANAGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog(void);
//	afx_msg void OnHdnItemclickVymList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedVfmAdd();
	afx_msg void OnBnClickedVfmDel();
	afx_msg void OnBnClickedVfmLoad();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickVymList(NMHDR *pNMHDR, LRESULT *pResult);
};
