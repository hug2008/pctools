// PaymentSet.cpp : implementation of the CPaymentSet class
//

#include "stdafx.h"
#include "Grid.h"
#include "PaymentSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaymentSet implementation

IMPLEMENT_DYNAMIC(CPaymentSet, CDaoRecordset)

CPaymentSet::CPaymentSet(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPaymentSet)
	m_CODE = 0;
	m_DATE = (DATE)0;
	m_NAME = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}

CString CPaymentSet::GetDefaultDBName()
{
	return m_strDefaultDBName;
}


CString CPaymentSet::GetDefaultSQL()
{
	return _T("[TPayment]");
}

void CPaymentSet::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPaymentSet)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[CODE]"), m_CODE);
	DFX_DateTime(pFX, _T("[DATE]"), m_DATE);
	DFX_Text(pFX, _T("[NAME]"), m_NAME);
	DFX_Currency(pFX, _T("[PAYMENT_CHANGED]"), m_PAYMENT_CHANGED);
	DFX_Currency(pFX, _T("[IS_PAID]"), m_IS_PAID);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPaymentSet diagnostics

#ifdef _DEBUG
void CPaymentSet::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CPaymentSet::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
