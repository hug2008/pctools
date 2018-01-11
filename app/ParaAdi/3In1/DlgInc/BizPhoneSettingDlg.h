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
#include "Para3In1Def.h"

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
class CPropPageOutput;

typedef enum PropPageIDTag
{
	PROPPAGE_SYS,
	PROPPAGE_PINSIM,
	PROPPAGE_LOCK,
	PROPPAGE_DIRECT,
	PROPPAGE_FORBID,
	PROPPAGE_REFUSE,
#if defined(ENABLE_MOBILE_HEAD)
	PROPPAGE_MOBILE_HEADER,
#endif
	PROPPAGE_LOCAL_MOBILE,

#if defined(ENABLE_AREA_CODE)
	PROPPAGE_AREA_CODE,
#endif

#if defined(LOCK_LAC)
	PROPPAGE_LOCK_LAC,
#endif
	PROPPAGE_IP,
	PROPPAGE_MIC,
	PROPPAGE_FEE,
	PROPPAGE_FEESECT,
	PROPPAGE_SWITCH,
#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
	PROPPAGE_ENCRYPT,
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

PropPageID getPageIdBySrcId(int nSrcId);



/************************************************************************/
/* 产生一个随机数,使用前先调用srand((unsigned)time( NULL ));进行初始化  */
#define GetRandom(minLimit, maxLimit) ((rand()%(int)(((maxLimit)+1)-(minLimit)))+(minLimit))
/************************************************************************/

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

	void Reset3In1PhSetting(void);
public:
	// is data changed?
	BOOL		m_bDataChanged;
	CString		m_strExchangeFileName;
	//	BOOL        m_bDisplayOnceTime;
	
	// 关于写密码的是否要继续写配置
	BOOL		m_bChangePwdAndSaveSetting;
	BOOL		m_bChangePwdAndSaveSettingAndWriteDefault;
	BOOL        m_bIsAllPasswordEmpty;


	/************************************************************************/
	/*       3In1 话机参数数据成员                                          */
	/************************************************************************/
	/************************************************************************/
	/*                  PHONE_NORMAL_SETTING                                */
	/************************************************************************/
	CString    szDialWaitingTime02,
		       szHowToUseCityNumber03,
			   szMscNum04,
			   szBaringOtherCityNum05,
			   szMscReceiveNum06,
			   szDisplayOriginalNumber07,
#if defined(CELL_LEVEL)
			   szCurrentCellLevelQuality08,
#endif
			   szClientID09,
			   szLocalNumberActive11,
#if defined(ICCID_BUILD)
			   szImeiCcid12,
#endif
			   szEncryptConfigfactoryId17,
			   szEncryptConfigBCompatibleWithNormalSim18,
			   szEncryptConfigSimEncryptSolution19,
			   szEncryptConfigEditionId31,
			   szEncryptConfigMainSimMpkKey689[5],
			   szIntelligentDial26,
			   szAutoLock27,
			   szTollIndiction28,
			   szSpecialApplication29,
			   szUserTag30;
	/************************************************************************/
	/*            PHONE_CITY_NUMBER_SETTING 本地区号部分                    */
	/************************************************************************/
	CString    szCityNum1000;

	/************************************************************************/
	/*           PHONE_HOT_NUMBER_SETTING  直拨号段部分                     */
	/************************************************************************/
	CString   szHotNum2001[NUM_HOT_NUMBER_TOTAL];

	/************************************************************************/
	/*      PHONE_BARRING_NUMBER_SETTING   禁拨号段部分                     */
	/************************************************************************/
	CString   szBarringCallNum3001[NUM_BARRING_NUMBER_TOTAL];

	/************************************************************************/
	/*      PHONE_LOCAL_NUMBER_HEADER_SETTING      本地号码段部分           */
	/************************************************************************/
	CString   szAddZeroIfOtherCity4000,
		      szLocalPhoneNumberHead[NUM_LOCALPHONE_HEADER_TOTAL];

	/************************************************************************/
	/*        PHONE_LIMIT_INCOMING_NUMBER_SETTING  限制呼入部分             */
	/************************************************************************/
	CString   szLimitAllIncomingCall5000,
		      szLimitIncomingCallHead5001[NUM_LIMITINCOMING_HEADER_TOTAL];

	/************************************************************************/
	/*       PHONE_PIN_SETTING     Pin码设置                                */
	/************************************************************************/
	CString   szIsHandsetAutoLockPinEnable6000,/*设置自动锁PIN码*/
		      szPinLen6001,/*设置PIN码长度*/
			  szInitPin6002,/*设置初始PIN码*/
			  szEncryptPin6003,/*随机PIN码*/
			  szFixPin6004,/*固定PIN码*/
			  szUseEncryptPinFlag6005;/*是否使用随机PIN码*/

	/************************************************************************/
	/*      PHONE_LOCK_SIM_SETTING           锁机锁卡锁运营商设置           */
	/************************************************************************/
	CString   szIsHandsetCanBeBindedToASim7000,/*设置话机绑定SIM卡*/
		      szLockedImsi7001,/*话机绑定的SIM卡号*/
			  szIsServiceProviderLockEnable7002,/*是否锁定运营商*/
			  szLockedServiceProvider7003;/*锁定的运营商*/
	

	/************************************************************************/
	/*      PHONE_LOCK_CELL_SETTING   锁小区设置                            */
	/************************************************************************/
	CString   szIsCellLockEnable8000,/*是否锁小区*/
		      szLockedCellNum8001,/*锁定的小区数*/
			  szIsCellAutoLockEnable8002,/*是否自动锁小区*/
			  szCellInf8003;/*已经锁定的小区号*/
#if defined(LOCK_LAC)
	CString   szIsLacLockEnable8004,/*是否锁lac*/
			  szLockedLacNum8005,/*锁定的Lac大区数*/
			  /* LAC_DOZEN_COUNT_AT_TOTAL * LAC_DOZEN_ONE_AT_TOTAL * 4 */
			  szLacInf8006[LAC_DOZEN_COUNT_AT_TOTAL];/* 已锁定的Lac号 */
#endif

	/************************************************************************/
	/*     PHONE_IPHEDER_SETTING  Ip 拨号规则部分 号码调整表                */
	/************************************************************************/
	CString   szMajoripFramIpSetting[NUM_IP_HEADER_TATOL];

	/************************************************************************/
	/*      PHONE_INT_SPEED_DIAL_SETTING     智能速拨参数                   */
	/************************************************************************/
#if defined(INTELLIGENT_SPEED_DIAL)
	CString  szIntSpeedDialSetting[NUM_INT_SPEED_DIAL_TOTAL];
#endif


	/************************************************************************/
	/*    PHONE_SPECIAL_DIAL_SETTING     特殊拨号参数表                     */
	/************************************************************************/
#if defined(SPECIAL_DIAL)
	CString  szSpecialDialSetting[NUM_INT_SPEED_DIAL_TOTAL];
#endif

	/************************************************************************/
	/*      PHONE_CURRENT_TOLL_RATE_SETTING 费率表设置部分                  */
	/************************************************************************/
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
	/*      主费率表设置,收费规律总条数                                     */
	CString  szMajorRateFramRateSetting12001[NUM_RATE_SETTING_TABLE_TOTAL],
	/*      主费率表设置,起跳时间                                           */
		     szMajorRateFramStartTime12026,
    /*      主费率表设置, 费率文件 Id                                       */
	         szMajorRateFramID12027,
	/*      主费率表设置, 费率号段记录总数                                  */
	         szMajorRateFramRateHeadCount12028,
	/*      主费率表设置, 费率表记录总数                                    */
	         szMajorRateFramRateCount12029,
	/*      主费率表设置, 费率文件帧标记                                    */
	         szMajorRateFramFramFlag12030,
	/*      主费率表设置,费率号段起始地址                                   */
	         szMajorRateFramRateHeadSetting[NUM_RATE_SETTING_HEADER_TYPE_TOTAL],
    /*      主费率表设置,费率文件校验码                                     */
			 szMajorRateFramRateVerify12800;

#if defined(VG_MINOR_RATE)
	/*      次费率表设置,收费规律总条数                                     */
	CString  szMinorRateFramRateSetting12171[NUM_RATE_SETTING_TABLE_TOTAL],
	/*      次费率表设置,起跳时间                                           */
		     szMinorRateFramStartTime12196,
	/*      次费率表设置, 费率文件 Id                                       */
		     szMinorRateFramID12197,
	/*      次费率表设置, 费率号段记录总数                                  */
		     szMinorRateFramRateHeadCount12198,
	/*      次费率表设置, 费率表记录总数                                    */
		     szMinorRateFramRateCount12199,
	/*      次费率表设置, 费率文件帧标记                                    */
		    szMinorRateFramFramFlag12200,
	/*      次费率表设置,费率号段起始地址                                   */
		    szMinorRateFramRateHeadSetting[NUM_RATE_SETTING_HEADER_TYPE_TOTAL],
	/*      次费率表设置,费率文件校验码                                     */
		    szMinorRateFramRateVerify12970;

#endif
#endif


	/************************************************************************/
	/*        PHONE_MOBILE_HEAD_SETTING 移动电话号段头                      */
	/************************************************************************/
#if defined(ENABLE_MOBILE_HEAD)
	CString  szMobileHeader14001[MOBILE_HEAD_TOTAL];
#endif

	/************************************************************************/
	/*       ATCMD 部分                                                     */
	/************************************************************************/
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

	/************************************************************************/
	/*          计算费率文件的总包校验码                                    */
	/************************************************************************/
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
	CString  MakeFeeVerifyCode(void);
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
	
	LRESULT OnReceiveLowNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnReceiveHighNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnStartSendAtcommandNotify(WPARAM wParam, LPARAM lParam);
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
	LRESULT OnStartMonitorTimerNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnStopMonitorTimerNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnActiveThisPageNotify(WPARAM wParam,LPARAM lParam);
	
	// added by Huang ye for activate commport
	LRESULT OnActivateCommPort(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartSendingTimerNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnStopSendingTimerNotify(WPARAM wParam,LPARAM lParam);
	// added by Huang ye for activate commport
	
	// 2007.06.07 add by hqr to Open/Close Comm
    LRESULT OnOpenCommConnectionNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseCommConnectionNotify(WPARAM wParam,LPARAM lParam);
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
	
	// 写出INI文件
	void SaveIniFile(CString strSaveFile, BOOL bFromIni);

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

extern int  GetEncryptSolution(CString val);

extern BOOL IsIpSettingItemEmpty(CString szVal);
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
extern BOOL IsRateTableItemEmpty(CString szVal);
extern BOOL IsRateRuleItemEmpty(CString szVal);
#endif

/*  计算费率文件的总包校验码  */
BOOL RateFramRateVerifyCode(char *Out_CheckCode, const char *In_HexField);
#endif // !defined(AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_)