// PropPageLockLac.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageLockLac.h"

#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MaxNum  50
/////////////////////////////////////////////////////////////////////////////
// CPropPageLockLac dialog


CPropPageLockLac::CPropPageLockLac(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageLockLac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageLockLac)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPropPageLockLac::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLockLac)
	DDX_Control(pDX, IDC_PAGE_10_REPORT_LOCKLAC, m_rcLockLac);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageLockLac, CStackPage)
	//{{AFX_MSG_MAP(CPropPageLockLac)
	ON_BN_CLICKED(IDC_PAGE_10_RADIO_LACYES, OnPage10RadioLacyes)
	ON_BN_CLICKED(IDC_PAGE_10_RADIO_LACNO, OnPage10RadioLacno)
	ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_10_REPORT_LOCKLAC, OnRvnEndItemEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageLockLac message handlers
void CPropPageLockLac::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 如果edit编辑结束，设置通知更改了数据
	m_pParentWnd->m_bDataChanged = TRUE;
}


void CPropPageLockLac::OnPage10RadioLacyes() 
{
	// TODO: Add your control notification handler code here
	m_rcLockLac.EnableWindow(TRUE);
	char pszTemp[3];
	m_rcLockLac.DeleteAllItems();

	for(int j=1; j< MaxNum+1; j++) 
	{
		_itoa( j, pszTemp, 10 );
		m_rcLockLac.InsertItem(j-1,pszTemp);
	}

	for(int i=1; i< MaxNum+1; i++)
	{
		RVITEM rvi;
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_rcLockLac.SetItem(&rvi);
	}

	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLockLac::OnPage10RadioLacno() 
{
	// TODO: Add your control notification handler code here
	m_rcLockLac.DeleteAllItems();
	m_rcLockLac.EnableWindow(FALSE);
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageLockLac::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);	

	//  -- the check button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACYES))->GetCheck()
		    ==
		  ((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACNO))->GetCheck())
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_10_LOCK_LAC);
		MessageBox(_T("Please select lock LAC or not!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	//  -- the list ctrl
	if (((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACYES))->GetCheck() == TRUE)
	{
		// 若为否，则不用检测数据
//		DWORD dInput,dMax = 65535;
		CPhoneNumber strInput("");
		CString sError;

		for (int i=0; i< MaxNum; i++)
		{
			strInput = m_rcLockLac.GetItemText(i,1);
			if (!strInput.IsEmpty())	// 允许不输入，或输入不足
			{
				if (!strInput.IsHexNumber())
				{
					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_10_LOCK_LAC);
					m_rcLockLac.SetFocus();
					m_rcLockLac.SetSelection(i);

					sError.Format(_T("No.%d: LAC must be hex chars"),i+1);
					MessageBox(sError, SYS_NAME,MB_ICONSTOP);

					return FALSE;
				}
				
				if (strInput.GetLength() > 4)
				{
					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_10_LOCK_LAC);
					m_rcLockLac.SetFocus();
					m_rcLockLac.SetSelection(i);

					sError.Format(_T("No.%d: LAC no more than 4 chars"),i+1);
					MessageBox(sError, SYS_NAME,MB_ICONSTOP);
					return FALSE;

				}
/*
				dInput = atoi(strInput);
				if ((strInput.GetLength()>5)||(dInput> dMax))		// 若输入了，则不允许大于FFFF
				{
					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_10_LOCK_LAC);
					m_rcLockLac.SetFocus();
					m_rcLockLac.SetSelection(i);

					sError.Format(_T("输入的锁定Lac编号不能大于65535！(第%d条)"),i+1);
					MessageBox(sError, SYS_NAME,MB_ICONSTOP);
					return FALSE;
				}
*/
				else if ( (strInput == "0") || (strInput == "00") || (strInput == "000") || (strInput == "0000") )
				{
					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_10_LOCK_LAC);
					m_rcLockLac.SetFocus();
					m_rcLockLac.SetSelection(i);

					sError.Format(_T("No.%d: LAC can't be zero!"),i+1);
					MessageBox(sError, SYS_NAME,MB_ICONSTOP);
					return FALSE;
				}
			}
		}
	}

	return TRUE;

}

void CPropPageLockLac::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);
	BOOL bFlag;
	// 3 -- the check button
	bFlag = ((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACYES))->GetCheck();

	if (bFlag)
	{
		m_pParentWnd->m_strLOCK_8004_bLockLac = "01";

	
		// 6 -- the list ctrl
		CString strResult("");
		CString strResult2("");
		for (int i=0; i< MaxNum; i++)
		{
			CString strRegion("");
			if (i <= MaxNum/2)
				{

					strRegion = m_rcLockLac.GetItemText(i,1);
					if (!strRegion.IsEmpty())
						{
				// 从字符得到整数，再从整数到16进制
				// 再从16进制到字符
						CString strTemp("");
/*						char pszTemp[4];

						int iRegion = atoi(strRegion);
						 _itoa( iRegion, pszTemp, 16 );	
						sprintf(pszTemp, "%04x", iRegion);
						strTemp = pszTemp;*/

						strTemp = strRegion;
	
						strTemp.MakeUpper();

						ASSERT(strTemp.GetLength()==4);

						strResult += strTemp;
						}
				}
			else
			{
					strRegion = m_rcLockLac.GetItemText(i,1);
					if (!strRegion.IsEmpty())
					{
					// 从字符得到整数，再从整数到16进制
					// 再从16进制到字符
						CString strTemp("");
/*						char pszTemp[4];

						int iRegion = atoi(strRegion);
						 _itoa( iRegion, pszTemp, 16 );	
						sprintf(pszTemp, "%04x", iRegion);
						strTemp = pszTemp;*/

						strTemp = strRegion;

						strTemp.MakeUpper();

						ASSERT(strTemp.GetLength()==4);

						strResult2 += strTemp;
					}
			}
		}
		m_pParentWnd->m_strLOCK_8005_strLockLac = strResult;
		m_pParentWnd->m_strLOCK_8006_strLockLac = strResult2;
	}
	else
	{
		m_pParentWnd->m_strLOCK_8004_bLockLac = "00";
		m_pParentWnd->m_strLOCK_8005_strLockLac = "";
		m_pParentWnd->m_strLOCK_8006_strLockLac = "";
	}
}
void CPropPageLockLac::OnRefreshPage(void)
{

	BOOL bFlag;	
	// -- the check button
	bFlag = atoi(m_pParentWnd->m_strLOCK_8004_bLockLac);
	((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACYES))->SetCheck(bFlag);
	((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACNO ))->SetCheck(!bFlag);

	if (bFlag)
	{
		//  -- the list ctrl
		m_rcLockLac.EnableWindow(TRUE);
		m_rcLockLac.DeleteAllItems();
		for (int i=1; i< MaxNum+1; i++)
			{
				char pszTemp[10];
				_itoa( i, pszTemp, 10 );
				m_rcLockLac.InsertItem(i-1,pszTemp);			// 前面的序号
			}

		int nLen = m_pParentWnd->m_strLOCK_8005_strLockLac.GetLength();
		ASSERT((nLen/4)*4 == nLen);	// 长度是四的整数倍
		ASSERT((nLen/4) < MaxNum/2+1);

		int nLen2 = m_pParentWnd->m_strLOCK_8006_strLockLac.GetLength();
		ASSERT((nLen2/4)*4 == nLen2);	// 长度是四的整数倍
		ASSERT((nLen2/4) < MaxNum/2+1);

		if (nLen != 0)	// 若为空串则直接返回
		{
			CString strAllLockLac = m_pParentWnd->m_strLOCK_8005_strLockLac;
			CString strAllLockLac2 = m_pParentWnd->m_strLOCK_8006_strLockLac;

			for (int j=1; j< MaxNum+1; j++)
			{
				
				if (j <=25)
				{
					CString strOneRegion("");
					if (j<= nLen/4)
					{
						int iRegion = 0;
							// 得到16进制的字符串
						strOneRegion = strAllLockLac.Mid((j-1)*4, 4);
/*							// 得到16进制的数字
						sscanf(strOneRegion.GetBuffer(4),"%4x", &iRegion);
						strOneRegion.ReleaseBuffer();                    	
							// 下面将数字变成字符
						strOneRegion.Format("%d", iRegion);*/
					}
					else
					{
						strOneRegion = "";	
					}

					m_rcLockLac.SetItemText(j-1,1,strOneRegion);
				}
				
				else
				{
					CString strOneRegion("");

					if (j< (nLen2/4+26))
					{
						int iRegion = 0;
							// 得到16进制的字符串
						strOneRegion = strAllLockLac2.Mid((j-26)*4, 4);
							// 得到16进制的数字
				/*		sscanf(strOneRegion.GetBuffer(4),"%4x", &iRegion);
						strOneRegion.ReleaseBuffer();                    	
							// 下面将数字变成字符
						strOneRegion.Format("%d", iRegion);*/
					}
					else
					{
						strOneRegion = "";	
					}

					m_rcLockLac.SetItemText(j-1,1,strOneRegion);

				}
				

			}
		}
		else
		{
			for (int j=1; j<MaxNum+1; j++)
			{
				m_rcLockLac.SetItemText(j-1,1,_T(""));
			}
		}
	}
	else // bFlag == FALSE
	{
	   m_rcLockLac.DeleteAllItems();
	}

	// Set the data to screen show
	UpdateData(FALSE);
}


BOOL CPropPageLockLac::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	m_bLockLac   = FALSE;
	((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACYES))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_PAGE_10_RADIO_LACNO))->SetCheck(1);

	// Add the vertical line and background color
	m_rcLockLac.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT
//fndef DISABLE_INPUT
		| RVS_FOCUSSUBITEMS
//ndif
		);
	m_rcLockLac.DeleteAllItems();
	InitLockRegionTable();

	m_rcLockLac.DeleteAllItems();
	m_rcLockLac.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPropPageLockLac::InitLockRegionTable()
{
	// insert the col. headline index
	m_rcLockLac.InsertColumn(0, _T("No."), 
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_AUTOWIDTH|
								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS
								);

	m_rcLockLac.InsertColumn(1, _T("lock LAC cell"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								125);

	// 加入列表项
	char pszTemp[3];
	m_rcLockLac.DeleteAllItems();
	for(int i=1; i<MaxNum+1; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcLockLac.InsertItem(i-1,pszTemp);
	}

}
void CPropPageLockLac::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_10_RADIO_LACYES)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_10_RADIO_LACNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_10_REPORT_LOCKLAC)->EnableWindow(bLock);

}

