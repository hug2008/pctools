#if !defined(AFX_PROPPAGELACLOCK_H__17E22E41_D52F_447F_9F55_27C7B458308C__INCLUDED_)
#define AFX_PROPPAGELACLOCK_H__17E22E41_D52F_447F_9F55_27C7B458308C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageLacLock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropPageLacLock dialog

class CPropPageLacLock : public CDialog
{
// Construction
public:
	CPropPageLacLock(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPropPageLacLock)
	enum { IDD = IDD_PROPPAGE_10_LAC_LOCK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageLacLock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageLacLock)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGELACLOCK_H__17E22E41_D52F_447F_9F55_27C7B458308C__INCLUDED_)
