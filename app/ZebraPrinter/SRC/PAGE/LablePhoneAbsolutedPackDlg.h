#pragma once

#include ".\bizphonesettingdlg.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ReportCtrl.h"
#include "barcode.h"
// CLablePhoneAbsolutedPackDlg dialog

/* Ini section, ident  def */
#define   SEC_PHONE_ABSOLUTE_PACK   _T("PhoneAbsolutePack")
#define   IDENT_BOXID_TITLE_TEXT    _T("BoxIdTitle")
#define   IDENT_BOXID_STR_TEXT      _T("BoxIdStr")
#define   IDENT_BOXID_STR_LEN       _T("BoxIdLen")
#define   IDENT_SYMBOL_TITLE_TEXT   _T("SymbolTitle")
#define   IDENT_SYMBOL_STR_TEXT     _T("SymbolStr")
#define   IDENT_MODEL_TITLE_TEXT    _T("ModelTitle")
#define   IDENT_MODEL_STR_TEXT      _T("ModelStr")
#define   IDENT_COLOR_TITLE_TEXT    _T("ColorTitle")
#define   IDENT_COLOR_STR_TEXT      _T("ColorStr")
#define   IDENT_WEIGHT_TITLE_TEXT   _T("WeightTitle")
#define   IDENT_WEIGHT_STR_TEXT     _T("WeightStr")
#define   IDENT_ADDONS_TITLE_TEXT   _T("AddonsTitle")
#define   IDENT_ADDONS_STR_TEXT     _T("AddonsStr")
#define   IDENT_EDITION_TITLE_TEXT  _T("EditionTitle")
#define   IDENT_EDITION_STR_TEXT    _T("EditionStr")
#define   IDENT_MESTATE_TITLE_TEXT  _T("MEstateTitle")
#define   IDENT_MESTATE_STR_TEXT    _T("MEstateStr")
#define   IDENT_AMOUNT_TITLE_TEXT   _T("AmountTitle")
#define   IDENT_AMOUNT_STR_TEXT     _T("AmountStr")
#define   IDENT_AMOUNT_STR_DEF      _T("AmountDef")
#define   IDENT_PESTATE_TITLE_TEXT  _T("PEstateTitle")
#define   IDENT_PESTATE_STR_TEXT    _T("PEstateStr")
#define   IDENT_REQ_TITLE_TEXT      _T("RequisitionTitle")
#define   IDENT_REQ_STR_TEXT        _T("RequisitionStr")
#define   IDENT_REQ_STR_LEN         _T("RequisitionStrLen")
#define   IDENT_OFM_STR_TEXT        _T("OrderForm")
#define   IDENT_IMEI_PREFIX_STR     _T("ImeiPrifox")

#define   IDENT_CD_XY_X             _T("xStartPoint")
#define   IDENT_CD_XY_Y             _T("yStartPoint")
#define   IDENT_IMEI_BAR_HEIGHT     _T("imeiHeight")
#define   IDENT_IMEI_BAR_WIDTH      _T("imeiWidth")
#define   IDENT_IMEI_BAR_RATIO      _T("imeiRatio")
#define   IDENT_PACK_FONT_NAME      _T("fontName")
#define   IDENT_PACK_FONT_HEIGHT    _T("fontHeight")
#define   IDENT_PACK_FONT_WIDTH     _T("fontWidth")
#define   IDENT_PACK_FONT_ORIENT    _T("fontOrient")
#define   IDENT_PACK_FONT_BOID      _T("fontBold")
#define   IDENT_PACK_FONT_ITALIC    _T("fontItalic")
#define   IDENT_PACK_FONT_XMF       _T("fontXmf")
#define   IDENT_PACK_FONT_YMF       _T("fontYmf")
#define   IDENT_PACK_IMEI_LEVEL_DIS _T("imeiLevelDis")
#define   IDENT_PACK_TITLE_LEVEL_DIS _T("titleLevelDis")
#define   IDENT_PACK_TITLE_DATE     _T("date")

typedef struct PackingTitleTag{
	CString  szBoxId;
	CString  szBoxIdLen;
	CString  szSymbol;
	CString  szModel;
	CString  szColor;
	CString  szWeight;
	CString  szAddons;
	CString  szEdition;
	CString  szMEstate;
	CString  szAmount;
	CString  szPEstate;
	CString  szRequisition;
	CString  szRequisitionLen;
	CString  szOfm;
	CString  szDate;
	CString  szImeiPrefix;
}PackingTitle;

typedef struct PackingPrintSetTag{
	CString  szBoxId;
	CString  szSymbol;
	CString  szModel;
	CString  szColor;
	CString  szWeight;
	CString  szAddons;
	CString  szEdition;
	CString  szMEstate;
	CString  szAmount;
	CString  szPEstate;
	CString  szRequisition;
	CString  szOrderForm;
	CStringArray szImeiArray;
}PackingPrintSet, *lpPackingPrintSet;

typedef struct PackingCoordinateTag{
	XY          xyStartPoint;   // 起始点坐标
	CHINESEZPL  chineseZpl;     // 汉字编码
	BARBY       imeiBarby;      // imei的设置
	PackingTitle packingTitle;  // 标题字符串
	CString     szImeiLevelDis; // Imei 横向间距
	CString     szTitleLevelDis;// 标题横向间距
}PackingCoordinate;


class CLablePhoneAbsolutedPackDlg : public CStackPage
{
public:
	CLablePhoneAbsolutedPackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLablePhoneAbsolutedPackDlg();

// Dialog Data
	enum { IDD = IDD_PHONE_ABSOLUTED_PACK };

public:
	virtual void OnSaveData(void);
	virtual void OnLockPage(BOOL bLock);

private:
	CBizPhoneSettingDlg  *m_pParentWnd;
	CReportCtrl	         m_list;
	CEditEx              m_eBoxId;
	CString              m_sBoxId;
	CEditEx              m_eSymbol;
	CString              m_sSymbol;
	CEditEx              m_eModel;
	CString              m_sModel;
	CEditEx              m_eColor;
	CString              m_sColor;
	CEditEx              m_eWeight;
	CString              m_sWeight;
	CEditEx              m_eAddons;
	CString              m_sAddons;
	CEditEx              m_eEdition;
	CString              m_sEdition;
	CEditEx              m_eMEstate;
	CString              m_sMEstate;
	CEditEx              m_eAmount;
	CString              m_sAmount;
	CEditEx              m_ePEstate;
	CString              m_sPEstate;
	CEditEx              m_eRequisition;
	CString              m_sRequisition;
	CEditEx              m_eOfm;
	CString              m_sOfm;
	CEditEx              m_eImei;
	CString              m_sImei;
	CButtonXP            m_bClear;
	CButtonXP            m_bQuery;
	CButtonXP            m_bReprint;

	PackingCoordinate    m_packingCoordinate;
	CStringArray         m_imeiArray;


protected:
	BOOL CheckMeSeting(void);
	BOOL PrintMeLable(void);
	void UpDateMeAfterPrint(void);
	void ProcessWmsgAmountEdit(MSG* pImeiMsg);
	void ProcessWmsgImeiEdit(MSG* pImeiMsg);
	void InitControlId(void);
	int GetlistData(void);
	void SaveListData(void);
	void RefreshList(int nItem);
	int  InsertItemToList(const char* szImei);
	BOOL isValidImeiString(CString szImei);
	BOOL DrawPrintLable(PackingPrintSet &, PackingCoordinate);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
#ifndef _VK_RETURN_EXIT_
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#endif//_VK_RETURN_EXIT_
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButPapClearList();
	afx_msg void OnBnClickedButPapQuery();
	afx_msg void OnBnClickedButPapRePrint();
	DECLARE_MESSAGE_MAP()
};
