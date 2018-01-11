#if !defined(AFX_SETWIDTH_H__1486B4C4_5899_11D4_8A40_005004555789__INCLUDED_)
#define AFX_SETWIDTH_H__1486B4C4_5899_11D4_8A40_005004555789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetWidth.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetWidthDlg dialog

class CSetWidthDlg : public CDialog
{
// Construction
public:
	CSetWidthDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetWidthDlg)
	enum { IDD = IDD_DIALOG_SETWIDTH };
	BYTE	m_bWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetWidthDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetWidthDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETWIDTH_H__1486B4C4_5899_11D4_8A40_005004555789__INCLUDED_)
