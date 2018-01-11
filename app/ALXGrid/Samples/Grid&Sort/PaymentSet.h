// PaymentSet.h : interface of the CPaymentSet class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAYMENTSET_H__218AE4ED_1926_11D5_8A50_000102CA073A__INCLUDED_)
#define AFX_PAYMENTSET_H__218AE4ED_1926_11D5_8A50_000102CA073A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPaymentSet : public CDaoRecordset
{
public:
	CPaymentSet(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPaymentSet)

	CString m_strDefaultDBName;

// Field/Param Data
	//{{AFX_FIELD(CPaymentSet, CDaoRecordset)
	long			m_CODE;
	COleDateTime	m_DATE;
	CString			m_NAME;
	COleCurrency	m_PAYMENT_CHANGED;
	COleCurrency	m_IS_PAID;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaymentSet)
	public:
	virtual CString GetDefaultDBName();		// REVIEW:  Get a comment here
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);	// RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAYMENTSET_H__218AE4ED_1926_11D5_8A50_000102CA073A__INCLUDED_)

