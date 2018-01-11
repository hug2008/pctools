// PropPageOutput.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageOutput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageOutput dialog


CPropPageOutput::CPropPageOutput(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageOutput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageOutput)
	//}}AFX_DATA_INIT
}


void CPropPageOutput::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOutput)
	DDX_Control(pDX, IDC_PAGE_X_REPORT_OUTPUT, m_rcOutput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropPageOutput, CStackPage)
	//{{AFX_MSG_MAP(CPropPageOutput)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, OnOutputOperationMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageOutput message handlers

BOOL CPropPageOutput::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// 增加竖线和底色
	m_rcOutput.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_SHOWSELALWAYS|
		RVS_NOSORT
//		RVS_FOCUSSUBITEMS
		);
	
	// 资源ID，或者资源名称。即：IDR_JPG_SPLASH或者153
	// m_rcOutput.SetBkImage(IDB_SIM);

	InitLogOutputList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageOutput::InitLogOutputList()
{
	// 加入报头
	m_rcOutput.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								40);

	m_rcOutput.InsertColumn(1, _T("Sta."), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								40);

	m_rcOutput.InsertColumn(2, _T("Info"), 
								RVCF_LEFT|
								RVCF_TEXT,
								200);

	m_rcOutput.InsertColumn(3, _T("Time"), 
								RVCF_CENTER|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								60);

	m_rcOutput.InsertColumn(4, _T("Date"), 
								RVCF_CENTER|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								80);
}

LRESULT CPropPageOutput::OnOutputOperationMessage(WPARAM wParam, LPARAM lParam)
{
	BOOL bNoError = (BOOL)wParam;
	CString* pMsgString = (CString*)lParam;

	if (pMsgString->IsEmpty()) return 0;

	int nCount = m_rcOutput.GetItemCount();

	char pszNO[6];
	_itoa( nCount+1, pszNO, 10 );

	CString strTime(""), strDate("");
	strTime = COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S"));
	strDate = COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y"));

	// 状态 为 正常或错误
	CString strState("");
	strState = (bNoError)? _T("OK"):  _T("ERR");

	m_rcOutput.InsertItem(nCount, pszNO);
	m_rcOutput.SetItemText(nCount, 1, strState);
	m_rcOutput.SetItemText(nCount, 2, *pMsgString);
	m_rcOutput.SetItemText(nCount, 3, strTime);
	m_rcOutput.SetItemText(nCount, 4, strDate);

	m_rcOutput.SetSelection(nCount, TRUE);
	m_rcOutput.EnsureVisible(nCount, FALSE);

	// 如果list过长，清除一部分
	if (nCount>50000)
	{
		for (int i=0; i< 10000; i++)
			m_rcOutput.DeleteItem(0);
	}

	if (!bNoError)
		if (*pMsgString != _T("Fail to open comm!"))
		{
			*pMsgString = _T("Fail:") + *pMsgString;
			MessageBox(*pMsgString, SYS_NAME, MB_ICONERROR | MB_OK);
		}

	delete pMsgString;
	pMsgString = NULL;
	return 0;
}
