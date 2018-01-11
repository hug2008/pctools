// CPropPageDirectTable.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageDirectTable.h"

#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropPageDirectTable dialog


CPropPageDirectTable::CPropPageDirectTable(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageDirectTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageDirectTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageDirectTable::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageDirectTable)
	DDX_Control(pDX, IDC_PAGE_3_REPORT_DIRECTTABLE, m_rcDirectTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageDirectTable, CStackPage)
	//{{AFX_MSG_MAP(CPropPageDirectTable)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_3_REPORT_DIRECTTABLE, OnRvnEndItemEdit)
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_MSG_EDIT_END_NOTIFY, OnEditEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageDirectTable message handlers

void CPropPageDirectTable::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 如果edit编辑结束，设置通知更改了数据
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageDirectTable::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	// ================================================================
	// Set features and content of Direct number infomation ReportCtrl
	
	// 设置底色
	//m_rcDirectTable.InsertColor(0, 0x00C0D8C0);
	//m_rcDirectTable.InsertColor(0, 0x00D0C0C0);

	//m_rcDirectTable.InsertColor(0, 0x00C0D8C0);
	//m_rcDirectTable.InsertColor(0, ::GetSysColor(COLOR_GRAYTEXT));
	//m_rcDirectTable.InsertColor(0, 0x00D0C0C0);
	//m_rcDirectTable.InsertColor(0, 0x00804000);
	
	// 增加竖线和底色
	m_rcDirectTable.ModifyStyle(0, 
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
	// m_rcDirectTable.SetBkImage(IDB_SIM);

	// 初始化ReportCtrl的Column报头和空的列表项
	InitDirectTable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageDirectTable::InitDirectTable()
{
	// 加入列表项
	char pszTemp[3];
	int i = 0;

	// 加入报头
	m_rcDirectTable.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcDirectTable.InsertColumn(1, _T("Hot number header"),// LVCFMT_LEFT, 234);
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								262);

	
	for(i=1; i<=INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcDirectTable.InsertItem(i-1,pszTemp);
	}

	// 更改列表项属性
	for(i=1; i<=INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		RVITEM rvi;
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcDirectTable.SetItem(&rvi);
	}
}

BOOL CPropPageDirectTable::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);
	
	// 1 -- the list ctrl
	CPhoneNumber strInput("");
	CString sError("");
	for (int i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		strInput = m_rcDirectTable.GetItemText(i,1);
		if (!strInput.IsEmpty())
		{
			if (!strInput.IsPhoneString())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_3_DIRECT_TABLE);
				m_rcDirectTable.SetFocus();
				m_rcDirectTable.SetSelection(i);

				sError.Format(_T("No.%d :[%s] is not a correct phone number string!") ,i+1, m_rcDirectTable.GetItemText(i,1));
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if ( strInput.GetLength()>16)
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_3_DIRECT_TABLE);
				m_rcDirectTable.SetFocus();
				m_rcDirectTable.SetSelection(i);
				sError.Format(_T("No.%d :hot number header is longer than 16!"),i+1);
				MessageBox(sError,SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
		}
	}

	return TRUE;
}

void CPropPageDirectTable::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the edit list ctrl
	CString strDirectNum("");
	for (int i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		strDirectNum = m_rcDirectTable.GetItemText(i,1);
		m_pParentWnd->m_AryDIRECT_2020_strDirectTable[i] = strDirectNum;
	}
}

void CPropPageDirectTable::OnRefreshPage(void)
{
	// 1 -- the edit list ctrl
	m_rcDirectTable.DeleteAllItems();

	// 加入列表项
	CString strDirectNum("");
	char pszTemp[20];

	for (int i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		_itoa( i+1, pszTemp, 10 );
		m_rcDirectTable.InsertItem(i,pszTemp);

		// 将得到的string写道listctrl中去
		strDirectNum = m_pParentWnd->m_AryDIRECT_2020_strDirectTable[i];
		m_rcDirectTable.SetItemText(i,1,strDirectNum);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}

void CPropPageDirectTable::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_3_REPORT_DIRECTTABLE)->EnableWindow(bLock);
}
