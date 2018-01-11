// GridDoc.cpp : implementation of the CGridDoc class
//

#include "stdafx.h"
#include "Grid.h"

#include "PaymentSet.h"
#include "GridDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridDoc

IMPLEMENT_DYNCREATE(CGridDoc, CDocument)

BEGIN_MESSAGE_MAP(CGridDoc, CDocument)
	//{{AFX_MSG_MAP(CGridDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDoc construction/destruction

CGridDoc::CGridDoc()
{
	// TODO: add one-time construction code here

}

CGridDoc::~CGridDoc()
{
	if(m_Set.IsOpen())
		m_Set.Close();
}

BOOL CGridDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGridDoc serialization

void CGridDoc::Serialize(CArchive& ar)
{
}

/////////////////////////////////////////////////////////////////////////////
// CGridDoc diagnostics

#ifdef _DEBUG
void CGridDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGridDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridDoc commands

BOOL CGridDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
/*
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
*/	
	DeleteContents();
	SetModifiedFlag(FALSE);     // start off with unmodified

	CWaitCursor WCur;

	if(m_Set.IsOpen())
		m_Set.Close();

	try
	{
		m_Set.m_strDefaultDBName = lpszPathName;
		m_Set.Open();
	}
	catch (CDaoException* e)
	{
		WCur.Restore();
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	
	CGridApp* pApp = (CGridApp*)AfxGetApp();
	pApp->m_strDBName = lpszPathName;

	WCur.Restore();

	return TRUE;
}
