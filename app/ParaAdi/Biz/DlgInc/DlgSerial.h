#if !defined(AFX_DLGSERIAL_H__FD83ADEF_DF3D_4DE1_A375_757A0EEF1597__INCLUDED_)
#define AFX_DLGSERIAL_H__FD83ADEF_DF3D_4DE1_A375_757A0EEF1597__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSerial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSerial dialog

class CDlgSerial : public CDialog
{
// Construction
public:
	CDlgSerial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSerial)
	enum { IDD = IDD_DLG_SERIAL };
	CString	m_strInputedSerial;
	CString	m_strCustomID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSerial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSerial)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSERIAL_H__FD83ADEF_DF3D_4DE1_A375_757A0EEF1597__INCLUDED_)
