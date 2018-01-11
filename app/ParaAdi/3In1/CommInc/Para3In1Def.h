/************************************************************************/
/* $Id:                  Para3In1Def.h                                  */
/*    单模 3In1 参数接口命令号定义枚举                                  */
/*    huangqr 2008-12-06 14:05 Add                                      */
/************************************************************************/
#if !defined(PARA_3IN1_DEF_H)
#define PARA_3IN1_DEF_H

/************************************************************************/
/*                         工程编译选项                                */
/************************************************************************/
#include <BldOpt.h>


/************************************************************************/
/*                          参数类型枚举                                */
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
/*             操作类型枚举                                             */
/************************************************************************/

#define CMD_PHONE_SETTING_BASE                  (1000)
/*             串口清空列表设置                                         */
#define CLEAR_PHONE_SETTING                     (900)



/************************************************************************/
/*                        系统特征部分                                  */
/************************************************************************/
/*       超级密码                                                       */
#define  CMD_SUPER_PASSWORD_W                   (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 1)

/*       速拨延时                                                       */
#define  CMD_DIAL_WAITING_TIME_RW               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 2)

/*       本地区号使用方式                                               */
#define  CMD_HOW_TO_USE_CITY_NUMBER_RW          (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 3)

/*       网管中心号码                                                   */
#define  CMD_SMS_CENTER_NUMBER_RW               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 4)

/*       是否禁止拨打长途电话                                           */
#define  CMD_BARING_OTHER_CITY_NUMBER_RW        (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 5)

#if defined(XMSTART_NET_AND_COMMGR_ENABLE)

/*       短信接受中心号码                                               */
#define  CMD_SMS_RECEIVE_NUMBER_RW              (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 6)

/*      显示原始号码                                                    */
#define  CMD_DISPLAY_ORIGINAL_NUMBER_RW         (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 7)

/*      小区信号强度质量信息                                            */
#define  CMD_CELL_LEVEL_QUALITY_R               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 8)

/*      生产商ID信息                                                    */
#define  CMD_CLIENT_ID_RW                       (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 9)
#endif/* XMSTART_NET_AND_COMMGR_ENABLE */


/*      本地手机号段是否有效                                            */
#define  CMD_LOCAL_NUMBER_ACTIVE_RW              (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 11)

#if defined (ICCID_BUILD)
/*      Imei和Ccid串号信息                                             */
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

/*      MIDI芯片驱动类型信息                                            */
#define  CMD_SIMCOM_MIDI_DRIVER_R                (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 13)


/*      MIDI铃音总数                                                    */
#define  CMD_SIMCOM_MIDI_TOTAL_R                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 14)
#endif/* SIMCOM_MIDI_DRIVER */


#if defined(SMS_MENU_SELECT)
/*      是否显示短消息菜单                                              */
#define  CMD_SMS_MENU_DISPLAY_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 15)
#endif/* SMS_MENU_SELECT */


#if defined(ENABLE_SHORT_NUMBER_FUNCTION)
/*      是否使用短号功能                                                */
#define  CMD_ENABLE_SHORT_NUMBER_FUNCTION_RW     (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 16)
#endif

#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
/*      Sim卡厂商ID信息                                                 */
#define  CMD_SIM_ENCRYPT_FACTORYID_RW            (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 17)

/*      是否兼容普通Sim卡                                               */
#define  CMD_SIM_ENCRYPT_COMPATIBLE_NORMAL_RW    (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 18)

/*      加密卡方案                                                      */
#define  CMD_SIM_ENCRYPT_SOLUTION_RW             (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 19)

/*      SIM版本信息                                                     */
#define  CMD_SIM_ENCRYPT_EDITION_ID_RW           (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 31)

/*      加密卡的五组密钥                                                */
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
/*      开机问候语显示Id                                                */
#define  CMD_CONFIG_GREET_RW                     (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 25)
#endif/*CONFIG_GREET*/


#if defined(XMSTART_3IN1_NET_MANAGER)
/*      智能拨号开关                                                    */
#define  CMD_SWITCH_INTELLIGENT_DAIL_RW          (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 26)

/*      自动锁屏开关                                                    */
#define  CMD_SWITCH_AOTU_LOCK_RW                 (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 27)

/*      收费提示音开关                                                  */
#define  CMD_SWITCH_TOLL_INDICTION_RW            (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 28)

/*      特殊应用开关                                                    */
#define  CMD_SWITCH_SPECIAL_APP_RW               (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 29)

/*      用户标识信息                                                    */
#define  CMD_USER_TAG_RW                         (PHONE_NORMAL_SETTING*CMD_PHONE_SETTING_BASE + 30)
#endif/* XMSTART_3IN1_NET_MANAGER */


/************************************************************************/
/*                        本地区号部分                                  */
/************************************************************************/
/*      本地区号                                                        */
#define  CMD_CITY_NUMBER_RW                      (PHONE_CITY_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 0)
#if defined(XMSTART_OPT_DIALFLOW)
#define  CITY_NUMBER_LEN                         (17)
#else
#define  CITY_NUMBER_LEN                         (6)
#endif



/************************************************************************/
/*                        直拨号段部分                                  */
/************************************************************************/
#define  NUM_HOT_NUMBER_TOTAL                    (36)
/*      直拨号段,起始地址                                               */
#define  CMD_HOT_NUMBER_STR_RW                  (PHONE_HOT_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      直拨号段,结束地址                                               */
#define  CMD_HOT_NUMBER_END_RW                  (PHONE_HOT_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + NUM_HOT_NUMBER_TOTAL)
/*      清空所有直拨号段                                                */
#define  CMD_HOT_NUMBER_CLEAR_ALL_W             (PHONE_HOT_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)


/************************************************************************/
/*                        禁拨号段部分                                  */
/************************************************************************/
#define  NUM_BARRING_NUMBER_TOTAL                (36)
/*      禁拨号段,起始地址                                               */
#define  CMD_BARRING_NUMBER_STR_RW              (PHONE_BARRING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      禁拨号段,结束地址                                               */
#define  CMD_BARRING_NUMBER_END_RW              (PHONE_BARRING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + NUM_BARRING_NUMBER_TOTAL)
/*      清空所有禁拨号段                                                */
#define  CMD_BARRING_NUMBER_CLEAR_ALL_W         (PHONE_BARRING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)



/************************************************************************/
/*                        本地号码段部分                                */
/************************************************************************/
/*      拨长途自动加 '0'                                                */
#define  CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW      (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + 0)
#if defined(XMSTART_3IN1_NET_MANAGER)
#define  NUM_LOCALPHONE_HEADER_TOTAL            (400 + 200)
#define  LOCALPHONE_HEADER_LEN                  (8)
#else
#define  NUM_LOCALPHONE_HEADER_TOTAL            (400 + 20)
#define  LOCALPHONE_HEADER_LEN                  (16)
#endif
/*      本地号段,起始地址                                               */
#define  CMD_LOCALPHONE_HEADER_STR_RW           (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      本地号段,结束地址                                               */
#define  CMD_LOCALPHONE_HEADER_END_RW           (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + NUM_LOCALPHONE_HEADER_TOTAL)

/*      清空所有本地号段                                                */
#define  CMD_LOCALPHONE_NUMBER_CLEAR_ALL_W      (PHONE_LOCAL_NUMBER_HEADER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)



/************************************************************************/
/*                       限制呼入部分                                   */
/************************************************************************/
#define  NUM_LIMITINCOMING_HEADER_TOTAL         (20)
/*      限制所有呼入                                                    */
#define  CMD_LIMIT_ALL_INCOMING_CALL_RW         (PHONE_LIMIT_INCOMING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      限制呼入号段,起始地址                                           */
#define  CMD_LIMITINCOMING_HEADER_STR_RW        (PHONE_LIMIT_INCOMING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      限制呼入号段,结束地址                                           */
#define  CMD_LIMITINCOMING_HEADER_END_RW        (PHONE_LIMIT_INCOMING_NUMBER_SETTING*CMD_PHONE_SETTING_BASE + NUM_LIMITINCOMING_HEADER_TOTAL)




/************************************************************************/
/*                    Pin码设置                                         */
/************************************************************************/
/*      设置自动锁PIN码                                                 */
#define  CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      设置PIN码长度                                                   */
#define  CMD_PIN_SETTING_PIN_LEN_RW              (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      设置初始PIN码                                                   */
#define  CMD_PIN_SETTING_INIT_PIN_RW             (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 2)
/*      设置随机PIN码                                                   */
#define  CMD_PIN_SETTING_ENCRYPT_PIN_RW          (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 3)
/*      设置固定PIN码                                                   */
#define  CMD_PIN_SETTING_FIX_PIN_RW              (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 4)
/*      是否使用随机PIN码                                               */
#define  CMD_PIN_SETTING_USE_ENCRYPT_PIN_RW      (PHONE_PIN_SETTING*CMD_PHONE_SETTING_BASE + 5)




/************************************************************************/
/*                       锁机锁卡锁运营商设置                           */
/************************************************************************/
/*      话机绑定SIM卡设置                                               */
#define  CMD_LOCK_SIM_BAND_TO_HANDSET_RW          (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      话机绑定SIM卡号(imsi)                                           */
#define  CMD_LOCK_SIM_LOCKED_IMSI_RW              (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      话机是否锁定运营商                                              */
#define  CMD_LOCK_SIM_SP_LOCK_ENABLE_RW           (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 2)
/*      话机锁定运营商号                                                */
#define  CMD_LOCK_SIM_LOCKED_SERVICE_PROVIDER_RW  (PHONE_LOCK_SIM_SETTING*CMD_PHONE_SETTING_BASE + 3)



/************************************************************************/
/*                        锁小区设置                                    */
/************************************************************************/
/*      是否锁小区                                                      */
#define  CMD_LOCK_CELL_LOCK_ENABLE_RW             (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 0)
/*      锁定小区号数目                                                  */
#define  CMD_LOCK_CELL_LOCKED_CELL_NUM_RW         (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      是否自动锁小区                                                  */
#define  CMD_LOCK_CELL_AUTO_LOCK_ENABLE_RW        (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 2)
/*      已经锁定的小区号                                                */
#define  CMD_LOCK_CELL_INFO_RW                    (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 3)

#if defined(LOCK_LAC)
/*      一条at指令处理的大区号数目                                      */
#define  LAC_DOZEN_ONE_AT_TOTAL                   (25) 
/*      大区号at指令处理总条数                                          */
#define  LAC_DOZEN_COUNT_AT_TOTAL                 (4) 
/*      大区号总个数                                                    */
#define  LAC_TOTAL                                (LAC_DOZEN_ONE_AT_TOTAL*LAC_DOZEN_COUNT_AT_TOTAL)
/*      是否锁大区                                                      */
#define  CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW         (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 4)
/*      锁定大区号数目                                                  */
#define  CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW     (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 5)
/*      已经锁定的大区号,起始地址                                       */
#define  CMD_LOCK_CELL_LAC_INFO_STR_RW            (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 6)
/*      已经锁定的大区号,起始地址                                       */
#define  CMD_LOCK_CELL_LAC_INFO_END_RW            (PHONE_LOCK_CELL_SETTING*CMD_PHONE_SETTING_BASE + 6 + LAC_DOZEN_COUNT_AT_TOTAL - 1)
#endif



/************************************************************************/
/*                 Ip 拨号规则部分                                      */
/************************************************************************/
#define  NUM_IP_HEADER_TATOL                     (40)
/*      Ip 拨号规则,起始地址                                            */
#define  CMD_IP_SETTING_STR_RW                   (PHONE_IPHEDER_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      Ip 拨号规则,结束地址                                            */
#define  CMD_IP_SETTING_END_RW                   (PHONE_IPHEDER_SETTING*CMD_PHONE_SETTING_BASE + NUM_IP_HEADER_TATOL)

/*      清空所有Ip 拨号规则                                             */
#define  CMD_IP_SETTING_CLEAR_ALL_W              (PHONE_IPHEDER_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)



/************************************************************************/
/*                       智能速拨参数设置部分                           */
/************************************************************************/
#if defined(INTELLIGENT_SPEED_DIAL)
/*      智能速拨参数的最大存储条数                                      */
#define  NUM_INT_SPEED_DIAL_TOTAL                 (48)
/*      智能速拨参数每条数据的长度                                      */
#define  NUM_INT_SPEED_DIAL_LEN                   (18)
/*      智能速拨参数,起始地址                                           */
#define  CMD_INTELLIGENT_SPEED_DIAL_STR_RW        (PHONE_INT_SPEED_DIAL_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      智能速拨参数,结束地址                                           */
#define  CMD_INTELLIGENT_SPEED_DIAL_END_RW        (PHONE_INT_SPEED_DIAL_SETTING*CMD_PHONE_SETTING_BASE + NUM_INT_SPEED_DIAL_TOTAL)

/*      清空所有智能速拨参数设置                                        */
#define  CMD_INTELLIGENT_SPEED_DIAL_CLEAR_ALL_W   (PHONE_INT_SPEED_DIAL_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)
#endif

/************************************************************************/
/*                       特殊拨号参数部分                               */
/************************************************************************/
#if defined(SPECIAL_DIAL)
/*      特殊拨号参数的最大存储条数                                      */
#if defined(XMSTART_3IN1_NET_MANAGER)
#define  NUM_SPECIAL_DIAL_TOTAL                    (8)
#else
#define  NUM_SPECIAL_DIAL_TOTAL                    (36)
#endif
/*       特殊拨号参数每条数据的长度                                      */
#define  NUM_SPECIAL_DIAL_LEN                      (18)
/*      特殊拨号参数,起始地址                                           */
#define  CMD_SPECIAL_DIAL_STR_RW                   (PHONE_SPECIAL_DIAL_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      特殊拨号参数,结束地址                                           */
#define  CMD_SPECIAL_DIAL_END_RW                   (PHONE_SPECIAL_DIAL_SETTING*CMD_PHONE_SETTING_BASE + NUM_INT_SPEED_DIAL_TOTAL)

/*      清空所有特殊拨号参数设置                                        */
#define  CMD_ISPECIAL_DIAL_CLEAR_ALL_W             (PHONE_SPECIAL_DIAL_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)
#endif


/************************************************************************/
/*                             费率表设置部分                           */
/************************************************************************/
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
/*      费率表设置,收费规律总条数                                       */
#define NUM_RATE_SETTING_TABLE_TOTAL             (25)
/*      费率表设置,收费规律每条的字节数 -- 10字节,串口20字符            */
#define NUM_RATE_SETTING_ITEM_LENGTH             (20)

/*      费率表设置,费率文件总帧数                                       */
#define  NUM_RATE_FRAM_HEADER_TOTAL              (23)

/*      费率表设置,校验码偏移地址                                       */
#define  RATE_FRAM_DOWNLOAD_VERIFY_CODE          (800)

/*      计费类型设置,号段总条数                                         */
#define NUM_RATE_SETTING_HEADER_TYPE_TOTAL       (127)
/*      计费类型设置,计费规律每条的字节数 --  串口13+2=15字符           */
#define NUM_RATE_SETTING_ITEM_HEADER_LENGTH      (13)
#define NUM_RATE_SETTING_ITEM_TYPE_LENGTH        (2)
#define NUM_RATE_SETTING_ITEM_HEADER_TYPE_LENGTH (NUM_RATE_SETTING_ITEM_HEADER_LENGTH + NUM_RATE_SETTING_ITEM_TYPE_LENGTH)

/*      主费率表设置,起始地址                                           */
#define CMD_MAJOR_RATE_SETTING_STR_RW            (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      主费率表设置,结束地址                                           */
#define CMD_MAJOR_RATE_SETTING_END_RW            (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + NUM_RATE_SETTING_TABLE_TOTAL)

/*      主费率表设置,起跳时间                                           */
#define CMD_MAJOR_RATE_SETTING_START_TIME_RW     (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 26)

/*      主费率表设置, 费率文件 Id                                       */
#define CMD_MAJOR_RATE_SETTING_FILE_ID_RW        (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 27)

/*      主费率表设置, 费率号段记录总数                                  */
#define CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R      (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 28)

/*      主费率表设置, 费率表记录总数                                    */
#define CMD_MAJOR_RATE_SETTING_RATE_COUNT_R      (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 29)

/*      主费率表设置, 费率文件帧标记                                    */
#define CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R       (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 30)

/*      主费率表设置,费率号段起始地址                                   */
#define CMD_MAJOR_RATE_HEADER_TYPE_STR_RW        (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 40)
/*      主费率表设置,费率号段结束地址                                   */
#define CMD_MAJOR_RATE_HEADER_TYPE_END_RW        (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + 40 + NUM_RATE_SETTING_HEADER_TYPE_TOTAL)

/*      主费率表设置,费率文件校验码                                     */
#define CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R    (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + RATE_FRAM_DOWNLOAD_VERIFY_CODE)


/*      次费率表相当于主费率的偏移地址                                  */
#define MINOR_RATE_OFFSET_MAJOR                  (170)

/*      次费率表设置,起始地址                                           */
#define CMD_MINOR_RATE_SETTING_STR_RW            (CMD_MAJOR_RATE_SETTING_STR_RW + MINOR_RATE_OFFSET_MAJOR)
/*      次费率表设置,结束地址                                           */
#define CMD_MINOR_RATE_SETTING_END_RW            (CMD_MAJOR_RATE_SETTING_END_RW + MINOR_RATE_OFFSET_MAJOR)

/*      次费率表设置,起跳时间                                           */
#define CMD_MINOR_RATE_SETTING_START_TIME_RW     (CMD_MAJOR_RATE_SETTING_START_TIME_RW + MINOR_RATE_OFFSET_MAJOR)

/*      次费率表设置, 费率文件 Id                                       */
#define CMD_MINOR_RATE_SETTING_FILE_ID_RW        (CMD_MAJOR_RATE_SETTING_FILE_ID_RW + MINOR_RATE_OFFSET_MAJOR)

/*      次费率表设置, 费率号段记录总数                                  */
#define CMD_MINOR_RATE_SETTING_HEAD_COUNT_R      (CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R + MINOR_RATE_OFFSET_MAJOR)

/*      次费率表设置, 费率表记录总数                                    */
#define CMD_MINOR_RATE_SETTING_RATE_COUNT_R      (CMD_MAJOR_RATE_SETTING_RATE_COUNT_R + MINOR_RATE_OFFSET_MAJOR)

/*      次费率表设置, 费率文件帧标记                                    */
#define CMD_MINOR_RATE_SETTING_FRAM_FLAG_R       (CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R + MINOR_RATE_OFFSET_MAJOR)

/*      次费率表设置,费率号段起始地址                                   */
#define CMD_MINOR_RATE_HEADER_TYPE_STR_RW        (CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + MINOR_RATE_OFFSET_MAJOR)
/*      次费率表设置,费率号段结束地址                                   */
#define CMD_MINOR_RATE_HEADER_TYPE_END_RW        (CMD_MAJOR_RATE_HEADER_TYPE_END_RW + MINOR_RATE_OFFSET_MAJOR)

/*      次费率表设置,费率文件校验码                                     */
#define CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R    (CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R + MINOR_RATE_OFFSET_MAJOR)

/*      清空所有费率设置                                                */
#define CMD_RATE_SETTING_CLEAR_ALL_W             (PHONE_CURRENT_TOLL_RATE_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)

#endif



/************************************************************************/
/*        PHONE_MOBILE_HEAD_SETTING 移动电话号段头                      */
/************************************************************************/
#if defined(ENABLE_MOBILE_HEAD)
/*                             号段头总数                               */
#define MOBILE_HEAD_TOTAL                        (50)
/*                             号段头长度 4字符                         */
#define MOBILE_HEAD_LEN                          (4)
/*      移动电话号段头,起始地址                                         */
#define CMD_MOBILE_HEAD_STR_RW                   (PHONE_MOBILE_HEAD_SETTING*CMD_PHONE_SETTING_BASE + 1)
/*      移动电话号段头,结束地址                                         */
#define CMD_MOBILE_HEAD_END_RW                   (PHONE_MOBILE_HEAD_SETTING*CMD_PHONE_SETTING_BASE + MOBILE_HEAD_TOTAL)

/*      清空所有移动电话号段头                                          */
#define CMD_MOBILE_HEAD_CLEAR_ALL_W              (PHONE_MOBILE_HEAD_SETTING*CMD_PHONE_SETTING_BASE + CLEAR_PHONE_SETTING)
#endif


/*        AT+RPAR=CMD的返回                                             */
#define  STR_READ_PARA_RESPONSE             "+RPAR"



/************************************************************************/
/*                             写操作的开关                             */
/************************************************************************/
#define CMD_SAVE_SETTING_STOP_W                  (9995)
#define CMD_SAVE_SETTING_START_W                 (9996)
#define CMD_RECOVER_DEFAULT_SETTING_W            (9997)
#define CMD_SAVE_SETTING_FACTORY_W               (9998)
#define CMD_SAVE_SETTING_SAVENV_W                (9999)


/************************************************************************/
/*        ATCMD 部分的AT指令格式                                        */
/************************************************************************/
/*                           音频参数                                   */
/************************************************************************/
/* Read Command: AT+CMIC? 
Response
+ CMIC: < gainlevel(Main_Mic) >, <gainlevel(Aux_Mic)>
OK                                                                      */
/************************************************************************/
#define  STR_CMIC0_CMD						"AT+CMIC0"  /* 设置主通道麦克风增益   */
#define  STR_CMIC1_CMD						"AT+CMIC1"  /* 设置辅助通道麦克风增益 */

#define  STR_READ_CMIC_RESPONSE             "+ CMIC:"   /* 通道增益返回开始       */

#if defined(R16_ATC)
/************************************************************************/
/* Read Command :AT+SIDET?
Response:
+SIDET(NORMAL_AUDIO): <gainlevel>
OK
+SIDET(AUX_AUDIO): <gainlevel>
OK																		*/
/************************************************************************/
#define  STR_SIDET0_CMD						"AT+SIDET0" /* 设置主侧音增益         */
#define  STR_SIDET1_CMD						"AT+SIDET1" /* 设置辅侧音增益         */

#define  STR_READ_SIDET0_RESPONSE           "+SIDET(NORMAL_AUDIO):"/* 主侧音增益  */
#define  STR_READ_SIDET1_RESPONSE           "+SIDET(AUX_AUDIO):"   /* 辅侧音增益  */
#else

/************************************************************************/
/* Read Command :AT+SIDET?
Response:
+ SIDET: < gainlevel>
OK																		*/
/************************************************************************/
#define  STR_SIDET_CMD						"AT+SIDET"  /* 设置侧音增益           */
#endif

#define  STR_READ_SIDET_RESPONSE            "+ SIDET:"   /* 侧音增益返回开始      */


/************************************************************************/
/* Read Command:AT+ECHO?
Response:
+ECHO(NORMAL_AUDIO):
<MAINvoxGain>,<MAINminMicEnergy>,<MAINsampSlncePrd>
+ECHO(AUX_AUDIO):
<AUXvoxGain>,<AUXminMicEnergy>,<AUXsampSlncePrd>
OK                                                                      */
/************************************************************************/
#define  STR_ECHO0_CMD						"AT+ECHO0"  /* 设置主通道回波抑制     */
#define  STR_ECHO1_CMD						"AT+ECHO1"  /* 设置辅助通道回波抑制   */

#define  STR_READ_ECHO_RESPONSE             "+ECHO"     /* 回波抑制返回开始       */
#define  STR_READ_ECHO0_RESPONSE            "+ECHO(NORMAL_AUDIO):" /* ECHO0的数据 */
#define  STR_READ_ECHO1_RESPONSE            "+ECHO(AUX_AUDIO):"    /* ECHO1的数据 */



/*                       点阵Lcd对比度参数                                        */
/************************************************************************/
/* Read Command:AT+CTRS?
Response:
+CTRS:26

OK                                                                     */
/************************************************************************/
#define  STR_CTRS_CMD						"AT+CTRS"   /* 设置液晶屏对比度       */
#define  STR_READ_CTRS_RESPONSE             "+CTRS:"    /* 读对比度返回开始       */

/*                       ATCMD部分的指令的总数                           */
#if defined(R16_ATC)
#define ATCMD_TOTAL                         (7)
#else
#define ATCMD_TOTAL                         (6)
#endif


#endif/*PARA_3IN1_DEF_H*/
