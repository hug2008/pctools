// GridDoc.h : interface of the CGridDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDDOC_H__542018CD_5037_11D4_B300_D23119F72142__INCLUDED_)
#define AFX_GRIDDOC_H__542018CD_5037_11D4_B300_D23119F72142__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>

typedef enum state { uncheck = 0, check = 1, partial = 2 } STATE;
typedef enum order { first = 1, second = 2, third = 3 } ORDER;
typedef struct StateText
	{
		STATE	m_State;
		CString m_Text;
	} STATE_TEXT;

typedef struct RowInfo
{
	CString m_LeftText;
	CString m_CenterText;
	CString m_RightText;
	CString m_BitmapText;
	STATE	m_CheckBox3State;
	STATE	m_CheckBox;
	STATE_TEXT m_CheckBox3StateText;
	STATE_TEXT m_CheckBoxText;
	CString	m_HideDropDownComboBox;
	CString	m_DropDownComboBox;
	ORDER	m_HideListComboBox;
	ORDER	m_ListComboBox;
	DWORD	m_nCustomCtrl;

} ROWINFO;

class CGridDoc : public CDocument
{
protected: // create from serialization only
	CGridDoc();
	DECLARE_DYNCREATE(CGridDoc)

// Attributes
public:
	CArray < ROWINFO, ROWINFO& > m_Rows;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGridDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDOC_H__542018CD_5037_11D4_B300_D23119F72142__INCLUDED_)
