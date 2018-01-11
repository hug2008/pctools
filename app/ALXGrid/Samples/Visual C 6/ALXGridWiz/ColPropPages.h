// ColPropPages.h : header file
//

#ifndef __COLPROPPAGES_H__
#define __COLPROPPAGES_H__

/////////////////////////////////////////////////////////////////////////////
// CGeneralPropPage dialog

class CGeneralPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPropPage)

// Construction
public:
	CGeneralPropPage();
	~CGeneralPropPage();

// Dialog Data
	//{{AFX_DATA(CGeneralPropPage)
	enum { IDD = IDD_PROPPAGE_GENERAL };
	CString	m_strTitle;
	CString	m_strID;
	int		m_nWidth;
	//}}AFX_DATA

	CWnd* m_pWnd;
	BOOL m_bValidate;
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPropPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DDV_IDName(CDataExchange* pDX, CString& s);
	// Generated message map functions
	//{{AFX_MSG(CGeneralPropPage)
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CAlignPropPage dialog

class CAlignPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAlignPropPage)

// Construction
public:
	CAlignPropPage();
	~CAlignPropPage();

// Dialog Data
	//{{AFX_DATA(CAlignPropPage)
	enum { IDD = IDD_PROPPAGE_ALIGN };
	int		m_nCAlign;
	int		m_nHAlign;
	//}}AFX_DATA

	CWnd* m_pWnd;
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAlignPropPage)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAlignPropPage)
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CCtrlPropPage dialog

class CCtrlPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCtrlPropPage)

// Construction
public:
	CCtrlPropPage();
	~CCtrlPropPage();

// Dialog Data
	//{{AFX_DATA(CCtrlPropPage)
	enum { IDD = IDD_PROPPAGE_CONTROL };
	int		m_nCtrl;
	int		m_nEAlign;
	int		m_nCHState;
	int		m_nCBDrop;
	//}}AFX_DATA

	CWnd* m_pWnd;
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCtrlPropPage)
	public:
	virtual void OnOK();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCtrlPropPage)
	afx_msg void OnRadioCtrl1();
	afx_msg void OnRadioCtrl2();
	afx_msg void OnRadioCtrl3();
	afx_msg void OnRadioCtrl4();
	virtual BOOL OnInitDialog();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};



#endif // __COLPROPPAGES_H__
