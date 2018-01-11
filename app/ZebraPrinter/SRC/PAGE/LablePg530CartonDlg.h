#pragma once

#include ".\bizphonesettingdlg.h"
#include "afxwin.h"
#include "afxcmn.h"
// CLablePg530CartonDlg dialog

#define   ZPL_PG_CATON_AXIS_X   _T("X_axis")
#define   ZPL_PG_CATON_AXIS_Y   _T("Y_axis")
#define   ZPL_PG_CATON_FONT_A   _T("A_font")
#define   ZPL_PG_CATON_FONT_W   _T("W_font")
#define   ZPL_PG_CATON_FONT_H   _T("H_font")
#define   ZPL_PG_CATON_BARCODE  _T("BcFmt")/*^B3N,N,,Y,N*/
#define   ZPL_PG_CATON_BCBY_RA  _T("BcBy_Ra")/* ^BY2,2, */
#define   ZPL_PG_CATON_BCBY_H   _T("BcBy_H")/* ^BY2,2,H */
#define   ZPL_PG_CATON_DIS_VTC  _T("BcSpaceVertical")/* ��ֱ��� */
#define   ZPL_PG_CATON_DIS_HZT  _T("BcSpaceHorizontal")/* ˮƽ��� */
#define   ZPL_PG_CATON_SNO_REP  _T("SnoReplace")/* S/N:��ȥ����ǰ׺�Ŷ� */

class CLablePg530CartonDlg : public CStackPage
{
public:
	CLablePg530CartonDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLablePg530CartonDlg();

// Dialog Data
	enum { IDD = IDD_PG530_CARTONBOX };

public:
	virtual void OnSaveData(void);
	virtual void OnLockPage(BOOL bLock);

protected:
	CStaticEx      m_tModel;
	CComboBoxXP    m_cModel;

	CStaticEx      m_tCtboxid;
	CEditEx        m_eCtboxid;
	CString        m_szCtboxid;
	int            m_nCtboxidLen;

	CStaticEx      m_tCount;
	CComboBoxXP    m_cCount;
	CString        m_szCount;

	CStaticEx      m_tImei;
	CEditEx        m_eImei;
	CString        m_szImei;

	CStaticEx      m_tSno;
	CEditEx        m_eSno;
	CString        m_szSno;
	int            m_nSnoLen;
	CString        m_szSnoRep;

	CXListCtrl     m_list;

	CStaticEx      m_tReprint;
	CEditEx        m_eReprint;
	CString        m_szReprint;

	CStringArray   m_szArray;

	CString        m_szImeiBarTip;
	CString        m_szSnoBarTip;

	CBizPhoneSettingDlg *m_pParentWnd;

	CStringArray   m_szImeiArray;
	CStringArray   m_szSnoArray;

    /* ������:NC00001 */
	CStaticEx      m_stOrder;
	CEditEx        m_Order;
	CString        m_strOrder;
	
    /* �������� */
	CStaticEx      m_stZone;
	CEditEx        m_eZone;
	CString        m_strZone;
	
    /* AG-169H(PCO)���⴦�� */
	CStaticEx      m_stAg169h;
	BOOL           m_bAg169h;
	CButton        m_cAg169h;
	int            m_nLenAg169h;

	BOOL           m_bSortImei;

	BOOL           m_bSortSno;

protected://for barcode
	CString     m_bXaxis; // x - ��ĳ�ʼֵ
	CString     m_bYaxis; // y - ��ĳ�ʼֵ
	CString     m_bAfont; // ����
	CString     m_bWfont; // �ֺ� - ���
	CString     m_bHfont; // �ֺ� - �߶�
	CString     m_bBcFmt; // ���� - �����ʽ
	CString     m_bBcByRa;// ���� - ���ܱ�
	CString     m_bBcByH; // ���� - �߶�
	CString     m_bBcSpaceVertical;// ����Ĵ�ֱ���
	CString     m_bBcSpaceHorizontal;// �����ˮƽ���

	CString     m_szZpl;  // �����ӡָ��
	CString     m_szSql;  // SQLָ��

protected:
	BOOL CheckMeSeting(void);

	BOOL PrintMeLable(void);

	void UpDateMeAfterPrint(void);

	void GetlistData(CStringArray &szImeiArray, CStringArray &szSnoArray);

	void SaveListData(CStringArray &szImeiArray, CStringArray &szSnoArray);

	void RefreshList(void);

	int  InsertItemToList(int nItemIndex, const char* szImei, const char* szSno);

	void ProcessWmsgImeiEdit(MSG* pImeiMsg);

	void ProcessWmsgSerialNoEdit(MSG* pSnoMsg);

	void ProcessWmsgReprintEdit(MSG* pReprintMsg);
    
    /* ��ѯ��¼�����ݿ��¼�б�����к� Serial */ 
	long GetItemIndexInMdb(const char* szFieldName, const char *szVal);
	/* ��List�ؼ������ݱ��浽���ݿ� */
	void SaveListDataToMdb(CStringArray &szImeiArray, CStringArray &szSnoArray);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
#ifndef _VK_RETURN_EXIT_
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#endif//_VK_RETURN_EXIT_
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkCartonList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeCombCartonQuantity();
	afx_msg void OnBnClickedCheckAg169h();/* AG-169H(PCO) ��ѡ���� */
	DECLARE_MESSAGE_MAP()
};

int IsItemOccurInList(CListCtrl* pListCtrl, int nCol, const char* szItemText);