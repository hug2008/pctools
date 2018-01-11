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

//�����Զ�����Ϣ
// for system
#define WM_SYSTEMTRAY                      WM_USER + 2
#define WM_SENDHIGH_NOTIFY                 WM_USER + 3
#define WM_SENDLOW_NOTIFY	               WM_USER + 4
#define WM_RECEIVEHIGH_NOTIFY              WM_USER + 5
#define WM_SENDNEWFIFO_NOTIFY              WM_USER + 6
#define WM_SENDCOMM_NOTIFY                 WM_USER + 7
#define WM_CLEARSTATE_NOTIFY               WM_USER + 8
#define WM_SENDFINISH_NOTIFY               WM_USER + 9
#define WM_COMMMANAGE_NOTIFY               WM_USER + 10
#define WM_RECEIVE_SERIALDATA_NOTIFY       WM_USER + 11
#define WM_RECEIVE_END_NOTIFY              WM_USER + 12
#define WM_START_MONITOR_TIMER_NOTIFY      WM_USER + 13
#define WM_STOP_MONITOR_TIMER_NOTIFY       WM_USER + 14
#define WM_OPEN_SERIAL_CONNECT_NOTIFY      WM_USER + 15
#define WM_CLOSE_SERIAL_CONNECT_NOTIFY     WM_USER + 16
#define WM_START_SEND_ATCOMMAND_NOTIFY     WM_USER + 17
// added by Huang ye for activate commport
#define WM_START_SENDING_TIMER_NOTIFY      WM_USER + 18
#define WM_STOP_SENDING_TIMER_NOTIFY       WM_USER + 19
// added by Huang ye for activate commport

// 2007.06.07 add by hqr to Open/Close Comm
#define WM_OPEN_COMM_CONNECTION_NOTIFY    WM_USER + 20
#define WM_CLOSE_COMM_CONNECTION_NOTIFY   WM_USER + 21
// 2007.06.07 add by hqr to Open/Close Comm

#define WM_READ_PARA_ERR_NOTIFY            WM_USER + 40
#define WM_READ_PARA_OK_NOTIFY             WM_USER + 41
#define WM_READ_PARA_STATE_NOTIFY          WM_USER + 42
#define WM_WRITE_PARA_ERR_NOTIFY           WM_USER + 43
#define WM_WRITE_PARA_OK_NOTIFY            WM_USER + 44
#define WM_WRITE_PARA_STATE_NOTIFY		   WM_USER + 45
#define WM_WRITE_TO_DEFAULT_SETTING_ERR_NOTIFY WM_USER +46
#define WM_WRITE_TO_DEFAULT_SETTING_OK_NOTIFY  WM_USER +47
#define WM_RECOVER_DEFAULT_SETTING_ERR_NOTIFY  WM_USER + 49
#define WM_RECOVER_DEFAULT_SETTING_OK_NOTIFY   WM_USER + 50

#define WM_CHANGE_PASS_ERR_NOTIFY              WM_USER + 52
#define WM_CHANGE_PASS_OK_NOTIFY               WM_USER + 53

#define WM_LOCK_PORO_PAGE_NOTIFY               WM_USER + 54

//for worknote
#define WM_DETECT_ONLINE_END_NOTIFY		   WM_USER + 141
#define WM_CLEAR_SEND_DATA_NOTIFY          WM_USER + 142
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
//#define  MONITORDETECT		   8000
// added by Huang ye for activate commport
#define  MONITORDETECT		       20000
// added by Huang ye for activate commport

#define PARAFILE        "Parafile.ini"
#define PARA_SECTION    "PARA"
#define ATCMD_SECTION   "ATCMD"

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
#ifdef  MENU_SAVE
    WRITE_TOTAL_SETTING_SAVE_9994_STATE,
#endif
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


	READ_TYPE_PARA,


    READ_SPEED_TIME_STATE,			//������ʱʱ��
    READ_LOCAL_USE_METHOD_STATE,    //����������ʹ�÷�ʽ
    /*added 11.24 by cyh*/
    READ_IF_BAN_DDN_STATE,          //���Ƿ�������ڳ�;
    /*added 11.24 by cyh*/
    READ_SMS_CENTER_STATE,          //������Ϣ����

#ifdef  _XIAMEN_START_BUILD
    READ_RECV_SMS_CENTER_STATE,
    READ_IF_DISPLAY_ORIGI_PHONE_NUM_STATE,
    READ_PRODUCE_ID_STATE,
    READ_IF_LOC_PHONE_SEG_VALID_STATE,
#endif // _XIAMEN_START_BUILD

#ifdef  _YINGNUO_SECURITY_BUILD
    READ_YINGNUO_SECURITY0_STATE,
    READ_YINGNUO_SECURITY1_STATE,
    READ_YINGNUO_SECURITY2_STATE,
    READ_YINGNUO_SECURITY3_STATE,
    READ_YINGNUO_SECURITY4_STATE,
#endif // _YINGNUO_SECURITY_BUILD

#ifdef  _TONGZE_PARA_BUILD
    READ_PRODUCE_ID_STATE,
    READ_IF_LOC_PHONE_SEG_VALID_STATE,
#endif // _TONGZE_PARA_BUILD

    READ_LOCAL_ZONE_STATE,			//����������
    READ_DIRECT_NUMBER_STATE,       //��ֱ������
#if defined(NETWORK_CONFIG)
    READ_EMERGENCY_CALL_STATE,      // ����������
    READ_OPERATION_NAME_STATE,      // ����Ӫ����
#endif
    READ_BAN_NUMBER_STATE,          //����������
    READ_IF_ADD_ZERO_FOR_EXTR_PHONE, //������ֻ��Ƿ����
    READ_LOCAL_PHONE_SEGMENT,        //�������ֻ������
    READ_LOCAL_USER_PHONE_SEGMENT,   //���Զ��屾���ֻ������
    READ_IF_STRICT_INCOMMING,   	 //���Ƿ����ƺ�����������
    READ_STRICT_INCOMMING_SEGMENT,   //�����ƺ�������
    READ_IF_AUTO_LOCK_PIN,           //���Ƿ��Զ���PIN��
    READ_SET_PIN_LENGTH,             //������PIN�볤��
    READ_SET_INIT_PIN_CODE,          //�����ó�ʼPIN��
    /*added 11.24 by cyh*/
    READ_PIN_CODE_STATE,			  //���̶�PIN��ֵ
    READ_IF_USE_RADOM_PIN_CODE_STATE, //���Ƿ�ʹ�����PIN��
    /*added 11.24 by cyh*/
    READ_IF_BIND_SIM_CARD,           //�������Ƿ��SIM��
    READ_BIND_SIM_CARD_NUMBER,       //�������󶨵�SIM����
    READ_IF_LOCK_COMPANY,            //���Ƿ�������Ӫ��
    READ_LOCKED_COMPANY,             //����������Ӫ��
    READ_IF_LOCK_DISTRICT,           //���Ƿ���С��
    READ_LOCKED_DISTRICT_NUMBER,     //��������С����
    READ_IF_AUTO_LOCK_DISTRICT,      //���Ƿ��Զ���С��
    READ_LOCKED_DISTRICT_CODE,       //���Ѿ�������С����


    READ_IF_LOCK_LAC,                 //read if locking lac
    READ_LOCKED_LAC_CODE,             // read the first part of the maxium locked lac Code
    READ_LOCKED_LAC_CODE2,              // read the second part of the maxium lac code

    READ_IP_SETTING,                 //��IP����
    

#ifdef      CARD_ENCRYPT
    // added by Huang Ye APR. 3 2006
    READ_ENCRYPT_SOLUTION,                  //Read the Encrypted solution of the card
    READ_COPATIBLE_COMMON_CARD,             //Read the Compatible solution if supported
    READ_VENDOR_SERIAL_NO,                  // Read the serial NO. of the vendor
    READ_MAIN1_ENCRYPTED_KEY,               // Read the first group key word
    READ_MAIN2_ENCRYPTED_KEY,                // Read the second group key word
    READ_MAIN3_ENCRYPTED_KEY,                // Read the third group key word
    READ_MAIN4_ENCRYPTED_KEY,                // Read the forth group key word
    READ_MAIN5_ENCRYPTED_KEY,                // Read the fifth group key word
    READ_KEY_SERIAL_NO,						 // Read the key no
#endif

#if !defined(NO_NXT)
    READ_SCROLL_HOTLINE_CMD,                 // 1001    //�ͷ����ߺ���

    READ_SCROLL_NXTNUM_CMD,                  // 1002    //ũ��ͨ����

    READ_SCROLL_MOBILESEC_CMD,               // 1003    //�ƶ��������

    READ_SCROLL_ISSUANCENUM_CMD,             // 1004    //��������������

    READ_LIMIT_MSGAD_CMD,                    // 1020    //����ͷ����

    READ_SCROLL_NXTMENUTITLE,                // 13000   //ũ��ͨ�˵�����

    READ_SCROLL_MATTER,                      // 13001   //����������

    READ_SRCOLL_MATTERAD,                    // 13002   //��������ͷ
#endif

#if !defined(NO_NXT) && defined(CXT_EX)
    READ_G166_IPADDRESS,                    // 981     IP��ַ
    READ_G166_IPPORT,                       // 982     IP�˿�
    READ_G166_DNS1,                         // 983     DNS������1
    READ_G166_DNS2,                         // 984     DNS������2
#endif

#if defined(ENABLE_MOBILE_HEAD)
	READ_MOBILE_HEAD,
#endif

	READ_TYPE_DIVIDING,

	READ_MIC_GAIN_LEVEL,			 //Read Microphone Gain Level
	READ_CHANGE_SIDE_TONE,			 //Read Side Tone Gain Level
	READ_ECHO_CANCEL_CONTROL,        //Read Echo Cancellation Control
	READ_LCD_CONTRAST,               //Read Lcd contrast

	READ_TOTAL

}ReadTotalSettingState;

#define  CHANG_COMMAND_CMD				1       //�޸ĳ�������
#define  SPEED_TIME_CMD					2		//����ʱʱ��
#define  LOCAL_USE_METHOD_CMD			3       //��������ʹ�÷�ʽ
#define  SMS_CENTER_CMD			        4
/*added 11.24 by cyh*/
#define  IF_BAN_DDN_CMD                 5       //�Ƿ�������ڳ�;
/*added 11.24 by cyh*/

#ifdef  _XIAMEN_START_BUILD
#define RECV_SMS_CENTER_CMD             6
#define IF_DISPLAY_ORIGI_PHONE_NUM_CMD  7
#define PRODUCE_ID_CMD                  9
#define IF_LOC_PHONE_SEG_VALID_CMD      11
#endif  // _XIAMEN_START_BUILD

#ifdef  _YINGNUO_SECURITY_BUILD
#define YINGNUO_SECURITY0_CMD			20
#define YINGNUO_SECURITY1_CMD			21
#define YINGNUO_SECURITY2_CMD			22
#define YINGNUO_SECURITY3_CMD			23
#define YINGNUO_SECURITY4_CMD			24
#endif  // _YINGNUO_SECURITY_BUILD

#ifdef  _TONGZE_PARA_BUILD
#define PRODUCE_ID_CMD                  9
#define IF_LOC_PHONE_SEG_VALID_CMD      11
#endif  // _TONGZE_PARA_BUILD

#ifdef      CARD_ENCRYPT
// added by Huang Ye APR. 3 2006
#define  ENCRYPT_SOLUTION_CMD			  19
#define  COPATIBLE_COMMON_CARD_CMD        18
#define  VENDOR_SERIAL_NO_CMD             17
#define  KEY_SERIAL_NO_CMD				  31
#define  MAIN1_ENCRYPTED_KEY_CMD          20
#define  MAIN2_ENCRYPTED_KEY_CMD          21
#define  MAIN3_ENCRYPTED_KEY_CMD          22
#define  MAIN4_ENCRYPTED_KEY_CMD          23
#define  MAIN5_ENCRYPTED_KEY_CMD          24
#endif

#if !defined(NO_NXT) && defined(CXT_EX)
#define  G166_IPADDRESS_CMD             981     // IP��ַ
#define  G166_IPPORT_CMD                982     // IP�˿�
#define  G166_DNS1_CMD                  983     // DNS������1
#define  G166_DNS2_CMD                  984     // DNS������2
#endif

#define  LOCAL_ZONE_CMD 				1000    //��������

#if !defined(NO_NXT)
#define  SCROLL_HOTLINE_CMD             1001    //�ͷ����ߺ���

#define  SCROLL_NXTNUM_CMD              1002    //ũ��ͨ����

#define  SCROLL_MOBILESEC_CMD           1003    //�ƶ��������

#define  SCROLL_ISSUANCENUM_CMD         1004    //��������������

#define  LIMIT_MSGAD_CMD                1020    //����ͷ����

#define  SCROLL_NXTMENUTITLE_CMD        13000   //ũ��ͨ�˵�����

#define  SCROLL_MATTER_CMD              13001   //����������

#define  SRCOLL_MATTERAD_CMD            13002   //��������ͷ
#endif

#define	 DIRECT_NUMBER_CMD				2001    //ֱ������
#define	 BAN_NUMBER_CMD			        3001    //��������
#define  IF_ADD_ZERO_FOR_EXTR_PHONE_CMD 4000    //����ֻ��Ƿ����
#define  LOCAL_PHONE_SEGMENT_CMD		4001    //�����ֻ������
#define	 IF_STRICT_INCOMMING_CMD		5000	//�Ƿ����ƺ�����������
#define  STRICT_INCOMMING_SEGMENT_CMD   5001    //�����ƺ�������
#define	 IF_AUTO_LOCK_PIN_CMD           6000    //���Ƿ��Զ���PIN��
#define	 SET_PIN_LENGTH_CMD             6001    //������PIN�볤��
#define  SET_INIT_PIN_CODE_CMD          6002    //�����ó�ʼPIN��
/*added 11.24 by cyh*/
#define  PIN_CODE_VALUE_CMD             6004    //�̶�PIN��ֵ
#define  IF_USE_RADOM_PIN_CODE_CMD      6005    //�Ƿ�ʹ�����PINֵ
/*added 11.24 by cyh*/
#define  IF_BIND_SIM_CARD_CMD			7000	//�������Ƿ��SIM��
#define	 BIND_SIM_CARD_NUMBER_CMD       7001    //�������󶨵�SIM����
#define	 IF_LOCK_COMPANY_CMD            7002    //���Ƿ�������Ӫ��
#define	 LOCKED_COMPANY_CMD             7003    //����������Ӫ��
#define  IF_LOCK_DISTRICT_CMD           8000    //���Ƿ���С��
#define  LOCKED_DISTRICT_NUMBER_CMD     8001	//��������С����
#define	 IF_AUTO_LOCK_DISTRICT_CMD      8002    //���Ƿ��Զ���С��
#define	 LOCKED_DISTRICT_CODE_CMD		8003    //���Ѿ�������С����
#define  IF_LOCK_LAC_CMD                8004    // read if locking lac
#define  LOCKED_LAC_CODE_CMD            8005    // read Locked Lac code
#define  LOCKED_LAC_CODE_CMD2           8006    // read Locked Lac code



#define	 IP_SETTING_CMD                 9001	//��IP����

#define  WRITE_END_CMD                  9995    //д��������
#define  WRITE_BEGIN_CMD                9996    //��ʼд����

#define	 RECOVER_DEFAULT_SETTING_CMD    9997    //�ָ�Ĭ������
#define  SAVE_TO_DEFAULT_SETTING_CMD    9998    //����ΪĬ������
#define  SAVE_SETTING_CMD               9999    //��������

#endif
