// E:\CvsRoot\ParaDownload\SRC\PAGE\PropPageG166Tcpip.cpp : implementation file
//
//#if !defined(NO_NXT) && defined(CXT_EX)
#include "stdafx.h"
#include "BizPhoneManager.h"
#include ".\PropPageG166Tcpip.h"


// CPropPageG166Tcpip dialog

CPropPageG166Tcpip::CPropPageG166Tcpip(CWnd* pParent /*=NULL*/)
	: CStackPage(CPropPageG166Tcpip::IDD, pParent)
	, m_strPort(_T(""))
{
}

CPropPageG166Tcpip::~CPropPageG166Tcpip()
{
}

void CPropPageG166Tcpip::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_G166_IP, m_ipAr);
	DDX_Control(pDX, IDC_G166_IPPORT, m_ePort);
	DDX_Text(pDX, IDC_G166_IPPORT, m_strPort);
	DDX_Control(pDX, IDC_G166_DNS1, m_dns01);
	DDX_Control(pDX, IDC_G166_DNS2, m_dns02);
}


BEGIN_MESSAGE_MAP(CPropPageG166Tcpip, CStackPage)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_G166_IP, OnIpnFieldchangedG166Ip)
	ON_EN_CHANGE(IDC_G166_IPPORT, OnEnChangeG166Ipport)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_G166_DNS1, OnIpnFieldchangedG166Dns1)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_G166_DNS2, OnIpnFieldchangedG166Dns2)
END_MESSAGE_MAP()


// CPropPageG166Tcpip message handlers
BOOL CPropPageG166Tcpip::OnInitDialog()
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg *)GetParent();
	ASSERT(m_pParentWnd);

	m_ipAr.SetFieldRange(0, 1, 255);
	m_ipAr.SetFieldRange(1, 0, 255);
	m_ipAr.SetFieldRange(2, 0, 255);
	m_ipAr.SetFieldRange(3, 0, 255);
	m_ipAr.SetAddress(1,0,0,0);

	m_dns01.SetFieldRange(0, 1, 255);
	m_dns01.SetFieldRange(1, 0, 255);
	m_dns01.SetFieldRange(2, 0, 255);
	m_dns01.SetFieldRange(3, 0, 255);
	m_dns01.SetAddress(1,0,0,0);

	m_dns02.SetFieldRange(0, 1, 255);
	m_dns02.SetFieldRange(1, 0, 255);
	m_dns02.SetFieldRange(2, 0, 255);
	m_dns02.SetFieldRange(3, 0, 255);
	m_dns02.SetAddress(1,0,0,0);

	return TRUE;
}

BOOL CPropPageG166Tcpip::OnCloseTest(void)
{
	CStackPage::UpdateData(TRUE);

	CString sError;

	BYTE  nField0 = 0,
		  nField1 = 0,
		  nField2 = 0,
		  nField3 = 0;
	m_ipAr.GetAddress(nField0, nField1, nField2, nField3);

	if (m_ipAr.IsBlank()){/* 空白的，提示填写 */
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_ipAr.SetFieldFocus(0);
		sError.Format(_T("请填写IP地址!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField0 > 255) || (nField0 < 1)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_ipAr.SetFieldFocus(0);
		sError.Format(_T("请填写正确的IP地址(Field0)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField1 > 255) || (nField1 < 0)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_ipAr.SetFieldFocus(1);
		sError.Format(_T("请填写正确的IP地址(Field1)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField2 > 255) || (nField2 < 0)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_ipAr.SetFieldFocus(2);
		sError.Format(_T("请填写正确的IP地址(Field2)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField3 > 255) || (nField3 < 1)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_ipAr.SetFieldFocus(3);
		sError.Format(_T("请填写正确的IP地址(Field3)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	if ((m_strPort.GetLength() > 5) ||   // 太长
		(!IsNumberString(m_strPort))){ // 非数字
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_ePort.SetFocus();
		m_ePort.SetSel(0, -1);
		sError.Format(_T("请填写正确的端口号!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
        
	m_dns01.GetAddress(nField0, nField1, nField2, nField3);
	if (m_dns01.IsBlank()){/* 空白的，提示填写 */
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns01.SetFieldFocus(0);
		sError.Format(_T("请填写DNS1!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField0 > 255) || (nField0 < 1)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns01.SetFieldFocus(0);
		sError.Format(_T("请填写正确的DNS1(Field0)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField1 > 255) || (nField1 < 0)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns01.SetFieldFocus(1);
		sError.Format(_T("请填写正确的DNS1(Field1)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField2 > 255) || (nField2 < 0)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns01.SetFieldFocus(2);
		sError.Format(_T("请填写正确的DNS1(Field2)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField3 > 255) || (nField3 < 1)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns01.SetFieldFocus(3);
		sError.Format(_T("请填写正确的DNS1(Field3)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}


	m_dns02.GetAddress(nField0, nField1, nField2, nField3);
	if (m_dns02.IsBlank()){/* 空白的，提示填写 */
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns02.SetFieldFocus(0);
		sError.Format(_T("请填写DNS2!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField0 > 255) || (nField0 < 1)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns02.SetFieldFocus(0);
		sError.Format(_T("请填写正确的DNS2(Field0)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField1 > 255) || (nField1 < 0)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns02.SetFieldFocus(1);
		sError.Format(_T("请填写正确的DNS2(Field1)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField2 > 255) || (nField2 < 0)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns02.SetFieldFocus(2);
		sError.Format(_T("请填写正确的DNS2(Field2)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}
	if ((nField3 > 255) || (nField3 < 1)){
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_G166_TCPIP);
		m_dns02.SetFieldFocus(3);
		sError.Format(_T("请填写正确的DNS2(Field3)!"));
		MessageBox(sError, SYS_NAME,MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

void CPropPageG166Tcpip::OnSaveData(void)
{
	CStackPage::UpdateData(TRUE);
#if !defined(NO_NXT)
	BYTE  nField0 = 0,
		  nField1 = 0,
		  nField2 = 0,
		  nField3 = 0;

	m_ipAr.GetAddress(nField0, nField1, nField2, nField3);
	m_pParentWnd->m_strG166_IpAddress.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);

	m_pParentWnd->m_strG166_IpPort.Format(_T("%d"), atoi(m_strPort));

	m_dns01.GetAddress(nField0, nField1, nField2, nField3);
	m_pParentWnd->m_strG166_Dns1.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);

	m_dns02.GetAddress(nField0, nField1, nField2, nField3);
	m_pParentWnd->m_strG166_Dns2.Format(_T("%d.%d.%d.%d"), nField0, nField1, nField2, nField3);
#endif
	return;
}

void CPropPageG166Tcpip::OnRefreshPage(void)
{
	BYTE  nField0 = 0,
		  nField1 = 0,
		  nField2 = 0,
		  nField3 = 0;
#if !defined(NO_NXT)
	m_strPort = m_pParentWnd->m_strG166_IpPort;

	if (AR_Nfield == 
		GetFieldfromValidIpAddress(nField0, nField1,
		                           nField2, nField3,
								   m_pParentWnd->m_strG166_IpAddress))
	{
		m_ipAr.SetAddress(nField0, nField1, nField2, nField3);
	}else{
		m_ipAr.SetAddress(0,0,0,0);
	}

	if (AR_Nfield == 
		GetFieldfromValidIpAddress(nField0, nField1,
		                           nField2, nField3,
								   m_pParentWnd->m_strG166_Dns1))
	{
		m_dns01.SetAddress(nField0, nField1, nField2, nField3);
	}else{
		m_dns01.SetAddress(0,0,0,0);
	}

	if (AR_Nfield == 
		GetFieldfromValidIpAddress(nField0, nField1,
		                           nField2, nField3,
								   m_pParentWnd->m_strG166_Dns2))
	{
		m_dns02.SetAddress(nField0, nField1, nField2, nField3);
	}else{
		m_dns02.SetAddress(0,0,0,0);
	}
#endif
	CStackPage::UpdateData(FALSE);
	return;
}

void CPropPageG166Tcpip::OnIpnFieldchangedG166Ip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	/* 数据已经更改了，标记全局变量 */
	m_pParentWnd->m_bDataChanged = TRUE;
	*pResult = 0;
}

void CPropPageG166Tcpip::OnEnChangeG166Ipport()
{
	/* 数据已经更改了，标记全局变量 */
	m_pParentWnd->m_bDataChanged = TRUE;
}

void CPropPageG166Tcpip::OnIpnFieldchangedG166Dns1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	/* 数据已经更改了，标记全局变量 */
	m_pParentWnd->m_bDataChanged = TRUE;
	*pResult = 0;
}

void CPropPageG166Tcpip::OnIpnFieldchangedG166Dns2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	/* 数据已经更改了，标记全局变量 */
	m_pParentWnd->m_bDataChanged = TRUE;
	*pResult = 0;
}

ValidIpAddress IsValidIpAddress(const char *szIpAddress)
{/*- xxx.xxx.xxx.xxx -*/
	ValidIpAddress nValid = AR_Valid;

	CString szAr =  szIpAddress;

	BYTE  nField0 = 0,
		  nField1 = 0,
		  nField2 = 0,
		  nField3 = 0;

	if (szAr.GetLength() > (int)AR_Long){
		nValid = AR_Long;
	}else if (szAr.GetLength() > (int)AR_Short){
		nValid = AR_Short;
	}else {
		if ((int)AR_Nfield != GetFieldfromValidIpAddress(nField0,nField1,
			                                        nField2,nField3,
													szIpAddress)){
			nValid = AR_Nfield;
		}else if ((nField0 < 0) || (nField0 > 255)){
			nValid = AR_Field0;
		}else if ((nField1 < 0) || (nField1 > 255)){
			nValid = AR_Field1;
		}else if ((nField2 < 0) || (nField2 > 255)){
			nValid = AR_Field2;
		}else if ((nField3 < 0) || (nField3 > 255)){
			nValid = AR_Field3;
		}else{
			/* OK */
		}
	}
	return nValid;
}

int GetFieldfromValidIpAddress(BYTE& nField0, 
								BYTE& nField1, 
								BYTE& nField2, 
								BYTE& nField3, 
								const char *szValidIpAddress)
{
	int nFields = 0,
		index   = 0;

	CString szField = _T("");

#ifdef _DEBUG_PARA
	FILE *fp = fopen("ip.log", "a+t");
	if (fp){
		fprintf(fp, "[%s]\r", szValidIpAddress);
	}
#endif//_DEBUG_PARA

	nField0 = 0;
	nField1 = 0;
	nField2 = 0;
	nField3 = 0;

	for (;index<(int)strlen(szValidIpAddress); index++){
		if ((szValidIpAddress[index] != (char)'.') &&
		   (index != strlen(szValidIpAddress) - 1)){
			szField += szValidIpAddress[index];
		}else{			
			if (index == strlen(szValidIpAddress) - 1){
				/* 最后一位数字在这里加 */
				szField += szValidIpAddress[index];
			}
			switch (nFields){
				case 0x00:
					nField0 = atoi(szField);
#ifdef _DEBUG_PARA
					if (fp){
						fprintf(fp, "nField0 = %s\r", szField);
					}
#endif//_DEBUG_PARA
					break;
				case 0x01:
					nField1 = atoi(szField);
#ifdef _DEBUG_PARA
					if (fp){
						fprintf(fp, "nField1 = %s\r", szField);
					}
#endif//_DEBUG_PARA
					break;
				case 0x02:
					nField2 = atoi(szField);
#ifdef _DEBUG_PARA
					if (fp){
						fprintf(fp, "nField2 = %s\r", szField);
					}
#endif//_DEBUG_PARA
					break;
				case 0x03:
					nField3 = atoi(szField);
#ifdef _DEBUG_PARA
					if (fp){
						fprintf(fp, "nField3 = %s\r", szField);
					}
#endif//_DEBUG_PARA
					break;
				default:
#ifdef _DEBUG_PARA
					if (fp){
						fclose(fp);
						fp = NULL;
					}
#endif//_DEBUG_PARA
					return 5;
			}
			nFields++;
			szField.Empty();
		}
	}
#ifdef _DEBUG_PARA
	if (fp){
		fclose(fp);
		fp = NULL;
	}
#endif//_DEBUG_PARA
	return nFields;
}
//#endif