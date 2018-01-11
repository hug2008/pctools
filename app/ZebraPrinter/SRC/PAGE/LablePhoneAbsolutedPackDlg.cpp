
#include "stdafx.h"
#include "./BizPhoneManager.h"
#include <LablePhoneAbsolutedPackDlg.h>
#include ".\lablephoneabsolutedpackdlg.h"
#include "CSpreadSheet.h"
#include <QcsDlg.h>

#define SQL_PACKING_IMEI_QUERY  _T("SELECT Serial FROM Packing WHERE Imei=\"%s\"")
#define SQL_PACKING_IMEI_INSERT  _T("INSERT INTO Packing ( Imei, BoxId, Symbol, Model, Color, Weight, Addons, Edition, MEstate, Amount, PEstate, Requisition, OrderForm, Dt)\
VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s');")
//VALUES ('012345678901234', '012345678901234567', 'DESAY', 'GSM169(C) V80', '银白色', '12600', '一电一充', '四川移动', '整机', '10', '满', 'DS030303501', 'NC00001', '2007-12-18');

#define SQL_PACKING_SELETE_BOXID   _T("SELECT * FROM Packing ")
#define SQL_PACKING_SELETE_FIELD    _T("SELECT %s FROM Packing WHERE BoxId=\"%s\";")
// CLablePhoneAbsolutedPackDlg dialog

CLablePhoneAbsolutedPackDlg::CLablePhoneAbsolutedPackDlg(CWnd* pParent /*=NULL*/)
	: CStackPage(CLablePhoneAbsolutedPackDlg::IDD, pParent)
	, m_sBoxId(_T(""))
	, m_sSymbol(_T(""))
	, m_sModel(_T(""))
	, m_sColor(_T(""))
	, m_sWeight(_T(""))
	, m_sAddons(_T(""))
	, m_sEdition(_T(""))
	, m_sMEstate(_T(""))
	, m_sAmount(_T(""))
	, m_sPEstate(_T(""))
	, m_sRequisition(_T(""))
	, m_sOfm(_T(""))
	, m_sImei(_T(""))	
{
}

CLablePhoneAbsolutedPackDlg::~CLablePhoneAbsolutedPackDlg()
{
}

void CLablePhoneAbsolutedPackDlg::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_PHONEIMEI_PACK, m_list);
	DDX_Control(pDX, IDC_ED_PAP_BOXID, m_eBoxId);
	DDX_Text(pDX, IDC_ED_PAP_BOXID, m_sBoxId);
	DDX_Control(pDX, IDC_ED_PAP_SYMBOL, m_eSymbol);
	DDX_Text(pDX, IDC_ED_PAP_SYMBOL, m_sSymbol);
	DDX_Control(pDX, IDC_ED_PAP_MODEL, m_eModel);
	DDX_Text(pDX, IDC_ED_PAP_MODEL, m_sModel);
	DDX_Control(pDX, IDC_ED_PAP_COLOR, m_eColor);
	DDX_Text(pDX, IDC_ED_PAP_COLOR, m_sColor);
	DDX_Control(pDX, IDC_ED_PAP_WEIGHT, m_eWeight);
	DDX_Text(pDX, IDC_ED_PAP_WEIGHT, m_sWeight);
	DDX_Control(pDX, IDC_ED_PAP_ADDONS, m_eAddons);
	DDX_Text(pDX, IDC_ED_PAP_ADDONS, m_sAddons);
	DDX_Control(pDX, IDC_ED_PAP_EDITION, m_eEdition);
	DDX_Text(pDX, IDC_ED_PAP_EDITION, m_sEdition);
	DDX_Control(pDX, IDC_ED_PAP_MESTATE, m_eMEstate);
	DDX_Text(pDX, IDC_ED_PAP_MESTATE, m_sMEstate);
	DDX_Control(pDX, IDC_ED_PAP_AMOUNT, m_eAmount);
	DDX_Text(pDX, IDC_ED_PAP_AMOUNT, m_sAmount);
	DDX_Control(pDX, IDC_ED_PAP_PESTATE, m_ePEstate);
	DDX_Text(pDX, IDC_ED_PAP_PESTATE, m_sPEstate);
	DDX_Control(pDX, IDC_ED_PAP_REQ, m_eRequisition);
	DDX_Text(pDX, IDC_ED_PAP_REQ, m_sRequisition);
	DDX_Control(pDX, IDC_ED_PAP_OFORM, m_eOfm);
	DDX_Text(pDX, IDC_ED_PAP_OFORM, m_sOfm);
	DDX_Control(pDX, IDC_ED_PAP_IMEI, m_eImei);
	DDX_Text(pDX, IDC_ED_PAP_IMEI, m_sImei);
	DDX_Control(pDX, IDC_BUT_PAP_CLEAR_LIST, m_bClear);
	DDX_Control(pDX, IDC_BUT_PAP_QUERY,      m_bQuery);
	DDX_Control(pDX, IDC_BUT_PAP_RE_PRINT,   m_bReprint);
}


BEGIN_MESSAGE_MAP(CLablePhoneAbsolutedPackDlg, CStackPage)
	ON_BN_CLICKED(IDC_BUT_PAP_CLEAR_LIST, OnBnClickedButPapClearList)
	ON_BN_CLICKED(IDC_BUT_PAP_QUERY, OnBnClickedButPapQuery)
	ON_BN_CLICKED(IDC_BUT_PAP_RE_PRINT, OnBnClickedButPapRePrint)
END_MESSAGE_MAP()


// CLablePhoneAbsolutedPackDlg message handlers
#ifndef _VK_RETURN_EXIT_
BOOL CLablePhoneAbsolutedPackDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)	{//键盘消息
		if (VK_RETURN == pMsg->wParam)		{// 回车键
			if (pMsg->hwnd == m_eImei.m_hWnd){
				ProcessWmsgImeiEdit(pMsg);
			}
			if (pMsg->hwnd == m_eAmount.m_hWnd){
				ProcessWmsgAmountEdit(pMsg);
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


BOOL CLablePhoneAbsolutedPackDlg::OnInitDialog()
{
	CStackPage::OnInitDialog();

	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	ASSERT(m_pParentWnd);

	m_list.ModifyStyle(0, RVS_SHOWHGRID|RVS_SHOWVGRID|RVS_SINGLESELECT|RVS_NOSORT/*|RVS_FOCUSSUBITEMS*/);

	InitControlId();

	OnSaveData();

	return TRUE;
}

void CLablePhoneAbsolutedPackDlg::OnSaveData(void)
{
	CString szVal = _T("");

	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_BOXID_TITLE_TEXT, m_packingCoordinate.packingTitle.szBoxId);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_BOXID_STR_LEN,    m_packingCoordinate.packingTitle.szBoxIdLen);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_BOXID_STR_TEXT,   m_sBoxId);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_SYMBOL_TITLE_TEXT,m_packingCoordinate.packingTitle.szSymbol);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_SYMBOL_STR_TEXT,  m_sSymbol);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MODEL_TITLE_TEXT, m_packingCoordinate.packingTitle.szModel);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MODEL_STR_TEXT,   m_sModel);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_COLOR_TITLE_TEXT, m_packingCoordinate.packingTitle.szColor);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_COLOR_STR_TEXT,   m_sColor);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_WEIGHT_TITLE_TEXT,m_packingCoordinate.packingTitle.szWeight);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_WEIGHT_STR_TEXT,  m_sWeight);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_ADDONS_TITLE_TEXT, m_packingCoordinate.packingTitle.szAddons);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_ADDONS_STR_TEXT,   m_sAddons);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_EDITION_TITLE_TEXT, m_packingCoordinate.packingTitle.szEdition);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_EDITION_STR_TEXT,   m_sEdition);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MESTATE_TITLE_TEXT, m_packingCoordinate.packingTitle.szMEstate);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MESTATE_STR_TEXT,   m_sMEstate);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_AMOUNT_TITLE_TEXT,  m_packingCoordinate.packingTitle.szAmount);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_AMOUNT_STR_TEXT,    m_sAmount);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PESTATE_TITLE_TEXT, m_packingCoordinate.packingTitle.szPEstate);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PESTATE_STR_TEXT,   m_sPEstate);

	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_OFM_STR_TEXT, m_sOfm);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_REQ_TITLE_TEXT, m_packingCoordinate.packingTitle.szRequisition);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_REQ_STR_LEN,    m_packingCoordinate.packingTitle.szRequisitionLen);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_REQ_STR_TEXT,   m_sRequisition);

	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_TITLE_DATE, m_packingCoordinate.packingTitle.szDate);
	
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_PREFIX_STR, m_packingCoordinate.packingTitle.szImeiPrefix);
	
	szVal.Format("%02d", m_packingCoordinate.xyStartPoint.x_axis);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_CD_XY_X, szVal);
	szVal.Format("%02d", m_packingCoordinate.xyStartPoint.y_axis);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_CD_XY_Y, szVal);

	szVal.Format("%02d", m_packingCoordinate.imeiBarby.heighet);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_BAR_HEIGHT, szVal);
	szVal.Format("%02d", m_packingCoordinate.imeiBarby.width);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_BAR_WIDTH, szVal);
	szVal.Format("%2.1f", m_packingCoordinate.imeiBarby.width_ratio);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_BAR_RATIO, szVal);

	//szVal = m_packingCoordinate.chineseZpl.lpFontName;
	//theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_NAME, szVal);
	szVal.Format("%02d", m_packingCoordinate.chineseZpl.intHeight);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_HEIGHT,  szVal);
	szVal.Format("%02d", m_packingCoordinate.chineseZpl.intWidth);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_WIDTH,  szVal);
	szVal.Format("%02d", m_packingCoordinate.chineseZpl.intOrient);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_ORIENT,  szVal);
	szVal.Format("%02d", m_packingCoordinate.chineseZpl.boolBold);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_BOID,   szVal);
	szVal.Format("%02d", m_packingCoordinate.chineseZpl.boolItalic);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_ITALIC,   szVal);
	szVal.Format("%02d", m_packingCoordinate.chineseZpl.xmf);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_XMF,   szVal);
	szVal.Format("%02d", m_packingCoordinate.chineseZpl.ymf);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_YMF,   szVal);

	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_IMEI_LEVEL_DIS,   m_packingCoordinate.szImeiLevelDis);
	theApp.m_Ini.WriteString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_TITLE_LEVEL_DIS,  m_packingCoordinate.szTitleLevelDis);

	return;
}

void CLablePhoneAbsolutedPackDlg::OnLockPage(BOOL bLock)
{
	m_eBoxId.EnableWindow(!bLock);
	m_eSymbol.EnableWindow(!bLock);
	m_eModel.EnableWindow(!bLock);
	m_eColor.EnableWindow(!bLock);
	m_eWeight.EnableWindow(!bLock);
	m_eAddons.EnableWindow(!bLock);
	m_eEdition.EnableWindow(!bLock);
	m_eMEstate.EnableWindow(!bLock);
	m_eAmount.EnableWindow(!bLock);
	m_ePEstate.EnableWindow(!bLock);
	m_eRequisition.EnableWindow(!bLock);
	m_bClear.EnableWindow(!bLock);
	m_bQuery.EnableWindow(!bLock);
	m_bReprint.EnableWindow(!bLock);

	return;
}

BOOL CLablePhoneAbsolutedPackDlg::CheckMeSeting(void)
{
	CStackPage::UpdateData(TRUE);
	CString szErr =_T("");

	// 1. 箱号为18位数字
	if (m_sBoxId.GetLength() != atoi(m_packingCoordinate.packingTitle.szBoxIdLen)){
		szErr.Format("BoxId Len must be %d chars!", atoi(m_packingCoordinate.packingTitle.szBoxIdLen));
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_sImei.Empty();
		m_eImei.SetWindowText(m_sImei);
		m_eBoxId.SetFocus();
		m_eBoxId.SetSel(0, m_sBoxId.GetLength());
		return FALSE;
	}

	if (!IsNumberString(m_sBoxId)){
		szErr.Format("BoxId must be number {0,1,...,9}!");
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_sImei.Empty();
		m_eImei.SetWindowText(m_sImei);
		m_eBoxId.SetFocus();
		m_eBoxId.SetSel(0, m_sBoxId.GetLength());
		return FALSE;
	}

	// 2. 代码 -- 必须是可打印字符串
	if (!IsPrintString(m_sSymbol)){
		szErr.Format("Symbol's char must between 0x20 and 0x7E, printed chars!");
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_sImei.Empty();
		m_eImei.SetWindowText(m_sImei);
		m_eSymbol.SetFocus();
		m_eSymbol.SetSel(0, m_sSymbol.GetLength());
		return FALSE;
	}

	// 3. 型号 -- 没有什么好检查的
	// 4. 颜色 -- 没有什么好检查的
	// 5. 重量 -- 必须是数字
	if (!IsEmptyOrNumberString(m_sWeight)){
		szErr.Format("Weight must be number {0,1,...,9}!");
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_sImei.Empty();
		m_eImei.SetWindowText(m_sImei);
		m_eWeight.SetFocus();
		m_eWeight.SetSel(0, m_sWeight.GetLength());
		return FALSE;
	}

	// 6. 数量必须是数字，而且要小于20
	if (!IsEmptyOrNumberString(m_sAmount)){
		szErr.Format("Amount must be number {0,1,...,9}!");
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_sImei.Empty();
		m_eImei.SetWindowText(m_sImei);
		m_eAmount.SetFocus();
		m_eAmount.SetSel(0, m_sAmount.GetLength());
		return FALSE;
	}

	if (atoi(m_sAmount) > 20){
		szErr.Format("Amount must be less than 20!");
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_sImei.Empty();
		m_eImei.SetWindowText(m_sImei);
		m_eAmount.SetFocus();
		m_eAmount.SetSel(0, m_sAmount.GetLength());
		return FALSE;
	}

	// 7. 数量不能够小于表格的记录
	if (atoi(m_sAmount) < m_list.GetItemCount()){
		szErr.Format("There are %d Items in the list, it's more than Amount(%02d)!", m_list.GetItemCount(), atoi(m_sAmount));
		m_sImei.Empty();
		m_eImei.SetWindowText(m_sImei);
		m_list.SetFocus();
		m_list.SetSelection(atoi(m_sAmount));
		return FALSE;
	}

	return TRUE;
}

BOOL CLablePhoneAbsolutedPackDlg::PrintMeLable(void)
{
	CStackPage::UpdateData(TRUE);
	GetlistData();
	PackingPrintSet packingPrintSet;
	packingPrintSet.szImeiArray.Copy(this->m_imeiArray);
	packingPrintSet.szBoxId    = this->m_sBoxId;
	packingPrintSet.szSymbol   = this->m_sSymbol;
	packingPrintSet.szModel    = this->m_sModel;
	packingPrintSet.szColor    = this->m_sColor;
	packingPrintSet.szAddons   = this->m_sAddons;
	packingPrintSet.szWeight   = this->m_sWeight;
	packingPrintSet.szEdition  = this->m_sEdition;
	packingPrintSet.szMEstate  = this->m_sMEstate;
	packingPrintSet.szAmount   = this->m_sAmount;
	packingPrintSet.szPEstate  = this->m_sPEstate;
	packingPrintSet.szRequisition = this->m_sRequisition;
	packingPrintSet.szOrderForm   = this->m_sOfm;

	return DrawPrintLable(packingPrintSet, this->m_packingCoordinate);
}
void CLablePhoneAbsolutedPackDlg::UpDateMeAfterPrint(void)
{
	char *pId = (char *)malloc(sizeof(char) * (m_sBoxId.GetLength() + 1));
	assert(pId);

	// 清空列表
	m_list.DeleteAllItems();

	// 保存数据到数据库, 清空数组
	SaveListData();
	m_imeiArray.RemoveAll();

	// 箱号自增1
	sprintf(pId, "%s", m_sBoxId);
	NumStrAutoAdd(pId);
	m_sBoxId = pId;
	free(pId);
	pId = NULL;
	m_eBoxId.SetWindowText(m_sBoxId);

	return;
}
void CLablePhoneAbsolutedPackDlg::ProcessWmsgImeiEdit(MSG* pImeiMsg)
{
	// 取数量的值
	int nAmount = 0;
	m_eAmount.GetWindowText(m_sAmount);
	nAmount = atoi(m_sAmount);

	// 其他设置检测
	if (!CheckMeSeting()){
		return;
	}

	// 取出imei串
	m_eImei.GetWindowText(m_sImei);
	// imei 有效性检测
	if (!isValidImeiString(m_sImei)){
		return;
	}

	if (nAmount == GetlistData()){
		if (PrintMeLable()){
			UpDateMeAfterPrint();
		}else{
			MessageBox(_T("Print failed, please check your zebra printer!"), _T("Err"), MB_OK|MB_ICONSTOP);
		}
	}else{
		// 向列表增加记录
		if (nAmount == InsertItemToList(m_sImei)){
			//ASSERT(nAmount == GetlistData());
			// print lable
			if (PrintMeLable()){
				UpDateMeAfterPrint();
			}else{
				MessageBox(_T("Print failed, please check your zebra printer!"), _T("Err"), MB_OK|MB_ICONSTOP);
			}
		}
	}
	

	m_sImei.Empty();
	m_eImei.SetWindowText(m_sImei);
	m_eImei.SetFocus();
	m_eImei.SetSel(0, 0);

	return;
}

void CLablePhoneAbsolutedPackDlg::ProcessWmsgAmountEdit(MSG* pImeiMsg)
{
	m_eAmount.GetWindowText(m_sAmount);

	int count = atoi(m_sAmount);

	if (count < 1){
		count = 0;
	}
	if (count > 20){
		count = 20;
	}

	m_sAmount.Format("%02d", count);
	m_eAmount.SetWindowText(m_sAmount);
	m_eAmount.SetSel(0, m_sAmount.GetLength());

	RefreshList(count);

	return;
}

void CLablePhoneAbsolutedPackDlg::InitControlId(void)
{
	CString szVal = _T("");
	m_list.InsertColumn(0, _T(" No "), RVCF_CENTER
		                            |RVCF_TEXT
									|RVCF_EX_AUTOWIDTH
									|RVCF_EX_FIXEDWIDTH
									|RVCF_SUBITEM_NOFOCUS);

	m_list.InsertColumn(1, _T("Imei"), RVCF_CENTER
		                               |RVCF_TEXT
									   |RVCF_EX_FIXEDWIDTH,
									   150);

	// Read title form ini
	m_packingCoordinate.packingTitle.szBoxId    = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_BOXID_TITLE_TEXT,_T("箱号:"));
	m_packingCoordinate.packingTitle.szBoxIdLen = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_BOXID_STR_LEN,   _T("18"));
	{
		CString str('8', atoi(m_packingCoordinate.packingTitle.szBoxIdLen));
		this->m_sBoxId      = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_BOXID_STR_TEXT,  str);
	}

	m_packingCoordinate.packingTitle.szSymbol   = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_SYMBOL_TITLE_TEXT,_T("代码:"));
	this->m_sSymbol         = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_SYMBOL_STR_TEXT,  _T("DESAY"));

	m_packingCoordinate.packingTitle.szModel    = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MODEL_TITLE_TEXT,_T("型号:"));
	this->m_sModel          = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MODEL_STR_TEXT,  _T("GSM169(C)V90"));

	m_packingCoordinate.packingTitle.szColor    = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_COLOR_TITLE_TEXT,_T("颜色:"));
	this->m_sColor          = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_COLOR_STR_TEXT,  _T("银白色"));

	m_packingCoordinate.packingTitle.szWeight   = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_WEIGHT_TITLE_TEXT,_T("重量:"));
	this->m_sWeight         = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_WEIGHT_STR_TEXT,  _T("12600"));

	m_packingCoordinate.packingTitle.szAddons   = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_ADDONS_TITLE_TEXT,_T("配件:"));
	this->m_sAddons         = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_ADDONS_STR_TEXT,_T("一电一充"));

	m_packingCoordinate.packingTitle.szEdition  = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_EDITION_TITLE_TEXT,_T("区域:"));
	this->m_sEdition        = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_EDITION_STR_TEXT,  _T("四川移动"));

	m_packingCoordinate.packingTitle.szMEstate  = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MESTATE_TITLE_TEXT,_T("机型状态:"));
	this->m_sMEstate        = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_MESTATE_STR_TEXT,  _T("整机"));

	m_packingCoordinate.packingTitle.szAmount   = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_AMOUNT_TITLE_TEXT,_T("数量:"));
	this->m_sAmount         = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_AMOUNT_STR_TEXT,  _T("10"));

	m_packingCoordinate.packingTitle.szPEstate  = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PESTATE_TITLE_TEXT,_T("装箱状态:"));
	this->m_sPEstate        = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PESTATE_STR_TEXT,  _T("满"));

	m_packingCoordinate.packingTitle.szRequisition = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_REQ_TITLE_TEXT,_T("送检查通知单:"));
	m_packingCoordinate.packingTitle.szRequisitionLen = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_REQ_STR_LEN,   _T("11"));
	this->m_sRequisition  = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_REQ_STR_TEXT,_T("DS080501000"));

	m_packingCoordinate.packingTitle.szDate = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_TITLE_DATE, _T("日期:"));

	m_sOfm = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_OFM_STR_TEXT, _T("NC0001"));


	m_packingCoordinate.packingTitle.szImeiPrefix = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_PREFIX_STR,_T("35"));

	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_CD_XY_X, _T("60"));
	m_packingCoordinate.xyStartPoint.x_axis = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_CD_XY_Y, _T("40"));
	m_packingCoordinate.xyStartPoint.y_axis = atoi(szVal);

	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_BAR_HEIGHT, _T("60"));
	m_packingCoordinate.imeiBarby.heighet = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_BAR_WIDTH,  _T("2"));
	m_packingCoordinate.imeiBarby.width = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_IMEI_BAR_RATIO,  _T("2.0"));
	m_packingCoordinate.imeiBarby.width_ratio = atof(szVal);

	//m_packingCoordinate.chineseZpl.lpFontName = (LPCSTR)theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_NAME,_T("宋体"));
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_HEIGHT,  _T("70"));
	m_packingCoordinate.chineseZpl.intHeight  = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_WIDTH,  _T("00"));
	m_packingCoordinate.chineseZpl.intWidth   = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_ORIENT,  _T("00"));
	m_packingCoordinate.chineseZpl.intOrient   = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_BOID,   _T("01"));
	m_packingCoordinate.chineseZpl.boolBold   = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_ITALIC,   _T("00"));
	m_packingCoordinate.chineseZpl.boolItalic = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_XMF,   _T("01"));
	m_packingCoordinate.chineseZpl.xmf   = atoi(szVal);
	szVal = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_FONT_YMF,   _T("01"));
	m_packingCoordinate.chineseZpl.ymf = atoi(szVal);

	m_packingCoordinate.szImeiLevelDis = theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_IMEI_LEVEL_DIS,  _T("600"));
	m_packingCoordinate.szTitleLevelDis= theApp.m_Ini.ReadString(SEC_PHONE_ABSOLUTE_PACK, IDENT_PACK_TITLE_LEVEL_DIS, _T("700"));

	

	// 样式设置
	m_eBoxId.setFont(-16);
	m_eSymbol.setFont(-16);
	m_eModel.setFont(-16);
	m_eColor.setFont(-16);
	m_eWeight.setFont(-16);
	m_eAddons.setFont(-16);
	m_eEdition.setFont(-16);
	m_eMEstate.setFont(-16);
	m_eAmount.setFont(-16);
	m_ePEstate.setFont(-16);
	m_eRequisition.setFont(-16);
	m_eOfm.setFont(-16);
	m_eImei.setFont(-22);
	m_eImei.textColor(RGB(255,0,0));

	// Tip
	szVal.Format(_T("箱号：%02d位阿拉伯数字，自增 1 。"), atoi(m_packingCoordinate.packingTitle.szBoxIdLen));
	m_eBoxId.SetToolTipText(szVal);
	m_eSymbol.SetToolTipText(_T("代码"));
	m_eModel.SetToolTipText(_T("机型"));
	m_eColor.SetToolTipText(_T("颜色"));
	m_eWeight.SetToolTipText(_T("重量：（克）"));
	m_eAddons.SetToolTipText(_T("配件"));
	m_eEdition.SetToolTipText(_T("区域"));
	m_eMEstate.SetToolTipText(_T("机型状态"));
	m_eAmount.SetToolTipText(_T("数量：[0, 20]之间，输入后回车确认。"));
	m_ePEstate.SetToolTipText(_T("装箱状态"));
	m_eRequisition.SetToolTipText(_T("送检查通知单，不自增。"));
	m_eOfm.SetToolTipText(_T("工单号"));

	if (m_packingCoordinate.packingTitle.szImeiPrefix.IsEmpty()){
		szVal.Format("Imei号：15位长度，输入后回车确认。");
	}else{
		szVal.Format("Imei号：15位16进制字符，必须以 \"%s\" 开头，输入后回车确认。", m_packingCoordinate.packingTitle.szImeiPrefix);
	}	
	m_eImei.SetToolTipText(szVal);
	m_bClear.SetToolTipText(_T("清空列表数据"));
	m_bQuery.SetToolTipText(_T("查询数据库记录"));
	m_bReprint.SetToolTipText(_T("重打此箱"));

	// 显示到界面上
	m_eBoxId.SetWindowText(m_sBoxId);
	m_eSymbol.SetWindowText(m_sSymbol);
	m_eModel.SetWindowText(m_sModel);
	m_eColor.SetWindowText(m_sColor);
	m_eWeight.SetWindowText(m_sWeight);
	m_eAddons.SetWindowText(m_sAddons);
	m_eEdition.SetWindowText(m_sEdition);
	m_eMEstate.SetWindowText(m_sMEstate);
	m_eAmount.SetWindowText(m_sAmount);
	m_ePEstate.SetWindowText(m_sPEstate);
	m_eRequisition.SetWindowText(m_sRequisition);
	m_eOfm.SetWindowText(m_sOfm);

	// 加入列表项
	for(int i=1; i<=atoi(m_sAmount); i++)
	{
		szVal.Format("%02d", i);
		m_list.InsertItem(i-1,szVal);
	}

	// 更改列表项属性
	for(i=1; i<=atoi(m_sAmount); i++)
	{
		RVITEM rvi;
		rvi.nMask = RVIM_TEXT;

		rvi.iItem = i-1;
		rvi.iSubItem = 1;
		rvi.lpszText = _T("");
		m_list.SetItem(&rvi);
	}

	m_list.DeleteAllItems();

	return;
}

int CLablePhoneAbsolutedPackDlg::GetlistData(void)
{
	int i     = 0,
        count = 0;

	CString  szImei = _T("");

	count = m_list.GetItemCount();
	m_imeiArray.RemoveAll();

	for (i=0; i<count; i++){
		szImei = m_list.GetItemText(i, 0x01);
		if (szImei.GetLength() == IMEI_LENGTH){
			m_imeiArray.Add(szImei);
		}
	}

	//ASSERT(count == m_imeiArray.GetSize());
	
	return m_imeiArray.GetSize();
}

void CLablePhoneAbsolutedPackDlg::SaveListData(void)
{
	CStackPage::UpdateData(TRUE);
	int i   = 0;
	CString szSql  = _T(""),
		    szTime = _T("");

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	szTime.Format("%04d-%02d-%02d",curTime.wYear,curTime.wMonth,curTime.wDay);
	//szTime.Format("%04d-%02d-%02d %02d%02d%02d",curTime.wYear,curTime.wMonth,curTime.wDay,curTime.wHour,curTime.wMinute,curTime.wSecond);

	theApp.m_ado.BeginTrans();
	for (i=0; i<m_imeiArray.GetSize(); i++)
	{
		//( Imei, BoxId, Symbol, Model, Color, Weight, Addons, Edition, MEstate, Amount, PEstate, Requisition, OrderForm, Dt)
		szSql.Format(SQL_PACKING_IMEI_INSERT, 
			         /*   imei 号   */m_imeiArray.GetAt(i),/* Imei */
					 /*   箱   号   */m_sBoxId,            /* BoxId */
					 /*   代   号   */m_sSymbol,           /* Symbol */
					 /*   型   号   */m_sModel,            /* Model */
					 /*   颜   色   */m_sColor,            /* Color */
					 /*   重   量   */m_sWeight,           /* Weight */
					 /*   附   件   */m_sAddons,           /* Addons */
					 /*   版   本   */m_sEdition,          /* Edition */
					 /*   机型状态  */m_sMEstate,          /* MEstate */
					 /*   数   量   */m_sAmount,           /* Amount */
					 /*   装箱状态  */m_sPEstate,          /* PEstate */
					 /*  送检通知单 */m_sRequisition,      /* Requisition */
					 /*   工单号    */m_sOfm,              /* OrderForm */
					 /*   生产时间  */szTime);             /* Dt */

		 theApp.m_ado.Execute(szSql);
	}

	theApp.m_ado.CommitTrans();

	return;
}

void CLablePhoneAbsolutedPackDlg::RefreshList(int nItem)
{
	int count    = (nItem > 0) ? nItem : 0;
	int index    = 0,
		curCount = m_list.GetItemCount();
	CString szNo = _T("");
	RVITEM       rvi;
	rvi.nMask = RVIM_TEXT;
	rvi.iSubItem = 1;
	rvi.lpszText = _T("");

	if (curCount == count){
		return;
	}

	if (curCount < count){
		for (index=curCount; index<count; index++)
		{
			// 加入列表项
			szNo.Format("%02d", index + 1);
			m_list.InsertItem(index,szNo);
			rvi.iItem = index;
			m_list.SetItem(&rvi);
		}
	}

	if (curCount > count){
		index = curCount;
		while (index > count)
		{
			m_list.DeleteItem(--index);
		}
	}

	return;
}

int CLablePhoneAbsolutedPackDlg::InsertItemToList(const char* szImei)
{
	CString szVal = szImei;

	int k   = 0,
		cou = m_list.GetItemCount();

	RVITEM rvi;
	rvi.nMask = RVIM_TEXT;
	rvi.iItem = cou;
	rvi.iSubItem = 1;
	rvi.lpszText = _T("");
	
	for (k; k<cou; k++)
	{
		szVal = m_list.GetItemText(k, 0x01);
		if (szVal.GetLength() == IMEI_LENGTH)
			continue;

		break;
	}

	if (k < cou){// 填充第 k 行缺失的imei号
		m_list.SetItemText(k, 0x01, szImei);
	}else{// 新增加一条记录
		szVal.Format("%02d", cou + 1);
		m_list.InsertItem(cou, szVal);
		m_list.SetItem(&rvi);
		m_list.SetItemText(cou, 0x01, szImei);
	}

	return GetlistData();
}


BOOL CLablePhoneAbsolutedPackDlg::isValidImeiString(CString szImei)
{
	CString szErr = _T(""), szCheck = _T("");
	// Imei len == 15
	if (szImei.GetLength() != IMEI_LENGTH){
		szErr.Format("imei len must be %d chars!", IMEI_LENGTH);
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_eImei.SetFocus();
		m_eImei.SetSel(0, szImei.GetLength());
		return FALSE;
	}

	// Prefix check
	szCheck = szImei.Mid(0, m_packingCoordinate.packingTitle.szImeiPrefix.GetLength());

	if (szCheck != m_packingCoordinate.packingTitle.szImeiPrefix){
		szErr.Format("imei prefix must be \"%s\"!", m_packingCoordinate.packingTitle.szImeiPrefix);
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_eImei.SetFocus();
		m_eImei.SetSel(0, szImei.GetLength());
		return FALSE;
	}

	// hex string check
	if (!IsHexString(szImei)){
		szErr.Format("imei must be hex string!");
		MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
		m_eImei.SetFocus();
		m_eImei.SetSel(0, szImei.GetLength());
		return FALSE;
	}

	{// in list check
		for (int i=0; i<m_list.GetItemCount(); i++)
		{
			szCheck = m_list.GetItemText(i, 1);
			if (szCheck == szImei)
			{
				szErr.Format("this imei is in list[%d]!", i+1);
				MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
				m_eImei.SetFocus();
				m_eImei.SetSel(0, szImei.GetLength());
				return FALSE;
			}
		}
	}

	{// in db check
		CString szSql = _T("");
		szSql.Format(SQL_PACKING_IMEI_QUERY, szImei);
		szCheck.Empty();
		theApp.m_ado.BeginTrans();
		theApp.m_ado.ExecuteQueryValue(szSql, szCheck);
		theApp.m_ado.CommitTrans();
		if (!szCheck.IsEmpty())
		{
			szErr.Format("[%s] is in table(Packing) serial=%s!", szImei, szCheck);
			MessageBox(szErr, _T("Err"), MB_OK|MB_ICONSTOP);
			m_eImei.SetFocus();
			m_eImei.SetSel(0, szImei.GetLength());
			return FALSE;
		}
	}


	return TRUE;
}

BOOL CLablePhoneAbsolutedPackDlg::DrawPrintLable(PackingPrintSet &packingPrintSet, PackingCoordinate packingCoordinate)
{
	CString szZpl = _T("^XA"), szTemp = _T("");
	char szZplString[1024 * 256] = _T("");
	//int  xLine    = 0;
#ifdef ME_DEBUG
	FILE *fp = fopen(_T("zpl.txt"), "w");
	if (fp){
		fprintf(fp, "^XA\r");
	}
#endif
	// 打印"箱号："
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)packingCoordinate.packingTitle.szBoxId;
	packingCoordinate.chineseZpl.lpFontName = (LPCSTR)_T("宋体");
	packingCoordinate.chineseZpl.xy.x_axis  = packingCoordinate.xyStartPoint.x_axis;
	packingCoordinate.chineseZpl.xy.y_axis  = packingCoordinate.xyStartPoint.y_axis;

	// 取"箱号："的ZPLII编码
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));


	// 打印箱号的条码
	sprintf(szZplString, "^FO%d,%d^BY%d,%2.1f,%d^B3N,,N,N,N^FD%s^FS", 
		                   packingCoordinate.xyStartPoint.x_axis + 200/*(12 * packingPrintSet.szBoxId.GetLength())*/,
						   packingCoordinate.xyStartPoint.y_axis - 24,
						   packingCoordinate.imeiBarby.width,
						   packingCoordinate.imeiBarby.width_ratio,
						   packingCoordinate.chineseZpl.intHeight + 18,
						   packingPrintSet.szBoxId);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));
	// 在箱号条码的下方打印具体箱号串
	sprintf(szZplString, "^FO%d,%d^A0,N,%d,%d^FD%s^FS", 
		                   packingCoordinate.xyStartPoint.x_axis + 200/*(12 * packingPrintSet.szBoxId.GetLength())*/ + 60,
						   (packingCoordinate.xyStartPoint.y_axis - 24) + (packingCoordinate.chineseZpl.intHeight + 18) + 12,
						   36, 
						   36,
						   packingPrintSet.szBoxId);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 取"代码："的ZPLII编码
	packingCoordinate.chineseZpl.xy.x_axis  += atoi(packingCoordinate.szTitleLevelDis) + 64;
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)packingCoordinate.packingTitle.szSymbol;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));
	// 打印代码的条码
	sprintf(szZplString, "^FO%d,%d^BY%d,%2.1f,%d^B3N,,N,N,N^FD%s^FS", 
		                   //packingCoordinate.xyStartPoint.x_axis + (12 * packingPrintSet.szSymbol.GetLength()) + 480,
						   packingCoordinate.chineseZpl.xy.x_axis + 200,
						   packingCoordinate.xyStartPoint.y_axis - 24,
						   packingCoordinate.imeiBarby.width,
						   packingCoordinate.imeiBarby.width_ratio,
						   packingCoordinate.chineseZpl.intHeight + 18,
						   packingPrintSet.szSymbol);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 在代码条码的下方打印具体代码串
	sprintf(szZplString, "^FO%d,%d^A0,N,%d,%d^FD%s^FS", 
		                   //packingCoordinate.xyStartPoint.x_axis + (12 * packingPrintSet.szBoxId.GetLength()) + 480 + 24,
						   packingCoordinate.chineseZpl.xy.x_axis + 200 + 48,
						   (packingCoordinate.xyStartPoint.y_axis - 24) + (packingCoordinate.chineseZpl.intHeight + 18) + 12,
						   36, 
						   36,
						   packingPrintSet.szSymbol);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 缩小字号
	packingCoordinate.chineseZpl.intHeight /= 2;
	packingCoordinate.chineseZpl.intWidth  /= 2;


	//打印一条分隔线 ------------------------------------------------------------------------------
	// 调整一下坐标到分隔线的起点
	packingCoordinate.xyStartPoint.x_axis -= 30;
	packingCoordinate.xyStartPoint.y_axis = (packingCoordinate.xyStartPoint.y_axis - 24) + 
		                                    (packingCoordinate.chineseZpl.intHeight + 18) + 12 + 24 + 12 + 36;
	// 记录一下横线起始点的x值
	//xLine = packingCoordinate.xyStartPoint.x_axis;

	// 绘线
	sprintf(szZplString, "^FO%d,%d^GB%d,0,5,B,0^FS",
		                  packingCoordinate.xyStartPoint.x_axis,
						  packingCoordinate.xyStartPoint.y_axis, 
						  110*12);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	/****************************************************************************************/
	// 打印 "型号： GSM169(C)V90"
	packingCoordinate.chineseZpl.xy.x_axis  = packingCoordinate.xyStartPoint.x_axis;
	packingCoordinate.chineseZpl.xy.y_axis  = packingCoordinate.xyStartPoint.y_axis + 24;
	packingCoordinate.chineseZpl.intHeight  = 40;
	packingCoordinate.chineseZpl.lpFontName = (LPCSTR)_T("宋体");

	szTemp.Format("%s%s", packingCoordinate.packingTitle.szModel, packingPrintSet.szModel);
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 打印 "颜色： 银白色"
	packingCoordinate.chineseZpl.xy.x_axis  = packingCoordinate.xyStartPoint.x_axis + atoi(packingCoordinate.szTitleLevelDis);
	szTemp.Format("%s%s", packingCoordinate.packingTitle.szColor, packingPrintSet.szColor);
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));


	// 打印 "重量： 12600 克"
	packingCoordinate.chineseZpl.xy.x_axis  -= atoi(packingCoordinate.szTitleLevelDis);
	packingCoordinate.chineseZpl.xy.y_axis  += (packingCoordinate.chineseZpl.intHeight + 18);
	szTemp.Format("%s%s (g)", packingCoordinate.packingTitle.szWeight, packingPrintSet.szWeight);
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 打印 "配件： 一电一冲"
	packingCoordinate.chineseZpl.xy.x_axis  += atoi(packingCoordinate.szTitleLevelDis);
	szTemp.Format("%s%s", packingCoordinate.packingTitle.szAddons, packingPrintSet.szAddons);
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 打印 "区域： 四川移动"
	packingCoordinate.chineseZpl.xy.x_axis  -= atoi(packingCoordinate.szTitleLevelDis);
	packingCoordinate.chineseZpl.xy.y_axis  += (packingCoordinate.chineseZpl.intHeight + 18);
	szTemp.Format("%s%s", packingCoordinate.packingTitle.szEdition, packingPrintSet.szEdition);
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 打印 "机型状态： 整机"
	packingCoordinate.chineseZpl.xy.x_axis  += atoi(packingCoordinate.szTitleLevelDis);
	szTemp.Format("%s%s", packingCoordinate.packingTitle.szMEstate, packingPrintSet.szMEstate);
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 打印 "数量： 10"
	packingCoordinate.chineseZpl.xy.x_axis  -= atoi(packingCoordinate.szTitleLevelDis);
	packingCoordinate.chineseZpl.xy.y_axis  += (packingCoordinate.chineseZpl.intHeight + 18);
	szTemp.Format("%s%02d", packingCoordinate.packingTitle.szAmount, atoi(packingPrintSet.szAmount));
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	// 打印 "装箱状态： 满"
	packingCoordinate.chineseZpl.xy.x_axis  += atoi(packingCoordinate.szTitleLevelDis);
	szTemp.Format("%s%s", packingCoordinate.packingTitle.szPEstate, packingPrintSet.szPEstate);
	packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
	GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));


	/**************************       打 印 imei 的 条 码       ****************************/
	{
#define IMEI_STR_HEIGH   (24)
#define IMEI_STR_BAR_GAP (10)
#define IMEI_NEXT_BAR    (18)
		int index = 0, 
			xAxis = packingCoordinate.chineseZpl.xy.x_axis - atoi(packingCoordinate.szTitleLevelDis) + 64, 
			yAxis = packingCoordinate.chineseZpl.xy.y_axis + packingCoordinate.chineseZpl.intHeight  + 48;
		for (index; index<packingPrintSet.szImeiArray.GetSize(); index++)
		{
			if (0 == index % 2){// 第 1 列
				if (index > 0){
					xAxis -= atoi(packingCoordinate.szImeiLevelDis);
				}				
				if (index > 0){
					yAxis += (packingCoordinate.imeiBarby.heighet + IMEI_STR_HEIGH + IMEI_STR_BAR_GAP + IMEI_NEXT_BAR);
				}
			}else{// 第 2 列
				xAxis += atoi(packingCoordinate.szImeiLevelDis);
			}
			// 打印 imei 号的条形码
			sprintf(szZplString, "^FO%d,%d^BY%d,%2.1f,%d^B3N,,N,N,N^FD%s^FS",
				                   xAxis,
								   yAxis,
								   packingCoordinate.imeiBarby.width,
								   packingCoordinate.imeiBarby.width_ratio,
								   packingCoordinate.imeiBarby.heighet,
								   packingPrintSet.szImeiArray.GetAt(index));
			szZpl += szZplString;	
#ifdef ME_DEBUG
			if (fp){
				fprintf(fp, "%s\r", szZplString);
			}
#endif
			memset(szZplString, '\0', sizeof(szZplString));


			// 在条形码的下面打印出具体 imei 字符串
			sprintf(szZplString, "^FO%d,%d^A0,N,%d,%d^FD%s^FS", 
				                     xAxis + 120,
									 yAxis + IMEI_STR_BAR_GAP + packingCoordinate.imeiBarby.heighet,
									 IMEI_STR_HEIGH,
									 IMEI_STR_HEIGH,
									 packingPrintSet.szImeiArray.GetAt(index));
			szZpl += szZplString;	
#ifdef ME_DEBUG
			if (fp){
				fprintf(fp, "%s\r", szZplString);
			}
#endif
			memset(szZplString, '\0', sizeof(szZplString));
		}
#undef IMEI_STR_HEIGH
#undef IMEI_STR_BAR_GAP
#undef IMEI_NEXT_BAR
	}

	/*****************************    打印下方分隔线    ************************************/
	packingCoordinate.xyStartPoint.y_axis = (150 - 10)*MMDOTSA; // 距离底线 1.5 mm处
	// 绘线
	sprintf(szZplString, "^FO%d,%d^GB%d,0,5,B,0^FS",
		                  packingCoordinate.xyStartPoint.x_axis,
						  packingCoordinate.xyStartPoint.y_axis, 
						  110*12);
	szZpl += szZplString;	
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	/***********************    送检通知单：   Q/A：       日期：    ***********************/
	{
		SYSTEMTIME curTime;
		GetLocalTime( &curTime );

		packingCoordinate.chineseZpl.xy.x_axis  = packingCoordinate.xyStartPoint.x_axis + 24;
		packingCoordinate.chineseZpl.xy.y_axis  = packingCoordinate.xyStartPoint.y_axis + 24;
		szTemp.Format("%s%s   Q/A：       %s %04d/%02d/%02d",
			           packingCoordinate.packingTitle.szRequisition,
					   packingPrintSet.szRequisition,
					   packingCoordinate.packingTitle.szDate,
					   curTime.wYear,
					   curTime.wMonth,
					   curTime.wDay);
		packingCoordinate.chineseZpl.lpChinese  = (LPCSTR)szTemp;
		GetChineseZplCode(&packingCoordinate.chineseZpl, szZplString);
		szZpl += szZplString;	
#ifdef ME_DEBUG
		if (fp){
			fprintf(fp, "%s\r", szZplString);
		}
#endif
		memset(szZplString, '\0', sizeof(szZplString));
	}
	/***************************************************************************************/

	// 打印份数以及结束
	sprintf(szZplString, "^PQ%d,0,1,Y^XZ", atoi(theApp.m_szShare));
	szZpl += szZplString;
#ifdef ME_DEBUG
	if (fp){
		fprintf(fp, "%s\r", szZplString);
	}
#endif
	memset(szZplString, '\0', sizeof(szZplString));

	return (SUCCEED_LPT == ExecutePrintCmdInLpt(szZpl, theApp.m_szLpt));
}

void CLablePhoneAbsolutedPackDlg::OnBnClickedButPapClearList()
{
	// TODO: Add your control notification handler code here
	m_list.DeleteAllItems();
}


void CLablePhoneAbsolutedPackDlg::OnBnClickedButPapQuery()
{
	// TODO: Add your control notification handler code here
	CString szSql    = _T("");
	CString szSqlQcs = _T("");
	CDStrs matrix;

	CQcsDlg   dQcs;
	dQcs.m_sBoxId   = m_sBoxId;
	dQcs.m_sEdition = m_sEdition;
	dQcs.m_sModel   = m_sModel;
	dQcs.m_sOfm     = m_sOfm;
	dQcs.m_sReq     = m_sRequisition;

	if (IDOK != dQcs.DoModal()){
		return;
	}

	CFileDialog fDlg(FALSE,
		             _T("xls"),
		             _T("Query.xls"),
		             OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		             _T("Excel File (*.xls)|*.xls|All File (*.*)|*.*||"),
		             this);
	
	// 打开的路径为本程序所在的路径
	char name[256];
	GetModuleFileName(NULL, name, 256);
	
	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;
	
	// 改变标题：Save - means 导出脚本文件
	fDlg.m_ofn.lpstrTitle = _T("Output Recordset to Excel:");
	
	if (IDOK != fDlg.DoModal()){
		return;
	}else{
		// 依据得到的路径和文件名
		path = fDlg.GetPathName();
	}
	
	CSpreadSheet xls(path, _T("Recordset"), 0);
	CStringArray szHeaders;
	//Imei, BoxId, Symbol, Model, Color, Weight, Addons, Edition, MEstate, Amount, PEstate, Requisition, OrderForm, Dt
	szHeaders.RemoveAll();
	szHeaders.Add("Serial");
	szHeaders.Add("BoxId");
	szHeaders.Add("Model");
	szHeaders.Add("Color");
	szHeaders.Add("Weight");
	szHeaders.Add("Addons");
	szHeaders.Add("Edition");
	szHeaders.Add("MEstate");
	szHeaders.Add("Amount");
	szHeaders.Add("PEstate");
	szHeaders.Add("Requisition");
	szHeaders.Add("OrderForm");
	szHeaders.Add("Dt");
	xls.BeginTransaction();
	xls.AddRow(szHeaders);
	

	m_bQuery.EnableWindow(FALSE);
	
	// query condition setting
	if (dQcs.m_sBoxId.GetLength() > 0){
		szSql.Format("BoxId=\"%s\" AND ", dQcs.m_sBoxId);
		szSqlQcs += szSql;
	}
	if (dQcs.m_sModel.GetLength() > 0){
		szSql.Format("Model=\"%s\" AND ", dQcs.m_sModel);
		szSqlQcs += szSql;
	}
	if (dQcs.m_sEdition.GetLength() > 0){
		szSql.Format("Edition=\"%s\" AND ", dQcs.m_sEdition);
		szSqlQcs += szSql;
	}
	if (dQcs.m_sReq.GetLength() > 0){
		szSql.Format("Requisition=\"%s\" AND ", dQcs.m_sReq);
		szSqlQcs += szSql;
	}
	if (dQcs.m_sOfm.GetLength() > 0){
		szSql.Format("OrderForm=\"%s\" AND ", dQcs.m_sOfm);
		szSqlQcs += szSql;
	}
	if ((dQcs.m_sDate.GetLength() > 0) && (dQcs.m_sDate != _T("YYYY-MM-DD")))
	{
		szSql.Format("Dt=\"%s\" AND ", dQcs.m_sDate);
		szSqlQcs += szSql;
	}

	if (_T(" AND ") == szSqlQcs.Right(strlen(_T(" AND "))))
	{
		szSqlQcs = szSqlQcs.Left(szSqlQcs.GetLength() - strlen(_T(" AND ")));
		szSqlQcs = _T("WHERE ") + szSqlQcs;
	}

	szSql = SQL_PACKING_SELETE_BOXID + szSqlQcs;
	theApp.m_ado.BeginTrans();
	theApp.m_ado.ExecuteQuery(szSql, matrix);
	theApp.m_ado.CommitTrans();

#ifdef ME_DEBUG
	{
		unsigned int i = 0, j = 0;
		FILE *fp = fopen(path + _T(".txt"), "w");
		if (fp){
			fprintf(fp, "sql=%s\r", szSql);
			fprintf(fp, "Size = %d\r", matrix.size());
			for (i=0; i<matrix.size(); i++)
			{
				szSql.Empty();
				for (j=0; j<matrix.at(i).size(); j++)
				{
					szSql += matrix.at(i).at(j);
					szSql += _T(" ");
				}
				fprintf(fp, "%s\r\n", szSql);
			}
			fclose(fp);

		}
	}
#endif


	// 将查询到的结果保存到 Excel
	{
		unsigned long nRow     = 0;
		unsigned int  nColumn  = 0;

		if ((matrix.size() == 0) || (matrix.at(0).size() == 0)){
			szSql.Format("There is none Recordset!");
			MessageBox(szSql, _T("Tip:"), MB_OK);
			m_bQuery.EnableWindow(TRUE);
			return;
		}

		for (nRow=0; nRow<matrix.size(); nRow++)
		{
			szHeaders.RemoveAll();
			for (nColumn=0; nColumn<matrix.at(nRow).size(); nColumn++)
			{
				szHeaders.Add(matrix.at(nRow).at(nColumn));
			}
			xls.AddRow(szHeaders, nRow+1,  true);
		}
		xls.Commit();

		szSql.Format("There are %d Recordsets, saved Ok!", matrix.size());
		MessageBox(szSql, _T("Tip:"), MB_OK);

		// 将原Excel文件转换为用分号分隔的文本cvs，并另存为同名文本文件
		//xls.Convert(";");
	}

	m_bQuery.EnableWindow(TRUE);
}


void CLablePhoneAbsolutedPackDlg::OnBnClickedButPapRePrint()
{
	// TODO: Add your control notification handler code here
	CString szSql = _T("");
	CDStrs matrixImei;
	PackingPrintSet   packingPrintSet;
	unsigned long nRow  = 0;

	m_bReprint.EnableWindow(FALSE);

	m_eBoxId.GetWindowText(m_sBoxId);
	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Imei"), m_sBoxId);
	theApp.m_ado.BeginTrans();
	theApp.m_ado.ExecuteQuery(szSql, matrixImei);
	theApp.m_ado.CommitTrans();

	// 是否查询到结果
	if (0 == matrixImei.size())
	{
		szSql.Format("There is none record match BoxId[%s]!", m_sBoxId);
		MessageBox(szSql, _T("Tip:"), MB_OK|MB_ICONSTOP);
		m_bReprint.EnableWindow(TRUE);
		return;
	}


	packingPrintSet.szBoxId = m_sBoxId;

	// 将imei保存数组
	for (nRow=0; nRow<matrixImei.size(); nRow++)
	{
		packingPrintSet.szImeiArray.Add(matrixImei.at(nRow).at(0));
	}

	//( Imei, BoxId, Symbol, Model, Color, Weight, Addons, Edition, MEstate, Amount, PEstate, Requisition, OrderForm, Dt)
	theApp.m_ado.BeginTrans();

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Symbol"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szSymbol);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Model"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szModel);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Color"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szColor);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Weight"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szWeight);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Addons"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szAddons);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Edition"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szEdition);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("MEstate"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szMEstate);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Amount"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szAmount);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("PEstate"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szPEstate);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("Requisition"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szRequisition);

	szSql.Format(SQL_PACKING_SELETE_FIELD, _T("OrderForm"), m_sBoxId);
	theApp.m_ado.ExecuteQueryValue(szSql, packingPrintSet.szOrderForm);

	theApp.m_ado.CommitTrans();

#ifdef ME_DEBUG
	{
		unsigned int i = 0;
		FILE *fp = fopen(_T("Imei.ini"), "a+t");
		if (fp){
			fprintf(fp, "[%s]\r", m_sBoxId);
			while (i < packingPrintSet.szImeiArray.GetSize()){
				fprintf(fp, "Imei%02d=%s\r", i+1, packingPrintSet.szImeiArray.GetAt(i++));
			}

			fprintf(fp, "Symbol=%s\r",      packingPrintSet.szSymbol);
			fprintf(fp, "Model=%s\r",       packingPrintSet.szModel);
			fprintf(fp, "Color=%s\r",       packingPrintSet.szColor);
			fprintf(fp, "Weight=%s\r",      packingPrintSet.szWeight);
			fprintf(fp, "Addons=%s\r",      packingPrintSet.szAddons);
			fprintf(fp, "Edition=%s\r",     packingPrintSet.szEdition);
			fprintf(fp, "MEstate=%s\r",     packingPrintSet.szMEstate);
			fprintf(fp, "Amount=%s\r",      packingPrintSet.szAmount);
			fprintf(fp, "PEstate=%s\r",     packingPrintSet.szPEstate);
			fprintf(fp, "Requisition=%s\r", packingPrintSet.szRequisition);
			fprintf(fp, "OrderForm=%s\r",   packingPrintSet.szOrderForm);
			fclose(fp);
		}
	}
#endif

	DrawPrintLable(packingPrintSet, this->m_packingCoordinate);

	m_bReprint.EnableWindow(TRUE);
}
