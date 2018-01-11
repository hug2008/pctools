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

typedef enum PushTypeTag//��������
{
	PT_NOTIFY
	,PT_MESSAGE
	,PT_BOTH
}PushType;

typedef enum PushRecvTypeTag//����������
{
	RECV_TYPE_ALL
	,RECV_TYPE_ALIAS_TAGS
	,RECV_TYPE_ACCURAT
	,RECV_TYPE_TOTAL
}PushRecvType;
typedef enum PushTagTypeTag//tag����
{
	TAG_TYPE_ADMIN
	,TAG_TYPE_GENERAL
	,TAG_TYPE_BOTH
	,TAG_TYPE_TOTAL
}PushTagType;
typedef enum PushContentTypeTag//��������
{
	PCT_GENERAL
	,PCT_UPGRADE
	,PCT_MARKETING
	,PCT_WARNING
	,PCT_TOTAL
}PushContentType;
typedef enum PushContentTypeWarnningTag//���ݾ�������
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


typedef enum PushToneTypeTag//��������
{
	TONE_DEFAULT                        //Ĭ��
	, CHECKIN_FINGER_FAIL               //�Ǽ�ʧ�����ذ���ָ
	, CHECKIN_INPUT_FAIL                //�Ǽ�ʧ������������
	, HOME_NO_PENSON_CALL_MASTER        //�Բ��𣬼�����ʱû�ˣ�����ϵ����
	, INPUT_MORE_ERROR_SYSTEM_CLOSE     //����������ϵͳ�ѹرգ�5���Ӻ�ָ�
	, PWD_ERROR_INPUT_AGAIN             //���������������������
	, PWD_CHECKIN_SUCCESS               //����Ǽǳɹ�
	, PWD_CHECKED_IN_CHANGE_PWD         //�����ѵǼ����������
	, PWD_SUCEESS_OPEN_DOOR             //������ȷ���뿪��
	, PLEASE_PRESS_FINGER               //�밴��ָ
	, PLEASE_OPEN_DOOR                  //�뿪��
	, DELETE_USER_NO                    //��ɾ���û����
	, INPUT_USER_NO                     //�������û����
	, INPUT_PWD                         //�������û�����
	, CHECK_ADMIN_FINGER_OR_PWD         //����֤����Աָ�ƻ�����
	, INPUT_PWD_AGAIN                   //���ٴ���������
	, DELETE_SUCCESS                    //ɾ���ɹ�
	, CHECK_SUCCEED_PRESS_HANDLE        //�������֤���밴�°��ֿ���
	, CHECK_SUCCESS_OPEN_DOOR           //��֤�ɹ��뿪��
	, BELL_RINGS_CHECK_IDENTITY         //�п������ڰ������壬��ȷ�����
	, FINGER_CHECKIN_SUCCESS            //ָ�ƵǼǳɹ�
	, CHECKIN_FAIL_REPEAT_FINGER        //ָ����֤ʧ�����ذ���ָ
	, CHECKIN_SUCCESS_CHANGE_FINGER     //ָ���ѵǼ��������ָ

	//APP
	,TALKBACK_OK                        //�Խ�����OK
	,FINGER_PWD_ERROR                   //����ָ�����������ʾ��OK
	,PICK_LOCK_WARNNING                 //����������

	,TONE_TOTAL
}PushToneType;

#endif // !_DEF_H_
