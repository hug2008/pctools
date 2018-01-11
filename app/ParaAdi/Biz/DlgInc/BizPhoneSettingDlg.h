// BizPhoneSettingDlg.h : header file
//

#if !defined(AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_)
#define AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "buttonxp.h"
#include "stackdialog.h"
#include "PictureEx.h"

#include "SerialComm.h"
#include "MainContent.h"
#include "WaitMsgDlg.h"

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

typedef enum PropPageIDTag
{
	PROPPAGE_SYS,
	PROPPAGE_LOCAL_AREA,
	PROPPAGE_PIN_SIM,
	PROPPAGE_LOCK,
#if defined(LOCK_SERVICE_PROVIDER)
	PROPPAGE_LOCK_OP,
#endif
	PROPPAGE_DIRECT,
	PROPPAGE_FORBID,
	PROPPAGE_REFUSE,
#if defined(ENABLE_MOBILE_HEAD)
	PROPPAGE_MOBILE_HEADER,
#endif
	PROPPAGE_LOCAL_MOBILESECT,
	PROPPAGE_LOCAL_USER_MOBILESECT,
	PROPPAGE_IP,
	PROPPAGE_MIC,
	PROPPAGE_LOCK_LAC,
#if !defined(NO_NXT)
	PROPPAGE_SCROLL,
	PROPPAGE_LIMIT,
#endif
#if defined(CXT_EX)
	PROPPAGE_TCPIP,
#endif

#if defined(NETWORK_CONFIG)
	PROPPAGE_EMERGENCY,
	PROPPAGE_OPN_CFG,
#endif

#if defined(CARD_ENCRYPT)
	PROPPAGE_ENCRYPTED,
#endif

	PROPPAGE_OUTPUT,
	PROPPAGE_DATA,
	PAGE_TOTAL
}PropPageID;

typedef struct PropTreeNoteTag
{
	PropPageID  pageId;
	int         nSrcId;
	char        szName[100];
}PropTreeNote;

#define PROP_TREE_NOTE_INIT(pageId, nSrcId, szName)\
                           {pageId, nSrcId, szName}


// 由于存在相互引用，故此处提前声明之
class CPropPageOutput;

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneSettingDlg dialog

class CBizPhoneSettingDlg : public CStackDialog
{
	// Construction
public:
	CBizPhoneSettingDlg(CWnd* pParent = NULL);	// standard constructor
	
	virtual CStackPage *CreatePage(UINT nId);
	virtual void OnPageChanged(UINT nId, BOOL bActivated);
	
	void ShowStatus(CString s, BOOL bNoError = TRUE);
	// void ShowStatus2(CString s);
	BOOL DeleteThisFile(CString strFile);
public:
	// is data changed?
	BOOL		m_bDataChanged;
	CString		m_strExchangeFileName;
	//	BOOL        m_bDisplayOnceTime;
	
	// 关于写密码的是否要继续写配置
	BOOL		m_bChangePwdAndSaveSetting;
	BOOL		m_bChangePwdAndSaveSettingAndWriteDefault;
	BOOL        m_bIsAllPasswordEmpty;
	
#if !defined(NO_NXT)
#if defined(CXT_EX)
	// ===========================================================================================
	// members for G166
	CString   m_strG166_IpAddress;
	CString   m_strG166_IpPort;
	CString   m_strG166_Dns1;
	CString   m_strG166_Dns2;
#endif
	// members for Scroll
	CString     m_strScroll_1001_HotLine;
	CString     m_strScroll_1002_NxtNum;
	CString     m_strScroll_1003_MobileSec;
	CString     m_strScroll_1004_IssuanceNum[7];
	CString     m_strScroll_13000_NxtTitle;
	CString     m_strScroll_13001_ScrollMatter;
	CString     m_strScroll_13002_ScrollAd;
	
	// members for MsgAd
	CString     m_strLimit_1020_MsgAd[40];
#endif
	// ======================================================================
	// members for SYS
	CString		m_strSYS_0002_nAutoDely;			// "01" [0,9]
	CString		m_strSYS_0004_sSMSCenter;			// "..." [1,16]
	CString		m_strSYS_4000_bNotLocalAddZero;		// 00/01
	CString		m_strSYS_0005_bForbidLongDistanceCall;		// 00/01
	
	// ======================================================================
	// members for LOCAL
	CString		m_strLOCAL_1000_strLocalZone;			// "..." [0,6]
	CString		m_strLOCAL_0003_nLocalMethod;			// 00/01/02
	
	// ======================================================================
	// members for PINSIM
	CString		m_strPINSIM_6000_bAutoLockPin;			// 0/1
	CString		m_strPINSIM_6001_nPinLength;			// "..." [4,8]
	CString		m_strPINSIM_6002_strOriginalPin; 		// "..." [4,8]
	CString		m_strPINSIM_6004_strFixedPin;			// "..." [4,8]
	CString		m_strPINSIM_6005_bUseRandomPin;			// 0/1
	
	CString		m_strPINSIM_7000_bLockSim;				// 0/1
	CString		m_strPINSIM_7001_strLockSimNum;			// "..." 定长20个字节
	
	// ======================================================================
	// members for Lock
	CString		m_strLOCK_7002_bLockProvider;			// 0/1
#if defined (LOCK_SERVICE_PROVIDER)
	CString     m_AryLOCK_7003_nProvider[SERVICE_PROVIDER_MAX];
#else
	CString		m_strLOCK_7003_nProvider;				// "..." [0,1]
#endif
	
	CString		m_strLOCK_8000_bLockRegion;				// 0/1	
	CString		m_strLOCK_8001_nRegionNum;				// "..." [0,20]
	CString		m_strLOCK_8002_bAutoLockRegion;			// 0/1
	CString		m_strLOCK_8003_strLockRegion; 			// "..." [0,20]*4


	// ===========================================================================================
	// members for Locking Lac
	CString		m_strLOCK_8004_bLockLac;			// 0/1
	CString     m_strLOCK_8005_strLockLac;          //first part of LacInf[50]
	CString     m_strLOCK_8006_strLockLac;          // second part of lacinf[50]
	
	// ===========================================================================================
	// members for Direct Table
	CString     m_AryDIRECT_2020_strDirectTable[INT_INIT_LISTBOX_ITEMNUM];	// 字符数组 0－9,*,#

//#if defined(NETWORK_CONFIG)
	// ===========================================================================================
	// members for CPropPageEmCallCfg Table
	CString     m_AryEmCall_2610_strNumberList[EMERGENCY_NUM_INT];

	// ===========================================================================================
	// members for CPropPageOpnCfg Table
	CString     m_AryNetWorkName_2750_strNameList[OPN_TOTAL];
//#endif
	
	// ===========================================================================================
	// members for Forbid Table
	CString     m_AryFORBID_3020_strForbidTable[INT_INIT_LISTBOX_ITEMNUM];	// 字符数组 0－9,*,#
	
	// ===========================================================================================
	// members for REFUSE
	CString		m_strREFUSE_5000_bRefuseType;			// 0/1
	CString		m_AryREFUSE_5020_strRefuseTable[INT_INIT_LISTBOX_ITEMNUM];	// 字符数组 0－9
	
	// ===========================================================================================
	// members for Local Mobile Sect
	// int			nLocalTableLength;					// < 400
	CStringArray	m_ArySECT_4400_strLocalTable;	// 字符数组 0－9,*,#
	
	// ===========================================================================================
	// members for Local User Mobile Sect
	CString     m_ArySECT_4420_strUserLocalTable[INT_INIT_LISTBOX_ITEMNUM];	// 字符数组 0－9,*,#
	
	// ===========================================================================================
	// members for IP
	CString     m_AryIP_9001_strIPRuleTable[INT_INIT_LISTBOX_ITEMNUM];	// 字符数组 0－9,*,#
	
	// ===========================================================================================
	// members for MIC
	CString		m_strMIC_CMIC0_nCMICMain;			// "02" [0,15]
	CString		m_strMIC_CMIC1_nCMICAux;			// "02" [0,15]
	
#if defined(R16_ATC)
	CString		m_strMIC_SIDET_MainGain;				// "4096" [0,32767]
	CString		m_strMIC_SIDET_AuxGain;				// "4096" [0,32767]
#else
	CString		m_strMIC_SIDET_Gain;				// "4096" [0,32767]
#endif
	
	CString		m_strMIC_ECHO0_Vox;					// "0" [0,32767]
	CString		m_strMIC_ECHO0_Min;					// "0" [0,32767]
	CString		m_strMIC_ECHO0_Samp;				// "0" [0,32767]
	
	CString		m_strMIC_ECHO1_Vox;					// "0" [0,32767]
	CString		m_strMIC_ECHO1_Min;					// "0" [0,32767]
	CString		m_strMIC_ECHO1_Samp;				// "0" [0,32767]
	
	// ===========================================================================================
	// members for Lcd contrast adjustment
	CString		m_strSYS_LCD_Contrast;			// "10" [10,40]
	
	
#ifdef _XIAMEN_START_BUILD
	// ===========================================================================================
	// members for XiaMen Start Setting
	CString		m_strXIAMEN_0006_sMscReceiveNum;	// "..." [0,16]   /*接收网管中心*/
	CString		m_strXIAMEN_0007_bShowOriginalNum;	// 00/01          /*是否只显示原始号码*/
	CString		m_strXIAMEN_0009_sProducerID;	    // "..." [1－23]  /*生产商ID信息*/
	CString		m_strXIAMEN_0011_bLocalSectEnabled; // 00/01		  /*本地手机号段是否生效*/
#endif // _XIAMEN_START_BUILD
	
#ifdef _YINGNUO_SECURITY_BUILD
	// ===========================================================================================
	// members for Yingnuo security Setting
	CString		m_strYINGNUO_0020_sSecurityNum0;	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第1组密钥*/
	CString		m_strYINGNUO_0021_sSecurityNum1;	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第2组密钥*/
	CString		m_strYINGNUO_0022_sSecurityNum2;	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第3组密钥*/
	CString		m_strYINGNUO_0023_sSecurityNum3;	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第4组密钥*/
	CString		m_strYINGNUO_0024_sSecurityNum4;	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第5组密钥*/
#endif // _YINGNUO_SECURITY_BUILD
	
#ifdef _TONGZE_PARA_BUILD
	// ===========================================================================================
	// members for Tongze Start Setting
	CString		m_strTONGZE_0009_sProducerID;	    // "..." [1－23]  /*生产商ID信息*/
	CString		m_strTONGZE_0011_bLocalSectEnabled; // 00/01		  /*本地手机号段是否生效*/
#endif // _TONGZE_PARA_BUILD

	
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
	

#if defined(ENABLE_MOBILE_HEAD)
	CString     m_strMobileHeader[MAX_MOBILE_HEADER_TOTAL];
#endif

public:
	// Dialog Data
	//{{AFX_DATA(CBizPhoneSettingDlg)
	enum { IDD = IDD_BIZPHONE_SEETING_SHEET };
	//CPictureEx	m_ctrGifLogo;
	CButtonXP	m_btExit;
	CTreeCtrl	m_ctrTree;
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
	afx_msg void OnSelchangedOptionTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDlgClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnChangePassword(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadFromUserIniFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveToUserIniFile(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLoadModuleSetting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveModuleSetting(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveModuleDefault(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecoverModuleDefault(WPARAM wParam, LPARAM lParam);
	
	long OnReceiveLowNotify(WPARAM wParam, LPARAM lParam);
	long OnReceiveHighNotify(WPARAM wParam, LPARAM lParam);
	long OnStartSendAtcommandNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnReadParaErrNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnReadParaOkNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnReadParaStateNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnWriteParaErrNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnWriteParaOkNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnWriteParaStateNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnWriteToDefaultSettingErrNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnWriteToDefaultSettingOkNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnRecoverDefaultSettingErrNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnRecoverDefaultSettingOkNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnChangePassErrNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnChangePassOkNotify(WPARAM wParam,LPARAM lParam);
	long OnStartMonitorTimerNotify(WPARAM wParam,LPARAM lParam);
	long OnStopMonitorTimerNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnActiveThisPageNotify(WPARAM wParam,LPARAM lParam);
	
	// added by Huang ye for activate commport
	LRESULT OnActivateCommPort(WPARAM wParam,LPARAM lParam);
	long OnStartSendingTimerNotify(WPARAM wParam,LPARAM lParam);
	long OnStopSendingTimerNotify(WPARAM wParam,LPARAM lParam);
	// added by Huang ye for activate commport
	
	// 2007.06.07 add by hqr to Open/Close Comm
    long OnOpenCommConnectionNotify(WPARAM wParam,LPARAM lParam);
	long OnCloseCommConnectionNotify(WPARAM wParam,LPARAM lParam);
	// 2007.06.07 add by hqr to Open/Close Comm

	// 2008.06.01 add by hqr to lock/unlock page
	LRESULT OnLockPoroPageNotify(WPARAM wParam,LPARAM lParam);
	// 2008.06.01 add by hqr to lock/unlock page


	LRESULT OnTreeControlNotify(WPARAM wParam,LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
		
protected:
	
	// 测试所有输入的数据是否合法
	BOOL CloseTest(void);
	
	// 将各个页面数据保存到全局变量中
	BOOL SaveData(void);
	
	// 将保存在全局变量中的数据体现到各个page中
	BOOL RefreshPage(void);

	// 显示状态
	void OnShowPageState(CompareIniState eState);

	// 锁定/解锁所有属性面页
	void EnablePoroPage(BOOL bEnable);
	
	// 读入INI文件
	void LoadIniFile(CString strLoadFile, BOOL bFromIni);

#if defined(PAGE_CFG)
	void SavePageOptionToIni(CString szIniFilePath);
#endif
	
	// 写出INI文件
	void SaveIniFile(CString strSaveFile, BOOL bFromIni);

	void ResetBizPhoneSetting(void);

	// 比较两个参数文件是否相同
	BOOL CompareIniSrcAndReadBack(CString szIniSrc, CString ReadBack);
	
	// 从模块写的特定文件中读出数据，并刷新界面
	void LoadFromModule(void);
	// added by Huang ye for activate commport
	void OnActivateComm(void);
	// added by Huang ye for activate commport
	void OnReadSetting(void);
	void OnWriteSetting(void);
	void OnWriteDefaultSetting(void);
	void OnChangePass(void);
	void OnRecoverDefaultSetting(void);
	
protected:
	HICON m_hIcon;
	
#if defined(USE_WAIT_WND)
	// 在进出信息的时候的，等待对话框
	CWaitMsgDlg m_dlgWaitMsg;
#endif
	
	// the pointer to CPropPageOutput CStackPage
	//	CPropPageOutput* m_pPageOutput;
	
	// tree control 根节点
	HTREEITEM m_htiRoot[PAGE_TOTAL];
	
	// name to be showed at the tree node
	CString m_strNodeName[PAGE_TOTAL];
	
private:
	BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout);
	void DoEvents(void);
	
public:
	void AppForceExit(void);         //强行终止程序
};

// ==========================================================================
// Global variable setting
// ==========================================================================
extern CSerialComm MainSerial;
extern CMainContent MainContent;

CompareIniState IniParaCmp(CString szFileOne, CString szFileTwo, CString szSecName);

CompareIniState IniParaIcmp(CString szFileOne, CString szFileTwo, CString szSecName);

void getIniParaFileSec(CString szFileOne,
					   CStringArray& m_FileOneSecs,
					   CString szFileTwo,
				       CStringArray& m_FileTwoSecs,
					   CString szSecName);

BOOL CompareParaIni(CString szFileOne, CString szFileTwo);

#endif // !defined(AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_)