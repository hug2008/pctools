// PropPageFee.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageFee.h"

#include "phonenumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CPropPageFee::CPropPageFee(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageFee::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageFee)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageFee::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageFee)
	DDX_Control(pDX, IDC_PAGE_H_REPORT_FEETABLE, m_rcFeeTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageFee, CStackPage)
	//{{AFX_MSG_MAP(CPropPageFee)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_H_REPORT_FEETABLE, OnRvnEndItemEdit)
	ON_NOTIFY(RVN_ITEMCLICK, IDC_PAGE_H_REPORT_FEETABLE, OnRvnItemClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageFee message handlers

void CPropPageFee::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ���edit�༭����������֪ͨ����������
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageFee::OnRvnItemClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMREPORTVIEW lpnmrv = (LPNMREPORTVIEW)pNMHDR;

	if(lpnmrv->iItem>=0 && lpnmrv->iSubItem>=0)
	{
		RVITEM rvi;
		rvi.iItem = lpnmrv->iItem;
		rvi.iSubItem = lpnmrv->iSubItem;
		m_rcFeeTable.GetItem(&rvi);

		if(rvi.nMask&RVIM_CHECK && lpnmrv->nFlags&RVHT_ONITEMCHECK)
		{
			rvi.iCheck = rvi.iCheck ? 0:1;

			// ���Check״̬�����ģ�����֪ͨ����������
			m_pParentWnd->m_bDataChanged = TRUE;
		}
		m_rcFeeTable.SetItem(&rvi);
	}

	*pResult = FALSE;
}

BOOL CPropPageFee::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	// �������ߺ͵�ɫ
	m_rcFeeTable.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT|
		RVS_FOCUSSUBITEMS
		);

	// ����listctrl ��editֻ�ܽ�����������
	// m_rcFeeTable.SetOnlyDigits(TRUE);

	InitFeeTable();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageFee::InitFeeTable()
{
	// ���뱨ͷ
	m_rcFeeTable.InsertColumn(0, _T("No."), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

#ifdef ENGLISH_VERSION
	m_rcFeeTable.InsertColumn(1, _T("Periods-1"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								70);

	m_rcFeeTable.InsertColumn(2, _T("Cost-1"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								55);

	m_rcFeeTable.InsertColumn(3, _T("Periods-2"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								70);

	m_rcFeeTable.InsertColumn(4, _T("Cost-2"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								55);

	m_rcFeeTable.InsertColumn(5, _T("Service fee"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								80);

#else
	m_rcFeeTable.InsertColumn(1, _T("�״�ʱ��"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								60);

	m_rcFeeTable.InsertColumn(2, _T("�״η���"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								60);

	m_rcFeeTable.InsertColumn(3, _T("�Ժ�ʱ��"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								60);

	m_rcFeeTable.InsertColumn(4, _T("�Ժ����"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								60);

	m_rcFeeTable.InsertColumn(5, _T("�����"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								59);

#endif
	// �����б���
	char pszTemp[3] = "";
	RVITEM rvi;
	int i = 0;
	for(i=1; i<=NUM_RATE_SETTING_TABLE_TOTAL; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcFeeTable.InsertItem(i-1,pszTemp);
	}

	// �����б�������
	for(i=1; i<=NUM_RATE_SETTING_TABLE_TOTAL; i++)
	{
		rvi.iItem = i-1;
		rvi.nMask = RVIM_TEXT;

		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcFeeTable.SetItem(&rvi);

		rvi.iSubItem = 2;
		m_rcFeeTable.SetItem(&rvi);

		rvi.iSubItem = 3;
		m_rcFeeTable.SetItem(&rvi);

		rvi.iSubItem = 4;
		m_rcFeeTable.SetItem(&rvi);

		rvi.iSubItem = 5;
		m_rcFeeTable.SetItem(&rvi);
	}
}

BOOL CPropPageFee::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);
	
	// 1 -- the list ctrl
	// ûʲô����У��� --- ���˳���
	CString strFee("");

	for (int i=0; i<NUM_RATE_SETTING_TABLE_TOTAL; i++)
	{
		CPhoneNumber strDuration(""), strUnitRate(""), strServiceFee(""), strLaterDuration(""), strLaterRate("");
		CString sError("");

		strDuration			= m_rcFeeTable.GetItemText(i,1);
		strUnitRate			= m_rcFeeTable.GetItemText(i,2);
		strLaterDuration	= m_rcFeeTable.GetItemText(i,3);
		strLaterRate		= m_rcFeeTable.GetItemText(i,4);
		strServiceFee		= m_rcFeeTable.GetItemText(i,5);

		strFee = strDuration + strUnitRate + strLaterDuration + strLaterRate + strServiceFee;

		if (!strFee.IsEmpty())
		{
			if (!strDuration.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("��������ȷ���״�ʱ����(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}
//			if (strFirstTime == "")
//			{
//				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, PROPPAGE_FEE);
//				m_rcFeeTable.SetFocus();
//				m_rcFeeTable.SetSelection(i);
//
//				sError.Format(_T("�����IP���ŵ����벻��Ϊ�գ�(��%d��)"),i+1);
//				AfxMessageBox(sError, MB_ICONSTOP);
//
//				return FALSE;
//			}
			if (strDuration.GetLength() >4 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("������״�ʱ�����Ȳ��ܴ���4��(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}
			// ==================================================================
			if (!strUnitRate.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("��������ȷ���״η��ʣ�(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}

			if (strUnitRate.GetLength() >4 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("������״η��ʳ��Ȳ��ܴ���4��(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}
			// ==================================================================
			if (!strLaterDuration.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("��������ȷ���Ժ�ʱ����(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}

			if (strLaterDuration.GetLength() >4 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("������Ժ�ʱ�����Ȳ��ܴ���4��(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}
			// ==================================================================
			if (!strLaterRate.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("��������ȷ���Ժ���ʣ�(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}

			if (strLaterRate.GetLength() >4 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("������Ժ���ʳ��Ȳ��ܴ���4��(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}
			// ==================================================================
			if (!strServiceFee.IsPhoneNumber())
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("��������ȷ�ķ���ѣ�(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}

			if (strServiceFee.GetLength() >4 )
			{
				m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_H_FEE));
				m_rcFeeTable.SetFocus();
				m_rcFeeTable.SetSelection(i);

				sError.Format(_T("����ķ���ѳ��Ȳ��ܴ���4��(��%d��)"),i+1);
				MessageBox(sError, SYS_NAME, MB_ICONSTOP);

				return FALSE;
			}
		}
	}

	/************************************************************************/
	/*           �����Ϻ󱣴�һ������,�Ա�Ʒ����͵ļ��                  */
	/************************************************************************/
	OnSaveData();

	return TRUE;
}

void CPropPageFee::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	int i=0;
	// 1 -- the edit list ctrl
	CStringArray strFeeAry;
	strFeeAry.RemoveAll();

	CString strFee("");
	for (i=0; i<NUM_RATE_SETTING_TABLE_TOTAL; i++)
	{
		CPhoneNumber strDuration(""), strUnitRate(""), strTax(""), strServiceFee(""), strLaterDuration(""), strLaterRate("");
		CString sError("");

		m_pParentWnd->szMajorRateFramRateSetting12001[i] = "";

		strDuration			= m_rcFeeTable.GetItemText(i,1);
		strUnitRate			= m_rcFeeTable.GetItemText(i,2);
		strLaterDuration	= m_rcFeeTable.GetItemText(i,3);
		strLaterRate		= m_rcFeeTable.GetItemText(i,4);
		strServiceFee		= m_rcFeeTable.GetItemText(i,5);

		strFee = strDuration + "*" + strUnitRate + "*" + strLaterDuration + "*" + strLaterRate + "*" + strServiceFee + "*";
		if (strFee.CompareNoCase("*****")==0)
			strFee = _T("0*0*0*0*0*");
		
		ASSERT(!strFee.IsEmpty());
		strFeeAry.Add(strFee);
	}

	int nCount = strFeeAry.GetSize();

	for (i=0;i<NUM_RATE_SETTING_TABLE_TOTAL;i++)
	{
		if (i<nCount)
			m_pParentWnd->szMajorRateFramRateSetting12001[i] = strFeeAry[i];
		else
			m_pParentWnd->szMajorRateFramRateSetting12001[i] = "";
	}
}

void CPropPageFee::OnRefreshPage(void)
{
	m_rcFeeTable.DeleteAllItems();

	// 1 -- the edit list ctrl
	CStringArray strFeeAry;
	strFeeAry.RemoveAll();

	CString strFee("");
	char pszTemp[3];
	int i=0;
	for (i=0; i<NUM_RATE_SETTING_TABLE_TOTAL; i++)
	{
		// �����ÿյ��б���
		_itoa( i+1, pszTemp, 10 );
		m_rcFeeTable.InsertItem(i,pszTemp);

		// �������б���
		m_rcFeeTable.SetItemText(i,1,_T(""));
		m_rcFeeTable.SetItemText(i,2,_T(""));
		m_rcFeeTable.SetItemText(i,3,_T(""));
		m_rcFeeTable.SetItemText(i,4,_T(""));
		m_rcFeeTable.SetItemText(i,5,_T(""));

		// �ַ�����������
		strFee = m_pParentWnd->szMajorRateFramRateSetting12001[i];
		if (!strFee.IsEmpty())
			strFeeAry.Add(strFee);
	}

	for (i=0; i<strFeeAry.GetSize(); i++)
	{
		CString strDuration(""), strUnitRate(""), strTax(""), strServiceFee(""), strLaterDuration(""), strLaterRate("");
		int nFirstStop(0), nSecondStop(0), nThirdStop(0), nFourthStop(0), nFifthStop(0), nSixthStop(0);	// ��ʾ*��λ��

		strFee = strFeeAry[i];

		// ��Ϊ�գ�����������
		if (strFee.IsEmpty()||(strFee.CompareNoCase("EMPTY")==0)||(strFee.CompareNoCase("****")==0)||(strFee.CompareNoCase("*****")==0))
			continue;

		nFirstStop = strFee.Find('*');
		if (nFirstStop == -1)
			ASSERT(FALSE);
		else
			strDuration = strFee.Left(nFirstStop);

		nSecondStop = strFee.Find('*', nFirstStop+1);
		if (nSecondStop==-1)
			ASSERT(FALSE);
		else
			strUnitRate = strFee.Mid(nFirstStop+1, nSecondStop-nFirstStop-1);

		nThirdStop = strFee.Find('*', nSecondStop+1);
		if (nThirdStop==-1)
			ASSERT(FALSE);
		else
			strLaterDuration = strFee.Mid(nSecondStop+1, nThirdStop-nSecondStop-1);

		nFourthStop = strFee.Find('*', nThirdStop+1);
		if (nFourthStop==-1)
			ASSERT(FALSE);
		else
			strLaterRate = strFee.Mid(nThirdStop+1, nFourthStop-nThirdStop-1);

		strServiceFee = strFee.Right(strFee.GetLength() - 1 - nFourthStop);

		int n1(0),n2(0),n3(0),n4(0),n5(0);
		n1 = atoi(strDuration);
		n2 = atoi(strUnitRate);
		n3 = atoi(strLaterDuration);
		n4 = atoi(strLaterRate);
		n5 = atoi(strServiceFee);

		if ((n1==0)&&(n2==0)&&(n3==0)&&(n4==0)&&(n5==0))	// ȫ��Ĳ�д��ȥ
			continue;

		strDuration.Format("%d", n1);
		strUnitRate.Format("%d", n2);
		strLaterDuration.Format("%d", n3);
		strLaterRate.Format("%d", n4);
		strServiceFee.Format("%d", n5);

		// ���õ���stringд��listctrl��ȥ
		m_rcFeeTable.SetItemText(i,1,strDuration);
		m_rcFeeTable.SetItemText(i,2,strUnitRate);
		m_rcFeeTable.SetItemText(i,3,strLaterDuration);
		m_rcFeeTable.SetItemText(i,4,strLaterRate);
		m_rcFeeTable.SetItemText(i,5,strServiceFee);
	}

	// Set the data to screen show
	UpdateData(FALSE);
}
