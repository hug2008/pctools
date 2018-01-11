

#pragma once
#include ".\bizphonesettingdlg.h"
#include "afxwin.h"

// CLablePg530ImeiDlg dialog

// 打印格式、初试化
// the default ZPLII's format of printing PG530 Contents & Imei Lable
#define   ZPL_PG_CONTENT_FORMAT   _T("PgContentZplii")
//^XA^LL0531
#define   ZPL_PG_CONTENT_STRAT    _T("^XA^FO%d,%d")
//^FT34,52^A0N,42,40^FH\^FDModel:PG120^FS
#define   ZPL_PG_CONTENT_CAPTION  _T("^FT%d,%d^A%dN,%d,%d^FH\^FD%s^FS")

#define   ZPL_PG_CONTENT_AXIS_X   _T("X_axis")
#define   ZPL_PG_CONTENT_AXIS_Y   _T("Y_axis")
#define   ZPL_PG_CONTENT_FONT_A   _T("A_font")
#define   ZPL_PG_CONTENT_FONT_W   _T("W_font")
#define   ZPL_PG_CONTENT_FONT_H   _T("H_font")
#define   ZPL_PG_CONTENT_BARCODE  _T("BcFmt")/*^B3N,N,,Y,N*/
#define   ZPL_PG_CONTENT_BCBY_RA  _T("BcBy_Ra")/* ^BY2,2, */
#define   ZPL_PG_CONTENT_BCBY_H   _T("BcBy_H")/* ^BY2,2,H */
#define   ZPL_PG_CONTENT_DIS_VTC  _T("BcSpaceVertical")/* 垂直间距 */

class CLablePg530ImeiDlg : public CStackPage
{
public:
	CLablePg530ImeiDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLablePg530ImeiDlg();

// Dialog Data
	enum { IDD = IDD_PG530_IMEI };

public:
	virtual void OnSaveData(void);
	virtual void OnLockPage(BOOL bLock);

protected:
	CComboBoxXP m_cModel;
	CString     m_szModel;
	CString     m_szModelSp;
	CStaticEx   m_tModel;
	CStaticEx   m_tContents;

	CEditEx     m_eMain;
	CString     m_szMain;
	CString     m_szMainSp;

	CEditEx     m_eHand;
	CString     m_szHand;
	CString     m_szHandSp;

	CEditEx     m_eAnt;
	CString     m_szAnt;
	CString     m_szAntSp;

	CEditEx     m_eAda;
	CString     m_szAda;
	CString     m_szAdaSp;

	CEditEx     m_eBat;
	CString     m_szBat;
	CString     m_szBatSp;

	CEditEx     m_eUser;
	CString     m_szUser;
	CString     m_szUserSp;

	CComboBoxXP m_cColor;
	CString     m_szColor;
	CString     m_szColorSp;
	CStaticEx   m_tColor;

	CEditEx     m_eImei;
	CString     m_szImei;
	CStaticEx   m_tImei;

	int         m_nSnoLen;
	CEditEx     m_eSno;
	CString     m_szSno;
	CStaticEx   m_tSno;

	CButton    m_cMmi;

	CComboBoxXP m_cYear;
	CComboBoxXP m_cMonth;

	CStringArray m_szArray;

	CBizPhoneSettingDlg *m_pParentWnd;

	BOOL CheckMeSeting(void);

	BOOL PrintMeLable(void);

	void UpDateMeAfterPrint(void);

	void ExchangeMmi(int nMmiIndex);
	void MapMmiViewId(int nMmiIndex);

	// To Verify Imei
	CString  m_szVerifyImeiPrefix;
	CString  m_szVerifyImeiPostfix;

	// image position
	CString  m_szImageX,
		     m_szImageY;

protected://for barcode
	CString     m_bXaxis; // x - 轴的初始值
	CString     m_bYaxis; // y - 轴的初始值
	CString     m_bAfont; // 字体
	CString     m_bWfont; // 字号 - 宽度
	CString     m_bHfont; // 字号 - 高度
	CString     m_bBcFmt; // 条码 - 编码格式
	CString     m_bBcByRa;// 条码 - 疏密比
	CString     m_bBcByH; // 条码 - 高度
	CString     m_bBcSpaceVertical;// 字符的垂直间距

	CString     m_szZpl;  // 条码打印指令

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
#ifndef _VK_RETURN_EXIT_
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#endif//_VK_RETURN_EXIT_
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckPg530Mmi();
	DECLARE_MESSAGE_MAP()
};
/* 根据机型的名字获取机型的代号: 见PG530 Label REV0.pdf*/
int GetModelId(const char *szModelName);