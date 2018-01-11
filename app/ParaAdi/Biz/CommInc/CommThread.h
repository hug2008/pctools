#if !defined(AFX_COMMTHREAD_H__07402C10_E7FD_48B9_AD60_F219F0273EAD__INCLUDED_)
#define AFX_COMMTHREAD_H__07402C10_E7FD_48B9_AD60_F219F0273EAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommThread.h : header file
//


#include "MainContent.h"
#include "SerialComm.h"
#define  ISSUANCENUM_NUM    (7)
#define  DIRECT_NUM			19
#define  BAN_NUM            19


#define  USER_LOCALPHONE_HEAHER  (20)

#define  LOCAL_SEGMENT_NUM  (INT_INIT_LOCAL_PHONE_ITEMNUM + USER_LOCALPHONE_HEAHER - 1)

#define  STRICT_INCOM_NUM   19
#define  IP_SET_NUM         19

// #define  _DEBUG_AT
#define  STRING_AT_LOG_NAME		"At.log"
#define  MAX_ACTIVATE_COMMPORT_TIMES   (150)

/////////////////////////////////////////////////////////////////////////////
// CCommThread thread

class CCommThread : public CWinThread
{
	DECLARE_DYNCREATE(CCommThread)
protected:
	CCommThread();           // protected constructor used by dynamic creation
	
	// Attributes
public:
	typedef struct CommandReceiveAllTag
	{
		int    commandlength;                        
		char   commanddata[MAXSENDCHARNUM];
	}CommandReceiveAll;
	
	CommandReceiveAll     CommThreadData;
	// added by Huang ye for activate commport
	typedef enum ActivateCommPortStateTag
	{
		ACTIVATE_COMMPORT_IDLE_STATE,
		ACTIVATE_COMMPORT_START_STATE,
		ACTIVATE_COMMPORT_OK_STATE
	}ActivateCommPortState;
	
	// added by Huang ye for activate commport
	typedef enum CommThreadStateTag            
	{		
		COMMTHREAD_IDLE_STATE,
		// added by Huang ye for activate commport
		ACTIVATE_COMMPORT,
		// added by Huang ye for activate commport
		COMMTHREAD_READ_TOTAL_SETTING,
		COMMTHREAD_WRITE_TOTAL_SETTING,
		COMMTHREAD_SAVE_TO_DEFAULT_SETTING,
		COMMTHREAD_RECOVER_DEFAULT_SETTING,
		COMMTHREAD_CHANGE_PASS
	}CommThreadState;
	
	typedef enum  PhoneModeTypeTag{
		PhoneModeERROR,
		DESAY_G169FV10ME,
		DESAY_G169FV11ME,
		DESAY_G169FV50ME,
		PhoneMode_UNKNOW
	}PhoneModeType;
	
	// 探测移动终端是否连接的状态定义 
	ActivateCommPortState      activateCommPortState;
	CommThreadState            commThreadState;
	ReadTotalSettingState      readTotalSettingState;
	WriteTotalSettingState     writeTotalSettingState;
	WriteToDefaultSettingState writeToDefaultSettingState;
	RecoverDefaultSettingState recoverDefaultSettingState;
	ChangePassState            changePassState;
	PhoneModeType              phonemodetype;
	
	char AtStringCR[FIXATCOMMANDLENGTH];
	char AtStringOK[FIXATCOMMANDLENGTH];
	char AtStringError[FIXATCOMMANDLENGTH];
	char AtStringWPAR[FIXATCOMMANDLENGTH];
	char AtStringRPAR[FIXATCOMMANDLENGTH];
	char AtStringNOCARRIER[FIXATCOMMANDLENGTH];
	char AtStringBUSY[FIXATCOMMANDLENGTH];
	char AtStringNETREADY[FIXATCOMMANDLENGTH];
	char AtStringSaveSetting[FIXATCOMMANDLENGTH];
	char AtStringSaveSetting_V11[FIXATCOMMANDLENGTH];
	char AtStringSaveToDefaultSetting[FIXATCOMMANDLENGTH];
	char AtStringRecoverDefaultSetting[FIXATCOMMANDLENGTH];
    char AtStringCMIC[FIXATCOMMANDLENGTH];
	char AtStringSIDET[FIXATCOMMANDLENGTH];
	char AtStringECHO[FIXATCOMMANDLENGTH];	
	char AtStringCTRS[FIXATCOMMANDLENGTH];	
	char AtStringAndF[FIXATCOMMANDLENGTH];
	char AtStringAndW[FIXATCOMMANDLENGTH];
	// added by Huang ye for activate commport
	char AtStringat[FIXATCOMMANDLENGTH];
	// added by Huang ye for activate commport	
	CStringArray m_paraInfo ;
	CStringArray m_paraValue;
	
	int m_nEncryptSolution;
	
	
	// Operations
public:
	UINT GetParaFileInfoAndValue(void);
	UINT GetParaFileAtCmdInfo(void);
	void WriteCommand(char *command);
	void CreateReadParaCommand(int iCMD);
	void CreateWriteParaCommand(UINT iIndex);
	void CreateWriteAtCmdCommand(UINT iIndex);
	void CreateChangePassCommand(CString sOldPass,CString sNewPass);
	void CreateBeginWriteCommand(CString sPass);
	void CreateEndWriteCommand();
	void CreateReadAtCmdCommand(CString sAtCmd);
	void CreateAtivateCmd(void);	
	
	void SendCreatedCommand(void);
	void *receiveSerialData;
	BOOL ReceiveShortMessageHead;
	void OnSendCommNotify(WPARAM wParam, LPARAM lParam);
	void OnClearStateNotify(WPARAM wParam, LPARAM lParam);
	void OnCommManageNotify(WPARAM wParam, LPARAM lParam);
	BOOL GetParaAndWriteFile(CString val, UINT iCMD);
	BOOL GetAtCmdAndWriteFile(CString sAtCmd,CString sVal);
#if defined(R16_ATC)
	BOOL GetAtCmdSidetAndWriteFile(CString sAtCmd,CString sVal);
#endif
	BOOL GetAtCmdEchoAndWriteFile(CString sAtCmd,CString sVal,CString sChannel);
	void NotifyMainWnd(UINT msg,WPARAM wParam,LPARAM lParam = 0);
	
	void CommThreadForceExit();
	
	int  GetEncryptSolution(CString val);
	void GetOptionAndWriteFile(CString szOptionName, CString szSubOptionName, CString szOptionValue);
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommThread)
public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// added by Huang ye for activate commport
	void StopSendingTimer();
	// added by Huang ye for activate commport
	
	void StopMoniterTimer();
	
	virtual ~CCommThread();
	
	// Generated message map functions
	//{{AFX_MSG(CCommThread)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
private:
	void NotifyAtCommandStartSend(CommandType commandtype);
	
	void CommThreadInfoExit(char* info);
	
	void CommThreadOkExit(char *okinfo);
	
	void CommThreadErrorExit(char *errorinfo);
	void CommThreadWarnExit(char *warninfo);
	BOOL GetErrorResult(char *returndata);
	BOOL GetReturnResult(char *returndata);
	CString GetParam(CString& mName);
	void ResetMoniterTimer();
	
	// added by Huang ye for activate commport
	void ResetSendingTimer();
	// added by Huang ye for activate commport
	
private:	
#ifdef _DEBUG_AT
	FILE* fAtLogFile;
	// CFile fAtLogFile;
	void WriteAtLog(CString string, BOOL bWrite);
#endif //_DEBUG_AT
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMTHREAD_H__07402C10_E7FD_48B9_AD60_F219F0273EAD__INCLUDED_)