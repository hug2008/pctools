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

// ���ڴ����໥���ã��ʴ˴���ǰ����֮
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
/* ����һ�������,ʹ��ǰ�ȵ���srand((unsigned)time( NULL ));���г�ʼ��  */
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
	
	// ����д������Ƿ�Ҫ����д����
	BOOL		m_bChangePwdAndSaveSetting;
	BOOL		m_bChangePwdAndSaveSettingAndWriteDefault;
	BOOL        m_bIsAllPasswordEmpty;


	/************************************************************************/
	/*       3In1 �����������ݳ�Ա                                          */
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
	/*            PHONE_CITY_NUMBER_SETTING �������Ų���                    */
	/************************************************************************/
	CString    szCityNum1000;

	/************************************************************************/
	/*           PHONE_HOT_NUMBER_SETTING  ֱ���Ŷβ���                     */
	/************************************************************************/
	CString   szHotNum2001[NUM_HOT_NUMBER_TOTAL];

	/************************************************************************/
	/*      PHONE_BARRING_NUMBER_SETTING   �����Ŷβ���                     */
	/************************************************************************/
	CString   szBarringCallNum3001[NUM_BARRING_NUMBER_TOTAL];

	/************************************************************************/
	/*      PHONE_LOCAL_NUMBER_HEADER_SETTING      ���غ���β���           */
	/************************************************************************/
	CString   szAddZeroIfOtherCity4000,
		      szLocalPhoneNumberHead[NUM_LOCALPHONE_HEADER_TOTAL];

	/************************************************************************/
	/*        PHONE_LIMIT_INCOMING_NUMBER_SETTING  ���ƺ��벿��             */
	/************************************************************************/
	CString   szLimitAllIncomingCall5000,
		      szLimitIncomingCallHead5001[NUM_LIMITINCOMING_HEADER_TOTAL];

	/************************************************************************/
	/*       PHONE_PIN_SETTING     Pin������                                */
	/************************************************************************/
	CString   szIsHandsetAutoLockPinEnable6000,/*�����Զ���PIN��*/
		      szPinLen6001,/*����PIN�볤��*/
			  szInitPin6002,/*���ó�ʼPIN��*/
			  szEncryptPin6003,/*���PIN��*/
			  szFixPin6004,/*�̶�PIN��*/
			  szUseEncryptPinFlag6005;/*�Ƿ�ʹ�����PIN��*/

	/************************************************************************/
	/*      PHONE_LOCK_SIM_SETTING           ������������Ӫ������           */
	/************************************************************************/
	CString   szIsHandsetCanBeBindedToASim7000,/*���û�����SIM��*/
		      szLockedImsi7001,/*�����󶨵�SIM����*/
			  szIsServiceProviderLockEnable7002,/*�Ƿ�������Ӫ��*/
			  szLockedServiceProvider7003;/*��������Ӫ��*/
	

	/************************************************************************/
	/*      PHONE_LOCK_CELL_SETTING   ��С������                            */
	/************************************************************************/
	CString   szIsCellLockEnable8000,/*�Ƿ���С��*/
		      szLockedCellNum8001,/*������С����*/
			  szIsCellAutoLockEnable8002,/*�Ƿ��Զ���С��*/
			  szCellInf8003;/*�Ѿ�������С����*/
#if defined(LOCK_LAC)
	CString   szIsLacLockEnable8004,/*�Ƿ���lac*/
			  szLockedLacNum8005,/*������Lac������*/
			  /* LAC_DOZEN_COUNT_AT_TOTAL * LAC_DOZEN_ONE_AT_TOTAL * 4 */
			  szLacInf8006[LAC_DOZEN_COUNT_AT_TOTAL];/* ��������Lac�� */
#endif

	/************************************************************************/
	/*     PHONE_IPHEDER_SETTING  Ip ���Ź��򲿷� ���������                */
	/************************************************************************/
	CString   szMajoripFramIpSetting[NUM_IP_HEADER_TATOL];

	/************************************************************************/
	/*      PHONE_INT_SPEED_DIAL_SETTING     �����ٲ�����                   */
	/************************************************************************/
#if defined(INTELLIGENT_SPEED_DIAL)
	CString  szIntSpeedDialSetting[NUM_INT_SPEED_DIAL_TOTAL];
#endif


	/************************************************************************/
	/*    PHONE_SPECIAL_DIAL_SETTING     ���Ⲧ�Ų�����                     */
	/************************************************************************/
#if defined(SPECIAL_DIAL)
	CString  szSpecialDialSetting[NUM_INT_SPEED_DIAL_TOTAL];
#endif

	/************************************************************************/
	/*      PHONE_CURRENT_TOLL_RATE_SETTING ���ʱ����ò���                  */
	/************************************************************************/
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
	/*      �����ʱ�����,�շѹ���������                                     */
	CString  szMajorRateFramRateSetting12001[NUM_RATE_SETTING_TABLE_TOTAL],
	/*      �����ʱ�����,����ʱ��                                           */
		     szMajorRateFramStartTime12026,
    /*      �����ʱ�����, �����ļ� Id                                       */
	         szMajorRateFramID12027,
	/*      �����ʱ�����, ���ʺŶμ�¼����                                  */
	         szMajorRateFramRateHeadCount12028,
	/*      �����ʱ�����, ���ʱ��¼����                                    */
	         szMajorRateFramRateCount12029,
	/*      �����ʱ�����, �����ļ�֡���                                    */
	         szMajorRateFramFramFlag12030,
	/*      �����ʱ�����,���ʺŶ���ʼ��ַ                                   */
	         szMajorRateFramRateHeadSetting[NUM_RATE_SETTING_HEADER_TYPE_TOTAL],
    /*      �����ʱ�����,�����ļ�У����                                     */
			 szMajorRateFramRateVerify12800;

#if defined(VG_MINOR_RATE)
	/*      �η��ʱ�����,�շѹ���������                                     */
	CString  szMinorRateFramRateSetting12171[NUM_RATE_SETTING_TABLE_TOTAL],
	/*      �η��ʱ�����,����ʱ��                                           */
		     szMinorRateFramStartTime12196,
	/*      �η��ʱ�����, �����ļ� Id                                       */
		     szMinorRateFramID12197,
	/*      �η��ʱ�����, ���ʺŶμ�¼����                                  */
		     szMinorRateFramRateHeadCount12198,
	/*      �η��ʱ�����, ���ʱ��¼����                                    */
		     szMinorRateFramRateCount12199,
	/*      �η��ʱ�����, �����ļ�֡���                                    */
		    szMinorRateFramFramFlag12200,
	/*      �η��ʱ�����,���ʺŶ���ʼ��ַ                                   */
		    szMinorRateFramRateHeadSetting[NUM_RATE_SETTING_HEADER_TYPE_TOTAL],
	/*      �η��ʱ�����,�����ļ�У����                                     */
		    szMinorRateFramRateVerify12970;

#endif
#endif


	/************************************************************************/
	/*        PHONE_MOBILE_HEAD_SETTING �ƶ��绰�Ŷ�ͷ                      */
	/************************************************************************/
#if defined(ENABLE_MOBILE_HEAD)
	CString  szMobileHeader14001[MOBILE_HEAD_TOTAL];
#endif

	/************************************************************************/
	/*       ATCMD ����                                                     */
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
	/*          ��������ļ����ܰ�У����                                    */
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
	
	// ������������������Ƿ�Ϸ�
	BOOL CloseTest(void);
	
	// ������ҳ�����ݱ��浽ȫ�ֱ�����
	BOOL SaveData(void);
	
	// ��������ȫ�ֱ����е��������ֵ�����page��
	BOOL RefreshPage(void);

	// ��ʾ״̬
	void OnShowPageState(CompareIniState eState);

	// ����/��������������ҳ
	void EnablePoroPage(BOOL bEnable);
	
	// ����INI�ļ�
	void LoadIniFile(CString strLoadFile, BOOL bFromIni);
	
	// д��INI�ļ�
	void SaveIniFile(CString strSaveFile, BOOL bFromIni);

	// �Ƚ����������ļ��Ƿ���ͬ
	BOOL CompareIniSrcAndReadBack(CString szIniSrc, CString ReadBack);
	
	// ��ģ��д���ض��ļ��ж������ݣ���ˢ�½���
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
	// �ڽ�����Ϣ��ʱ��ģ��ȴ��Ի���
	CWaitMsgDlg m_dlgWaitMsg;
#endif
	
	// the pointer to CPropPageOutput CStackPage
	//	CPropPageOutput* m_pPageOutput;
	
	// tree control ���ڵ�
	HTREEITEM m_htiRoot[PAGE_TOTAL];
	
private:
	BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout);
	void DoEvents(void);
	
public:
	void AppForceExit(void);         //ǿ����ֹ����
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

/*  ��������ļ����ܰ�У����  */
BOOL RateFramRateVerifyCode(char *Out_CheckCode, const char *In_HexField);
#endif // !defined(AFX_BIZPHONESETTINGDLG_H__C81CA411_3BD2_4AD9_BB4D_8E94C6D634F9__INCLUDED_)