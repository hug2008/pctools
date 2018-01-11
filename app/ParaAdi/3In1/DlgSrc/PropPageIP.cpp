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
	// 如果edit编辑结束，设置通知更改了数据
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageIP::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// 增加竖线和底色
	m_rcIPRuleTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT|
		RVS_FOCUSSUBITEMS
		);

	// 设置listctrl 的edit只能接受数字输入
	// m_rcIPRuleTable.SetOnlyDigits(TRUE);

	InitIPRuleTable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageIP::InitIPRuleTable()
{
	// 加入报头
	m_rcIPRuleTable.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcIPRuleTable.InsertColumn(1, _T("IP prefix"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								90);

	m_rcIPRuleTable.InsertColumn(2, _T("cut prefix length"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								120);

	m_rcIPRuleTable.InsertColumn(3, _T("IP header"),
                            	 RVCF_LEFT|
								 RVCF_TEXT|
								 RVCF_EX_FIXEDWIDTH,
								 90);

	// 加入列表项
	char pszTemp[3];
	int i = 0;
	RVITEM rvi;
	for(i=1; i<=NUM_IP_HEADER_TATOL; i++)
	{
		sprintf(pszTemp, "%2.1d", i);
		m_rcIPRuleTable.InsertItem(i-1,pszTemp);
	}

	// 更改列表项属性
	for(i=1; i<=NUM_IP_HEADER_TATOL; i++)
	{
		
		rvi.iItem = i-1;
		rvi.nMask = RVIM_TEXT;

		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcIPRuleTable.SetItem(&rvi);

		rvi.iSubItem = 2;
		rvi.lpszText = _T("");
		m_rcIPRuleTable.SetItem(&rvi);

		rvi.iSubItem = 3;
		rvi.lpszText = _T("");
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
			// 如果Check状态被更改，设置通知更改了数据
			m_pParentWnd->m_bDataChanged = TRUE;
		}
		m_rcIPRuleTable.SetItem(&rvi);
	}

	*pResult = FALSE;
}

BOOL CPropPageIP::OnCloseTest(void)
{
	UpdateData(TRUE);

	// 1 -- the list ctrl
	// 没什么可以校验的 --- 除了长度
	CString strIPRule("");
	for (int i=0; i<NUM_IP_HEADER_TATOL; i++)
	{
		CPhoneNumber strDaoRuCode(""), strTianJiaCode(""), strShowIPHead(""), strQieChuCode("");
		CString sError("");

		strDaoRuCode   = m_rcIPRuleTable.GetItemText(i,1);
		strQieChuCode  = m_rcIPRuleTable.GetItemText(i,2);
		strTianJiaCode = m_rcIPRuleTable.GetItemText(i,3);

		strIPRule = strDaoRuCode + strTianJiaCode + strQieChuCode;

		if (!strIPRule.IsEmpty())
		{
			if (!strDaoRuCode.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_A_IP));
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				/* sError.Format(_T("请输入正确的IP拨号导入码！(第%d条)"),i+1); */
				sError.Format(_T("Inputed NO.%d IP prefix is illeagal, try again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}
			if (strDaoRuCode == "")
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_A_IP));
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				/* sError.Format(_T("输入的IP拨号导入码不能为空！(第%d条)"),i+1); */
				sError.Format(_T("Inputed NO.%d IP prefix can not be empty!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}
			if (strDaoRuCode.GetLength() >12 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_A_IP));
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);
				/* sError.Format(_T("输入的IP拨号导入码长度不能大于12！(第%d条)"),i+1); */
				sError.Format(_T("Inputed NO.%d IP prefix is empty, try again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}

			if (!strQieChuCode.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_A_IP));
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				/* sError.Format(_T("请输入正确的IP拨号前部截除位数！(第%d条)"),i+1); */
				sError.Format(_T("Inputed NO.%d IP cut prefix length is illeagal, try again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}
			if (atoi(strQieChuCode) >12 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_A_IP));
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				/* sError.Format(_T("输入的IP拨号前部截除位数不能大于12！(第%d条)"),i+1); */
				sError.Format(_T("Inputed NO.%d IP cut prefix length is greater than 12, try again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}

			/*
			if (strTianJiaCode == "")
			{
			m_rcIPRuleTable.SetFocus();
			m_rcIPRuleTable.SetSelection(i);
			AfxMessageBox(_T("输入的IP拨号添加码不能为空！"),MB_ICONSTOP);
			return FALSE;
			}
			*/  //IP拨号IP接入码可以为空！

			if (!strTianJiaCode.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_A_IP));
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);

				/* sError.Format(_T("请输入正确的IP拨号接入码！(第%d条)"),i+1); */
				sError.Format(_T("Inputed NO.%d IP header is illeagal, try again!"),i+1);
				MessageBox(sError, SYS_NAME,MB_ICONSTOP);
				return FALSE;
			}
			if (strTianJiaCode.GetLength() >12 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_A_IP));
				m_rcIPRuleTable.SetFocus();
				m_rcIPRuleTable.SetSelection(i);


				/*sError.Format(_T("输入的IP拨号接入码长度不能大于12！(第%d条)"),i+1);*/
				sError.Format(_T("Inputed NO.%d IP header is longer than 12, try again!"),i+1);
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

	int i = 0;
	// 1 -- the edit list ctrl
	CStringArray strIPRuleAry;
	strIPRuleAry.RemoveAll();

	CString strIPRule("");
	for (i=0; i<NUM_IP_HEADER_TATOL; i++)
	{
		CString strDaoRuCode(""), strQieChuCode(""), strTianJiaCode("");
		strDaoRuCode	= m_rcIPRuleTable.GetItemText(i,1);
		strQieChuCode	= m_rcIPRuleTable.GetItemText(i,2); 
		strTianJiaCode  = m_rcIPRuleTable.GetItemText(i,3);
		
		strIPRule = strDaoRuCode + "*" + strQieChuCode + "*" + strTianJiaCode + "*";
		ASSERT(!strIPRule.IsEmpty());
		strIPRuleAry.Add(strIPRule);
	}


	int nCount = strIPRuleAry.GetSize();


	// now they were sorted
	for (i=0;i<nCount;i++)
	{
		m_pParentWnd->szMajoripFramIpSetting[i] = strIPRuleAry[i];
	}
}

void CPropPageIP::OnRefreshPage(void)
{
	m_rcIPRuleTable.DeleteAllItems();

	// 1 -- the edit list ctrl
	CStringArray strIPRuleAry;
	strIPRuleAry.RemoveAll();

	CString strIPRule("");
	char pszTemp[3];
	int i = 0;
	for (i=0; i<NUM_IP_HEADER_TATOL; i++)
	{
		// 加入置空的列表项
		_itoa( i+1, pszTemp, 10 );
		m_rcIPRuleTable.InsertItem(i,pszTemp);

		// 加入子列表项
		m_rcIPRuleTable.SetItemText(i,1,_T(""));
		m_rcIPRuleTable.SetItemText(i,2,_T(""));
		m_rcIPRuleTable.SetItemText(i,3,_T(""));

		// 字符串加入数组
		strIPRule = m_pParentWnd->szMajoripFramIpSetting[i];
		if (!strIPRule.IsEmpty())
			strIPRuleAry.Add(strIPRule);
	}

	// 下面开始对strIPRuleAry中的字符串按照导入码开始排序
	//	DoIPRuleSort(strIPRuleAry);

	for (i=0; i<strIPRuleAry.GetSize(); i++)
	{
		CString strDaoRuCode(""), strQieChuCode(""), strTianJiaCode("");
		int nFirstStop(0), nSecondStop(0), nThirdStop(0);

		strIPRule = strIPRuleAry[i];

		// 若为空，则抛弃数据
		if (strIPRule.IsEmpty()||(strIPRule.CompareNoCase("Empty")==0)||(strIPRule.CompareNoCase("*##")==0))
			continue;

		nFirstStop = strIPRule.Find('*');
		if (nFirstStop == -1)
			ASSERT(FALSE);
		else
			strDaoRuCode = strIPRule.Left(nFirstStop);

		nSecondStop = strIPRule.Find('*', nFirstStop+1);
		if (nSecondStop == -1)
			ASSERT(FALSE);
		else
			strQieChuCode = strIPRule.Mid(nFirstStop+1, nSecondStop-nFirstStop-1);

		nThirdStop = strIPRule.Find('*', nSecondStop+1);
		if (nThirdStop == -1)
			ASSERT(FALSE);
		else
			strTianJiaCode = strIPRule.Mid(nSecondStop+1, nThirdStop-nSecondStop-1);

		// 将得到的string写道listctrl中去
		m_rcIPRuleTable.SetItemText(i,1,strDaoRuCode);
		m_rcIPRuleTable.SetItemText(i,2,strQieChuCode);
		m_rcIPRuleTable.SetItemText(i,3,strTianJiaCode);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}

void CPropPageIP::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_A_REPORT_IPTABLE)->EnableWindow(bLock);
}
