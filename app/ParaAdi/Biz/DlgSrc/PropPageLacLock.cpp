// PropPageLacLock.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageLacLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageLacLock dialog


CPropPageLacLock::CPropPageLacLock(CWnd* pParent /*=NULL*/)
	: CDialog(CPropPageLacLock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageLacLock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageLacLock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLacLock)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageLacLock, CDialog)
	//{{AFX_MSG_MAP(CPropPageLacLock)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageLacLock message handlers
