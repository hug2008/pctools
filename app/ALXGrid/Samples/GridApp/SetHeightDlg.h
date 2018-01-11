#if !defined(AFX_SETHEIGHT_H__1486B4C4_5899_11D4_8A40_005004555789__INCLUDED_)
#define AFX_SETHEIGHT_H__1486B4C4_5899_11D4_8A40_005004555789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetHeight.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetHeightDlg dialog

class CSetHeightDlg : public CDialog
{
// Construction
public:
	CSetHeightDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetHeightDlg)
	enum { IDD = IDD_DIALOG_SETHEIGHT };
	BYTE	m_bHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetHeightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetHeightDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETHEIGHT_H__1486B4C4_5899_11D4_8A40_005004555789__INCLUDED_)
