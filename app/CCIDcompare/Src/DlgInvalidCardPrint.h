#pragma once
#include "afxwin.h"
#include "ADO\\ADODB.h"
#include "Barcode\\barcode.h"
#include "PhoneNumber.h"
// CDlgInvalidCardPrint 对话框

class CDlgInvalidCardPrint : public CDialog
{
	DECLARE_DYNAMIC(CDlgInvalidCardPrint)

public:
	CDlgInvalidCardPrint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInvalidCardPrint();

// 对话框数据
	enum { IDD = IDD_INVALIDCARD_PRINT };

protected:
	HICON     m_hIcon;

protected:
	CEdit   m_edTel;
	CEdit   m_edImei;
	CEdit   m_edCcid;
	CButton m_btPrint;

public:
	CPhoneNumber m_szTel;
	CPhoneNumber m_szImei;
	CPhoneNumber m_szCcid;

	// 打印坐标部分
public:
	ZPLFONT m_font;
	BARBY   m_by;
	int     m_X_coordinate;
	int     m_Y_coordinate;
	BOOL    m_OverBarcode;
	int     m_nShare;

	CString m_szBuyer;
	CString m_szArea;

	CAdoDB  *m_pAdodb;

private:
	BOOL  IsValidSetting(void);

	void  EnableEditId(BOOL bEnable);

	void  StoreItemToDb(void);

	void  ResetEditId(void);

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtInvalidPrint();
	afx_msg void OnEnChangeEditInvalidTel();
	afx_msg void OnEnChangeEditInvalidImei();
	afx_msg void OnEnChangeEditInvalidCcid();
	DECLARE_MESSAGE_MAP()
};
