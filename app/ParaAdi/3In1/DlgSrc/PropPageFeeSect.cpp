// PropPageFeeSectSect.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageFeeSect.h"

#include "phonenumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CPropPageFeeSect::CPropPageFeeSect(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageFeeSect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageFeeSect)
	//}}AFX_DATA_INIT
}


void CPropPageFeeSect::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageFeeSect)
	DDX_Control(pDX, IDC_PAGE_I_REPORT_FEESECT, m_rcFeeSectTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageFeeSect, CStackPage)
	//{{AFX_MSG_MAP(CPropPageFeeSect)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_I_REPORT_FEESECT, OnRvnEndItemEdit)
	ON_NOTIFY(RVN_ITEMCLICK, IDC_PAGE_I_REPORT_FEESECT, OnRvnItemClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageFeeSect message handlers
void CPropPageFeeSect::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 如果edit编辑结束，设置通知更改了数据
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageFeeSect::OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMREPORTVIEW lpnmrv = (LPNMREPORTVIEW)pNMHDR;

	if(lpnmrv->iItem>=0 && lpnmrv->iSubItem>=0)
	{
		RVITEM rvi;
		rvi.iItem = lpnmrv->iItem;
		rvi.iSubItem = lpnmrv->iSubItem;
		m_rcFeeSectTable.GetItem(&rvi);

		if(rvi.nMask&RVIM_CHECK && lpnmrv->nFlags&RVHT_ONITEMCHECK)
		{
			rvi.iCheck = rvi.iCheck ? 0:1;

			// 如果Check状态被更改，设置通知更改了数据
			m_pParentWnd->m_bDataChanged = TRUE;
		}
		m_rcFeeSectTable.SetItem(&rvi);
	}

	*pResult = FALSE;
}

BOOL CPropPageFeeSect::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// 增加竖线和底色
	m_rcFeeSectTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT|
		RVS_FOCUSSUBITEMS
		);

	// 设置listctrl 的edit只能接受数字输入
	// m_rcFeeSectTable.SetOnlyDigits(TRUE);

	InitFeeSectTable();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageFeeSect::InitFeeSectTable()
{
	char pszTemp[5] = "";
	int i = 0;
	RVITEM rvi;
	// 加入报头
	m_rcFeeSectTable.InsertColumn(0, _T("No "), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS,
								40);
#ifdef ENGLISH_VERSION
	m_rcFeeSectTable.InsertColumn(1, _T("Fee Sect"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								160);

	m_rcFeeSectTable.InsertColumn(2, _T("Fee Type"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								123);

#else

	m_rcFeeSectTable.InsertColumn(1, _T("费率号段"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								160);

	m_rcFeeSectTable.InsertColumn(2, _T("计费类型"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								123);
#endif

	// 加入列表项
	for(i=0; i<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; i++)
	{
		sprintf(pszTemp, "%3.1d", i+1);
		m_rcFeeSectTable.InsertItem(i,pszTemp);

		// 加入子列表项
		m_rcFeeSectTable.SetItemText(i,1,_T(""));
		m_rcFeeSectTable.SetItemText(i,2,_T(""));
		rvi.iItem = i;
		rvi.nMask = RVIM_TEXT;

		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcFeeSectTable.SetItem(&rvi);

		rvi.iSubItem = 2;
		m_rcFeeSectTable.SetItem(&rvi);
	}

}

BOOL CPropPageFeeSect::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);

	/************************************************************************/
	/*                       首先记录一下有多少条费率设置                   */
	/************************************************************************/
	int nFeeTableTotal = 0;
	int i              = 0;
	CStringArray szArySect;
	CPhoneNumber strFeeSectString(""), strFeeType("");
	CString      sError("");
	BOOL         bMatchLocalNum = FALSE;
	
	// 1 -- the list ctrl
	// 没什么可以校验的 --- 除了长度
	CString strFeeSect("");

	for (i=0; i<NUM_RATE_SETTING_TABLE_TOTAL; i++)
	{
		if (IsRateTableItemEmpty(m_pParentWnd->szMajorRateFramRateSetting12001[i]))
		{
			continue;
		}
		nFeeTableTotal++;
	}

	nFeeTableTotal = (nFeeTableTotal > NUM_RATE_SETTING_TABLE_TOTAL) ? NUM_RATE_SETTING_TABLE_TOTAL : nFeeTableTotal;


	for (i=0; i<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; i++)
	{
		strFeeSectString	= m_rcFeeSectTable.GetItemText(i,1);
		strFeeType	        = m_rcFeeSectTable.GetItemText(i,2);

		strFeeSect = strFeeSectString + strFeeType;

		if (!strFeeSect.IsEmpty())
		{
			if (!strFeeSectString.IsPhoneString())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_I_FEESECT));
				m_rcFeeSectTable.SetFocus();
				m_rcFeeSectTable.SetSelection(i);
				/* sError.Format(_T("请输入正确的费率号段！(第%d条)"),i+1); */
				sError.Format(_T("Please input the correct fee sect!(No %d)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);
				return FALSE;
			}
			if (strFeeSectString.GetLength() >13 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_I_FEESECT));
				m_rcFeeSectTable.SetFocus();
				m_rcFeeSectTable.SetSelection(i);
				/* sError.Format(_T("输入的费率号段长度不能大于10！(第%d条)"),i+1); */
				sError.Format(_T("The length of fee sect can not more than 10！(No %d)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);
				return FALSE;
			}
			// ==================================================================
			if (!strFeeType.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_I_FEESECT));
				m_rcFeeSectTable.SetFocus();
				m_rcFeeSectTable.SetSelection(i);

				/* sError.Format(_T("请输入正确的计费类型！(第%d条)"),i+1); */
				sError.Format(_T("Please input the correct fee type！(No %d)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);
				return FALSE;
			}

			if (atoi(strFeeType) == 1 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_I_FEESECT));
				m_rcFeeSectTable.SetFocus();
				m_rcFeeSectTable.SetSelection(i);

				sError.Format(_T("计费类型1已固定为接听电话时的费率！(第%d条)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);
				return FALSE;
			}
/*
			if (2 == atoi(strFeeType))
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_I_FEESECT));
				m_rcFeeSectTable.SetFocus();
				m_rcFeeSectTable.SetSelection(i);

				sError.Format(_T("计费类型2已固定为免费电话的费率！(第%d条)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);
				return FALSE;
			}
*/
			
			if (atoi(strFeeType) > nFeeTableTotal )
			{
				extern PropTreeNote pageNode[PAGE_TOTAL];
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_I_FEESECT));
				m_rcFeeSectTable.SetFocus();
				m_rcFeeSectTable.SetSelection(i);
#ifdef ENGLISH_VERSION
				sError.Format(_T("there are %d items has been setted in %s, Fee Type no more than %d!(No %d)"),
					              nFeeTableTotal, pageNode[PROPPAGE_FEE].szName, nFeeTableTotal, i+1);
				
#else
				sError.Format(_T("费率表里面只设置了 %d 条费率,计费类型不能够超过 %d！(第%d条)"), nFeeTableTotal, nFeeTableTotal, i+1);
#endif
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);
				return FALSE;
			}
		}
	}

	/************************************************************************/
	/*          在这里检查本地号段是否在计费类型中,没有的话就报错           */
	/*   CString   szLocalPhoneNumberHead[NUM_LOCALPHONE_HEADER_TOTAL];     */
	/************************************************************************/
	szArySect.RemoveAll();
	for (i=0; i<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; i++)
	{
		strFeeSectString = m_rcFeeSectTable.GetItemText(i, 1);
		if (strFeeSectString.IsEmpty())
		{
			continue;
		}
		szArySect.Add(strFeeSectString);
	}

	for (i=0; i<NUM_LOCALPHONE_HEADER_TOTAL; i++)
	{
		if (m_pParentWnd->szLocalPhoneNumberHead[i].IsEmpty())
		{
			continue;
		}

		bMatchLocalNum = FALSE;

		for (nFeeTableTotal=0; nFeeTableTotal<szArySect.GetSize(); nFeeTableTotal++)
		{
			if (m_pParentWnd->szLocalPhoneNumberHead[i].GetLength() < szArySect.GetAt(nFeeTableTotal).GetLength())
			{
				continue;
			}

			if (0 == strncmp(m_pParentWnd->szLocalPhoneNumberHead[i],
				             szArySect.GetAt(nFeeTableTotal),
							 szArySect.GetAt(nFeeTableTotal).GetLength()))
			{
				bMatchLocalNum = TRUE;
				break;
			}
		}

		if (!bMatchLocalNum)
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_I_FEESECT));
			m_rcFeeSectTable.SetFocus();
			m_rcFeeSectTable.SetSelection(0);
#ifdef ENGLISH_VERSION
			sError.Format("Local Phone Number [%s] is not match none of \"Fee Sect\"!", m_pParentWnd->szLocalPhoneNumberHead[i]);

#else
			sError.Format("本地号码段 [%s] 没有匹配的 \"费率号段\"!", m_pParentWnd->szLocalPhoneNumberHead[i]);
#endif
			MessageBox(sError, SYS_NAME, MB_ICONSTOP);

			return FALSE;
		}
	}
	

	return TRUE;
}

void CPropPageFeeSect::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	int i=0;

	// 1 -- the edit list ctrl
	CStringArray strFeeSectAry;
	strFeeSectAry.RemoveAll();

	CString strFeeSect("");
	for (i=0; i<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; i++)
	{
		CPhoneNumber strFeeSectString(""), strFeeType("");
		CString sError("");
		int nTemp = 0;

		strFeeSectString	= m_rcFeeSectTable.GetItemText(i,1);
		strFeeType			= m_rcFeeSectTable.GetItemText(i,2);

		strFeeSect = strFeeSectString + "*" + strFeeType + "*";
		ASSERT(!strFeeSect.IsEmpty());
		if (strFeeSect.CompareNoCase("**")!=0) 
		{
			nTemp = atoi(strFeeType)-1;
			strFeeType.Format("%d", nTemp);
			strFeeSect = strFeeSectString + "*" + strFeeType + "*";
			strFeeSectAry.Add(strFeeSect);
		}
	}

	int nCount = strFeeSectAry.GetSize();

	for (i=0;i<NUM_RATE_SETTING_HEADER_TYPE_TOTAL;i++)
	{
		if (i<nCount)
			m_pParentWnd->szMajorRateFramRateHeadSetting[i] = strFeeSectAry[i];
		else
			m_pParentWnd->szMajorRateFramRateHeadSetting[i] = "";
	}
}

void CPropPageFeeSect::OnRefreshPage(void)
{
	m_rcFeeSectTable.DeleteAllItems();

	// 1 -- the edit list ctrl
	CStringArray strFeeSectAry;
	strFeeSectAry.RemoveAll();

	CString strFeeSect("");
	char pszTemp[5];
	int i=0;
	for (i=0; i<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; i++)
	{
		// 加入置空的列表项
		_itoa( i+1, pszTemp, 10 );
		m_rcFeeSectTable.InsertItem(i,pszTemp);

		// 加入子列表项
		m_rcFeeSectTable.SetItemText(i,1,_T(""));
		m_rcFeeSectTable.SetItemText(i,2,_T(""));

		// 字符串加入数组
		strFeeSect = m_pParentWnd->szMajorRateFramRateHeadSetting[i];
		if (!strFeeSect.IsEmpty())
			strFeeSectAry.Add(strFeeSect);
	}

	for (i=0; i<strFeeSectAry.GetSize(); i++)
	{
		CString strFeeSectString(""), strFeeType("");
		int nFirstStop(0), nSecondStop(0);	// 表示*的位置

		strFeeSect = strFeeSectAry[i];

		// 若为空，则抛弃数据
		if (strFeeSect.IsEmpty()||(strFeeSect.CompareNoCase("*0*")==0)||(strFeeSect.CompareNoCase("Empty")==0)||(strFeeSect.CompareNoCase("*")==0))
			continue;

		nFirstStop = strFeeSect.Find('*');
		if (nFirstStop == -1)
			ASSERT(FALSE);
		else
			strFeeSectString = strFeeSect.Left(nFirstStop);
/*
		nSecondStop = strFeeSect.Find('*', nFirstStop+1);
		if (nSecondStop==-1)
			ASSERT(FALSE);
		else
			strFeeType = strFeeSect.Mid(nFirstStop+1, nSecondStop-nFirstStop-1);
*/
		strFeeType = strFeeSect.Right(strFeeSect.GetLength() - nFirstStop - 1);
		int /*n1(0),*/n2(0);
		// n1 = atoi(strFeeSectString);
		n2 = atoi(strFeeType);

		if (strFeeSectString.IsEmpty()&&strFeeType.IsEmpty())	// 全零的不写进去
			continue;

		// strFeeSectString.Format("%d", n1);
		strFeeType.Format("%d", n2+1);
		
		// 将得到的string写道listctrl中去
		m_rcFeeSectTable.SetItemText(i,1,strFeeSectString);
		m_rcFeeSectTable.SetItemText(i,2,strFeeType);
	}

	// Set the data to screen show

	UpdateData(FALSE);
}

