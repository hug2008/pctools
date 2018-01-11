// E:\Visual Studio Project\ZeberPrint\SRC\PAGE\LablePg530ImeiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include ".\LablePg530ImeiDlg.h"

// CLablePg530ImeiDlg dialog

CLablePg530ImeiDlg::CLablePg530ImeiDlg(CWnd* pParent /*=NULL*/)
	: CStackPage(CLablePg530ImeiDlg::IDD, pParent)
	, m_szMain(_T(""))
	, m_szHand(_T(""))
	, m_szAnt(_T(""))
	, m_szBat(_T(""))
	, m_szAda(_T(""))
	, m_szUser(_T(""))
	, m_szImei(_T(""))
	, m_szSno(_T(""))
{
	m_nSnoLen             = 0;

	m_szVerifyImeiPrefix  = _T("");

	m_szVerifyImeiPostfix = _T("");

	m_bXaxis              = _T("");

	m_bYaxis              = _T("");

	m_bAfont              = _T("");

	m_bWfont              = _T("");

	m_bHfont              = _T("");

	m_bBcFmt              = _T("");

	m_bBcByRa             = _T("");

	m_bBcByH              = _T("");

	m_szZpl               = _T("");

	m_szImageX            = _T("");

	m_szImageY            = _T("");
}

CLablePg530ImeiDlg::~CLablePg530ImeiDlg()
{
}

void CLablePg530ImeiDlg::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1_IMEI_MODEL, m_cModel);
	DDX_Control(pDX, IDC_ST_IMEI_CONTENTS, m_tContents);
	DDX_Control(pDX, IDC_ET_IMEI_MAINSET, m_eMain);
	//DDX_Text(pDX, IDC_ET_IMEI_MAINSET, m_szMain);
	DDX_Control(pDX, IDC_ET_IMEI_HANDSET, m_eHand);
	//DDX_Text(pDX, IDC_ET_IMEI_HANDSET, m_szHand);
	DDX_Control(pDX, IDC_ET_IMEI_ANT, m_eAnt);
	//DDX_Text(pDX, IDC_ET_IMEI_ANT, m_szAnt);
	DDX_Control(pDX, IDC_ET_IMEI_BAT, m_eBat);
	//DDX_Text(pDX, IDC_ET_IMEI_BAT, m_szBat);
	DDX_Control(pDX, IDC_ET_IMEI_ADA, m_eAda);
	//DDX_Text(pDX, IDC_ET_IMEI_ADA, m_szAda);
	DDX_Control(pDX, IDC_ET_IMEI_USER, m_eUser);
	//DDX_Text(pDX, IDC_ET_IMEI_USER, m_szUser);
	DDX_Control(pDX, IDC_COMBO1_IMEI_COLOR, m_cColor);
	DDX_Control(pDX, IDC_ET_IMEI_IMEI, m_eImei);
	DDX_Text(pDX, IDC_ET_IMEI_IMEI, m_szImei);
	DDX_Control(pDX, IDC_ET_IMEI_SNO, m_eSno);
	DDX_Text(pDX, IDC_ET_IMEI_SNO, m_szSno);
	DDX_Control(pDX, IDC_ST_IMEI_MODEL, m_tModel);
	DDX_Control(pDX, IDC_ST_IMEI_MODEL2, m_tColor);
	DDX_Control(pDX, IDC_ST_IMEI, m_tImei);
	DDX_Control(pDX, IDC_ST_IMEI2, m_tSno);
	DDX_Control(pDX, IDC_COMB_CONTENT_YEAR, m_cYear);
	DDX_Control(pDX, IDC_COMB_CONTENT_MON, m_cMonth);
	DDX_Control(pDX, IDC_CHECK_PG530_MMI, m_cMmi);
}


BEGIN_MESSAGE_MAP(CLablePg530ImeiDlg, CStackPage)
	ON_BN_CLICKED(IDC_CHECK_PG530_MMI, OnBnClickedCheckPg530Mmi)
END_MESSAGE_MAP()


// CLablePg530ImeiDlg message handlers
#ifndef _VK_RETURN_EXIT_
BOOL CLablePg530ImeiDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)	{//键盘消息
		if (VK_RETURN == pMsg->wParam)		{// 回车键
			if (m_eImei.m_hWnd == pMsg->hwnd){
				if (CheckMeSeting()){
					if (PrintMeLable()){
						UpDateMeAfterPrint();
					}
				}
			}
			return TRUE;
		}else{// if (VK_RETURN == pMsg->wParam)	{
			return CStackPage::PreTranslateMessage(pMsg); 
		}// if (VK_RETURN == pMsg->wParam){}else{}
	}else{// if (WM_KEYDOWN == pMsg->message){
		return CStackPage::PreTranslateMessage(pMsg); 
	}// if (WM_KEYDOWN == pMsg->message){}else{}
}
#endif//_VK_RETURN_EXIT_
BOOL CLablePg530ImeiDlg::OnInitDialog()
{
	int     nSel   = 0;
	CString szSel  = _T("");

	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	ASSERT(m_pParentWnd);
    /* AG-169H(PCO)不需要打印这张贴纸，从列表中去除 */
	szSel = theApp.m_Ini.ReadString(SEC_MAINSETTING, IDENT_CONTENTS_MODELNAME, _T("PG530@PG110@PG120"));
	if (PutStringIntoCStringArray(szSel,m_szArray)){
		FillStringIntoComboBox(&m_cModel,m_szArray);
		szSel = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_MODELSEL, _T("00"));
		nSel  = atoi(szSel);
		if (m_cModel.GetCount())
			m_cModel.SetCurSel(nSel % m_cModel.GetCount());
	}
	

	m_tModel.SetWindowText(_T("MODEL:"));
	m_tModel.textColor(RGB(0,0,255));
	m_tModel.setFont(-18);

	m_tContents.SetWindowText(_T("Contents:"));
	m_tContents.textColor(RGB(0,0,255));
	m_tContents.setFont(-18);

	m_tColor.SetWindowText(_T("COLOR:"));
	m_tColor.textColor(RGB(0,0,255));
	m_tColor.setFont(-18);

	m_tImei.SetWindowText(_T("IMEI:"));
	m_tImei.textColor(RGB(0,0,255));
	m_tImei.setFont(-18);

	m_tSno.SetWindowText(_T("S/NO:"));
	m_tSno.textColor(RGB(0,0,255));
	m_tSno.setFont(-18);

	szSel = theApp.m_Ini.ReadString(SEC_MAINSETTING, IDENT_CONTENTS_COLORNAME,    _T("黑 - Black@白 - White"));
	m_szColor = szSel;
	szSel = theApp.m_Ini.ReadString(SEC_MAINSETTING, IDENT_CONTENTS_COLORNAME_SP, _T("黑 - Negro@白 - Blanco"));
	m_szColorSp = szSel;

	if (0 == theApp.m_nMmiIndex)
		szSel = m_szColor;
	else
		szSel = m_szColorSp;

	if (PutStringIntoCStringArray(szSel,m_szArray)){
		FillStringIntoComboBox(&m_cColor,m_szArray);
		szSel = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_COLORSEL, _T("00"));
		nSel  = atoi(szSel);
		if (m_cColor.GetCount())
			m_cColor.SetCurSel(nSel % m_cColor.GetCount());
	}
	

	m_eMain.textColor(RGB(0,0,255));
	m_eMain.setFont(-16);
	m_szMain   = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_MAINSET,    _T("-Main Set"));
	m_szMainSp = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_MAINSET_SP, _T("-Teléfono"));
	//m_eMain.SetWindowText(m_szMain);

	m_eHand.textColor(RGB(0,0,255));
	m_eHand.setFont(-16);
	m_szHand   = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_HANDSET,    _T("-Hand Set"));
	m_szHandSp = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_HANDSET_SP, _T("-Auricular"));
	//m_eHand.SetWindowText(m_szHand);

	m_eAnt.textColor(RGB(0,0,255));
	m_eAnt.setFont(-16);
	m_szAnt   = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_ANTENNA,    _T("-Antenna"));
	m_szAntSp = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_ANTENNA_SP, _T("-Antena"));
	//m_eAnt.SetWindowText(m_szAnt);

	m_eAda.textColor(RGB(0,0,255));
	m_eAda.setFont(-16);
	m_szAda   = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_ADAPTOR,    _T("-Adaptor"));
	m_szAdaSp = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_ADAPTOR_SP, _T("-Adaptador"));
	//m_eAda.SetWindowText(m_szAda);

	m_eBat.textColor(RGB(0,0,255));
	m_eBat.setFont(-16);
	m_szBat   = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_BATTERY,    _T("-Battery"));
	m_szBatSp = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_BATTERY_SP, _T("-Batería"));
	//m_eBat.SetWindowText(m_szBat);

	m_eUser.textColor(RGB(0,0,255));
	m_eUser.setFont(-16);
	m_szUser   = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_USER,    _T("-User Manual"));
	m_szUserSp = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_USER_SP, _T("-Manual de Usuario"));
	//m_eUser.SetWindowText(m_szUser);

	m_eImei.LimitText(IMEI_LENGTH);
	m_eImei.textColor(RGB(255,0,0));
	m_eImei.setFont(-16);

	szSel = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_SNOLEN, _T("05"));
	m_nSnoLen = atoi(szSel);
	m_eSno.LimitText(m_nSnoLen);
	m_eSno.textColor(RGB(255,0,0));
	m_eSno.setFont(-16);
	m_szSno = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_SNO, _T("00001"));
	m_eSno.SetWindowText(m_szSno);

	for (nSel=0; nSel<10; nSel++){
		szSel.Format("20%02d", nSel);
		m_cYear.AddString(szSel);
	}
	szSel = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_YEAR, _T("00"));
	nSel  = atoi(szSel);
	m_cYear.SetCurSel(nSel % m_cYear.GetCount());

	for (nSel=0; nSel<12; nSel++){
		szSel.Format("%02d", nSel+1);
		m_cMonth.AddString(szSel);
	}
	szSel = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_MONTH, _T("00"));
	nSel  = atoi(szSel);
	/* 修正月份当前光标位置计算方法的一个错误 */
	m_cMonth.SetCurSel(nSel % m_cMonth.GetCount());

	m_szVerifyImeiPrefix  = theApp.m_Ini.ReadString(SEC_MAINSETTING, IDENT_CONTENTS_IMEIPREFIXION,  _T(""));
	if (!IsEmptyOrNumberString(m_szVerifyImeiPrefix))
		m_szVerifyImeiPrefix.Empty();

	m_szVerifyImeiPostfix = theApp.m_Ini.ReadString(SEC_MAINSETTING, IDENT_CONTENTS_IMEIPOSTFIXION, _T(""));
	if (!IsEmptyOrNumberString(m_szVerifyImeiPostfix))
		m_szVerifyImeiPostfix.Empty();


	// 图片的位置
	m_szImageX = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_IMAGE_X, _T("00"));
	m_szImageY = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_IMAGE_Y, _T("5"));

	// 打印格式初始化
	m_bXaxis  = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_AXIS_X,  _T("65"));
	m_bYaxis  = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_AXIS_Y,  _T("25"));
	m_bAfont  = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_FONT_A,  _T("^A0N,%d,%d"));
	m_bWfont  = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_FONT_W,  _T("35"));
	m_bHfont  = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_FONT_H,  _T("35"));
	m_bBcFmt  = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_BARCODE, _T("^FO%d,%d^B3N,N,,N,N^FD%s^FS"));
	m_bBcByRa = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_BCBY_RA, _T("^BY2,2,%d"));
	m_bBcByH  = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_BCBY_H,  _T("60"));
	m_bBcSpaceVertical = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_DIS_VTC,  _T("5"));

    
	ExchangeMmi(theApp.m_nMmiIndex);
	m_cMmi.SetCheck(theApp.m_nMmiIndex);

	// 占领个位置
	OnSaveData();

#if !defined(UCS2_ENCODE)
	m_cMmi.ShowWindow(SW_HIDE);
#endif//UCS2_ENCODE

	// edit 控件可用性
	{
		m_eMain.EnableWindow(!theApp.m_bLockPage);
		m_eHand.EnableWindow(!theApp.m_bLockPage);
		m_eAnt.EnableWindow(!theApp.m_bLockPage);
		m_eAda.EnableWindow(!theApp.m_bLockPage);
		m_eBat.EnableWindow(!theApp.m_bLockPage);
		m_eUser.EnableWindow(!theApp.m_bLockPage);
	}

	return TRUE;
}

void CLablePg530ImeiDlg::OnSaveData(void)
{
	CStackPage::UpdateData(TRUE);

	MapMmiViewId(theApp.m_nMmiIndex);

	CString szVal = _T("");

	GetComboBoxItemToStringArray(m_szArray, &m_cModel);
	PrintfCStringArrayToString(szVal, m_szArray);
	theApp.m_Ini.WriteString(SEC_MAINSETTING, IDENT_CONTENTS_MODELNAME, szVal);

	szVal.Format("%02d", m_cModel.GetCurSel());
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_MODELSEL, szVal);

	// 颜色名字列表
	GetComboBoxItemToStringArray(m_szArray, &m_cColor);
	PrintfCStringArrayToString(szVal, m_szArray);
	if (0 == theApp.m_nMmiIndex){
		theApp.m_Ini.WriteString(SEC_MAINSETTING, IDENT_CONTENTS_COLORNAME,    m_szColor);
		theApp.m_Ini.WriteString(SEC_MAINSETTING, IDENT_CONTENTS_COLORNAME_SP, m_szColorSp);
	}else{
		theApp.m_Ini.WriteString(SEC_MAINSETTING, IDENT_CONTENTS_COLORNAME,    m_szColor);
		theApp.m_Ini.WriteString(SEC_MAINSETTING, IDENT_CONTENTS_COLORNAME_SP, m_szColorSp);
	}
	

	// 当前颜色
	szVal.Format("%02d", m_cColor.GetCurSel());
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_COLORSEL, szVal);

	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_MAINSET,    m_szMain);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_MAINSET_SP, m_szMainSp);

	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_HANDSET,    m_szHand);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_HANDSET_SP, m_szHandSp);

	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_ANTENNA,    m_szAnt);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_ANTENNA_SP, m_szAntSp);

	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_ADAPTOR,    m_szAda);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_ADAPTOR_SP, m_szAdaSp);

	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_BATTERY,    m_szBat);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_BATTERY_SP, m_szBatSp);

	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_USER,       m_szUser);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_USER_SP,    m_szUserSp);

	m_eSno.GetWindowText(m_szSno);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_SNO, m_szSno);

	szVal.Format("%02d", m_cYear.GetCurSel());
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_YEAR, szVal);

	szVal.Format("%02d", m_cMonth.GetCurSel());
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_MONTH, szVal);

	szVal.Format("%02d", m_nSnoLen);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_SNOLEN, szVal);

	// 图片的位置
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_IMAGE_X, m_szImageX);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, IDENT_CONTENTS_IMAGE_Y, m_szImageY);

	theApp.m_Ini.WriteString(SEC_MAINSETTING, IDENT_CONTENTS_IMEIPREFIXION,  m_szVerifyImeiPrefix);
	theApp.m_Ini.WriteString(SEC_MAINSETTING, IDENT_CONTENTS_IMEIPOSTFIXION, m_szVerifyImeiPostfix);

	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_AXIS_X,  m_bXaxis);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_AXIS_Y,  m_bYaxis);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_FONT_A,  m_bAfont);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_FONT_W,  m_bWfont);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_FONT_H,  m_bHfont);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_BARCODE, m_bBcFmt);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_BCBY_RA, m_bBcByRa);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_BCBY_H,  m_bBcByH);
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI, ZPL_PG_CONTENT_DIS_VTC, m_bBcSpaceVertical);
}

/* 锁定解锁 */
void CLablePg530ImeiDlg::OnLockPage(BOOL bLock)
{
	m_cModel.EnableWindow(!bLock);
	m_eMain.EnableWindow(!bLock);
	m_eHand.EnableWindow(!bLock);
	m_eAnt.EnableWindow(!bLock);
	m_eAda.EnableWindow(!bLock);
	m_eBat.EnableWindow(!bLock);
	m_eUser.EnableWindow(!bLock);
	m_cColor.EnableWindow(!bLock);
	m_eSno.EnableWindow(!bLock);
	m_cYear.EnableWindow(!bLock);
	m_cMonth.EnableWindow(!bLock);
	m_cMmi.EnableWindow(!bLock);
}


BOOL CLablePg530ImeiDlg::CheckMeSeting(void)
{
	CStackPage::UpdateData(TRUE);

	int nVerifyImeiLen   = 0;
	CString szSnoOverlap = _T(""),
		    szMmiInput   = _T("");

	//MapMmiViewId(theApp.m_nMmiIndex);

	m_cModel.GetWindowText(m_szModel);

	if (!IsEmptyOrAlnumString(m_szModel)){

		theApp.m_szStatus.Format(_T("打印机无法识别的机型[%s]"), m_szModel);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_cModel.SetFocus();

		return FALSE;
	}

	m_eMain.GetWindowText(szMmiInput);
	if (!IsZplDirectValidString(szMmiInput)){

		theApp.m_szStatus.Format(_T("打印机无法识别的字符串[%s]"), szMmiInput);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eMain.SetFocus();

		m_eMain.SetSel(0, -1);

		return FALSE;
	}
	

	m_eHand.GetWindowText(szMmiInput);
	if (!IsZplDirectValidString(szMmiInput)){

		theApp.m_szStatus.Format(_T("打印机无法识别的字符串[%s]"), szMmiInput);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eHand.SetFocus();

		m_eHand.SetSel(0, -1);

		return FALSE;
	}

	m_eAnt.GetWindowText(szMmiInput);
	if (!IsZplDirectValidString(szMmiInput)){

		theApp.m_szStatus.Format(_T("打印机无法识别的字符串[%s]"), szMmiInput);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eAnt.SetFocus();

		m_eAnt.SetSel(0, -1);

		return FALSE;
	}

	m_eAda.GetWindowText(szMmiInput);
	if (!IsZplDirectValidString(szMmiInput)){

		theApp.m_szStatus.Format(_T("打印机无法识别的字符串[%s]"), szMmiInput);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eAda.SetFocus();

		m_eAda.SetSel(0, -1);

		return FALSE;
	}

	m_eBat.GetWindowText(szMmiInput);
	if (!IsZplDirectValidString(szMmiInput)){

		theApp.m_szStatus.Format(_T("打印机无法识别的字符串[%s]"), szMmiInput);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eBat.SetFocus();

		m_eBat.SetSel(0, -1);

		return FALSE;
	}

	m_eUser.GetWindowText(szMmiInput);
	if (!IsZplDirectValidString(szMmiInput)){

		theApp.m_szStatus.Format(_T("打印机无法识别的字符串[%s]"), szMmiInput);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eUser.SetFocus();

		m_eUser.SetSel(0, -1);

		return FALSE;
	}

	m_cColor.GetWindowText(szMmiInput);
	if (!GetPostfixString(szMmiInput, szMmiInput, '-', 1)){

		m_cColor.GetWindowText(szMmiInput);

		theApp.m_szStatus.Format(_T("颜色格式错误[%s]"), szMmiInput);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_cColor.SetFocus();

		return FALSE;
	}

	if ((m_nSnoLen != m_szSno.GetLength()) || (!IsNumberString(m_szSno))){

		theApp.m_szStatus.Format(_T("序列号[%s]只能够是长度为[%d]的数字串"), m_szSno, m_nSnoLen);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eSno.SetFocus();

		m_eSno.SetSel(0, -1);

		return FALSE;
	}

	{// 在此将S/NO填充完毕
		CString szSno = _T("");
		CString szModelName = _T("");
		int nSel            = m_cModel.GetCurSel();
		m_cModel.GetLBText(nSel, szModelName);
		szSno.Format("A%X%d%X%sD", GetModelId(szModelName), m_cYear.GetCurSel(), m_cMonth.GetCurSel()+1, m_szSno);
		m_szSno = szSno;
	}

	if ((IMEI_LENGTH != m_szImei.GetLength()) || (!IsHexString(m_szImei))){

		theApp.m_szStatus.Format(_T("Imei号[%s]只能够是长度为[%d]的 16 进制数字串"), m_szImei, IMEI_LENGTH);

		::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);

		m_szImei.Empty();

		m_eImei.SetWindowText(m_szImei);

		m_eImei.SetFocus();

		m_eImei.SetSel(0, -1);

		return FALSE;
	}

	// Verify Imei with Prefixion 
	nVerifyImeiLen = m_szVerifyImeiPrefix.GetLength();
	if ((IMEI_LENGTH > nVerifyImeiLen) && (nVerifyImeiLen>0)){
		if (m_szImei.Left(nVerifyImeiLen) != m_szVerifyImeiPrefix){

			theApp.m_szStatus.Format(_T("Imei号[%s]的前[%d]位必须是[%s]"), 
				                     m_szImei, nVerifyImeiLen, m_szVerifyImeiPrefix);
			
			::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);
			
			m_szImei.Empty();
			
			m_eImei.SetWindowText(m_szImei);
			
			m_eImei.SetFocus();
			
			m_eImei.SetSel(0, -1);
			
			return FALSE;
		}
	}

	// Verify Imei with Postfixion 
	nVerifyImeiLen = m_szVerifyImeiPostfix.GetLength();
	if ((IMEI_LENGTH > nVerifyImeiLen) && (nVerifyImeiLen>0)){
		if (m_szImei.Left(nVerifyImeiLen) != m_szVerifyImeiPostfix){

			theApp.m_szStatus.Format(_T("Imei号[%s]的后[%d]位必须是[%s]"), 
				                     m_szImei, nVerifyImeiLen, m_szVerifyImeiPostfix);
			
			::SendMessage(m_pParentWnd->m_hWnd, WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, FALSE, NULL);
			
			m_szImei.Empty();
			
			m_eImei.SetWindowText(m_szImei);
			
			m_eImei.SetFocus();
			
			m_eImei.SetSel(0, -1);
			
			return FALSE;
		}
	}

	// IMEI重复性校验
	szSnoOverlap = theApp.m_Ini.ReadString(SEC_CONTENTIMEI_DBLOG, m_szImei, _T(""));
	if (!szSnoOverlap.IsEmpty()){

		theApp.m_szStatus.Format(_T("IMEI[%s]重复, 是否继续？"), m_szImei);

		if (IDYES != MessageBox(theApp.m_szStatus, _T("Zaber Printer"), MB_YESNO|MB_ICONASTERISK)){

			m_szImei.Empty();
			
			m_eImei.SetWindowText(m_szImei);
			
			m_eImei.SetFocus();
			
			m_eImei.SetSel(0, -1);
			
			return FALSE;
		}
	}


	return TRUE;
}

BOOL CLablePg530ImeiDlg::PrintMeLable(void)
{
	CString  szZpl     = _T(""),
		     szMmiCtId = _T("");

	CString  szFontFmt = _T(""),
		     szBcodeFmt= _T(""),
			 szBcodeStr= _T("");

	int nXaxis         = atoi(m_bXaxis),
		nYaxis         = atoi(m_bYaxis);

	int nFontW         = atoi(m_bWfont),
		nFontH         = atoi(m_bHfont);
/*
#if defined(UCS2_ENCODE)
	CHINESEZPL         ChineseZpl;
	char  szUcs2Encode[2048] = _T("");
	ChineseZpl.lpFontName    = _T("黑体");
	ChineseZpl.boolBold      = TRUE;
	ChineseZpl.boolItalic    = FALSE;
	ChineseZpl.intOrient     = 0;
#endif//UCS2_ENCODE
*/
	m_szZpl.Empty();

	// 打印格式定义
	szFontFmt += _T("^FO%d,%d");
	szFontFmt += m_bAfont;
	szFontFmt += _T("^FH\^FD%s^FS");

	// 开始定位
	szZpl.Format(_T("^XA^FO%d,%d"), nXaxis, nYaxis);
	m_szZpl += szZpl;

#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "[Imei:%s]\r", m_szImei);
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG


#if defined (UCS2_ENCODE)
	// 加载图片
	{
		char szImage[1024*512] = _T("");
		if (theApp.m_nMmiIndex > 0){
			if (0 == loadfile(_T("SpImei.prn"), szImage)){
				MessageBox(_T("加载文件SpImei.prn失败!"), _T("严重错误"), MB_OK|MB_ICONSTOP);
				m_szZpl.Empty();
				return FALSE;
			}

			m_szZpl += szImage;
			szZpl.Format(_T("\r^FO%s,%s^XG000.GRF,1,1^FS"), m_szImageX, m_szImageY);
			m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
			if (theApp.m_fLog){
				fprintf(theApp.m_fLog, "%s\r",   szImage);
				fprintf(theApp.m_fLog, "%s\r",   szZpl);
			}
#endif//SEC_ZPL_LOG
		}		
	}
#endif//UCS2_ENCODE

	// 型号说明model
	m_tModel.GetWindowText(szMmiCtId);
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*_T("MODEL: ")*/szMmiCtId + m_szModel);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// Contents 说明:
	m_tContents.GetWindowText(szMmiCtId);
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */	
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*_T("Contents:")*/szMmiCtId);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	nXaxis += 15;
	nFontH = nFontH - 5;
	nFontW = nFontW - 5;

	// Main Set 说明:************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	nFontH = nFontH - 5;
	nFontW = nFontW - 5;
	m_eMain.GetWindowText(szMmiCtId);
#if !defined(UCS2_ENCODE)
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szMain*/szMmiCtId);
	m_szZpl += szZpl;
#else//应用宽字符编码打印
	if (0 == theApp.m_nMmiIndex){
		szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, szMmiCtId);
		m_szZpl += szZpl;
	}else{/*
		ChineseZpl.lpChinese = (LPCSTR)szMmiCtId;
		ChineseZpl.xy.x_axis = nXaxis;
		ChineseZpl.xy.y_axis = nYaxis;
		ChineseZpl.intHeight = nFontH - 5;
		ChineseZpl.intWidth  = nFontW - 5;
		GetChineseZplCode(&ChineseZpl, szUcs2Encode);
		szZpl  = szUcs2Encode;*/
	}
#endif//UCS2_ENCODE
	
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// Hand Set 说明::************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	m_eHand.GetWindowText(szMmiCtId);
#if !defined(UCS2_ENCODE)
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szHand*/szMmiCtId);
	m_szZpl += szZpl;
#else
	if (0 == theApp.m_nMmiIndex){
		szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szHand*/szMmiCtId);
		m_szZpl += szZpl;
	}else{/*
		ChineseZpl.lpChinese = (LPCSTR)szMmiCtId;
		ChineseZpl.xy.x_axis = nXaxis;
		ChineseZpl.xy.y_axis = nYaxis;
		GetChineseZplCode(&ChineseZpl, szUcs2Encode);
		szZpl  = szUcs2Encode;*/
	}
#endif//UCS2_ENCODE
	
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// Antenna 说明:::************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	m_eAnt.GetWindowText(szMmiCtId);
#if !defined(UCS2_ENCODE)
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szAnt*/szMmiCtId);
	m_szZpl += szZpl;
#else
	if (0 == theApp.m_nMmiIndex){
		szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, szMmiCtId);
		m_szZpl += szZpl;
	}else{/*
		ChineseZpl.lpChinese = (LPCSTR)szMmiCtId;
		ChineseZpl.xy.y_axis = nYaxis;
		GetChineseZplCode(&ChineseZpl, szUcs2Encode);
		szZpl  = szUcs2Encode;*/
	}
#endif//UCS2_ENCODE
	
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// Adaptor 说明:::************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	m_eAda.GetWindowText(szMmiCtId);
#if !defined(UCS2_ENCODE)
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szAda*/szMmiCtId);
	m_szZpl += szZpl;
#else
	if (0 == theApp.m_nMmiIndex){
		szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szAda*/szMmiCtId);
		m_szZpl += szZpl;
	}else{/*
		ChineseZpl.lpChinese = (LPCSTR)szMmiCtId;
		ChineseZpl.xy.y_axis = nYaxis;
		GetChineseZplCode(&ChineseZpl, szUcs2Encode);
		szZpl  = szUcs2Encode;*/
	}
#endif//UCS2_ENCODE
	
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// Battery 说明::************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	m_eBat.GetWindowText(szMmiCtId);
#if !defined(UCS2_ENCODE)
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szBat*/szMmiCtId);
	m_szZpl += szZpl;
#else
	if (0 == theApp.m_nMmiIndex){
		szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szBat*/szMmiCtId);
		m_szZpl += szZpl;
	}else{/*
		ChineseZpl.lpChinese = (LPCSTR)szMmiCtId;
		ChineseZpl.xy.y_axis = nYaxis;
		GetChineseZplCode(&ChineseZpl, szUcs2Encode);
		szZpl  = szUcs2Encode;*/
	}
#endif//UCS2_ENCODE

#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// User Manual 说明:************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	m_eUser.GetWindowText(szMmiCtId);
#if !defined(UCS2_ENCODE)
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szUser*/szMmiCtId);
	m_szZpl += szZpl;
#else
	if (0 == theApp.m_nMmiIndex){
		szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, /*m_szUser*/szMmiCtId);
		m_szZpl += szZpl;
	}else{/*
		ChineseZpl.lpChinese = (LPCSTR)szMmiCtId;
		ChineseZpl.xy.y_axis = nYaxis;
		GetChineseZplCode(&ChineseZpl, szUcs2Encode);
		szZpl  = szUcs2Encode;*/
	}
#endif//UCS2_ENCODE

#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	nXaxis -= 15;
	// color颜色说明************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	m_tColor.GetWindowText(szMmiCtId);
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH+5, nFontW+5, szMmiCtId);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG
	//nYaxis = nYaxis + (nFontH + 5) + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	m_cColor.GetWindowText(szMmiCtId);
	GetPostfixString(szMmiCtId, szMmiCtId, '-', 1);
	szZpl.Format(szFontFmt, (nXaxis + 80), nYaxis, nFontH+5, nFontW+5, szMmiCtId);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// Imei 条码************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical) + 15;/* Y + 字体高度 + 间距 */
	szZpl.Format(m_bBcByRa, atoi(m_bBcByH));
	// 打印格式定义
	szBcodeFmt = szZpl + m_bBcFmt;
	szZpl.Format(szBcodeFmt, nXaxis, nYaxis, m_szImei);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG
	// 在条码下面打印Imei的Ascii字符
	nYaxis = nYaxis + atoi(m_bBcByH) + 10;/* Y + 条码高度 + 间距 */
	m_tImei.GetWindowText(szMmiCtId);
	szBcodeStr.Format("%s %s", szMmiCtId, m_szImei);
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, szBcodeStr);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	// S/NO条码************************************************************
	nYaxis = nYaxis + nFontH + atoi(m_bBcSpaceVertical);/* Y + 字体高度 + 间距 */
	szZpl.Format(m_bBcByRa, atoi(m_bBcByH));
	// 打印格式定义
	szBcodeFmt = szZpl + m_bBcFmt;
	szZpl.Format(szBcodeFmt, nXaxis, nYaxis, m_szSno);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG
	// 在条码下面打印S/NO的Ascii字符
	nYaxis = nYaxis + atoi(m_bBcByH) + 10;/* Y + 条码高度 + 间距 */
	m_tSno.GetWindowText(szMmiCtId);
	szBcodeStr.Format("%s %s", szMmiCtId, m_szSno);
	szZpl.Format(szFontFmt, nXaxis, nYaxis, nFontH, nFontW, szBcodeStr);
	m_szZpl += szZpl;
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r",   szZpl);
	}
#endif//SEC_ZPL_LOG


	// 打印份数
	szZpl.Format(_T("^PQ%d,0,1,Y"), atoi(theApp.m_szShare));
	m_szZpl += szZpl;

	// 打印结束
	m_szZpl += _T("^XZ");
#ifdef SEC_ZPL_LOG
	if (theApp.m_fLog){
		fprintf(theApp.m_fLog, "%s\r^XZ\r",   szZpl);
	}
#endif//SEC_ZPL_LOG

	return (ExecutePrintCmdInLpt(m_szZpl, theApp.m_szLpt) == SUCCEED_LPT);
}

void CLablePg530ImeiDlg::UpDateMeAfterPrint(void)
{
	char szSno[32] = _T("");

	m_eSno.GetWindowText(m_szSno);

	strcpy(szSno, m_szSno.Right(m_nSnoLen));

	// Save Items
	theApp.m_Ini.WriteString(SEC_CONTENTIMEI_DBLOG, m_szImei, m_szSno);

	NumStrAutoAdd(szSno);
	
	m_szSno  =  szSno;

	m_szImei.Empty();

	m_eImei.SetFocus();

	m_eImei.SetSel(0, -1);

	m_eSno.SetWindowText(m_szSno);

	m_szZpl.Empty();

	CStackPage::UpdateData(FALSE);

	return;
}

void CLablePg530ImeiDlg::ExchangeMmi(int nMmiIndex)
{
	CStringArray sColorArray;
	CString      szSel = _T("");
	int          nSel  = 0;

	// 当前颜色
	szSel = theApp.m_Ini.ReadString(SEC_CONTENTIMEI, IDENT_CONTENTS_COLORSEL, _T("00"));

	// edit 控件可用性
	m_eMain.EnableWindow(!nMmiIndex);
	m_eHand.EnableWindow(!nMmiIndex);
	m_eAnt.EnableWindow(!nMmiIndex);
	m_eAda.EnableWindow(!nMmiIndex);
	m_eBat.EnableWindow(!nMmiIndex);
	m_eUser.EnableWindow(!nMmiIndex);

	if (0 == nMmiIndex){// english
		m_tModel.SetWindowText(_T("MODEL:"));
		m_tContents.SetWindowText(_T("Contents:"));
		m_tColor.SetWindowText(_T("Color:"));
		m_tImei.SetWindowText(_T("IMEI:"));
		m_tSno.SetWindowText(_T("S/NO:"));
		ClearComboBoxItem(&m_cColor);

		if (PutStringIntoCStringArray(m_szColor,sColorArray)){
			FillStringIntoComboBox(&m_cColor,sColorArray);			
			nSel  = atoi(szSel);
			if (m_cColor.GetCount())
				m_cColor.SetCurSel(nSel % m_cColor.GetCount());
		}

		// edit Id
		m_eMain.SetWindowText(m_szMain);
		m_eHand.SetWindowText(m_szHand);
		m_eAnt.SetWindowText(m_szAnt);
		m_eAda.SetWindowText(m_szAda);
		m_eBat.SetWindowText(m_szBat);
		m_eUser.SetWindowText(m_szUser);

	}else if (1 == nMmiIndex){// spanish
		m_tModel.SetWindowText(_T("Modelo:"));
		m_tContents.SetWindowText(_T("Contenido:"));
		m_tColor.SetWindowText(_T("Color:"));
		m_tImei.SetWindowText(_T("IMEI:"));
		m_tSno.SetWindowText(_T("S/Nro:"));
		ClearComboBoxItem(&m_cColor);

		if (PutStringIntoCStringArray(m_szColorSp,sColorArray)){
			FillStringIntoComboBox(&m_cColor,sColorArray);			
			nSel  = atoi(szSel);
			if (m_cColor.GetCount())
				m_cColor.SetCurSel(nSel % m_cColor.GetCount());
		}

		m_eMain.SetWindowText(m_szMainSp);
		m_eHand.SetWindowText(m_szHandSp);
		m_eAnt.SetWindowText(m_szAntSp);
		m_eAda.SetWindowText(m_szAdaSp);
		m_eBat.SetWindowText(m_szBatSp);
		m_eUser.SetWindowText(m_szUserSp);
		
	}else{
		// 没有这种mmi
	}
}

void CLablePg530ImeiDlg::MapMmiViewId(int nMmiIndex)
{
	if (0 == theApp.m_nMmiIndex){// english
		m_eMain.GetWindowText(m_szMain);
		m_eHand.GetWindowText(m_szHand);
		m_eAnt.GetWindowText(m_szAnt);
		m_eAda.GetWindowText(m_szAda);
		m_eBat.GetWindowText(m_szBat);
		m_eUser.GetWindowText(m_szUser);
	}else if (1 == theApp.m_nMmiIndex){// spanish
		m_eMain.GetWindowText(m_szMainSp);
		m_eHand.GetWindowText(m_szHandSp);
		m_eAnt.GetWindowText(m_szAntSp);
		m_eAda.GetWindowText(m_szAdaSp);
		m_eBat.GetWindowText(m_szBatSp);
		m_eUser.GetWindowText(m_szUserSp);
	}else{
		// none
	}
}

void CLablePg530ImeiDlg::OnBnClickedCheckPg530Mmi()
{
	// TODO: Add your control notification handler code here
	theApp.m_nMmiIndex = m_cMmi.GetCheck();
	ExchangeMmi(theApp.m_nMmiIndex);
}


int GetModelId(const char *szModelName)
{
	CString szName = szModelName;
	if (_T("PG530") == szName){
		return 1;
	}else if (_T("PG110") == szName){
		return 2;
	}else if (_T("PG120") == szName){
		return 3;
	}else if (_T("AG-169H") == szName){
		return 4;
	}else{
	}
	return 0;
}
