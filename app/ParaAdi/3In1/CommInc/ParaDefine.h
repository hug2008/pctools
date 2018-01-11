#ifndef PHONE_MANAGER_PARA_DEFINE
#define PHONE_MANAGER_PARA_DEFINE

#include "NB30.H"
//������ض���
#define XON             0x11
#define XOFF            0x13
//Ĭ�ϴ��ڣ�����ʱ�趨ΪCOM1��ʵ��ʹ��ʱ�趨ΪCOM6
#define COMMPORT "COM3"
#define CONNECTBAUD     115200
#define CONNECTDATABITS 8
#define CONNECTPARITY   0
#define CONNECTSTOPBITS 0
#define CONNECTFLOWCTRL 1

#include "Para3In1Def.h"


#define MAXLENGTH    20

//��������ʱʹ�õ����ݽṹ
typedef struct SerialSendTag
{
    int        sendlength;
    char      *senddata;
}SerialSend;

//���ڽ��պͷ��͵����������
#define MAXRECEIVECHARNUM      1024
#define MAXSENDCHARNUM         512

#define FIXATCOMMANDLENGTH     (MAXSENDCHARNUM)

#define MAXRECEIVECHARNUM      1024
#define MAXSENDCHARNUM         512

#define CR                     0x0d
#define LF                     0x0a
#define CTRL_Z                 26

#define FIFONEWDATA		       0
#define FIFOFINISH             1
#define FIFOLOST               2

// for timer

#define  DETECTTIMER		   12
#define  MONITORTIMER		   13

// added by Huang ye for activate commport
#define   SENDINGOVERTIMER      14

#define   SENDINGTIME           1000
// added by Huang ye for activate commport


// 2007.06.07 add by hqr to Open/Close Comm

#define  OPENCOMMTIMER          10

#define  CLOSECOMMTIMER         11

#define  COMMOPERATETIME        1000

#define  AfterOpenComm_ReadPara        0x00

#define  AfterOpenComm_WritePara       0x01

#define  AfterOpenComm_ChangpassW      0x02

#define  AfterOpenComm_ChangPwLoad     0x03

#define  AfterOpenComm_ProduceLoad     0x04

#define  AfterOpenComm_SaveDefault     0x05

#define  AfterOpenComm_RecoverDefault  0x06

// 2007.06.07 add by hqr to Open/Close Comm


#define  DETECTTIME	 	       50
#define  FIRSTTTIME	 	       1800

// ���ƶ��ն˷��Ͳ�ͬ���ݰ�ʱ��ض�ʱ����ʱ���С����
// added by Huang ye for activate commport
#define  MONITORDETECT		       20000
// added by Huang ye for activate commport

#define PARAFILE "Parafile.ini"
#define PARA_SECTION  "PARA"
#define ATCMD_SECTION "ATCMD"

typedef unsigned char          Int8;
typedef unsigned short         Int16;



typedef enum CommandTypeTag
{
    /* added by Huang ye for activate commport */

    ACTIVATE_COMMPORT_STATE_CMD,
    /* added by Huang ye for activate commport */
    READ_TOTAL_SETTING_STATE_CMD,
    WRITE_TOTAL_SETTING_STATE_CMD,
    SAVE_TO_DEFAULT_SETTING_STATE_CMD,
    RECOVER_DEFAULT_SETTING_STATE_CMD,
    CHANGE_PASS_STATE_CMD
}CommandType;

typedef struct SendDataTag
{
    CommandType  commandtype;
    void         *senddata;
}SendData;

typedef struct SendFIFOTag
{
    CommandType   		 commandtype;
    void                 *senddata;
    struct SendFIFOTag   *next;
}SendFIFO;

typedef enum WriteToDefaultSettingTag
{
    WRITE_TO_DEFAULT_SETTING_IDLE_STATE,
    WRITE_TO_DEFAULT_SETTING_BEGIN_WRITE_STATE,
    WRITE_TO_DEFAULT_SETTING_END_STATE,
    WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE,
    WRITE_TO_FACTORY_END_STATE
}WriteToDefaultSettingState;

typedef enum RecoverDefaultSettingTag
{
    RECOVER_DEFAULT_SETTING_IDLE_STATE,
    RECOVER_DEFAULT_SETTING_BEGIN_WRITE_STATE,
    RECOVER_DEFAULT_SETTING_END_STATE,
    RECOVER_DEFAULT_SETTING_END_WRITE_STATE
}RecoverDefaultSettingState;

typedef enum ChangePassTag
{
    CHANGE_PASS_IDLE_STATE,
    CHANGE_PASS_BEGIN_WRITE_STATE,
    CHANGE_PASS_END_STATE,
    CHANGE_PASS_END_WRITE_STATE
}ChangePassState;

typedef enum WriteTotalSettingTag
{
    WRITE_TOTAL_SETTING_IDLE_STATE,
    WRITE_TOTAL_SETTING_BEGIN_WRITE_STATE,
    WRITE_TOTAL_SETTING_WAIT_STATE,
    WRITE_ATCMD_SETTING_WAIT_STATE,
    WRITE_TOTAL_SETTING_SAVE_STATE,
    WRITE_TOTAL_SETTING_END_WRITE_STATE,
    WRITE_TOTAL_ATCMD_END_WRITE_STATE

}WriteTotalSettingState;

typedef enum ReadTotalSettingTag
{
    /* added by Huang ye for activate commport */
    ACTIVATE_COMMPORT_BEING,
    /* added by Huang ye for activate commport */

    READ_TOTAL_SETTING_IDLE_STATE,

    /* added by Huang ye for activate commport */
    ACTIVATE_COMMPORT_STATE,
    /* added by Huang ye for activate commport */


	READ_CMD_DIAL_WAITING_TIME_RW,       /* �ٲ���ʱ */

	READ_CMD_HOW_TO_USE_CITY_NUMBER_RW,  /* ��������ʹ�÷�ʽ */

	READ_CMD_SMS_CENTER_NUMBER_RW,       /* �������ĺ���  */

	READ_CMD_BARING_OTHER_CITY_NUMBER_RW,/* �Ƿ��ֹ����;�绰 */

#if defined(XMSTART_NET_AND_COMMGR_ENABLE)
	READ_SMS_RECEIVE_NUMBER_RW,       /* �����Ž������ĺ��� */
	READ_DISPLAY_ORIGINAL_NUMBER_RW,  /* ��ʾԭʼ����       */
	READ_CELL_LEVEL_QUALITY_R,        /* С���ź�ǿ��������Ϣ */
	READ_CLIENT_ID_RW,                /* ������ID��Ϣ */
#endif

	READ_CMD_LOCAL_NUMBER_ACTIVE_RW, /* �����ֻ��Ŷ��Ƿ���Ч */

#if defined (ICCID_BUILD)
	READ_IMEI_CCID_INFO_R,           /* Imei��Ccid������Ϣ */
#endif

#ifdef      SIM_ENCRYPT_COMPLETE_SOLUTIONS
	READ_ENCRYPT_SOLUTION,                  //Read the Encrypted solution of the card
	READ_COPATIBLE_COMMON_CARD,             //Read the Compatible solution if supported
	READ_VENDOR_SERIAL_NO,               // Read the serial NO. of the vendor
	READ_MAIN1_ENCRYPTED_KEY,              // Read the first group key word
	READ_MAIN2_ENCRYPTED_KEY,                // Read the second group key word
	READ_MAIN3_ENCRYPTED_KEY,                // Read the third group key word
	READ_MAIN4_ENCRYPTED_KEY,                // Read the forth group key word
	READ_MAIN5_ENCRYPTED_KEY,                // Read the fifth group key word
	READ_KEY_SERIAL_NO,						 // Read the key no
#endif

#if defined(XMSTART_3IN1_NET_MANAGER)
	READ_CMD_SWITCH_INTELLIGENT_DAIL_RW,/* ���ܲ��ſ���   */
	READ_CMD_SWITCH_AOTU_LOCK_RW,       /* �Զ���������   */	
	READ_CMD_SWITCH_TOLL_INDICTION_RW,  /* �շ���ʾ������ */
	READ_CMD_SWITCH_SPECIAL_APP_RW,     /* ����Ӧ�ÿ���   */
	READ_CMD_USER_TAG_RW,               /* �û���ʶ��Ϣ   */
#endif


	READ_CMD_CITY_NUMBER_RW,            /* ��������       */

	READ_HOT_NUMBER_TOTAL,              /* ֱ���Ŷ��б�   */

	READ_BARRING_NUMBER_TOTAL,          /* �����Ŷ��б�   */

	READ_CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW,/* ����;�Զ��� '0' */
	READ_LOCALPHONE_HEADER_TOTAL,       /* ���غŶ��б�   */

	READ_CMD_LIMIT_ALL_INCOMING_CALL_RW,/* �������к���   */
	READ_LIMITINCOMING_HEADER_TOTAL,    /* ���ƺ��벿��   */

	READ_CMD_AUTO_LOCK_PIN_ENABLE_RW,   /* �����Զ���PIN�� */
	READ_CMD_PIN_LEN_RW,                /* ����PIN�볤��   */
	READ_CMD_INIT_PIN_RW,               /* ���ó�ʼPIN��   */
	READ_CMD_ENCRYPT_PIN_RW,            /* �������PIN��   */
	READ_CMD_FIX_PIN_RW,                /* ���ù̶�PIN��   */
	READ_CMD_USE_ENCRYPT_PIN_RW,        /* �Ƿ�ʹ��PIN��   */


	READ_CMD_SIM_BAND_TO_HANDSET_RW,    /* ������SIM������ */
	READ_CMD_SIM_LOCKED_IMSI_RW,        /* ������SIM����(imsi) */
	READ_CMD_SP_LOCK_ENABLE_RW,         /* �����Ƿ�������Ӫ�� */
	READ_CMD_LOCKED_SERVICE_PROVIDER_RW,/* ����������Ӫ�̺�  */


	READ_CMD_CELL_LOCK_ENABLE_RW,       /* �Ƿ���С��      */
	READ_CMO_LOCKED_CELL_NUM_RW,        /* ����С������Ŀ  */
	READ_CMD_CELL_AUTO_LOCK_ENABLE_RW,  /* �Ƿ��Զ���С��  */
	READ_CMD_CELL_INFO_RW,              /* �Ѿ�������С����*/

#if defined(LOCK_LAC)
	READ_CMD_LAC_LOCK_ENABLE_RW,       /* �Ƿ�������       */
	READ_CMD_LAC_LOCKED_CELL_NUM_RW,   /* ������������Ŀ   */
	READ_CMD_LAC_INFO_STR_RW,          /* �Ѿ������Ĵ����� */
#endif

	READ_NUM_IP_HEADER_TATOL,          /* Ip ���Ź���      */

#if defined(INTELLIGENT_SPEED_DIAL)
	READ_NUM_INT_SPEED_DIAL_TOTAL,     /* �����ٲ�����     */
#endif

#if defined(SPECIAL_DIAL)
	READ_NUM_SPECIAL_DIAL_TOTAL,       /* ���Ⲧ��         */
#endif

#if defined(ENABLE_SESSION_TOLL_FUNCTION)
	READ_CMD_MAJOR_RATE_SETTING_STR_RW,       /* �����ʱ��շѹ���������          */
	READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW,/* �����ʱ�����,����ʱ��           */
	READ_CMD_MAJOR_RATE_SETTING_FILE_ID_RW,   /* �����ʱ�����, �����ļ� Id       */
	READ_CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R, /* �����ʱ�����, ���ʺŶμ�¼����  */
	READ_CMD_MAJOR_RATE_SETTING_RATE_COUNT_R, /* �����ʱ�����, ���ʱ��¼����    */
	READ_CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R,  /* �����ʱ�����, �����ļ�֡���    */
	READ_CMD_MAJOR_RATE_HEADER_TYPE_STR_RW,   /* �����ʱ�����,���ʺŶ���ʼ��ַ   */
	READ_CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R,/*�����ʱ�����,�����ļ�У����     */
#if defined(VG_MINOR_RATE)
	READ_CMD_MINOR_RATE_SETTING_STR_RW,       /* �η��ʱ��շѹ���������          */
	READ_CMD_MINOR_RATE_SETTING_START_TIME_RW,/* �η��ʱ�����,����ʱ��           */
	READ_CMD_MINOR_RATE_SETTING_FILE_ID_RW,   /* �η��ʱ�����, �����ļ� Id       */
	READ_CMD_MINOR_RATE_SETTING_HEAD_COUNT_R, /* �η��ʱ�����, ���ʺŶμ�¼����  */
	READ_CMD_MINOR_RATE_SETTING_RATE_COUNT_R, /* �η��ʱ�����, ���ʱ��¼����    */
	READ_CMD_MINOR_RATE_SETTING_FRAM_FLAG_R,  /* �η��ʱ�����, �����ļ�֡���    */
	READ_CMD_MINOR_RATE_HEADER_TYPE_STR_RW,   /* �η��ʱ�����,���ʺŶ���ʼ��ַ   */
	READ_CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R,/*�η��ʱ�����,�����ļ�У����     */
#endif
#endif

#if defined(ENABLE_MOBILE_HEAD)
	READ_CMD_MOBILE_HEADER_RW,
#endif

	READ_TYPE_DIVIDING,               /* ������Para=Code, �����Ǿ��������� */

	READ_MIC_GAIN_LEVEL,			 //Read Microphone Gain Level
	READ_CHANGE_SIDE_TONE,			 //Read Side Tone Gain Level
	READ_ECHO_CANCEL_CONTROL,        //Read Echo Cancellation Control
	READ_LCD_CONTRAST                //Read Lcd contrast


}ReadTotalSettingState;

#endif
