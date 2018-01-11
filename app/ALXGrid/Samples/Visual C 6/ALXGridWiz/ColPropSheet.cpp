// ColPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ColPropSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColPropSheet

IMPLEMENT_DYNAMIC(CColPropSheet, CPropertySheet)

CColPropSheet::CColPropSheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION, pWndParent)
{
	m_bEdit = FALSE;

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
}

CColPropSheet::~CColPropSheet()
{
}


BEGIN_MESSAGE_MAP(CColPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CColPropSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColPropSheet message handlers

BOOL CColPropSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	if(!m_bEdit)
	{
		m_Page1.SetModified(TRUE);
	}

	CString strTitle;
	GetWindowText(strTitle);
	strTitle.Format("%s %u",strTitle,m_nCol);
	SetWindowText(strTitle);

	int PropSheetButtons[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };
	CWnd* pWndBtnHlp = GetDlgItem(PropSheetButtons[3]);
	if(pWndBtnHlp != NULL)
	{
		CRect rectHlp, rectBtn;
		pWndBtnHlp->GetWindowRect(rectHlp);
		ScreenToClient(rectHlp); 
		pWndBtnHlp->ModifyStyle(WS_VISIBLE,WS_DISABLED);

		int dx = 0;
		for(int i = 2; i >= 0; i--)
		{
			CWnd* pWndBtn = GetDlgItem(PropSheetButtons[i]);
			if(pWndBtn != NULL)
			{
				pWndBtn->GetWindowRect(rectBtn);
				ScreenToClient(rectBtn); 
				if(dx == 0)
					dx = rectHlp.right - rectBtn.right;
				pWndBtn->MoveWindow(rectBtn.left + dx, rectBtn.top, rectBtn.Width(), rectBtn.Height());
			}
		}
	}

	return bResult;
}

