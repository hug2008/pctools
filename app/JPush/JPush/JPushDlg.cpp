
// JPushDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JPush.h"
#include "JPushDlg.h"
#include "afxdialogex.h"
#include <stdlib.h>

#if defined(GDOG_PUSH)
#define  TOOL_INFO		_T("德犬安防推送工具-V1.0.0")
#define  CUSTOM_TITLE	_T("标题")
#define  CUSTOM_INFO	_T("内容")
#elif(PUBLIC_SHOW)
#define  TOOL_INFO		_T("极光推送V3[Q:767861149-定制]V1.0.0")
#define  CUSTOM_INFO	_T("可定制更多参数界面和适合服务器运行的命令行程序[Windows/linux/unix]")
#else
#ifdef _DEBUG
#define  TOOL_INFO	_T("JPushIOs D0.0.0.0")
#else
#define  TOOL_INFO	_T("JPushIOs R0.0.0.0")
#endif // _DEBUG
#endif


#define JSON_KEY_AUDIENCE			"audience"
#define JSON_KEY_NOTIFICATION		"notification"
#define JSON_KEY_MESSAGE			"message"
#define JSON_KEY_EXTRAS				"extras"
#define JSON_KEY_SMS_MESSAGE		"sms_message"
#define JSON_KEY_OPTIONS			"options"
#define JSON_KEY_APNS_PRODUCTION	"apns_production"
#define JSON_KEY_SENDNO				"sendno"

#if defined(_UNICODE) || defined(UNICODE)
#error 请使用多字符集！
#endif

#include "Poco/Exception.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Base64Encoder.h"
#include "Poco/MD5Engine.h"
#include "Poco/Timespan.h"
#include <iostream>

using Poco::Exception;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::StringPartSource;
using Poco::Net::Context;
using Poco::Path;
using Poco::StreamCopier;
using Poco::URI;
using Poco::Base64Encoder;
using Poco::MD5Engine;
using Poco::DigestEngine;
using Poco::Timespan;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJPushDlg 对话框



CJPushDlg::CJPushDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJPushDlg::IDD, pParent)
	, m_sAppKey(_T(""))
	, m_sMasteSecret(_T(""))
	, m_szAlias(_T(""))
	, m_szDetails(_T("通知中心内容"))
	, m_szTags(_T(""))
	, m_bAlias(FALSE)
	, m_bTags(FALSE)
	, m_szAccurat(_T(""))
	, m_szTitle(_T("通知中心标题"))
	, m_szAdminLockId(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJPushDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_JPUSH_APPKEY, m_sAppKey);
	DDX_Control(pDX, IDC_EDIT_JPUSH_APPKEY, m_edAppkey);
	DDX_Control(pDX, IDC_EDIT_JPUSH_MASTER_SECRET, m_edMasterSecret);
	DDX_Text(pDX, IDC_EDIT_JPUSH_MASTER_SECRET, m_sMasteSecret);
	DDX_Control(pDX, IDC_EDIT_RECV_APPOINT_ALIAS, m_edAlias);
	DDX_Text(pDX, IDC_EDIT_RECV_APPOINT_ALIAS, m_szAlias);
	DDX_Control(pDX, IDC_EDIT_NC_DETAILS, m_edDetails);
	DDX_Text(pDX, IDC_EDIT_NC_DETAILS, m_szDetails);
	DDX_Control(pDX, IDC_CHECK_IDC_PLATFORM_IOS, m_cdPlatformIos);
	DDX_Control(pDX, IDC_CHECK_IDC_PLATFORM_ANDROID, m_cdPlatformAndroid);
	DDX_Control(pDX, IDC_CHECK_TAGS, m_cbTags);
	DDX_Text(pDX, IDC_EDIT_RECV_APPOINT_TAGS, m_szTags);
	DDX_Control(pDX, IDC_EDIT_RECV_APPOINT_TAGS, m_ceTags);
	DDX_Control(pDX, IDC_CHECK_ALIAS, m_cbAlias);
	DDX_Check(pDX, IDC_CHECK_ALIAS, m_bAlias);
	DDX_Check(pDX, IDC_CHECK_TAGS, m_bTags);
	DDX_Control(pDX, IDC_EDIT_RECV_ACCURATE, m_edAccurat);
	DDX_Text(pDX, IDC_EDIT_RECV_ACCURATE, m_szAccurat);
	DDX_Control(pDX, IDC_COMBO_CONTENT_TYPE_WARNING, m_ccbWarnningType);
	DDX_Control(pDX, IDC_COMBO_SOUND, m_ccbTone);
	DDX_Control(pDX, IDC_EDIT_NC_TITLE, m_cdTitle);
	DDX_Text(pDX, IDC_EDIT_NC_TITLE, m_szTitle);
	DDX_Control(pDX, IDC_EDIT_ADMIN_LOCKID, m_ceAdminLockId);
	DDX_Text(pDX, IDC_EDIT_ADMIN_LOCKID, m_szAdminLockId);
	DDX_Control(pDX, IDC_CHECK_SMS_FOR_ANDROID, m_cbEnableSmsForAndroid);
}

BEGIN_MESSAGE_MAP(CJPushDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_RADIO_RECV_ALL, &CJPushDlg::OnRadioRecvAll)
	ON_COMMAND(IDC_RADIO_RECV_APPOINT_ALIAS_TAGS, &CJPushDlg::OnRadioRecvAppointAliasTags)
	ON_BN_CLICKED(IDC_CHECK_IDC_PLATFORM_IOS, &CJPushDlg::OnClickedCheckPlatformIos)
	ON_BN_CLICKED(IDC_CHECK_IDC_PLATFORM_ANDROID, &CJPushDlg::OnClickedCheckPlatformAndroid)
	ON_COMMAND(IDC_RADIO_RECV_ACCURATE, &CJPushDlg::OnRadioRecvAccurate)
	ON_COMMAND(IDC_RADIO_CONTENT_TYPE_GENERAL, &CJPushDlg::OnRadioContentTypeGeneral)
	ON_COMMAND(IDC_RADIO_CONTENT_TYPE_UPDATE, &CJPushDlg::OnRadioContentTypeUpdate)
	ON_COMMAND(IDC_RADIO_CONTENT_TYPE_MARKETING, &CJPushDlg::OnRadioContentTypeMarketing)
	ON_COMMAND(IDC_RADIO_CONTENT_TYPE_WARNING, &CJPushDlg::OnRadioContentTypeWarning)
	ON_BN_CLICKED(IDC_CHECK_TAGS, &CJPushDlg::OnClickedCheckTags)
	ON_BN_CLICKED(IDC_CHECK_ALIAS, &CJPushDlg::OnClickedCheckAlias)
	ON_COMMAND(IDC_RADIO_PUSH_TYPE_NOTIFY, &CJPushDlg::OnRadioPushTypeNotify)
	ON_COMMAND(IDC_RADIO_PUSH_TYPE_MESSAGE, &CJPushDlg::OnRadioPushTypeMessage)
	ON_COMMAND(IDC_RADIO_PUSH_TYPE_BOTH, &CJPushDlg::OnRadioPushTypeBoth)
	ON_CBN_SELCHANGE(IDC_COMBO_CONTENT_TYPE_WARNING, &CJPushDlg::OnSelchangeComboContentTypeWarning)
END_MESSAGE_MAP()


// CJPushDlg 消息处理程序
BOOL CJPushDlg::PreTranslateMessage(MSG* pMsg)
{
	// http://bbs.csdn.net/topics/311470
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam)
		{
			int iDlgCtrlID = ::GetDlgCtrlID(pMsg->hwnd);
			if ((IDD_JPUSH_DIALOG == iDlgCtrlID) || ((IDC_EDIT_JPUSH_APPKEY <= iDlgCtrlID) && (IDC_CHECK_SMS_FOR_ANDROID >= iDlgCtrlID)))
			{
				JpushV3Send();
				return TRUE;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CJPushDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	SetWindowText(TOOL_INFO);


	m_radioRecvAll			= (CButton*)GetDlgItem(IDC_RADIO_RECV_ALL);
	m_radioRecvAppointAlias = (CButton*)GetDlgItem(IDC_RADIO_RECV_APPOINT_ALIAS_TAGS);
	m_radioRecvAccurat		= (CButton*)GetDlgItem(IDC_RADIO_RECV_ACCURATE);


	m_radioTagAdmin			= (CButton*)GetDlgItem(IDC_RADIO_USR_TYPE_ADMIN);
	m_radioTagGeneral		= (CButton*)GetDlgItem(IDC_RADIO_USR_TYPE_GENERAL);
	m_radioTagBoth			= (CButton*)GetDlgItem(IDC_RADIO_USR_TYPE_BOTH);

	m_radioDeveloper		= (CButton*)GetDlgItem(IDC_RADIO_IOS_RECV_MODE_DEVELOPER);
	m_radioProdution		= (CButton*)GetDlgItem(IDC_RADIO_IOS_RECV_MODE_PRODUTION);

	m_radioTypeNotify		= (CButton*)GetDlgItem(IDC_RADIO_PUSH_TYPE_NOTIFY);
	m_radioTypeMessage		= (CButton*)GetDlgItem(IDC_RADIO_PUSH_TYPE_MESSAGE);
	m_radioTypeBoth			= (CButton*)GetDlgItem(IDC_RADIO_PUSH_TYPE_BOTH);



	m_radioCxtTypeGeneral			= (CButton*)GetDlgItem(IDC_RADIO_CONTENT_TYPE_GENERAL);
	m_radioCxtTypeUpgrad			= (CButton*)GetDlgItem(IDC_RADIO_CONTENT_TYPE_UPDATE);
	m_radioCxtTypeMakerting			= (CButton*)GetDlgItem(IDC_RADIO_CONTENT_TYPE_MARKETING);
	m_radioCxtTypeWarning			= (CButton*)GetDlgItem(IDC_RADIO_CONTENT_TYPE_WARNING);

#if defined(GDOG_PUSH)
	m_edAppkey.EnableWindow(FALSE);
	m_edMasterSecret.EnableWindow(FALSE);
#endif

	m_ceTags.SetLimitText(GDOG_DEV_LOCK_ID_LEN);
	m_ceAdminLockId.SetLimitText(GDOG_DEV_LOCK_ID_LEN);
	m_edDetails.SetLimitText(JPUSH_ALERT_MAX_LEN);

	getToneName(TONE_DEFAULT);
	loadIniUpdate();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJPushDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CJPushDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CJPushDlg::JpushV3Send()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::UpdateData(TRUE);

	if (!checkValid())
	{
		return;
	}
	saveIni();

	String sInfo;

	try
	{
		URI purl(JPUSH_POST_URL_V3);

		HTTPSClientSession session(purl.getHost(), purl.getPort(), new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false));
		session.setTimeout(Timespan(30, 0));
		Poco::Net::HTTPRequest req(HTTPRequest::HTTP_POST, purl.getPath().c_str(), Poco::Net::HTTPMessage::HTTP_1_1);

		vector<std::string> vectorPackges = jsonJpushPackges();
		int count = vectorPackges.size();
		for (int pushIndex=0; pushIndex<count; pushIndex++)
		{
			Poco::Net::HTTPRequest pushReq(HTTPRequest::HTTP_POST, purl.getPath().c_str(), Poco::Net::HTTPMessage::HTTP_1_1);
			std::ostringstream pushAuth;
			pushAuth << "Basic " << authorization().str().c_str();
			pushReq.set("Authorization", pushAuth.str().c_str());

			std::string szData = ConvertANSIToUTF8(vectorPackges.at(pushIndex).c_str());
			pushReq.setContentLength(szData.length());
			pushReq.setContentType("Content-Type: application/json;charset=utf-8");
			session.sendRequest(pushReq) << szData;

			sInfo.Format("[TIME: %s\tURL: %s]\r", stringNowTime().c_str(), JPUSH_POST_URL_V3);

			log(sInfo.c_str());
			sInfo.Format("POST:\r\t%s", vectorPackges.at(pushIndex).c_str());
			log(sInfo.c_str());

			HTTPResponse pushRsp;
			std::istream& rs = session.receiveResponse(pushRsp);
			std::ostringstream ostr;
			StreamCopier::copyStream(rs, ostr);

			sInfo.Format("Rsp:\r\t%s\r\n", ostr.str().c_str());
			log(sInfo.c_str());

			Json::Reader readerRsp;
			Json::Value rootRsp;

			if (readerRsp.parse(ostr.str().c_str(), rootRsp))
			{
				CString szMsg = _T("");
				if (rootRsp[JSON_KEY_SENDNO].isNull())
				{
					szMsg.Format(_T("错误[%d]\ncode:%d, msg:%s"), m_vectorSendno.at(pushIndex), rootRsp["error"]["code"].asInt(), CString(rootRsp["error"]["message"].asString().c_str()));

					MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
				}
				else
				{
					INT			dwRspSendno = atol(rootRsp[JSON_KEY_SENDNO].asCString());

					if (m_vectorSendno.at(pushIndex) == dwRspSendno)
					{
						szMsg.Format(_T("推送成功[%d]\n%s\n\r"), m_vectorSendno.at(pushIndex), CString(ostr.str().c_str()));
						MessageBox(szMsg, TOOL_INFO, MB_OK);
					}
					else
					{
						szMsg.Format(_T("错误\n不是我推送的sendno:{me:%d, recv:%d}"), m_vectorSendno.at(pushIndex), dwRspSendno);
						MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
					}
				}
			}
			else
			{
				MessageBox(_T("未知返回格式"), TOOL_INFO, MB_OK | MB_ICONERROR);
			}
		}

#if 0

		std::ostringstream auth;
		auth << "Basic " << authorization().str().c_str();
		req.set("Authorization", auth.str().c_str());

		Json::FastWriter writer;
		std::string reqBody(writer.write(jsonJpushPackge()).data());
		std::string szData = ConvertANSIToUTF8(reqBody.c_str());
		req.setContentLength(szData.length());
		req.setContentType("Content-Type: application/json;charset=utf-8");
		session.sendRequest(req) << szData;

#if defined(_UNICODE) || defined(UNICODE)
		sInfo.Format("[TIME: %s\tURL: %s]\r", stringNowTime().c_str(), sUrl);
#else
		sInfo.Format("[TIME: %s\tURL: %s]\r", stringNowTime().c_str(), JPUSH_POST_URL_V3);
#endif
		log(sInfo.c_str());

		sInfo.Format("POST:\r\t%s", reqBody.c_str());
		log(sInfo.c_str());

		HTTPResponse response;
		std::istream& rs = session.receiveResponse(response);
		std::ostringstream ostr;
		StreamCopier::copyStream(rs, ostr);
		Json::Reader reader;

		sInfo.Format("Rsp:\r\t%s\r\n", ostr.str().c_str());
		log(sInfo.c_str());

		Json::Value rootRsp;
		if (reader.parse(ostr.str().c_str(), rootRsp))
		{
			CString szMsg = _T("");
			if (rootRsp[JSON_KEY_SENDNO].isNull())
			{
				szMsg.Format(_T("错误\ncode:%d, msg:%s"), rootRsp["error"]["code"].asInt(), CString(rootRsp["error"]["message"].asString().c_str()));

				MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			}
			else
			{
				INT			dwRspSendno = rootRsp[JSON_KEY_SENDNO].asInt();

				if (dwSendno == dwRspSendno)
				{
					szMsg.Format(_T("推送成功\n%s\n\r"), CString(ostr.str().c_str()));
					MessageBox(szMsg, TOOL_INFO, MB_OK);
				}
				else
				{
					szMsg.Format(_T("错误\n不是我推送的sendno:{me:%d, recv:%d}"), dwSendno, dwRspSendno);
					MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
				}
			}
		}
		else
		{
			MessageBox(_T("未知结果"), TOOL_INFO, MB_OK | MB_ICONERROR);
		}
#endif
	}
	catch (Poco::Net::NetException& ex)
	{
		TRACE(traceAppMsg, 0, ex.displayText().c_str());
		TRACE(traceAppMsg, 0, "\r\n");
	}
	catch (Poco::Exception& ex)
	{
		TRACE(traceAppMsg, 0, ex.displayText().c_str());
		TRACE(traceAppMsg, 0, "\r\n");
	}
}


void CJPushDlg::loadIniUpdate()
{
	CDialogEx::UpdateData(TRUE);

	CIniFile	m_ini(CONFIG_FILE_NAME);

	m_sAppKey		= m_ini.ReadString(INI_JPUSH_SECTION, INI_JPUSH_APPKEY, JPUSH_APPKEY);

	m_sMasteSecret = m_ini.ReadString(INI_JPUSH_SECTION, INI_JPUSH_MASTER_SECRET, JPUSH_MASTER_SECRET);

	PushRecvType recvType = (PushRecvType)m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE, RECV_TYPE_ALL);


	m_radioRecvAll->SetCheck(RECV_TYPE_ALL == recvType);
	m_radioRecvAppointAlias->SetCheck(RECV_TYPE_ALIAS_TAGS == recvType);
	m_radioRecvAccurat->SetCheck(RECV_TYPE_ACCURAT == recvType);


	UINT aVal = m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_TAG_CHECK, 0);
	m_bTags = (0 != aVal) && (RECV_TYPE_ALIAS_TAGS == recvType);
	m_ceTags.EnableWindow(m_bTags);
	m_radioTagAdmin->EnableWindow(m_bTags);
	m_radioTagGeneral->EnableWindow(m_bTags);
	m_radioTagBoth->EnableWindow(m_bTags);
	
	PushTagType tagType = (PushTagType)(m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_TAG_TYPE, TAG_TYPE_ADMIN)%TAG_TYPE_TOTAL);
	m_radioTagAdmin->SetCheck(TAG_TYPE_ADMIN == tagType);
	m_radioTagGeneral->SetCheck(TAG_TYPE_GENERAL == tagType);
	m_radioTagBoth->SetCheck(TAG_TYPE_BOTH == tagType);


	m_szTags = m_ini.ReadString(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_TAG_DETAILS, _T(""));
	m_szAlias = m_ini.ReadString(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_ALIAS_DETAILS, _T(""));
	m_szAccurat = m_ini.ReadString(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_ACCURAT_DETAILS, _T(""));

	aVal = m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_ALIAS_CHECK, 0);
	m_bAlias = (0 != aVal) && (RECV_TYPE_ALIAS_TAGS == recvType);
	m_edAlias.EnableWindow(m_bAlias);

	m_edAccurat.EnableWindow((RECV_TYPE_ACCURAT == recvType) && m_radioRecvAccurat->GetCheck());


	PushType pushType = (PushType)m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_PUSH_TYPE, PT_NOTIFY);

	// 推送平台
	m_cdPlatformIos.SetCheck(m_ini.ReadInt(JPUSH_PLATFORM, PLATFORM_IOS, 0));
	m_cdPlatformAndroid.SetCheck(m_ini.ReadInt(JPUSH_PLATFORM, PLATFORM_ANDROID, 0) && (pushType != PT_NOTIFY));

	m_cdPlatformAndroid.EnableWindow(pushType != PT_NOTIFY);
	m_radioDeveloper->EnableWindow(m_cdPlatformIos.GetCheck());
	m_radioProdution->EnableWindow(m_cdPlatformIos.GetCheck());
	//m_cdTitle.EnableWindow(!m_cdPlatformIos.GetCheck());
	setIosMode((0 == m_ini.ReadInt(JPUSH_PLATFORM, INI_PAGE_RADIO_PRODUTION, 1)));

	//推送类型
	setPushType(pushType);


	// android 短信补充
	aVal = m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_ENABLE_SMS_FOR_ANDROID, 0);
	m_cbEnableSmsForAndroid.SetCheck(0 < aVal);
	m_cbEnableSmsForAndroid.EnableWindow(m_cdPlatformAndroid.GetCheck());

	//内容类型
	setPushContentType((PushContentType)(m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_CONTENT_TYPE, PCT_GENERAL)));


	m_szTitle = m_ini.ReadString(INI_PAGE_SECTION, INI_PAGE_CONTENT_TITLE, _T("通知中心标题"));
	m_szDetails = m_ini.ReadString(INI_PAGE_SECTION, INI_PAGE_CONTENT_DETAILS,_T("通知中心内容"));


	int index = m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_WARNNING_TYPE_INDEX, PCTW_OPEN_DOOR) % PCTW_TOTAL;
	m_ccbWarnningType.InsertString(PCTW_OPEN_DOOR, _T("开门"));
	m_ccbWarnningType.InsertString(PCTW_CLOSE_DOOR, _T("关门"));
	m_ccbWarnningType.InsertString(PCTW_PRY_DOOR, _T("撬门"));
	m_ccbWarnningType.InsertString(PCTW_VOICE, _T("语音"));
	m_ccbWarnningType.InsertString(PCTW_IMAGE, _T("异常图片"));
	m_ccbWarnningType.InsertString(PCTW_BATTERY_LOW, _T("电量低"));
	m_ccbWarnningType.InsertString(PCTW_MAKE_ADMIN, _T("成为管理员"));
	m_ccbWarnningType.SetCurSel(index);
	m_ceAdminLockId.EnableWindow(PCTW_MAKE_ADMIN == index);
	m_szAdminLockId = m_ini.ReadString(INI_PAGE_SECTION, INI_PAGE_MAKE_ADMIN_LOCK_ID, _T(""));

	index = m_ini.ReadInt(INI_PAGE_SECTION, INI_PAGE_TONE_INDEX, TONE_DEFAULT) % TONE_TOTAL;
	for (int toneId=0; toneId<m_vectorTones.size(); toneId++){
		m_ccbTone.InsertString(toneId,getToneName((PushToneType)toneId).getDisplay());
	}
	m_ccbTone.SetCurSel(index);
	


	CDialogEx::UpdateData(FALSE);
}

void CJPushDlg::saveIni()
{
	CDialogEx::UpdateData(TRUE);

	CIniFile	m_ini(CONFIG_FILE_NAME);

	m_ini.WriteString(INI_JPUSH_SECTION, INI_JPUSH_APPKEY, m_sAppKey);
	m_ini.WriteString(INI_JPUSH_SECTION, INI_JPUSH_MASTER_SECRET, m_sMasteSecret);

	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_TAG_CHECK, m_cbTags.GetCheck());

	PushTagType tagType = m_radioTagGeneral->GetCheck() ? TAG_TYPE_ADMIN : (m_radioTagGeneral->GetCheck() ? TAG_TYPE_GENERAL : TAG_TYPE_BOTH);
	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_TAG_TYPE, tagType);

	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_ALIAS_CHECK, m_cbAlias.GetCheck());


	m_ini.WriteString(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_TAG_DETAILS, m_szTags);
	m_ini.WriteString(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_ALIAS_DETAILS, m_szAlias);
	m_ini.WriteString(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE_ACCURAT_DETAILS, m_szAccurat);

	PushRecvType prt = m_radioRecvAll->GetCheck() ? RECV_TYPE_ALL : (m_radioRecvAppointAlias->GetCheck() ? RECV_TYPE_ALIAS_TAGS : RECV_TYPE_ACCURAT);
	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_RECV_TYPE, prt);


	PushContentType pct = m_radioCxtTypeGeneral->GetCheck() ? PCT_GENERAL : (m_radioCxtTypeUpgrad->GetCheck() ? PCT_UPGRADE : (m_radioCxtTypeMakerting->GetCheck() ? PCT_MARKETING : PCT_WARNING));
	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_CONTENT_TYPE, pct);


	m_ini.WriteInt(JPUSH_PLATFORM, PLATFORM_IOS, m_cdPlatformIos.GetCheck());
	m_ini.WriteInt(JPUSH_PLATFORM, PLATFORM_ANDROID, m_cdPlatformAndroid.GetCheck());
	m_ini.WriteInt(JPUSH_PLATFORM, INI_PAGE_RADIO_PRODUTION, m_radioProdution->GetCheck());

	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_ENABLE_SMS_FOR_ANDROID, m_cbEnableSmsForAndroid.GetCheck());


	PushType pt = m_radioTypeNotify->GetCheck() ? PT_NOTIFY : (m_radioTypeMessage->GetCheck() ? PT_MESSAGE : PT_BOTH);
	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_PUSH_TYPE, pt);

	m_ini.WriteString(INI_PAGE_SECTION, INI_PAGE_CONTENT_TITLE, m_szTitle);
	m_ini.WriteString(INI_PAGE_SECTION, INI_PAGE_CONTENT_DETAILS, m_szDetails);

	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_WARNNING_TYPE_INDEX, m_ccbWarnningType.GetCurSel());
	m_ini.WriteString(INI_PAGE_SECTION, INI_PAGE_MAKE_ADMIN_LOCK_ID, m_szAdminLockId);
	m_ini.WriteInt(INI_PAGE_SECTION, INI_PAGE_TONE_INDEX, m_ccbTone.GetCurSel());

}


void CJPushDlg::setPushType(PushType pt)
{
	m_radioTypeNotify->SetCheck(PT_NOTIFY == pt);
	m_radioTypeMessage->SetCheck(PT_MESSAGE == pt);
	m_radioTypeBoth->SetCheck(PT_BOTH == pt);
}

void  CJPushDlg::setPushContentType(PushContentType pct)
{
	m_ccbWarnningType.EnableWindow(PCT_WARNING == pct);
	m_radioCxtTypeGeneral->SetCheck(PCT_GENERAL == pct);
	m_radioCxtTypeUpgrad->SetCheck(PCT_UPGRADE == pct);
	m_radioCxtTypeMakerting->SetCheck(PCT_MARKETING == pct);
	m_radioCxtTypeWarning->SetCheck(PCT_WARNING == pct);
	m_ceAdminLockId.EnableWindow((PCT_WARNING == pct) && (PCTW_MAKE_ADMIN == m_ccbWarnningType.GetCurSel()));
	if ((PCT_WARNING == pct) && (PCTW_MAKE_ADMIN == m_ccbWarnningType.GetCurSel()))
	{
		m_bTags = FALSE;
		CDialogEx::UpdateData(FALSE);
	}
}

void CJPushDlg::setRecvType(PushRecvType type)
{
	m_radioRecvAll->SetCheck(RECV_TYPE_ALL == type);
	m_radioRecvAppointAlias->SetCheck(RECV_TYPE_ALIAS_TAGS == type);
	m_radioRecvAccurat->SetCheck(RECV_TYPE_ACCURAT == type);

	m_cbTags.EnableWindow(RECV_TYPE_ALIAS_TAGS == type);
	m_ceTags.EnableWindow((RECV_TYPE_ALIAS_TAGS == type) && m_cbTags.GetCheck());
	m_radioTagAdmin->EnableWindow((RECV_TYPE_ALIAS_TAGS == type) && m_cbTags.GetCheck());
	m_radioTagGeneral->EnableWindow((RECV_TYPE_ALIAS_TAGS == type) && m_cbTags.GetCheck());
	m_radioTagBoth->EnableWindow((RECV_TYPE_ALIAS_TAGS == type) && m_cbTags.GetCheck());


	m_cbAlias.EnableWindow(RECV_TYPE_ALIAS_TAGS == type);
	m_edAlias.EnableWindow((RECV_TYPE_ALIAS_TAGS == type) && m_cbAlias.GetCheck());

	m_edAccurat.EnableWindow((RECV_TYPE_ACCURAT == type) && m_radioRecvAccurat->GetCheck());
}

void CJPushDlg::setIosMode(BOOL isProdution)
{
	m_radioDeveloper->SetCheck(isProdution);
	m_radioProdution->SetCheck(!isProdution);
}

std::ostringstream  CJPushDlg::authorization()
{
	std::ostringstream base64;

	Base64Encoder encoder(base64);
#if defined(_UNICODE) || defined(UNICODE)
	CHAR appKey[128] = { 0 };
	CHAR masteSecret[128] = { 0 };

	WcharToChar(appKey, m_sAppKey, ARRAY_SZIE(appKey));
	WcharToChar(masteSecret, m_sMasteSecret, ARRAY_SZIE(masteSecret));

	encoder << appKey << ":" << masteSecret;
#else
	encoder << m_sAppKey << ":" << m_sMasteSecret;
#endif;
	encoder.close();

	return base64;
}
INT  CJPushDlg::randSendno()
{
	INT  iVal = RAND_MAX;

	srand((unsigned int)time(0));

	return rand();
}

PushToneInfo CJPushDlg::getToneName(PushToneType ptt){
	if (0 == m_vectorTones.size())
	{
		m_vectorTones.push_back(PushToneInfo(TONE_DEFAULT, "default", _T("默认")));
		m_vectorTones.push_back(PushToneInfo(CHECKIN_FINGER_FAIL, "push_tone_01_checkin_finger_fail.wav", _T("登记失败请重按手指")));
		m_vectorTones.push_back(PushToneInfo(CHECKIN_INPUT_FAIL, "push_tone_02_checkin_input_fail.wav", _T("登记失败请重新输入")));
		m_vectorTones.push_back(PushToneInfo(HOME_NO_PENSON_CALL_MASTER, "push_tone_03_home_no_penson_call_master.wav", _T("对不起，家里暂时没人，请联系主人")));
		m_vectorTones.push_back(PushToneInfo(INPUT_MORE_ERROR_SYSTEM_CLOSE, "push_tone_04_input_more_error_system_close.wav", _T("多次输入错误，系统已关闭，5分钟后恢复")));
		m_vectorTones.push_back(PushToneInfo(PWD_ERROR_INPUT_AGAIN, "push_tone_05_pwd_error_input_again.wav", _T("密码错误，请重新输入密码")));
		m_vectorTones.push_back(PushToneInfo(PWD_CHECKIN_SUCCESS, "push_tone_06_pwd_checkin_success.wav", _T("密码登记成功")));
		m_vectorTones.push_back(PushToneInfo(PWD_CHECKED_IN_CHANGE_PWD, "push_tone_07 _pwd_checked_in_change_pwd.wav", _T("密码已登记请更换密码")));
		m_vectorTones.push_back(PushToneInfo(PWD_SUCEESS_OPEN_DOOR, "push_tone_08_pwd_suceess_open_door.wav", _T("密码正确，请开门")));
		m_vectorTones.push_back(PushToneInfo(PLEASE_PRESS_FINGER, "push_tone_09_please_press_finger.wav", _T("请按手指")));
		m_vectorTones.push_back(PushToneInfo(PLEASE_OPEN_DOOR, "push_tone_10_please_open_door.wav", _T("请开门")));
		m_vectorTones.push_back(PushToneInfo(DELETE_USER_NO, "push_tone_11_delete_user_no.wav", _T("请删除用户编号")));
		m_vectorTones.push_back(PushToneInfo(INPUT_USER_NO, "push_tone_12_input_user_no.wav", _T("请输入用户编号")));
		m_vectorTones.push_back(PushToneInfo(INPUT_PWD, "push_tone_13_input_pwd.wav", _T("请输入用户密码")));
		m_vectorTones.push_back(PushToneInfo(CHECK_ADMIN_FINGER_OR_PWD, "push_tone_14_check_admin_finger_or_pwd.wav", _T("请验证管理员指纹或密码")));
		m_vectorTones.push_back(PushToneInfo(INPUT_PWD_AGAIN, "push_tone_15_input_pwd_again.wav", _T("请再次输入密码")));
		m_vectorTones.push_back(PushToneInfo(DELETE_SUCCESS, "push_tone_16_delete_success.wav", _T("删除成功")));
		m_vectorTones.push_back(PushToneInfo(CHECK_SUCCEED_PRESS_HANDLE, "push_tone_17_check_succeed_press_handle.wav", _T("身份已验证，请按下把手开门")));
		m_vectorTones.push_back(PushToneInfo(CHECK_SUCCESS_OPEN_DOOR, "push_tone_18_check_success_open_door.wav", _T("验证成功请开门")));
		m_vectorTones.push_back(PushToneInfo(BELL_RINGS_CHECK_IDENTITY, "push_tone_19_bell_rings_check_identity.wav", _T("有客人正在按响门铃，请确认身份")));
		m_vectorTones.push_back(PushToneInfo(FINGER_CHECKIN_SUCCESS, "push_tone_20_finger_checkin_success.wav", _T("指纹登记成功")));
		m_vectorTones.push_back(PushToneInfo(CHECKIN_FAIL_REPEAT_FINGER, "push_tone_21_checkin_fail_repeat_finger.wav", _T("指纹验证失败请重按手指")));
		m_vectorTones.push_back(PushToneInfo(CHECKIN_SUCCESS_CHANGE_FINGER, "push_tone_22_checkin_success_change_finger.wav", _T("指纹已登记请更换手指")));
		m_vectorTones.push_back(PushToneInfo(TALKBACK_OK, "push_tone_23_talkback_ok.wav", _T("对讲铃声OK")));
		m_vectorTones.push_back(PushToneInfo(FINGER_PWD_ERROR, "push_tone_24_finger_pwd_error.wav", _T("密码指纹输入错误提示音OK")));
		m_vectorTones.push_back(PushToneInfo(PICK_LOCK_WARNNING, "push_tone_25_pick_lock_warnning.wav", _T("撬锁警报音")));
	}

	return m_vectorTones.at(ptt);
}

CString  CJPushDlg::nowTime()
{
	time_t now;
	struct tm l_time = { 0 };

	now = time((time_t *)NULL);

	localtime_s(&l_time, &now);

	CString szInfo = _T("");
	szInfo.Format(_T("%04d%02d%02d-%02d:%02d:%02d"),
		l_time.tm_year + 1900,
		l_time.tm_mon + 1,
		l_time.tm_mday,
		l_time.tm_hour,
		l_time.tm_min,
		l_time.tm_sec);

	return szInfo;
}


String	 CJPushDlg::stringNowTime()
{
	String sTime;

	time_t now;
	struct tm l_time = { 0 };

	now = time((time_t *)NULL);

	localtime_s(&l_time, &now);

	sTime.Format("%04d%02d%02d-%02d:%02d:%02d", 
		l_time.tm_year + 1900,
		l_time.tm_mon + 1,
		l_time.tm_mday,
		l_time.tm_hour,
		l_time.tm_min,
		l_time.tm_sec);

	return sTime;
}

void CJPushDlg::log(const char* sInfo)
{
#if !defined(PUBLIC_SHOW)
	ofstream outF("log.txt", ios_base::out | ios_base::app);

	outF.write(sInfo, strlen(sInfo));

	outF.close();
#endif
}

Json::Value	CJPushDlg::jsonNotificationExtras()
{
	Json::Value	rootExtras;
	CString sVal = _T("aValue");
#if !defined(GDOG_PUSH)
#if defined(_UNICODE) || defined(UNICODE)
	CHAR cVal[JPUSH_IOS_NOTIFICATION_BODY_MAX / 2 + 1] = { 0 };
	WcharToChar(cVal, sVal, ARRAY_SZIE(cVal));
	rootExtras["aKey"] = cVal;
#else
	rootExtras["aKey"] = std::string(sVal);
#endif // _UNICODE
#endif
	PushContentType pct = PCT_GENERAL;
	if (m_radioCxtTypeUpgrad->GetCheck())
	{
		pct = PCT_UPGRADE;
	}
	else if (m_radioCxtTypeMakerting->GetCheck())
	{
		pct = PCT_MARKETING;
	}
	else if (m_radioCxtTypeWarning->GetCheck())
	{
		pct = PCT_WARNING;

		rootExtras["wType"] = m_ccbWarnningType.GetCurSel();
	}
	else
	{

	}

	rootExtras["cType"] = pct;
	rootExtras["tone"] = m_ccbTone.GetCurSel();

	return rootExtras;
}

Json::Value	CJPushDlg::jsonOptions()
{
	Json::Value	rootOptions;

	rootOptions["time_to_live"] = 24 * 60 * 60;

	rootOptions[JSON_KEY_APNS_PRODUCTION] = (0 != m_radioProdution->GetCheck());

	rootOptions[JSON_KEY_SENDNO] = randSendno();

	return rootOptions;
}

Json::Value CJPushDlg::jsonAudience()
{
	Json::Value	root;

	CHAR cVal[1024] = { 0 };

	if (m_radioRecvAll->GetCheck())
	{
		root = "all";
	}
	else if (m_radioRecvAppointAlias->GetCheck())
	{
		if (m_cbTags.GetCheck())//tag
		{
			BOOL bAdmin = m_radioTagAdmin->GetCheck();
			BOOL bBoth = m_radioTagBoth->GetCheck();
			if (0 == m_szTags.GetLength())
			{
				if (bBoth)
				{
					root["tag"].append("Admin").append("General");
				}
				else if (bAdmin)
				{
					root["tag"].append("Admin");
				}
				else
				{
					root["tag"].append("General");
				}
			}
			else
			{
				CString szRoleLockId;
				if (bBoth)
				{
					root["tag"].append((const char*)m_szTags);
				}
				else if (bAdmin)
				{
					szRoleLockId.Format(_T("Admin_%s"), m_szTags);
					root["tag_and"].append((const char*)szRoleLockId);
					root["tag_and"].append((const char*)m_szTags);
				}
				else
				{
					szRoleLockId.Format(_T("General_%s"), m_szTags);
					root["tag_and"].append((const char*)szRoleLockId);
					root["tag_and"].append((const char*)m_szTags);
				}
			}
		}

		if (m_cbAlias.GetCheck() && (0 < m_szAlias.GetLength()))
		{
			CStringArray arrayAlias;
			SplitString(m_szAlias, ';', arrayAlias);
			for (int index=0; index<arrayAlias.GetSize(); index++)
			{
				root["alias"].append((const char*)arrayAlias.GetAt(index));
			}
		}
	}
	else
	{
#if defined(GDOG_PUSH)
		if (0 < m_szAccurat.GetLength())
		{
			CStringArray arrayAccurats;
			SplitString(m_szAccurat, ';', arrayAccurats);
			for (int index=0; index<arrayAccurats.GetSize(); index++)
			{
				root["registration_id"].append((const char*)arrayAccurats.GetAt(index));
			}
		}
#else
		MD5Engine engine;
#if defined(_UNICODE) || defined(UNICODE)
		WcharToChar(cVal, m_sAlias, ARRAY_SZIE(cVal));
		engine.update(std::string(cVal));
#else
		engine.update(std::string(m_szAlias));
#endif

#if defined(PUBLIC_SHOW)
#if defined(_UNICODE) || defined(UNICODE)
		WcharToChar(cVal, m_sAlias, ARRAY_SZIE(cVal));
		root["alias"].append(std::string(cVal));
#else
		root["alias"].append((const char*)m_sAlias);
#endif
#else
		root["alias"].append(DigestEngine::digestToHex(engine.digest()).c_str());
#endif
#endif
	};

	if (root.empty()){
		root = "all";
	}

	return root;
}

Json::Value CJPushDlg::jsonNotification()
{
	Json::Value	root;
	if (m_cdPlatformIos.GetCheck())
	{
		root[PLATFORM_IOS] = jsonNotificationIos();
	}

	/*
	if (m_cdPlatformAndroid.GetCheck())
	{
		root[PLATFORM_ANDROID] = jsonNotificationAndroid();
	}
	*/
	

	return root;
}

Json::Value CJPushDlg::jsonNotificationIos()
{
	Json::Value	root;
#if defined(_UNICODE) || defined(UNICODE)
	CHAR cVal[1024] = { 0 };

	WcharToChar(cVal, m_szDetails, ARRAY_SZIE(cVal));

	root["alert"] = cVal;
#else
	root["alert"] = std::string(m_szDetails);
#endif

	//root["subtitle"] = std::string(m_szTitle);

	root["sound"] = (const char*)getToneName((PushToneType)m_ccbTone.GetCurSel()).getToneName();

	root["badge"] = "+1";

	root[JSON_KEY_EXTRAS] = jsonNotificationExtras();

	root["content-available"] = true;

	return root;
}

Json::Value CJPushDlg::jsonNotificationAndroid()//"notification"->"android"
{
	Json::Value	root;

	root["alert"] = std::string(m_szDetails);
	root["title"] = "title";
	root["builder_id"] = "3";
	root[JSON_KEY_EXTRAS] = jsonNotificationExtras();

	return root;
}


Json::Value CJPushDlg::jsonMessageExtras()
{
	Json::Value	messageExtras;

	if (m_cdPlatformAndroid.GetCheck())
	{
		messageExtras[PLATFORM_ANDROID] = jsonNotificationAndroid();
	}

	if (m_cdPlatformIos.GetCheck())
	{
		messageExtras[PLATFORM_IOS] = jsonNotificationIos();
	}

	return messageExtras;
}

Json::Value CJPushDlg::jsonMessage()
{
	Json::Value	root;

	root["msg_content"] = "msg_content";
	root["title"] = "title";
	root["content_type"] = "content_type";
	root[JSON_KEY_EXTRAS] = jsonMessageExtras();

	return root;
}

Json::Value CJPushDlg::jsonSmsMessage()
{
	Json::Value	root;

	m_szDetails = _T("[德犬安防]") + m_szDetails;

	root["content"] = std::string(m_szDetails);

	root["delay_time"] = 60*1;//3分钟

	return root;
}

Json::Value CJPushDlg::jsonJpushPackge()
{
	Json::Value	root;

	if (m_cdPlatformIos.GetCheck())
	{
		root[JPUSH_PLATFORM].append(PLATFORM_IOS);
	}

	if (m_cdPlatformAndroid.GetCheck())
	{
		root[JPUSH_PLATFORM].append(PLATFORM_ANDROID);
	}

	root[JSON_KEY_AUDIENCE] = jsonAudience();

	if (m_radioTypeBoth->GetCheck())
	{
		root[JSON_KEY_NOTIFICATION] = jsonNotification();

		root[JSON_KEY_MESSAGE]		= jsonMessage();
	}
	else if (m_radioTypeNotify->GetCheck())
	{
		root[JSON_KEY_NOTIFICATION] = jsonNotification();
	}
	else
	{
		root[JSON_KEY_MESSAGE]		= jsonMessage();
	}
	

	root[JSON_KEY_OPTIONS] = jsonOptions();

	return root;
}

vector<std::string>  CJPushDlg::jsonJpushPackges()
{
	vector<std::string> vectorPackges;

	Json::Value packgesDisableSms;
	Json::Value packgesSmsEnableAndroid;
	Json::Value packgesSmsEnableIos;

	Json::FastWriter writer;


	m_vectorSendno.clear();

	if (m_cdPlatformAndroid.GetCheck() && m_cbEnableSmsForAndroid.IsWindowEnabled() && m_cbEnableSmsForAndroid.GetCheck())
	{
		if (m_cdPlatformIos.GetCheck())
		{
			packgesSmsEnableIos = jsonJpushPackge();
			packgesSmsEnableIos[JPUSH_PLATFORM].clear();
			packgesSmsEnableIos[JPUSH_PLATFORM].append(PLATFORM_IOS);
			packgesSmsEnableIos[JSON_KEY_MESSAGE][JSON_KEY_EXTRAS].removeMember(PLATFORM_ANDROID);
			m_vectorSendno.push_back(packgesSmsEnableIos[JSON_KEY_OPTIONS][JSON_KEY_SENDNO].asInt());
			vectorPackges.push_back(writer.write(packgesSmsEnableIos).data());
		}

		packgesSmsEnableAndroid[JSON_KEY_AUDIENCE] = jsonJpushPackge()[JSON_KEY_AUDIENCE];		
		packgesSmsEnableAndroid[JSON_KEY_MESSAGE] = jsonJpushPackge()[JSON_KEY_MESSAGE];
		packgesSmsEnableAndroid[JSON_KEY_MESSAGE][JSON_KEY_EXTRAS].removeMember(PLATFORM_IOS);
		packgesSmsEnableAndroid[JPUSH_PLATFORM].append(PLATFORM_ANDROID);
		packgesSmsEnableAndroid[JSON_KEY_SMS_MESSAGE] = jsonSmsMessage();
		packgesSmsEnableAndroid[JSON_KEY_OPTIONS] = jsonJpushPackge()[JSON_KEY_OPTIONS];
		packgesSmsEnableAndroid[JSON_KEY_OPTIONS].removeMember(JSON_KEY_APNS_PRODUCTION);
		m_vectorSendno.push_back(packgesSmsEnableAndroid[JSON_KEY_OPTIONS][JSON_KEY_SENDNO].asInt());
		vectorPackges.push_back(writer.write(packgesSmsEnableAndroid).data());

	}
	else
	{
		packgesDisableSms = jsonJpushPackge();

		m_vectorSendno.push_back(packgesDisableSms[JSON_KEY_OPTIONS][JSON_KEY_SENDNO].asInt());
		vectorPackges.push_back(writer.write(packgesDisableSms).data());
	}

	return vectorPackges;
}

BOOL CJPushDlg::checkValid()
{
	BOOL bRet = FALSE;

	CString szMsg = _T("");

	int nLen = lstrlen(JPUSH_APPKEY);

	do 
	{
		if ((0 == m_radioTypeNotify->GetCheck()) && (0 == m_radioTypeMessage->GetCheck()) && (0 == m_radioTypeBoth->GetCheck()))
		{
			MessageBox(_T("类型：通知、消息、两者三选一！"), TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		if (!(m_cdPlatformIos.GetCheck() || m_cdPlatformAndroid.GetCheck()))
		{
			MessageBox(_T("推送平台至少选一种！"), TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		if (m_cdPlatformIos.GetCheck())
		{
			if ((0 == m_radioDeveloper->GetCheck()) && (0 == m_radioProdution->GetCheck()))
			{
				MessageBox(_T("iOS环境二选一！"), TOOL_INFO, MB_OK | MB_ICONERROR);
				break;
			}
		}

		if (!m_radioTypeMessage->GetCheck() && jsonNotification().empty())
		{
			MessageBox(_T("请在推送平台处勾选ios"), TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		if (m_sAppKey.GetLength() != nLen)
		{
			szMsg.Format(_T("Appkey 长度应该为: %d 位！"), nLen);
			m_edAppkey.SetFocus();
			m_edAppkey.SetSel(-1);
			MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}
		nLen = lstrlen(JPUSH_MASTER_SECRET);
		if (m_sMasteSecret.GetLength() != nLen)
		{
			szMsg.Format(_T("Master Secret 长度应该为: %d 位！"), nLen);
			m_edMasterSecret.SetFocus();
			m_edMasterSecret.SetSel(-1);
			MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		//接收者
		if ((0 == m_radioRecvAll->GetCheck()) && (0 == m_radioRecvAppointAlias->GetCheck()) && (0 == m_radioRecvAccurat->GetCheck()))
		{
			MessageBox(_T("接收者是谁啊？"), TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		if (m_radioRecvAppointAlias->GetCheck())
		{
			if (!m_cbTags.GetCheck() && !m_cbAlias.GetCheck())
			{
				MessageBox(_T("至少指定门锁<tag>和账户<Alias>之一"), TOOL_INFO, MB_OK | MB_ICONERROR);
				break;
			}

			if (m_cbTags.GetCheck() && (0 == m_szTags.GetLength()))
			{
				MessageBox(_T("指定门锁<tag> 不允许为空!"), TOOL_INFO, MB_OK | MB_ICONERROR);
				m_ceTags.SetFocus();
				m_ceTags.SetSel(-1);
				break;
			}

			if (m_cbTags.GetCheck() && (GDOG_DEV_LOCK_ID_LEN != m_szTags.GetLength()))
			{
				MessageBox(_T("指定门锁<tag> 长度为 10 !"), TOOL_INFO, MB_OK | MB_ICONERROR);
				m_ceTags.SetFocus();
				m_ceTags.SetSel(-1);
				break;
			}

			if (m_cbTags.GetCheck() && !m_radioTagAdmin->GetCheck() && !m_radioTagGeneral->GetCheck() && !m_radioTagBoth->GetCheck())
			{
				MessageBox(_T("指定门锁<tag> -> [管理员/普通用户/两者]"), TOOL_INFO, MB_OK | MB_ICONERROR);
				break;
			}

			if (m_cbAlias.GetCheck() && (0 == m_szAlias.GetLength()))
			{
				MessageBox(_T("指定账户<Alias>为空"), TOOL_INFO, MB_OK | MB_ICONERROR);
				m_edAlias.SetFocus();
				m_edAlias.SetSel(-1);
				break;
			}
		}

		if (m_radioRecvAccurat->GetCheck() && (0 == m_szAccurat.GetLength()))
		{
			MessageBox(_T("指定->精准->为空"), TOOL_INFO, MB_OK | MB_ICONERROR);
			m_edAccurat.SetFocus();
			m_edAccurat.SetSel(-1);
			break;
		}


		// 内容类型
		if (!(m_radioCxtTypeGeneral->GetCheck() || m_radioCxtTypeUpgrad->GetCheck() || m_radioCxtTypeMakerting->GetCheck() || m_radioCxtTypeWarning->GetCheck()))
		{
			MessageBox(_T("内容类型要选定"), TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}
		if (m_radioCxtTypeWarning->GetCheck() && (-1 == m_ccbWarnningType.GetCurSel()))
		{
			MessageBox(_T("门锁报警类型要选定"), TOOL_INFO, MB_OK | MB_ICONERROR);
			m_ccbWarnningType.SetFocus();
			break;
		}
		if (-1 == m_ccbTone.GetCurSel())
		{
			MessageBox(_T("选定一种声音"), TOOL_INFO, MB_OK | MB_ICONERROR);
			m_ccbTone.SetFocus();
			break;
		}

		if (m_radioCxtTypeWarning->GetCheck() && (PCTW_MAKE_ADMIN == m_ccbWarnningType.GetCurSel()))
		{
			if (!m_radioRecvAppointAlias->GetCheck())
			{
				MessageBox(_T("<成为管理员>要求接收者为指定注册账户类型"), TOOL_INFO, MB_OK | MB_ICONERROR);
				break;
			}

			if (m_cbTags.GetCheck())
			{
				MessageBox(_T("<成为管理员>不能够指定接收者为：门锁Id."), TOOL_INFO, MB_OK | MB_ICONERROR);
				break;
			}

			if (!m_cbAlias.GetCheck())
			{
				MessageBox(_T("<成为管理员>要求接收者为指定注册账户类型"), TOOL_INFO, MB_OK | MB_ICONERROR);
				break;
			}

			if (GDOG_DEV_LOCK_ID_LEN != m_szAdminLockId.GetLength())
			{
				MessageBox(_T("<成为管理员>要求成为管理员的门锁Id长度 10 位"), TOOL_INFO, MB_OK | MB_ICONERROR);
				m_ceAdminLockId.SetFocus();
				m_ceAdminLockId.SetSel(-1);
				break;
			}
		}
		else
		{

		}

		if (m_szDetails.GetLength() > JPUSH_IOS_DETAILS_MAX)
		{
			szMsg.Format(_T("内容部分长度超过 (%d) 了！"), JPUSH_IOS_DETAILS_MAX);
			m_edDetails.SetFocus();
			m_edDetails.SetSel(-1);
			MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		if (0 == m_szDetails.GetLength())
		{
			szMsg.Format(_T("写点内容啊！"));
			m_edDetails.SetFocus();
			m_edDetails.SetSel(-1);
			MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		if (m_cbEnableSmsForAndroid.IsWindowEnabled() && m_cbEnableSmsForAndroid.GetCheck() && (JPUSH_ALERT_MAX_LEN < m_szDetails.GetLength()))
		{
			szMsg.Format(_T("内容太长，不能够超过 %d 个字。"), JPUSH_ALERT_MAX_LEN);
			m_edDetails.SetFocus();
			m_edDetails.SetSel(-1);
			MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}
		if (0 == m_szTitle.GetLength())
		{
			szMsg.Format(_T("写个标题啊！"));
			m_cdTitle.SetFocus();
			m_cdTitle.SetSel(-1);
			MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}
		
		Json::FastWriter writer;
		std::string sNotificationIos(writer.write(jsonNotificationIos()).data());

		if (sNotificationIos.length() > JPUSH_IOS_NOTIFICATION_BODY_MAX)
		{
			szMsg.Format(_T("ios:{} 部分长度超过 (%d) 了！"), JPUSH_IOS_NOTIFICATION_BODY_MAX);
			MessageBox(szMsg, TOOL_INFO, MB_OK | MB_ICONERROR);
			break;
		}

		bRet = TRUE;
	} while (0);

	return bRet;
}

void CJPushDlg::OnRadioRecvAll()
{
	// TODO: 在此添加命令处理程序代码
	setRecvType(RECV_TYPE_ALL);
}


void CJPushDlg::OnRadioRecvAppointAliasTags()
{
	// TODO: 在此添加命令处理程序代码
	setRecvType(RECV_TYPE_ALIAS_TAGS);
}


void CJPushDlg::OnRadioRecvAccurate()
{
	// TODO: 在此添加命令处理程序代码
	setRecvType(RECV_TYPE_ACCURAT);
}




void CJPushDlg::OnClickedCheckTags()
{
	CDialogEx::UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	if (m_bTags)
	{
		if (m_radioCxtTypeWarning->GetCheck() && (PCTW_MAKE_ADMIN == m_ccbWarnningType.GetCurSel()))
		{
			MessageBox(_T("当警报类型为<成为管理员>时，不允许指定门锁Id"), TOOL_INFO, MB_OK | MB_ICONERROR);

			m_bTags = FALSE;
		}
	}

	m_ceTags.EnableWindow(m_bTags);
	m_radioTagAdmin->EnableWindow(m_bTags);
	m_radioTagGeneral->EnableWindow(m_bTags);
	m_radioTagBoth->EnableWindow(m_bTags);
	

	CDialogEx::UpdateData(FALSE);
}


void CJPushDlg::OnClickedCheckAlias()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::UpdateData(TRUE);

	m_edAlias.EnableWindow(m_bAlias);

	CDialogEx::UpdateData(FALSE);
}


void CJPushDlg::OnClickedCheckPlatformIos()
{
	CDialogEx::UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	m_radioDeveloper->EnableWindow(m_cdPlatformIos.GetCheck());
	m_radioProdution->EnableWindow(m_cdPlatformIos.GetCheck());
	//m_cdTitle.EnableWindow(m_cdPlatformIos.GetCheck());
}


void CJPushDlg::OnClickedCheckPlatformAndroid()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::UpdateData(TRUE);
	m_cbEnableSmsForAndroid.EnableWindow(m_cdPlatformAndroid.GetCheck());
	CDialogEx::UpdateData(FALSE);
}

void CJPushDlg::OnRadioContentTypeGeneral()
{
	// TODO: 在此添加命令处理程序代码
	setPushContentType(PCT_GENERAL);
}


void CJPushDlg::OnRadioContentTypeUpdate()
{
	// TODO: 在此添加命令处理程序代码
	setPushContentType(PCT_UPGRADE);
}


void CJPushDlg::OnRadioContentTypeMarketing()
{
	// TODO: 在此添加命令处理程序代码
	setPushContentType(PCT_MARKETING);
}


void CJPushDlg::OnRadioContentTypeWarning()
{
	// TODO: 在此添加命令处理程序代码
	setPushContentType(PCT_WARNING);
}


void CJPushDlg::OnRadioPushTypeNotify()
{
	// TODO: 在此添加命令处理程序代码
	m_cdPlatformAndroid.EnableWindow(!m_radioTypeNotify->GetCheck());
	m_cdPlatformAndroid.SetCheck(FALSE);
}


void CJPushDlg::OnRadioPushTypeMessage()
{
	// TODO: 在此添加命令处理程序代码

	m_cdPlatformAndroid.EnableWindow(!m_radioTypeNotify->GetCheck());
}


void CJPushDlg::OnRadioPushTypeBoth()
{
	// TODO: 在此添加命令处理程序代码

	m_cdPlatformAndroid.EnableWindow(!m_radioTypeNotify->GetCheck());
}


void CJPushDlg::OnSelchangeComboContentTypeWarning()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::UpdateData(TRUE);
	m_ceAdminLockId.EnableWindow(PCTW_MAKE_ADMIN == m_ccbWarnningType.GetCurSel());
	if (PCTW_MAKE_ADMIN == m_ccbWarnningType.GetCurSel())
	{
		m_bTags = FALSE;
	}
	CDialogEx::UpdateData(FALSE);
}
