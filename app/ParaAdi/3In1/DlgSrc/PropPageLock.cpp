// PropPageLock.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageLock.h"

#include "PhoneNumber.h"
#include "StringCheckEncode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageLock dialog


CPropPageLock::CPropPageLock(CWnd* pParent /*=NULL*/)
: CStackPage(CPropPageLock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageLock)
	
	//}}AFX_DATA_INIT
#if defined(R16_ATC)
	m_strProvider = _T("00000");
#else
	m_strProvider = _T("00");
#endif
	m_bLockProvider   = FALSE;
	m_bLockRegion     = FALSE;
	m_bAutoLockRegion = FALSE;
		
	m_pParentWnd = NULL;
}


void CPropPageLock::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageLock)
	DDX_Control(pDX, IDC_PAGE_9_REPORT_LOCKREGION, m_rcLockRegion);
	DDX_Control(pDX, IDC_PAGE_9_EDIT_REGIONNUM,    m_edtRegionNum);
	DDX_Control(pDX, IDC_PAGE_9_SPIN_REGIONNUM,    m_spnRegionNum);
    DDX_Text(pDX, IDC_PAGE_9_EDIT_PROVIDER,        m_strProvider);
#if defined(R16_ATC)
	DDV_MaxChars(pDX,   m_strProvider,             5);
#else
	DDV_MaxChars(pDX,   m_strProvider,             2);
#endif
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageLock, CStackPage)
//{{AFX_MSG_MAP(CPropPageLock)
ON_BN_CLICKED(IDC_PAGE_9_RADIO_PRVNO, OnPage9RadioProviderNo)
ON_BN_CLICKED(IDC_PAGE_9_RADIO_PRVYES, OnPage9RadioProviderYes)
ON_BN_CLICKED(IDC_PAGE_9_RADIO_REGIONYES, OnPage9RadioRegionYes)
ON_BN_CLICKED(IDC_PAGE_9_RADIO_REGIONNO, OnPage9RadioRegionNo)
ON_BN_CLICKED(IDC_PAGE_9_RADIO_AUTOLOCKNO, OnPage9RadioAutolockno)
ON_BN_CLICKED(IDC_PAGE_9_RADIO_AUTOLOCKYES, OnPage9RadioAutolockyes)
ON_NOTIFY(RVN_ENDITEMEDIT, IDC_PAGE_9_REPORT_LOCKREGION, OnRvnEndItemEdit)
ON_EN_CHANGE(IDC_PAGE_9_EDIT_REGIONNUM, OnChangeEditRegionNum)
ON_EN_CHANGE(IDC_PAGE_9_EDIT_PROVIDER, OnChangeEditProvider)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageLock message handlers

void CPropPageLock::OnRvnEndItemEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 如果edit编辑结束，设置通知更改了数据
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLock::OnPage9RadioProviderYes() 
{
	GetDlgItem(IDC_PAGE_9_EDIT_PROVIDER)->EnableWindow(TRUE);
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLock::OnPage9RadioProviderNo() 
{
	GetDlgItem(IDC_PAGE_9_EDIT_PROVIDER)->EnableWindow(FALSE);
#if defined(R16_ATC)
	m_strProvider = _T("00000");
#else
	m_strProvider = _T("00");
#endif
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLock::OnPage9RadioRegionYes() 
{
	m_edtRegionNum.EnableWindow(TRUE);
	m_spnRegionNum.EnableWindow(TRUE);
	
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKNO))->EnableWindow(TRUE);
	
	// 设置自动锁小区
	if (m_bAutoLockRegion)
		((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKNO))->SetCheck(1);
	
	// m_rcLockRegion.EnableWindow(TRUE);
	
	//	OnSelchangeRegionNum();
	OnChangeEditRegionNum();
	
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLock::OnPage9RadioRegionNo() 
{
	m_edtRegionNum.EnableWindow(FALSE);
	m_spnRegionNum.EnableWindow(FALSE);
	
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->SetCheck(0);
	
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->EnableWindow(0);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKNO))->EnableWindow(0);
	
	//	m_cbRegionNum.SetCurSel(-1);
	
	SetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM, 0);
	
	m_rcLockRegion.DeleteAllItems();
	// m_rcLockRegion.EnableWindow(FALSE);
	
	m_pParentWnd->m_bDataChanged = TRUE;
}


void CPropPageLock::OnPage9RadioAutolockno() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageLock::OnPage9RadioAutolockyes() 
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

BOOL CPropPageLock::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	
	m_bLockProvider   = FALSE;
	m_bLockRegion     = FALSE;
	m_bAutoLockRegion = FALSE;
	
	// 设置锁运营商
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_PRVYES))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_PRVNO))->SetCheck(1);
	
	GetDlgItem(IDC_PAGE_9_EDIT_PROVIDER)->EnableWindow(FALSE);
	
	// 设置锁小区
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONNO))->SetCheck(1);
	
	// 设置自动锁小区
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKNO))->EnableWindow(FALSE);
	
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKNO))->SetCheck(0);
	
	SetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM, 0);
	m_spnRegionNum.SetRange(0,500);
	
	//	m_cbRegionNum.EnableWindow(FALSE);
	
	SetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM, 0);
	
	m_edtRegionNum.EnableWindow(FALSE);
	m_spnRegionNum.EnableWindow(FALSE);
	
	// 增加竖线和底色
	m_rcLockRegion.ModifyStyle(0, 
		RVS_SHOWHGRID|
		RVS_SHOWVGRID|
		//		RVS_SHOWCOLORALTERNATE|
		RVS_SINGLESELECT|
		RVS_NOSORT
		//fndef DISABLE_INPUT
		| RVS_FOCUSSUBITEMS
		//ndif
		);
	
	// 资源ID，或者资源名称。即：IDR_JPG_SPLASH或者153
	// m_rcLockRegion.SetBkImage(IDB_SIM);
	
	m_rcLockRegion.DeleteAllItems();
	
	// 设置listctrl 的edit只能接受数字输入
	//	m_rcLockRegion.SetOnlyDigits(TRUE);
	
	InitLockRegionTable();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropPageLock::InitLockRegionTable()
{
	// 加入报头
	m_rcLockRegion.InsertColumn(0, _T("No"), 
								RVCF_LEFT|
								RVCF_TEXT|
								//								RVCF_EX_AUTOWIDTH|
								//								RVCF_EX_FIXEDWIDTH|
								RVCF_SUBITEM_NOFOCUS,
								40);
	
	m_rcLockRegion.InsertColumn(1, _T("locked Region"),
								RVCF_LEFT|
								RVCF_TEXT|
								RVCF_EX_FIXEDWIDTH,
								125);
	
	// 加入列表项
	char pszTemp[10];
	m_rcLockRegion.DeleteAllItems();
	
	int nValue = GetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM) - 1; 
	for(int i=1; i<nValue; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcLockRegion.InsertItem(i-1,pszTemp);
	}
}

BOOL CPropPageLock::OnCloseTest(void)
{
	// Get Input
	UpdateData(TRUE);
	// 1 -- the check button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_PRVYES))->GetCheck()
		==
		((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_PRVNO))->GetCheck()
		)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
		MessageBox(_T("Please select lock or not lock operator!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	// 2 -- the edit
#if defined(R16_ATC)
	if (!IsEmptyOrNumberString(m_strProvider) || (m_strProvider.GetLength() > 6))//MCC+MNC最多 6 位
#else
	if ((!IsEmptyOrNumberString(m_strProvider)) || (atoi(m_strProvider) > 99) || (atoi(m_strProvider) < 0)/*(m_strProvider.GetLength() != 2)*/)
#endif
	{
		
		((CEdit*)GetDlgItem(IDC_PAGE_9_EDIT_PROVIDER))->SetSel(0,-1);
		
		GetDlgItem(IDC_PAGE_9_EDIT_PROVIDER)->SetFocus();
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
#if defined(R16_ATC)
		MessageBox(_T("The locked Provider no more than 6 chars!"),SYS_NAME,MB_ICONSTOP);
#else
		MessageBox(_T("The locked Provider no more than 2 chars!"),SYS_NAME,MB_ICONSTOP);
#endif
		
		return FALSE;
	}
	
	// 3 -- the check button
	if  ( ((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES))->GetCheck()
		==
		((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONNO))->GetCheck()
		)
	{
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
		MessageBox(_T("Please select wether lock region!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	
	// 5 -- the check button
	if (((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES))->GetCheck() == TRUE)
	{
		if  ( ((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->GetCheck()
			==
			((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKNO))->GetCheck()
			)
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
			MessageBox(_T("Please select wether auto-lock region!"),SYS_NAME,MB_ICONSTOP);
			return FALSE;
		}
	}
	
	// 4 -- the combo box
	if (((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES))->GetCheck() == TRUE)
	{
		if (GetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM) == 0)
		{
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
			m_edtRegionNum.SetFocus();
			// 没有做选择
			
			MessageBox(_T("Please input the count of region!"),SYS_NAME,MB_ICONSTOP);
			
			return FALSE;
		}
	}
	
	// 6 -- the list ctrl
	if (((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES))->GetCheck() == TRUE)
	{
		// 若为否，则不用检测数据
		// DWORD dInput,dMax = 65535;
		CPhoneNumber strInput("");
		CString sError;
		int nValue = GetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM);
		/* 2007.05.26 add by hqr  限制小区数最大不超过 MAXREGIONNUM */
		if (nValue > MAXREGIONNUM)
		{
			sError.Format(_T("count of region no more than %d!"), MAXREGIONNUM);
			MessageBox(sError, SYS_NAME,MB_ICONSTOP);
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
			GetDlgItem(IDC_PAGE_9_EDIT_REGIONNUM)->SetFocus();
			return FALSE;
		}
		/* 2007.05.26 add by hqr  限制小区数最大不超过 MAXREGIONNUM */
		for (int i=0; i< nValue; i++)
		{
			strInput = m_rcLockRegion.GetItemText(i,1);
			if (!strInput.IsEmpty())	// 允许不输入，或输入不足
			{
				/*if (!strInput.IsPhoneNumber())*/
				if (!strInput.IsHexNumber())
				{
					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
					m_rcLockRegion.SetFocus();
					m_rcLockRegion.SetSelection(i);
					
					sError.Format(_T("No.%d:Region must be  hex!"),i+1);
					MessageBox(sError, SYS_NAME,MB_ICONSTOP);
					
					return FALSE;
				}
				
				//dInput = atoi(strInput);
				if (strInput.GetLength() > 4)	// dInput> dMax)		// 若输入了，则不允许大于FFFF
				{
					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
					m_rcLockRegion.SetFocus();
					m_rcLockRegion.SetSelection(i);
					
					sError.Format(_T("No.%d:Region no more than 4 chars"),i+1);
					MessageBox(sError, SYS_NAME,MB_ICONSTOP);
					return FALSE;
				}
				else if ( (strInput == "0") || (strInput == "00") || (strInput == "000") || (strInput == "0000") )
				{
					m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_9_LOCK));
					m_rcLockRegion.SetFocus();
					m_rcLockRegion.SetSelection(i);
					
					sError.Format(_T("No.%d:Region can't be zero!"),i+1);
					MessageBox(sError, SYS_NAME,MB_ICONSTOP);

					return FALSE;
				}
			}
		}
	}
	
	return TRUE;
	
}

void CPropPageLock::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);
	int  nRegionNum      = 0,
		 nIndex          = 0;

	CString szCell       = _T("");
	char    pCell[6]     = "";
	
	// 1 -- 是否锁定运营商
	m_bLockProvider = ((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_PRVYES))->GetCheck();


	m_pParentWnd->szIsServiceProviderLockEnable7002 = m_bLockProvider ? _T("01") : _T("00");
#if defined(R16_ATC)
	m_pParentWnd->szLockedServiceProvider7003     = m_bLockProvider ? m_strProvider : _T("00000");
#else
	m_pParentWnd->szLockedServiceProvider7003     = m_bLockProvider ? m_strProvider : _T("00");
#endif

	// 2. 是否锁定小区
	m_bLockRegion = ((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES))->GetCheck();
	m_pParentWnd->szIsCellLockEnable8000 = m_bLockRegion ? _T("01") : _T("00");

	if (m_bLockRegion)
	{
		// 3. 锁定的小区数
		nRegionNum  = GetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM);
		nRegionNum  = nRegionNum>MAXREGIONNUM ? MAXREGIONNUM : nRegionNum;
		m_pParentWnd->szLockedCellNum8001.Format("%02d", nRegionNum);

		// 4. 是否自动锁小区
		m_bAutoLockRegion = ((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES))->GetCheck();
		m_pParentWnd->szIsCellAutoLockEnable8002 = m_bAutoLockRegion ? _T("01") : _T("00");

		// 5. 锁定的小区号
		for (nIndex=0; nIndex<nRegionNum; nIndex++)
		{
			szCell = m_rcLockRegion.GetItemText(nIndex, 1);
			if (szCell.IsEmpty())
			{
				continue;
			}
			sprintf(pCell, "%s", szCell.Left(4));			
			uintToHexchar((unsigned int)HexstrToInt(pCell), pCell);
			m_pParentWnd->szCellInf8003 += pCell;
		}
		m_pParentWnd->szCellInf8003 = m_pParentWnd->szCellInf8003.Left(nRegionNum*4);
		m_pParentWnd->szCellInf8003.MakeUpper();
	}
	else
	{
		m_bAutoLockRegion                            = FALSE;
		m_pParentWnd->szLockedCellNum8001      = _T("00");
		m_pParentWnd->szIsCellAutoLockEnable8002 = _T("00");
		m_pParentWnd->szCellInf8003.Empty();
	}	
}

void CPropPageLock::OnRefreshPage(void)
{
	BOOL bLockFlag       = FALSE;
	int  nRegionNum      = 0,
		 nIndex          = 0;

	// 1 -- 是否锁定运营商
	bLockFlag = atoi(m_pParentWnd->szIsServiceProviderLockEnable7002);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_PRVYES))->SetCheck(bLockFlag);	
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_PRVNO ))->SetCheck(!bLockFlag);

	// 2. 锁定的运营商号
	GetDlgItem(IDC_PAGE_9_EDIT_PROVIDER)->EnableWindow(bLockFlag);	
	m_strProvider = m_pParentWnd->szLockedServiceProvider7003;

	// 2 -- 是否锁小区
	m_bLockRegion     = atoi(m_pParentWnd->szIsCellLockEnable8000);
	m_bAutoLockRegion = atoi(m_pParentWnd->szIsCellAutoLockEnable8002);

	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES))->SetCheck(m_bLockRegion);
	((CButton*)GetDlgItem(IDC_PAGE_9_RADIO_REGIONNO))->SetCheck(!m_bLockRegion);
	if (!m_bLockRegion)
	{
		OnPage9RadioRegionNo();
		CDialog::UpdateData(FALSE);
		return;
	}
	OnPage9RadioRegionYes();

	nRegionNum = atoi(m_pParentWnd->szLockedCellNum8001);
	nRegionNum = nRegionNum>MAXREGIONNUM ? MAXREGIONNUM : nRegionNum;
	
	SetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM, nRegionNum);
	OnChangeEditRegionNum();
	
	nRegionNum = min(m_pParentWnd->szCellInf8003.GetLength()/4, nRegionNum);
	for (nIndex=0; nIndex<nRegionNum; nIndex++)
		m_rcLockRegion.SetItemText(nIndex, 1,m_pParentWnd->szCellInf8003.Mid(nIndex*4, 4)); 
	CDialog::UpdateData(FALSE);
}


void CPropPageLock::OnChangeEditRegionNum()
{
	int nValue = GetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM);
	int nIndex = nValue - 1;	
	// 加入列表项
	char pszTemp[11];
	int i = 0;
	
	/* 2007.05.25 modify by hqr, 把最大的锁小区数限制在 20 以内 */
	if (nValue > MAXREGIONNUM)
	{
		nValue = MAXREGIONNUM;
		
		SetDlgItemInt(IDC_PAGE_9_EDIT_REGIONNUM, nValue);
	}
	
	m_rcLockRegion.DeleteAllItems();
	for(i=1; i< nIndex+2; i++)
	{
		_itoa( i, pszTemp, 10 );
		m_rcLockRegion.InsertItem(i-1,pszTemp);
	}
	
	// 更改列表项属性
	for(i=1; i< nIndex+2; i++)
	{
		RVITEM rvi;
		rvi.nMask = RVIM_TEXT;
		
		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		
		m_rcLockRegion.SetItem(&rvi);
	}
	
	if (m_pParentWnd && m_pParentWnd->GetSafeHwnd())
		m_pParentWnd->m_bDataChanged = TRUE;
	
}

void CPropPageLock::OnChangeEditProvider()
{
	m_pParentWnd->m_bDataChanged = TRUE;
}


void CPropPageLock::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_PAGE_9_RADIO_PRVYES)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_9_RADIO_PRVNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_9_RADIO_REGIONYES)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_9_RADIO_REGIONNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKNO)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_9_RADIO_AUTOLOCKYES)->EnableWindow(bLock);

	GetDlgItem(IDC_PAGE_9_REPORT_LOCKREGION)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_9_SPIN_REGIONNUM)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_9_EDIT_PROVIDER)->EnableWindow(bLock);

}

