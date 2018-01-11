// CommThread.cpp : implementation file
//

#include "stdafx.h"
#include "CommThread.h"
#include "Afxwin.h"
#include "Inifile.h"
#include "myheader.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if !defined(SYS_NAME)
#define SYS_NAME  _T("3In1PhManagerR16")
#endif
extern CMainContent MainContent;

//added by stevens,used for creat the key for unlocking the Lock_Process
static const __int8 EncipherSource[36]={'Z','T','E','C','O','R','P','S','H','A','N','G','H','A','I','N','O','.','2','R','E','S','E','A','R','C','H','I','N','S','T','I','T','U','T','E'};
/////////////////////////////////////////////////////////////////////////////
// CCommThread

IMPLEMENT_DYNCREATE(CCommThread, CWinThread)

CCommThread::CCommThread()
{
    receiveSerialData = NULL;
    commThreadState   = COMMTHREAD_IDLE_STATE;
    AtStringCR[0]     = 13;
    AtStringCR[1]     = '\0';
	
    memset(AtStringNOCARRIER,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringNOCARRIER,"NO CARRIER");
	
    memset(AtStringNETREADY,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringNETREADY,"NET READY");
	
    memset(AtStringBUSY,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringBUSY,"BUSY");
	
    memset(AtStringOK, '\0', FIXATCOMMANDLENGTH);
    strcat(AtStringOK, "OK");
	
    memset(AtStringError, '\0', FIXATCOMMANDLENGTH);
    strcat(AtStringError, "ERROR");
	
    memset(AtStringWPAR,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringWPAR,"AT+WPAR=");
	
    memset(AtStringRPAR,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringRPAR,"AT+RPAR=");
	
    memset(AtStringSaveSetting,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringSaveSetting,"AT+WPAR=9999");
    memset(AtStringSaveSetting_V11,'\0',FIXATCOMMANDLENGTH);
	
    strcat(AtStringSaveSetting_V11,"AT+WPAR=9994");
	
    memset(AtStringSaveToDefaultSetting,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringSaveToDefaultSetting,"AT+WPAR=9998");
	
    memset(AtStringRecoverDefaultSetting,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringRecoverDefaultSetting,"AT+WPAR=9997");
	
    memset(AtStringCMIC,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringCMIC,"AT+CMIC");
	
    memset(AtStringSIDET,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringSIDET,"AT+SIDET");
	
    memset(AtStringECHO,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringECHO,"AT+ECHO");
	
    memset(AtStringCTRS,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringCTRS,"AT+CTRS");
	
    memset(AtStringAndF,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringAndF,"AT&F");
	
    memset(AtStringAndW,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringAndW,"AT&W");
    // added by Huang ye for activate commport
    memset(AtStringat,'\0',FIXATCOMMANDLENGTH);
    strcat(AtStringat,"AT+RPAR=2");
    // added by Huang ye for activate commport
	
    m_paraInfo.RemoveAll();
    m_paraValue.RemoveAll();
	
    m_nEncryptSolution = 0;

	m_nLockLacTotal    = 0;
	
    phonemodetype      = PhoneModeERROR;
	
}

CCommThread::~CCommThread()
{
}

BOOL CCommThread::InitInstance()
{
    // TODO:  perform and per-thread initialization here
#ifdef _DEBUG_AT
#endif //_DEBUG_AT
    return TRUE;
}

int CCommThread::ExitInstance()
{
    // TODO:  perform any per-thread cleanup here
    m_paraInfo.RemoveAll();
    m_paraValue.RemoveAll();
    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCommThread, CWinThread)
//{{AFX_MSG_MAP(CCommThread)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
ON_THREAD_MESSAGE(WM_SENDCOMM_NOTIFY, OnSendCommNotify)
ON_THREAD_MESSAGE(WM_CLEARSTATE_NOTIFY, OnClearStateNotify)
ON_THREAD_MESSAGE(WM_COMMMANAGE_NOTIFY, OnCommManageNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommThread message handlers

void CCommThread::OnSendCommNotify(WPARAM wParam, LPARAM lParam)
{
    extern     CMainContent MainContent;
    SendFIFO   *newdata  = (SendFIFO *)wParam;
    char* c=(char*)(newdata->senddata);
	
    switch (newdata->commandtype)
    {
        // added by Huang ye for activate commport
    case ACTIVATE_COMMPORT_STATE_CMD:
        commThreadState = ACTIVATE_COMMPORT;
        activateCommPortState= ACTIVATE_COMMPORT_IDLE_STATE;
        this->PostThreadMessage(
            WM_COMMMANAGE_NOTIFY,
            0,
            0);
        break;
        // added by Huang ye for activate commport
    case READ_TOTAL_SETTING_STATE_CMD:
        commThreadState = COMMTHREAD_READ_TOTAL_SETTING;
        readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
        this->PostThreadMessage(
            WM_COMMMANAGE_NOTIFY,
            0,
            0);
        break;
    case WRITE_TOTAL_SETTING_STATE_CMD:
        commThreadState = COMMTHREAD_WRITE_TOTAL_SETTING;
        writeTotalSettingState = WRITE_TOTAL_SETTING_IDLE_STATE;
        this->PostThreadMessage(
            WM_COMMMANAGE_NOTIFY,
            0,
            0);
        break;
    case SAVE_TO_DEFAULT_SETTING_STATE_CMD:
        commThreadState = COMMTHREAD_SAVE_TO_DEFAULT_SETTING;
        writeToDefaultSettingState = WRITE_TO_DEFAULT_SETTING_IDLE_STATE;
        this->PostThreadMessage(
            WM_COMMMANAGE_NOTIFY,
            0,
            0);
        break;
    case RECOVER_DEFAULT_SETTING_STATE_CMD:
        commThreadState = COMMTHREAD_RECOVER_DEFAULT_SETTING;
        recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_IDLE_STATE;
        this->PostThreadMessage(
            WM_COMMMANAGE_NOTIFY,
            0,
            0);
        break;
    case CHANGE_PASS_STATE_CMD:
        commThreadState = COMMTHREAD_CHANGE_PASS;
        changePassState = CHANGE_PASS_IDLE_STATE;
        this->PostThreadMessage(
            WM_COMMMANAGE_NOTIFY,
            0,
            0);
        break;
    default:
        MessageBox(NULL, _T("Comm Thread Recv exceptional Message!"), SYS_NAME, MB_OK|MB_ICONSTOP);
        break;
    }
	
	
}

#ifdef _DEBUG_AT
void CCommThread::WriteAtLog(CString string, BOOL bWrite)
{
    //	BOOL bOpened = fAtLogFile.Open(STRING_AT_LOG_NAME,
    //		CFile::typeText|CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	
    CString strFile;
    strFile = (MainContent.m_sWorkPath)+STRING_AT_LOG_NAME;
    if ( fAtLogFile = fopen( strFile, "a+t") )
    {
        if (!string.IsEmpty())
        {
            CString strTime, strDate, strStatus, strAll;
            strTime = COleDateTime::GetCurrentTime().Format(_T("%H:%M:%S "));
            strDate = COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y "));
            strStatus = bWrite? _T("Recv:") : _T("Send:");
			
            if (string.Right(1) == '\r')
                string = string.Left(string.GetLength()-1);
			
            strAll = strDate + strTime + strStatus + string + _T("\r\n");
			
            fwrite( strAll, sizeof( char ), strAll.GetLength(), fAtLogFile );
            // fAtLogFile.Write( strAll, strAll.GetLength());
        }
        // fAtLogFile.Close();
        fclose( fAtLogFile );
    }
}
#endif //_DEBUG_AT

void CCommThread::OnCommManageNotify(WPARAM wParam, LPARAM lParam)
{
    extern CSerialComm			MainSerial;
    extern CMainContent			MainContent;
	
    UINT   iStringLen = 0;
    static UINT nParaListSize = 0;
    static UINT totalNum = 0;
    static BOOL bIfContinue = true;
    static BOOL bIfNeedSaveAtCmd = false;
    ResetMoniterTimer();
    // added by Huang ye for activate commport
    int activatetimes = 0;
    BOOL  onetime = TRUE;
    // added by Huang ye for activate commport
	
	
#ifdef _DEBUG_AT
    if (wParam != NULL) //Recv AT com
    {
        WriteAtLog((char *)wParam, TRUE);
    }
#endif //_DEBUG_AT
	
    switch (commThreadState)
    {
    case COMMTHREAD_IDLE_STATE:
        if (GetErrorResult((char *)wParam) == TRUE)
        {
            free((char *)wParam);
        }
        else
        {
            if (wParam!=0)
            {
                if (_strnicmp((char *)wParam,"RDY",3)==0)
                {
                    //MainSerial.m_hMainWnd->PostMessage(WM_START_DETECT_NOTIFY,0,0);
                }
            }
            StopMoniterTimer();
            free((char *)wParam);
        }
        break;
		
        // added by Huang ye for activate commport
    case  ACTIVATE_COMMPORT:
        switch (activateCommPortState)
        {
        case ACTIVATE_COMMPORT_IDLE_STATE:
			
            activatetimes = 0;
			
            MainSerial.bActivated = FALSE;
			
            activateCommPortState = ACTIVATE_COMMPORT_START_STATE;
			
            while (activatetimes < MAX_ACTIVATE_COMMPORT_TIMES)
            {
				
                if (MainSerial.bActivated)
                {
                    onetime = FALSE;
                    activatetimes = MAX_ACTIVATE_COMMPORT_TIMES;
                }
				
                if (activatetimes < MAX_ACTIVATE_COMMPORT_TIMES)
                {
                    if (activatetimes == 0)
                    {
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, ACTIVATE_COMMPORT_BEING);
                        CreateAtivateCmd();
                    }
					
                    Sleep(150);
					
                    SendCreatedCommand();
                }
				
				
                if (MainSerial.bActivated)
                {
                    onetime = FALSE;
                    activatetimes = MAX_ACTIVATE_COMMPORT_TIMES;
                }
				
                activatetimes = activatetimes + 1;
            }
            //free((char *)wParam);
            break;
			
        case ACTIVATE_COMMPORT_START_STATE:
			
            MainContent.m_bDisplayOnceTime  = FALSE;
			
            StopMoniterTimer();
            ResetSendingTimer();
			
            MainSerial.bActivated = FALSE;
			
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    //ResetSendingTimer();
                }
                else
                {
                    commThreadState = ACTIVATE_COMMPORT;
                    activateCommPortState = ACTIVATE_COMMPORT_START_STATE;
                    MainSerial.bDeleteBufferATCommand = TRUE;
                }
				
            }
            else
            {
                //MessageBox(NULL, (char *)wParam, _T("ACTIVATE_COMM"), MB_OK);
            }
			
            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, ACTIVATE_COMMPORT_STATE);
			
            CommThreadOkExit(_T(""));
			
            free((char *)wParam);
            break;
			
            //	case ACTIVATE_COMMPORT_OK_STATE:
            //	break;
        default:
            free((char *)wParam);
            break;
			
        }
        break;
		
		
        // added by Huang ye for activate commport
		
		case COMMTHREAD_READ_TOTAL_SETTING:
			// added by Huang ye for activate commport
			//			     StopSendingTimer();
			// added by Huang ye for activate commport
			switch (readTotalSettingState)
			{
			case READ_TOTAL_SETTING_IDLE_STATE:
				
				StopSendingTimer();
				//read speed time para
				readTotalSettingState = READ_CMD_DIAL_WAITING_TIME_RW;
				
				CreateReadParaCommand(CMD_DIAL_WAITING_TIME_RW);
				SendCreatedCommand();
				NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_DIAL_WAITING_TIME_RW, CMD_DIAL_WAITING_TIME_RW);
				// added by Huang ye for activate commport
				free((char *)wParam);
				// added by Huang ye for activate commport
				break;

		   /***************************************************************************************************************/
		   /*********************************    3In1参数读过程处理 huangqr 2008-12-11 ************************************/
		   /***************************************************************************************************************/
			case READ_CMD_DIAL_WAITING_TIME_RW:			//读速延时时间
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState                          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState                    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读速延时时间错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_DIAL_WAITING_TIME_RW, CMD_DIAL_WAITING_TIME_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState                    = READ_CMD_HOW_TO_USE_CITY_NUMBER_RW;
						CreateReadParaCommand(CMD_HOW_TO_USE_CITY_NUMBER_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_HOW_TO_USE_CITY_NUMBER_RW, CMD_HOW_TO_USE_CITY_NUMBER_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5) == 0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_DIAL_WAITING_TIME_RW);
					}
				}
				free((char *)wParam);
				break;
		   /***************************************************************************************************************/
		   /*********************************    3In1参数读过程处理 huangqr 2008-12-11 ************************************/
		   /***************************************************************************************************************/
			case READ_CMD_HOW_TO_USE_CITY_NUMBER_RW:    //读本地区号使用方式
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState                    = COMMTHREAD_IDLE_STATE;
						readTotalSettingState              = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读本地区号使用方式错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_HOW_TO_USE_CITY_NUMBER_RW, CMD_HOW_TO_USE_CITY_NUMBER_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_CMD_SMS_CENTER_NUMBER_RW;
						CreateReadParaCommand(CMD_SMS_CENTER_NUMBER_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_CMD_SMS_CENTER_NUMBER_RW, CMD_SMS_CENTER_NUMBER_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_HOW_TO_USE_CITY_NUMBER_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_SMS_CENTER_NUMBER_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState                             = COMMTHREAD_IDLE_STATE;
						readTotalSettingState                       = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读短信中心错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_SMS_CENTER_NUMBER_RW, CMD_SMS_CENTER_NUMBER_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState                       = READ_CMD_BARING_OTHER_CITY_NUMBER_RW;
						CreateReadParaCommand(CMD_BARING_OTHER_CITY_NUMBER_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_BARING_OTHER_CITY_NUMBER_RW, CMD_BARING_OTHER_CITY_NUMBER_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SMS_CENTER_NUMBER_RW);
					}
				}
				free((char *)wParam);
				break;
				
				/*added 11.24 by cyh*/
			case READ_CMD_BARING_OTHER_CITY_NUMBER_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读是否禁拨国内长途错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_BARING_OTHER_CITY_NUMBER_RW,  CMD_BARING_OTHER_CITY_NUMBER_RW);
						
					}
					else  //READ NEXT PARA
					{
#if defined(XMSTART_NET_AND_COMMGR_ENABLE)
						readTotalSettingState = READ_SMS_RECEIVE_NUMBER_RW;
						CreateReadParaCommand(CMD_SMS_RECEIVE_NUMBER_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_SMS_RECEIVE_NUMBER_RW, CMD_SMS_RECEIVE_NUMBER_RW);
#else
						readTotalSettingState = READ_CMD_LOCAL_NUMBER_ACTIVE_RW;
						CreateReadParaCommand(CMD_LOCAL_NUMBER_ACTIVE_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_LOCAL_NUMBER_ACTIVE_RW, CMD_LOCAL_NUMBER_ACTIVE_RW);
#endif						
						SendCreatedCommand();
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_BARING_OTHER_CITY_NUMBER_RW);
					}
				}
				free((char *)wParam);
				break;
				
				
#if defined(XMSTART_NET_AND_COMMGR_ENABLE)
			case READ_SMS_RECEIVE_NUMBER_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读短信息接收中心地址错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_SMS_RECEIVE_NUMBER_RW,  CMD_SMS_RECEIVE_NUMBER_RW);
					}
					else
					{
						readTotalSettingState = READ_DISPLAY_ORIGINAL_NUMBER_RW;
						CreateReadParaCommand(CMD_DISPLAY_ORIGINAL_NUMBER_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_DISPLAY_ORIGINAL_NUMBER_RW, CMD_DISPLAY_ORIGINAL_NUMBER_RW);
						SendCreatedCommand();
					}
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SMS_RECEIVE_NUMBER_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_DISPLAY_ORIGINAL_NUMBER_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读是否显示原始号码错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_SMS_RECEIVE_NUMBER_RW,  CMD_DISPLAY_ORIGINAL_NUMBER_RW);
					}
					else
					{
#if defined(CELL_LEVEL) && defined(FOR_CLIENT)
						readTotalSettingState = READ_CELL_LEVEL_QUALITY_R;
						CreateReadParaCommand(CMD_CELL_LEVEL_QUALITY_R);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CELL_LEVEL_QUALITY_R, CMD_CELL_LEVEL_QUALITY_R);
#else
						readTotalSettingState = READ_CLIENT_ID_RW;
						CreateReadParaCommand(CMD_CLIENT_ID_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CLIENT_ID_RW, CMD_CLIENT_ID_RW);
#endif
						SendCreatedCommand();
					}
				} 
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_DISPLAY_ORIGINAL_NUMBER_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_CELL_LEVEL_QUALITY_R:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读小区信号强度质量信息错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CELL_LEVEL_QUALITY_R,  CMD_CELL_LEVEL_QUALITY_R);
					}
					else
					{
						readTotalSettingState = READ_CLIENT_ID_RW;
						CreateReadParaCommand(CMD_CLIENT_ID_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CLIENT_ID_RW, CMD_CLIENT_ID_RW);
						SendCreatedCommand();
					}
				} 
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_CELL_LEVEL_QUALITY_R);
					}
				}
				free((char *)wParam);
				break;

			case READ_CLIENT_ID_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读生产商ID信息错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CLIENT_ID_RW,  CMD_CELL_LEVEL_QUALITY_R);
					}
					else
					{
						readTotalSettingState = READ_CMD_LOCAL_NUMBER_ACTIVE_RW;
						CreateReadParaCommand(CMD_LOCAL_NUMBER_ACTIVE_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_LOCAL_NUMBER_ACTIVE_RW, CMD_LOCAL_NUMBER_ACTIVE_RW);
						SendCreatedCommand();
					}
				} 
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_CLIENT_ID_RW);
					}
				}
				free((char *)wParam);
				break;

#else
			case READ_CMD_LOCAL_NUMBER_ACTIVE_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读本地手机号段是否有效错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_LOCAL_NUMBER_ACTIVE_RW,  CMD_BARING_OTHER_CITY_NUMBER_RW);
					}
					else
					{
#if defined (ICCID_BUILD) && defined(FOR_CLIENT)
						readTotalSettingState = READ_IMEI_CCID_INFO_R;
						CreateReadParaCommand(CMD_IMEI_CCID_INFO_R);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IMEI_CCID_INFO_R, CMD_IMEI_CCID_INFO_R);
#else /* ICCID_BUILD */
#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
						readTotalSettingState = READ_ENCRYPT_SOLUTION;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_SOLUTION_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_ENCRYPT_SOLUTION, CMD_SIM_ENCRYPT_SOLUTION_RW);
#else /* SIM_ENCRYPT_COMPLETE_SOLUTIONS */
#if defined(XMSTART_3IN1_NET_MANAGER)
						readTotalSettingState = READ_CMD_SWITCH_INTELLIGENT_DAIL_RW;
						CreateReadParaCommand(CMD_SWITCH_INTELLIGENT_DAIL_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_INTELLIGENT_DAIL_RW, CMD_SWITCH_INTELLIGENT_DAIL_RW);
#else
						readTotalSettingState = READ_CMD_CITY_NUMBER_RW;
						CreateReadParaCommand(CMD_CITY_NUMBER_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CITY_NUMBER_RW, CMD_CITY_NUMBER_RW);
#endif/* XMSTART_3IN1_NET_MANAGER */
#endif /* SIM_ENCRYPT_COMPLETE_SOLUTIONS */
#endif/* ICCID_BUILD */
						SendCreatedCommand();
					}
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_LOCAL_NUMBER_ACTIVE_RW);
					}
				}
				free((char *)wParam);
				break;
#endif

			case READ_CMD_LOCAL_NUMBER_ACTIVE_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读本地手机号段是否有效错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_LOCAL_NUMBER_ACTIVE_RW,  CMD_BARING_OTHER_CITY_NUMBER_RW);
					}
					else
					{
#if defined (ICCID_BUILD) && defined(FOR_CLIENT)
						readTotalSettingState = READ_IMEI_CCID_INFO_R;
						CreateReadParaCommand(CMD_IMEI_CCID_INFO_R);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IMEI_CCID_INFO_R, CMD_IMEI_CCID_INFO_R);
#else /* ICCID_BUILD */
#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
						readTotalSettingState = READ_ENCRYPT_SOLUTION;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_SOLUTION_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_ENCRYPT_SOLUTION, CMD_SIM_ENCRYPT_SOLUTION_RW);
#else /* SIM_ENCRYPT_COMPLETE_SOLUTIONS */
#if defined(XMSTART_3IN1_NET_MANAGER)
						readTotalSettingState = READ_CMD_SWITCH_INTELLIGENT_DAIL_RW;
						CreateReadParaCommand(CMD_SWITCH_INTELLIGENT_DAIL_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_INTELLIGENT_DAIL_RW, CMD_SWITCH_INTELLIGENT_DAIL_RW);
#else
						readTotalSettingState = READ_CMD_CITY_NUMBER_RW;
						CreateReadParaCommand(CMD_CITY_NUMBER_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CITY_NUMBER_RW, CMD_CITY_NUMBER_RW);
#endif/* XMSTART_3IN1_NET_MANAGER */
#endif /* SIM_ENCRYPT_COMPLETE_SOLUTIONS */
#endif/* ICCID_BUILD */
						SendCreatedCommand();
					}
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_LOCAL_NUMBER_ACTIVE_RW);
					}
				}
				free((char *)wParam);
				break;

#if defined (ICCID_BUILD)
			case READ_IMEI_CCID_INFO_R:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState          = COMMTHREAD_IDLE_STATE;
						readTotalSettingState    = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读Imei和Ccid串号信息错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_IMEI_CCID_INFO_R,  CMD_IMEI_CCID_INFO_R);
					}
					else
					{
#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
						readTotalSettingState = READ_ENCRYPT_SOLUTION;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_SOLUTION_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_ENCRYPT_SOLUTION, CMD_SIM_ENCRYPT_SOLUTION_RW);
#else /* SIM_ENCRYPT_COMPLETE_SOLUTIONS */
#if defined(XMSTART_3IN1_NET_MANAGER)
						readTotalSettingState = READ_CMD_SWITCH_INTELLIGENT_DAIL_RW;
						CreateReadParaCommand(CMD_SWITCH_INTELLIGENT_DAIL_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_INTELLIGENT_DAIL_RW, CMD_SWITCH_INTELLIGENT_DAIL_RW);
#else
						readTotalSettingState = READ_CMD_CITY_NUMBER_RW;
						CreateReadParaCommand(CMD_CITY_NUMBER_RW);
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CITY_NUMBER_RW, CMD_CITY_NUMBER_RW);
#endif/* XMSTART_3IN1_NET_MANAGER */
#endif /* SIM_ENCRYPT_COMPLETE_SOLUTIONS */
						SendCreatedCommand();
					}
				} 
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_IMEI_CCID_INFO_R);
					}
				}
				free((char *)wParam);
				break;
#endif
				
#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)				
			case READ_ENCRYPT_SOLUTION:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读加密方案错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_ENCRYPT_SOLUTION, CMD_SIM_ENCRYPT_SOLUTION_RW);
						
					}
					else  //READ NEXT PARA
					{
						if (m_nEncryptSolution == 13)
						{
							readTotalSettingState = READ_COPATIBLE_COMMON_CARD;
							CreateReadParaCommand(CMD_SIM_ENCRYPT_COMPATIBLE_NORMAL_RW);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_COPATIBLE_COMMON_CARD, CMD_SIM_ENCRYPT_COMPATIBLE_NORMAL_RW);
							
						}
						else
						{
#if defined(XMSTART_3IN1_NET_MANAGER)
							readTotalSettingState = READ_CMD_SWITCH_INTELLIGENT_DAIL_RW;
							CreateReadParaCommand(CMD_SWITCH_INTELLIGENT_DAIL_RW);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_INTELLIGENT_DAIL_RW, CMD_SWITCH_INTELLIGENT_DAIL_RW);
#else
							readTotalSettingState = READ_CMD_CITY_NUMBER_RW;
							CreateReadParaCommand(CMD_CITY_NUMBER_RW);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CITY_NUMBER_RW, CMD_CITY_NUMBER_RW);
#endif/* XMSTART_3IN1_NET_MANAGER */
							
						}
						
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),CMD_SIM_ENCRYPT_SOLUTION_RW);
						
						m_nEncryptSolution = GetEncryptSolution((&((char *)wParam)[10]));
					}
				}
				free((char *)wParam);
				break;

			case READ_COPATIBLE_COMMON_CARD:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读加密卡是否兼容普通卡错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_COPATIBLE_COMMON_CARD);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_VENDOR_SERIAL_NO;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_FACTORYID_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_VENDOR_SERIAL_NO, CMD_SIM_ENCRYPT_FACTORYID_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SIM_ENCRYPT_COMPATIBLE_NORMAL_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_VENDOR_SERIAL_NO:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState        = COMMTHREAD_IDLE_STATE;
						readTotalSettingState  = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读加密卡厂商号错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_VENDOR_SERIAL_NO, CMD_SIM_ENCRYPT_FACTORYID_RW);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_KEY_SERIAL_NO;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_EDITION_ID_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_KEY_SERIAL_NO, CMD_SIM_ENCRYPT_EDITION_ID_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),CMD_SIM_ENCRYPT_FACTORYID_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_KEY_SERIAL_NO:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读密钥版本号错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_KEY_SERIAL_NO, CMD_SIM_ENCRYPT_EDITION_ID_RW);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState=READ_MAIN1_ENCRYPTED_KEY;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_KEY1_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_MAIN1_ENCRYPTED_KEY,CMD_SIM_ENCRYPT_KEY1_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SIM_ENCRYPT_EDITION_ID_RW);
					}
				}
				free((char *)wParam);
				break;
				
				
			case READ_MAIN1_ENCRYPTED_KEY:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读密钥1错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN1_ENCRYPTED_KEY, CMD_SIM_ENCRYPT_KEY1_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState=READ_MAIN2_ENCRYPTED_KEY;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_KEY2_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_MAIN2_ENCRYPTED_KEY,CMD_SIM_ENCRYPT_KEY2_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),CMD_SIM_ENCRYPT_KEY1_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_MAIN2_ENCRYPTED_KEY:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读密钥2错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN2_ENCRYPTED_KEY, CMD_SIM_ENCRYPT_KEY2_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState=READ_MAIN3_ENCRYPTED_KEY;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_KEY3_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_MAIN3_ENCRYPTED_KEY, CMD_SIM_ENCRYPT_KEY3_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SIM_ENCRYPT_KEY2_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_MAIN3_ENCRYPTED_KEY:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读密钥3错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN3_ENCRYPTED_KEY);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState=READ_MAIN4_ENCRYPTED_KEY;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_KEY4_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MAIN4_ENCRYPTED_KEY, CMD_SIM_ENCRYPT_KEY4_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SIM_ENCRYPT_KEY3_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_MAIN4_ENCRYPTED_KEY:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读密钥4错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN4_ENCRYPTED_KEY, CMD_SIM_ENCRYPT_KEY4_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState=READ_MAIN5_ENCRYPTED_KEY;
						CreateReadParaCommand(CMD_SIM_ENCRYPT_KEY5_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_MAIN5_ENCRYPTED_KEY, CMD_SIM_ENCRYPT_KEY5_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SIM_ENCRYPT_KEY4_RW);
					}
				}
				free((char *)wParam);
				break;
				
			case READ_MAIN5_ENCRYPTED_KEY:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读密钥5错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN5_ENCRYPTED_KEY, CMD_SIM_ENCRYPT_KEY5_RW);
						
					}
					else  //READ NEXT PARA
					{
#if defined(XMSTART_3IN1_NET_MANAGER)
						readTotalSettingState = READ_CMD_SWITCH_INTELLIGENT_DAIL_RW;
						CreateReadParaCommand(CMD_SWITCH_INTELLIGENT_DAIL_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_INTELLIGENT_DAIL_RW, CMD_SWITCH_INTELLIGENT_DAIL_RW);
#else
						readTotalSettingState = READ_CMD_CITY_NUMBER_RW;
						CreateReadParaCommand(CMD_CITY_NUMBER_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CITY_NUMBER_RW, CMD_CITY_NUMBER_RW);
#endif/*  XMSTART_3IN1_NET_MANAGER  */
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SIM_ENCRYPT_KEY5_RW);
					}
				}
				free((char *)wParam);
				break;
				
#endif /* SIM_ENCRYPT_COMPLETE_SOLUTIONS */


#if defined(XMSTART_3IN1_NET_MANAGER)
			case READ_CMD_SWITCH_INTELLIGENT_DAIL_RW:/* 智能拨号开关 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读智能拨号开关错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_SWITCH_INTELLIGENT_DAIL_RW, CMD_SWITCH_INTELLIGENT_DAIL_RW);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_CMD_SWITCH_AOTU_LOCK_RW;
						CreateReadParaCommand(CMD_SWITCH_AOTU_LOCK_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_AOTU_LOCK_RW, CMD_SWITCH_AOTU_LOCK_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SWITCH_INTELLIGENT_DAIL_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_SWITCH_AOTU_LOCK_RW:/* 自动锁屏开关 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读自动锁屏开关错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_SWITCH_AOTU_LOCK_RW, CMD_SWITCH_AOTU_LOCK_RW);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_CMD_SWITCH_TOLL_INDICTION_RW;
						CreateReadParaCommand(CMD_SWITCH_TOLL_INDICTION_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_TOLL_INDICTION_RW, CMD_SWITCH_TOLL_INDICTION_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SWITCH_AOTU_LOCK_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_SWITCH_TOLL_INDICTION_RW:/* 收费提示音开关 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读收费提示音开关错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_SWITCH_TOLL_INDICTION_RW, CMD_SWITCH_TOLL_INDICTION_RW);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_CMD_SWITCH_SPECIAL_APP_RW;
						CreateReadParaCommand(CMD_SWITCH_SPECIAL_APP_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SWITCH_SPECIAL_APP_RW, CMD_SWITCH_SPECIAL_APP_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SWITCH_TOLL_INDICTION_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_SWITCH_SPECIAL_APP_RW:/* 特殊应用开关 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读特殊应用开关错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_SWITCH_SPECIAL_APP_RW, CMD_SWITCH_SPECIAL_APP_RW);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_CMD_USER_TAG_RW;
						CreateReadParaCommand(CMD_USER_TAG_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_USER_TAG_RW, CMD_USER_TAG_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_SWITCH_SPECIAL_APP_RW);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_USER_TAG_RW:/* 用户标识信息 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读用户标识信息错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_USER_TAG_RW, CMD_USER_TAG_RW);

					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_CMD_CITY_NUMBER_RW;
						CreateReadParaCommand(CMD_CITY_NUMBER_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CITY_NUMBER_RW, CMD_CITY_NUMBER_RW);
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_USER_TAG_RW);
					}
				}
				free((char *)wParam);
				break;
#endif/*  XMSTART_3IN1_NET_MANAGER  */
				
				
			case READ_CMD_CITY_NUMBER_RW:			//读本地区号
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读本地区号错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_CITY_NUMBER_RW, CMD_CITY_NUMBER_RW);
						
					}
					else  //READ NEXT PARA
					{
						nParaListSize = 0;
						bIfContinue   = TRUE;
						readTotalSettingState = READ_HOT_NUMBER_TOTAL;
						CreateReadParaCommand(CMD_HOT_NUMBER_STR_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_HOT_NUMBER_TOTAL, CMD_HOT_NUMBER_STR_RW + nParaListSize);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),  CMD_CITY_NUMBER_RW);
					}
				}
				free((char *)wParam);
				break;
				
			case READ_HOT_NUMBER_TOTAL:       //读直拨号码
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读直拨号码错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_HOT_NUMBER_TOTAL, CMD_HOT_NUMBER_STR_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_HOT_NUMBER_TOTAL-1))
#else
						if ((nParaListSize < (NUM_HOT_NUMBER_TOTAL - 1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_HOT_NUMBER_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_HOT_NUMBER_TOTAL, CMD_HOT_NUMBER_STR_RW + nParaListSize);
						}
						else
						{
							readTotalSettingState = READ_BARRING_NUMBER_TOTAL;
							nParaListSize = 0;
							bIfContinue   = TRUE;
							CreateReadParaCommand(CMD_BARRING_NUMBER_STR_RW);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_BARRING_NUMBER_TOTAL, CMD_BARRING_NUMBER_STR_RW + nParaListSize);
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),   CMD_HOT_NUMBER_STR_RW + nParaListSize);
						bIfContinue = IsValidNumHeader((&((char *)wParam)[10]));
					}
				}
				free((char *)wParam);
				break;
				
				
			case READ_BARRING_NUMBER_TOTAL:          //读禁拨号码
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读禁拨号码错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_BARRING_NUMBER_TOTAL, CMD_BARRING_NUMBER_STR_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_BARRING_NUMBER_TOTAL-1))
#else
						if ((nParaListSize < (NUM_BARRING_NUMBER_TOTAL-1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_BARRING_NUMBER_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_BARRING_NUMBER_TOTAL, CMD_BARRING_NUMBER_STR_RW + nParaListSize);
						}
						else
						{
							readTotalSettingState = READ_CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW;
							nParaListSize = 0;
							bIfContinue   = TRUE;
							CreateReadParaCommand(CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW, CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW);
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_BARRING_NUMBER_STR_RW + nParaListSize);
						bIfContinue = IsValidNumHeader((&((char *)wParam)[10]));
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW: //读外地手机是否加零
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读外地手机是否加零错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW, CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_LOCALPHONE_HEADER_TOTAL;
						nParaListSize = 0;
						bIfContinue   = TRUE;
						CreateReadParaCommand(CMD_LOCALPHONE_HEADER_STR_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCALPHONE_HEADER_TOTAL, CMD_LOCALPHONE_HEADER_STR_RW + nParaListSize);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW);
					}
				}
				free((char *)wParam);
				break;
			case READ_LOCALPHONE_HEADER_TOTAL:        //读本地手机号码段
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读本地手机号码段错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_LOCALPHONE_HEADER_TOTAL, CMD_LOCALPHONE_HEADER_STR_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_LOCALPHONE_HEADER_TOTAL-1))
#else
						if ((nParaListSize <(NUM_LOCALPHONE_HEADER_TOTAL-1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_LOCALPHONE_HEADER_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCALPHONE_HEADER_TOTAL, CMD_LOCALPHONE_HEADER_STR_RW + nParaListSize);
						}
						else
						{
							readTotalSettingState = READ_CMD_LIMIT_ALL_INCOMING_CALL_RW;
							CreateReadParaCommand(CMD_LIMIT_ALL_INCOMING_CALL_RW);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_LIMIT_ALL_INCOMING_CALL_RW, CMD_LIMIT_ALL_INCOMING_CALL_RW);
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_LOCALPHONE_HEADER_STR_RW + nParaListSize);
						bIfContinue = IsValidNumHeader((&((char *)wParam)[10]));
					}
				}
				free((char *)wParam);
				break;
			case READ_CMD_LIMIT_ALL_INCOMING_CALL_RW:   	 //读是否限制呼入所有来电
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读是否限制呼入所有来电错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_LIMIT_ALL_INCOMING_CALL_RW, CMD_LIMIT_ALL_INCOMING_CALL_RW);
						
					}
					else  //READ NEXT PARA
					{
						nParaListSize = 0;
						bIfContinue   = TRUE;
						readTotalSettingState = READ_LIMITINCOMING_HEADER_TOTAL;
						CreateReadParaCommand(CMD_LIMITINCOMING_HEADER_STR_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LIMITINCOMING_HEADER_TOTAL, CMD_LIMITINCOMING_HEADER_STR_RW+nParaListSize);
						
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_LIMIT_ALL_INCOMING_CALL_RW);
					}
				}
				free((char *)wParam);
				break;
			case READ_LIMITINCOMING_HEADER_TOTAL:   //读限制呼入号码段
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读限制呼入号码段错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_LIMITINCOMING_HEADER_TOTAL, CMD_LIMITINCOMING_HEADER_STR_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
						if (nParaListSize < (NUM_LIMITINCOMING_HEADER_TOTAL-1))
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_LIMITINCOMING_HEADER_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_LIMITINCOMING_HEADER_TOTAL, CMD_LIMITINCOMING_HEADER_STR_RW + nParaListSize);
							
						}
						else
						{
						    nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_AUTO_LOCK_PIN_ENABLE_RW;
							CreateReadParaCommand(CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_AUTO_LOCK_PIN_ENABLE_RW, CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW+nParaListSize);
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						bIfContinue = GetParaAndWriteFile((&((char *)wParam)[10]),CMD_LIMITINCOMING_HEADER_STR_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;
			case READ_CMD_AUTO_LOCK_PIN_ENABLE_RW:/* Pin码设置 6001 ~~ 6005 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读Pin码设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_AUTO_LOCK_PIN_ENABLE_RW, CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
						if (nParaListSize < (CMD_PIN_SETTING_USE_ENCRYPT_PIN_RW - CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW))
						{/* 6001 ~~ 6005 */
							nParaListSize++;
							readTotalSettingState = READ_CMD_AUTO_LOCK_PIN_ENABLE_RW;
							CreateReadParaCommand(CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_CMD_AUTO_LOCK_PIN_ENABLE_RW, CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_SIM_BAND_TO_HANDSET_RW;
							CreateReadParaCommand(CMD_LOCK_SIM_BAND_TO_HANDSET_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_SIM_BAND_TO_HANDSET_RW, CMD_LOCK_SIM_BAND_TO_HANDSET_RW + nParaListSize);
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW+nParaListSize);
					}
				}
				free((char *)wParam);
				break;
				
			case READ_CMD_SIM_BAND_TO_HANDSET_RW: /* 锁卡锁运营商设置 7000~~7003 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读锁卡锁运营商设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_SIM_BAND_TO_HANDSET_RW, CMD_LOCK_SIM_BAND_TO_HANDSET_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
						if (nParaListSize < (CMD_LOCK_SIM_LOCKED_SERVICE_PROVIDER_RW - CMD_LOCK_SIM_BAND_TO_HANDSET_RW))
						{/* 7000 ~~ 7003 */
							nParaListSize++;
							readTotalSettingState = READ_CMD_SIM_BAND_TO_HANDSET_RW;
							CreateReadParaCommand(CMD_LOCK_SIM_BAND_TO_HANDSET_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_CMD_AUTO_LOCK_PIN_ENABLE_RW, CMD_LOCK_SIM_BAND_TO_HANDSET_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_CELL_LOCK_ENABLE_RW;
							CreateReadParaCommand(CMD_LOCK_CELL_LOCK_ENABLE_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CELL_LOCK_ENABLE_RW, CMD_LOCK_CELL_LOCK_ENABLE_RW + nParaListSize);
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_LOCK_SIM_BAND_TO_HANDSET_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_CELL_LOCK_ENABLE_RW:/* 锁小区设置 8000~~8003 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读锁小区设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_CELL_LOCK_ENABLE_RW, CMD_LOCK_CELL_LOCK_ENABLE_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
						if (nParaListSize < (CMD_LOCK_CELL_INFO_RW - CMD_LOCK_CELL_LOCK_ENABLE_RW))
						{/* 8000 ~~ 8003 */
							nParaListSize++;
							CreateReadParaCommand(CMD_LOCK_CELL_LOCK_ENABLE_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_CELL_LOCK_ENABLE_RW, CMD_LOCK_CELL_LOCK_ENABLE_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
#if defined(LOCK_LAC)
							readTotalSettingState = READ_CMD_LAC_LOCK_ENABLE_RW;
							CreateReadParaCommand(CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_LAC_LOCK_ENABLE_RW, CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW);
#else
                            readTotalSettingState = READ_NUM_IP_HEADER_TATOL;
							CreateReadParaCommand(CMD_IP_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_IP_HEADER_TATOL, CMD_IP_SETTING_STR_RW + nParaListSize);
#endif
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),  CMD_LOCK_CELL_LOCK_ENABLE_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;
				
#if defined(LOCK_LAC)	
			case READ_CMD_LAC_LOCK_ENABLE_RW:           //读是否锁LAC
				
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读是否锁LAC错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_LAC_LOCK_ENABLE_RW, CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW);
						
					}
					else  //READ NEXT PARA
					{
						readTotalSettingState = READ_CMD_LAC_LOCKED_CELL_NUM_RW;
						CreateReadParaCommand(CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_LAC_LOCKED_CELL_NUM_RW, CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW);
					}
				}
				free((char *)wParam);
				break;
				
			case READ_CMD_LAC_LOCKED_CELL_NUM_RW:       /* 读已经锁定的Lac数目  */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读已经锁定的Lac数目错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CMD_LAC_LOCKED_CELL_NUM_RW, CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW);
						
					}
					else  //READ NEXT PARA
					{
					    nParaListSize = 0;
						bIfContinue   = TRUE;
#if defined(FULL_LIST)
						if (TRUE)
#else
						if (m_nLockLacTotal > 0)
#endif
						{
						   m_nLockLacTotal = 0;
						   readTotalSettingState = READ_CMD_LAC_INFO_STR_RW;
						   CreateReadParaCommand(CMD_LOCK_CELL_LAC_INFO_STR_RW+nParaListSize);
						   SendCreatedCommand();
						   NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_LAC_INFO_STR_RW, CMD_LOCK_CELL_LAC_INFO_STR_RW+nParaListSize);
						}
						else
						{
						   readTotalSettingState = READ_NUM_IP_HEADER_TATOL;
						   CreateReadParaCommand(CMD_IP_SETTING_STR_RW + nParaListSize);
						   SendCreatedCommand();
						   NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_IP_HEADER_TATOL, CMD_IP_SETTING_STR_RW + nParaListSize);
						}
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]),CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW);
						m_nLockLacTotal = GetEncryptSolution((&((char *)wParam)[10]));
					}
				}
				free((char *)wParam);
				break;
				
			case READ_CMD_LAC_INFO_STR_RW:/* 读已经锁定的Lac大区号 */
				if (GetReturnResult((char *) wParam) == TRUE)
				{
					if (GetErrorResult((char *) wParam) == TRUE)
					{
						commThreadState = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读已经锁定的LAC号错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_LAC_INFO_STR_RW, CMD_LOCK_CELL_LAC_INFO_STR_RW+nParaListSize);
					}
					else
					{
						if (nParaListSize < (LAC_DOZEN_COUNT_AT_TOTAL-1))
						{/* 8006 ~~ 8030 */
							nParaListSize++;
							CreateReadParaCommand(CMD_LOCK_CELL_LAC_INFO_STR_RW+nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_LAC_INFO_STR_RW, CMD_LOCK_CELL_LAC_INFO_STR_RW+nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_NUM_IP_HEADER_TATOL;
							CreateReadParaCommand(CMD_IP_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_IP_HEADER_TATOL, CMD_IP_SETTING_STR_RW + nParaListSize);
						}
					}
				}
				else
				{
					if (_strnicmp((char*)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char*)wParam)[10]),CMD_LOCK_CELL_LAC_INFO_STR_RW+nParaListSize);
					}
				}
				free((char*)wParam);
				break;
#endif
			case READ_NUM_IP_HEADER_TATOL:                  //读IP设置
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读IP设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_NUM_IP_HEADER_TATOL, CMD_IP_SETTING_STR_RW + nParaListSize);
						
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize<(NUM_IP_HEADER_TATOL-1))
#else
                        if ((nParaListSize<(NUM_IP_HEADER_TATOL-1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_IP_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_IP_HEADER_TATOL, CMD_IP_SETTING_STR_RW + nParaListSize);
						}
						else
						{
						    nParaListSize = 0;
							bIfContinue   = TRUE;
#if defined(INTELLIGENT_SPEED_DIAL)
                            readTotalSettingState = READ_NUM_INT_SPEED_DIAL_TOTAL;
							CreateReadParaCommand(CMD_INTELLIGENT_SPEED_DIAL_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_INT_SPEED_DIAL_TOTAL, CMD_INTELLIGENT_SPEED_DIAL_STR_RW + nParaListSize);
#elif defined(SPECIAL_DIAL)
                            readTotalSettingState = READ_NUM_SPECIAL_DIAL_TOTAL;
							CreateReadParaCommand(CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_SPECIAL_DIAL_TOTAL, CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
#elif defined(ENABLE_SESSION_TOLL_FUNCTION)
                            readTotalSettingState = READ_CMD_MAJOR_RATE_SETTING_STR_RW;
							CreateReadParaCommand(CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_STR_RW, CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
#elif defined(ENABLE_MOBILE_HEAD)
							readTotalSettingState = READ_CMD_MOBILE_HEADER_RW;
							CreateReadParaCommand(CMD_MOBILE_HEAD_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MOBILE_HEADER_RW, CMD_MOBILE_HEAD_STR_RW + nParaListSize);
#else
							readTotalSettingState = READ_MIC_GAIN_LEVEL;
							CreateReadAtCmdCommand(AtStringCMIC);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MIC_GAIN_LEVEL);
#endif
						}
					}
				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[10]), CMD_IP_SETTING_STR_RW + nParaListSize);
						bIfContinue = !IsIpSettingItemEmpty((&((char *)wParam)[10]));
					}
				}
				free((char *)wParam);
				break;
				
#if defined(INTELLIGENT_SPEED_DIAL)
			case READ_NUM_INT_SPEED_DIAL_TOTAL:/* 智能速拨参数 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读智能速拨参数设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_NUM_INT_SPEED_DIAL_TOTAL, CMD_INTELLIGENT_SPEED_DIAL_STR_RW + nParaListSize);
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_INT_SPEED_DIAL_TOTAL-1))
#else
                        if ((nParaListSize < (NUM_INT_SPEED_DIAL_TOTAL-1)) && bIfContinue)
#endif
                        {
							nParaListSize++;
                            CreateReadParaCommand(CMD_INTELLIGENT_SPEED_DIAL_STR_RW + nParaListSize);
						    SendCreatedCommand();
						    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_INT_SPEED_DIAL_TOTAL, CMD_INTELLIGENT_SPEED_DIAL_STR_RW + nParaListSize);
                        }
                        else
						{
						    nParaListSize = 0;
							bIfContinue   = TRUE;
#if defined(SPECIAL_DIAL)
                            readTotalSettingState = READ_NUM_SPECIAL_DIAL_TOTAL;
							CreateReadParaCommand(CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_SPECIAL_DIAL_TOTAL, CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
#elif defined(ENABLE_SESSION_TOLL_FUNCTION)
                            readTotalSettingState = READ_CMD_MAJOR_RATE_SETTING_STR_RW;
							CreateReadParaCommand(CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_STR_RW, CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
#elif defined(ENABLE_MOBILE_HEAD)
							readTotalSettingState = READ_CMD_MOBILE_HEADER_RW;
							CreateReadParaCommand(CMD_MOBILE_HEAD_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MOBILE_HEADER_RW, CMD_MOBILE_HEAD_STR_RW + nParaListSize);
#else
							readTotalSettingState = READ_MIC_GAIN_LEVEL;
							CreateReadAtCmdCommand(AtStringCMIC);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MIC_GAIN_LEVEL);
#endif
						}
					}
				}
				else
				{
				    if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						bIfContinue = GetParaAndWriteFile((&((char *)wParam)[11]), CMD_INTELLIGENT_SPEED_DIAL_STR_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;
#endif

#if defined(SPECIAL_DIAL)
			case READ_NUM_SPECIAL_DIAL_TOTAL:/* 特殊拨号参数 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读特殊拨号参数设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_NUM_SPECIAL_DIAL_TOTAL, CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_SPECIAL_DIAL_TOTAL-1))
#else
                        if ((nParaListSize < (NUM_SPECIAL_DIAL_TOTAL-1)) && bIfContinue)
#endif
                        {
							nParaListSize++;
                            CreateReadParaCommand(CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_SPECIAL_DIAL_TOTAL, CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
                        }
                        else
						{
						    nParaListSize = 0;
							bIfContinue   = TRUE;
#if defined(SPECIAL_DIAL)
                            readTotalSettingState = READ_NUM_SPECIAL_DIAL_TOTAL;
							CreateReadParaCommand(CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_NUM_SPECIAL_DIAL_TOTAL, CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
#elif defined(ENABLE_SESSION_TOLL_FUNCTION)
                            readTotalSettingState = READ_CMD_MAJOR_RATE_SETTING_STR_RW;
							CreateReadParaCommand(CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_STR_RW, CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
#elif defined(ENABLE_MOBILE_HEAD)
							readTotalSettingState = READ_CMD_MOBILE_HEADER_RW;
							CreateReadParaCommand(CMD_MOBILE_HEAD_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MOBILE_HEADER_RW, CMD_MOBILE_HEAD_STR_RW + nParaListSize);
#else
							readTotalSettingState = READ_MIC_GAIN_LEVEL;
							CreateReadAtCmdCommand(AtStringCMIC);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MIC_GAIN_LEVEL);
#endif
					}
				}
				else
				{
				    if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						bIfContinue = GetParaAndWriteFile((&((char *)wParam)[11]), CMD_SPECIAL_DIAL_STR_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;
#endif

#if defined(ENABLE_SESSION_TOLL_FUNCTION)
			case READ_CMD_MAJOR_RATE_SETTING_STR_RW:/* 主费率表参数 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读主费率表参数设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_STR_RW, CMD_MAJOR_RATE_SETTING_STR_RW  + nParaListSize);
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_RATE_SETTING_TABLE_TOTAL-1))
#else
						if ((nParaListSize < (NUM_RATE_SETTING_TABLE_TOTAL-1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							readTotalSettingState = READ_CMD_MAJOR_RATE_SETTING_STR_RW;
							CreateReadParaCommand(CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_STR_RW, CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
						}
						else
						{/* 起跳时间(12026), 费率文件 Id(12027), 费率号段记录总数(12028), 费率表记录总数(12029), 费率文件帧标记(12030)*/
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW;
							CreateReadParaCommand(CMD_MAJOR_RATE_SETTING_START_TIME_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW, CMD_MAJOR_RATE_SETTING_START_TIME_RW + nParaListSize);
						}
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
						bIfContinue = !IsRateTableItemEmpty((&((char *)wParam)[11]));
					}
				}
				free((char *)wParam);
				break;

			/* 起跳时间(12026), 费率文件 Id(12027), 费率号段记录总数(12028), 费率表记录总数(12029), 费率文件帧标记(12030)*/
			case READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读主费率文件特征错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW, CMD_MAJOR_RATE_SETTING_START_TIME_RW  + nParaListSize);
					}
					else  //READ NEXT PARA
					{
						if (nParaListSize < (CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R - CMD_MAJOR_RATE_SETTING_START_TIME_RW))
						{
							nParaListSize++;
							readTotalSettingState = READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW;
							CreateReadParaCommand(CMD_MAJOR_RATE_SETTING_START_TIME_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW, CMD_MAJOR_RATE_SETTING_START_TIME_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_MAJOR_RATE_HEADER_TYPE_STR_RW;
							CreateReadParaCommand(CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_HEADER_TYPE_STR_RW, CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
						}
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MAJOR_RATE_SETTING_START_TIME_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_MAJOR_RATE_HEADER_TYPE_STR_RW:/* 主费率表设置,费率号段 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读主费率表设置费率号段错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MAJOR_RATE_HEADER_TYPE_STR_RW, CMD_MAJOR_RATE_HEADER_TYPE_STR_RW  + nParaListSize);
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_RATE_SETTING_HEADER_TYPE_TOTAL-1))
#else
						if ((nParaListSize < (NUM_RATE_SETTING_HEADER_TYPE_TOTAL-1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_HEADER_TYPE_STR_RW, CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R;
							CreateReadParaCommand(CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R, CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R);
						}
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
						bIfContinue = !IsRateRuleItemEmpty((&((char *)wParam)[11]));
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R:/* 主费率文件包的总包校验码 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读主费率文件包的总包校验码错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R, CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R);
					}
					else  //READ NEXT PARA
					{
						nParaListSize = 0;
						bIfContinue   = TRUE;
#if defined(VG_MINOR_RATE)
						readTotalSettingState = READ_CMD_MINOR_RATE_SETTING_STR_RW;
						CreateReadParaCommand(CMD_MAJOR_RATE_SETTING_STR_RW + nParaListSize);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MINOR_RATE_SETTING_STR_RW, CMD_MINOR_RATE_SETTING_STR_RW + nParaListSize);
#elif defined(ENABLE_MOBILE_HEAD)
						readTotalSettingState = READ_CMD_MOBILE_HEADER_RW;
						CreateReadParaCommand(CMD_MOBILE_HEAD_STR_RW + nParaListSize);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MOBILE_HEADER_RW, CMD_MOBILE_HEAD_STR_RW + nParaListSize);
#else
						readTotalSettingState = READ_MIC_GAIN_LEVEL;
						CreateReadAtCmdCommand(AtStringCMIC);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MIC_GAIN_LEVEL);
#endif
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R);
					}
				}
				free((char *)wParam);
				break;

#if defined(VG_MINOR_RATE)
			case READ_CMD_MINOR_RATE_SETTING_STR_RW:/* 次费率表参数 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读次费率表参数设置错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MINOR_RATE_SETTING_STR_RW, CMD_MINOR_RATE_SETTING_STR_RW  + nParaListSize);
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_RATE_SETTING_TABLE_TOTAL-1))
#else
						if ((nParaListSize < (NUM_RATE_SETTING_TABLE_TOTAL-1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_MINOR_RATE_SETTING_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MINOR_RATE_SETTING_STR_RW, CMD_MINOR_RATE_SETTING_STR_RW + nParaListSize);
						}
						else
						{/* 起跳时间(12026), 费率文件 Id(12027), 费率号段记录总数(12028), 费率表记录总数(12029), 费率文件帧标记(12030)*/
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_MINOR_RATE_SETTING_START_TIME_RW;
							CreateReadParaCommand(CMD_MINOR_RATE_SETTING_START_TIME_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MINOR_RATE_SETTING_START_TIME_RW, CMD_MINOR_RATE_SETTING_START_TIME_RW + nParaListSize);
						}
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MINOR_RATE_SETTING_STR_RW + nParaListSize);
						bIfContinue = !IsRateTableItemEmpty((&((char *)wParam)[11]));
					}
				}
				free((char *)wParam);
				break;

				/* 起跳时间(12026), 费率文件 Id(12027), 费率号段记录总数(12028), 费率表记录总数(12029), 费率文件帧标记(12030)*/
			case READ_CMD_MINOR_RATE_SETTING_START_TIME_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读次费率文件特征错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MINOR_RATE_SETTING_START_TIME_RW, READ_CMD_MINOR_RATE_SETTING_START_TIME_RW  + nParaListSize);
					}
					else  //READ NEXT PARA
					{
						if (nParaListSize < (CMD_MINOR_RATE_SETTING_FRAM_FLAG_R - CMD_MINOR_RATE_SETTING_START_TIME_RW))
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_MINOR_RATE_SETTING_START_TIME_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MINOR_RATE_SETTING_START_TIME_RW, CMD_MINOR_RATE_SETTING_START_TIME_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_MINOR_RATE_HEADER_TYPE_STR_RW;
							CreateReadParaCommand(CMD_MINOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MINOR_RATE_HEADER_TYPE_STR_RW, CMD_MINOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
						}
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MINOR_RATE_SETTING_START_TIME_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_MINOR_RATE_HEADER_TYPE_STR_RW:/* 次费率表设置,费率号段 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读次费率表设置费率号段错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MINOR_RATE_HEADER_TYPE_STR_RW, CMD_MINOR_RATE_HEADER_TYPE_STR_RW  + nParaListSize);
					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (NUM_RATE_SETTING_HEADER_TYPE_TOTAL-1))
#else
						if ((nParaListSize < (NUM_RATE_SETTING_HEADER_TYPE_TOTAL-1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_MINOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MINOR_RATE_HEADER_TYPE_STR_RW, CMD_MINOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R;
							CreateReadParaCommand(CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R, CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R);
						}
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MINOR_RATE_HEADER_TYPE_STR_RW + nParaListSize);
						bIfContinue = !IsRateRuleItemEmpty((&((char *)wParam)[11]));
					}
				}
				free((char *)wParam);
				break;

			case READ_CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R:/* 次费率文件包的总包校验码 */
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState       = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读次费率文件包的总包校验码错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R, CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R);
					}
					else  //READ NEXT PARA
					{
						nParaListSize = 0;
						bIfContinue   = TRUE;
#if defined(ENABLE_MOBILE_HEAD)
						readTotalSettingState = READ_CMD_MOBILE_HEADER_RW;
						CreateReadParaCommand(CMD_MOBILE_HEAD_STR_RW + nParaListSize);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MOBILE_HEADER_RW, CMD_MOBILE_HEAD_STR_RW + nParaListSize);
#else
						readTotalSettingState = READ_MIC_GAIN_LEVEL;
						CreateReadAtCmdCommand(AtStringCMIC);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MIC_GAIN_LEVEL);
#endif
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						GetParaAndWriteFile((&((char *)wParam)[11]), CMD_MINOR_RATE_SETTING_START_TIME_RW);
					}
				}
				free((char *)wParam);
				break;
#endif/* VG_MINOR_RATE */
#endif

#if defined(ENABLE_MOBILE_HEAD)
			case READ_CMD_MOBILE_HEADER_RW:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读移动电话头错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_CMD_MOBILE_HEADER_RW, CMD_MOBILE_HEAD_STR_RW + nParaListSize);

					}
					else  //READ NEXT PARA
					{
#if defined(FULL_LIST)
						if (nParaListSize < (MOBILE_HEAD_TOTAL-1))
#else
						if ((nParaListSize < (MOBILE_HEAD_TOTAL - 1)) && bIfContinue)
#endif
						{
							nParaListSize++;
							CreateReadParaCommand(CMD_MOBILE_HEAD_STR_RW + nParaListSize);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CMD_MOBILE_HEADER_RW, CMD_MOBILE_HEAD_STR_RW + nParaListSize);
						}
						else
						{
							nParaListSize = 0;
							bIfContinue   = TRUE;
							readTotalSettingState = READ_MIC_GAIN_LEVEL;
							CreateReadAtCmdCommand(AtStringCMIC);
							SendCreatedCommand();
							NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MIC_GAIN_LEVEL);
						}
					}

				}
				else
				{
					if (_strnicmp((char *)wParam,"+RPAR",5)==0)
					{
						bIfContinue = GetParaAndWriteFile((&((char *)wParam)[11]),   CMD_MOBILE_HEAD_STR_RW + nParaListSize);
					}
				}
				free((char *)wParam);
				break;
#endif

				
			case READ_MIC_GAIN_LEVEL:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读Mic Gain Level错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MIC_GAIN_LEVEL);
					}
					else  //READ NEXT PARA
					{
						nParaListSize = 0;
						readTotalSettingState = READ_CHANGE_SIDE_TONE;
						CreateReadAtCmdCommand(AtStringSIDET);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,
							READ_CHANGE_SIDE_TONE);
						
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+CMIC",5)==0)
					{
						GetAtCmdAndWriteFile(AtStringCMIC,(&((char *)wParam)[7]));
					}
				}
				free((char *)wParam);
				break;
			case READ_CHANGE_SIDE_TONE:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读Change Side Tone Gain Level错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_CHANGE_SIDE_TONE);
					}
					else  //READ NEXT PARA
					{
						nParaListSize=0;
						readTotalSettingState = READ_ECHO_CANCEL_CONTROL;
						CreateReadAtCmdCommand(AtStringECHO);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,
							READ_ECHO_CANCEL_CONTROL);
					}
					
				}
				else
				{
#if defined(R16_ATC)
					//AT+SIDET?
					//+SIDET(NORMAL_AUDIO): 4096+SIDET(AUX_AUDIO): 0
					//OK
					if ((NULL != strstr((char *)wParam,"+SIDET(N")) && (NULL != strstr((const char*)wParam, "+SIDET(A")))
					{
						GetAtCmdSidetAndWriteFile(AtStringSIDET,(&((char *)wParam)[0]));
					}
					else
					{
						//AT+SIDET?
						//+SIDET(NORMAL_AUDIO): 0
						//+SIDET(AUX_AUDIO): 0
						//
						//OK
						if (_strnicmp((char *)wParam,"+SIDET(NORMAL_AUDIO)",  strlen("+SIDET(NORMAL_AUDIO)"))==0)
						{
							GetAtCmdSidetTwoLineAndWriteFile(AtStringSIDET, (&((char *)wParam)[strlen("+SIDET(NORMAL_AUDIO)")+2]),"0");
						}
						else
						{
							GetAtCmdSidetTwoLineAndWriteFile(AtStringSIDET, (&((char *)wParam)[strlen("+SIDET(AUX_AUDIO)")+2]),"1");
						}
					}
					
#else
					if (_strnicmp((char *)wParam,"+SIDET",6)==0)
					{
						GetAtCmdAndWriteFile(AtStringSIDET,(&((char *)wParam)[8]));
					}
#endif
				}
				free((char *)wParam);
				break;
				
			case READ_ECHO_CANCEL_CONTROL:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读Echo cancellation control 错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_ECHO_CANCEL_CONTROL);
					}
					else  //READ NEXT PARA
					{
						nParaListSize = 0;
						readTotalSettingState = READ_LCD_CONTRAST;
						CreateReadAtCmdCommand(AtStringCTRS);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LCD_CONTRAST);
					}
				}
				else
				{
					if (_strnicmp((char *)wParam,"+ECHO(N",7)==0)
					{
						GetAtCmdEchoAndWriteFile(AtStringECHO,(&((char *)wParam)[21]),"0");
					}
					else if (_strnicmp((char *)wParam,"+ECHO(A",7)==0)
					{
						GetAtCmdEchoAndWriteFile(AtStringECHO,(&((char *)wParam)[18]),"1");
					}
				}
				free((char *)wParam);
				break;
				
			case READ_LCD_CONTRAST:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadErrorExit("读Lcd contrast 错误");
						NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_ECHO_CANCEL_CONTROL);
					}
					else  //READ NEXT PARA
					{
						nParaListSize = 0;
						commThreadState  = COMMTHREAD_IDLE_STATE;
						readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
						CommThreadOkExit("读所有参数正确返回!");
						NotifyMainWnd(WM_READ_PARA_OK_NOTIFY,READ_LCD_CONTRAST);
					}
					
				}
				else
				{
					if (_strnicmp((char *)wParam,"+CTRS",5)==0)
					{
						GetAtCmdAndWriteFile(AtStringCTRS,(&((char *)wParam)[6]));
					}
				}
				free((char *)wParam);
				
				// added by Huang ye for activate commport
				onetime = FALSE;
				// added by Huang ye for activate commport
				break;
			default:
				free((char *)wParam);
				break;
        }
        break;// end case COMMTHREAD_READ_TOTAL_SETTING
    case COMMTHREAD_WRITE_TOTAL_SETTING:
        switch (writeTotalSettingState)
        {
        case WRITE_TOTAL_SETTING_IDLE_STATE:
            // added by Huang ye for activate commport
			
            StopSendingTimer();
            commThreadState  = COMMTHREAD_WRITE_TOTAL_SETTING;
            // added by Huang ye for activate commport
            writeTotalSettingState = WRITE_TOTAL_SETTING_BEGIN_WRITE_STATE;
            CreateBeginWriteCommand(MainContent.m_sOldPass);
            SendCreatedCommand();
            break;
        case WRITE_TOTAL_SETTING_BEGIN_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
					
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    writeTotalSettingState = WRITE_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("密码错误!");
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY,
						WRITE_TOTAL_SETTING_BEGIN_WRITE_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
                    totalNum = GetParaFileInfoAndValue();
                    writeTotalSettingState = WRITE_TOTAL_SETTING_WAIT_STATE;
                    CreateWriteParaCommand(nParaListSize);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_WAIT_STATE,++nParaListSize);
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        case WRITE_TOTAL_SETTING_WAIT_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    writeTotalSettingState=WRITE_TOTAL_SETTING_SAVE_STATE;
                    WriteCommand(AtStringSaveSetting);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_SAVE_STATE);
					
					
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY, WRITE_TOTAL_SETTING_WAIT_STATE,nParaListSize);
                    nParaListSize = 0;
                }
                else  //WRITE NEXT PARA
                {
                    if (nParaListSize<totalNum)
                    {
						
                        CreateWriteParaCommand(nParaListSize);						
                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_WAIT_STATE, ++nParaListSize);
                    }
                    else
                    {
                        nParaListSize = 0;
                        totalNum = GetParaFileAtCmdInfo();
                        if (totalNum> 0)
                        {
                            writeTotalSettingState = WRITE_ATCMD_SETTING_WAIT_STATE;
                            CreateWriteAtCmdCommand(nParaListSize);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_ATCMD_SETTING_WAIT_STATE, ++nParaListSize);
                            bIfNeedSaveAtCmd = true;
                        }
                        else
                        {
                            nParaListSize = 0;
                            writeTotalSettingState=WRITE_TOTAL_SETTING_SAVE_STATE;
                            WriteCommand(AtStringSaveSetting);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);														
                            bIfNeedSaveAtCmd = false;
                        }
                    }
                }
            }
            else  //UNKONW DATA
            {
				
            }
            free((char *)wParam);
            break;
        case WRITE_ATCMD_SETTING_WAIT_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    writeTotalSettingState=WRITE_TOTAL_SETTING_SAVE_STATE;
                    WriteCommand(AtStringSaveSetting);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_SAVE_STATE);
					
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY,WRITE_ATCMD_SETTING_WAIT_STATE,nParaListSize);
                    nParaListSize = 0;
                }
                else  //WRITE NEXT PARA
                {
                    if (nParaListSize < totalNum)
                    {
						
                        CreateWriteAtCmdCommand(nParaListSize);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_ATCMD_SETTING_WAIT_STATE, ++ nParaListSize);
                    }
                    else
                    {
                        nParaListSize = 0;
                        writeTotalSettingState=WRITE_TOTAL_SETTING_SAVE_STATE;
                        WriteCommand(AtStringSaveSetting);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);
                    }
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char *)wParam);
            break;
			
        case WRITE_TOTAL_SETTING_SAVE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
					
                    writeTotalSettingState = WRITE_TOTAL_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
					

                    writeTotalSettingState = WRITE_TOTAL_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char *)wParam);
            break;
			
			
        case WRITE_TOTAL_SETTING_END_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    writeTotalSettingState=WRITE_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("结束写操作时错误!");
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY,
						WRITE_TOTAL_SETTING_END_WRITE_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    if (bIfNeedSaveAtCmd)
                    {
                        writeTotalSettingState=WRITE_TOTAL_ATCMD_END_WRITE_STATE;
                        WriteCommand(AtStringAndW);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY, WRITE_TOTAL_SETTING_END_WRITE_STATE);
                        //在最后完成了结束写操作的时候,应该发一条AT&W命令,实际上却是发了两条,现更正!
                        //SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_TOTAL_ATCMD_END_WRITE_STATE);
                    }
                    else
                    {
                        nParaListSize = 0;
                        writeTotalSettingState=WRITE_TOTAL_SETTING_IDLE_STATE;
                        CommThreadOkExit("正确结束写操作时!");
                        NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY, WRITE_TOTAL_SETTING_END_WRITE_STATE);
                    }
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        case WRITE_TOTAL_ATCMD_END_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    writeTotalSettingState=WRITE_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("结束写AT操作时错误!");
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY, WRITE_TOTAL_ATCMD_END_WRITE_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
                    writeTotalSettingState = WRITE_TOTAL_SETTING_IDLE_STATE;
                    CommThreadOkExit("正确结束写AT操作时!");
                    NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY, WRITE_TOTAL_ATCMD_END_WRITE_STATE);
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        }
        break; // end case COMMTHREAD_WRITE_TOTAL_SETTING
		
    case COMMTHREAD_SAVE_TO_DEFAULT_SETTING:
        switch (writeToDefaultSettingState)
        {
        case WRITE_TO_DEFAULT_SETTING_IDLE_STATE:
            writeToDefaultSettingState = WRITE_TO_DEFAULT_SETTING_BEGIN_WRITE_STATE;
            CreateBeginWriteCommand(MainContent.m_sOldPass);
            SendCreatedCommand();
            break;
        case WRITE_TO_DEFAULT_SETTING_BEGIN_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
					
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    writeToDefaultSettingState = WRITE_TO_DEFAULT_SETTING_IDLE_STATE;
                    CommThreadErrorExit("用户密码错误!");
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_ERR_NOTIFY,
						WRITE_TO_DEFAULT_SETTING_BEGIN_WRITE_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    writeToDefaultSettingState = WRITE_TO_DEFAULT_SETTING_END_STATE;
                    WriteCommand(AtStringSaveToDefaultSetting);
                    SendCreatedCommand();
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        case WRITE_TO_DEFAULT_SETTING_END_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    writeToDefaultSettingState = WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_ERR_NOTIFY, WRITE_TO_DEFAULT_SETTING_END_STATE);
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
                    writeToDefaultSettingState = WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_OK_NOTIFY, WRITE_TO_DEFAULT_SETTING_END_STATE);
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char *)wParam);
            break;
        case WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    writeToDefaultSettingState = WRITE_TO_FACTORY_END_STATE;
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_ERR_NOTIFY, WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE);
                    WriteCommand("AT");
                    SendCreatedCommand();
					
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
                    writeToDefaultSettingState = WRITE_TO_FACTORY_END_STATE;
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_OK_NOTIFY, WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE);
                    WriteCommand("AT");
                    SendCreatedCommand();
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        case WRITE_TO_FACTORY_END_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
					
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    writeToDefaultSettingState = WRITE_TO_DEFAULT_SETTING_IDLE_STATE;
                    CommThreadErrorExit("保存AT操作为默认时错误!");
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_OK_NOTIFY, WRITE_TO_FACTORY_END_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
                    writeToDefaultSettingState=WRITE_TO_DEFAULT_SETTING_IDLE_STATE;
                    CommThreadOkExit("正确保存AT操作为默认!");
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_OK_NOTIFY, WRITE_TO_FACTORY_END_STATE);
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        }
        break; // end case COMMTHREAD_SAVE_TO_DEFAULT_SETTING
    case COMMTHREAD_RECOVER_DEFAULT_SETTING:
        switch (recoverDefaultSettingState)
        {
        case RECOVER_DEFAULT_SETTING_IDLE_STATE:
            recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_BEGIN_WRITE_STATE;
            CreateBeginWriteCommand(MainContent.m_sOldPass);
            SendCreatedCommand();
            break;
        case RECOVER_DEFAULT_SETTING_BEGIN_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
					
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_IDLE_STATE;
                    CommThreadErrorExit("用户密码错误!");
                    NotifyMainWnd(WM_RECOVER_DEFAULT_SETTING_ERR_NOTIFY, RECOVER_DEFAULT_SETTING_BEGIN_WRITE_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_END_STATE;
                    WriteCommand(AtStringRecoverDefaultSetting);
                    SendCreatedCommand();
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        case RECOVER_DEFAULT_SETTING_END_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
					
                    recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_RECOVER_DEFAULT_SETTING_ERR_NOTIFY, RECOVER_DEFAULT_SETTING_END_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
                    recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_RECOVER_DEFAULT_SETTING_OK_NOTIFY, RECOVER_DEFAULT_SETTING_END_STATE);
                }
				
            }
            else  //UNKONW DATA
            {
				
            }
            free((char*)wParam);
            break;
        case RECOVER_DEFAULT_SETTING_END_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
					
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_IDLE_STATE;
                    CommThreadErrorExit("结束写操作时错误!");
                    NotifyMainWnd(WM_RECOVER_DEFAULT_SETTING_ERR_NOTIFY, RECOVER_DEFAULT_SETTING_END_WRITE_STATE);
					
                }
                else  //WRITE NEXT PARA
                {
                    nParaListSize = 0;
                    recoverDefaultSettingState = RECOVER_DEFAULT_SETTING_IDLE_STATE;
                    CommThreadOkExit("正确结束写操作时!");
                    NotifyMainWnd(WM_RECOVER_DEFAULT_SETTING_OK_NOTIFY, RECOVER_DEFAULT_SETTING_END_WRITE_STATE);
                }
				
            }
            else  //UNKONW DATA
            {
            }
            free((char*)wParam);
            break;
        }
        break; // end case COMMTHREAD_RECOVER_DEFAULT_SETTING
		case COMMTHREAD_CHANGE_PASS:
			switch (changePassState)
			{
			case CHANGE_PASS_IDLE_STATE:
				changePassState = CHANGE_PASS_BEGIN_WRITE_STATE;
				CreateBeginWriteCommand(MainContent.m_sOldPass);
				SendCreatedCommand();
				break;
			case CHANGE_PASS_BEGIN_WRITE_STATE:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						
						commThreadState  = COMMTHREAD_IDLE_STATE;
						changePassState = CHANGE_PASS_IDLE_STATE;
						CommThreadErrorExit("密码错误!");
						NotifyMainWnd(WM_CHANGE_PASS_ERR_NOTIFY, CHANGE_PASS_BEGIN_WRITE_STATE);
						
					}
					else  //WRITE NEXT PARA
					{
						changePassState = CHANGE_PASS_END_STATE;
						CreateChangePassCommand(MainContent.m_sOldPass,MainContent.m_sNewPass1);
						SendCreatedCommand();
					}
					
				}
				else  //UNKONW DATA
				{
					
				}
				free((char*)wParam);
				break;
			case CHANGE_PASS_END_STATE:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						
						changePassState = CHANGE_PASS_END_WRITE_STATE;
						CreateEndWriteCommand();
						SendCreatedCommand();
						NotifyMainWnd(WM_CHANGE_PASS_ERR_NOTIFY, CHANGE_PASS_END_STATE);
						
					}
					else  //WRITE NEXT PARA
					{
						changePassState = CHANGE_PASS_END_WRITE_STATE;
						CreateEndWriteCommand();
						SendCreatedCommand();
						NotifyMainWnd(WM_CHANGE_PASS_OK_NOTIFY, CHANGE_PASS_END_STATE);
					}
					
				}
				else  //UNKONW DATA
				{
					
				}
				free((char*)wParam);
				break;
			case CHANGE_PASS_END_WRITE_STATE:
				if (GetReturnResult((char *)wParam) == TRUE)
				{
					if (GetErrorResult((char *)wParam) == TRUE)
					{
						
						commThreadState  = COMMTHREAD_IDLE_STATE;
						changePassState = CHANGE_PASS_IDLE_STATE;
						CommThreadErrorExit("结束写操作时错误!");
						NotifyMainWnd(WM_CHANGE_PASS_ERR_NOTIFY, CHANGE_PASS_END_WRITE_STATE);
						
					}
					else  //WRITE NEXT PARA
					{
						nParaListSize = 0;
						changePassState = CHANGE_PASS_IDLE_STATE;
						CommThreadOkExit("正确结束写操作时!");
						NotifyMainWnd(WM_CHANGE_PASS_OK_NOTIFY, CHANGE_PASS_END_WRITE_STATE);
					}
					
				}
				else  //UNKONW DATA
				{
					
				}
				free((char*)wParam);
				break;
			}
			break; // end case COMMTHREAD_CHANGE_PASS
			default:
				break;
    }
}

void CCommThread::WriteCommand(char *command)
{
    memset(CommThreadData.commanddata,
		'\0', MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata, command);
    CommThreadData.commandlength = (int)strlen(command);
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
}
void CCommThread::CreateReadAtCmdCommand(CString sAtCmd)
{
    memset(CommThreadData.commanddata,
		'\0', MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata, sAtCmd);
    CommThreadData.commandlength = (int)strlen(sAtCmd);
    strcat(CommThreadData.commanddata,"?");
    CommThreadData.commandlength += (int)strlen("?");
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
}

void CCommThread::CreateReadParaCommand(int iCMD)
{
    memset(CommThreadData.commanddata,'\0',MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata,AtStringRPAR);
    CommThreadData.commandlength = (int)strlen(AtStringRPAR);
    CString s;
    s.Empty();
    s.Format("%d",iCMD);
    strcat(CommThreadData.commanddata,s);
    CommThreadData.commandlength += (int)strlen(s);
	
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
}

void CCommThread::CreateAtivateCmd(void)
{
    memset(CommThreadData.commanddata,'\0',MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata, _T("AT+RPAR=6000\r"));
    CommThreadData.commandlength = (int)strlen(_T("AT+RPAR=6000\r"));
}


void CCommThread::CreateEndWriteCommand()
{
    memset(CommThreadData.commanddata,'\0',MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata, AtStringWPAR);
    CommThreadData.commandlength = (int)strlen(AtStringWPAR);
    CString s = "";
    s.Format("%d", CMD_SAVE_SETTING_STOP_W);
    strcat(CommThreadData.commanddata,s);
    CommThreadData.commandlength += (int)strlen(s);
	
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
}

void CCommThread::CreateBeginWriteCommand(CString sPass)
{
    memset(CommThreadData.commanddata,'\0',MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata,AtStringWPAR);
    CommThreadData.commandlength = (int)strlen(AtStringWPAR);
    CString s = "";
    s.Format("%d",CMD_SAVE_SETTING_START_W);
    strcat(CommThreadData.commanddata,s);
    CommThreadData.commandlength += (int)strlen(s);
	
    strcat(CommThreadData.commanddata,",");
    CommThreadData.commandlength += 1;
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata,sPass);
    CommThreadData.commandlength += (int)strlen(sPass);
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
}

void CCommThread::CreateChangePassCommand(CString sOldPass,CString sNewPass)
{
    memset(CommThreadData.commanddata,'\0',MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata,AtStringWPAR);
    CommThreadData.commandlength = (int)strlen(AtStringWPAR);
    CString s = "";
    s.Format("%d", CMD_SUPER_PASSWORD_W);
    strcat(CommThreadData.commanddata,s);
    CommThreadData.commandlength += (int)strlen(s);
	
    strcat(CommThreadData.commanddata,",");
    CommThreadData.commandlength += 1;
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata,sOldPass);
    CommThreadData.commandlength += (int)strlen(sOldPass);
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata,",");
    CommThreadData.commandlength += 1;
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata,sNewPass);
    CommThreadData.commandlength += (int)strlen(sNewPass);
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata,",");
    CommThreadData.commandlength += 1;
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata,sNewPass);
    CommThreadData.commandlength += (int)strlen(sNewPass);
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
}

void CCommThread::CreateWriteAtCmdCommand(UINT iIndex)
{
    memset(CommThreadData.commanddata,'\0',MAXSENDCHARNUM);
    if ((((CString)(m_paraInfo[iIndex])).Find(AtStringCMIC) == 0)||
		(((CString)(m_paraInfo[iIndex])).Find(AtStringECHO) == 0)
#if defined(R16_ATC)
		|| (((CString)(m_paraInfo[iIndex])).Find(AtStringSIDET) == 0)
#endif
		)
    {
		
        strcat(CommThreadData.commanddata,m_paraInfo[iIndex]);
        CommThreadData.commandlength = (int)strlen(m_paraInfo[iIndex])-1;
        CommThreadData.commanddata[CommThreadData.commandlength] = '\0';
		
    }
    else
    {
        strcat(CommThreadData.commanddata,m_paraInfo[iIndex]);
        CommThreadData.commandlength = (int)strlen(m_paraInfo[iIndex]);
    }
    strcat(CommThreadData.commanddata,"=");
    CommThreadData.commandlength += 1;
	
    strcat(CommThreadData.commanddata,m_paraValue[iIndex]);
    CommThreadData.commandlength += (int)strlen(m_paraValue[iIndex]);
	
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
	
}

void CCommThread::CreateWriteParaCommand(UINT iIndex)
{
    memset(CommThreadData.commanddata,'\0',MAXSENDCHARNUM);
    strcat(CommThreadData.commanddata,AtStringWPAR);
    CommThreadData.commandlength = (int)strlen(AtStringWPAR);
    strcat(CommThreadData.commanddata,m_paraInfo[iIndex]);
    CommThreadData.commandlength += (int)strlen(m_paraInfo[iIndex]);
    strcat(CommThreadData.commanddata,",");
    CommThreadData.commandlength += 1;
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
    strcat(CommThreadData.commanddata,m_paraValue[iIndex]);
    CommThreadData.commandlength += (int)strlen(m_paraValue[iIndex]);
    strcat(CommThreadData.commanddata,"\"");
    CommThreadData.commandlength += 1;
    strcat(CommThreadData.commanddata, AtStringCR);
    CommThreadData.commandlength += (int)strlen(AtStringCR);
}

void CCommThread::OnClearStateNotify(WPARAM wParam, LPARAM lParam)
{
    commThreadState             = COMMTHREAD_IDLE_STATE;
    readTotalSettingState       = READ_TOTAL_SETTING_IDLE_STATE;
    writeTotalSettingState      = WRITE_TOTAL_SETTING_IDLE_STATE;
    changePassState             = CHANGE_PASS_IDLE_STATE;
    recoverDefaultSettingState  = RECOVER_DEFAULT_SETTING_IDLE_STATE;
    writeToDefaultSettingState  = WRITE_TO_DEFAULT_SETTING_IDLE_STATE;
}

// 判断收到的字符是否是ERROR的错误结果返回字符
BOOL CCommThread::GetErrorResult(char *returndata)
{
    BOOL result;
	
    if (_strnicmp(returndata, AtStringError,
		sizeof(AtStringError)) == 0)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
	
    return result;
}

// 当通讯线程在进行正常AT命令通讯时收到ERROR错误返回后的处理
void CCommThread::CommThreadErrorExit(char *errorinfo)
{
    extern CMainContent MainContent;
    TRACE0(errorinfo);
    // 状态复位，通知FIFO线程进行出错处理
    commThreadState = COMMTHREAD_IDLE_STATE;
    SetEvent(MainContent.m_hPostSendingEvent);
    MainContent.m_pFIFOThread->PostThreadMessage(
        WM_SENDFINISH_NOTIFY,
        FIFOFINISH,
        0);
}

// 当通讯线程在进行正常AT命令通讯时收到ERROR警告返回后的处理
void CCommThread::CommThreadWarnExit(char *warninfo)
{
    extern CMainContent MainContent;
	
    // 显示出错信息
    //	MainContent.SendEventRecord(warninfo, WARN);
	
    // 状态复位，通知FIFO线程进行出错处理
    commThreadState = COMMTHREAD_IDLE_STATE;
    SetEvent(MainContent.m_hPostSendingEvent);
    MainContent.m_pFIFOThread->PostThreadMessage(
        WM_SENDFINISH_NOTIFY,
        FIFOFINISH,
        0);
}

// 当AT命令发送完毕，调用此函数正常退出
void CCommThread::CommThreadOkExit(char *okinfo)
{
	
	
	
    // 状态复位，通知FIFO线程进行出错处理
    commThreadState = COMMTHREAD_IDLE_STATE;
    SetEvent(MainContent.m_hPostSendingEvent);
    MainContent.m_pFIFOThread->PostThreadMessage(
        WM_SENDFINISH_NOTIFY,
        FIFOFINISH,
        0);
}

// 通知前台MMI，某条AT命令正在开始发送
void CCommThread::NotifyAtCommandStartSend(CommandType commandtype)
{
    extern CSerialComm MainSerial;
	
    // 向前台发送某一条AT命令开始发送的指示
    MainSerial.m_hMainWnd->PostMessage(
        WM_START_SEND_ATCOMMAND_NOTIFY,
        commandtype,
        0);
}
void CCommThread::SendCreatedCommand(void)
{
    extern CSerialComm	MainSerial;
    //send AT Com
#ifdef _DEBUG_AT
    WriteAtLog(CommThreadData.commanddata, FALSE);
#endif //_DEBUG_AT
    MainSerial.StartToSend(CommThreadData.commanddata,
		CommThreadData.commandlength);
}

BOOL CCommThread::GetReturnResult(char *returndata)
{
    BOOL result;
	
    if ((_strnicmp(returndata, AtStringOK,
		sizeof(AtStringOK)) == 0) ||
		(_strnicmp(returndata, AtStringError,
		sizeof(AtStringError)) == 0))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
	
    return result;
}
// 通知后台有新消息到达的指示
void CCommThread::NotifyMainWnd(UINT msg,WPARAM wParam,LPARAM lParam)
{
    extern CSerialComm MainSerial;
	
    MainSerial.m_hMainWnd->PostMessage(
        msg,
        wParam,
        lParam);
}

#if defined(R16_ATC)
BOOL CCommThread::GetAtCmdSidetAndWriteFile(CString sAtCmd,CString sVal)
{
    //AT+SIDET?
    //+SIDET(NORMAL_AUDIO): 0
    //+SIDET(AUX_AUDIO): 0
    //
    //OK
	
    //+SIDET(NORMAL_AUDIO): 4096+SIDET(AUX_AUDIO): 0
    //OK
	
    extern CMainContent MainContent;
	
    CIniFile m_ParaFile((MainContent.m_sWorkPath)+PARAFILE);
    CString szIniIdent = _T(""),
		szIniVal   = _T("");
	
#if defined(REVERSE_SIDET)
    // 2. AUX_AUDIO
    szIniIdent = sAtCmd + "1";
    szIniVal   = sVal.Mid(sVal.Find('+') + 1);
    szIniVal   = szIniVal.Mid(szIniVal.Find('+'));
    szIniVal   = szIniVal.Mid(szIniVal.Find(':') + 2);
    szIniVal   = "1," + szIniVal;
    m_ParaFile.WriteString(ATCMD_SECTION,szIniIdent, szIniVal);
	
    // 1. NORMAL_AUDIO
    szIniIdent = sAtCmd + "0";
    szIniVal   = sVal.Mid(sVal.Find(':')+2);
    szIniVal   = szIniVal.Left(szIniVal.Find('+'));
    szIniVal.Remove(0x0d);
    szIniVal.Remove(0x0a);
    szIniVal   = "0," + szIniVal;
    m_ParaFile.WriteString(ATCMD_SECTION,szIniIdent, szIniVal);
#else
    // 1. NORMAL_AUDIO
    szIniIdent = sAtCmd + "0";
    szIniVal   = sVal.Mid(sVal.Find(':')+2);
    szIniVal   = szIniVal.Left(szIniVal.Find('+'));
    szIniVal   = "0," + szIniVal;
    m_ParaFile.WriteString(ATCMD_SECTION,szIniIdent, szIniVal);
	
    // 2. AUX_AUDIO
    szIniIdent = sAtCmd + "1";
    szIniVal   = sVal.Mid(sVal.Find('+') + 1);
    szIniVal   = szIniVal.Mid(szIniVal.Find('+'));
    szIniVal   = szIniVal.Mid(szIniVal.Find(':') + 2);
    szIniVal   = "1," + szIniVal;
    m_ParaFile.WriteString(ATCMD_SECTION,szIniIdent, szIniVal);
#endif
	
    return TRUE;
}
BOOL CCommThread::GetAtCmdSidetTwoLineAndWriteFile(CString sAtCmd,CString sVal,CString sChannel)
{
    extern CMainContent MainContent;
	
    CIniFile m_ParaFile((MainContent.m_sWorkPath)+PARAFILE);
    sAtCmd += sChannel;
    CString s = sChannel + "," + sVal;
    if (!sVal.IsEmpty())
    {
        m_ParaFile.WriteString(ATCMD_SECTION,sAtCmd,s);
        return TRUE;
    }
    return FALSE;
}
#endif

BOOL CCommThread::GetAtCmdEchoAndWriteFile(CString sAtCmd,CString sVal,CString sChannel)
{
    extern CMainContent MainContent;
    CIniFile m_ParaFile((MainContent.m_sWorkPath)+PARAFILE);
    sAtCmd += sChannel;
    CString s;
    s = sVal + "," + sChannel;
    if (!sVal.IsEmpty())
    {
        m_ParaFile.WriteString(ATCMD_SECTION,sAtCmd,s);
        return TRUE;
    }
    return FALSE;
}

BOOL CCommThread::GetAtCmdAndWriteFile(CString sAtCmd,CString sVal)
{
    extern CMainContent MainContent;
    CIniFile m_ParaFile((MainContent.m_sWorkPath)+PARAFILE);
    if (!sVal.IsEmpty())
    {
        if (!sAtCmd.CompareNoCase(AtStringCMIC))
        {
            CString s1,s2;
            UINT i=sVal.Find(",");
            s1 = sVal.Left(i);
            s2 = sVal.Mid(i+1);
            m_ParaFile.WriteString(ATCMD_SECTION,  sAtCmd + "0", CString("0") + "," + s1);
            m_ParaFile.WriteString(ATCMD_SECTION,  sAtCmd + "1", CString("1") + "," + s2);
        }
        else
        {
            m_ParaFile.WriteString(ATCMD_SECTION, sAtCmd, sVal);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL CCommThread::GetParaAndWriteFile(CString val, UINT iCMD)
{
    extern CMainContent MainContent;
	
    CIniFile m_ParaFile((MainContent.m_sWorkPath)+PARAFILE);
    //CString sParam=GetParam(val);
    CString s;
    s.Format("%d",iCMD);
	
    if ((!val.IsEmpty()) && ((val.Find("EMPTY")==-1)))
    {
        m_ParaFile.WriteString(PARA_SECTION, s, "\"" + val + "\"");
        return true;
    }
    else if (val.Find("EMPTY")>=0)
    {
        m_ParaFile.WriteString(PARA_SECTION, s, CString("\"") + "" + "\"");
    }
    else
    {
        m_ParaFile.WriteString(PARA_SECTION, s, CString("\"") + "" + "\"");
    }
    return false;
}

CString CCommThread::GetParam(CString& mName)
{
    int P = mName.Find(":");
	
    char m_Res[256];
    sprintf(m_Res, "%s", mName);
    m_Res[P] = '\0';
	
    mName.Delete(0, P + 1);
    mName.TrimLeft();
    mName.TrimRight();
	
    return m_Res;
}

void CCommThread::ResetMoniterTimer()
{
    extern CSerialComm MainSerial;
	
    MainSerial.m_hMainWnd->PostMessage(
        WM_STOP_MONITOR_TIMER_NOTIFY,
        0,
        0
		)  ;
    Sleep(20);
    MainSerial.m_hMainWnd->PostMessage(
        WM_START_MONITOR_TIMER_NOTIFY,
        0,
        0);
}


// added by Huang ye for activate commport
void CCommThread::ResetSendingTimer()
{
    extern CSerialComm MainSerial;
    MainSerial.m_hMainWnd->PostMessage(
        WM_STOP_SENDING_TIMER_NOTIFY,
        0,
        0
		)  ;
    Sleep(20);
    MainSerial.m_hMainWnd->PostMessage(
        WM_START_SENDING_TIMER_NOTIFY,
        0,
        0);
}

void CCommThread::StopSendingTimer()
{
    extern CSerialComm MainSerial;
    MainSerial.m_hMainWnd->PostMessage(
        WM_STOP_SENDING_TIMER_NOTIFY,
        0,
        0
		);
    Sleep(20);
}
// added by Huang ye for activate commport


void CCommThread::StopMoniterTimer()
{
    extern CSerialComm MainSerial;
    MainSerial.m_hMainWnd->PostMessage(
        WM_STOP_MONITOR_TIMER_NOTIFY,
        0,
        0
		)  ;
    Sleep(20);
}

UINT CCommThread::GetParaFileAtCmdInfo(void)
{
    extern CMainContent MainContent;
    CIniFile m_ParaFile((MainContent.m_sWorkPath)+CString(PARAFILE));
	
    m_paraInfo.RemoveAll();
    m_paraValue.RemoveAll();
    m_ParaFile.ReadSection(ATCMD_SECTION,m_paraInfo);
    UINT atCmdNum=m_paraInfo.GetSize();
    if (atCmdNum>0)
    {
        for (UINT i=0;i<atCmdNum;i++)
            m_paraValue.Add(m_ParaFile.ReadString(ATCMD_SECTION,m_paraInfo[i],""));
    }
    return atCmdNum;
}

UINT CCommThread::GetParaFileInfoAndValue(void)
{
    extern CMainContent MainContent;
    CIniFile m_ParaFile((MainContent.m_sWorkPath)+CString(PARAFILE));
    UINT i = 0;
	
    m_paraInfo.RemoveAll();
    m_paraValue.RemoveAll();
    m_ParaFile.ReadSection(PARA_SECTION,m_paraInfo);
    UINT paraNum = m_paraInfo.GetSize();
    if (paraNum > 0)
    {
        for (i=0; i<paraNum; i++)
            m_paraValue.Add(m_ParaFile.ReadString(PARA_SECTION,m_paraInfo[i],"")) ;
		
    }
    return paraNum;
}

void CCommThread::CommThreadForceExit()
{
    CommThreadOkExit(_T(""));
}

void CCommThread::GetOptionAndWriteFile(CString szOptionName, CString szSubOptionName, CString szOptionValue)
{
    extern CMainContent MainContent;
	
    WritePrivateProfileString(szOptionName, szSubOptionName, szOptionValue,\
		((MainContent.m_sWorkPath)+CString(PARAFILE)));
	
    return;
	
}

int GetEncryptSolution(CString val)
{
	int nResolution = 0;
	CString strResolution;
	if ((!val.IsEmpty())&&((val.Find("EMPTY")==-1)))
	{
		nResolution = atoi(val);
	}

	return nResolution;

}

BOOL IsValidNumHeader(const char *Val_p)
{
	if (0 == strlen(Val_p))
	{
		return FALSE;
	}
	if (NULL != strstr(Val_p, "EMPTY"))
	{
		return FALSE;
	}
	while (*Val_p)
	{
		if (0x30 != *Val_p)
		{
			return TRUE;
		}
		Val_p++;
	}
	return FALSE;
}

BOOL IsIpSettingItemEmpty(CString szVal)
{
	return szVal.IsEmpty() || (-1 < szVal.Find("EMPTY")) || (-1 < szVal.Find("***"));
}

#if defined(ENABLE_SESSION_TOLL_FUNCTION)
BOOL IsRateTableItemEmpty(CString szVal)
{
	return szVal.IsEmpty() || (-1 < szVal.Find("EMPTY")) || (-1 < szVal.Find("0*0*0*0*0*"));
}
BOOL IsRateRuleItemEmpty(CString szVal)
{
	return szVal.IsEmpty() || (-1 < szVal.Find("EMPTY")) || (-1 < szVal.Find("0*0*")) || (-1 < szVal.Find("*0*"));
}
#endif