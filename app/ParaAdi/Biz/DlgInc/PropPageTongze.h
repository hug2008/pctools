#if !defined(AFX_PROPPAGETONGZE_H__A32DFAFB_B261_48D5_B2D0_A1C212956F33__INCLUDED_)
#define AFX_PROPPAGETONGZE_H__A32DFAFB_B261_48D5_B2D0_A1C212956F33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageTongze.h : header file
//

#include "stackdialog.h"
#include "PhoneNumber.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageTongze dialog

class CPropPageTongze : public CStackPage
{
// Construction
public:
	CPropPageTongze(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	BOOL m_bShowOriginalPhoneNum;
	BOOL m_bLocalSectEnabled;

public:
// Dialog Data
	//{{AFX_DATA(CPropPageTongze)
	enum { IDD = IDD_PROPPAGE_F_TONGZE };
	CPhoneNumber	m_strProdecerID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageTongze)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageTongze)
	afx_msg void OnChangeProducerid();
	virtual BOOL OnInitDialog();
	afx_msg void OnPageEnableYes();
	afx_msg void OnPageEnableNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGETONGZE_H__A32DFAFB_B261_48D5_B2D0_A1C212956F33__INCLUDED_)
