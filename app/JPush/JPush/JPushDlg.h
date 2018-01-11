
// JPushDlg.h : 头文件
//

#pragma once

#include "json/json.h"
#include "IniFile.h"
#include "afxwin.h"
#include "sstring.h"
#include <iostream>  
#include <fstream>  
#include <string> 
#include <vector>
using namespace std;

#define INI_JPUSH_SECTION					_T("jpush")
#define INI_JPUSH_APPKEY					_T("AppKey")
#define INI_JPUSH_MASTER_SECRET				_T("MasterSecret")

#define INI_PAGE_SECTION					_T("page")

#define INI_PAGE_RADIO_PRODUTION			_T("radioProdution")


#define INI_PAGE_CONTENT_TITLE				_T("ContentTitle")
#define INI_PAGE_CONTENT_DETAILS			_T("ContentDetails")
#define INI_PAGE_ENABLE_SMS_FOR_ANDROID		_T("EnableSmsForAndroid")

#define INI_PAGE_PUSH_TYPE					_T("PushType")

#define INI_PAGE_CONTENT_TYPE				_T("PushContentType")
#define INI_PAGE_RECV_TYPE					_T("PushRecvType")
#define INI_PAGE_RECV_TYPE_APPOINT_CHECK	_T("AppointAliasTags")
#define INI_PAGE_RECV_TYPE_TAG_CHECK		_T("AppointTagCheck")
#define INI_PAGE_RECV_TYPE_TAG_TYPE			_T("AppointTagType")
#define INI_PAGE_RECV_TYPE_TAG_DETAILS		_T("AppointTagDetails")
#define INI_PAGE_RECV_TYPE_ALIAS_CHECK		_T("AppointAliasCheck")
#define INI_PAGE_RECV_TYPE_ALIAS_DETAILS	_T("AppointAliasDetails")
#define INI_PAGE_RECV_TYPE_ACCURAT_DETAILS	_T("AppointAccuratDetails")


#define INI_PAGE_WARNNING_TYPE_INDEX	    _T("WarnningType")
#define INI_PAGE_TONE_INDEX	                _T("ToneId")
#define INI_PAGE_MAKE_ADMIN_LOCK_ID         _T("AdminLockId")


#define GDOG_DEV_LOCK_ID_LEN				(10)


// CJPushDlg 对话框
class CJPushDlg : public CDialogEx
{
// 构造
public:
	CJPushDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_JPUSH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


private:
	void		loadIniUpdate();
	void		saveIni();
	void        setPushType(PushType pt);
	void        setPushContentType(PushContentType pct);
	void		setRecvType(PushRecvType type);
	void		setIosMode(BOOL isProdution);

	BOOL		checkValid();

	// http://blog.csdn.net/wilsonke/article/details/9238395
	// C++ 中int，char，string，CString类型转换 
	Json::Value	jsonNotificationExtras();// "notification"->"ios"->"extras"

	Json::Value jsonOptions();// "options"

	Json::Value jsonAudience();// "audience"

	Json::Value jsonNotification();//"notification"

	Json::Value jsonNotificationIos();//"notification"->"ios"

	Json::Value jsonNotificationAndroid();//"notification"->"android"

	Json::Value jsonMessage();
	Json::Value jsonMessageExtras();

	Json::Value jsonSmsMessage();

	Json::Value jsonJpushPackge();

	vector<std::string>  jsonJpushPackges();

	std::ostringstream     authorization();//base64(appKey:masterSecret)

	INT       randSendno();

	PushToneInfo  getToneName(PushToneType ptt);

	CString			nowTime();
	String		stringNowTime();

	void		log(const char* sInfo);


	void		JpushV3Send();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CString		m_sAppKey;
	CEdit		m_edAppkey;
	CEdit		m_edMasterSecret;
	CString		m_sMasteSecret;



	//接收者
	CButton		*m_radioRecvAll;
	CButton		*m_radioRecvAppointAlias;
	CButton		*m_radioRecvAccurat;

	//指定-门锁tag
	CButton		m_cbTags;
	BOOL		m_bTags;
	CString		m_szTags;
	CEdit		m_ceTags;

	CButton		*m_radioTagAdmin;
	CButton		*m_radioTagGeneral;
	CButton		*m_radioTagBoth;


	//指定-账户
	CButton		m_cbAlias;
	BOOL		m_bAlias;
	CEdit		m_edAlias;
	CString		m_szAlias;

	//指定-精准
	CEdit		m_edAccurat;
	CString		m_szAccurat;

	// 平台
	CButton		m_cdPlatformIos;
	CButton		m_cdPlatformAndroid;

	//ios模式
	CButton		*m_radioDeveloper;
	CButton		*m_radioProdution;

	//推送类型
	CButton		*m_radioTypeNotify;
	CButton		*m_radioTypeMessage;
	CButton		*m_radioTypeBoth;

	//内容类型
	CButton		*m_radioCxtTypeGeneral;
	CButton		*m_radioCxtTypeUpgrad;
	CButton		*m_radioCxtTypeMakerting;
	CButton		*m_radioCxtTypeWarning;

	CEdit		m_ceAdminLockId;
	CString		m_szAdminLockId;

	// 报警子类型
	CComboBox	m_ccbWarnningType;

	// 声音编号
	CComboBox	m_ccbTone;


	// 通知中心显示
	CEdit		m_cdTitle;
	CString		m_szTitle;
	CEdit		m_edDetails;
	CString		m_szDetails;


	// android短信补充
	CButton		m_cbEnableSmsForAndroid;

	vector<INT>			 m_vectorSendno;
	vector<PushToneInfo> m_vectorTones;

public:
	afx_msg void OnRadioRecvAll();
	afx_msg void OnRadioRecvAppointAliasTags();
	afx_msg void OnRadioRecvAccurate();

	afx_msg void OnClickedCheckPlatformIos();
	afx_msg void OnClickedCheckPlatformAndroid();
	afx_msg void OnClickedCheckPlatformWinphone();
	afx_msg void OnRadioContentTypeGeneral();
	afx_msg void OnRadioContentTypeUpdate();
	afx_msg void OnRadioContentTypeMarketing();
	afx_msg void OnRadioContentTypeWarning();
	afx_msg void OnClickedCheckTags();
	afx_msg void OnClickedCheckAlias();
	afx_msg void OnRadioPushTypeNotify();
	afx_msg void OnRadioPushTypeMessage();
	afx_msg void OnRadioPushTypeBoth();
	afx_msg void OnSelchangeComboContentTypeWarning();
};
