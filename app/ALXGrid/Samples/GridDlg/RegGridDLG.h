#if !defined(AFX_REGGRIDDLG_H__4BDAB1A1_570A_11D4_8A40_005004555789__INCLUDED_)
#define AFX_REGGRIDDLG_H__4BDAB1A1_570A_11D4_8A40_005004555789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegGridDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegGridDLG dialog

class CRegGridDLG : public CDialog
{
// Construction
public:
	CRegGridDLG(CWnd* pParent = NULL);   // standard constructor
	~CRegGridDLG();

// Dialog Data
	//{{AFX_DATA(CRegGridDLG)
	enum { IDD = IDD_REGGRID_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegGridDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegGridDLG)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGGRIDDLG_H__4BDAB1A1_570A_11D4_8A40_005004555789__INCLUDED_)
