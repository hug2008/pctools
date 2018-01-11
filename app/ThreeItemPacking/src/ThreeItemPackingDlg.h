// ThreeItemPackingDlg.h : ͷ�ļ�
//

#pragma once
#include "mydefine.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "MyAdo\\Ado.h"
#include "SrcEx\\XPButton.h"
#include "SrcEx\\barcode.h"
#include "SrcEx\\myglobals.h"
// CThreeItemPackingDlg �Ի���
#define     TicketLen       (12)  
class CThreeItemPackingDlg : public CDialog
{
// ����
public:
	CThreeItemPackingDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CThreeItemPackingDlg();

// �Ի�������
	enum { IDD = IDD_THREEITEMPACKING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	//��Ϣ����ӿ�
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DeleteListAllItems(void);
	virtual int GetListAmountCount(void);
	virtual int GetListItemCount(void);
	virtual void AddItemToList(CString strimei, CString strccid, CListCtrl *list);
	virtual void AddItemToList(CString strtel, CString strimei, CString strccid, CListCtrl *list);


	BOOL   ItemExistInList(CString str, int nSubItem);
	void   EmptyItemSetFocus(int nId, CString &var);
	void   EmptyCEidtItemInList(BOOL bTypeCard = FALSE);
	void   EnableCEidtItemInList(BOOL bEnable);
	void   EnableCComboxInList(BOOL bEnable);	
	SHORT  InitCComboBoxByDiscontinuousStr(CComboBox *VarCtrl,        //CComboBox�ؼ���ָ��
		                                   CString &DiscontinuousStr, //���ϵ��־���ַ����磺12,34,56
										   char BrokenCh,             //�ϵ��־�����桰12,34,56���еġ�,��
										   int nCurSel = 0);          //��ʼ����

	//���༭�����ݵ�������
	int CheckEditIntegrity(void);
	//���б��ڵ��������ݴ洢�����ݿ�
	int  StoreListDataToAccess(void);
	//��ӡ�б��ڵ��������ݵ�һ�ű�ǩֽ��
	int  PrintfListData(void);
	//����б��еĺ������nstep,���ұ����´�,��������´���longֵ
	void UpdateNumEditAdd(CString &strVar, int nStep = 1);

private:
	BOOL         m_boolCardType;
	CString      m_strBuyer;
	CString      m_streditboxnum;
	CString      m_strcommodel;
	CString      m_strcomcolor;
	CString      m_strcomedition;
	CString      m_strcomamount;
	CString      m_strcomnshare;
	CString      m_strconfigpathname;
	CString      m_strTelnum;
	CString      m_strImei;
	CString      m_strCcid;		
	CString      m_strTicket;
	CString      m_strComboAmount;
	CString      m_strkeymsg;
	CString      m_strcardtype;

	KEYDEF       *Mykey;
	CSendkey     *MySendkey;
	int          m_nIndex;
	BOOL         m_bLock;
	char         *pdate;
	char         *ptime;

protected:
	CXPButton        m_CtrlButtonLock;
	CXPButton        m_CtrlButtonPrint;
	CXPButton        m_CtrlButtonStroe;
	CXPButton        m_CtrlButtonDelete;
	CAdoConnection   m_adoConnection;	

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	
	CComboBox                m_CtrlCommodel;
	CComboBox                m_CtrlComcolor;
	CComboBox                m_CtrlComedition;
	CComboBox                m_CtrlComamount;
	CComboBox                m_CtrlComnshare;
	CListCtrl                m_list;

	CStaticEx                m_stcticket;
	CEditEx                  m_CtrlTicket;
	CEditEx                  m_CtrlBuyer;
	CEditEx                  m_CtrlBoxnum;
	CEditEx                  m_CtrlTelnum;
	CEditEx                  m_CtrlImei;
	CEditEx                  m_CtrlCcid;

public:
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();	
	afx_msg void OnBnClickedButton2();	
	afx_msg void OnBnClickedButton3();	
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnCbnSelchangeComboModel();
	afx_msg void OnCbnSelchangeComboColor();
	afx_msg void OnCbnSelchangeComboZone();
	afx_msg void OnCbnSelchangeComboAmount();
	afx_msg void OnCbnSelchangeComboNshare();
};
