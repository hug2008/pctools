// ClassDlg.h : header file
// CG: This file was added by the AppWizard Tools component

#include "Names.h"

/////////////////////////////////////////////////////////////////////////////
// CClassesDlg dialog

class CClassesDlg : public CAppWizStepDlg
{
// Construction
public:
	CClassesDlg();
	virtual BOOL OnDismiss();

// Dialog Data
	//{{AFX_DATA(CClassesDlg)
	enum { IDD = IDD_CLASSES };
	CListBox	m_classList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iSelOld;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void WriteValues();
	BOOL m_bValidate;
	void DDV_FileName(CDataExchange* pDX, CString& s, LPCTSTR pszExtension = NULL);
	void DDV_ClassName(CDataExchange* pDX, CString& s);

	CBitmap m_bmpFlag;
	CBitmap m_bmpFon;

	// Generated message map functions
	//{{AFX_MSG(CClassesDlg)
	afx_msg void OnSelectClass();
	afx_msg void OnChangeClassName();
	afx_msg void OnChangeHFile();
	afx_msg void OnChangeIFile();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};
