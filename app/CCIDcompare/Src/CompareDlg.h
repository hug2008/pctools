#pragma once

#include "SkinWin\SkinWin.h"
#include "SkinWin\SkinButton.h"
#include "EXCEl\\CSpreadSheet.h"
#include "ADO\\ADODB.h"
#include "Barcode\\barcode.h"
#include "Src\\myglobals.h"
#include "CtrlEx\\editex.hxx"
#include "afxwin.h"
// CCompareDlg dialog

#define    NORMALTYPE_PRINT       (0x01)
#define    NON_NORMALTYPE_PRINT   (0x00)
#define    NORMALTYPE_CAR         (0x00)
#define    NON_NORMALTYPE_CAR     (0x01)

class CCompareDlg : public CDialog
{
	DECLARE_DYNAMIC(CCompareDlg)

public:
	CCompareDlg(CWnd* pParent = NULL);   // standard constructor
	void PrintTest();
	virtual ~CCompareDlg();

// Dialog Data
	enum { IDD = IDD_COMPARE_DIALOG };

protected:

	HICON     m_hIcon;

	CString   M_STRCARDTYPE;

	CString   M_STRCHECTYPE;

#ifdef  UNICOM_CCID_19
	CString szCcid19;
	char    cFreeCard;
	char    cNumCard;
#endif//UNICOM_CCID_19

	BOOL                    m_bLock;

	//������������壬�����
	char *buf;
	ZPLFONT font;
	BARBY   by;
	int X_coordinate;
	int Y_coordinate;
	BOOL OverBarcode;
	int nShare;

	//�༭������չ��Ķ���
	CEditEx   m_edit;
	CEditEx   m_ctrlbuyer;
	CString   m_strbuyer;

	//�����ӡ���ͺͿ�������
	int       m_typecar;
	int       m_typeprint;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//���ص�PreTranslateMessage���������λس���ESC����Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:
	CComboBox m_CtrlAotu;
	CComboBox m_CtrlSimtype;
	CString   m_input;
	CAdoDB    m_adodb;    //���ݿ��������
	char      *OpenLink;  //���ݿ���������������Դ��

	CButton m_btInput;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();	
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnEnChangeEdit3();
	//�����ǩ����У������
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonInput();
	DECLARE_MESSAGE_MAP()	
};
