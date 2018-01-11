// PropPageLocalUserMobileSect.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageLocalUserMobileSect.h"

#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalUserMobileSect dialog


CPropPageLocalUserMobileSect::CPropPageLocalUserMobileSect(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLocalUserMobileSect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageLocalUserMobileSect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageLocalUserMobileSect::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLocalUserMobileSect)
	DDX_Control(pDX, IDC_PAGE_6_REPORT_USER_LOCALTABLE, m_rcUserLocalTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageLocalUserMobileSect, CStackPage)
	//{{AFX_MSG_MAP(CPropPageLocalUserMobileSect)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_6_REPORT_USER_LOCALTABLE, OnRvnEndItemEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageLocalUserMobileSect message handlers

void CPropPageLocalUserMobileSect::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 如果edit编辑结束，设置通知更改了数据
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageLocalUserMobileSect::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// 增加竖线和底色
	m_rcUserLocalTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT
//#ifndef DISABLE_INPUT
		| RVS_FOCUSSUBITEMS
//#endif
		);
	
	// 资源ID，或者资源名称。即：IDR_JPG_SPLASH或者153
	// m_rcUserLocalTable.SetBkImage(IDB_SIM);

	// 设置listctrl 的edit只能接受数字输入
	// m_rcUserLocalTable.SetOnlyDigits(TRUE);

	InitLocalUserMobileTable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageLocalUserMobileSect::InitLocalUserMobileTable()
{
	// 加入报头
	m_rcUserLocalTable.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcUserLocalTable.InsertColumn(1, _T("User-defined local mobile number header"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								260);

	// 加入列表项
	char pszTemp[3];
	int i = 0;
	for(i=1; i<=INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcUserLocalTable.InsertItem(i-1,pszTemp);
	}

	// 更改列表项属性
	for(i=1; i<=INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		RVITEM rvi;
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		
		m_rcUserLocalTable.SetItem(&rvi);
	}
}

BOOL CPropPageLocalUserMobileSect::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);
	
	// 1 -- the list ctrl
	CPhoneNumber strInput(""),sError("");
	for (int i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		strInput = m_rcUserLocalTable.GetItemText(i,1);
		if (!strInput.IsEmpty())
		{
			if (!strInput.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_5_LOCAL_MOBILESECT);
				m_rcUserLocalTable.SetFocus();
				m_rcUserLocalTable.SetSelection(i);

				sError.Format(_T("No.%d :[%s] is not a correct phone number string!") ,i+1, m_rcUserLocalTable.GetItemText(i,1));
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if ( strInput.GetLength()>16)
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_5_LOCAL_MOBILESECT);
				m_rcUserLocalTable.SetFocus();
				m_rcUserLocalTable.SetSelection(i);

				sError.Format(_T("No.%d :header is longer than 16!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
		}
	}

	return TRUE;
}

void CPropPageLocalUserMobileSect::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the edit list ctrl
	CString strUserLocalNum("");
	for (int i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		strUserLocalNum = m_rcUserLocalTable.GetItemText(i,1);
		m_pParentWnd->m_ArySECT_4420_strUserLocalTable[i] = strUserLocalNum;
	}
}

void CPropPageLocalUserMobileSect::OnRefreshPage(void)
{
	// 1 -- the edit list ctrl
	m_rcUserLocalTable.DeleteAllItems();

	// 加入列表项
	CString strUserLocalNum("");
	char pszTemp[20];

	for (int i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		_itoa( i+1, pszTemp, 10 );
		m_rcUserLocalTable.InsertItem(i,pszTemp);

		// 将得到的string写道listctrl中去
		strUserLocalNum = m_pParentWnd->m_ArySECT_4420_strUserLocalTable[i];
		m_rcUserLocalTable.SetItemText(i,1,strUserLocalNum);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}

void CPropPageLocalUserMobileSect::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_6_REPORT_USER_LOCALTABLE)->EnableWindow(bLock);
}

