// GridDoc.cpp : implementation of the CGridDoc class
//

#include "stdafx.h"
#include "Grid.h"

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
	m_Rows.RemoveAll();
}

BOOL CGridDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_Rows.RemoveAll();

	ROWINFO RowInfo;

	for(int i = 0; i < 100; i++)
	{
		RowInfo.m_LeftText.Format("0 - %u",i);
		RowInfo.m_CenterText.Format("1 - %u",i);
		RowInfo.m_RightText.Format("2 - %u",i);
		RowInfo.m_BitmapText.Format("3 - %u",i);
		switch(i%3)
		{
			case 0:
			{
				RowInfo.m_CheckBox3State = partial;
				break;
			}
			case 1:
			{
				RowInfo.m_CheckBox3State = check;
				break;
			}
			default:
				RowInfo.m_CheckBox3State = uncheck;
		}
		if(i%5 == 0) 
			RowInfo.m_CheckBox = uncheck;
		else
			RowInfo.m_CheckBox = check;

		RowInfo.m_CheckBox3StateText.m_Text.Format("6 - %u",i);
		if(i%7 == 0) 
			RowInfo.m_CheckBox3StateText.m_State = uncheck;
		else
			if(i%4 == 0) 
				RowInfo.m_CheckBox3StateText.m_State = partial;
			else
				RowInfo.m_CheckBox3StateText.m_State = check;

		RowInfo.m_CheckBoxText.m_Text.Format("7 - %u",i);
		if(i%11 == 0) 
			RowInfo.m_CheckBoxText.m_State = uncheck;
		else
			RowInfo.m_CheckBoxText.m_State = check;

		RowInfo.m_HideDropDownComboBox = "One";
		RowInfo.m_DropDownComboBox = "First";
		RowInfo.m_HideListComboBox = first;
		RowInfo.m_ListComboBox = first;

		m_Rows.Add(RowInfo);
	}
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGridDoc serialization

void CGridDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
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

void CGridDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::OnCloseDocument();
}
