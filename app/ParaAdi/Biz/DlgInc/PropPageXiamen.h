#if !defined(AFX_PROPPAGEXIAMEN_H__291C9DE1_D609_4600_B97D_1ECD49600122__INCLUDED_)
#define AFX_PROPPAGEXIAMEN_H__291C9DE1_D609_4600_B97D_1ECD49600122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageXiamen.h : header file
//


#include "stackdialog.h"
#include "comboboxxp.h"
#include "PhoneNumber.h"
#include "BizPhoneSettingDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageXiamen dialog

class CPropPageXiamen : public CStackPage
{
// Construction
public:
	CPropPageXiamen(CWnd* pParent = NULL);   // standard constructor
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);

protected:
	CBizPhoneSettingDlg* m_pParentWnd;
	BOOL m_bShowOriginalPhoneNum;
	BOOL m_bLocalSectEnabled;

public:
// Dialog Data
	//{{AFX_DATA(CPropPageXiamen)
	enum { IDD = IDD_PROPPAGE_D_XIAMEN_PARA };
	CPhoneNumber	m_strProdecerID;
	CPhoneNumber	m_strMscReceiveNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageXiamen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropPageXiamen)
	afx_msg void OnChangeMscreceivenum();
	afx_msg void OnChangeProducerid();
	afx_msg void OnPageRadioShowYes();
	afx_msg void OnPageRadioShowNo();
	virtual BOOL OnInitDialog();
	afx_msg void OnPageEnableYes();
	afx_msg void OnPageEnableNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEXIAMEN_H__291C9DE1_D609_4600_B97D_1ECD49600122__INCLUDED_)
