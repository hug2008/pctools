#if !defined(AFX_SELCOMPORT_H__FD575471_BC39_4025_90A6_AEB43B1B476B__INCLUDED_)
#define AFX_SELCOMPORT_H__FD575471_BC39_4025_90A6_AEB43B1B476B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelComPort.h : header file
//

#include "buttonxp.h"
#include "comboboxxp.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CSelComPort dialog

class CSelComPort : public CDialog
{
// Construction
public:
	CSelComPort(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelComPort)
	enum { IDD = IDD_DLG_SELCOMPORT };
	CComboBoxXP	m_cbPort;
	CComboBoxXP	m_cbBaut;
	CString	m_strBaut;
	CString	m_ComPort;
	CButtonXP	m_btOK;
	CButtonXP	m_btCancel;
	CString m_szEnterUser;
	CString m_szEnterPassW;
	CComboBoxXP m_cbMode;
	int m_cbCurMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelComPort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelComPort)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	HICON m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELCOMPORT_H__FD575471_BC39_4025_90A6_AEB43B1B476B__INCLUDED_)
