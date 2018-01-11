// E:\CvsRoot\bizphonemanage\SRC\PAGE\PropPageScroll.cpp : implementation file
//
//#if !defined(NO_NXT)
#include "stdafx.h"
#include "bizphonemanager.h"
#include "PropPageScroll.h"

#include "StringCheckEncode.h"

#ifdef _DEBUG

#define new DEBUG_NEW

#undef THIS_FILE

static char THIS_FILE[] = __FILE__;

#endif

// CPropPageScroll dialog

CPropPageScroll::CPropPageScroll(CWnd* pParent /*=NULL*/)
: CStackPage(CPropPageScroll::IDD, pParent)
{
	m_strScroll_IssuanceNum01 = _T("");
	
	m_strScroll_IssuanceNum02 = _T("");
	
	m_strScroll_IssuanceNum03 = _T("");
	
	m_strScroll_IssuanceNum04 = _T("");
	
	m_strScroll_IssuanceNum05 = _T("");
	
	m_strScroll_IssuanceNum06 = _T("");
	
	m_strScroll_IssuanceNum07 = _T("");
	
	m_strScroll_HotLine       = _T("");
	
	m_strScroll_NxtNum        = _T("");
	
	m_strScroll_MobileSec     = _T("");
	
	m_strScroll_NxtTitle      = _T("");
	
	m_strScroll_ScrollMatter  = _T("");
	
	m_strScroll_ScrollAd      = _T("");
}


void CPropPageScroll::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_SCROLL_SNUM1, m_strScroll_IssuanceNum01);
	DDV_MaxChars(pDX, m_strScroll_IssuanceNum01, 12);
	
	DDX_Text(pDX, IDC_SCROLL_SNUM2, m_strScroll_IssuanceNum02);
	DDV_MaxChars(pDX, m_strScroll_IssuanceNum02, 12);
	
	DDX_Text(pDX, IDC_SCROLL_SNUM3, m_strScroll_IssuanceNum03);
	DDV_MaxChars(pDX, m_strScroll_IssuanceNum03, 12);
	
	DDX_Text(pDX, IDC_SCROLL_SNUM4, m_strScroll_IssuanceNum04);
	DDV_MaxChars(pDX, m_strScroll_IssuanceNum04, 12);
	
	DDX_Text(pDX, IDC_SCROLL_SNUM5, m_strScroll_IssuanceNum05);
	DDV_MaxChars(pDX, m_strScroll_IssuanceNum05, 12);
	
	DDX_Text(pDX, IDC_SCROLL_SNUM6, m_strScroll_IssuanceNum06);
	DDV_MaxChars(pDX, m_strScroll_IssuanceNum06, 12);
	
	DDX_Text(pDX, IDC_SCROLL_SNUM7, m_strScroll_IssuanceNum07);
	DDV_MaxChars(pDX, m_strScroll_IssuanceNum07, 12);
	
	DDX_Text(pDX, IDC_SCROLL_HOTLINE, m_strScroll_HotLine);
	DDV_MaxChars(pDX, m_strScroll_HotLine, 12);
	
	DDX_Text(pDX, IDC_SCROLL_NXT, m_strScroll_NxtNum);
	DDV_MaxChars(pDX, m_strScroll_NxtNum, 12);
	
	DDX_Text(pDX, IDC_SCROLL_MOBILESEC, m_strScroll_MobileSec);
	DDV_MaxChars(pDX, m_strScroll_MobileSec, 12);
	
	DDX_Text(pDX, IDC_SCROLL_MENUTITLE, m_strScroll_NxtTitle);
	DDV_MaxChars(pDX, m_strScroll_NxtTitle, 20);
	
	DDX_Text(pDX, IDC_SCROLL_MATTER, m_strScroll_ScrollMatter);
	DDV_MaxChars(pDX, m_strScroll_ScrollMatter, 200);
	
	DDX_Text(pDX, IDC_SCROLL_AD, m_strScroll_ScrollAd);
	DDV_MaxChars(pDX, m_strScroll_ScrollAd, 40);
	
}


BEGIN_MESSAGE_MAP(CPropPageScroll, CStackPage)
END_MESSAGE_MAP()


// CPropPageScroll message handlers

BOOL CPropPageScroll::OnInitDialog()
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg *)GetParent();
	
	return TRUE;
	
}

BOOL CPropPageScroll::OnCloseTest(void)
{
	UpdateData(TRUE);
	
	char Src_string[201] = "",
		
		Dst_string[804] = "";
	
	CString     errmsg   = _T("");
	
	if (m_strScroll_NxtTitle.IsEmpty()){
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
		
		((CEdit*)GetDlgItem(IDC_SCROLL_MENUTITLE))->SetSel(0,-1);
		
		GetDlgItem(IDC_SCROLL_MENUTITLE)->SetFocus();
		
		MessageBox(_T("农信通菜单标题不允许为空！"),SYS_NAME,MB_ICONSTOP);
		
		return FALSE;
		
	}
	
	if (m_strScroll_ScrollMatter.IsEmpty()){
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
		
		((CEdit*)GetDlgItem(IDC_SCROLL_MATTER))->SetSel(0,-1);
		
		GetDlgItem(IDC_SCROLL_MATTER)->SetFocus();
		
		MessageBox(_T("滚动条内容不允许为空！"),SYS_NAME,MB_ICONSTOP);
		
		return FALSE;
		
	}
	
	if (m_strScroll_ScrollAd.IsEmpty()){
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
		
		((CEdit*)GetDlgItem(IDC_SCROLL_AD))->SetSel(0,-1);
		
		GetDlgItem(IDC_SCROLL_AD)->SetFocus();
		
		MessageBox(_T("滚动条字头不允许为空！"),SYS_NAME,MB_ICONSTOP);
		
		return FALSE;
		
	}

#if VS_IS_2005
	strcpy_s(Src_string, sizeof(Src_string), m_strScroll_NxtTitle);
#else
	strcpy(Src_string, m_strScroll_NxtTitle);
#endif
	
	AscEncodeHexUcs2(Src_string, Dst_string);
	
	if (MAX_NXTMENU_TITLE_LEN*4 < (int)strlen(Dst_string)){
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
		
		((CEdit*)GetDlgItem(IDC_SCROLL_MENUTITLE))->SetSel(0,-1);
		
		GetDlgItem(IDC_SCROLL_MENUTITLE)->SetFocus();
		
		errmsg.Format(_T("农信通菜单标题长度超过了 %d UCS2！"), MAX_NXTMENU_TITLE_LEN);
		
		MessageBox(errmsg, SYS_NAME,MB_ICONSTOP);
		
		return FALSE;
		
	}
	
	strcpy(Src_string, m_strScroll_ScrollMatter);
	
	AscEncodeHexUcs2(Src_string, Dst_string);
	
	if (MAX_NXTSCROLL_MATTER_LEN*4 < (int)strlen(Dst_string)){
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
		
		((CEdit*)GetDlgItem(IDC_SCROLL_MATTER))->SetSel(0,-1);
		
		GetDlgItem(IDC_SCROLL_MATTER)->SetFocus();
		
		errmsg.Format(_T("滚动条内容长度超过了 %d UCS2！"), MAX_NXTSCROLL_MATTER_LEN);
		
		MessageBox(errmsg, SYS_NAME,MB_ICONSTOP);
		
		return FALSE;
		
	}
	
	strcpy(Src_string, m_strScroll_ScrollAd);
	
	AscEncodeHexUcs2(Src_string, Dst_string);
	
	if (MAX_NXTSCROLL_AD_LEN*4 < (int)strlen(Dst_string)){
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
		
		((CEdit*)GetDlgItem(IDC_SCROLL_AD))->SetSel(0,-1);
		
		GetDlgItem(IDC_SCROLL_AD)->SetFocus();
		
		errmsg.Format(_T("滚动条字头长度超过了 %d UCS2！"), MAX_NXTSCROLL_AD_LEN);
		
		MessageBox(errmsg, SYS_NAME,MB_ICONSTOP);
		
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_IssuanceNum01)){
		
		if (m_strScroll_IssuanceNum01.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_SNUM1))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_SNUM1)->SetFocus();
			
			errmsg.Format(_T("滚动发布号码 1 超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_SNUM1))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_SNUM1)->SetFocus();
        
		errmsg.Format(_T("滚动发布号码 1 不是合法的电话号码：非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_IssuanceNum02)){
		
		if (m_strScroll_IssuanceNum02.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_SNUM2))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_SNUM2)->SetFocus();
			
			errmsg.Format(_T("滚动发布号码 2 超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_SNUM2))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_SNUM2)->SetFocus();
        
		errmsg.Format(_T("滚动发布号码 2 不是合法的电话号码：非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_IssuanceNum03)){
		
		if (m_strScroll_IssuanceNum03.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_SNUM3))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_SNUM3)->SetFocus();
			
			errmsg.Format(_T("滚动发布号码 3 超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_SNUM3))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_SNUM3)->SetFocus();
        
		errmsg.Format(_T("滚动发布号码 3 不是合法的电话号码：非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_IssuanceNum04)){
		
		if (m_strScroll_IssuanceNum04.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_SNUM4))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_SNUM4)->SetFocus();
			
			errmsg.Format(_T("滚动发布号码 4 超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_SNUM4))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_SNUM4)->SetFocus();
        
		errmsg.Format(_T("滚动发布号码 4 不是合法的电话号码：非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_IssuanceNum05)){
		
		if (m_strScroll_IssuanceNum05.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_SNUM5))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_SNUM5)->SetFocus();
			
			errmsg.Format(_T("滚动发布号码 5 超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_SNUM5))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_SNUM5)->SetFocus();
        
		errmsg.Format(_T("滚动发布号码 5 不是合法的电话号码：非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_IssuanceNum06)){
		
		if (m_strScroll_IssuanceNum06.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_SNUM6))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_SNUM6)->SetFocus();
			
			errmsg.Format(_T("滚动发布号码 6 超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_SNUM6))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_SNUM6)->SetFocus();
        
		errmsg.Format(_T("滚动发布号码 6 不是合法的电话号码：非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_IssuanceNum07)){
		
		if (m_strScroll_IssuanceNum07.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_SNUM7))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_SNUM7)->SetFocus();
			
			errmsg.Format(_T("滚动发布号码 7 超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_SNUM7))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_SNUM7)->SetFocus();
        
		errmsg.Format(_T("滚动发布号码 7 不是合法的电话号码：非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	
	if (IsEmptyOrPhoneString(m_strScroll_HotLine)){
		
		if (m_strScroll_HotLine.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_HOTLINE))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_HOTLINE)->SetFocus();
			
			errmsg.Format(_T("客服热线号码超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_HOTLINE))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_HOTLINE)->SetFocus();
        
		errmsg.Format(_T("客服热线号码不是合法的电话号码： 空/非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	if (IsEmptyOrPhoneString(m_strScroll_NxtNum)){
		
		if (m_strScroll_NxtNum.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_NXT))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_NXT)->SetFocus();
			
			errmsg.Format(_T("农信通号码超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_NXT))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_NXT)->SetFocus();
        
		errmsg.Format(_T("农信通号码不是合法的电话号码： 空/非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	
	if (IsEmptyOrPhoneString(m_strScroll_MobileSec)){
		
		if (m_strScroll_MobileSec.GetLength() > MAX_SCROLLNUM_LEN){
			
			m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
            
			((CEdit*)GetDlgItem(IDC_SCROLL_MOBILESEC))->SetSel(0,-1);
            
			GetDlgItem(IDC_SCROLL_MOBILESEC)->SetFocus();
			
			errmsg.Format(_T("移动秘书号码超过了 %d 字符！"), MAX_SCROLLNUM_LEN);
            
			MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
            
			return FALSE;
		}
		
	}else{
		
		m_pParentWnd->SendMessage(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, IDD_PROPPAGE_SCROLL);
        
		((CEdit*)GetDlgItem(IDC_SCROLL_MOBILESEC))->SetSel(0,-1);
        
		GetDlgItem(IDC_SCROLL_MOBILESEC)->SetFocus();
        
		errmsg.Format(_T("移动秘书号码不是合法的电话号码： 空/非数字！"));
        
		MessageBox(errmsg,SYS_NAME,MB_ICONSTOP);
        
		return FALSE;
		
	}
	
	
	return TRUE;
}


void CPropPageScroll::OnSaveData(void)
{
	UpdateData(TRUE);
	
	char Src_string[201] = "",
		
		Dst_string[804] = "";
#if !defined(NO_NXT)
	m_pParentWnd->m_strScroll_1001_HotLine        = m_strScroll_HotLine;
	
	m_pParentWnd->m_strScroll_1002_NxtNum         = m_strScroll_NxtNum;
	
	m_pParentWnd->m_strScroll_1003_MobileSec      = m_strScroll_MobileSec;
	
	m_pParentWnd->m_strScroll_1004_IssuanceNum[0] = m_strScroll_IssuanceNum01;
	
	m_pParentWnd->m_strScroll_1004_IssuanceNum[1] = m_strScroll_IssuanceNum02;
	
	m_pParentWnd->m_strScroll_1004_IssuanceNum[2] = m_strScroll_IssuanceNum03;
	
	m_pParentWnd->m_strScroll_1004_IssuanceNum[3] = m_strScroll_IssuanceNum04;
	
	m_pParentWnd->m_strScroll_1004_IssuanceNum[4] = m_strScroll_IssuanceNum05;
	
	m_pParentWnd->m_strScroll_1004_IssuanceNum[5] = m_strScroll_IssuanceNum06;
	
	m_pParentWnd->m_strScroll_1004_IssuanceNum[6] = m_strScroll_IssuanceNum07;
	
	strcpy(Src_string, m_strScroll_NxtTitle);
	
	AscEncodeHexUcs2(Src_string, Dst_string);
	
	ReverseBytes(Dst_string);
	
	MakeUpperString(Dst_string);
	
	ucharToHexchar((unsigned char)strlen(Dst_string)/4, Src_string);
	
	MakeUpperString(Src_string);
	
	m_pParentWnd->m_strScroll_13000_NxtTitle = Src_string;
	
	m_pParentWnd->m_strScroll_13000_NxtTitle += Dst_string;
	
	strcpy(Src_string, m_strScroll_ScrollMatter);
	
	AscEncodeHexUcs2(Src_string, Dst_string);
	
	ReverseBytes(Dst_string);
	
	MakeUpperString(Dst_string);
	
	ucharToHexchar((unsigned char)strlen(Dst_string)/4, Src_string);
	
	MakeUpperString(Src_string);
	
	m_pParentWnd->m_strScroll_13001_ScrollMatter = Src_string;
	
	m_pParentWnd->m_strScroll_13001_ScrollMatter += Dst_string;
	
	strcpy(Src_string, m_strScroll_ScrollAd);
	
	AscEncodeHexUcs2(Src_string, Dst_string);
	
	ReverseBytes(Dst_string);
	
	MakeUpperString(Dst_string);
	
	ucharToHexchar((unsigned char)strlen(Dst_string)/4, Src_string);
	
	MakeUpperString(Src_string);
	
	m_pParentWnd->m_strScroll_13002_ScrollAd = Src_string;
	
	m_pParentWnd->m_strScroll_13002_ScrollAd += Dst_string;
#endif
	return;
}


void CPropPageScroll::OnRefreshPage(void)
{
	//
	
	char Src_string[804] = "",
		
		Dst_string[201] = "";
#if !defined(NO_NXT)
	
	if (m_pParentWnd->m_strScroll_13000_NxtTitle.GetLength()%4 == 2){
		strcpy(Src_string, m_pParentWnd->m_strScroll_13000_NxtTitle.Mid(2));
	}else if (m_pParentWnd->m_strScroll_13000_NxtTitle.GetLength()%4 == 0){
		strcpy(Src_string, m_pParentWnd->m_strScroll_13000_NxtTitle);
	}else{
		MessageBox(_T("严重错误，无法识别的农信通菜单标题数据格式！"), SYS_NAME,MB_ICONSTOP);
		//直接退出程序
		//m_pParentWnd->AppForceExit();
		exit(0);
	}
	
	ReverseBytes(Src_string);
	
	MakeUpperString(Src_string);
	
	HexUcs2DecodeAsc(Src_string, Dst_string);
	
	m_strScroll_NxtTitle  =  Dst_string;
	
	
	if (m_pParentWnd->m_strScroll_13001_ScrollMatter.GetLength()%4 == 2){
		strcpy(Src_string, m_pParentWnd->m_strScroll_13001_ScrollMatter.Mid(2));
	}else if (m_pParentWnd->m_strScroll_13001_ScrollMatter.GetLength()%4 == 0){
		strcpy(Src_string, m_pParentWnd->m_strScroll_13001_ScrollMatter);
	}else{
		MessageBox(_T("严重错误，无法识别的滚动条内容数据格式！"), SYS_NAME,MB_ICONSTOP);
		//直接退出程序
		//m_pParentWnd->AppForceExit();
		exit(0);
	}
	
	ReverseBytes(Src_string);
	
	MakeUpperString(Src_string);
	
	HexUcs2DecodeAsc(Src_string, Dst_string);
	
	m_strScroll_ScrollMatter  =  Dst_string;
	
	
	if (m_pParentWnd->m_strScroll_13002_ScrollAd.GetLength()%4 == 2){
		strcpy(Src_string, m_pParentWnd->m_strScroll_13002_ScrollAd.Mid(2));
	}else if (m_pParentWnd->m_strScroll_13002_ScrollAd.GetLength()%4 == 0){
		strcpy(Src_string, m_pParentWnd->m_strScroll_13002_ScrollAd);
	}else{
		MessageBox(_T("严重错误，无法识别的滚动条字头数据格式！"), SYS_NAME,MB_ICONSTOP);
		//直接退出程序
		//m_pParentWnd->AppForceExit();
		exit(0);
	}
	
	ReverseBytes(Src_string);
	
	MakeUpperString(Src_string);
	
	HexUcs2DecodeAsc(Src_string, Dst_string);
	
	m_strScroll_ScrollAd  =  Dst_string; 
	
	m_strScroll_HotLine            = m_pParentWnd->m_strScroll_1001_HotLine;
	
	m_strScroll_NxtNum             = m_pParentWnd->m_strScroll_1002_NxtNum;
	
	m_strScroll_MobileSec          = m_pParentWnd->m_strScroll_1003_MobileSec;
	
	m_strScroll_IssuanceNum01      = m_pParentWnd->m_strScroll_1004_IssuanceNum[0];
	
	m_strScroll_IssuanceNum02      = m_pParentWnd->m_strScroll_1004_IssuanceNum[1];
	
	m_strScroll_IssuanceNum03      = m_pParentWnd->m_strScroll_1004_IssuanceNum[2];
	
	m_strScroll_IssuanceNum04      = m_pParentWnd->m_strScroll_1004_IssuanceNum[3];
	
	m_strScroll_IssuanceNum05      = m_pParentWnd->m_strScroll_1004_IssuanceNum[4];
	
	m_strScroll_IssuanceNum06      = m_pParentWnd->m_strScroll_1004_IssuanceNum[5];
	
	m_strScroll_IssuanceNum07      = m_pParentWnd->m_strScroll_1004_IssuanceNum[6];
	
#endif
	UpdateData(FALSE);
	
	return;
}

void CPropPageScroll::OnLockPage(BOOL bLock)
{
	GetDlgItem(IDC_SCROLL_SNUM1)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_SNUM2)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_SNUM3)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_SNUM4)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_SNUM5)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_SNUM6)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_SNUM7)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_HOTLINE)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_NXT)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_MOBILESEC)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_MENUTITLE)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_MATTER)->EnableWindow(bLock);
	GetDlgItem(IDC_SCROLL_AD)->EnableWindow(bLock);
}
//#endif