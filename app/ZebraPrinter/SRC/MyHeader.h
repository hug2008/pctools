#if !defined(AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)
#define AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyHeader.h : header file
//windows msg
#define     WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY    (WM_USER + 100)

#define     WM_TAG_CHAR                     (0x40)

#define     IMEI_LENGTH                     (0x0f)

#define     INT_NUM_OF_PHONE_TYPE			(0x02)
// PG530 标签数目
#define     INI_NUM_OF_PG530_LABLE          (0x02)


// 配置文件设置
#define ININAME                 _T("ZaberCnf.ini")

#define DBNAME                  _T("ZebraPrinter.mdb")

#define SEC_MAINSETTING         _T("PrintSetting")

#define ZPL_CONTENTIMEI         _T("ZPL_ContentImei")

#define SEC_CONTENTIMEI         _T("ContentImei")

#define SEC_CONTENTIMEI_DBLOG   _T("ContentImeiDbLog")

#define SEC_CARTONBOX           _T("CartonBoxId")

//#define SEC_ZPL_LOG             _T("ZplCmd.log")


// PG530 Contents & Imei Items
#define IDENT_CONTENTS_MODELNAME       _T("ModelName")
#define IDENT_CONTENTS_MODELSEL        _T("ModelSel")
#define IDENT_CONTENTS_MAINSET         _T("MainSet")
#define IDENT_CONTENTS_MAINSET_SP      _T("Telefono")
#define IDENT_CONTENTS_HANDSET         _T("HandSet")
#define IDENT_CONTENTS_HANDSET_SP      _T("Auricular")
#define IDENT_CONTENTS_ANTENNA         _T("Antenna")
#define IDENT_CONTENTS_ANTENNA_SP      _T("Antena")
#define IDENT_CONTENTS_ADAPTOR         _T("Adaptor")
#define IDENT_CONTENTS_ADAPTOR_SP      _T("Adaptador")
#define IDENT_CONTENTS_BATTERY         _T("Battery")
#define IDENT_CONTENTS_BATTERY_SP      _T("Bateria")
#define IDENT_CONTENTS_USER            _T("User")
#define IDENT_CONTENTS_USER_SP         _T("UserSp")
#define IDENT_CONTENTS_COLORSEL        _T("ColorSel")
#define IDENT_CONTENTS_COLORNAME       _T("ColorName")
#define IDENT_CONTENTS_COLORNAME_SP    _T("ColorNameSp")
#define IDENT_CONTENTS_YEAR            _T("Year")
#define IDENT_CONTENTS_MONTH           _T("Month")
#define IDENT_CONTENTS_SNO             _T("Sno")
#define IDENT_CONTENTS_SNOLEN          _T("SnoLen")
#define IDENT_CONTENTS_IMEIPREFIXION   _T("ImeiPrefixion")
#define IDENT_CONTENTS_IMEIPOSTFIXION  _T("ImeiPostfixion")
#define IDENT_CONTENTS_IMAGE_X          _T("ImageX")
#define IDENT_CONTENTS_IMAGE_Y          _T("ImageY")

// PG530 Carton box Items
#define IDENT_CARTON_MODELNAME         _T("ModelName")
#define IDENT_CARTON_MODELSEL          _T("ModelSel")
#define INENT_CARTON_CTBOXIDLEN        _T("CtboxIdLen")
#define INENT_CARTON_CTBOXIDLAST       _T("CtboxIdLast")
#define INENT_CARTON_QUANTITY          _T("Quantity")
#define INENT_CARTON_QUANTITYSEL       _T("QuantitySel")
#define IDENT_CARTON_SERIALNO          _T("SerialNo")
#define IDENT_CARTON_SERIALNOLEN       _T("SerialNoLen")
#define IDENT_CARTON_AG169HLEN         _T("AG169HSNoLen")
#define IDENT_CARTON_IMEIBARCODETIP    _T("ImeiBarcodeTip")
#define IDENT_CARTON_SNOBARCODETIP     _T("SnoBarcodeTip")
#define IDENT_CARTON_ORDER             _T("Order")
#define IDENT_CARTON_ZONE              _T("Zone")
#define IDENT_CARTON_AG169H            _T("Ag169h")
#define IDENT_CARTON_IMEISORT          _T("SortImei")
#define IDENT_CARTON_IMEISNO           _T("SortSno")


#endif // !defined(AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)