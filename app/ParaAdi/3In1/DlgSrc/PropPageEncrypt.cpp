// PropPageEncrypt.cpp: implementation of the CPropPageEncrypt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageEncrypt.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPropPageEncrypt::CPropPageEncrypt(CWnd* pParent /*=NULL*/)
: CStackPage(CPropPageEncrypt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPropPageEncrypt)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nEncryptSolution = 0;
}
/*
CPropPageEncrypt::~CPropPageEncrypt()
{

}
*/
void CPropPageEncrypt::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageEncrypt)
	DDX_Control(pDX, IDC_COMBO_ENCRYPTED_SOLUTION, m_cbEncryptMethod);
	DDX_CBIndex(pDX, IDC_COMBO_ENCRYPTED_SOLUTION, m_nEncryptSolution);

    DDX_Text(pDX, IDC_EDIT_VENDOR_NO, m_sVendor);
    DDV_MaxChars(pDX, m_sVendor, 2);
    DDX_Text(pDX, IDC_EDIT_VENDOR_KEYNO, m_skeyno);
    DDV_MaxChars(pDX, m_skeyno, 2);
	DDX_Text(pDX, IDC_EDIT_KEYWORD1, m_skeyword1);
    DDV_MaxChars(pDX, m_skeyword1, 32);
	DDX_Text(pDX, IDC_EDIT_KEYWORD2, m_skeyword2);
    DDV_MaxChars(pDX, m_skeyword2, 32);
	DDX_Text(pDX, IDC_EDIT_KEYWORD3, m_skeyword3);
    DDV_MaxChars(pDX, m_skeyword3, 32);
	DDX_Text(pDX, IDC_EDIT_KEYWORD4, m_skeyword4);
    DDV_MaxChars(pDX, m_skeyword4, 32);
	DDX_Text(pDX, IDC_EDIT_KEYWORD5, m_skeyword5);
    DDV_MaxChars(pDX, m_skeyword5, 32);

//	DDX_CBIndex(pDX, IDC_COMBO_VENDOR, m_nVendor);
	/*
	*	
	DDV_MaxChars(pDX, m_strLocalNum, 6);
	DDX_CBIndex(pDX, IDC_PAGE_2_COMBO_LOCALMETHOD, m_nLocalMethod);
	 */
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageEncrypt, CStackPage)
//{{AFX_MSG_MAP(CPropPageEncrypt)
ON_CBN_SELCHANGE(IDC_COMBO_ENCRYPTED_SOLUTION, OnSelchangeEncryption)
ON_EN_CHANGE(IDC_EDIT_KEYWORD1, OnChangeEditKeyword1)
ON_EN_CHANGE(IDC_EDIT_KEYWORD2, OnChangeEditKeyword2)
ON_EN_CHANGE(IDC_EDIT_KEYWORD3, OnChangeEditKeyword3)
ON_EN_CHANGE(IDC_EDIT_KEYWORD4, OnChangeEditKeyword4)
ON_EN_CHANGE(IDC_EDIT_KEYWORD5, OnChangeEditKeyword5)
ON_EN_CHANGE(IDC_EDIT_VENDOR_NO, OnChangeEditVendorNo)
ON_EN_CHANGE(IDC_EDIT_VENDOR_KEYNO, OnChangeEditKeyNo)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageEncrypt message handlers
BOOL CPropPageEncrypt::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();
	m_skeyword5="00000000000000000000000000000000";
	m_skeyword4="00000000000000000000000000000000";
	m_skeyword3="00000000000000000000000000000000";
	m_skeyword2="00000000000000000000000000000000";
	m_skeyword1="00000000000000000000000000000000";
	m_sVendor = "00";
	m_skeyno = "00";
	CEdit * pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD1));
	pedit->SetWindowText(m_skeyword1);
	pedit->EnableWindow(FALSE);
    
	pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD2));
	pedit->SetWindowText(m_skeyword2);
	pedit->EnableWindow(FALSE);
	
	pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD3));
	pedit->SetWindowText(m_skeyword3);
	pedit->EnableWindow(FALSE);

	pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD4));
	pedit->SetWindowText(m_skeyword4);
	pedit->EnableWindow(FALSE);

	pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD5));
	pedit->SetWindowText(m_skeyword5);
	pedit->EnableWindow(FALSE);

	pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_NO));
	pedit->SetWindowText(m_sVendor);
	pedit->EnableWindow(FALSE);

	pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_KEYNO));
	pedit->SetWindowText(m_skeyno);
	pedit->EnableWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CPropPageEncrypt::OnCloseTest(void)
{
	UpdateData(TRUE);
	//Radio Button
	
	// Vendor?
/*	if (m_nVendor == -1)
	{
	//	m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_16_ENCRYPTED_SOLUTION));
		m_cVendor.SetFocus();
		// 没有做选择
		MessageBox(_T("请选择厂商号！"),SYS_NAME,MB_ICONSTOP);		
		return FALSE;
	}
	*/


	// keyword1
	if (m_skeyword1.GetLength()!=32)
	{
		//m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_16_ENCRYPTED_SOLUTION));
		((CEdit*)GetDlgItem(IDC_EDIT_KEYWORD1))->SetSel(0,-1);
		GetDlgItem(IDC_EDIT_KEYWORD1)->SetFocus();
		
		MessageBox(_T("Key1's length must 32 chars!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	//keyword2
	if (m_skeyword2.GetLength()!=32)
	{
		//m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_16_ENCRYPTED_SOLUTION));
		((CEdit*)GetDlgItem(IDC_EDIT_KEYWORD2))->SetSel(0,-1);
		GetDlgItem(IDC_EDIT_KEYWORD2)->SetFocus();
		
		MessageBox(_T("Key2's length must 32 chars!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	// keyword3
	if (m_skeyword3.GetLength()!=32)
	{
		//m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_16_ENCRYPTED_SOLUTION));
		((CEdit*)GetDlgItem(IDC_EDIT_KEYWORD3))->SetSel(0,-1);
		GetDlgItem(IDC_EDIT_KEYWORD3)->SetFocus();
		
		MessageBox(_T("Key3's length must 32 chars!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	//keyword4
	if (m_skeyword4.GetLength()!=32)
	{
		//m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_16_ENCRYPTED_SOLUTION));
		((CEdit*)GetDlgItem(IDC_EDIT_KEYWORD4))->SetSel(0,-1);
		GetDlgItem(IDC_EDIT_KEYWORD4)->SetFocus();
		
		MessageBox(_T("Key4's length must 32 chars!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	//keyword5
	if (m_skeyword5.GetLength()!=32)
	{
		//m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, (IDD_PROPPAGE_16_ENCRYPTED_SOLUTION));
		((CEdit*)GetDlgItem(IDC_EDIT_KEYWORD5))->SetSel(0,-1);
		GetDlgItem(IDC_EDIT_KEYWORD5)->SetFocus();
		
		MessageBox(_T("Key5's length must 32 chars!"),SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

void CPropPageEncrypt::OnSaveData(void)
{
	// Get Input
	UpdateData(TRUE);

	 //m_nEncryptSolution = 12;
	 if (m_nEncryptSolution == 0)
		m_pParentWnd->szEncryptConfigSimEncryptSolution19.Format("%d", 12);
	 //m_nEncryptSolution = 13;
	 if (m_nEncryptSolution == 1)
		m_pParentWnd->szEncryptConfigSimEncryptSolution19.Format("%d", 13);

#ifdef SIM_ENCRYPT_COMPLETE_SOLUTIONS	
	m_pParentWnd->szEncryptConfigfactoryId17= m_sVendor;
	m_pParentWnd->szEncryptConfigEditionId31= m_skeyno;

	m_pParentWnd->szEncryptConfigMainSimMpkKey689[0] = m_skeyword1;
	m_pParentWnd->szEncryptConfigMainSimMpkKey689[1] = m_skeyword2;
	m_pParentWnd->szEncryptConfigMainSimMpkKey689[2] = m_skeyword3;
	m_pParentWnd->szEncryptConfigMainSimMpkKey689[3] = m_skeyword4;
	m_pParentWnd->szEncryptConfigMainSimMpkKey689[4] = m_skeyword5;
#endif
}

void CPropPageEncrypt::OnRefreshPage(void)
{

	
//	CString strTemp;
	int  temp =0;

	BOOL bEnableCtrl = FALSE;

	// 1 -- the combo box, Encrypted solution
	temp = atoi(m_pParentWnd->szEncryptConfigSimEncryptSolution19);

	if (temp == 12)
	{
		m_nEncryptSolution=0;
		bEnableCtrl = FALSE;

#ifdef SIM_ENCRYPT_COMPLETE_SOLUTIONS
		m_sVendor = "00";
		m_skeyno = "00";

		m_skeyword5="00000000000000000000000000000000";
		m_skeyword4="00000000000000000000000000000000";
		m_skeyword3="00000000000000000000000000000000";
		m_skeyword2="00000000000000000000000000000000";
		m_skeyword1="00000000000000000000000000000000";
#endif
	}
	if (temp == 13)
	{
		m_nEncryptSolution =1;
		bEnableCtrl = TRUE;

	#ifdef SIM_ENCRYPT_COMPLETE_SOLUTIONS
		// 3 -- the edit  --vendor NO.
		m_sVendor  = m_pParentWnd->szEncryptConfigfactoryId17;
		// 4 -- the edit  --key NO.
		m_skeyno  = m_pParentWnd->szEncryptConfigEditionId31;
		
		// 5 -- the edits -- keywords of the Encrypted info.
		m_skeyword1 = m_pParentWnd->szEncryptConfigMainSimMpkKey689[0];
		m_skeyword2 = m_pParentWnd->szEncryptConfigMainSimMpkKey689[1];
		m_skeyword3 = m_pParentWnd->szEncryptConfigMainSimMpkKey689[2];
		m_skeyword4 = m_pParentWnd->szEncryptConfigMainSimMpkKey689[3];
		m_skeyword5 = m_pParentWnd->szEncryptConfigMainSimMpkKey689[4];
	#endif

	}
	
		//	m_nEncryptSolution = atoi(m_pParentWnd->szEncryptConfigSimEncryptSolution19);
	
/*
	m_cbPinLength.EnableWindow(m_bUseRandomPin);
	ASSERT((m_nPinLength>=0)&&(m_nPinLength<=4));	
*/

	CEdit * pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD1));
   pedit->EnableWindow(bEnableCtrl);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD2));
   pedit->EnableWindow(bEnableCtrl);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD3));
   pedit->EnableWindow(bEnableCtrl);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD4));
   pedit->EnableWindow(bEnableCtrl);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD5));
   pedit->EnableWindow(bEnableCtrl);		
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_NO));
   pedit->EnableWindow(bEnableCtrl);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_KEYNO));
   pedit->EnableWindow(bEnableCtrl);

	UpdateData(FALSE);
}

void CPropPageEncrypt::OnSelchangeEncryption()
{
	int temp = m_cbEncryptMethod.GetCurSel();
#ifdef  SIM_ENCRYPT_COMPLETE_SOLUTIONS
	UpdateData(TRUE);

   if (temp==0)
   {
	CEdit * pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD1));
   pedit->EnableWindow(FALSE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD2));
   pedit->EnableWindow(FALSE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD3));
   pedit->EnableWindow(FALSE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD4));
   pedit->EnableWindow(FALSE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD5));
   pedit->EnableWindow(FALSE);		
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_NO));
   pedit->EnableWindow(FALSE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_KEYNO));
   pedit->EnableWindow(FALSE);

	m_skeyword5="00000000000000000000000000000000";
	m_skeyword4="00000000000000000000000000000000";
	m_skeyword3="00000000000000000000000000000000";
	m_skeyword2="00000000000000000000000000000000";
	m_skeyword1="00000000000000000000000000000000";
	m_sVendor = "00";
	m_skeyno = "00";

   }
   if (temp==1)
   {
	CEdit * pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD1));
   pedit->EnableWindow(TRUE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD2));
   pedit->EnableWindow(TRUE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD3));
   pedit->EnableWindow(TRUE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD4));
   pedit->EnableWindow(TRUE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_KEYWORD5));
   pedit->EnableWindow(TRUE);		
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_NO));
   pedit->EnableWindow(TRUE);
   pedit = ((CEdit *)GetDlgItem(IDC_EDIT_VENDOR_KEYNO));
   pedit->EnableWindow(TRUE);


   }


	UpdateData(FALSE);


#endif	
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageEncrypt::OnChangeEditKeyword1()
{
	m_pParentWnd->m_bDataChanged = TRUE;
}
void CPropPageEncrypt::OnChangeEditKeyword2()
{
	m_pParentWnd->m_bDataChanged = TRUE;	
}
void CPropPageEncrypt::OnChangeEditKeyword3()
{
	m_pParentWnd->m_bDataChanged = TRUE;	
}
void CPropPageEncrypt::OnChangeEditKeyword4()
{
	m_pParentWnd->m_bDataChanged = TRUE;	
}
void CPropPageEncrypt::OnChangeEditKeyword5()
{
	m_pParentWnd->m_bDataChanged = TRUE;	
}
void CPropPageEncrypt::OnChangeEditVendorNo()
{
	m_pParentWnd->m_bDataChanged = TRUE;	
}
void CPropPageEncrypt::OnChangeEditKeyNo()
{
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageEncrypt::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_COMBO_ENCRYPTED_SOLUTION)->EnableWindow(bLock);
	GetDlgItem(IDC_EDIT_KEYWORD1)->EnableWindow(bLock);
	GetDlgItem(IDC_EDIT_KEYWORD2)->EnableWindow(bLock);
	GetDlgItem(IDC_EDIT_KEYWORD3)->EnableWindow(bLock);
	GetDlgItem(IDC_EDIT_KEYWORD4)->EnableWindow(bLock);
	GetDlgItem(IDC_EDIT_KEYWORD5)->EnableWindow(bLock);
	GetDlgItem(IDC_EDIT_VENDOR_NO)->EnableWindow(bLock);
	GetDlgItem(IDC_EDIT_VENDOR_KEYNO)->EnableWindow(bLock);
}