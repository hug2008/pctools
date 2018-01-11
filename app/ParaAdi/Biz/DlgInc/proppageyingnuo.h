#if !defined(AFX_PROPPAGEYINGNUO_H__CA819253_E307_4F14_84B4_ADF92C9EF85A__INCLUDED_)
#define AFX_PROPPAGEYINGNUO_H__CA819253_E307_4F14_84B4_ADF92C9EF85A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// proppageyingnuo.h : header file
//

#include "buttonxp.h"
#include "stackdialog.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageYingnuo dialog

class CPropPageYingnuo : public CStackPage
{
// Construction
public:
	CPropPageYingnuo(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	BYTE ConvertHexStringToInt8(char ch1, char ch2);
	void ImportInnoString(char* pBuffer);

public:
// Dialog Data
	//{{AFX_DATA(CPropPageYingnuo)
	enum { IDD = IDD_PROPPAGE_E_YINGNUO };
	BYTE	m_bSec[6][17];
	CButtonXP	m_btImport;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageYingnuo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageYingnuo)
	virtual BOOL OnInitDialog();
	afx_msg void OnImportSecurity();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEYINGNUO_H__CA819253_E307_4F14_84B4_ADF92C9EF85A__INCLUDED_)
/*
[
[
[
[
[
[
[
[
[
[
[
[
[
[
[
[

]
]
]
]
]
]
]
]
]
]
]
]
]
]
]
]

Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
Sec
*/
