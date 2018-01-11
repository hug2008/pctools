#if !defined(AFX_WAITMSGDLG_H__DCAE98CF_A180_473A_BBBB_BA14F267D4C9__INCLUDED_)
#define AFX_WAITMSGDLG_H__DCAE98CF_A180_473A_BBBB_BA14F267D4C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitMsgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaitMsgDlg dialog

class CWaitMsgDlg : public CDialog
{
// Construction
public:
	CWaitMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaitMsgDlg)
	enum { IDD = IDD_WAIT_DIALOG };
	CStatic	m_ctrTask;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaitMsgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITMSGDLG_H__DCAE98CF_A180_473A_BBBB_BA14F267D4C9__INCLUDED_)
