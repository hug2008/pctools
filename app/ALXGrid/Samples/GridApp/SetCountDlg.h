#if !defined(AFX_SETCOUNTDLG_H__1486B4C3_5899_11D4_8A40_005004555789__INCLUDED_)
#define AFX_SETCOUNTDLG_H__1486B4C3_5899_11D4_8A40_005004555789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCountDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetCountDlg dialog

class CSetCountDlg : public CDialog
{
// Construction
public:
	CSetCountDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetCountDlg)
	enum { IDD = IDD_DIALOG_SETCOUNT };
	BYTE	m_bCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetCountDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCOUNTDLG_H__1486B4C3_5899_11D4_8A40_005004555789__INCLUDED_)
