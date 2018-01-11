// PropPageIP.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageIP.h"

#include "phonenumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageIP dialog


CPropPageIP::CPropPageIP(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageIP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageIP::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageIP)
	DDX_Control(pDX, IDC_PAGE_A_REPORT_IPTABLE, m_rcIPRuleTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageIP, CStackPage)
	//{{AFX_MSG_MAP(CPropPageIP)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_A_REPORT_IPTABLE, OnRvnEndItemEdit)
	ON_NOTIFY(RVN_ITEMCLICK, IDC_PAGE_A_REPORT_IPTABLE, OnRvnItemClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageIP message handlers

void CPropPageIP::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ���edit�༭����������֪ͨ����������
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageIP::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// �������ߺ͵�ɫ
	m_rcIPRuleTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT
//#ifndef DISABLE_INPUT
		| RVS_FOCUSSUBITEMS
//#endif
		);

	// ����listctrl ��editֻ�ܽ�����������
	// m_rcIPRuleTable.SetOnlyDigits(TRUE);

	InitIPRuleTable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageIP::InitIPRuleTable()
{
	// ���뱨ͷ
	m_rcIPRuleTable.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcIPRuleTable.InsertColumn(1, _T("IP prefix"),//_T("IP������"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								95);

	m_rcIPRuleTable.InsertColumn(2, _T("IP header"),//_T("IP������"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								85);

	m_rcIPRuleTable.InsertColumn(3, _T("Show header"),//_T("��ʾIPͷ"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH
								);

	m_rcIPRuleTable.InsertColumn(4, _T("cut prefix len"),//_T("�س�λ��"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH
								);

	// �����б���
	char pszTemp[3];
	int i = 0;
	for(i=1; i<=INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcIPRuleTable.InsertItem(i-1,pszTemp);
	}

	// �����б�������
	for(i=1; i<=INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		RVITEM rvi;
		rvi.iItem = i-1;
		rvi.nMask = RVIM_TEXT;

		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcIPRuleTable.SetItem(&rvi);

		rvi.iSubItem = 2;
		rvi.lpszText = _T("");
		m_rcIPRuleTable.SetItem(&rvi);

		rvi.iSubItem = 4;
		rvi.lpszText = _T("");
		m_rcIPRuleTable.SetItem(&rvi);

		rvi.nMask = RVIM_CHECK;//|RVIM_TEXT;
		rvi.iSubItem = 3;
		rvi.iCheck = 0;
		m_rcIPRuleTable.SetItem(&rvi);
	}
}

void CPropPageIP::OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMREPORTVIEW lpnmrv = (LPNMREPORTVIEW)pNMHDR;

	if(lpnmrv->iItem>=0 && lpnmrv->iSubItem>=0)
	{
		RVITEM rvi;
		rvi.iItem = lpnmrv->iItem;
		rvi.iSubItem = lpnmrv->iSubItem;
		m_rcIPRuleTable.GetItem(&rvi);

		if(rvi.nMask&RVIM_CHECK && lpnmrv->nFlags&RVHT_ONITEMCHECK)
		{
//#ifndef DISABLE_INPUT
			rvi.iCheck = rvi.iCheck ? 0:1;
//#endif
			// ���Check״̬�����ģ�����֪ͨ����������
			m_pParentWnd->m_bDataChanged = TRUE;
		}
		m_rcIPRuleTable.SetItem(&rvi);
	}

	*pResult = FALSE;
}

BOOL CPropPageIP::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);
	
	// 1 -- the list ctrl
	// ûʲô����У��� --- ���˳���
	CString strIPRule("");
	for (int i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		CPhoneNumber strDaoRuCode(""), strJieRuCode(""), strShowIPHead(""), strCutLength("");
		CString sError("");

		strDaoRuCode  = m_rcIPRuleTable.GetItemText(i,1);
		strJieRuCode  = m_rcIPRuleTable.GetItemText(i,2);
//		strShowIPHead = m_rcIPRuleTable.GetItemCheck(i,3) ? "#" : "*" ;
		strCutLength  = m_rcIPRuleTable.GetItemText(i,4);

		// ����Checkû�б�ѡ����״̬����"0"�����Բ�����ȫ���ж���ȥ
		strIPRule = strDaoRuCode + strJieRuCode + strCutLength;

		if (!strIPRule.IsEmpty())
		{
			if (!strDaoRuCode.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("No.%d:the preifx is illeagal, try it again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if (strDaoRuCode == "")
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("NO.%d:the prefix could be empty!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if (strDaoRuCode.GetLength() >12 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("No.%d:the prefix could not be longer than 12 chars!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}

			/*
			if (strJieRuCode == "")
			{
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);
				MessageBox(_T("�����IP���Ž����벻��Ϊ�գ�"),SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}
			*/  //IP���Ž��������Ϊ�գ�

			if (!strJieRuCode.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("No.%d:the IP header is illeagal, try it again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if (strJieRuCode.GetLength() >8 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("No.%d:the IP header could not be longer than 8 chars!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}

			/*
			if ((strShowIPHead != "1")&&(strShowIPHead != "0")&&(strShowIPHead != ""))
			{
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);
				MessageBox(_T("�������Ƿ���ʾIPͷ��0����1���ǣ�"),SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}*/
			
			if (!strCutLength.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("No.%d:the cut prefix len is illeagal, try it again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
			if (atoi(strCutLength)>12 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("No.%d:the cut prefix len is no more than 8!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}

			if ( strIPRule.GetLength()>24-1)	// ��һλ����ΪstrShowIPHead  // ��Ϊ�˴�strCutLengthҪ��ռһλ
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_A_IP);
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				sError.Format(_T("No.%d:Rule no more than 24 chars!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);

				return FALSE;
			}
		}
	}
	return TRUE;
}

void CPropPageIP::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	// 1 -- the edit list ctrl
	CStringArray strSortedCodeAry;
	strSortedCodeAry.RemoveAll();

	CString strIPRule("");
	int nCount = 0;
	int i = 0;
	for (i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		CString strDaoRuCode(""), strJieRuCode(""), strShowIPHead(""), strCutLength("");
		strDaoRuCode  = m_rcIPRuleTable.GetItemText(i,1);
		strJieRuCode  = m_rcIPRuleTable.GetItemText(i,2);
		strShowIPHead = m_rcIPRuleTable.GetItemCheck(i,3) ? "#" : "*" ;
//		strCutLength  = "#" + (m_rcIPRuleTable.GetItemText(i,4)=="" ? 
//			"0" : m_rcIPRuleTable.GetItemText(i,4)); 
		strCutLength += "#";
		strCutLength += (m_rcIPRuleTable.GetItemText(i,4)==""?"0":m_rcIPRuleTable.GetItemText(i,4)); 

		if (strDaoRuCode == "" )//||(strJieRuCode == ""))
			strIPRule = "";
		else
		{
			strIPRule = strDaoRuCode + strShowIPHead + strJieRuCode + strCutLength;
			ASSERT(!strIPRule.IsEmpty());
			strSortedCodeAry.Add(strIPRule);
		}
	}

	// ���濪ʼ��strSortedCodeAry�е��ַ�����ʼ����
	nCount = strSortedCodeAry.GetSize();
	DoIPRuleSort(strSortedCodeAry);

	// now they were sorted
	for (i=0;i<INT_INIT_LISTBOX_ITEMNUM;i++)
	{
		if (i<nCount)
			m_pParentWnd->m_AryIP_9001_strIPRuleTable[i] = strSortedCodeAry[i];
		else
			m_pParentWnd->m_AryIP_9001_strIPRuleTable[i] = "";
	}
}

int CPropPageIP::GetDaoRuCodeLength(CString string)
{
	int nFirstStop = string.Find('*');
	if (nFirstStop == -1)
		nFirstStop = string.Find('#');
	
	ASSERT(nFirstStop!=-1);
	
	return (string.Left(nFirstStop)).GetLength();
}

void CPropPageIP::DoIPRuleSort(CStringArray &StrAry)
{
	int nCount = StrAry.GetSize();
	int len1,len2, i = 0;
	BOOL bOperation;
	do
	{
		bOperation = FALSE;
		for (i=0; i< nCount-1; i++)
		{
			len1 = GetDaoRuCodeLength(StrAry[i]);
			len2 = GetDaoRuCodeLength(StrAry[i+1]);

			if (len1<len2)
			{
				// now SWAP this two string
				CString strTemp = StrAry[i];
				StrAry[i] = StrAry[i+1];
				StrAry[i+1] = strTemp;
				bOperation = TRUE;
			}
		}
	}
	while (bOperation == TRUE);
}

void CPropPageIP::OnRefreshPage(void)
{
	m_rcIPRuleTable.DeleteAllItems();

	// 1 -- the edit list ctrl
	CStringArray strSortedCodeAry;
	strSortedCodeAry.RemoveAll();

	CString strIPRule("");
	char pszTemp[20];
	int i = 0;
	for (i=0; i<INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		// �����ÿյ��б���
		_itoa( i+1, pszTemp, 10 );
		m_rcIPRuleTable.InsertItem(i,pszTemp);

		// �������б���
		m_rcIPRuleTable.SetItemText(i,1,_T(""));
		m_rcIPRuleTable.SetItemText(i,2,_T(""));
		m_rcIPRuleTable.SetItemCheck(i,3,0);
		m_rcIPRuleTable.SetItemText(i,4,_T(""));

		// �ַ�����������
		strIPRule = m_pParentWnd->m_AryIP_9001_strIPRuleTable[i];
		if (!strIPRule.IsEmpty())
			strSortedCodeAry.Add(strIPRule);
	}

	// ���濪ʼ��strSortedCodeAry�е��ַ������յ����뿪ʼ����
	DoIPRuleSort(strSortedCodeAry);

	for (i=0; i<strSortedCodeAry.GetSize(); i++)
	{
		int nShowIPCheck = 0;
		CString strDaoRuCode(""), strJieRuCode(""), strCutLength("");
		int nFirstStop(0), nSecondStop(0);	// ��ʾ*��#��λ��

		strIPRule = strSortedCodeAry[i];

		// ��Ϊ�գ�����������
		if (!strIPRule.IsEmpty())
		{
			nFirstStop = strIPRule.Find('*');
			if (nFirstStop == -1)
			{
				nShowIPCheck = 1;
				nFirstStop = strIPRule.Find('#');
			}
			else
				nShowIPCheck = 0;

			ASSERT(nFirstStop!=-1);

			strDaoRuCode = strIPRule.Left(nFirstStop);

			nSecondStop = strIPRule.Find('#', nFirstStop+1);

			if (nSecondStop!=-1)
			{
				// �ҵ���#
				strJieRuCode = strIPRule.Mid(nFirstStop+1, nSecondStop-nFirstStop-1);
				strCutLength = strIPRule.Mid(nSecondStop+1);
			}
			else
			{
				// û�ҵ�#������ʡ��#�����
				strJieRuCode = strIPRule.Mid(nFirstStop+1);
				strCutLength = "0";
			}
		}

		// ���õ���stringд��listctrl��ȥ
		m_rcIPRuleTable.SetItemText(i,1,strDaoRuCode);
		m_rcIPRuleTable.SetItemText(i,2,strJieRuCode);
		m_rcIPRuleTable.SetItemCheck(i,3,nShowIPCheck);
		m_rcIPRuleTable.SetItemText(i,4,strCutLength);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}

void CPropPageIP::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_A_REPORT_IPTABLE)->EnableWindow(bLock);
}
