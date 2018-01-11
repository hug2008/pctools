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

#if defined(PAGE_CFG)
#include "BizPhoneManager.h"
#else
#if !defined(SYS_NAME)
#define SYS_NAME  _T("BinPhoneManager")
#endif
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
    static UINT num = 0;
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
            readTotalSettingState = READ_SPEED_TIME_STATE;

            CreateReadParaCommand(SPEED_TIME_CMD);
            SendCreatedCommand();
            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_SPEED_TIME_STATE, SPEED_TIME_CMD);
            free((char *)wParam);
            break;
        case READ_SPEED_TIME_STATE:			//读速延时时间
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState       = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读速延时时间错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_SPEED_TIME_STATE, SPEED_TIME_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_LOCAL_USE_METHOD_STATE;
                    CreateReadParaCommand(LOCAL_USE_METHOD_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_USE_METHOD_STATE, LOCAL_USE_METHOD_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5) == 0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), SPEED_TIME_CMD);
                }
            }
            free((char *)wParam);
            break;

        case READ_LOCAL_USE_METHOD_STATE:    //读本地区号使用方式
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState       = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读本地区号使用方式错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_LOCAL_USE_METHOD_STATE, LOCAL_USE_METHOD_CMD);
                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState=READ_SMS_CENTER_STATE;
                    CreateReadParaCommand(SMS_CENTER_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_SMS_CENTER_STATE, SMS_CENTER_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), LOCAL_USE_METHOD_CMD);
                }
            }
            free((char *)wParam);
            break;

        case READ_SMS_CENTER_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState       = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读短信中心错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_SMS_CENTER_STATE, SMS_CENTER_CMD);
                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_IF_BAN_DDN_STATE;
                    CreateReadParaCommand(IF_BAN_DDN_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_BAN_DDN_STATE, IF_BAN_DDN_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), SMS_CENTER_CMD);
                }
            }
            free((char *)wParam);
            break;


        case READ_IF_BAN_DDN_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState       = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读是否禁拨国内长途错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_IF_BAN_DDN_STATE, IF_BAN_DDN_CMD);

                }
                else  //READ NEXT PARA
                {
#if defined CARD_ENCRYPT
                    readTotalSettingState = READ_ENCRYPT_SOLUTION;
                    CreateReadParaCommand(ENCRYPT_SOLUTION_CMD);
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_ENCRYPT_SOLUTION, ENCRYPT_SOLUTION_CMD);
#elif !defined(NO_NXT) && defined(CXT_EX)
                    readTotalSettingState = READ_G166_IPADDRESS;
                    CreateReadParaCommand(G166_IPADDRESS_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_G166_IPADDRESS, G166_IPADDRESS_CMD);
#else
                    readTotalSettingState = READ_LOCAL_ZONE_STATE;
                    CreateReadParaCommand(LOCAL_ZONE_CMD);
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_ZONE_STATE, LOCAL_ZONE_CMD);
#endif
                    SendCreatedCommand();
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), IF_BAN_DDN_CMD);
                }
            }
            free((char *)wParam);
            break;


#ifdef CARD_ENCRYPT

        case READ_ENCRYPT_SOLUTION:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读加密方案错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_ENCRYPT_SOLUTION, ENCRYPT_SOLUTION_CMD);

                }
                else  //READ NEXT PARA
                {
                    if (m_nEncryptSolution == 13)
                    {
                        readTotalSettingState =READ_VENDOR_SERIAL_NO;
                        CreateReadParaCommand(VENDOR_SERIAL_NO_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_VENDOR_SERIAL_NO, VENDOR_SERIAL_NO_CMD);

                    }
                    else
                    {
#if !defined(NO_NXT) && defined(CXT_EX)
                        readTotalSettingState = READ_G166_IPADDRESS;
                        CreateReadParaCommand(G166_IPADDRESS_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_G166_IPADDRESS, G166_IPADDRESS_CMD);
#else
                        readTotalSettingState = READ_LOCAL_ZONE_STATE;
                        CreateReadParaCommand(LOCAL_ZONE_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_ZONE_STATE, LOCAL_ZONE_CMD);
#endif

                    }

                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), ENCRYPT_SOLUTION_CMD);
                    m_nEncryptSolution = GetEncryptSolution((&((char *)wParam)[10]));
                }
            }
            free((char *)wParam);
            break;

        case READ_VENDOR_SERIAL_NO:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState       = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读加密卡厂商号错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_VENDOR_SERIAL_NO, VENDOR_SERIAL_NO_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_KEY_SERIAL_NO;
                    CreateReadParaCommand(KEY_SERIAL_NO_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_KEY_SERIAL_NO, KEY_SERIAL_NO_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), VENDOR_SERIAL_NO_CMD);
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
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_KEY_SERIAL_NO, KEY_SERIAL_NO_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState=READ_MAIN1_ENCRYPTED_KEY;
                    CreateReadParaCommand(MAIN1_ENCRYPTED_KEY_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MAIN1_ENCRYPTED_KEY, MAIN1_ENCRYPTED_KEY_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), KEY_SERIAL_NO_CMD);
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
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_MAIN1_ENCRYPTED_KEY, MAIN1_ENCRYPTED_KEY_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState=READ_MAIN2_ENCRYPTED_KEY;
                    CreateReadParaCommand(MAIN2_ENCRYPTED_KEY_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MAIN2_ENCRYPTED_KEY, MAIN2_ENCRYPTED_KEY_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), MAIN1_ENCRYPTED_KEY_CMD);
                }
            }
            free((char *)wParam);
            break;
        case READ_MAIN2_ENCRYPTED_KEY:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读密钥2错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN2_ENCRYPTED_KEY, MAIN2_ENCRYPTED_KEY_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState=READ_MAIN3_ENCRYPTED_KEY;
                    CreateReadParaCommand(MAIN3_ENCRYPTED_KEY_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MAIN3_ENCRYPTED_KEY, MAIN3_ENCRYPTED_KEY_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), MAIN2_ENCRYPTED_KEY_CMD);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_MAIN3_ENCRYPTED_KEY:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState       = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读密钥3错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN3_ENCRYPTED_KEY, MAIN3_ENCRYPTED_KEY_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_MAIN4_ENCRYPTED_KEY;
                    CreateReadParaCommand(MAIN4_ENCRYPTED_KEY_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MAIN4_ENCRYPTED_KEY, MAIN4_ENCRYPTED_KEY_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), MAIN3_ENCRYPTED_KEY_CMD);
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
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN4_ENCRYPTED_KEY, MAIN4_ENCRYPTED_KEY_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState=READ_MAIN5_ENCRYPTED_KEY;
                    CreateReadParaCommand(MAIN5_ENCRYPTED_KEY_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MAIN5_ENCRYPTED_KEY, MAIN5_ENCRYPTED_KEY_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), MAIN4_ENCRYPTED_KEY_CMD);
                }
            }
            free((char *)wParam);
            break;

        case READ_MAIN5_ENCRYPTED_KEY:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读密钥5错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MAIN5_ENCRYPTED_KEY, MAIN5_ENCRYPTED_KEY_CMD);

                }
                else  //READ NEXT PARA
                {
#if !defined(NO_NXT) && defined(CXT_EX)
                    readTotalSettingState = READ_G166_IPADDRESS;
                    CreateReadParaCommand(G166_IPADDRESS_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_G166_IPADDRESS, G166_IPADDRESS_CMD);
#else
                    readTotalSettingState = READ_LOCAL_ZONE_STATE;
                    CreateReadParaCommand(LOCAL_ZONE_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_ZONE_STATE, LOCAL_ZONE_CMD);
#endif
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), MAIN5_ENCRYPTED_KEY_CMD);
                }
            }
            free((char *)wParam);
            break;

#endif// end of the CARD_ENCRYPT

#if !defined(NO_NXT) && defined(CXT_EX)

        case READ_G166_IPADDRESS:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读IP地址错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_G166_IPADDRESS, G166_IPADDRESS_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_G166_IPPORT;
                    CreateReadParaCommand(G166_IPPORT_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_G166_IPPORT, G166_IPPORT_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),G166_IPADDRESS_CMD);
                }
            }
            free((char *)wParam);
            break;

        case READ_G166_IPPORT:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读IP端口错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_G166_IPPORT, G166_IPPORT_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_G166_DNS1;
                    CreateReadParaCommand(G166_DNS1_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_G166_DNS1, G166_DNS1_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),G166_IPPORT_CMD);
                }
            }
            free((char *)wParam);
            break;

        case READ_G166_DNS1:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读DNS服务器1错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_G166_DNS1, G166_DNS1_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_G166_DNS2;
                    CreateReadParaCommand(G166_DNS2_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_G166_DNS1, G166_DNS2_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),G166_DNS1_CMD);
                }
            }
            free((char *)wParam);
            break;

        case READ_G166_DNS2:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读DNS服务器2错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_G166_DNS2, G166_DNS2_CMD);

                }
                else  //READ NEXT PARA
                {
                    readTotalSettingState = READ_LOCAL_ZONE_STATE;
                    CreateReadParaCommand(LOCAL_ZONE_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_ZONE_STATE, LOCAL_ZONE_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),G166_DNS2_CMD);
                }
            }
            free((char *)wParam);
            break;

#endif

        case READ_LOCAL_ZONE_STATE:			//读本地区号
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读本地区号错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_LOCAL_ZONE_STATE, LOCAL_ZONE_CMD);

                }
                else  //READ NEXT PARA
                {
#if defined(NO_NXT)
                    num = 0;
					bIfContinue = TRUE;
                    readTotalSettingState = READ_DIRECT_NUMBER_STATE;
                    CreateReadParaCommand(DIRECT_NUMBER_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_DIRECT_NUMBER_STATE, num+DIRECT_NUMBER_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),LOCAL_ZONE_CMD);
                }
            }
            free((char *)wParam);
            break;
#else
                    readTotalSettingState = READ_SCROLL_HOTLINE_CMD;

                    CreateReadParaCommand(SCROLL_HOTLINE_CMD);

                    SendCreatedCommand();

                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_SCROLL_HOTLINE_CMD, SCROLL_HOTLINE_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),LOCAL_ZONE_CMD);
                }
            }
            free((char *)wParam);
            break;

        case READ_SCROLL_HOTLINE_CMD:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读客服热线号码错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_LOCAL_ZONE_STATE, SCROLL_HOTLINE_CMD);

                }
                else  //READ NEXT PARA
                {

                    readTotalSettingState = READ_SCROLL_NXTNUM_CMD;
                    CreateReadParaCommand(SCROLL_NXTNUM_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_SCROLL_NXTNUM_CMD, SCROLL_NXTNUM_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), SCROLL_HOTLINE_CMD);
                }
            }
            free((char *)wParam);
            break;
        case READ_SCROLL_NXTNUM_CMD:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读农信通号码错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_LOCAL_ZONE_STATE, SCROLL_NXTNUM_CMD);

                }
                else  //READ NEXT PARA
                {

                    readTotalSettingState = READ_SCROLL_MOBILESEC_CMD;
                    CreateReadParaCommand(SCROLL_MOBILESEC_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_SCROLL_MOBILESEC_CMD, SCROLL_MOBILESEC_CMD);
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), SCROLL_NXTNUM_CMD);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_SCROLL_MOBILESEC_CMD:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读农信通号码错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_LOCAL_ZONE_STATE, SCROLL_NXTNUM_CMD);

                }
                else  //READ NEXT PARA
                {
                    num = 0;
                    readTotalSettingState = READ_SCROLL_ISSUANCENUM_CMD;
                    CreateReadParaCommand(SCROLL_ISSUANCENUM_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(READ_SCROLL_ISSUANCENUM_CMD, READ_SCROLL_ISSUANCENUM_CMD, num + SCROLL_ISSUANCENUM_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), SCROLL_MOBILESEC_CMD);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_SCROLL_ISSUANCENUM_CMD:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读滚动条发布号码错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_SCROLL_ISSUANCENUM_CMD, num+SCROLL_ISSUANCENUM_CMD);

                }
                else  //READ NEXT PARA
                {
                    if (num<ISSUANCENUM_NUM-1)
                    {
                        num++;
                        CreateReadParaCommand(SCROLL_ISSUANCENUM_CMD + num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_SCROLL_ISSUANCENUM_CMD, num+SCROLL_ISSUANCENUM_CMD);
                    }
                    else
                    {
                        num = 0;
						bIfContinue = TRUE;
						readTotalSettingState = READ_DIRECT_NUMBER_STATE;
						CreateReadParaCommand(DIRECT_NUMBER_CMD);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_DIRECT_NUMBER_STATE, num+DIRECT_NUMBER_CMD);
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), SCROLL_ISSUANCENUM_CMD+num);
                }
            }
            free((char *)wParam);
            break;
#endif

        case READ_DIRECT_NUMBER_STATE:       //读直拨号码
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读直拨号码错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_DIRECT_NUMBER_STATE, DIRECT_NUMBER_CMD+num);

                }
                else  //READ NEXT PARA
                {

                    if (bIfContinue && (num<DIRECT_NUM))
                    {
                        num++;
                        CreateReadParaCommand(DIRECT_NUMBER_CMD+num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_DIRECT_NUMBER_STATE, DIRECT_NUMBER_CMD+num);
                    }
                    else
                    {
						num  =  0;
						bIfContinue = TRUE;
#if defined(NETWORK_CONFIG)
#if defined(PAGE_CFG)                        
                        if (ENABLE_EMERGENCY_CALL)
                        {
                            readTotalSettingState = READ_EMERGENCY_CALL_STATE;
                            CreateReadParaCommand(INT_EMERGENCY_CALL_CMD);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_EMERGENCY_CALL_STATE, num + INT_EMERGENCY_CALL_CMD);
                        }
                        else
                        {
                            if (ENABLE_OPN_CFG)
                            {
                                readTotalSettingState = READ_OPERATION_NAME_STATE;
                                CreateReadParaCommand(INT_OPERATION_NAME_CMD);
                                SendCreatedCommand();
                                NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_OPERATION_NAME_STATE, num+INT_OPERATION_NAME_CMD);
                            }
                            else
                            {
                                readTotalSettingState = READ_BAN_NUMBER_STATE;
                                bIfContinue = TRUE;
                                CreateReadParaCommand(BAN_NUMBER_CMD);
                                SendCreatedCommand();
                                NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_BAN_NUMBER_STATE, num+BAN_NUMBER_CMD);
                            }
                        }
#else
                        readTotalSettingState = READ_EMERGENCY_CALL_STATE;
                        CreateReadParaCommand(INT_EMERGENCY_CALL_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_EMERGENCY_CALL_STATE, num+INT_EMERGENCY_CALL_CMD);
#endif
#else
                        readTotalSettingState = READ_BAN_NUMBER_STATE;
                        CreateReadParaCommand(BAN_NUMBER_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_BAN_NUMBER_STATE, num+BAN_NUMBER_CMD);
#endif
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    bIfContinue = GetParaAndWriteFile((&((char *)wParam)[10]),DIRECT_NUMBER_CMD+num);
                }
            }
            free((char *)wParam);
            break;


#if defined(NETWORK_CONFIG)
        case READ_EMERGENCY_CALL_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("Read Emergency call Error!");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_EMERGENCY_CALL_STATE, INT_EMERGENCY_CALL_CMD + num);

                }
                else
                {
                    //READ NEXT PARA
                    if (bIfContinue && (num < (EMERGENCY_NUM_INT-1)))
                    {
                        num++;
                        CreateReadParaCommand(INT_EMERGENCY_CALL_CMD + num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_EMERGENCY_CALL_STATE, num+INT_EMERGENCY_CALL_CMD);
                    }
                    else
                    {
                        num = 0;
						bIfContinue = TRUE;
#if defined(PAGE_CFG)
                        if (ENABLE_OPN_CFG)
                        {
                            readTotalSettingState = READ_OPERATION_NAME_STATE;
                            CreateReadParaCommand(INT_OPERATION_NAME_CMD);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_OPERATION_NAME_STATE, num+INT_OPERATION_NAME_CMD);
                        }
                        else
                        {
                            readTotalSettingState = READ_BAN_NUMBER_STATE;
                            bIfContinue = TRUE;
                            CreateReadParaCommand(BAN_NUMBER_CMD);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_BAN_NUMBER_STATE, num+BAN_NUMBER_CMD);
                        }
#else
                        readTotalSettingState = READ_OPERATION_NAME_STATE;
                        CreateReadParaCommand(INT_OPERATION_NAME_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,READ_OPERATION_NAME_STATE, num+INT_OPERATION_NAME_CMD);
#endif
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                   bIfContinue = GetParaAndWriteFile((&((char *)wParam)[10]),  INT_EMERGENCY_CALL_CMD + num);
                }
            }
            free((char *)wParam);
            break;

        case READ_OPERATION_NAME_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("Read network name Error!");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_OPERATION_NAME_STATE, INT_OPERATION_NAME_CMD + num);

                }
                else  //READ NEXT PARA
                {
                    if (bIfContinue && (num < (OPN_TOTAL-1)))
                    {
                        num++;
                        CreateReadParaCommand(INT_OPERATION_NAME_CMD + num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_OPERATION_NAME_STATE,  num+INT_OPERATION_NAME_CMD);
                    }
                    else
                    {
                        readTotalSettingState = READ_BAN_NUMBER_STATE;
                        num         = 0;
                        bIfContinue = TRUE;
                        CreateReadParaCommand(BAN_NUMBER_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_BAN_NUMBER_STATE, num+BAN_NUMBER_CMD);
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    bIfContinue = GetParaAndWriteFile((&((char *)wParam)[10]),  INT_OPERATION_NAME_CMD + num);
                }
            }
            free((char *)wParam);
            break;
#endif

        case READ_BAN_NUMBER_STATE:          //读禁拨号码
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读禁拨号码错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_BAN_NUMBER_STATE, BAN_NUMBER_CMD+num);

                }
                else  //READ NEXT PARA
                {
                    if (bIfContinue && (num<BAN_NUM))
                    {
                        num++;
                        CreateReadParaCommand(BAN_NUMBER_CMD+num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_BAN_NUMBER_STATE, BAN_NUMBER_CMD+num);
                    }
                    else
                    {
                        readTotalSettingState=READ_IF_ADD_ZERO_FOR_EXTR_PHONE;
                        num=0;
						bIfContinue = TRUE;
                        CreateReadParaCommand(IF_ADD_ZERO_FOR_EXTR_PHONE_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_ADD_ZERO_FOR_EXTR_PHONE, IF_ADD_ZERO_FOR_EXTR_PHONE_CMD);
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    bIfContinue = GetParaAndWriteFile((&((char *)wParam)[10]), BAN_NUMBER_CMD+num);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_IF_ADD_ZERO_FOR_EXTR_PHONE: //读外地手机是否加零
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读外地手机是否加零错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_IF_ADD_ZERO_FOR_EXTR_PHONE, IF_ADD_ZERO_FOR_EXTR_PHONE_CMD);
                }
                else  //READ NEXT PARA
                {
                    num=0;
                    readTotalSettingState=READ_LOCAL_PHONE_SEGMENT;
                    bIfContinue=TRUE;
                    CreateReadParaCommand(LOCAL_PHONE_SEGMENT_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_PHONE_SEGMENT, num+LOCAL_PHONE_SEGMENT_CMD);
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),IF_ADD_ZERO_FOR_EXTR_PHONE_CMD);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_LOCAL_PHONE_SEGMENT:        //读本地手机号码段
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读本地手机号码段错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_LOCAL_PHONE_SEGMENT, LOCAL_PHONE_SEGMENT_CMD+num);

                }
                else  //READ NEXT PARA
                {
                    if (num < LOCAL_SEGMENT_NUM)
                    {
                        if ((num <(INT_INIT_LOCAL_PHONE_ITEMNUM - 1)) && (bIfContinue))
                        {
                            num++;
                            CreateReadParaCommand(LOCAL_PHONE_SEGMENT_CMD+num);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_PHONE_SEGMENT, LOCAL_PHONE_SEGMENT_CMD+num);
                        }
                        else if ((num < (INT_INIT_LOCAL_PHONE_ITEMNUM - 1))&&(!bIfContinue))
                        {
                            num = INT_INIT_LOCAL_PHONE_ITEMNUM;
                            CreateReadParaCommand(LOCAL_PHONE_SEGMENT_CMD+num);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_PHONE_SEGMENT, LOCAL_PHONE_SEGMENT_CMD+num);
                        }
                        else
                        {
                            num++;
                            CreateReadParaCommand(LOCAL_PHONE_SEGMENT_CMD+num);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_LOCAL_PHONE_SEGMENT, LOCAL_PHONE_SEGMENT_CMD+num);
                        }
                    }
                    else
                    {
                        readTotalSettingState = READ_IF_STRICT_INCOMMING;
                        num = 0;
						bIfContinue = TRUE;
                        CreateReadParaCommand(IF_STRICT_INCOMMING_CMD);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_STRICT_INCOMMING, IF_STRICT_INCOMMING_CMD);
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    bIfContinue=GetParaAndWriteFile((&((char *)wParam)[10]),LOCAL_PHONE_SEGMENT_CMD+num);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_IF_STRICT_INCOMMING:   	 //读是否限制呼入所有来电
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读是否限制呼入所有来电错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_IF_STRICT_INCOMMING, IF_STRICT_INCOMMING_CMD);

                }
                else  //READ NEXT PARA
                {
                    num=0;
                    readTotalSettingState=READ_STRICT_INCOMMING_SEGMENT;
                    CreateReadParaCommand(STRICT_INCOMMING_SEGMENT_CMD);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_STRICT_INCOMMING_SEGMENT, STRICT_INCOMMING_SEGMENT_CMD+num);

                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),IF_STRICT_INCOMMING_CMD);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_STRICT_INCOMMING_SEGMENT:   //读限制呼入号码段
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读限制呼入号码段错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_STRICT_INCOMMING_SEGMENT, STRICT_INCOMMING_SEGMENT_CMD+num);

                }
                else  //READ NEXT PARA
                {
                    if (num<STRICT_INCOM_NUM)
                    {
                        num++;
                        CreateReadParaCommand(STRICT_INCOMMING_SEGMENT_CMD+num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_STRICT_INCOMMING_SEGMENT, STRICT_INCOMMING_SEGMENT_CMD+num);

                    }
                    else
                    {
                        readTotalSettingState=READ_IF_AUTO_LOCK_PIN;
                        num = 0;
						bIfContinue = TRUE;
                        CreateReadParaCommand(IF_AUTO_LOCK_PIN_CMD+num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_AUTO_LOCK_PIN, IF_AUTO_LOCK_PIN_CMD+num);
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                   bIfContinue = GetParaAndWriteFile((&((char *)wParam)[10]),STRICT_INCOMMING_SEGMENT_CMD+num);
                }
            }
            free((char *)wParam);
            break;
			
        case READ_IF_AUTO_LOCK_PIN:           //读是否自动锁PIN码
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读是否自动锁PIN码错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_IF_AUTO_LOCK_PIN, IF_AUTO_LOCK_PIN_CMD+num);

                }
                else  //READ NEXT PARA
                {
					if (num < (IF_USE_RADOM_PIN_CODE_CMD - IF_AUTO_LOCK_PIN_CMD - 1))
					{
						num++;
						readTotalSettingState=READ_IF_AUTO_LOCK_PIN;
						CreateReadParaCommand(IF_AUTO_LOCK_PIN_CMD+num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_AUTO_LOCK_PIN, IF_AUTO_LOCK_PIN_CMD+num);
					} 
					else
					{
						num=0;
						readTotalSettingState=READ_IF_BIND_SIM_CARD;
						CreateReadParaCommand(IF_BIND_SIM_CARD_CMD + num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_BIND_SIM_CARD, IF_BIND_SIM_CARD_CMD + num);
					}
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]),IF_AUTO_LOCK_PIN_CMD+num);
                }
            }
            free((char *)wParam);
            break;
			

        case READ_IF_BIND_SIM_CARD:           //读话机是否绑定SIM卡
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState=READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读话机是否绑定SIM卡错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_IF_BIND_SIM_CARD, IF_BIND_SIM_CARD_CMD + num);

                }
                else  //READ NEXT PARA
                {
#if defined(LOCK_SERVICE_PROVIDER)
					if (num < (SERVICE_PROVIDER_MAX + LOCKED_COMPANY_CMD - IF_BIND_SIM_CARD_CMD))
#else
					if (num < (LOCKED_COMPANY_CMD - IF_BIND_SIM_CARD_CMD))
#endif
					{
						num++;
						readTotalSettingState= READ_IF_BIND_SIM_CARD;
						CreateReadParaCommand(IF_BIND_SIM_CARD_CMD + num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_BIND_SIM_CARD, IF_BIND_SIM_CARD_CMD + num);
					}
					else
					{
						num = 0;
						bIfContinue = TRUE;
						readTotalSettingState = READ_IF_LOCK_DISTRICT;
						CreateReadParaCommand(IF_LOCK_DISTRICT_CMD + num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_LOCK_DISTRICT, IF_LOCK_DISTRICT_CMD + num);
					}                    
                }
            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), IF_BIND_SIM_CARD_CMD + num);
                }
            }
            free((char *)wParam);
            break;
			
			
        case READ_IF_LOCK_DISTRICT:           //读是否锁小区
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读是否锁小区错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY, READ_IF_LOCK_DISTRICT, IF_LOCK_DISTRICT_CMD + num);

                }
                else  //READ NEXT PARA
                {
                    if (num < (LOCKED_LAC_CODE_CMD2 - IF_LOCK_DISTRICT_CMD - 1))
                    {
						num++;
						readTotalSettingState = READ_IF_LOCK_DISTRICT;
						CreateReadParaCommand(IF_LOCK_DISTRICT_CMD + num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IF_LOCK_DISTRICT, IF_LOCK_DISTRICT_CMD + num);
                    } 
                    else
                    {
						num = 0;
						bIfContinue = TRUE;
						readTotalSettingState = READ_IP_SETTING;
						CreateReadParaCommand(IP_SETTING_CMD + num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IP_SETTING, IP_SETTING_CMD + num);
                    }
                }

            }
            else
            {
                if (_strnicmp((char *)wParam,"+RPAR",5)==0)
                {
                    GetParaAndWriteFile((&((char *)wParam)[10]), IF_LOCK_DISTRICT_CMD + num);
                }
            }
            free((char *)wParam);
            break;
			

        case READ_IP_SETTING:                  //读IP设置
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("读IP设置错误");
                    NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_IP_SETTING, IP_SETTING_CMD+num);

                }
                else  //READ NEXT PARA
                {
                    if (num<IP_SET_NUM)
                    {
                        num++;
                        CreateReadParaCommand(IP_SETTING_CMD+num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_IP_SETTING,IP_SETTING_CMD + num);
                    }
                    else
                    {
                        num = 0;
						bIfContinue = TRUE;
#if defined(ENABLE_MOBILE_HEAD)
						readTotalSettingState = READ_MOBILE_HEAD;
						CreateReadParaCommand(INT_MOBILE_HEADER_STR_CMD + num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MOBILE_HEAD, INT_MOBILE_HEADER_STR_CMD + num);
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
                    bIfContinue = GetParaAndWriteFile((&((char *)wParam)[10]),IP_SETTING_CMD+num);
                }
            }
            free((char *)wParam);
            break;

#if defined(ENABLE_MOBILE_HEAD)
		case READ_MOBILE_HEAD:
			if (GetReturnResult((char *)wParam) == TRUE)
			{
				if (GetErrorResult((char *)wParam) == TRUE)
				{
					commThreadState  = COMMTHREAD_IDLE_STATE;
					readTotalSettingState = READ_TOTAL_SETTING_IDLE_STATE;
					CommThreadErrorExit("读IP移动电话头错误");
					NotifyMainWnd(WM_READ_PARA_ERR_NOTIFY,READ_MOBILE_HEAD, INT_MOBILE_HEADER_STR_CMD + num);

				}
				else  //READ NEXT PARA
				{
					if (bIfContinue && (num<MAX_MOBILE_HEADER_TOTAL-1))
					{
						num++;
						readTotalSettingState = READ_MOBILE_HEAD;
						CreateReadParaCommand(INT_MOBILE_HEADER_STR_CMD + num);
						SendCreatedCommand();
						NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_MOBILE_HEAD, INT_MOBILE_HEADER_STR_CMD + num);
					}
					else
					{
						num = 0;
						bIfContinue = TRUE;
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
					bIfContinue = GetParaAndWriteFile((&((char *)wParam)[11]), INT_MOBILE_HEADER_STR_CMD + num);
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
                    num = 0;
                    readTotalSettingState = READ_CHANGE_SIDE_TONE;
                    CreateReadAtCmdCommand(AtStringSIDET);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY, READ_CHANGE_SIDE_TONE);

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
                    num=0;
                    readTotalSettingState = READ_ECHO_CANCEL_CONTROL;
                    CreateReadAtCmdCommand(AtStringECHO);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_READ_PARA_STATE_NOTIFY,  READ_ECHO_CANCEL_CONTROL);
                }

            }
            else
            {
#if defined(R16_ATC)
                if (_strnicmp((char *)wParam,"+SIDET",6)==0)
                {
                    GetAtCmdSidetAndWriteFile(AtStringSIDET,(&((char *)wParam)[0]));
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
                    num = 0;
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
                    num = 0;
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
                    num = 0;
                    totalNum = GetParaFileInfoAndValue();
                    writeTotalSettingState = WRITE_TOTAL_SETTING_WAIT_STATE;
                    CreateWriteParaCommand(num);
                    if ((atoi(m_paraInfo[num]) >= 8010 && atoi(m_paraInfo[num]) <= 8029) ||
                            (atoi(m_paraInfo[num]) >= 8050 && atoi(m_paraInfo[num]) <= 8069))
                    {
                        Sleep(50);
                    }
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_WAIT_STATE,  ++num);
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

#ifdef   MENU_SAVE
                    Sleep(100);
                    WriteCommand(AtStringSaveSetting_V11);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_SAVE_9994_STATE);
#endif

                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY,WRITE_TOTAL_SETTING_WAIT_STATE,num);
                    num = 0;
                }
                else  //WRITE NEXT PARA
                {
                    if (num<totalNum)
                    {

                        CreateWriteParaCommand(num);
                        if ((atoi(m_paraInfo[num]) >= 8010 && atoi(m_paraInfo[num]) <= 8029) || (atoi(m_paraInfo[num]) >= 8050 && atoi(m_paraInfo[num]) <= 8069))
                            Sleep(50);

                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_WAIT_STATE,++num);
                    }
                    else
                    {
                        num = 0;
                        totalNum = GetParaFileAtCmdInfo();
                        if (totalNum>0)
                        {
                            writeTotalSettingState = WRITE_ATCMD_SETTING_WAIT_STATE;
                            CreateWriteAtCmdCommand(num);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_ATCMD_SETTING_WAIT_STATE,++num);
                            bIfNeedSaveAtCmd = true;
                        }
                        else
                        {
                            num = 0;
                            writeTotalSettingState=WRITE_TOTAL_SETTING_SAVE_STATE;
                            WriteCommand(AtStringSaveSetting);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_SAVE_STATE);



#ifdef   MENU_SAVE

                            Sleep(100);
                            WriteCommand(AtStringSaveSetting_V11);
                            SendCreatedCommand();
                            NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_TOTAL_SETTING_SAVE_9994_STATE);


#endif

                            bIfNeedSaveAtCmd=false;
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
                    NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);

#ifdef   MENU_SAVE
                    Sleep(100);
                    WriteCommand(AtStringSaveSetting_V11);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_TOTAL_SETTING_SAVE_9994_STATE);


#endif
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY, WRITE_ATCMD_SETTING_WAIT_STATE, num);
                    num = 0;
                }
                else  //WRITE NEXT PARA
                {
                    if (num<totalNum)
                    {

                        CreateWriteAtCmdCommand(num);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_ATCMD_SETTING_WAIT_STATE, ++num);
                    }
                    else
                    {
                        num = 0;
                        writeTotalSettingState=WRITE_TOTAL_SETTING_SAVE_STATE;
                        WriteCommand(AtStringSaveSetting);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);
#ifdef   MENU_SAVE

                        Sleep(100);
                        WriteCommand(AtStringSaveSetting_V11);
                        SendCreatedCommand();
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_SETTING_SAVE_9994_STATE);


#endif
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
                    num = 0;

#ifdef  MENU_SAVE
                    writeTotalSettingState = WRITE_TOTAL_SETTING_SAVE_9994_STATE;
                    WriteCommand(AtStringSaveSetting_V11);
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);


#else
                    writeTotalSettingState = WRITE_TOTAL_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY, WRITE_TOTAL_SETTING_SAVE_STATE);
#endif
                }

            }
            else  //UNKONW DATA
            {

            }
            free((char *)wParam);
            break;

#ifdef  MENU_SAVE
        case WRITE_TOTAL_SETTING_SAVE_9994_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {

                    writeTotalSettingState = WRITE_TOTAL_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY, WRITE_TOTAL_SETTING_SAVE_9994_STATE);

                }
                else  //WRITE NEXT PARA
                {
                    num = 0;
                    writeTotalSettingState = WRITE_TOTAL_SETTING_END_WRITE_STATE;
                    CreateEndWriteCommand();
                    SendCreatedCommand();
                    NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY, WRITE_TOTAL_SETTING_SAVE_9994_STATE);
                }

            }
            else  //UNKONW DATA
            {

            }
            free((char *)wParam);
            break;
#endif

        case WRITE_TOTAL_SETTING_END_WRITE_STATE:
            if (GetReturnResult((char *)wParam) == TRUE)
            {
                if (GetErrorResult((char *)wParam) == TRUE)
                {
                    commThreadState  = COMMTHREAD_IDLE_STATE;
                    writeTotalSettingState=WRITE_TOTAL_SETTING_IDLE_STATE;
                    CommThreadErrorExit("结束写操作时错误!");
                    NotifyMainWnd(WM_WRITE_PARA_ERR_NOTIFY, WRITE_TOTAL_SETTING_END_WRITE_STATE);

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
                        NotifyMainWnd(WM_WRITE_PARA_STATE_NOTIFY,WRITE_TOTAL_ATCMD_END_WRITE_STATE);
                    }
                    else
                    {
                        num = 0;
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
                    num = 0;
                    writeTotalSettingState = WRITE_TOTAL_SETTING_IDLE_STATE;
                    CommThreadOkExit("正确结束写AT操作时!");
                    NotifyMainWnd(WM_WRITE_PARA_OK_NOTIFY,WRITE_TOTAL_ATCMD_END_WRITE_STATE);
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
                    NotifyMainWnd(WM_WRITE_TO_DEFAULT_SETTING_ERR_NOTIFY,  WRITE_TO_DEFAULT_SETTING_BEGIN_WRITE_STATE);

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
                    num = 0;
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
                    num = 0;
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
                    num = 0;
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
                    num = 0;
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
                    num = 0;
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
                    num = 0;
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
    s.Format("%d", WRITE_END_CMD);
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
    s.Format("%d",WRITE_BEGIN_CMD);
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
    s.Format("%d", CHANG_COMMAND_CMD);
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

#if defined(NETWORK_CONFIG)
    if ((iCMD>CLEAR_OPN_CFG) && (iCMD <= (CLEAR_OPN_CFG + OPN_TOTAL)))
    {
        m_ParaFile.WriteString(PARA_SECTION, s, "\"" + val + "\"");
        if (_T("FFFFFFFFFFFF") == val.Left(12))
        {
            return false;
        }
        return true;
    }
#endif

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

int CCommThread::GetEncryptSolution(CString val)
{
    int nResolution = 0;
    CString strResolution;
    if ((!val.IsEmpty())&&((val.Find("EMPTY")==-1)))
    {
        nResolution = atoi(val);
    }

    return nResolution;

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
