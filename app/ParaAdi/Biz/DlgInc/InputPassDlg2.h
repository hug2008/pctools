#if !defined(AFX_INPUTPASSDLG2_H__AF4B7F56_B127_4404_B3A8_6275A9F28AC0__INCLUDED_)
#define AFX_INPUTPASSDLG2_H__AF4B7F56_B127_4404_B3A8_6275A9F28AC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputPassDlg2.h : header file
//

#include "buttonxp.h"

/////////////////////////////////////////////////////////////////////////////
// CInputPassDlg2 dialog

class CInputPassDlg2 : public CDialog
{
// Construction
public:
	CInputPassDlg2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputPassDlg2)
	enum { IDD = IDD_INPUT_PASS };
	CString	m_sOldPass;
	CButtonXP	m_btOK;
	CButtonXP	m_btCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputPassDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputPassDlg2)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTPASSDLG2_H__AF4B7F56_B127_4404_B3A8_6275A9F28AC0__INCLUDED_)
