#if !defined(AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)
#define AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyHeader.h : header file

#define		INT_INIT_LISTBOX_ITEMNUM        20
#if defined(PAGE_CFG)
extern unsigned int getPageCount(unsigned int  nPageId);
#define		INT_INIT_LOCAL_PHONE_ITEMNUM    getPageCount(IDD_PROPPAGE_5_LOCAL_MOBILESECT)

extern BOOL getPageEnableStatus(unsigned int  nPageId);
#define     ENABLE_EMERGENCY_CALL           getPageEnableStatus(IDD_PROPPAGE_EMERGENCY_CALL)
#define     ENABLE_OPN_CFG                  getPageEnableStatus(IDD_PROPPAGE_OPN_CFG)
#else
#if defined(LP_HEADER30)
#define		INT_INIT_LOCAL_PHONE_ITEMNUM    30
#else
#define		INT_INIT_LOCAL_PHONE_ITEMNUM    400
#endif
#endif


#define     INT_PAGE_BASE_NODE				0
#define     STRING_PAGELIMIT_NAME           "PAGELIMITOPTION"
#define		STRING_SECTION1_NAME			"PARA"
#define		STRING_SECTION2_NAME			"ATCMD"
#if defined(PAGE_CFG)
#define     STRING_SECTION3_NAME            "PAGECFG"
#endif
#define		STRING_PARAFILE_NAME		    "parafile.ini"


// --------------------------------------------------------------------//
// --- ALL COMMAND USED  --- 
// --------------------------------------------------------------------//

// ---		dialog -> SYS				    --- 
#define  INT_AUTO_DELAY_CMD					2		// 速延时时间
#define  INT_SMS_CENTER_CMD					4		// 短消息中心号码
#define  INT_IF_ENABLE_LD_CALL_CMD			5		// 是否允许国内长途
#define  INT_IF_ADD_ZERO_FOR_EXTR_PHONE_CMD 4000    //外地手机是否加零


#if defined(CXT_EX)
#define   INT_G166_IPADDRESS_CMD   (981)

#define   INT_G166_IPPORT_CMD      (982)

#define   INT_G166_DNS1_CMD        (983)

#define   INT_G166_DNS2_CMD        (984)
#endif

#define  INT_SCROLL_HOTLINE_CMD 			1001    //客服热线号码
#define  INT_SCROLL_NXTNUM_CMD 		    	1002    //农信通号码
#define  INT_SCROLL_MOBILESEC_CMD 		   	1003    //移动秘书号码
#define  INT_SCROLL_ISSUANCENUM_CMD 		1004    //滚动条发布号码
#define  INT_LIMIT_MSGAD_CMD                1020    //短信头限制
#define  INT_SCROLL_NXTMENUTITLE_CMD 		13000   //农信通菜单标题
#define  INT_SCROLL_MATTER_CMD 		        13001   //滚动条内容
#define  INT_SCROLL_MATTERAD_CMD 		    13002   //滚动条字头
#define  COUNT_SCROLL_ISSUANCENUM           7       //滚动条发布号码个数
#define  COUNT_LIMIT_MSG_AD                 40      //短信头限制个数

// ---		dialog -> LOCAL				    --- 
#define  INT_LOCAL_ZONE_CMD 				1000    //本地区号
#define  INT_LOCAL_METHOD_CMD				3       // 本地区号使用方式

// ---		dialog -> PINSIM			    ---
#define	 INT_IF_AUTO_LOCK_PIN_CMD           6000    //读是否自动锁PIN码
#define	 INT_SET_PIN_LENGTH_CMD             6001    //读设置PIN码长度
#define  INT_SET_INIT_PIN_CODE_CMD          6002    //读设置初始PIN码
#define	 INT_SET_FIXED_PIN_CODE_CMD         6004    //读设置固定PIN码
#define	 INT_IF_USE_RANDOM_PIN_CMD          6005    //读是否使用随机PIN码

#define  INT_IF_BIND_SIM_CARD_CMD			7000	//读话机是否绑定SIM卡
#define	 INT_BIND_SIM_CARD_NUMBER_CMD       7001    //读话机绑定的SIM卡号

// ---		dialog -> LOCK			        ---
#define	 INT_IF_LOCK_COMPANY_CMD            7002    //读是否锁定运营商
#define	 INT_LOCKED_COMPANY_CMD             7003    //读锁定的运营商

#if defined (LOCK_SERVICE_PROVIDER)
#define  SERVICE_PROVIDER_MAX               (32)
#endif

#define  INT_IF_LOCK_DISTRICT_CMD           8000    //读是否锁小区
#define	 INT_IF_AUTO_LOCK_DISTRICT_CMD      8002    //读是否自动锁小区

#define  INT_LOCKED_DISTRICT_NUMBER_CMD     8001	//读锁定的小区数
#define	 INT_LOCKED_DISTRICT_CODE_CMD		8003    //读已经锁定的小区号


#define  INT_IF_LOCK_LAC_CMD                8004    // read if Lock the Lac
#define  INT_LOCKED_LAC_CODE_CMD            8005    // read the first half of the maxium locked Lac
#define  INT_LOCKED_LAC_CODE_CMD2           8006     //  read the second half of the maxium locked lac

// ---		dialog -> Direct Table			---
#define	 INT_DIRECT_NUMBER_TABLE_CMD		2001    //直拨号码

//#if defined(NETWORK_CONFIG)
#define  EMERGENCY_HEAD_LEN                (3)     // 每个紧急号码长度为3Bcd
#define  EMERGENCY_NUM_INT                 (10)    // 总共有 10 个 
#define  CLEAR_EMERGENCY_SETTING           (2600)  // 清楚紧急号码命令
#define  INT_EMERGENCY_CALL_CMD            (2601)  // 读紧急号码起始

#define  OPN_LEN                           (18)    // 每个运营商名长度为 18 Ucs2
#define  OPN_TOTAL                         (50)    // 总共有 50 个
#define  CLEAR_OPN_CFG                     (2700)  // 清除运营商命令
#define  INT_OPERATION_NAME_CMD            (2701)   // 读运营商名起始
//#endif

// ---		dialog -> Forbid Table			---
#define	 INT_FORBID_NUMBER_TABLE_CMD		3001    //禁拨号码

// ---		dialog -> REFUSE			    ---
#define	 INT_REFUSE_TYPE_CMD				5000	//是否限制呼入所有来电
#define  INT_REFUSE_NUMBER_TABLE_CMD		5001    //读限制呼入号码段

// ---		dialog -> Local Mobile Sect	    ---
#define  INT_LOCAL_SEGMENT_TABLE_CMD		4001    //本地手机号码段

// ---		dialog -> Local User Mobile Sect	    ---
#define  INT_LOCAL_USER_SEGMENT_TABLE_CMD	(INT_LOCAL_SEGMENT_TABLE_CMD + INT_INIT_LOCAL_PHONE_ITEMNUM)    //用户自定义本地手机号码段

// ---		dialog -> IP Rule Table         ---
#define	 INT_IP_RULE_TABLE_CMD              9001	//IP规则设置


#if defined(ENABLE_MOBILE_HEAD)
#define INT_MOBILE_HEADER_STR_CMD           (14001)
#define MAX_MOBILE_HEADER_TOTAL             (50)
#define MOBILE_HEADER_ITEM_LEN              (4)
#define MOBILE_HEADER_CLEAR_CMD             (14900)
#endif

// ---		others							---
#define  INT_CHANGE_PWD_CMD					1       // 修改超级密码 
#define	 INT_RECOVER_DEFAULT_SETTING_CMD    9997    //恢复默认设置
#define  INT_SAVE_TO_DEFAULT_SETTING_CMD    9998    //保存为默认设置
#define  INT_SAVE_SETTING_CMD               9999    //保存设置

// ---		Mic   							---
#define  STR_CMIC0_CMD						"AT+CMIC0" // 设置主通道麦克风增益
#define  STR_CMIC1_CMD						"AT+CMIC1" // 设置辅助通道麦克风增益
#if defined(R16_ATC)
#define  STR_SIDET0_CMD						"AT+SIDET0" // 设置主侧音增益
#define  STR_SIDET1_CMD						"AT+SIDET1" // 设置辅侧音增益
#else
#define  STR_SIDET_CMD						"AT+SIDET" // 设置侧音增益
#endif
#define  STR_ECHO0_CMD						"AT+ECHO0" // 设置主通道回波抑制
#define  STR_ECHO1_CMD						"AT+ECHO1" // 设置辅助通道回波抑制

// ---		Lcd   							---
#define  STR_CTRS_CMD						"AT+CTRS"  // 设置液晶屏对比度

#if defined(PAGE_CFG)
#define  STR_PAGECFG_ENABLE                 "PageEnable" // 面页可用
#define  STR_PAGECFG_COUNTEC                "CountEc"    // 紧急号码的数量(该项未使用)
#define  STR_PAGECFG_COUNTOC                "CountOc"    // 网络名的数量(该项未使用)
#define  STR_PAGECFG_COUNTLM                "CountLm"    // 本地号段的数量
#endif

#ifdef  _XIAMEN_START_BUILD
// ---		dialog -> Xiamen Para Setting         ---
	#define  INT_MSC_RECEIVER_CMD				6		// 接收网管中心号码
	#define  INT_IF_SHOW_ORIGINAL_NUM_CMD		7		// 是否只显示原始号码
	#define  INT_PRODUCER_ID_CMD				9		// 生产商ID信息
	#define  INT_IF_LOCAL_SECT_ENABLED_CMD		11		// 本地手机号段是否生效
#endif  //_XIAMEN_START_BUILD

#ifdef  _YINGNUO_SECURITY_BUILD
// ---		dialog -> Yingnuo Security Setting         ---
	#define  INT_SECURITY_SETTING1_CMD			20		// 第1组密钥
	#define  INT_SECURITY_SETTING2_CMD			21		// 第2组密钥
	#define  INT_SECURITY_SETTING3_CMD			22		// 第3组密钥
	#define  INT_SECURITY_SETTING4_CMD			23		// 第4组密钥
	#define  INT_SECURITY_SETTING5_CMD			24		// 第5组密钥
#endif  //_YINGNUO_SECURITY_BUILD

#ifdef  _TONGZE_PARA_BUILD
// ---		dialog -> Tongze Para Setting         ---
	#define  INT_PRODUCER_ID_CMD				9		// 生产商ID信息
	#define  INT_IF_LOCAL_SECT_ENABLED_CMD		11		// 本地手机号段是否生效
#endif  //_TONGZE_PARA_BUILD

// --------------------------------------------------------------------//
// --- From [EditListCtrl] to [parent dialog] --- 
// --------------------------------------------------------------------//
#define		WM_MSG_EDIT_END_NOTIFY						WM_USER+ 100

// --------------------------------------------------------------------//
// --- From [Data Dialog] to [App dialog] --- 
// --------------------------------------------------------------------//
#define		WM_MSG_LOAD_USER_INI_NOTIFY					WM_USER+ 200
#define		WM_MSG_SAVE_USER_INI_NOTIFY					WM_USER+ 201

#define		WM_MSG_LOAD_MODULE_SETTING_NOTIFY			WM_USER+ 202
#define		WM_MSG_SAVE_MODULE_SETTING_NOTIFY			WM_USER+ 203

#define		WM_MSG_SAVE_MODULE_DEFAULT_NOTIFY			WM_USER+ 204
#define		WM_MSG_RECOVER_MODULE_DEFAULT_NOTIFY		WM_USER+ 205

#define		WM_MSG_CHANGE_PWD_NOTIFY					WM_USER+ 206
#define		WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY		WM_USER+ 207
#define		WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY		WM_USER+ 208

#define		WM_MSG_ACTIVE_THIS_PAGE_NOTIFY				WM_USER+ 209
// added by Huang ye for activate commport
#define     WM_MSG_ACTIVATE_COMMPORT_NOTIFY             WM_USER+ 210
// added by Huang ye for activate commport

#define		WM_MSG_MAIN_WND_TREE_CONTROL_NOTIFY	        WM_USER+ 211

#if defined(PAGE_CFG)
typedef struct PageEnableTag{
	unsigned int  nPageId;
	BOOL          bEnable;
	unsigned int  nCount;
}PageEnable;
#endif
#endif // !defined(AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)
