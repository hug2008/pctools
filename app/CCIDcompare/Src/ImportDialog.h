#pragma once


// CImportDialog dialog

class CImportDialog : public CDialog
{
	DECLARE_DYNAMIC(CImportDialog)

public:
	CImportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportDialog();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_Import_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strpathname;
	afx_msg void OnBnClickedButton1();
	CString m_strSheetname;
	int M_INTTELCOL;
	int M_INTCCIDCOL;
};
