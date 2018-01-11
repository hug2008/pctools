#if !defined(AFX_PROPPAGEOUTPUT_H__0C0A0C92_FC25_4065_AA08_FFD6E5CC909A__INCLUDED_)
#define AFX_PROPPAGEOUTPUT_H__0C0A0C92_FC25_4065_AA08_FFD6E5CC909A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageOutput.h : header file
//

#include "stackdialog.h"
#include "ReportCtrl.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageOutput dialog

class CPropPageOutput : public CStackPage
{
// Construction
public:
	CPropPageOutput(CWnd* pParent = NULL);   // standard constructor
	void OutputMessage(CString strMsg, BOOL bNoError = TRUE);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	void InitLogOutputList();
public:
// Dialog Data
	//{{AFX_DATA(CPropPageOutput)
	enum { IDD = IDD_PROPPAGE_X_OUTPUT };
	CReportCtrl	m_rcOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageOutput)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnOutputOperationMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEOUTPUT_H__0C0A0C92_FC25_4065_AA08_FFD6E5CC909A__INCLUDED_)
