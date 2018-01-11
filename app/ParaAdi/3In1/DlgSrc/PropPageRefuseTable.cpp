// PropPageRefuseTable.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageRefuseTable.h"

#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageRefuseTable dialog


CPropPageRefuseTable::CPropPageRefuseTable(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageRefuseTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageRefuseTable)
	m_nRefuseType = 0;
	//}}AFX_DATA_INIT
}


void CPropPageRefuseTable::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageRefuseTable)
	DDX_Control(pDX, IDC_PAGE_7_COMBO_REFUSETYPE, m_cbRefuseType);
	DDX_CBIndex(pDX, IDC_PAGE_7_COMBO_REFUSETYPE, m_nRefuseType);
	DDX_Control(pDX, IDC_PAGE_7_REPORT_REFUSETABLE, m_rcRefuseTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageRefuseTable, CStackPage)
	//{{AFX_MSG_MAP(CPropPageRefuseTable)
	ON_CBN_SELCHANGE(IDC_PAGE_7_COMBO_REFUSETYPE, OnSelchangeRefuseType)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_7_REPORT_REFUSETABLE, OnRvnEndItemEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageRefuseTable message handlers

void CPropPageRefuseTable::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 如果edit编辑结束，设置通知更改了数据
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageRefuseTable::OnSelchangeRefuseType() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageRefuseTable::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// 设置底色
	//m_rcRefuseTable.InsertColor(0, 0x00C0D8C0);
	//m_rcRefuseTable.InsertColor(0, 0x00D0C0C0);

	// 增加竖线和底色
	m_rcRefuseTable.ModifyStyle(0, 
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
	// m_rcRefuseTable.SetBkImage(IDB_SIM);

	// 设置ReportCtrl 的edit只能接受数字输入
	// m_rcRefuseTable.SetOnlyDigits(TRUE);

	InitRefuseTable();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageRefuseTable::InitRefuseTable()
{
	// 加入报头
	m_rcRefuseTable.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcRefuseTable.InsertColumn(1, _T("Restricted number header"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								262);

	// 加入列表项
	char pszTemp[3];
	int i = 0;
	for(i=1; i<=NUM_LIMITINCOMING_HEADER_TOTAL; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcRefuseTable.InsertItem(i-1,pszTemp);
	}

	// 更改列表项属性
	for(i=1; i<=NUM_LIMITINCOMING_HEADER_TOTAL; i++)
	{
		RVITEM rvi;
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		
		m_rcRefuseTable.SetItem(&rvi);
	}
}

BOOL CPropPageRefuseTable::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

	int i = 0;
	
	// 1 -- the combo box
	if (m_nRefuseType == -1)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_7_REFUSE_TABLE));
		m_cbRefuseType.SetFocus();
		// 没有做选择
		MessageBox(_T("Please select using of incomming!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// 2 -- the list ctrl
	CPhoneNumber strInput(""),sError;
	for (i=0; i<NUM_LIMITINCOMING_HEADER_TOTAL; i++)
	{
		strInput = m_rcRefuseTable.GetItemText(i,1);
		if (!strInput.IsEmpty())
		{
			if (!strInput.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_7_REFUSE_TABLE));
				m_rcRefuseTable.SetFocus();
				m_rcRefuseTable.SetSelection(i);

				sError.Format(_T("No.%d:Restricted number header must be phone number!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if ( strInput.GetLength()>16)
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_7_REFUSE_TABLE));
				m_rcRefuseTable.SetFocus();
				m_rcRefuseTable.SetSelection(i);

				sError.Format(_T("No.%d:Restricted number header no more than 16 chars!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
		}
	}

	return TRUE;
}

void CPropPageRefuseTable::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	int i = 0;

	// 1 -- the combo box
	m_pParentWnd->szLimitAllIncomingCall5000.Format("0%d", m_nRefuseType);

	// 2 -- the edit list ctrl
	CString strDirectNum("");
	for (int i=0; i<NUM_LIMITINCOMING_HEADER_TOTAL; i++)
	{
		strDirectNum = m_rcRefuseTable.GetItemText(i,1);
		m_pParentWnd->szLimitIncomingCallHead5001[i] = strDirectNum;
	}
}

void CPropPageRefuseTable::OnRefreshPage(void)
{
	// 1 -- the combo box
	m_nRefuseType = atoi(m_pParentWnd->szLimitAllIncomingCall5000);

	// 2 -- the edit list ctrl
	m_rcRefuseTable.DeleteAllItems();

	// 加入列表项
	CString strRefuseNum("");
	char pszTemp[20];
	int i = 0;

	for (i=0; i<NUM_LIMITINCOMING_HEADER_TOTAL; i++)
	{
		_itoa( i+1, pszTemp, 10 );
		m_rcRefuseTable.InsertItem(i,pszTemp);

		// 将得到的string写道listctrl中去
		strRefuseNum = m_pParentWnd->szLimitIncomingCallHead5001[i];
		m_rcRefuseTable.SetItemText(i,1,strRefuseNum);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}
void CPropPageRefuseTable::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_7_COMBO_REFUSETYPE)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_7_REPORT_REFUSETABLE)->EnableWindow(bLock);
}
