#ifndef PHONE_MANAGER_PARA_DEFINE
#define PHONE_MANAGER_PARA_DEFINE

#include "NB30.H"
//串口相关定义
#define XON             0x11
#define XOFF            0x13
//默认串口，调试时设定为COM1，实际使用时设定为COM6
#define COMMPORT "COM3"
#define CONNECTBAUD     115200
#define CONNECTDATABITS 8
#define CONNECTPARITY   0
#define CONNECTSTOPBITS 0
#define CONNECTFLOWCTRL 1

#include "Para3In1Def.h"


#define MAXLENGTH    20

//发送数据时使用的数据结构
typedef struct SerialSendTag
{
    int        sendlength;
    char      *senddata;
}SerialSend;

//串口接收和发送的最大数据量
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

// 向移动终端发送不同数据包时监控定时器的时间大小定义
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


	READ_CMD_DIAL_WAITING_TIME_RW,       /* 速拨延时 */

	READ_CMD_HOW_TO_USE_CITY_NUMBER_RW,  /* 本地区号使用方式 */

	READ_CMD_SMS_CENTER_NUMBER_RW,       /* 网管中心号码  */

	READ_CMD_BARING_OTHER_CITY_NUMBER_RW,/* 是否禁止拨打长途电话 */

#if defined(XMSTART_NET_AND_COMMGR_ENABLE)
	READ_SMS_RECEIVE_NUMBER_RW,       /* 读短信接受中心号码 */
	READ_DISPLAY_ORIGINAL_NUMBER_RW,  /* 显示原始号码       */
	READ_CELL_LEVEL_QUALITY_R,        /* 小区信号强度质量信息 */
	READ_CLIENT_ID_RW,                /* 生产商ID信息 */
#endif

	READ_CMD_LOCAL_NUMBER_ACTIVE_RW, /* 本地手机号段是否有效 */

#if defined (ICCID_BUILD)
	READ_IMEI_CCID_INFO_R,           /* Imei和Ccid串号信息 */
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
	READ_CMD_SWITCH_INTELLIGENT_DAIL_RW,/* 智能拨号开关   */
	READ_CMD_SWITCH_AOTU_LOCK_RW,       /* 自动锁屏开关   */	
	READ_CMD_SWITCH_TOLL_INDICTION_RW,  /* 收费提示音开关 */
	READ_CMD_SWITCH_SPECIAL_APP_RW,     /* 特殊应用开关   */
	READ_CMD_USER_TAG_RW,               /* 用户标识信息   */
#endif


	READ_CMD_CITY_NUMBER_RW,            /* 本地区号       */

	READ_HOT_NUMBER_TOTAL,              /* 直拨号段列表   */

	READ_BARRING_NUMBER_TOTAL,          /* 禁拨号段列表   */

	READ_CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW,/* 拨长途自动加 '0' */
	READ_LOCALPHONE_HEADER_TOTAL,       /* 本地号段列表   */

	READ_CMD_LIMIT_ALL_INCOMING_CALL_RW,/* 限制所有呼入   */
	READ_LIMITINCOMING_HEADER_TOTAL,    /* 限制呼入部分   */

	READ_CMD_AUTO_LOCK_PIN_ENABLE_RW,   /* 设置自动锁PIN码 */
	READ_CMD_PIN_LEN_RW,                /* 设置PIN码长度   */
	READ_CMD_INIT_PIN_RW,               /* 设置初始PIN码   */
	READ_CMD_ENCRYPT_PIN_RW,            /* 设置随机PIN码   */
	READ_CMD_FIX_PIN_RW,                /* 设置固定PIN码   */
	READ_CMD_USE_ENCRYPT_PIN_RW,        /* 是否使用PIN码   */


	READ_CMD_SIM_BAND_TO_HANDSET_RW,    /* 话机绑定SIM卡设置 */
	READ_CMD_SIM_LOCKED_IMSI_RW,        /* 话机绑定SIM卡号(imsi) */
	READ_CMD_SP_LOCK_ENABLE_RW,         /* 话机是否锁定运营商 */
	READ_CMD_LOCKED_SERVICE_PROVIDER_RW,/* 话机锁定运营商号  */


	READ_CMD_CELL_LOCK_ENABLE_RW,       /* 是否锁小区      */
	READ_CMO_LOCKED_CELL_NUM_RW,        /* 锁定小区号数目  */
	READ_CMD_CELL_AUTO_LOCK_ENABLE_RW,  /* 是否自动锁小区  */
	READ_CMD_CELL_INFO_RW,              /* 已经锁定的小区号*/

#if defined(LOCK_LAC)
	READ_CMD_LAC_LOCK_ENABLE_RW,       /* 是否锁大区       */
	READ_CMD_LAC_LOCKED_CELL_NUM_RW,   /* 锁定大区号数目   */
	READ_CMD_LAC_INFO_STR_RW,          /* 已经锁定的大区号 */
#endif

	READ_NUM_IP_HEADER_TATOL,          /* Ip 拨号规则      */

#if defined(INTELLIGENT_SPEED_DIAL)
	READ_NUM_INT_SPEED_DIAL_TOTAL,     /* 智能速拨参数     */
#endif

#if defined(SPECIAL_DIAL)
	READ_NUM_SPECIAL_DIAL_TOTAL,       /* 特殊拨号         */
#endif

#if defined(ENABLE_SESSION_TOLL_FUNCTION)
	READ_CMD_MAJOR_RATE_SETTING_STR_RW,       /* 主费率表收费规律总条数          */
	READ_CMD_MAJOR_RATE_SETTING_START_TIME_RW,/* 主费率表设置,起跳时间           */
	READ_CMD_MAJOR_RATE_SETTING_FILE_ID_RW,   /* 主费率表设置, 费率文件 Id       */
	READ_CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R, /* 主费率表设置, 费率号段记录总数  */
	READ_CMD_MAJOR_RATE_SETTING_RATE_COUNT_R, /* 主费率表设置, 费率表记录总数    */
	READ_CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R,  /* 主费率表设置, 费率文件帧标记    */
	READ_CMD_MAJOR_RATE_HEADER_TYPE_STR_RW,   /* 主费率表设置,费率号段起始地址   */
	READ_CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R,/*主费率表设置,费率文件校验码     */
#if defined(VG_MINOR_RATE)
	READ_CMD_MINOR_RATE_SETTING_STR_RW,       /* 次费率表收费规律总条数          */
	READ_CMD_MINOR_RATE_SETTING_START_TIME_RW,/* 次费率表设置,起跳时间           */
	READ_CMD_MINOR_RATE_SETTING_FILE_ID_RW,   /* 次费率表设置, 费率文件 Id       */
	READ_CMD_MINOR_RATE_SETTING_HEAD_COUNT_R, /* 次费率表设置, 费率号段记录总数  */
	READ_CMD_MINOR_RATE_SETTING_RATE_COUNT_R, /* 次费率表设置, 费率表记录总数    */
	READ_CMD_MINOR_RATE_SETTING_FRAM_FLAG_R,  /* 次费率表设置, 费率文件帧标记    */
	READ_CMD_MINOR_RATE_HEADER_TYPE_STR_RW,   /* 次费率表设置,费率号段起始地址   */
	READ_CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R,/*次费率表设置,费率文件校验码     */
#endif
#endif

#if defined(ENABLE_MOBILE_HEAD)
	READ_CMD_MOBILE_HEADER_RW,
#endif

	READ_TYPE_DIVIDING,               /* 上面是Para=Code, 下面是具体的命令号 */

	READ_MIC_GAIN_LEVEL,			 //Read Microphone Gain Level
	READ_CHANGE_SIDE_TONE,			 //Read Side Tone Gain Level
	READ_ECHO_CANCEL_CONTROL,        //Read Echo Cancellation Control
	READ_LCD_CONTRAST                //Read Lcd contrast


}ReadTotalSettingState;

#endif
