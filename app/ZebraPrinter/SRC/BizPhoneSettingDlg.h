// BizPhoneSettingDlg.h : header file
//

#if !defined(AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_)
#define AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <buttonxp.h>
#include <ComboBoxXP.h>
#include <stackdialog.h>
#include <PictureEx.h>
#include <XListCtrl.h>
#include <staticex.hxx>
#include <editex.hxx>
#include <IniFile.H>
#include <stackdialog.h>
#include <barcode.h>
#include "afxwin.h"

#ifdef USE_SERIAL_CODE
#define STRING_PROFILE_SECTION_REGISTER		_T("REGISTER")
#define STRING_PROFILE_ENTRY_SERIALCODE		_T("SerialCode")
#define STRING_PROFILE_ENTRY_PROBATION		_T("Probation")

#define STRING_CUSTOM_CODE_FILTER_PRE	    _T("1842")
#define STRING_CUSTOM_CODE_FILTER_POS	    _T("0349")
#define STRING_CUSTOM_CODE_FILTER		    _T("03491842")

#define STRING_SERIAL_CODE_FILTER_PRE	    _T("8421")
#define STRING_SERIAL_CODE_FILTER_POS	    _T("3750")
#define STRING_SERIAL_CODE_FILTER		    _T("37508421")
#define STRING_SERIAL_CODE_FILTER_TIMES		_T("5854")

#define STRING_RUNTIME_CODE_FILTER          _T("7296")
#endif


// 由于存在相互引用，故此处提前声明之

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneSettingDlg dialog

class CBizPhoneSettingDlg : public CStackDialog
{
	// Construction
public:
	CBizPhoneSettingDlg(CWnd* pParent = NULL);	// standard constructor
	
	virtual CStackPage *CreatePage(UINT nId);
	virtual void OnPageChanged(UINT nId, BOOL bActivated);
	


	BOOL DeleteThisFile(CString strFile);


#ifdef  CARD_ENCRYPT
    CString     m_strStartNet_0019_sEncryptSolution;
	CString     m_strStartNet_0018_sIfCompatibleCard;
	CString     m_strStartNet_0017_sVenderNO;
	CString     m_strStartNet_0031_sKeyNO;
	CString     m_strStartNet_0020_sKeyword1;
	CString     m_strStartNet_0021_sKeyword2;
	CString     m_strStartNet_0022_sKeyword3;
	CString     m_strStartNet_0023_sKeyword4;
	CString     m_strStartNet_0024_sKeyword5;
#endif	
	
public:
	// Dialog Data
	//{{AFX_DATA(CBizPhoneSettingDlg)
	enum { IDD = IDD_BIZPHONE_SEETING_SHEET };
	CPictureEx	m_ctrGifLogo;
	CButtonXP	m_btExit;
	CButtonXP   m_btLock;
	CTreeCtrl	m_ctrTree;
	CStaticEx   m_st_status;
	CStaticEx   m_st_tips;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBizPhoneSettingDlg)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
		
#ifdef USE_SERIAL_CODE
	void    GetHardwareID(CString& strHDSerial);
	void    GetHardwareID(DWORD& dwHDSerial);
	
	CString FormatCustomStringFromHardwareID();
	BOOL ProcessAccordingGivenString(CString& strObject, CString strGiven, BOOL bAdd = 1);
	CString ProcessADDAccordingGivenString(CString strObject, CString strGiven);
	
	BOOL    VerifyInputedSerialCode(CString strInputedSerial, BOOL bFirst = FALSE);
	
	void    QuitSoftware(UINT nExitCode1, DWORD dExitCode2 = 0);
	void    RunSoftware(void);
	
	CString LoadSerialCodeFromRegistry(void);
	void    SaveSerialCodeIntoRegistry(CString strSerial);
	
	DWORD   LoadRunNumberFromRegistry(void);
	void    SaveRunNumberIntoRegistry(DWORD nRunNumber);
	
	BOOL    LoadMaxRunNumFromRegisterInfo(DWORD& dwMaxRun, CString strSerial);
	BOOL    XtoD(DWORD& dValue, CString strMid);
#endif
	
	// Generated message map functions
	//{{AFX_MSG(CBizPhoneSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
#ifndef _VK_RETURN_EXIT_
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#endif//_VK_RETURN_EXIT_
	afx_msg void OnSelchangedOptionTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedSettingSheetBtnExit();
	afx_msg LRESULT NotifyShowStatus(WPARAM bNoError,LPARAM lpShowText);
	afx_msg void OnBnClickedSettingSheetBtnLock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	HICON                m_hIcon;

	// 机种数
	HTREEITEM m_hTypeRoot[INT_NUM_OF_PHONE_TYPE];
	CString m_strTypeName[INT_NUM_OF_PHONE_TYPE];

	// PG530 标签名称
	HTREEITEM m_hSubPg530[INI_NUM_OF_PG530_LABLE];	

public:
	// Pub name to be showed at the tree node
	CString m_strSubPg530[INI_NUM_OF_PG530_LABLE];

	void SavePageSet(void);
	void LockPageSet(BOOL bLock);
	
private:
	BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout);
	void DoEvents(void);
};

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
BOOL IsNumberChar(const char Src_Ch);

BOOL IsHexChar(const char Src_Ch);

char GetHexChar(int cVal);

BOOL IsAlphaChar(const char Src_Ch);

BOOL IsAlnumChar(const char Src_Ch);

BOOL IsEmptyOrAlnumString(const char *pSrc);

BOOL IsNumberString(CString &Src_string);  // 0 - 9

BOOL IsEmptyOrNumberString(CString &Src_string);// empty or only 1-9

BOOL IsHexString(CString &Src_string);     // 0 - 9, a - f, A - F

BOOL IsAlnumString(CString &Src_string);     // 0 - 9, a - z, A - Z

BOOL IsPrintString(CString &Src_string);     // 0x20 ~ 0x7E

BOOL IsEmptyOrHexString(CString &Src_string);// empty or only 0 - 9, a - f, A - F

BOOL IsPhoneString(CString &Src_string);		// only 1-9 and * and #

BOOL IsEmptyOrPhoneString(CString &Src_string); // empty or only 1-9 and * and #

void NumStrAutoAdd(char *IO, short increment = 1);

BOOL GetPostfixString(const char *Src_String, CString &szPostfix, int cTagChar = WM_TAG_CHAR, int bNoSpace = 1);

// 把格式为：Str1-Str2-Str3-Str4-...-Strn 的字符串，存放进数组，返回数组宽度
int PutStringIntoCStringArray(IN const char *szFormatString, OUT CStringArray &szArray, IN int cTagChar = WM_TAG_CHAR);

// 把数组里面的每一行打印到字符串，中间以字符 cTagChar 间隔
void PrintfCStringArrayToString(CString &szString, CStringArray &szPrintfStringArray, IN int cTagChar = WM_TAG_CHAR);

// 把数组里面的每一项填充到下拉列表中
int FillStringIntoComboBox(CComboBox *pCComboBox, CStringArray &szStringArray);

// 把下拉列表里面的所以字符串存放到数组里面去
int GetComboBoxItemToStringArray(CStringArray &szStringArray, CComboBox *pComboBox);

// 清空下拉列表
int ClearComboBoxItem(CComboBox *pComboBox);
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_)
