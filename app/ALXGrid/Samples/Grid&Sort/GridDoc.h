// GridDoc.h : interface of the CGridDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDDOC_H__218AE4E9_1926_11D5_8A50_000102CA073A__INCLUDED_)
#define AFX_GRIDDOC_H__218AE4E9_1926_11D5_8A50_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PaymentSet.h"


class CGridDoc : public CDocument
{
protected: // create from serialization only
	CGridDoc();
	DECLARE_DYNCREATE(CGridDoc)

// Attributes
public:
	CPaymentSet m_Set;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
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

#endif // !defined(AFX_GRIDDOC_H__218AE4E9_1926_11D5_8A50_000102CA073A__INCLUDED_)
