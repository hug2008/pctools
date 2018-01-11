#ifndef _DEF_H_
#define _DEF_H_

#define INDEX_INVALID			(-1)
#define LOG_FILE				_T("log.txt")

#define JPUSH_APPKEY			_T("6affd925fa96663e7d9d8750")//_T("5e496db9cfdcd07bb280be7f")
#define JPUSH_MASTER_SECRET		_T("0af689ff95a3022a424ef2e4")//_T("6ebe60aa5b1c933588325352")
#define JPUSH_POST_URL_V3		_T("https://api.jpush.cn/v3/push")


#define JPUSH_RAND_MAX			0xffffffffUL
#define JPUSH_RAND_MIN			0x0UL


#define JPUSH_IOS_NOTIFICATION_BODY_MAX			(2000)
#define JPUSH_IOS_NOTIFICATION_MAX				(2048)
#define JPUSH_IOS_DETAILS_MAX					(58)
#define JPUSH_ALERT_MAX_LEN						(70)

#define ARRAY_SZIE(ary)			((NULL == ary) ? 0 :(sizeof(ary)/sizeof(ary[0])))

#define JPUSH_PLATFORM		_T("platform")
#define PLATFORM_IOS		_T("ios")
#define PLATFORM_ANDROID	_T("android")
#define PLATFORM_WINPHONE	_T("winphone")


#define CONFIG_FILE_NAME		_T("config.ini")
#define LOG_FILE_NAME			_T("log.ini")

typedef enum PushTypeTag//推送类型
{
	PT_NOTIFY
	,PT_MESSAGE
	,PT_BOTH
}PushType;

typedef enum PushRecvTypeTag//接收者类型
{
	RECV_TYPE_ALL
	,RECV_TYPE_ALIAS_TAGS
	,RECV_TYPE_ACCURAT
	,RECV_TYPE_TOTAL
}PushRecvType;
typedef enum PushTagTypeTag//tag类型
{
	TAG_TYPE_ADMIN
	,TAG_TYPE_GENERAL
	,TAG_TYPE_BOTH
	,TAG_TYPE_TOTAL
}PushTagType;
typedef enum PushContentTypeTag//内容类型
{
	PCT_GENERAL
	,PCT_UPGRADE
	,PCT_MARKETING
	,PCT_WARNING
	,PCT_TOTAL
}PushContentType;
typedef enum PushContentTypeWarnningTag//内容警报类型
{
	PCTW_OPEN_DOOR
	,PCTW_CLOSE_DOOR
	,PCTW_PRY_DOOR
	,PCTW_VOICE
	,PCTW_IMAGE
	,PCTW_BATTERY_LOW
	,PCTW_MAKE_ADMIN
	,PCTW_TOTAL
}PushContentTypeWarnning;


typedef enum PushToneTypeTag//声音类型
{
	TONE_DEFAULT                        //默认
	, CHECKIN_FINGER_FAIL               //登记失败请重按手指
	, CHECKIN_INPUT_FAIL                //登记失败请重新输入
	, HOME_NO_PENSON_CALL_MASTER        //对不起，家里暂时没人，请联系主人
	, INPUT_MORE_ERROR_SYSTEM_CLOSE     //多次输入错误，系统已关闭，5分钟后恢复
	, PWD_ERROR_INPUT_AGAIN             //密码错误，请重新输入密码
	, PWD_CHECKIN_SUCCESS               //密码登记成功
	, PWD_CHECKED_IN_CHANGE_PWD         //密码已登记请更换密码
	, PWD_SUCEESS_OPEN_DOOR             //密码正确，请开门
	, PLEASE_PRESS_FINGER               //请按手指
	, PLEASE_OPEN_DOOR                  //请开门
	, DELETE_USER_NO                    //请删除用户编号
	, INPUT_USER_NO                     //请输入用户编号
	, INPUT_PWD                         //请输入用户密码
	, CHECK_ADMIN_FINGER_OR_PWD         //请验证管理员指纹或密码
	, INPUT_PWD_AGAIN                   //请再次输入密码
	, DELETE_SUCCESS                    //删除成功
	, CHECK_SUCCEED_PRESS_HANDLE        //身份已验证，请按下把手开门
	, CHECK_SUCCESS_OPEN_DOOR           //验证成功请开门
	, BELL_RINGS_CHECK_IDENTITY         //有客人正在按响门铃，请确认身分
	, FINGER_CHECKIN_SUCCESS            //指纹登记成功
	, CHECKIN_FAIL_REPEAT_FINGER        //指纹验证失败请重按手指
	, CHECKIN_SUCCESS_CHANGE_FINGER     //指纹已登记请更换手指

	//APP
	,TALKBACK_OK                        //对讲铃声OK
	,FINGER_PWD_ERROR                   //密码指纹输入错误提示音OK
	,PICK_LOCK_WARNNING                 //撬锁警报音

	,TONE_TOTAL
}PushToneType;

#endif // !_DEF_H_
