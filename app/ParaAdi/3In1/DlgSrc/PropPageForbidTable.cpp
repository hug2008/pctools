// PropPageForbidTable.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageForbidTable.h"

#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageForbidTable dialog


CPropPageForbidTable::CPropPageForbidTable(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageForbidTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageForbidTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageForbidTable::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageForbidTable)
	DDX_Control(pDX, IDC_PAGE_4_REPORT_FORBIDTABLE, m_rcForbidTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageForbidTable, CStackPage)
	//{{AFX_MSG_MAP(CPropPageForbidTable)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_4_REPORT_FORBIDTABLE, OnRvnEndItemEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageForbidTable message handlers

BOOL CPropPageForbidTable::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// ���õ�ɫ
	// m_rcForbidTable.InsertColor(0, 0x00C0D8C0);
	// m_rcForbidTable.InsertColor(0, 0x00D0C0C0);

	// �������ߺ͵�ɫ
	m_rcForbidTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT
//#ifndef DISABLE_INPUT
		| RVS_FOCUSSUBITEMS
//#endif
		);
	
	// ��ԴID��������Դ���ơ�����IDR_JPG_SPLASH����153
	// m_rcForbidTable.SetBkImage(IDB_SIM);

	// ��ʼ��ReportCtrl��Column��ͷ�Ϳյ��б���
	InitForbidTable();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageForbidTable::InitForbidTable()
{
	// ���뱨ͷ
	m_rcForbidTable.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcForbidTable.InsertColumn(1, _T("Forbidden Number Header"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								262);

	// �����б���
	char pszTemp[3];
	int i = 0;
	for(i=1; i<=NUM_BARRING_NUMBER_TOTAL; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcForbidTable.InsertItem(i-1,pszTemp);
	}

	// �����б�������
	for(i=1; i<=NUM_BARRING_NUMBER_TOTAL; i++)
	{
		RVITEM rvi;
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		
		m_rcForbidTable.SetItem(&rvi);
	}
}

BOOL CPropPageForbidTable::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);
	
	// 1 -- the list ctrl
	CPhoneNumber strInput(""), sError("");
	for (int i=0; i<NUM_BARRING_NUMBER_TOTAL; i++)
	{
		strInput = m_rcForbidTable.GetItemText(i,1);
		if (!strInput.IsEmpty())
		{
			if (!strInput.IsPhoneString())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_4_FORBID_TABLE));
				m_rcForbidTable.SetFocus();
				m_rcForbidTable.SetSelection(i);

				sError.Format(_T("No.%d :[%s] is not a correct phone number string!") ,i+1, m_rcForbidTable.GetItemText(i,1));
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if ( strInput.GetLength()>16)
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_4_FORBID_TABLE));
				m_rcForbidTable.SetFocus();
				m_rcForbidTable.SetSelection(i);

				sError.Format(_T("No.%d :forbidden number header is longer than 16!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
		}
	}

	return TRUE;
}

void CPropPageForbidTable::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ���edit�༭����������֪ͨ����������
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageForbidTable::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the edit list ctrl
	CString strDirectNum("");
	for (int i=0; i<NUM_BARRING_NUMBER_TOTAL; i++)
	{
		strDirectNum = m_rcForbidTable.GetItemText(i,1);
		m_pParentWnd->szBarringCallNum3001[i] = strDirectNum;
	}
}

void CPropPageForbidTable::OnRefreshPage(void)
{
	// 1 -- the edit list ctrl
	m_rcForbidTable.DeleteAllItems();

	// �����б���
	CString strForbidNum("");
	char pszTemp[20];

	for (int i=0; i<NUM_BARRING_NUMBER_TOTAL; i++)
	{
		_itoa( i+1, pszTemp, 10 );
		m_rcForbidTable.InsertItem(i,pszTemp);

		// ���õ���stringд��listctrl��ȥ
		strForbidNum = m_pParentWnd->szBarringCallNum3001[i];
		m_rcForbidTable.SetItemText(i,1,strForbidNum);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}

void CPropPageForbidTable::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_4_REPORT_FORBIDTABLE)->EnableWindow(bLock);
}
