/************************************************************************/
/* $Id:                  Para3In1Def.h                                  */
/*    ��ģ 3In1 �����ӿ�����Ŷ���ö��                                  */
/*    huangqr 2008-12-06 14:05 Add                                      */
/************************************************************************/
#if !defined(PARA_3IN1_DEF_H)
#define PARA_3IN1_DEF_H

/************************************************************************/
/*                         ���̱���ѡ��                                */
/************************************************************************/
#include <BldOpt.h>


/************************************************************************/
/*                          ��������ö��                                */
/************************************************************************/
typedef enum PhoneSetingTypTag
{
	PHONE_NORMAL_SETTING,
	PHONE_CITY_NUMBER_SETTING,
	PHONE_HOT_NUMBER_SETTING,
	PHONE_BARRING_NUMBER_SETTING,
	PHONE_LOCAL_NUMBER_HEADER_SETTING,
	PHONE_LIMIT_INCOMING_NUMBER_SETTING,
	PHONE_PIN_SETTING,
	PHONE_LOCK_SIM_SETTING,
	PHONE_LOCK_CELL_SETTING,
	PHONE_IPHEDER_SETTING,
	PHONE_INT_SPEED_DIAL_SETTING,
	PHONE_SPECIAL_DIAL_SETTING,
	PHONE_CURRENT_TOLL_RATE_SETTING,
	PHONE_BIZ_NXT_SETTING,
	PHONE_MOBILE_HEAD_SETTING,

	PHONE_SETTING_TYPE_TOTAL
}PhoneSetingTyp;

/************************************************************************/
/*             ��������ö��                                             */
/************************************************************************/

#define CMD_PHONE_SETTING_BASE                  (1000)
/*             ��������б�����                                         */
#define CLEAR_PHONE_SETTING                     (900)



/************************************************************************/
/*                        ϵͳ��������                                  */
/************************************************************************/
/*       ��������                                                       */
#define  CMD_SUPER_PASSWORD_W                   (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 1)

/*       �ٲ���ʱ                                                       */
#define  CMD_DIAL_WAITING_TIME_RW               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 2)

/*       ��������ʹ�÷�ʽ                                               */
#define  CMD_HOW_TO_USE_CITY_NUMBER_RW          (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 3)

/*       �������ĺ���                                                   */
#define  CMD_SMS_CENTER_NUMBER_RW               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 4)

/*       �Ƿ��ֹ����;�绰                                           */
#define  CMD_BARING_OTHER_CITY_NUMBER_RW        (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 5)

#if defined(XMSTART_NET_AND_COMMGR_ENABLE)

/*       ���Ž������ĺ���                                               */
#define  CMD_SMS_RECEIVE_NUMBER_RW              (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 6)

/*      ��ʾԭʼ����                                                    */
#define  CMD_DISPLAY_ORIGINAL_NUMBER_RW         (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 7)

/*      С���ź�ǿ��������Ϣ                                            */
#define  CMD_CELL_LEVEL_QUALITY_R               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 8)

/*      ������ID��Ϣ                                                    */
#define  CMD_CLIENT_ID_RW                       (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 9)
#endif/* XMSTART_NET_AND_COMMGR_ENABLE */


/*      �����ֻ��Ŷ��Ƿ���Ч                                            */
#define  CMD_LOCAL_NUMBER_ACTIVE_RW              (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 11)

#if defined (ICCID_BUILD)
/*      Imei��Ccid������Ϣ                                             */
#define  CMD_IMEI_CCID_INFO_R                    (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 12)
#endif

#if defined(SIMCOM_MIDI_DRIVER)
typedef enum SimcomMidiTypeTag
{
	MIDI_GD2318,
	MIDI_QD5118A,
	MIDI_REALHUMAN,
	MIDI_NO_CHIPSET
}SimcomMidiType;

/*      MIDIоƬ����������Ϣ                                            */
#define  CMD_SIMCOM_MIDI_DRIVER_R                (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 13)


/*      MIDI��������                                                    */
#define  CMD_SIMCOM_MIDI_TOTAL_R                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 14)
#endif/* SIMCOM_MIDI_DRIVER */


#if defined(SMS_MENU_SELECT)
/*      �Ƿ���ʾ����Ϣ�˵�                                              */
#define  CMD_SMS_MENU_DISPLAY_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 15)
#endif/* SMS_MENU_SELECT */


#if defined(ENABLE_SHORT_NUMBER_FUNCTION)
/*      �Ƿ�ʹ�ö̺Ź���                                                */
#define  CMD_ENABLE_SHORT_NUMBER_FUNCTION_RW     (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 16)
#endif

#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
/*      Sim������ID��Ϣ                                                 */
#define  CMD_SIM_ENCRYPT_FACTORYID_RW            (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 17)

/*      �Ƿ������ͨSim��                                               */
#define  CMD_SIM_ENCRYPT_COMPATIBLE_NORMAL_RW    (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 18)

/*      ���ܿ�����                                                      */
#define  CMD_SIM_ENCRYPT_SOLUTION_RW             (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 19)

/*      SIM�汾��Ϣ                                                     */
#define  CMD_SIM_ENCRYPT_EDITION_ID_RW           (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 31)

/*      ���ܿ���������Կ                                                */
#if defined(MAIN_SIMMPK_XMSTART)
#define  CMD_SIM_ENCRYPT_KEY1_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 689)
#define  CMD_SIM_ENCRYPT_KEY2_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 690)
#define  CMD_SIM_ENCRYPT_KEY3_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 691)
#define  CMD_SIM_ENCRYPT_KEY4_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 692)
#define  CMD_SIM_ENCRYPT_KEY5_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 693)
#elif defined(MAIN_SIMMPK_TONGZE)
#define  CMD_SIM_ENCRYPT_KEY1_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 344)
#define  CMD_SIM_ENCRYPT_KEY2_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 345)
#define  CMD_SIM_ENCRYPT_KEY3_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 346)
#define  CMD_SIM_ENCRYPT_KEY4_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 347)
#define  CMD_SIM_ENCRYPT_KEY5_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 348)
#else
#define  CMD_SIM_ENCRYPT_KEY1_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 20)
#define  CMD_SIM_ENCRYPT_KEY2_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 21)
#define  CMD_SIM_ENCRYPT_KEY3_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 22)
#define  CMD_SIM_ENCRYPT_KEY4_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 23)
#define  CMD_SIM_ENCRYPT_KEY5_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 24)
#endif/* MAIN_SIMMPK_XMSTART */

#endif/* ENABLE_SHORT_NUMBER_FUNCTION */


#if defined (CONFIG_GREET)
/*      �����ʺ�����ʾId                                                */
#define  CMD_CONFIG_GREET_RW                     (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 25)
#endif/*CONFIG_GREET*/


#if defined(XMSTART_3IN1_NET_MANAGER)
/*      ���ܲ��ſ���                                                    */
#define  CMD_SWITCH_INTELLIGENT_DAIL_RW          (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 26)

/*      �Զ���������                                                    */
#define  CMD_SWITCH_AOTU_LOCK_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 27)

/*      �շ���ʾ������                                                  */
#define  CMD_SWITCH_TOLL_INDICTION_RW            (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 28)

/*      ����Ӧ�ÿ���                                                    */
#define  CMD_SWITCH_SPECIAL_APP_RW               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 29)

/*      �û���ʶ��Ϣ                                                    */
#define  CMD_USER_TAG_RW                         (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 30)
#endif/* XMSTART_3IN1_NET_MANAGER */


/************************************************************************/
/*                        �������Ų���                                  */
/************************************************************************/
/*      ��������                                                        */
#define  CMD_CITY_NUMBER_RW                      (PHONE_CITY_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 0)
#if defined(XMSTART_OPT_DIALFLOW)
#define  CITY_NUMBER_LEN                         (17)
#else
#define  CITY_NUMBER_LEN                         (6)
#endif



/************************************************************************/
/*                        ֱ���Ŷβ���                                  */
/************************************************************************/
#define  NUM_HOT_NUMBER_TOTAL                    (36)
/*      ֱ���Ŷ�,��ʼ��ַ                                               */
#define  CMD_HOT_NUMBER_STR_RW                  (PHONE_HOT_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      ֱ���Ŷ�,������ַ                                               */
#define  CMD_HOT_NUMBER_END_RW                  (PHONE_HOT_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + NUM_HOT_NUMBER_TOTAL)
/*      �������ֱ���Ŷ�                                                */
#define  CMD_HOT_NUMBER_CLEAR_ALL_W             (PHONE_HOT_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)


/************************************************************************/
/*                        �����Ŷβ���                                  */
/************************************************************************/
#define  NUM_BARRING_NUMBER_TOTAL                (36)
/*      �����Ŷ�,��ʼ��ַ                                               */
#define  CMD_BARRING_NUMBER_STR_RW              (PHONE_BARRING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      �����Ŷ�,������ַ                                               */
#define  CMD_BARRING_NUMBER_END_RW              (PHONE_BARRING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + NUM_BARRING_NUMBER_TOTAL)
/*      ������н����Ŷ�                                                */
#define  CMD_BARRING_NUMBER_CLEAR_ALL_W         (PHONE_BARRING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)



/************************************************************************/
/*                        ���غ���β���                                */
/************************************************************************/
/*      ����;�Զ��� '0'                                                */
#define  CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW      (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + 0)
#if defined(XMSTART_3IN1_NET_MANAGER)
#define  NUM_LOCALPHONE_HEADER_TOTAL            (400 + 200)
#define  LOCALPHONE_HEADER_LEN                  (8)
#else
#define  NUM_LOCALPHONE_HEADER_TOTAL            (400 + 20)
#define  LOCALPHONE_HEADER_LEN                  (16)
#endif
/*      ���غŶ�,��ʼ��ַ                                               */
#define  CMD_LOCALPHONE_HEADER_STR_RW           (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      ���غŶ�,������ַ                                               */
#define  CMD_LOCALPHONE_HEADER_END_RW           (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + NUM_LOCALPHONE_HEADER_TOTAL)

/*      ������б��غŶ�                                                */
#define  CMD_LOCALPHONE_NUMBER_CLEAR_ALL_W      (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)



/************************************************************************/
/*                       ���ƺ��벿��                                   */
/************************************************************************/
#define  NUM_LIMITINCOMING_HEADER_TOTAL         (20)
/*      �������к���                                                    */
#define  CMD_LIMIT_ALL_INCOMING_CALL_RW         (PHONE_LIMIT_INCOMING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      ���ƺ���Ŷ�,��ʼ��ַ                                           */
#define  CMD_LIMITINCOMING_HEADER_STR_RW        (PHONE_LIMIT_INCOMING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      ���ƺ���Ŷ�,������ַ                                           */
#define  CMD_LIMITINCOMING_HEADER_END_RW        (PHONE_LIMIT_INCOMING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + NUM_LIMITINCOMING_HEADER_TOTAL)




/************************************************************************/
/*                    Pin������                                         */
/************************************************************************/
/*      �����Զ���PIN��                                                 */
#define  CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      ����PIN�볤��                                                   */
#define  CMD_PIN_SETTING_PIN_LEN_RW              (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      ���ó�ʼPIN��                                                   */
#define  CMD_PIN_SETTING_INIT_PIN_RW             (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 2)
/*      �������PIN��                                                   */
#define  CMD_PIN_SETTING_ENCRYPT_PIN_RW          (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 3)
/*      ���ù̶�PIN��                                                   */
#define  CMD_PIN_SETTING_FIX_PIN_RW              (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 4)
/*      �Ƿ�ʹ�����PIN��                                               */
#define  CMD_PIN_SETTING_USE_ENCRYPT_PIN_RW      (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 5)




/************************************************************************/
/*                       ������������Ӫ������                           */
/************************************************************************/
/*      ������SIM������                                               */
#define  CMD_LOCK_SIM_BAND_TO_HANDSET_RW          (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      ������SIM����(imsi)                                           */
#define  CMD_LOCK_SIM_LOCKED_IMSI_RW              (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      �����Ƿ�������Ӫ��                                              */
#define  CMD_LOCK_SIM_SP_LOCK_ENABLE_RW           (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 2)
/*      ����������Ӫ�̺�                                                */
#define  CMD_LOCK_SIM_LOCKED_SERVICE_PROVIDER_RW  (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 3)



/************************************************************************/
/*                        ��С������                                    */
/************************************************************************/
/*      �Ƿ���С��                                                      */
#define  CMD_LOCK_CELL_LOCK_ENABLE_RW             (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      ����С������Ŀ                                                  */
#define  CMD_LOCK_CELL_LOCKED_CELL_NUM_RW         (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      �Ƿ��Զ���С��                                                  */
#define  CMD_LOCK_CELL_AUTO_LOCK_ENABLE_RW        (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 2)
/*      �Ѿ�������С����                                                */
#define  CMD_LOCK_CELL_INFO_RW                    (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 3)

#if defined(LOCK_LAC)
/*      һ��atָ���Ĵ�������Ŀ                                      */
#define  LAC_DOZEN_ONE_AT_TOTAL                   (25) 
/*      ������atָ���������                                          */
#define  LAC_DOZEN_COUNT_AT_TOTAL                 (4) 
/*      �������ܸ���                                                    */
#define  LAC_TOTAL                                (LAC_DOZEN_ONE_AT_TOTAL*LAC_DOZEN_COUNT_AT_TOTAL)
/*      �Ƿ�������                                                      */
#define  CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW         (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 4)
/*      ������������Ŀ                                                  */
#define  CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW     (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 5)
/*      �Ѿ������Ĵ�����,��ʼ��ַ                                       */
#define  CMD_LOCK_CELL_LAC_INFO_STR_RW            (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 6)
/*      �Ѿ������Ĵ�����,��ʼ��ַ                                       */
#define  CMD_LOCK_CELL_LAC_INFO_END_RW            (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 6 + LAC_DOZEN_COUNT_AT_TOTAL - 1)
#endif



/************************************************************************/
/*                 Ip ���Ź��򲿷�                                      */
/************************************************************************/
#define  NUM_IP_HEADER_TATOL                     (40)
/*      Ip ���Ź���,��ʼ��ַ                                            */
#define  CMD_IP_SETTING_STR_RW                   (PHONE_IPHEDER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      Ip ���Ź���,������ַ                                            */
#define  CMD_IP_SETTING_END_RW                   (PHONE_IPHEDER_SETTING*CMD_PHONE_SETTING_BASE + NUM_IP_HEADER_TATOL)

/*      �������Ip ���Ź���                                             */
#define  CMD_IP_SETTING_CLEAR_ALL_W              (PHONE_IPHEDER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)



/************************************************************************/
/*                       �����ٲ��������ò���                           */
/************************************************************************/
#if defined(INTELLIGENT_SPEED_DIAL)
/*      �����ٲ����������洢����                                      */
#define  NUM_INT_SPEED_DIAL_TOTAL                 (48)
/*      �����ٲ�����ÿ�����ݵĳ���                                      */
#define  NUM_INT_SPEED_DIAL_LEN                   (18)
/*      �����ٲ�����,��ʼ��ַ                                           */
#define  CMD_INTELLIGENT_SPEED_DIAL_STR_RW        (PHONE_INT_SPEED_DIAL_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      �����ٲ�����,������ַ                                           */
#define  CMD_INTELLIGENT_SPEED_DIAL_END_RW        (PHONE_INT_SPEED_DIAL_SETTING*CMD_PHONE_SETTING_BASE + NUM_INT_SPEED_DIAL_TOTAL)

/*      ������������ٲ���������                                        */
#define  CMD_INTELLIGENT_SPEED_DIAL_CLEAR_ALL_W   (PHONE_INT_SPEED_DIAL_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)
#endif

/************************************************************************/
/*                       ���Ⲧ�Ų�������                               */
/************************************************************************/
#if defined(SPECIAL_DIAL)
/*      ���Ⲧ�Ų��������洢����                                      */
#if defined(XMSTART_3IN1_NET_MANAGER)
#define  NUM_SPECIAL_DIAL_TOTAL                    (8)
#else
#define  NUM_SPECIAL_DIAL_TOTAL                    (36)
#endif
/*       ���Ⲧ�Ų���ÿ�����ݵĳ���                                      */
#define  NUM_SPECIAL_DIAL_LEN                      (18)
/*      ���Ⲧ�Ų���,��ʼ��ַ                                           */
#define  CMD_SPECIAL_DIAL_STR_RW                   (PHONE_SPECIAL_DIAL_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      ���Ⲧ�Ų���,������ַ                                           */
#define  CMD_SPECIAL_DIAL_END_RW                   (PHONE_SPECIAL_DIAL_SETTING*CMD_PHONE_SETTING_BASE + NUM_INT_SPEED_DIAL_TOTAL)

/*      ����������Ⲧ�Ų�������                                        */
#define  CMD_ISPECIAL_DIAL_CLEAR_ALL_W             (PHONE_SPECIAL_DIAL_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)
#endif


/************************************************************************/
/*                             ���ʱ����ò���                           */
/************************************************************************/
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
/*      ���ʱ�����,�շѹ���������                                       */
#define NUM_RATE_SETTING_TABLE_TOTAL             (25)
/*      ���ʱ�����,�շѹ���ÿ�����ֽ��� -- 10�ֽ�,����20�ַ�            */
#define NUM_RATE_SETTING_ITEM_LENGTH             (20)

/*      ���ʱ�����,�����ļ���֡��                                       */
#define  NUM_RATE_FRAM_HEADER_TOTAL              (23)

/*      ���ʱ�����,У����ƫ�Ƶ�ַ                                       */
#define  RATE_FRAM_DOWNLOAD_VERIFY_CODE          (800)

/*      �Ʒ���������,�Ŷ�������                                         */
#define NUM_RATE_SETTING_HEADER_TYPE_TOTAL       (127)
/*      �Ʒ���������,�Ʒѹ���ÿ�����ֽ��� --  ����13+2=15�ַ�           */
#define NUM_RATE_SETTING_ITEM_HEADER_LENGTH      (13)
#define NUM_RATE_SETTING_ITEM_TYPE_LENGTH        (2)
#define NUM_RATE_SETTING_ITEM_HEADER_TYPE_LENGTH (NUM_RATE_SETTING_ITEM_HEADER_LENGTH + NUM_RATE_SETTING_ITEM_TYPE_LENGTH)

/*      �����ʱ�����,��ʼ��ַ                                           */
#define CMD_MAJOR_RATE_SETTING_STR_RW            (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      �����ʱ�����,������ַ                                           */
#define CMD_MAJOR_RATE_SETTING_END_RW            (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + NUM_RATE_SETTING_TABLE_TOTAL)

/*      �����ʱ�����,����ʱ��                                           */
#define CMD_MAJOR_RATE_SETTING_START_TIME_RW     (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 26)

/*      �����ʱ�����, �����ļ� Id                                       */
#define CMD_MAJOR_RATE_SETTING_FILE_ID_RW        (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 27)

/*      �����ʱ�����, ���ʺŶμ�¼����                                  */
#define CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R      (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 28)

/*      �����ʱ�����, ���ʱ��¼����                                    */
#define CMD_MAJOR_RATE_SETTING_RATE_COUNT_R      (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 29)

/*      �����ʱ�����, �����ļ�֡���                                    */
#define CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R       (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 30)

/*      �����ʱ�����,���ʺŶ���ʼ��ַ                                   */
#define CMD_MAJOR_RATE_HEADER_TYPE_STR_RW        (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 40)
/*      �����ʱ�����,���ʺŶν�����ַ                                   */
#define CMD_MAJOR_RATE_HEADER_TYPE_END_RW        (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 40 + NUM_RATE_SETTING_HEADER_TYPE_TOTAL)

/*      �����ʱ�����,�����ļ�У����                                     */
#define CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R    (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + RATE_FRAM_DOWNLOAD_VERIFY_CODE)


/*      �η��ʱ��൱�������ʵ�ƫ�Ƶ�ַ                                  */
#define MINOR_RATE_OFFSET_MAJOR                  (170)

/*      �η��ʱ�����,��ʼ��ַ                                           */
#define CMD_MINOR_RATE_SETTING_STR_RW            (CMD_MAJOR_RATE_SETTING_STR_RW + MINOR_RATE_OFFSET_MAJOR)
/*      �η��ʱ�����,������ַ                                           */
#define CMD_MINOR_RATE_SETTING_END_RW            (CMD_MAJOR_RATE_SETTING_END_RW + MINOR_RATE_OFFSET_MAJOR)

/*      �η��ʱ�����,����ʱ��                                           */
#define CMD_MINOR_RATE_SETTING_START_TIME_RW     (CMD_MAJOR_RATE_SETTING_START_TIME_RW + MINOR_RATE_OFFSET_MAJOR)

/*      �η��ʱ�����, �����ļ� Id                                       */
#define CMD_MINOR_RATE_SETTING_FILE_ID_RW        (CMD_MAJOR_RATE_SETTING_FILE_ID_RW + MINOR_RATE_OFFSET_MAJOR)

/*      �η��ʱ�����, ���ʺŶμ�¼����                                  */
#define CMD_MINOR_RATE_SETTING_HEAD_COUNT_R      (CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R + MINOR_RATE_OFFSET_MAJOR)

/*      �η��ʱ�����, ���ʱ��¼����                                    */
#define CMD_MINOR_RATE_SETTING_RATE_COUNT_R      (CMD_MAJOR_RATE_SETTING_RATE_COUNT_R + MINOR_RATE_OFFSET_MAJOR)

/*      �η��ʱ�����, �����ļ�֡���                                    */
#define CMD_MINOR_RATE_SETTING_FRAM_FLAG_R       (CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R + MINOR_RATE_OFFSET_MAJOR)

/*      �η��ʱ�����,���ʺŶ���ʼ��ַ                                   */
#define CMD_MINOR_RATE_HEADER_TYPE_STR_RW        (CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + MINOR_RATE_OFFSET_MAJOR)
/*      �η��ʱ�����,���ʺŶν�����ַ                                   */
#define CMD_MINOR_RATE_HEADER_TYPE_END_RW        (CMD_MAJOR_RATE_HEADER_TYPE_END_RW + MINOR_RATE_OFFSET_MAJOR)

/*      �η��ʱ�����,�����ļ�У����                                     */
#define CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R    (CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R + MINOR_RATE_OFFSET_MAJOR)

/*      ������з�������                                                */
#define CMD_RATE_SETTING_CLEAR_ALL_W             (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)

#endif



/************************************************************************/
/*        PHONE_MOBILE_HEAD_SETTING �ƶ��绰�Ŷ�ͷ                      */
/************************************************************************/
#if defined(ENABLE_MOBILE_HEAD)
/*                             �Ŷ�ͷ����                               */
#define MOBILE_HEAD_TOTAL                        (50)
/*                             �Ŷ�ͷ���� 4�ַ�                         */
#define MOBILE_HEAD_LEN                          (4)
/*      �ƶ��绰�Ŷ�ͷ,��ʼ��ַ                                         */
#define CMD_MOBILE_HEAD_STR_RW                   (PHONE_MOBILE_HEAD_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      �ƶ��绰�Ŷ�ͷ,������ַ                                         */
#define CMD_MOBILE_HEAD_END_RW                   (PHONE_MOBILE_HEAD_SETTING*CMD_PHONE_SETTING_BASE + MOBILE_HEAD_TOTAL)

/*      ��������ƶ��绰�Ŷ�ͷ                                          */
#define CMD_MOBILE_HEAD_CLEAR_ALL_W              (PHONE_MOBILE_HEAD_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)
#endif


/*        AT+RPAR=CMD�ķ���                                             */
#define  STR_READ_PARA_RESPONSE             "+RPAR"



/************************************************************************/
/*                             д�����Ŀ���                             */
/************************************************************************/
#define CMD_SAVE_SETTING_STOP_W                  (9995)
#define CMD_SAVE_SETTING_START_W                 (9996)
#define CMD_RECOVER_DEFAULT_SETTING_W            (9997)
#define CMD_SAVE_SETTING_FACTORY_W               (9998)
#define CMD_SAVE_SETTING_SAVENV_W                (9999)


/************************************************************************/
/*        ATCMD ���ֵ�ATָ���ʽ                                        */
/************************************************************************/
/*                           ��Ƶ����                                   */
/************************************************************************/
/* Read Command: AT+CMIC? 
Response
+ CMIC: < gainlevel(Main_Mic) >, <gainlevel(Aux_Mic)>
OK                                                                      */
/************************************************************************/
#define  STR_CMIC0_CMD						"AT+CMIC0"  /* ������ͨ����˷�����   */
#define  STR_CMIC1_CMD						"AT+CMIC1"  /* ���ø���ͨ����˷����� */

#define  STR_READ_CMIC_RESPONSE             "+ CMIC:"   /* ͨ�����淵�ؿ�ʼ       */

#if defined(R16_ATC)
/************************************************************************/
/* Read Command :AT+SIDET?
Response:
+SIDET(NORMAL_AUDIO): <gainlevel>
OK
+SIDET(AUX_AUDIO): <gainlevel>
OK																		*/
/************************************************************************/
#define  STR_SIDET0_CMD						"AT+SIDET0" /* ��������������         */
#define  STR_SIDET1_CMD						"AT+SIDET1" /* ���ø���������         */

#define  STR_READ_SIDET0_RESPONSE           "+SIDET(NORMAL_AUDIO):"/* ����������  */
#define  STR_READ_SIDET1_RESPONSE           "+SIDET(AUX_AUDIO):"   /* ����������  */
#else

/************************************************************************/
/* Read Command :AT+SIDET?
Response:
+ SIDET: < gainlevel>
OK																		*/
/************************************************************************/
#define  STR_SIDET_CMD						"AT+SIDET"  /* ���ò�������           */
#endif

#define  STR_READ_SIDET_RESPONSE            "+ SIDET:"   /* �������淵�ؿ�ʼ      */


/************************************************************************/
/* Read Command:AT+ECHO?
Response:
+ECHO(NORMAL_AUDIO):
<MAINvoxGain>,<MAINminMicEnergy>,<MAINsampSlncePrd>
+ECHO(AUX_AUDIO):
<AUXvoxGain>,<AUXminMicEnergy>,<AUXsampSlncePrd>
OK                                                                      */
/************************************************************************/
#define  STR_ECHO0_CMD						"AT+ECHO0"  /* ������ͨ���ز�����     */
#define  STR_ECHO1_CMD						"AT+ECHO1"  /* ���ø���ͨ���ز�����   */

#define  STR_READ_ECHO_RESPONSE             "+ECHO"     /* �ز����Ʒ��ؿ�ʼ       */
#define  STR_READ_ECHO0_RESPONSE            "+ECHO(NORMAL_AUDIO):" /* ECHO0������ */
#define  STR_READ_ECHO1_RESPONSE            "+ECHO(AUX_AUDIO):"    /* ECHO1������ */



/*                       ����Lcd�ԱȶȲ���                                        */
/************************************************************************/
/* Read Command:AT+CTRS?
Response:
+CTRS:26

OK                                                                     */
/************************************************************************/
#define  STR_CTRS_CMD						"AT+CTRS"   /* ����Һ�����Աȶ�       */
#define  STR_READ_CTRS_RESPONSE             "+CTRS:"    /* ���Աȶȷ��ؿ�ʼ       */

/*                       ATCMD���ֵ�ָ�������                           */
#if defined(R16_ATC)
#define ATCMD_TOTAL                         (7)
#else
#define ATCMD_TOTAL                         (6)
#endif


#endif/*PARA_3IN1_DEF_H*/
