// BizPhoneSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "BizPhoneSettingDlg.h"
#include "AboutDlg.h"

#ifdef USE_SERIAL_CODE
#include "DlgSerial.h"
#endif

#include "stackdialog.h"
#include "PropPageLockLac.h"
#include "PropPageSys.h"
#include "PropPageLocal.h"
#include "PropPageDirectTable.h"
#include "PropPageForbidTable.h"
#include "PropPageLocalMobileSect.h"
#include "PropPageLocalUserMobileSect.h"
#include "PropPageRefuseTable.h"
#include "PropPagePinSim.h"
#include "PropPageLock.h"
#include "PropPageIP.h"
#include "PropPageMic.h"
#include "PropPageOutput.h"
#include "PropPageData.h"

#if defined(NETWORK_CONFIG)
#include "PropPageEmCallCfg.h"
#include "PropPageOpnCfg.h"
#endif

#if defined(ENABLE_MOBILE_HEAD)
#include "PropPageHeader.h"
#endif

#ifdef    CARD_ENCRYPT
#include "PropPageEncrypt.h"
#endif

#if !defined(NO_NXT)
#include "PropPageScroll.h"
#include "PropPageLimitSms.h"
#if defined(CXT_EX)
#include "PropPageG166Tcpip.h"
#endif
#endif

#if defined (LOCK_SERVICE_PROVIDER)
#include "ProPageLockOp.h"
#endif

#ifdef _XIAMEN_START_BUILD
#include "PropPageXiamen.h"
#endif // _XIAMEN_START_BUILD

#ifdef _YINGNUO_SECURITY_BUILD
#include "PropPageYingnuo.h"
#endif // _YINGNUO_SECURITY_BUILD

#ifdef _TONGZE_PARA_BUILD
#include "PropPageTongze.h"
#endif // _TONGZE_PARA_BUILD

#include "Inifile.h"
#include "SendDataFIFO.h"
#include "CommThread.h"
#include "InputPassDlg2.h"
#include "SplashWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************/
/*                           属性面页初始化                             */
/************************************************************************/

PropTreeNote pageNode[PAGE_TOTAL] = 
{
	PROP_TREE_NOTE_INIT(PROPPAGE_SYS,                        IDD_PROPPAGE_1_SYS,                         _T("System")),
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCAL_AREA,                 IDD_PROPPAGE_2_LOCAL,                       _T("Area Code")),
	PROP_TREE_NOTE_INIT(PROPPAGE_PIN_SIM,                    IDD_PROPPAGE_8_PIN_SIM,                     _T("PIN & SIM")),
#if defined(LOCK_SERVICE_PROVIDER)
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCK,                       IDD_PROPPAGE_9_LOCK,                        _T("Lock Region")),
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCK_OP,                    IDD_PROPPAGE_LOCK_OP,                       _T("Lock Op")),
#else 
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCK,                       IDD_PROPPAGE_9_LOCK,                        _T("Lock")),
#endif
	PROP_TREE_NOTE_INIT(PROPPAGE_DIRECT,                     IDD_PROPPAGE_3_DIRECT_TABLE,                _T("Hot")),
	PROP_TREE_NOTE_INIT(PROPPAGE_FORBID,                     IDD_PROPPAGE_4_FORBID_TABLE,                _T("Forbidden")),
	PROP_TREE_NOTE_INIT(PROPPAGE_REFUSE,                     IDD_PROPPAGE_7_REFUSE_TABLE,                _T("Restricted")),
#if defined(ENABLE_MOBILE_HEAD)
	PROP_TREE_NOTE_INIT(PROPPAGE_MOBILE_HEADER,              IDD_PROPPAGE_MOBILE_HEADER,                 _T("Mobile Header")),
#endif
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCAL_MOBILESECT,           IDD_PROPPAGE_5_LOCAL_MOBILESECT,            _T("Local Mobile")),
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCAL_USER_MOBILESECT,      IDD_PROPPAGE_6_LOCAL_USER_MOBILESECT,       _T("User defined")),
	PROP_TREE_NOTE_INIT(PROPPAGE_IP,                         IDD_PROPPAGE_A_IP,                          _T("IP Dial")),
	PROP_TREE_NOTE_INIT(PROPPAGE_MIC,                        IDD_PROPPAGE_C_MIC,                         _T("Tone")),
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCK_LAC,                   IDD_PROPPAGE_10_LOCK_LAC,                   _T("Lock Lac")),
#if !defined(NO_NXT)
	PROP_TREE_NOTE_INIT(PROPPAGE_SCROLL,                     IDD_PROPPAGE_SCROLL,                        _T("Scroll")),
	PROP_TREE_NOTE_INIT(PROPPAGE_LIMIT,                      IDD_PROPPAGE_L_LIMIT,                       _T("Limit")),
#endif

#if defined(CXT_EX)
	PROP_TREE_NOTE_INIT(PROPPAGE_TCPIP,                      IDD_PROPPAGE_G166_TCPIP,                    _T("TcpIp")),
#endif

#if defined(NETWORK_CONFIG)
	PROP_TREE_NOTE_INIT(PROPPAGE_EMERGENCY,                  IDD_PROPPAGE_EMERGENCY_CALL,                _T("Emergency Call")),
	PROP_TREE_NOTE_INIT(PROPPAGE_OPN_CFG,                    IDD_PROPPAGE_OPN_CFG,                       _T("NetWork Name")),
#endif

#if defined(CARD_ENCRYPT)
	PROP_TREE_NOTE_INIT(PROPPAGE_ENCRYPTED,                  IDD_PROPPAGE_16_ENCRYPTED_SOLUTION,         _T("Encrypt")),
#endif
	PROP_TREE_NOTE_INIT(PROPPAGE_OUTPUT,                     IDD_PROPPAGE_X_OUTPUT,                      _T("Info")),
	PROP_TREE_NOTE_INIT(PROPPAGE_DATA,                       IDD_PROPPAGE_B_DATA,                        _T("Admin"))
};
PropPageID getPageIdBySrcId(int nSrcId)
{
	int Index = 0;
	for (Index=0; Index<(int)PAGE_TOTAL; Index++)
	{
		if (nSrcId == pageNode[Index].nSrcId)
			return pageNode[Index].pageId;
	}
	return PAGE_TOTAL;
}

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneSettingDlg dialog
CSerialComm MainSerial;
CMainContent MainContent;

CBizPhoneSettingDlg::CBizPhoneSettingDlg(CWnd* pParent /*=NULL*/)
: CStackDialog(IDC_SETTING_SHEET_PLACEHOLDER, CBizPhoneSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBizPhoneSettingDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	
	/*启动FIFO线程*/
	MainContent.m_pFIFOThread = AfxBeginThread(RUNTIME_CLASS(CSendDataFIFO));
	
	/*启动COMM线程*/
	MainContent.m_pCommThread = AfxBeginThread(RUNTIME_CLASS(CCommThread));
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bChangePwdAndSaveSetting = FALSE;
	m_bChangePwdAndSaveSettingAndWriteDefault = FALSE;
	m_bIsAllPasswordEmpty      = FALSE;
	
}

void CBizPhoneSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CStackDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBizPhoneSettingDlg)
	//DDX_Control(pDX, IDC_PIC_GIF_LOGO, m_ctrGifLogo);
	DDX_Control(pDX, ID_SETTING_SHEET_BTN_EXIT, m_btExit);
	DDX_Control(pDX, IDC_SETTING_CTRL_TREE, m_ctrTree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBizPhoneSettingDlg, CStackDialog)
//{{AFX_MSG_MAP(CBizPhoneSettingDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_NOTIFY(TVN_SELCHANGED, IDC_SETTING_CTRL_TREE, OnSelchangedOptionTree)
ON_WM_CLOSE()
ON_BN_CLICKED(ID_SETTING_SHEET_BTN_EXIT, OnDlgClose)
ON_WM_TIMER()
ON_WM_CREATE()
ON_WM_DESTROY()
ON_MESSAGE(WM_MSG_CHANGE_PWD_NOTIFY, OnChangePassword)
ON_MESSAGE(WM_MSG_LOAD_USER_INI_NOTIFY, OnLoadFromUserIniFile)
ON_MESSAGE(WM_MSG_SAVE_USER_INI_NOTIFY, OnSaveToUserIniFile)
ON_MESSAGE(WM_MSG_LOAD_MODULE_SETTING_NOTIFY, OnLoadModuleSetting)
ON_MESSAGE(WM_MSG_SAVE_MODULE_SETTING_NOTIFY, OnSaveModuleSetting)
ON_MESSAGE(WM_MSG_SAVE_MODULE_DEFAULT_NOTIFY, OnSaveModuleDefault)
ON_MESSAGE(WM_MSG_RECOVER_MODULE_DEFAULT_NOTIFY, OnRecoverModuleDefault)
ON_MESSAGE(WM_MSG_ACTIVATE_COMMPORT_NOTIFY, OnActivateCommPort)
ON_MESSAGE(WM_COMMNOTIFY,OnReceiveLowNotify)
ON_MESSAGE(WM_RECEIVEHIGH_NOTIFY, OnReceiveHighNotify)
ON_MESSAGE(WM_START_SEND_ATCOMMAND_NOTIFY,OnStartSendAtcommandNotify)
ON_MESSAGE(WM_READ_PARA_ERR_NOTIFY,OnReadParaErrNotify)
ON_MESSAGE(WM_READ_PARA_OK_NOTIFY,OnReadParaOkNotify)
ON_MESSAGE(WM_READ_PARA_STATE_NOTIFY,OnReadParaStateNotify)
ON_MESSAGE(WM_WRITE_PARA_ERR_NOTIFY,OnWriteParaErrNotify)
ON_MESSAGE(WM_WRITE_PARA_OK_NOTIFY,OnWriteParaOkNotify)
ON_MESSAGE(WM_WRITE_PARA_STATE_NOTIFY,OnWriteParaStateNotify)
ON_MESSAGE(WM_WRITE_TO_DEFAULT_SETTING_ERR_NOTIFY,OnWriteToDefaultSettingErrNotify)
ON_MESSAGE(WM_WRITE_TO_DEFAULT_SETTING_OK_NOTIFY,OnWriteToDefaultSettingOkNotify)
ON_MESSAGE(WM_RECOVER_DEFAULT_SETTING_ERR_NOTIFY,OnRecoverDefaultSettingErrNotify)
ON_MESSAGE(WM_RECOVER_DEFAULT_SETTING_OK_NOTIFY,OnRecoverDefaultSettingOkNotify)
ON_MESSAGE(WM_CHANGE_PASS_ERR_NOTIFY,OnChangePassErrNotify)
ON_MESSAGE(WM_CHANGE_PASS_OK_NOTIFY,OnChangePassOkNotify)
ON_MESSAGE(WM_START_MONITOR_TIMER_NOTIFY, OnStartMonitorTimerNotify)
ON_MESSAGE(WM_STOP_MONITOR_TIMER_NOTIFY, OnStopMonitorTimerNotify)
ON_MESSAGE(WM_MSG_ACTIVE_THIS_PAGE_NOTIFY, OnActiveThisPageNotify)
/////////////////////////////////////////
ON_MESSAGE(WM_START_SENDING_TIMER_NOTIFY, OnStartSendingTimerNotify)
ON_MESSAGE(WM_STOP_SENDING_TIMER_NOTIFY, OnStopSendingTimerNotify)
///////////////////////////////////////////

// 2007.06.07 add by hqr to Open/Close Comm
ON_MESSAGE(WM_OPEN_COMM_CONNECTION_NOTIFY, OnOpenCommConnectionNotify)
ON_MESSAGE(WM_CLOSE_COMM_CONNECTION_NOTIFY, OnCloseCommConnectionNotify)
// 2007.06.07 add by hqr to Open/Close Comm

// 2008.06.01 add by hqr to lock/unlock page
ON_MESSAGE(WM_LOCK_PORO_PAGE_NOTIFY, OnLockPoroPageNotify)
// 2008.06.01 add by hqr to lock/unlock page

ON_MESSAGE(WM_MSG_MAIN_WND_TREE_CONTROL_NOTIFY, OnTreeControlNotify)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneSettingDlg message handlers

void CBizPhoneSettingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CStackDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBizPhoneSettingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CStackDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBizPhoneSettingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ========================================================================== //
// My Dialog Init procedure
// ========================================================================== //

BOOL CBizPhoneSettingDlg::OnInitDialog()
{
	CStackDialog::OnInitDialog();
	
	// Create and show the splash screen.
	//CSplashWnd::ShowSplashScreen(1000, IDR_JPG_SPLASH, this, FALSE);
    MainContent.m_bDisplayOnceTime  = FALSE;
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
#ifdef USE_SERIAL_CODE
	//////////////////////////////////////////////////////////////////////////
	// 查看是否注册了（Yes->2, No->1）
	//
	// 1、没注册项的话，就计算给出custom string，要求输入注册码，不输入的话则退出运行
	//    然后验证输入码，验证不通过的话则退出运行，然后写验证码到注册表的注册项继续下一步3
	//
	// 2、有注册项的话，读出注册表项，验证注册项中的注册码，正确的话继续下一步3，不正确则退出运行
	//////////////////////////////////////////////////////////////////////////
	
	
	//	// 获得硬盘序列号
	//	CString strHardwareID = GetHardwareID();
	
	CString strRegistryItem = LoadSerialCodeFromRegistry();
	if (strRegistryItem.IsEmpty())
	{
		// 分支 1 没注册项的话
		
		// 计算给出custom string
		CString strCustomString;
		strCustomString = FormatCustomStringFromHardwareID();
		
		// 要求输入注册码
		BOOL bFlag = FALSE;
		CDlgSerial dlg;
		dlg.m_strCustomID = strCustomString;
		
		while(!bFlag)
		{
			dlg.m_strInputedSerial = "";
			if (IDCANCEL == dlg.DoModal())
			{
				QuitSoftware(0);	// 不输入的话则退出运行
				return FALSE;
			}
			
			// 验证输入码
			if (VerifyInputedSerialCode(dlg.m_strInputedSerial, TRUE))
				// 验证不通过的话则要求再次输入密码
				bFlag = TRUE;
		}
		
		// 写验证码到注册表的注册项
		strRegistryItem = dlg.m_strInputedSerial;
		SaveSerialCodeIntoRegistry(strRegistryItem);
	}
	else
	{
		// 有注册项的话
		
		// 读出注册表项
		strRegistryItem;
		
		// 验证注册项中的注册码
		if (!VerifyInputedSerialCode(strRegistryItem))
		{
			// 验证不通过的话则要求再次输入密码
			
			// 计算给出custom string
			CString strCustomString;
			strCustomString = FormatCustomStringFromHardwareID();
			
			// 要求输入注册码
			BOOL bFlag = FALSE;
			CDlgSerial dlg;
			dlg.m_strCustomID = strCustomString;
			
			while(!bFlag)
			{
				if (IDCANCEL == dlg.DoModal())
				{
					QuitSoftware(0);	// 不输入的话则退出运行
					return FALSE;
				}
				
				// 验证输入码
				if (VerifyInputedSerialCode(dlg.m_strInputedSerial))
					// 验证不通过的话则要求再次输入密码
					bFlag = TRUE;
			}
			
			// 写验证码到注册表的注册项
			strRegistryItem = dlg.m_strInputedSerial;
			SaveSerialCodeIntoRegistry(strRegistryItem);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 步骤 3， 验证运行的次数 （次数>N->4, 次数<=N ->5）
	//
	// 3、从注册表中读出运行了的次数，从注册码中读出最大运行次数，比较二者
	//
	// 4、通知超出了可以运行的最大次数，退出运行
	//
	// 5、将计数器累加然后存入注册表，然后开始运行程序
	//////////////////////////////////////////////////////////////////////////
	
	// 继续下一步3
	DWORD dRunNumber = 0;
	dRunNumber = LoadRunNumberFromRegistry();
	
	DWORD dRunMaxNum = 0;
	LoadMaxRunNumFromRegisterInfo(dRunMaxNum, strRegistryItem);
	
	
	if (dRunNumber >= dRunMaxNum)
	{
		// 4、通知超出了可以运行的最大次数，退出运行
		QuitSoftware(1, dRunMaxNum);
		return FALSE;
	}
	else
	{
		// 5、将计数器累加然后存入注册表，然后开始运行程序
		dRunNumber++;
		SaveRunNumberIntoRegistry(dRunNumber);
	}
	
	RunSoftware();
#endif
	
	MainSerial.initialize(this);
	SetTimer(DETECTTIMER, FIRSTTTIME, NULL);
	
	
	// ----------------------------------------------------------------------
	// 下面开始自定义的初始化过程
	// ----------------------------------------------------------------------
	
	m_bDataChanged = FALSE;		   // 初始化“曾经更改数据”为否
	
	
	// Set the tooltip string of CButtonXP button
	m_btExit.SetToolTipText(IDS_DLG_COMMON_CLOSE);
	
	m_ctrTree.DeleteAllItems();


	CString szVerInfo =  _T("");
	int i = 0;
	for (i=0; i<PAGE_TOTAL; i++)
	{/* 在这里做一个初始化的检查 */
		if (i != pageNode[i].pageId)
		{
			szVerInfo.Format(_T("pageNode[%d].pageId = %d"), i, pageNode[i].pageId);
			MessageBox(szVerInfo, SYS_NAME, MB_ICONSTOP);
		}
	}
	
	for (i=0; i<PAGE_TOTAL; i++)
	{
		m_htiRoot[i]  = m_ctrTree.InsertItem(pageNode[i].szName);
		AddPage(pageNode[i].nSrcId);
	}
	
	m_ctrTree.Expand(m_htiRoot[0], TVE_EXPAND);
	
	// You can not remove this line of code or there will be errors
	SetPage(IDD_PROPPAGE_B_DATA);
	
	// to show the logo gif picture
	/*	if (m_ctrGifLogo.Load(MAKEINTRESOURCE(IDR_GIF_LOGO),_T("IMAGE")))
	m_ctrGifLogo.Draw();
	*/
	GetDlgItem(IDC_PIC_GIF_LOGO)->ShowWindow(FALSE);
#if defined(USE_WAIT_WND)
	m_dlgWaitMsg.Create(IDD_WAIT_DIALOG);
#endif
	
	// 由于设置了m_ctrTree的Show Selection Always的属性
	// 如果没有设定一个选择项，系统会自动选定第一个
	m_ctrTree.SelectItem(m_htiRoot[PAGE_TOTAL-1]);

	szVerInfo =  SYS_NAME;
	szVerInfo         += _T(" ");
	szVerInfo         += VERSION_STATE;
	szVerInfo         += _T(" ");
	szVerInfo         += VERSION_ID;	
	
	SetWindowText(szVerInfo);
#if !defined(FOR_CLIENT)
	EnablePoroPage(theApp.m_bEnablePage);
#endif

	MainContent.m_sWorkPath = theApp.m_szAppPath;
    
	return TRUE;
}


// ==========================================================================
// virtual function which Must be implemented
// Create a dialog object based on dialog resource id
CStackPage *CBizPhoneSettingDlg::CreatePage(UINT nId)
{
	switch (nId) 
	{
	case IDD_PROPPAGE_1_SYS: 
		return new CPropPageSys(this); 
		
	case IDD_PROPPAGE_2_LOCAL: 
		return new CPropPageLocal(this);
		
	case IDD_PROPPAGE_3_DIRECT_TABLE:
		return new CPropPageDirectTable(this);
		
	case IDD_PROPPAGE_4_FORBID_TABLE:
		return new CPropPageForbidTable(this);
		
	case IDD_PROPPAGE_5_LOCAL_MOBILESECT:
		return new CPropPageLocalMobileSect(this);
		
	case IDD_PROPPAGE_6_LOCAL_USER_MOBILESECT:
		return new CPropPageLocalUserMobileSect(this);
		
	case IDD_PROPPAGE_7_REFUSE_TABLE:
		return new CPropPageRefuseTable(this);

#if defined(ENABLE_MOBILE_HEAD)
	case IDD_PROPPAGE_MOBILE_HEADER:
		return new CPropPageHeader(this);
#endif
		
	case IDD_PROPPAGE_8_PIN_SIM:
		return new CPropPagePinSim(this);
		
	case IDD_PROPPAGE_9_LOCK:
		return new CPropPageLock(this);
		
	case IDD_PROPPAGE_A_IP:
		return new CPropPageIP(this);
		
	case IDD_PROPPAGE_B_DATA:
		return new CPropPageData(this);
		
	case IDD_PROPPAGE_C_MIC:
		return new CPropPageMic(this);
		
#ifdef _XIAMEN_START_BUILD
	case IDD_PROPPAGE_D_XIAMEN_PARA:
		return new CPropPageXiamen(this);
#endif // _XIAMEN_START_BUILD
		
#ifdef _YINGNUO_SECURITY_BUILD
	case IDD_PROPPAGE_E_YINGNUO:
		return new CPropPageYingnuo(this);
#endif // _YINGNUO_SECURITY_BUILD
		
#ifdef _TONGZE_PARA_BUILD
	case IDD_PROPPAGE_F_TONGZE:
		return new CPropPageTongze(this);
#endif // _TONGZE_PARA_BUILD
		
#ifdef CARD_ENCRYPT
	case IDD_PROPPAGE_16_ENCRYPTED_SOLUTION:
		return new CPropPageEncrypt(this);	
#endif
		
	case IDD_PROPPAGE_X_OUTPUT:
		return new CPropPageOutput(this);

#if defined(NETWORK_CONFIG)
	case IDD_PROPPAGE_EMERGENCY_CALL:
		return new CPropPageEmCallCfg(this);

	case IDD_PROPPAGE_OPN_CFG:
		return new CPropPageOpnCfg(this);
#endif
		
#if defined(LOCK_SERVICE_PROVIDER)
	case IDD_PROPPAGE_LOCK_OP:
		return new CPropPageLockOp(this);
#endif

	case IDD_PROPPAGE_10_LOCK_LAC:
		return new CPropPageLockLac(this);

#if !defined(NO_NXT)
	case IDD_PROPPAGE_SCROLL:
		return new CPropPageScroll(this);

	case IDD_PROPPAGE_L_LIMIT:
		return new CPropPageLimitSms(this);

#if defined(CXT_EX)
	case IDD_PROPPAGE_G166_TCPIP:
	    return new CPropPageG166Tcpip(this);
#endif
#endif
		
	default:
		// here must not be reached
		ASSERT(TRUE);
	}
	
	return NULL;    // this will assert
}

// ==========================================================================
// Must be implemented but may have an ampty implementation
// or we can update the controls based on the currently active page
void CBizPhoneSettingDlg::OnPageChanged(UINT nId, BOOL bActivated)
{
	if (bActivated)
	{
		switch (nId) 
		{
		case IDD_PROPPAGE_1_SYS: 
			break;
			
		case IDD_PROPPAGE_2_LOCAL: 
			break;
			
		case IDD_PROPPAGE_3_DIRECT_TABLE:
			break;
			
		case IDD_PROPPAGE_4_FORBID_TABLE:
			break;
			
		case IDD_PROPPAGE_5_LOCAL_MOBILESECT:
			break;
			
		case IDD_PROPPAGE_6_LOCAL_USER_MOBILESECT:
			break;
			
		case IDD_PROPPAGE_7_REFUSE_TABLE:
			break;

#if defined(ENABLE_MOBILE_HEAD)
		case IDD_PROPPAGE_MOBILE_HEADER:
			break;
#endif
			
		case IDD_PROPPAGE_8_PIN_SIM:
			break;
			
		case IDD_PROPPAGE_9_LOCK:
			break;
			
		case IDD_PROPPAGE_A_IP:
			break;
			
		case IDD_PROPPAGE_B_DATA:
			break;
			
		case IDD_PROPPAGE_C_MIC:
			break;

#if !defined(NO_NXT)
		case IDD_PROPPAGE_SCROLL:
			break;
		case IDD_PROPPAGE_L_LIMIT:
			break;

#if defined(CXT_EX)
		case IDD_PROPPAGE_G166_TCPIP:
	        break;
#endif
#endif
			
#ifdef _XIAMEN_START_BUILD
		case IDD_PROPPAGE_D_XIAMEN_PARA:
			break;
#endif // _XIAMEN_START_BUILD
			
#ifdef _YINGNUO_SECURITY_BUILD
		case IDD_PROPPAGE_E_YINGNUO:
			break;
#endif // _YINGNUO_SECURITY_BUILD
			
#ifdef _TONGZE_PARA_BUILD
		case IDD_PROPPAGE_F_TONGZE:
			break;
#endif // _TONGZE_PARA_BUILD
			
#ifdef   CARD_ENCRYPT
		case IDD_PROPPAGE_16_ENCRYPTED_SOLUTION:
			break;
#endif //CARD_ENCRYPT
		case IDD_PROPPAGE_X_OUTPUT:
			break;

#if defined(NETWORK_CONFIG)
		case IDD_PROPPAGE_EMERGENCY_CALL:
			break;

		case IDD_PROPPAGE_OPN_CFG:
			break;
#endif

#if defined(LOCK_SERVICE_PROVIDER)
		case IDD_PROPPAGE_LOCK_OP:
			break;
#endif

		case IDD_PROPPAGE_10_LOCK_LAC:
			break;
		}
	}
	else
	{}
}

void CBizPhoneSettingDlg::OnSelchangedOptionTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	HTREEITEM htiNow = pNMTreeView->itemNew.hItem;
	// m_bIsLeafNode = !( m_pTreeCtrl->ItemHasChildren(htiNow) );
	
	CString pszItemText = m_ctrTree.GetItemText(htiNow);
	
	for (int Index=0; Index<(int)PAGE_TOTAL; Index++)
	{
		if (pszItemText == pageNode[Index].szName)
		{
			SetPage(pageNode[Index].nSrcId);
			return;
		}
	}
	
	*pResult = 0;
}

void CBizPhoneSettingDlg::OnOK() 
{
	OnDlgClose();
}

void CBizPhoneSettingDlg::OnCancel() 
{
	OnDlgClose();
}

void CBizPhoneSettingDlg::OnClose() 
{	
	OnDlgClose();
}

void CBizPhoneSettingDlg::OnDlgClose() 
{
	// 在关闭该对话框之前必须验证是否合法关闭，也就是当前是否在读写
	if (0 == ((CCommThread*)MainContent.m_pCommThread)->commThreadState)
	{
		// 如果有数据曾经改变，则要考虑保存
#if !defined(FOR_CLIENT)
		if (m_bDataChanged)
		{
			if (IDNO == MessageBox(_T("Parameters has been changed and not saved, exit now?"),SYS_NAME,MB_ICONQUESTION|MB_YESNO))
				return;
			else
				DestroyWindow();
		}
		else
			DestroyWindow();
#else
		DestroyWindow();
#endif
	}
	//	else
	//		MessageBox(_T("正在读写数据，此时不能退出系统！"),SYS_NAME,MB_ICONSTOP);
}

BOOL CBizPhoneSettingDlg::DestroyWindow() 
{
#if defined(USE_WAIT_WND)
	// 销毁WaitMsg Dialog
	if (m_dlgWaitMsg.GetSafeHwnd())
		m_dlgWaitMsg.DestroyWindow();
#endif
	
	return CStackDialog::DestroyWindow();
}

void CBizPhoneSettingDlg::OnDestroy() 
{
	CStackDialog::OnDestroy();
	
	// 销毁全局变量所创建的线程的操作应该在窗口销毁之后进行
	
	// tell thread to stop
	if(MainContent.m_pCommThread != NULL)
	{
		HANDLE hThread = MainContent.m_pCommThread->m_hThread;
		
		MainContent.m_pCommThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		MainContent.m_pCommThread->PostThreadMessage(WM_QUIT,0,0);
		
		BOOL bRet = WaitWithMessageLoop(hThread, 1000);
		if (!bRet)
		{
			// thread doesn't want to stopped
			//ShowStatus("Problem while killing COMM thread.");
			TRACE("Error - Problem while killing COMM thread (m_pCommThread) !!!\n");
			// TerminateThread(hThread, 3);
		}
		else
		{
			//ShowStatus("COMM Thread successfully stopped.");
			TRACE("Success - COMM Thread (m_pCommThread) successfully stopped.!!!\n");
		}
		MainContent.m_pCommThread=NULL;
	}
	
	// tell thread to stop
    if(MainContent.m_pFIFOThread != NULL)
	{
		HANDLE hThread = MainContent.m_pFIFOThread->m_hThread;
		
		MainContent.m_pFIFOThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		MainContent.m_pFIFOThread->PostThreadMessage(WM_QUIT,0,0);
		
		BOOL bRet = WaitWithMessageLoop(hThread, 1000);
		if (!bRet)
		{
			// thread doesn't want to stopped
			// ShowStatus("Problem while killing COMM thread.");
			TRACE("Error - Problem while killing COMM thread (m_pFIFOThread) !!!\n");
			// TerminateThread(hThread, 3);
		}
		else
		{
			// ShowStatus("COMM Thread successfully stopped.");
			TRACE("Success - COMM Thread (m_pFIFOThread) successfully stopped.!!!\n");
		}
		MainContent.m_pFIFOThread=NULL;
	}
}

/********************************************************************/
/*																	*/
/* Function name : WaitWithMessageLoop								*/
/* Description   : Pump messages while waiting for event			*/
/*																	*/
/********************************************************************/
BOOL CBizPhoneSettingDlg::WaitWithMessageLoop(HANDLE hEvent, int nTimeout)
{   
	DWORD dwRet;
	
	DWORD dTemp = GetTickCount();
    DWORD dwMaxTick = dTemp + nTimeout;
	
	while (1)
	{
		// wait for event or message, if it's a message, process it and return to waiting state
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, dwMaxTick - GetTickCount(), QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
		{
			// the return value minus WAIT_OBJECT_0 indicates
			// the pHandles array index of the object that satisfied the wait.
			TRACE0("WaitWithMessageLoop() event triggered.\n");
			return TRUE;
		}   
		else if (dwRet == WAIT_OBJECT_0 + 1)
		{
			// indicate New input of the type specified in the dwWakeMask
			// parameter is available in the thread's input queue
			// process window messages
			DoEvents();
		}  
		else if (dwRet == WAIT_TIMEOUT)
		{
			// The time-out interval elapsed and the conditions
			// specified by the bWaitAll and dwWakeMask parameters
			// were not satisfied.
			//  - timed out !
			return FALSE;
		}
		else if (dwRet == WAIT_ABANDONED_0)
		{
			// the return value minus WAIT_ABANDONED_0 indicates
			// the pHandles array index of an abandoned mutex 
			// object that satisfied the wait.
			return FALSE;
		}
		else if (dwRet == WAIT_FAILED)
		{
			DWORD dError = GetLastError();
			if (dError == 6)
			{
				TRACE0("ERROR_INVALID_HANDLE - The handle is invalid.\n");
				return TRUE;
			}
			else
				return FALSE;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}
void CBizPhoneSettingDlg::DoEvents(void)
{
	MSG msg;
	
	// window message         
	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}

LRESULT CBizPhoneSettingDlg::OnActiveThisPageNotify(WPARAM wParam,LPARAM lParam)
{
	PropPageID nPage = getPageIdBySrcId((int)wParam);
	if (PAGE_TOTAL == nPage)
	{
		nPage = (PropPageID)0;
	}
	// 只有当出现错误的时候，才直接选中出错的页面
	m_ctrTree.SelectItem(m_htiRoot[nPage]);
	return 0;
}

BOOL CBizPhoneSettingDlg::CloseTest(void)
{
	int nPage = 0;
	CStackPage* pStackPage = NULL;
	
	// 保存当前活动页面
	CStackPageHelper *pPageCurrent = getCurrent();
	
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it++, nPage++) 
	{
		// 注释该语句，避免在验证数据正确性的时候要不停的在各个
		// 页面之间切换造成闪动
		// m_ctrTree.SelectItem(m_htiRoot[nPage++]);			// 选中该页
		
		pStackPage = it->GetPage();
		if (pStackPage->OnCloseTest() == FALSE){
			PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
			return FALSE;
		}			
	}
	
	return TRUE;
}

BOOL CBizPhoneSettingDlg::SaveData(void)
{
	// 将各个页面数据保存到全局变量中
	CStackPage* pStackPage = NULL;
	CStackPageHelperList::iterator it;
	
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) 
	{
		pStackPage = it->GetPage();
		pStackPage->OnSaveData();
	}
	
	// 然后，开文件，并将数据写入脚本文件
	return TRUE;
}

void CBizPhoneSettingDlg::LoadIniFile(CString strLoadFile, BOOL bFromIni)
{
	// 读入INI文件
	ASSERT(!strLoadFile.IsEmpty()); 
	
	m_ArySECT_4400_strLocalTable.RemoveAll();		// 首先将原来的数据清空
	
	CIniFile m_fIniFile(strLoadFile);
	int i = 0;
	unsigned int nCommand = 0;

	BOOL   bHasNoneUsefulIni = FALSE;

#if !defined(NO_NXT)	
	for(i=0; i<MAX_PAGELIMIT_LIMITITEM_COUNT; i++){
		m_strLimit_1020_MsgAd[i] = _T("");
	}
#endif

	CStringArray strCommandAry;
	m_fIniFile.ReadSection(STRING_SECTION1_NAME, strCommandAry);
	int nCommandNum = strCommandAry.GetSize();
	CStringArray strATCommandAry;
	m_fIniFile.ReadSection(STRING_SECTION2_NAME, strATCommandAry);
	int nATCommandNum = strATCommandAry.GetSize();

#if defined(PAGE_CFG)
	CStringArray strPageCfgAry;
	CString strPageCfgCmd;
	CString strPageCfgVal;
	m_fIniFile.ReadSection(STRING_SECTION3_NAME, strPageCfgAry);
	int nPageCfgNum = strPageCfgAry.GetSize();
#endif
	
	if (nCommandNum + nATCommandNum ==0 )
	{
		// 倒入的配置文件为空，或是文件错误
		if (bFromIni) 
		{
			bHasNoneUsefulIni = TRUE;
			theApp.m_szIniInput.Empty();
			CString strInfo = _T("There is nothing valid could be matched in the script!");
			MessageBox(strInfo,SYS_NAME,MB_ICONSTOP);
			ShowStatus(strInfo, FALSE);
		}
	}
	else
	{
		ResetBizPhoneSetting();

#if defined(PAGE_CFG)
		/*
		m_pageEnable[0].nPageId = IDD_PROPPAGE_EMERGENCY_CALL;
		m_pageEnable[1].nPageId = IDD_PROPPAGE_OPN_CFG;
		m_pageEnable[2].nPageId = IDD_PROPPAGE_5_LOCAL_MOBILESECT;
		*/
		for (i=0; i<nPageCfgNum; i++)
		{
			strPageCfgCmd   = strPageCfgAry[i];
			strPageCfgVal   = m_fIniFile.ReadString(STRING_SECTION3_NAME, strPageCfgCmd, "");
			if (STR_PAGECFG_ENABLE == strPageCfgCmd)
			{
				setPageEnableStatusString(strPageCfgVal);
			}
			else if (STR_PAGECFG_COUNTEC == strPageCfgCmd)
			{/* 没有啥用处的 */
				setPageCount(IDD_PROPPAGE_EMERGENCY_CALL, atoi(strPageCfgVal));
			}
			else if (STR_PAGECFG_COUNTOC == strPageCfgCmd)
			{/* 没有啥用处的 */
				setPageCount(IDD_PROPPAGE_OPN_CFG, atoi(strPageCfgVal));
			}
			else if (STR_PAGECFG_COUNTLM == strPageCfgCmd)
			{
				setPageCount(IDD_PROPPAGE_5_LOCAL_MOBILESECT, atoi(strPageCfgVal));
			}
			else
			{//InValid pagecfg
				ASSERT(0);
			}
		}
#endif

		for (i= 0; i< nCommandNum; i++)
		{
			CString strCommand = strCommandAry[i];
			CString strValue   = m_fIniFile.ReadString(STRING_SECTION1_NAME, strCommand, "");
			
			nCommand = atoi(strCommand);
			
			// --------------------------------------------------------
			// DIRECT TABLE
			// --------------------------------------------------------
			if ( (nCommand>=INT_DIRECT_NUMBER_TABLE_CMD)&&(nCommand<INT_DIRECT_NUMBER_TABLE_CMD+INT_INIT_LISTBOX_ITEMNUM))
			{
				// [2001, 2020]
				m_AryDIRECT_2020_strDirectTable[nCommand-INT_DIRECT_NUMBER_TABLE_CMD] = strValue;
				continue;
			}
#if !defined(NO_NXT)
			if ((nCommand>=1020) && (nCommand <=1020+MAX_PAGELIMIT_LIMITITEM_COUNT))
			{
				m_strLimit_1020_MsgAd[nCommand - 1020] = strValue;
				continue;
			}
#endif
			
#if defined(NETWORK_CONFIG)
#if defined(PAGE_CFG)
			if ((nCommand >= CLEAR_EMERGENCY_SETTING) && (nCommand <= CLEAR_EMERGENCY_SETTING+EMERGENCY_NUM_INT) && ENABLE_EMERGENCY_CALL)
#else
			if ((nCommand >= CLEAR_EMERGENCY_SETTING) && (nCommand <= CLEAR_EMERGENCY_SETTING+EMERGENCY_NUM_INT))
#endif
			{
				if (nCommand > CLEAR_EMERGENCY_SETTING)
					m_AryEmCall_2610_strNumberList[nCommand - CLEAR_EMERGENCY_SETTING - 1] = strValue;
				continue;
			}

#if defined(PAGE_CFG)
			if ((nCommand >= CLEAR_OPN_CFG) && (nCommand <= CLEAR_OPN_CFG+OPN_TOTAL) && ENABLE_OPN_CFG)
#else
			if ((nCommand >= CLEAR_OPN_CFG) && (nCommand <= CLEAR_OPN_CFG+OPN_TOTAL))
#endif
			{
				if (nCommand > CLEAR_OPN_CFG)
					m_AryNetWorkName_2750_strNameList[nCommand - CLEAR_OPN_CFG - 1] = strValue;
				continue;
			}
#endif

			// --------------------------------------------------------
			// FORBID TABLE
			// --------------------------------------------------------
			if ( (nCommand>=INT_FORBID_NUMBER_TABLE_CMD)&&(nCommand<INT_FORBID_NUMBER_TABLE_CMD+INT_INIT_LISTBOX_ITEMNUM))
			{
				// [3001, 3020]
				m_AryFORBID_3020_strForbidTable[nCommand-INT_FORBID_NUMBER_TABLE_CMD] = strValue;
				continue;
			}
			
			// --------------------------------------------------------
			// REFUSE TABLE
			// --------------------------------------------------------
			if ( (nCommand>=INT_REFUSE_NUMBER_TABLE_CMD)&&(nCommand<INT_REFUSE_NUMBER_TABLE_CMD+INT_INIT_LISTBOX_ITEMNUM))
			{
				// [5001, 5020]
				m_AryREFUSE_5020_strRefuseTable[nCommand-INT_REFUSE_NUMBER_TABLE_CMD] = strValue;
				continue;
			}
			
			// --------------------------------------------------------
			// LOCAL MOBILE SECT TABLE
			// --------------------------------------------------------
			if ( (nCommand >= INT_LOCAL_SEGMENT_TABLE_CMD) && (nCommand < INT_LOCAL_SEGMENT_TABLE_CMD+INT_INIT_LOCAL_PHONE_ITEMNUM))
			{
				// [4001, 4400]
				// 按照道理来说，如果教本正确的话，此处数据应该是顺序存放的
				// 故我们这里不在校验数据的合法性
				// 我们认定：数据是按照命令号依次顺序存放的
				if ((strValue != _T("FFFFFFFF")) && (strValue != _T("00000000")))
				{
					m_ArySECT_4400_strLocalTable.Add(strValue);
				}
				continue;
			}
			
			// --------------------------------------------------------
			// LOCAL USER MOBILE SECT TABLE
			// --------------------------------------------------------
			if ( (nCommand>=INT_LOCAL_USER_SEGMENT_TABLE_CMD)&&(nCommand<INT_LOCAL_USER_SEGMENT_TABLE_CMD+INT_INIT_LISTBOX_ITEMNUM))
			{
				// [4401, 4420]
				m_ArySECT_4420_strUserLocalTable[nCommand-INT_LOCAL_USER_SEGMENT_TABLE_CMD] = strValue;
				continue;
			}
			
			// --------------------------------------------------------
			// IP RULE TABLE
			// --------------------------------------------------------
			if ( (nCommand>=INT_IP_RULE_TABLE_CMD)&&(nCommand<INT_IP_RULE_TABLE_CMD+INT_INIT_LISTBOX_ITEMNUM))
			{
				// [9001, 9021]
				m_AryIP_9001_strIPRuleTable[nCommand-INT_IP_RULE_TABLE_CMD] = strValue;
				continue;
			}
#if defined(LOCK_SERVICE_PROVIDER)	
			if ((nCommand >= INT_LOCKED_COMPANY_CMD) && (nCommand < INT_LOCKED_COMPANY_CMD+SERVICE_PROVIDER_MAX))
			{
				m_AryLOCK_7003_nProvider[nCommand - INT_LOCKED_COMPANY_CMD] = strValue;
			}
#endif

#if defined(ENABLE_MOBILE_HEAD)
			if ((nCommand >= INT_MOBILE_HEADER_STR_CMD) && (nCommand < INT_MOBILE_HEADER_STR_CMD+MAX_MOBILE_HEADER_TOTAL))
			{
				m_strMobileHeader[nCommand - INT_MOBILE_HEADER_STR_CMD] = strValue;
			}
#endif
			switch (nCommand)
			{
				// --------------------------------------------------------
				// SYS
				// --------------------------------------------------------
			case INT_AUTO_DELAY_CMD:	//	2		// 速延时时间
				m_strSYS_0002_nAutoDely = strValue;
				break;
				
			case INT_SMS_CENTER_CMD:	//	4		// 短消息中心号码
				m_strSYS_0004_sSMSCenter = strValue;
				break;
				
			case INT_IF_ADD_ZERO_FOR_EXTR_PHONE_CMD:	// 4000    //外地手机是否加零
				m_strSYS_4000_bNotLocalAddZero = strValue;
				break;
				
			case INT_IF_ENABLE_LD_CALL_CMD:	// 5    //是否允许国内长途
				m_strSYS_0005_bForbidLongDistanceCall = strValue;
				break;
				
				// --------------------------------------------------------
				// LOCAL
				// --------------------------------------------------------
			case INT_LOCAL_ZONE_CMD: 					// 1000    //本地区号
				m_strLOCAL_1000_strLocalZone = strValue;
				break;
				
#if !defined(NO_NXT)
#if defined(CXT_EX)
			case INT_G166_IPADDRESS_CMD:
				m_strG166_IpAddress = strValue;
				break;

			case INT_G166_IPPORT_CMD:
				m_strG166_IpPort = strValue;
				break;

			case INT_G166_DNS1_CMD:
				m_strG166_Dns1 = strValue;
				break;

			case INT_G166_DNS2_CMD:
				m_strG166_Dns2 = strValue;
				break;
#endif
			case INT_SCROLL_HOTLINE_CMD:			    //1001    //客服热线号码
				m_strScroll_1001_HotLine = strValue;
				break;
				
				
			case INT_SCROLL_NXTNUM_CMD: 		    	//1002    //农信通号码
				m_strScroll_1002_NxtNum = strValue;
				break;
				
				
			case INT_SCROLL_MOBILESEC_CMD:		       //1003    //移动秘书号码
				m_strScroll_1003_MobileSec = strValue;
				break;
				
				
			case INT_SCROLL_ISSUANCENUM_CMD: 		  //1004    //滚动条发布号码1
				m_strScroll_1004_IssuanceNum[0] = strValue;
				break;
				
				
			case INT_SCROLL_ISSUANCENUM_CMD+1: 		  //1005    //滚动条发布号码2
				m_strScroll_1004_IssuanceNum[1] = strValue;
				break;
				
				
			case INT_SCROLL_ISSUANCENUM_CMD+2: 		  //1006    //滚动条发布号码3
				m_strScroll_1004_IssuanceNum[2] = strValue;
				break;
				
			case INT_SCROLL_ISSUANCENUM_CMD+3: 		  //1007    //滚动条发布号码4
				m_strScroll_1004_IssuanceNum[3] = strValue;
				break;
				
				
			case INT_SCROLL_ISSUANCENUM_CMD+4: 		  //1008    //滚动条发布号码5
				m_strScroll_1004_IssuanceNum[4] = strValue;
				break;
				
				
			case INT_SCROLL_ISSUANCENUM_CMD+5: 		  //1009   //滚动条发布号码6
				m_strScroll_1004_IssuanceNum[5] = strValue;
				break;
				
				
			case INT_SCROLL_ISSUANCENUM_CMD+6: 		  //1010    //滚动条发布号码7
				m_strScroll_1004_IssuanceNum[6] = strValue;
				break;
				
				
			case INT_SCROLL_NXTMENUTITLE_CMD: 		  //13000   //农信通菜单标题
				m_strScroll_13000_NxtTitle = strValue;
				break;
				
				
			case INT_SCROLL_MATTER_CMD:	             // 13001   //滚动条内容
				m_strScroll_13001_ScrollMatter = strValue;
				break;
				
				
			case INT_SCROLL_MATTERAD_CMD: 		    // 13002   //滚动条字头
				m_strScroll_13002_ScrollAd = strValue;
				break;
#endif			
			case INT_LOCAL_METHOD_CMD:					// 3       // 本地区号使用方式
				m_strLOCAL_0003_nLocalMethod = strValue;
				break;
				
				// --------------------------------------------------------
				// PINSIM
				// --------------------------------------------------------
			case INT_IF_AUTO_LOCK_PIN_CMD:				// 6000    //读是否自动锁PIN码
				m_strPINSIM_6000_bAutoLockPin = strValue;
				break;
				
			case INT_SET_PIN_LENGTH_CMD:				// 6001    //读设置PIN码长度
				m_strPINSIM_6001_nPinLength = strValue;
				break;
				
			case INT_SET_INIT_PIN_CODE_CMD:				// 6002    //读设置初始PIN码
				m_strPINSIM_6002_strOriginalPin = strValue;
				break;
				
			case INT_SET_FIXED_PIN_CODE_CMD:			// 6004    //读设置固定PIN码
				m_strPINSIM_6004_strFixedPin = strValue;
				break;
				
			case INT_IF_USE_RANDOM_PIN_CMD:				// 6005    //读是否使用随机PIN码
				m_strPINSIM_6005_bUseRandomPin = strValue;
				break;
				
			case INT_IF_BIND_SIM_CARD_CMD:				// 7000		//读话机是否绑定SIM卡
				m_strPINSIM_7000_bLockSim = strValue;
				break;
				
			case INT_BIND_SIM_CARD_NUMBER_CMD:			// 7001    //读话机绑定的SIM卡号
				if (strValue.GetLength()>19)
					m_strPINSIM_7001_strLockSimNum = strValue.Left(19);
				else
					m_strPINSIM_7001_strLockSimNum = strValue;
				break;
				
				// --------------------------------------------------------
				// LOCK
				// --------------------------------------------------------
			case INT_IF_LOCK_COMPANY_CMD:				// 7002    //读是否锁定运营商
				m_strLOCK_7002_bLockProvider = strValue;
				break;
#if !defined(LOCK_SERVICE_PROVIDER)		
			case INT_LOCKED_COMPANY_CMD:				// 7003    //读锁定的运营商
				m_strLOCK_7003_nProvider = strValue;
				break;
#endif
			case INT_IF_LOCK_DISTRICT_CMD:				// 8000    //读是否锁小区
				m_strLOCK_8000_bLockRegion = strValue;
				break;
				
			case INT_IF_AUTO_LOCK_DISTRICT_CMD:			// 8002    //读是否自动锁小区
				m_strLOCK_8002_bAutoLockRegion = strValue;
				break;
				
			case INT_LOCKED_DISTRICT_NUMBER_CMD:		// 8001	//读锁定的小区数
				m_strLOCK_8001_nRegionNum = strValue;
				break;
				
			case INT_LOCKED_DISTRICT_CODE_CMD:			// 8003    //读已经锁定的小区号
				m_strLOCK_8003_strLockRegion = strValue;
				break;

			case INT_IF_LOCK_LAC_CMD:                    // 8004    // read the if lock the Lac
				m_strLOCK_8004_bLockLac = strValue;
				break;
				
			case INT_LOCKED_LAC_CODE_CMD:                 // 8005    // read the first half of the Max lac no.  
				m_strLOCK_8005_strLockLac = strValue;
				break;
				
			case INT_LOCKED_LAC_CODE_CMD2:                // 8006    // read the second half of the Max lac no.
				m_strLOCK_8006_strLockLac = strValue;
				break;
				
				// --------------------------------------------------------
				// REFUSE
				// --------------------------------------------------------
			case INT_REFUSE_TYPE_CMD:				// 5000	//是否限制呼入所有来电
				m_strREFUSE_5000_bRefuseType = strValue;
				break;
				
#ifdef  _XIAMEN_START_BUILD
				// --------------------------------------------------------
				// Xiamen Para
				// --------------------------------------------------------
			case INT_MSC_RECEIVER_CMD:	//	6		// 接收网管中心号码
				m_strXIAMEN_0006_sMscReceiveNum = strValue;
				break;
				
			case INT_IF_SHOW_ORIGINAL_NUM_CMD:		// 7		// 是否只显示原始号码
				m_strXIAMEN_0007_bShowOriginalNum = strValue;
				break;
				
			case INT_PRODUCER_ID_CMD:			//	9		// 生产商ID信息
				m_strXIAMEN_0009_sProducerID = strValue;
				break;
				
			case INT_IF_LOCAL_SECT_ENABLED_CMD:		// 11		// 本地手机号段是否生效
				m_strXIAMEN_0011_bLocalSectEnabled = strValue;
				break;
#endif  //_XIAMEN_START_BUILD
				
#ifdef  _YINGNUO_SECURITY_BUILD
				// --------------------------------------------------------
				// YINGNUO Para
				// --------------------------------------------------------
			case INT_SECURITY_SETTING1_CMD:				// 20		// 第1组密钥
				strValue.MakeUpper();
				m_strYINGNUO_0020_sSecurityNum0 = strValue;
				break;
				
			case INT_SECURITY_SETTING2_CMD:				// 21		// 第2组密钥
				strValue.MakeUpper();
				m_strYINGNUO_0021_sSecurityNum1 = strValue;
				break;
				
			case INT_SECURITY_SETTING3_CMD:				// 22		// 第3组密钥
				strValue.MakeUpper();
				m_strYINGNUO_0022_sSecurityNum2 = strValue;
				break;
				
			case INT_SECURITY_SETTING4_CMD:				// 23		// 第4组密钥
				strValue.MakeUpper();
				m_strYINGNUO_0023_sSecurityNum3 = strValue;
				break;
				
			case INT_SECURITY_SETTING5_CMD:				// 24		// 第5组密钥
				strValue.MakeUpper();
				m_strYINGNUO_0024_sSecurityNum4 = strValue;
				break;
				
#endif  //_YINGNUO_SECURITY_BUILD
				
#ifdef  _TONGZE_PARA_BUILD
				// --------------------------------------------------------
				// Tongze Para
				// --------------------------------------------------------
			case INT_PRODUCER_ID_CMD:			//	9		// 生产商ID信息
				m_strTONGZE_0009_sProducerID = strValue;
				break;
				
			case INT_IF_LOCAL_SECT_ENABLED_CMD:		// 11		// 本地手机号段是否生效
				m_strTONGZE_0011_bLocalSectEnabled = strValue;
				break;
#endif  //_TONGZE_PARA_BUILD
				
#ifdef  CARD_ENCRYPT
			case ENCRYPT_SOLUTION_CMD:
				m_strStartNet_0019_sEncryptSolution = strValue;
				break;
				
			case VENDOR_SERIAL_NO_CMD:
				m_strStartNet_0017_sVenderNO = strValue;
				break;
			case MAIN1_ENCRYPTED_KEY_CMD:
				m_strStartNet_0020_sKeyword1 = strValue;
				break;
			case  MAIN2_ENCRYPTED_KEY_CMD:
				m_strStartNet_0021_sKeyword2 = strValue;
				break;
			case MAIN3_ENCRYPTED_KEY_CMD:
				m_strStartNet_0022_sKeyword3 = strValue;
				break;
			case   MAIN4_ENCRYPTED_KEY_CMD:
				m_strStartNet_0023_sKeyword4 = strValue;
				break;
			case  MAIN5_ENCRYPTED_KEY_CMD:
				m_strStartNet_0024_sKeyword5 = strValue;
				break;
				
			case KEY_SERIAL_NO_CMD:
				m_strStartNet_0031_sKeyNO = strValue;
				break;
				
#endif
				
			}// end of switch (nCommand)
		}// end of for (int i= 0; i< nCommandNum; i++)
		
		for (i= 0; i< nATCommandNum; i++)
		{
			CString strATCommand = strATCommandAry[i];
			CString strATValue   = m_fIniFile.ReadString(STRING_SECTION2_NAME, strATCommand, "");
			
			// --------------------------------------------------------
			// MIC
			// --------------------------------------------------------
			
			if (strATCommand == STR_CMIC0_CMD)					//1	"AT+CMIC0" // 设置主通道麦克风增益
			{
				// strATValue 形为 "0,2" 
				ASSERT( strATValue.Left(1)== "0" );
				m_strMIC_CMIC0_nCMICMain = strATValue.Mid(strATValue.Find(',')+1);
			}
			else if (strATCommand == STR_CMIC1_CMD)				//2	"AT+CMIC1" // 设置辅助通道麦克风增益
			{
				// strATValue 形为 "1,2" 
				ASSERT( strATValue.Left(1)== "1" );
				m_strMIC_CMIC1_nCMICAux = strATValue.Mid(strATValue.Find(',')+1);
			}
#if defined(R16_ATC)
			else if (strATCommand == STR_SIDET0_CMD)				//3	"AT+SIDET0" // 设置主侧音增益
			{
				ASSERT( strATValue.Left(1) == "0" );
				if (0 < strATValue.Find('+'))
				{// Format-R: AT+SIDET0=0,4096+SIDET(AUX_AUDIO): 0
					m_strMIC_SIDET_MainGain = strATValue.Mid(strATValue.Left(strATValue.Find('+')).Find(',')+1);
					m_strMIC_SIDET_AuxGain  = strATValue.Mid(strATValue.Find(':') + 2);
				}
				else
				{// Format-W: AT+SIDET0=0,4096					
					m_strMIC_SIDET_MainGain = strATValue.Mid(strATValue.Find(',')+1);
				}
			}
			else if (strATCommand == STR_SIDET1_CMD)				//3	"AT+SIDET1" // 设置辅侧音增益
			{
				ASSERT( strATValue.Left(1) == "1" );
				m_strMIC_SIDET_AuxGain  = strATValue.Mid(strATValue.Find(',') + 1);
			}
#else
			else if (strATCommand == STR_SIDET_CMD)				//3	"AT+SIDET" // 设置侧音增益
			{
				// strATValue 形为 "4096" 
				m_strMIC_SIDET_Gain = strATValue;
			}
#endif
			else if (strATCommand == STR_ECHO0_CMD)				//4	"AT+ECHO0" // 设置主通道回波抑制
			{
				// strATValue 形为 "0,0,0,0" 
				ASSERT( strATValue.Right(1)== "0" );
				
				// 设置主通道 - 回波抑制 - 耳机耦合系数
				int nPos = strATValue.Find(',');
				m_strMIC_ECHO0_Vox = strATValue.Left(nPos);
				
				// 设置主通道 - 回波抑制 - 麦克风阈值
				int nNext = strATValue.Find(',', nPos+1);
				m_strMIC_ECHO0_Min = strATValue.Mid(nPos+1, nNext-nPos-1);
				nPos = nNext;
				
				// 设置主通道 - 回波抑制 - 回波抑制持续
				nNext = strATValue.Find(',', nPos+1);
				m_strMIC_ECHO0_Samp = strATValue.Mid(nPos+1, nNext-nPos-1);
			}
			else if (strATCommand == STR_ECHO1_CMD)				//5	"AT+ECHO1" // 设置辅助通道回波抑制
			{
				// strATValue 形为 "0,0,0,1" 
				ASSERT( strATValue.Right(1)== "1" );
				
				// 设置辅助通道 - 回波抑制 - 耳机耦合系数
				int nPos = strATValue.Find(',');
				m_strMIC_ECHO1_Vox = strATValue.Left(nPos);
				
				// 设置辅助通道 - 回波抑制 - 麦克风阈值
				int nNext = strATValue.Find(',', nPos+1);
				m_strMIC_ECHO1_Min = strATValue.Mid(nPos+1, nNext-nPos-1);
				nPos = nNext;
				
				// 设置辅助通道 - 回波抑制 - 回波抑制持续
				nNext = strATValue.Find(',', nPos+1);
				m_strMIC_ECHO1_Samp = strATValue.Mid(nPos+1, nNext-nPos-1);
			}
			else if (strATCommand == STR_CTRS_CMD)				//6	"AT+CTRS" // 设置辅助通道麦克风增益
			{
				// strATValue 形为 "10" 
				m_strSYS_LCD_Contrast = strATValue;
			}
			else
				ASSERT(0);	// some thing error happened here
		}
	}

	if (bFromIni) 
	{
		CString strInfo; 		
		strInfo.Format(_T("successful to import script[%s]..."), strLoadFile.Mid(strLoadFile.ReverseFind('\\')+1));
		if (!bHasNoneUsefulIni){
			if ((NULL == strstr(strLoadFile, STRING_PARAFILE_NAME)) && 
				(NULL == strstr(strLoadFile, PARAFILE)))
			{
				theApp.m_szIniInput = strLoadFile;
			}
			else
			{
				theApp.m_szIniInput.Empty();
			}
		}
		
		ShowStatus(strInfo);
	}
	
	// 刷新所有的页面
	RefreshPage();
	
	// 标记数据已经修改过了
	m_bDataChanged = TRUE;
}

#if defined(PAGE_CFG)
void CBizPhoneSettingDlg::SavePageOptionToIni(CString szIniFilePath)
{
	CIniFile m_fIniFile(szIniFilePath);

	CString  strATValue = _T("");
	/*
	m_pageEnable[0].nPageId = IDD_PROPPAGE_EMERGENCY_CALL;
	m_pageEnable[1].nPageId = IDD_PROPPAGE_OPN_CFG;
	m_pageEnable[2].nPageId = IDD_PROPPAGE_5_LOCAL_MOBILESECT;
	*/
	getPageEnableStatusString(strATValue);
	strATValue = _T("\"") + strATValue;
	strATValue = strATValue + _T("\"");
	m_fIniFile.WriteString(STRING_SECTION3_NAME, STR_PAGECFG_ENABLE, strATValue); 

	strATValue.Format(_T("\"%d\""), getPageCount(IDD_PROPPAGE_EMERGENCY_CALL));
	m_fIniFile.WriteString(STRING_SECTION3_NAME, STR_PAGECFG_COUNTEC, strATValue);

	strATValue.Format(_T("\"%d\""), getPageCount(IDD_PROPPAGE_OPN_CFG));
	m_fIniFile.WriteString(STRING_SECTION3_NAME, STR_PAGECFG_COUNTOC, strATValue);

	strATValue.Format(_T("\"%d\""), getPageCount(IDD_PROPPAGE_5_LOCAL_MOBILESECT));
	m_fIniFile.WriteString(STRING_SECTION3_NAME, STR_PAGECFG_COUNTLM, strATValue);
}
#endif

void CBizPhoneSettingDlg::SaveIniFile(CString strSaveFile, BOOL bFromIni)
{
	ASSERT(!strSaveFile.IsEmpty()); 
	unsigned int i = 0;
	
	// 写出INI文件
	CIniFile m_fIniFile(strSaveFile);
	CString strCommand(""), strValue("");
	
	strCommand.Format("%d", INT_AUTO_DELAY_CMD);							//	2		// 速延时时间
	strValue = m_strSYS_0002_nAutoDely;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_LOCAL_METHOD_CMD);							//  3       // 本地区号使用方式
	strValue = m_strLOCAL_0003_nLocalMethod;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d", INT_SMS_CENTER_CMD);							//	4		// 短消息中心号码
	strValue = m_strSYS_0004_sSMSCenter;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d", INT_IF_ENABLE_LD_CALL_CMD);						//	5		// 是否允许国内长途
	strValue = m_strSYS_0005_bForbidLongDistanceCall;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
#ifdef  _XIAMEN_START_BUILD
	strCommand.Format("%d",  INT_MSC_RECEIVER_CMD);							//  6       // 接收网管中心号码
	strValue = m_strXIAMEN_0006_sMscReceiveNum;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d", INT_IF_SHOW_ORIGINAL_NUM_CMD);					//	7		// 是否只显示原始号码
	strValue = m_strXIAMEN_0007_bShowOriginalNum;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d", INT_PRODUCER_ID_CMD);							//	9		// 生产商ID信息
	strValue = m_strXIAMEN_0009_sProducerID;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d", INT_IF_LOCAL_SECT_ENABLED_CMD);					//	11		// 本地手机号段是否生效
	strValue = m_strXIAMEN_0011_bLocalSectEnabled;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
#endif  //_XIAMEN_START_BUILD
	
#ifdef  _YINGNUO_SECURITY_BUILD	
	strCommand.Format("%d",  INT_SECURITY_SETTING1_CMD);					//  20       // 第1组密钥
	strValue = m_strYINGNUO_0020_sSecurityNum0;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_SECURITY_SETTING2_CMD);					//  21       // 第2组密钥
	strValue = m_strYINGNUO_0021_sSecurityNum1;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_SECURITY_SETTING3_CMD);					//  22       // 第3组密钥
	strValue = m_strYINGNUO_0022_sSecurityNum2;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_SECURITY_SETTING4_CMD);					//  23       // 第4组密钥
	strValue = m_strYINGNUO_0023_sSecurityNum3;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_SECURITY_SETTING5_CMD);					//  24       // 第5组密钥
	strValue = m_strYINGNUO_0024_sSecurityNum4;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
#endif  //_YINGNUO_SECURITY_BUILD
	
#ifdef  _TONGZE_PARA_BUILD
	strCommand.Format("%d", INT_PRODUCER_ID_CMD);							//	9		// 生产商ID信息
	strValue = m_strTONGZE_0009_sProducerID;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d", INT_IF_LOCAL_SECT_ENABLED_CMD);					//	11		// 本地手机号段是否生效
	strValue = m_strTONGZE_0011_bLocalSectEnabled;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
#endif  //_TONGZE_PARA_BUILD
	
#ifdef CARD_ENCRYPT
	/*	
	strCommand.Format("%d", COPATIBLE_COMMON_CARD_CMD);						//	18		// if compatible card
	strValue = m_strStartNet_0018_sIfCompatibleCard;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	*/	
	strCommand.Format("%d", ENCRYPT_SOLUTION_CMD);						//	19		// Encrypted solution
	strValue = m_strStartNet_0019_sEncryptSolution;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	if (m_strStartNet_0019_sEncryptSolution.Find(_T("13")) == 0)
	{
		strCommand.Format("%d", VENDOR_SERIAL_NO_CMD);						//	17		// vendor NO.
		strValue = m_strStartNet_0017_sVenderNO;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
		
		strCommand.Format("%d", MAIN1_ENCRYPTED_KEY_CMD);						//	20		// keyword1
		strValue = m_strStartNet_0020_sKeyword1;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
		
		strCommand.Format("%d", MAIN2_ENCRYPTED_KEY_CMD);						//	21		// keyword2
		strValue = m_strStartNet_0021_sKeyword2;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
		
		strCommand.Format("%d", MAIN3_ENCRYPTED_KEY_CMD);						//	22		// keyword3
		strValue = m_strStartNet_0022_sKeyword3;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
		
		strCommand.Format("%d", MAIN4_ENCRYPTED_KEY_CMD);						//23		// keyword4
		strValue = m_strStartNet_0023_sKeyword4;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
		
		strCommand.Format("%d", MAIN5_ENCRYPTED_KEY_CMD);						//	24		// keyword5
		strValue = m_strStartNet_0024_sKeyword5;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
		
		strCommand.Format("%d", KEY_SERIAL_NO_CMD);								//	31		// Key No
		strValue = m_strStartNet_0031_sKeyNO;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
		
	}
	
#endif //CARD_ENCRYPT
	
	strCommand.Format("%d", INT_LOCAL_ZONE_CMD); 							// 1000    //本地区号
	strValue = m_strLOCAL_1000_strLocalZone;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 

#if !defined(NO_NXT)
#if defined(CXT_EX)
	strCommand.Format("%d", INT_G166_IPADDRESS_CMD);                           // 981    IP地址
	strValue = m_strG166_IpAddress;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 

	strCommand.Format("%d", INT_G166_IPPORT_CMD);                              // 982    IP端口
	strValue = m_strG166_IpPort;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 

	strCommand.Format("%d", INT_G166_DNS1_CMD);                                // 983    DNS服务器1
	strValue = m_strG166_Dns1;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 

	strCommand.Format("%d", INT_G166_DNS2_CMD);                              // 984    DNS服务器2
	strValue = m_strG166_Dns2;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
#endif
	strCommand.Format("%d", INT_SCROLL_HOTLINE_CMD); 				    	// 1001    //客服热线号码
	strValue = m_strScroll_1001_HotLine;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	strCommand.Format("%d", INT_SCROLL_NXTNUM_CMD); 				    	// 1002    //农信通号码
	strValue = m_strScroll_1002_NxtNum;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
	strCommand.Format("%d", INT_SCROLL_MOBILESEC_CMD); 				    	// 1003    //移动秘书号码
	strValue = m_strScroll_1003_MobileSec;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
	for (i=0; i<COUNT_SCROLL_ISSUANCENUM; i++){
		strCommand.Format("%d", INT_SCROLL_ISSUANCENUM_CMD + i); 		// [1004, 1010] //滚动条发布号码
        
		strValue = m_strScroll_1004_IssuanceNum[i];
        
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
	}	
#endif	
	for (i=0; i< INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		// [2001, 2020]
		strCommand.Format("%d",  INT_DIRECT_NUMBER_TABLE_CMD + i);			//	2001    //直拨号码
		strValue = m_AryDIRECT_2020_strDirectTable[i];
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	}

#if defined(NETWORK_CONFIG)
#if defined(PAGE_CFG)
	if (ENABLE_EMERGENCY_CALL)
#endif
	{
		int j = 0;//第 j 个非空设置
		// 2600, for clear EmCall setting
		strCommand.Format("%d",  CLEAR_EMERGENCY_SETTING);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"\""); 
		// [2601, 2610] members for CPropPageEmCallCfg Table
		for (i=0; i< EMERGENCY_NUM_INT; i++)
		{
			strCommand.Format("%d",  INT_EMERGENCY_CALL_CMD + j);
			strValue = m_AryEmCall_2610_strNumberList[i];
			if (!strValue.IsEmpty())
			{
				m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
				j++;
			}
		}
	}
	
	// ===========================================================================================
	// 2700, for clear OpnCfg seetting
#if defined(PAGE_CFG)
	if (ENABLE_OPN_CFG)
#endif
	{
		int j = 0;//第 j 个非空设置
		strCommand.Format("%d",  CLEAR_OPN_CFG);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"\""); 
		// [2701, 2750] members for CPropPageOpnCfg Table
		for (i=0; i< OPN_TOTAL; i++)
		{
			strCommand.Format("%d",  INT_OPERATION_NAME_CMD + j);
			strValue = m_AryNetWorkName_2750_strNameList[i];
			if ((13 > strValue.GetLength())||// NetWorkName is empty
				(_T("FFF") == strValue.Left(3)))   // mcc is invalid
				continue;
            
			m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
			j++;
		}
	}
#endif
	
	// --------------------------------------------------------
	// FORBID TABLE
	// --------------------------------------------------------
	for (i=0; i< INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		// [3001, 3020]
		strCommand.Format("%d",  INT_FORBID_NUMBER_TABLE_CMD + i);			// 3001    //禁拨号码
		strValue = m_AryFORBID_3020_strForbidTable[i];
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	}
	
	strCommand.Format("%d",  INT_IF_ADD_ZERO_FOR_EXTR_PHONE_CMD);			// 4000    //外地手机是否加零
	strValue = m_strSYS_4000_bNotLocalAddZero;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	unsigned int nArrayLength = m_ArySECT_4400_strLocalTable.GetSize();
	unsigned int nStrNum = (INT_INIT_LOCAL_PHONE_ITEMNUM> nArrayLength)? nArrayLength :INT_INIT_LOCAL_PHONE_ITEMNUM;
	for (i=0; i< nStrNum; i++)
	{
		// [4001, 4400]
		// 按照道理来说，如果教本正确的话，此处数据应该是顺序存放的
		// 故我们这里不在校验数据的合法性
		// 我们认定：数据是按照命令号依次顺序存放的
		strCommand.Format("%d",  INT_LOCAL_SEGMENT_TABLE_CMD + i);			// 4001    //本地手机号段
		strValue = m_ArySECT_4400_strLocalTable[i];
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	}
	strCommand.Format("%d",  INT_LOCAL_SEGMENT_TABLE_CMD + i);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"\""); 
	
	for (i=0; i< INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		// [4401, 4420]
		strCommand.Format("%d",  INT_LOCAL_USER_SEGMENT_TABLE_CMD + i);		// 4401    //本地手机号段
		strValue = m_ArySECT_4420_strUserLocalTable[i];
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	}
	
	strCommand.Format("%d",  INT_REFUSE_TYPE_CMD);							// 5000	//是否限制呼入所有来电
	strValue = m_strREFUSE_5000_bRefuseType;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	for (i=0; i< INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		// [5001, 5020]
		strCommand.Format("%d",  INT_REFUSE_NUMBER_TABLE_CMD + i);			// 5001    //读限制呼入号段
		strValue = m_AryREFUSE_5020_strRefuseTable[i];
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	}
	
	strCommand.Format("%d",  INT_IF_AUTO_LOCK_PIN_CMD);						// 6000    //读是否自动锁PIN码
	strValue = m_strPINSIM_6000_bAutoLockPin;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_SET_PIN_LENGTH_CMD);						// 6001    //读设置PIN码长度
	strValue = m_strPINSIM_6001_nPinLength;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_SET_INIT_PIN_CODE_CMD);					// 6002    //读设置初始PIN码
	strValue = m_strPINSIM_6002_strOriginalPin;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	// ----------------------------------------------------------
	// 2003.12.23 依据冯永君的建议，如果6004固定PIN码为空
	// 不论固定PIN码窗口是否被enable了
	// 都不再往下发送这项内容！
	// ----------------------------------------------------------
	if (!m_strPINSIM_6004_strFixedPin.IsEmpty())
	{
		strCommand.Format("%d",  INT_SET_FIXED_PIN_CODE_CMD);					// 6004    //读设置固定PIN码
		strValue = m_strPINSIM_6004_strFixedPin;
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	}
	
	strCommand.Format("%d",  INT_IF_USE_RANDOM_PIN_CMD);					// 6005    //读是否使用随机PIN码
	strValue = m_strPINSIM_6005_bUseRandomPin;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_IF_BIND_SIM_CARD_CMD);						// 7000		//读话机是否绑定SIM卡
	strValue = m_strPINSIM_7000_bLockSim;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_BIND_SIM_CARD_NUMBER_CMD);					// 7001    //读话机绑定的SIM卡号
	strValue = m_strPINSIM_7001_strLockSimNum;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_IF_LOCK_COMPANY_CMD);						// 7002    //读是否锁定运营商
	strValue = m_strLOCK_7002_bLockProvider;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 

#if defined(LOCK_SERVICE_PROVIDER)
	strCommand.Format("%d", INT_LOCKED_COMPANY_CMD + SERVICE_PROVIDER_MAX);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"\"");
	if (atoi(m_strLOCK_7002_bLockProvider) > 0)
	{
		for (i=0; i<SERVICE_PROVIDER_MAX; i++)
		{
			strCommand.Format("%d", INT_LOCKED_COMPANY_CMD+i);
			strValue = m_AryLOCK_7003_nProvider[i];
			m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
		}
	}
#else
	strCommand.Format("%d",  INT_LOCKED_COMPANY_CMD);						// 7003    //读锁定的运营商
	strValue = m_strLOCK_7003_nProvider;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
#endif

	//锁定小区
	strCommand.Format("%d",  INT_IF_LOCK_DISTRICT_CMD);						// 8000    //读是否锁小区
	strValue = m_strLOCK_8000_bLockRegion;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_LOCKED_DISTRICT_NUMBER_CMD);				// 8001	//读锁定的小区数
	strValue = m_strLOCK_8001_nRegionNum;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_IF_AUTO_LOCK_DISTRICT_CMD);				// 8002    //读是否自动锁小区
	strValue = m_strLOCK_8002_bAutoLockRegion;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 


	strCommand.Format("%d",  INT_LOCKED_DISTRICT_CODE_CMD);					// 8003    //读已经锁定的小区号
	strValue = m_strLOCK_8003_strLockRegion;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_IF_LOCK_LAC_CMD);					// 8004    //read if lock the lac
	strValue = m_strLOCK_8004_bLockLac;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	strCommand.Format("%d",  INT_LOCKED_LAC_CODE_CMD);					// 8005    //read the first half locked Lac code
	strValue = m_strLOCK_8005_strLockLac;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
    strCommand.Format("%d",  INT_LOCKED_LAC_CODE_CMD2);					// 8006    //read the second half locked Lac code
	strValue = m_strLOCK_8006_strLockLac;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	
	
	
	for (i=0; i< INT_INIT_LISTBOX_ITEMNUM; i++)
	{
		// [9001, 9020]
		strCommand.Format("%d",  INT_IP_RULE_TABLE_CMD + i);		// 9001	//IP规则设置
		strValue = m_AryIP_9001_strIPRuleTable[i];
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\""); 
	}

#if !defined(NO_NXT)
	strCommand.Format("%d", INT_SCROLL_NXTMENUTITLE_CMD); 			  //  13000   //农信通菜单标题
	strValue = m_strScroll_13000_NxtTitle;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
	strCommand.Format("%d", INT_SCROLL_MATTER_CMD); 			     //  13001   //滚动条内容
	strValue = m_strScroll_13001_ScrollMatter;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
	strCommand.Format("%d", INT_SCROLL_MATTERAD_CMD); 			     //  13002   //滚动条字头
	strValue = m_strScroll_13002_ScrollAd;
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
#endif

#if defined(ENABLE_MOBILE_HEAD)
	strCommand.Format("%d", MOBILE_HEADER_CLEAR_CMD);
	strValue = "CLEAR";
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");

	nStrNum = 0;

	for (i=0; i< MAX_MOBILE_HEADER_TOTAL; i++)
	{
		strValue = m_strMobileHeader[i];
		if (!strValue.IsEmpty())
		{
			strCommand.Format("%d", INT_MOBILE_HEADER_STR_CMD + nStrNum);
			m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, "\"" + strValue + "\"");
			nStrNum++;
		}
	}
#endif
	
	CString strATValue;
	
	// AT+CMIC0=0,2
	strATValue = m_strMIC_CMIC0_nCMICMain;		//1	"CMIC0" // 设置主通道麦克风增益
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_CMIC0_CMD, "0," + strATValue); 
	
	// AT+CMIC1=1,2
	strATValue = m_strMIC_CMIC1_nCMICAux;		//2	"CMIC1" // 设置辅助通道麦克风增益
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_CMIC1_CMD, "1," + strATValue); 
	
#if defined(R16_ATC)
#if defined(REVERSE_SIDET)
	strATValue = m_strMIC_SIDET_AuxGain;			//3	"SIDET1" // 设置辅侧音增益
	strATValue = _T("1,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET1_CMD, strATValue); 

	strATValue = m_strMIC_SIDET_MainGain;			//3	"SIDET0" // 设置主侧音增益
	strATValue = _T("0,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET0_CMD, strATValue);
#else
	strATValue = m_strMIC_SIDET_MainGain;			//3	"SIDET0" // 设置主侧音增益
	strATValue = _T("0,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET0_CMD, strATValue); 
	
	strATValue = m_strMIC_SIDET_AuxGain;			//3	"SIDET1" // 设置辅侧音增益
	strATValue = _T("1,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET1_CMD, strATValue); 
#endif
#else
	// AT+SIDET=4096
	strATValue = m_strMIC_SIDET_Gain;			//3	"SIDET" // 设置侧音增益
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET_CMD, strATValue); 
#endif
	
	// AT+ECHO0=0,0,0,0							//4	"ECHO0" // 设置主通道回波抑制
	strATValue.Format("%s,%s,%s,0", m_strMIC_ECHO0_Vox, m_strMIC_ECHO0_Min, m_strMIC_ECHO0_Samp);
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_ECHO0_CMD, strATValue); 
	
	// AT+ECHO1=0,0,0,1							//5	"ECHO1" // 设置辅助通道回波抑制
	strATValue.Format("%s,%s,%s,1", m_strMIC_ECHO1_Vox, m_strMIC_ECHO1_Min, m_strMIC_ECHO1_Samp);
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_ECHO1_CMD, strATValue); 
	
	// AT+CMIC0=0,2
	strATValue = m_strSYS_LCD_Contrast;		//1	"CMIC0" // 设置主通道麦克风增益
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_CTRS_CMD, strATValue); 
	// 短信限制面页

#if defined(PAGE_CFG)
	SavePageOptionToIni(strSaveFile);
#endif

	if (bFromIni) 
	{
		CString strInfo; 
		strInfo.Format(_T("successful to export script[%s]..."), strSaveFile.Mid(strSaveFile.ReverseFind('\\')+1));
		ShowStatus(strInfo);
	}
	
	// 标记数据为不曾修改过
	m_bDataChanged = FALSE;
}

void CBizPhoneSettingDlg::ResetBizPhoneSetting(void)
{
	int nIndex = 0;
#if !defined(NO_NXT)
#if defined(CXT_EX)
	// ===========================================================================================
	// members for G166
	m_strG166_IpAddress.Empty();
	m_strG166_IpPort.Empty();
	m_strG166_Dns1.Empty();
	m_strG166_Dns2.Empty();
#endif
	// members for Scroll
	m_strScroll_1001_HotLine.Empty();
	m_strScroll_1002_NxtNum.Empty();
	m_strScroll_1003_MobileSec.Empty();

	for (nIndex=0; nIndex<7; nIndex++)
	{
		m_strScroll_1004_IssuanceNum[nIndex].Empty();
	}

	m_strScroll_13000_NxtTitle.Empty();
	m_strScroll_13001_ScrollMatter.Empty();
	m_strScroll_13002_ScrollAd.Empty();

	for (nIndex=0; nIndex<40; nIndex++)
		m_strLimit_1020_MsgAd[nIndex].Empty();
#endif

	// ======================================================================
	// members for SYS
	m_strSYS_0002_nAutoDely.Empty();			// "01" [0,9]
	m_strSYS_0004_sSMSCenter.Empty();			// "..." [1,16]
	m_strSYS_4000_bNotLocalAddZero.Empty();		// 00/01
	m_strSYS_0005_bForbidLongDistanceCall.Empty();	// 00/01

	// ======================================================================
	// members for LOCAL
	m_strLOCAL_1000_strLocalZone.Empty();			// "..." [0,6]
	m_strLOCAL_0003_nLocalMethod.Empty();			// 00/01/02

	// ======================================================================
	// members for PINSIM
	m_strPINSIM_6000_bAutoLockPin.Empty();			// 0/1
	m_strPINSIM_6001_nPinLength.Empty();			// "..." [4,8]
	m_strPINSIM_6002_strOriginalPin.Empty(); 		// "..." [4,8]
	m_strPINSIM_6004_strFixedPin.Empty();			// "..." [4,8]
	m_strPINSIM_6005_bUseRandomPin.Empty();		// 0/1
	m_strPINSIM_7000_bLockSim.Empty();				// 0/1
	m_strPINSIM_7001_strLockSimNum.Empty();			// "..." 定长20个字节

	// ======================================================================
	// members for Lock
	m_strLOCK_7002_bLockProvider.Empty();			// 0/1
#if defined (LOCK_SERVICE_PROVIDER)
	for (nIndex=0; nIndex<SERVICE_PROVIDER_MAX; nIndex++)
	{
		m_AryLOCK_7003_nProvider[nIndex].Empty();
	}
#else
	m_strLOCK_7003_nProvider.Empty();				// "..." [0,1]
#endif
	m_strLOCK_8000_bLockRegion.Empty();				// 0/1	
	m_strLOCK_8001_nRegionNum.Empty();				// "..." [0,20]
	m_strLOCK_8002_bAutoLockRegion.Empty();			// 0/1
	m_strLOCK_8003_strLockRegion.Empty(); 			// "..." [0,20]*4


	// ===========================================================================================
	// members for Locking Lac
	m_strLOCK_8004_bLockLac.Empty();			// 0/1
	m_strLOCK_8005_strLockLac.Empty();          //first part of LacInf[50]
	m_strLOCK_8006_strLockLac.Empty();          // second part of lacinf[50]

	for (nIndex=0; nIndex<INT_INIT_LISTBOX_ITEMNUM; nIndex++)
	{
		m_AryDIRECT_2020_strDirectTable[nIndex].Empty();
	}

	for (nIndex=0; nIndex<EMERGENCY_NUM_INT; nIndex++)
	{
		m_AryEmCall_2610_strNumberList[nIndex].Empty();
	}

	for (nIndex=0; nIndex<OPN_TOTAL; nIndex++)
	{
		m_AryNetWorkName_2750_strNameList[nIndex].Empty();
	}

	for (nIndex=0; nIndex<INT_INIT_LISTBOX_ITEMNUM; nIndex++)
	{
		m_AryFORBID_3020_strForbidTable[nIndex].Empty();
	}

	m_strREFUSE_5000_bRefuseType.Empty();
	for (nIndex=0; nIndex<INT_INIT_LISTBOX_ITEMNUM; nIndex++)
	{
		m_AryREFUSE_5020_strRefuseTable[nIndex].Empty();
	}

	m_ArySECT_4400_strLocalTable.RemoveAll();

	for (nIndex=0; nIndex<INT_INIT_LISTBOX_ITEMNUM; nIndex++)
	{
		m_ArySECT_4420_strUserLocalTable[nIndex].Empty();
	}

	for (nIndex=0; nIndex<INT_INIT_LISTBOX_ITEMNUM; nIndex++)
	{
		m_AryIP_9001_strIPRuleTable[nIndex].Empty();
	}

#ifdef _XIAMEN_START_BUILD
	// ===========================================================================================
	// members for XiaMen Start Setting
	m_strXIAMEN_0006_sMscReceiveNum.Empty();	// "..." [0,16]   /*接收网管中心*/
	m_strXIAMEN_0007_bShowOriginalNum.Empty();	// 00/01          /*是否只显示原始号码*/
	m_strXIAMEN_0009_sProducerID.Empty();	    // "..." [1－23]  /*生产商ID信息*/
	m_strXIAMEN_0011_bLocalSectEnabled.Empty(); // 00/01		  /*本地手机号段是否生效*/
#endif // _XIAMEN_START_BUILD

#ifdef _YINGNUO_SECURITY_BUILD
	// ===========================================================================================
	// members for Yingnuo security Setting
	m_strYINGNUO_0020_sSecurityNum0.Empty();	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第1组密钥*/
	m_strYINGNUO_0021_sSecurityNum1.Empty();	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第2组密钥*/
	m_strYINGNUO_0022_sSecurityNum2.Empty();	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第3组密钥*/
	m_strYINGNUO_0023_sSecurityNum3.Empty();	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第4组密钥*/
	m_strYINGNUO_0024_sSecurityNum4.Empty();	// 一组[0,255]的整数共16个，每个用00,ff表示   /*第5组密钥*/
#endif // _YINGNUO_SECURITY_BUILD

#ifdef _TONGZE_PARA_BUILD
	// ===========================================================================================
	// members for Tongze Start Setting
	m_strTONGZE_0009_sProducerID.Empty();	    // "..." [1－23]  /*生产商ID信息*/
	m_strTONGZE_0011_bLocalSectEnabled.Empty(); // 00/01		  /*本地手机号段是否生效*/
#endif // _TONGZE_PARA_BUILD


#ifdef  CARD_ENCRYPT
	m_strStartNet_0019_sEncryptSolution.Empty();
	m_strStartNet_0018_sIfCompatibleCard.Empty();
	m_strStartNet_0017_sVenderNO.Empty();
	m_strStartNet_0031_sKeyNO.Empty();
	m_strStartNet_0020_sKeyword1.Empty();
	m_strStartNet_0021_sKeyword2.Empty();
	m_strStartNet_0022_sKeyword3.Empty();
	m_strStartNet_0023_sKeyword4.Empty();
	m_strStartNet_0024_sKeyword5.Empty();
#endif	


#if defined(ENABLE_MOBILE_HEAD)
	for (nIndex=0; nIndex<MAX_MOBILE_HEADER_TOTAL; nIndex++)
		m_strMobileHeader[nIndex].Empty();
#endif
}

BOOL CBizPhoneSettingDlg::CompareIniSrcAndReadBack(CString szIniSrc, CString ReadBack)
{
	return CompareParaIni(szIniSrc, ReadBack);
}

long CBizPhoneSettingDlg::OnReceiveLowNotify(WPARAM wParam, LPARAM lParam)
{
	/////////////////////////////////////////////////////
	MainSerial.bActivated = TRUE;
	/////////////////////////////////////////////////////
	char         recbuf[MAXRECEIVECHARNUM];
	int          nLength;
	static char *datapointer;    
    static BOOL  ReceDataFlag = FALSE;
	//	POSITION p2;
	//	POSITION p1;
	
	/* 收到短消息中> 的表示符 */
	static BOOL  ReceSMFlag   = FALSE;
    
	// 判断接收缓冲区中有新数据到达，否则直接返回
	if(!MainSerial.m_bConnected || (wParam & EV_RXCHAR) != EV_RXCHAR)
	{
		SetEvent(MainSerial.m_hPostMsgEvent);  //允许发送下一个WM_COMMNOTIFY消息
		return 0L;
	}
	
	// 从串口接收缓冲区中将数据读出
	memset(recbuf, '\0', MAXRECEIVECHARNUM);
	nLength = MainSerial.ReadComm(recbuf, MAXRECEIVECHARNUM);
	
	
	if(nLength)
	{
		for(int i=0; i < nLength; i++)
		{
			if(ReceDataFlag)
			{
				if(recbuf[i] != CR)
				{
					if((MainSerial.m_pDataRxBufferInUseVar->length) 
						< MAXRECEIVECHARNUM)
					{
						*datapointer = recbuf[i];
						datapointer++;
						MainSerial.m_pDataRxBufferInUseVar->length++;
						
						if(ReceSMFlag == TRUE)
						{
							if(recbuf[i] == ' ')
							{
								if(MainSerial.m_bDataRxBufferFree 
									== TRUE)
								{
									MainSerial.m_bDataRxBufferFree 
										= FALSE;
									MainSerial.SwapDataRxBuffers();
									SendMessage(WM_RECEIVEHIGH_NOTIFY, 0, 0);
								}
								else
								{
									MessageBox(_T("Can't exchange buffers which is using as a receiver!"), SYS_NAME, MB_OK|MB_ICONSTOP);
									return 1;
								}
								ReceDataFlag = FALSE;
							}
							
							ReceSMFlag = FALSE;
						}
					}
					else
					{
						MessageBox(_T("buffers which is using as a receiver if overflow!"), SYS_NAME, MB_OK|MB_ICONSTOP);
						return 1;
					}
				}
				else
				{
					if(MainSerial.m_bDataRxBufferFree == TRUE)
					{
						MainSerial.m_bDataRxBufferFree = FALSE;
						MainSerial.SwapDataRxBuffers();
						SendMessage(WM_RECEIVEHIGH_NOTIFY, 0, 0);
					}
					else
					{
						MessageBox(_T("Can't exchange buffers which is using as a receiver!"), SYS_NAME, MB_OK|MB_ICONSTOP);
						return 1;
					}
					ReceDataFlag = FALSE;
				}
			}
			else
			{
				if((recbuf[i] != CR) && (recbuf[i] != LF))
				{
					ReceDataFlag = TRUE;
					datapointer  = MainSerial.m_pDataRxBufferInUseVar->data;
					MainSerial.m_pDataRxBufferInUseVar->length = 1;
					*datapointer = recbuf[i];
					datapointer++;
					
					if(recbuf[i] == '>'){
						ReceSMFlag = TRUE;
					}
				}
			}
		}
	}
	
	SetEvent(MainSerial.m_hPostMsgEvent);	
	
	return 0L;
}
long CBizPhoneSettingDlg::OnReceiveHighNotify(WPARAM wParam, LPARAM lParam)
{
	char *messagebuffer;
	BOOL r;
	if((MainSerial.m_bDataRxBufferFree == FALSE) &&
		(MainSerial.m_pDataRxBufferUnUseVar->length != 0))
	{
		messagebuffer = (char *)malloc(
			MainSerial.m_pDataRxBufferUnUseVar->length+1);
		for(int i = 0; i < MainSerial.m_pDataRxBufferUnUseVar->length; i++)
		{
			messagebuffer[i] = 
				MainSerial.m_pDataRxBufferUnUseVar->data[i];
		}
		
		messagebuffer[MainSerial.m_pDataRxBufferUnUseVar->length]
			= '\0';
		r = MainContent.m_pCommThread->PostThreadMessage(
			WM_COMMMANAGE_NOTIFY, 
			(unsigned int)messagebuffer, 
			0);
		
		MainSerial.m_bDataRxBufferFree = TRUE;		
	}
	else
	{
		MessageBox(_T("There is no data to deal with front layer!"), SYS_NAME, MB_OK|MB_ICONSTOP);
		return 1;
	}
	
	return 0;
}
// MainFrame收到底层发来的开始发送某条发送AT命令的消息指示
long CBizPhoneSettingDlg::OnStartSendAtcommandNotify(WPARAM wParam, 
													 LPARAM lParam)
{
	// 得到正在发送的命令类型
	CommandType commandtype = (CommandType)wParam;
	
	switch(commandtype) 
	{
	case READ_TOTAL_SETTING_STATE_CMD:	
		break;
		
	default:
		MessageBox(_T("OnStartSendAtcommandNotify Recv exceptional data!"), SYS_NAME, MB_OK|MB_ICONSTOP);
		break;
	}
	
	return 0;
}

// Huang Ye Feb. 21 2006
void CBizPhoneSettingDlg::OnActivateComm(void)
{
	ShowStatus("Send activated task..");
	/*	
	if (!DeleteThisFile((MainContent.m_sWorkPath)+CString(PARAFILE)))
	ShowStatus(_T("无法删除临时文件！操作终止！"), FALSE);
	*/	
#if defined(USE_WAIT_WND)
	if (!m_dlgWaitMsg.IsWindowVisible())
	{
		m_dlgWaitMsg.CenterWindow();
		m_dlgWaitMsg.ShowWindow(SW_SHOW);
	}

	m_dlgWaitMsg.m_ctrTask.SetWindowText(_T("Please wait...!"));
#endif
	
	//	Sleep(5000);	// for NiuLiQun temp use, by chensheng 20050422
	
	MainContent.m_pMainSendData = (SendData *)malloc(sizeof(SendData));
    MainContent.m_pMainSendData->senddata = malloc(sizeof(""));    
	MainContent.m_pMainSendData->commandtype = ACTIVATE_COMMPORT_STATE_CMD;
	MainContent.m_pFIFOThread->PostThreadMessage(WM_SENDNEWFIFO_NOTIFY,
		(unsigned int)MainContent.m_pMainSendData,	0);
	
	
}

// Huang Ye Feb. 21 2006



void CBizPhoneSettingDlg::OnReadSetting(void) 
{
	ShowStatus("Send loaded task..");

	//OnShowPageState(CIS_READING);
	
	if (!DeleteThisFile((MainContent.m_sWorkPath) + CString(PARAFILE)))
		ShowStatus(_T("Can't delete tempfile, stop task!"), FALSE);
	
#if defined(USE_WAIT_WND)
	if (!m_dlgWaitMsg.IsWindowVisible())
	{
		m_dlgWaitMsg.CenterWindow();
		m_dlgWaitMsg.ShowWindow(SW_SHOW);
	}
	m_dlgWaitMsg.m_ctrTask.SetWindowText(_T("please wait..!"));
#endif
	
	//Sleep(5000);	// for NiuLiQun temp use, by chensheng 20050422
#ifdef CARD_ENCRYPT
	Sleep(500);
#endif
	MainContent.m_pMainSendData = (SendData *)malloc(sizeof(SendData));
    MainContent.m_pMainSendData->senddata = malloc(sizeof(""));    
	MainContent.m_pMainSendData->commandtype = READ_TOTAL_SETTING_STATE_CMD;
	MainContent.m_pFIFOThread->PostThreadMessage(WM_SENDNEWFIFO_NOTIFY,
		(unsigned int)MainContent.m_pMainSendData,	0);
	
}
LRESULT CBizPhoneSettingDlg::OnReadParaStateNotify(WPARAM wParam,LPARAM lParam)
{
	UINT curState = wParam;
	UINT curCmd = lParam;
	CString szTips = "";

	switch(curState)
	{
		////////////////////////////////////////
	case ACTIVATE_COMMPORT_STATE:
		ShowStatus("Comm is activated..");
		Sleep(500);
#if defined(USE_WAIT_WND)
		if (m_dlgWaitMsg.IsWindowVisible())
		{
			m_dlgWaitMsg.ShowWindow(FALSE);
			Invalidate();
		}
#endif
		break;	

	case ACTIVATE_COMMPORT_BEING:			//读速延时时间
		ShowStatus("It is activating com..");
		break;

	case READ_MIC_GAIN_LEVEL:			 //Read Microphone Gain Level
		ShowStatus("loading mic level..");	
		break;
	case READ_CHANGE_SIDE_TONE:			 //Read Side Tone Gain Level
		ShowStatus("loading side tone..");	
		break;
	case READ_ECHO_CANCEL_CONTROL:         //Read Echo Cancellation Control
		ShowStatus("loading Echo cancel control..");	
		break;
	case READ_LCD_CONTRAST:                //Read Lcd contrast
		ShowStatus("loading lcd contrast..");	
		break;

	default:
		if (wParam < READ_TYPE_DIVIDING)
		{
			szTips.Format("Read[%d]...", curCmd);
			ShowStatus(szTips);
		}
		break;
	}

	return 0;
} 
LRESULT CBizPhoneSettingDlg::OnReadParaErrNotify(WPARAM wParam, LPARAM lParam)
{
	UINT curState = wParam;
	UINT curCmd = lParam;
	CString s   = "";
	switch(curState)
	{
	case READ_MIC_GAIN_LEVEL:			 //Read Microphone Gain Level
		ShowStatus("Err: Load Microphone Gain Level!", FALSE);	
		break;
	case READ_CHANGE_SIDE_TONE:			 //Read Side Tone Gain Level
		ShowStatus("Err: Load Side Tone Gain Level!", FALSE);	
		break;
	case READ_ECHO_CANCEL_CONTROL:         //Read Echo Cancellation Control
		ShowStatus("Err: Load Echo Cancellation Control!", FALSE);	
		break;
	case READ_LCD_CONTRAST:                //Read Lcd contrast
		ShowStatus("Err: Load lcd contrast!", FALSE);
		break;

	default:
		if (wParam < READ_TYPE_DIVIDING)
		{
			s.Format("Err:Para = %d", curCmd);
			ShowStatus(s, FALSE);
		}
		break;
	}
	//ShowParaFile();
	LoadFromModule();
	//m_btnReadSetting.EnableWindow();
#if defined(USE_WAIT_WND)
	if (m_dlgWaitMsg.IsWindowVisible())
	{
		m_dlgWaitMsg.ShowWindow(FALSE);
		Invalidate();
	}
#endif
	
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnReadParaOkNotify(WPARAM wParam, LPARAM lParam)
{
#if defined(PAGE_CFG)
	{// 读取完成后,将page option 保存下来
		CString strFileName = MainContent.m_sWorkPath + STRING_PARAFILE_NAME;
		SavePageOptionToIni(strFileName);
	}
#endif
	ShowStatus("Load all parameter successful!");	
	LoadFromModule();
#if defined(USE_WAIT_WND)
	if (m_dlgWaitMsg.IsWindowVisible())
	{
		m_dlgWaitMsg.ShowWindow(FALSE);
		Invalidate();
	}
#endif
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	
#if !defined(FOR_CLIENT)
	{
		// 比较参数文件
		CString szReadBack = MainContent.m_sWorkPath + PARAFILE;
		if (CompareIniSrcAndReadBack(theApp.m_szIniInput, szReadBack))
		{
			OnShowPageState(CIS_SAME);
		}
		else
		{
			OnShowPageState(CIS_DIF);
		}
	}
#endif
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnWriteParaStateNotify(WPARAM wParam,LPARAM lParam)
{
	UINT curState=wParam;
	UINT curCmd=lParam;
	CString s="";
	switch(curState)
	{
	case WRITE_TOTAL_SETTING_WAIT_STATE:
		s.Format("%d",curCmd);
		ShowStatus("It is writting ["+s+"] parameter..");
		break;
	case WRITE_ATCMD_SETTING_WAIT_STATE:
		s.Format("%d",curCmd);
		ShowStatus("It is writting ["+s+"] AT Cmd..");
		break;
	case WRITE_TOTAL_SETTING_SAVE_STATE:
		ShowStatus("It is saving setting [9999]..");
		break;
#ifdef  MENU_SAVE
	case WRITE_TOTAL_SETTING_SAVE_9994_STATE:
		ShowStatus("It is saving setting [9994]..");
		break;
#endif
	}
	//m_btnWriteSetting.EnableWindow();
	return 0;
} 
LRESULT CBizPhoneSettingDlg::OnWriteParaErrNotify(WPARAM wParam, LPARAM lParam)
{
	UINT curState = wParam;
	UINT curCmd = lParam;
	CString s="";
	switch(curState)
	{
	case WRITE_TOTAL_SETTING_BEGIN_WRITE_STATE:
		ShowStatus("Err: the input user's PassWord!", FALSE);
		//m_btnWriteSetting.EnableWindow();
		break;
	case WRITE_TOTAL_SETTING_WAIT_STATE:
		s.Format("%d",curCmd);
		ShowStatus("Err: writting ["+s+"] parameter!", FALSE);
		//m_btnWriteSetting.EnableWindow(); 
		break;
	case WRITE_ATCMD_SETTING_WAIT_STATE:
		s.Format("%d",curCmd);
		ShowStatus("Err: writting ["+s+"] AT Cmd!", FALSE);
		//m_btnWriteSetting.EnableWindow(); 
		break;
	case WRITE_TOTAL_SETTING_SAVE_STATE:
		ShowStatus("Err: saving setting[9999]!", FALSE);		
		break;
#ifdef  MENU_SAVE
	case WRITE_TOTAL_SETTING_SAVE_9994_STATE:
		ShowStatus("Err: saving setting[9994]!", FALSE);
		break;
#endif
	case WRITE_TOTAL_SETTING_END_WRITE_STATE:
		ShowStatus("Fail to save parameter!", FALSE);
		//m_btnWriteSetting.EnableWindow();
		break;
	case WRITE_TOTAL_ATCMD_END_WRITE_STATE:
		ShowStatus("Fail to save AT Cmd!", FALSE);	
		break;
	}
#if defined(USE_WAIT_WND)
	if (m_dlgWaitMsg.IsWindowVisible())
	{
		m_dlgWaitMsg.ShowWindow(FALSE);
		Invalidate();
	}
#endif
	
	// 当设定了更改密码并且下载的标志的时候
	if (m_bChangePwdAndSaveSettingAndWriteDefault)
	{
		// 将标志位复位
		m_bChangePwdAndSaveSettingAndWriteDefault = FALSE;
	}
	
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnWriteParaOkNotify(WPARAM wParam, LPARAM lParam)
{
	UINT curState=wParam;
	switch(curState)
	{
	case WRITE_TOTAL_SETTING_SAVE_STATE:
		ShowStatus("Successful to save [9999]!");
		break;
#ifdef  MENU_SAVE
	case WRITE_TOTAL_SETTING_SAVE_9994_STATE:
		ShowStatus("Successful to save [9994]!");
		break;
#endif
	case WRITE_TOTAL_SETTING_END_WRITE_STATE:
		ShowStatus("Successful to save all parameter!");
		//m_btnWriteSetting.EnableWindow();
		break;
	case WRITE_TOTAL_ATCMD_END_WRITE_STATE:
		ShowStatus("Successful to save all AT Cmd!");
		break;
	}
	
	// 只有当全部操作结束之后再判断是否开始下一个操作
	if (WRITE_TOTAL_ATCMD_END_WRITE_STATE == curState)
	{
		// 复位数据变更标志位
		m_bDataChanged = FALSE;
		
#if defined(USE_WAIT_WND)
		// 关闭显示消息的对话框
		if (m_dlgWaitMsg.IsWindowVisible())
		{
			m_dlgWaitMsg.ShowWindow(FALSE);
			Invalidate();
		}
#endif
		
		// 当设定了下载并且存为出厂设置的标志的时候
		if (m_bChangePwdAndSaveSettingAndWriteDefault)
		{
			// 接下去继续执行原定的操作
			OnSaveModuleDefault(0,0);
			
			// 将标志位复位
			m_bChangePwdAndSaveSettingAndWriteDefault = FALSE;
		}
		else
		{
			// Enable所有的操作按钮
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
		}
	}
	return 0;
}
void CBizPhoneSettingDlg::ShowStatus(CString str, BOOL bNoError)
{
	CWnd* pWnd = GetDlgItem(IDC_CURRENT_STATUS);
	
	// 输出到output page'd log listctrl
	//	if (m_pPageOutput)
	//		m_pPageOutput->OutputMessage(str, bNoError);
	
	CString* pStr = new CString;
	*pStr = str;
	
	// 输出到output page'd log listctrl
	// 首先获得OutPut page的指针
	CStackPageHelper* pHelper = GetPage(IDD_PROPPAGE_X_OUTPUT);
	CWnd* pOutputPage;
	
	if (pHelper)
		pOutputPage = pHelper->GetPage();
	else
		return;
	
	if (pOutputPage)
		pOutputPage->PostMessage(WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY, (WPARAM)bNoError, (LPARAM)pStr);
	else
		return;
	
	// 输出到主对话框的STATIC
	pWnd->SetWindowText(str);
#if defined(USE_WAIT_WND)
	// 输出到等待对话框 或者弹出单独的对话框
	if 	(m_dlgWaitMsg.IsWindowEnabled())
		m_dlgWaitMsg.m_ctrTask.SetWindowText(str);
#endif
}

LRESULT CBizPhoneSettingDlg::OnWriteToDefaultSettingErrNotify(WPARAM wParam,LPARAM lParam)
{
	UINT curState = wParam;
	switch(curState)
	{
	case WRITE_TO_DEFAULT_SETTING_BEGIN_WRITE_STATE:
		ShowStatus("Err: the input user's PassWord!", FALSE);
		//m_btnWriteDefaultSetting.EnableWindow();
		break;
	case WRITE_TO_DEFAULT_SETTING_END_STATE:
		ShowStatus("Err: save default end!", FALSE);
		break;
	case WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE:
		ShowStatus("Err: stop default writing!", FALSE);
		//m_btnWriteDefaultSetting.EnableWindow();
		break;
	case WRITE_TO_FACTORY_END_STATE:
		ShowStatus("Err: save factory end!", FALSE);
		break;
	}
	
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnWriteToDefaultSettingOkNotify(WPARAM wParam,LPARAM lParam)
{
	UINT curState = wParam;
	switch(curState)
	{
	case WRITE_TO_DEFAULT_SETTING_END_STATE:
		ShowStatus("Successful to save default setting!");
		break;
	case WRITE_TO_DEFAULT_SETTING_END_WRITE_STATE:
		ShowStatus("Successful save all!");
		//m_btnWriteDefaultSetting.EnableWindow();
		break;
	case WRITE_TO_FACTORY_END_STATE:
		ShowStatus("Successful save factory end!");
		break;
	}
	
	// 只有当全部操作结束之后再判断是否开始下一个操作
	if (WRITE_TO_FACTORY_END_STATE == curState)
	{
		// Enable所有的操作按钮
		(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
			SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);		
	}
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnRecoverDefaultSettingErrNotify(WPARAM wParam,LPARAM lParam)
{
	UINT curState = wParam;
	switch(curState)
	{
	case RECOVER_DEFAULT_SETTING_BEGIN_WRITE_STATE:
		ShowStatus("Err: the input user's PassWord!", FALSE);
		//m_btnRecoverDefaultSetting.EnableWindow(); 
		break;
	case RECOVER_DEFAULT_SETTING_END_STATE:
		ShowStatus("Err: recover default seetting!", FALSE);
		break;
	case RECOVER_DEFAULT_SETTING_END_WRITE_STATE:
		ShowStatus("Err: recover default seetting end!", FALSE);
		//m_btnRecoverDefaultSetting.EnableWindow();
		break;
	}
	
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnChangePassErrNotify(WPARAM wParam, LPARAM lParam)
{
	UINT curState = wParam;
	switch(curState)
	{
	case CHANGE_PASS_BEGIN_WRITE_STATE:
		ShowStatus("Err: the input user's PassWord!", FALSE);
		//	m_btnChangePass.EnableWindow();
		break;
	case CHANGE_PASS_END_STATE:
		ShowStatus("Err: chang PassWord!", FALSE);
		break;
	case CHANGE_PASS_END_WRITE_STATE:
		ShowStatus("Err: end writing!", FALSE);
		//m_btnChangePass.EnableWindow();
		break;
	default:
		ShowStatus("Err: chang PassWord!", FALSE);
		break;
	}	
	
	// 当设定了更改密码并且下载的标志的时候
	if (m_bChangePwdAndSaveSetting)
	{
		// 将标志位复位
		m_bChangePwdAndSaveSetting = FALSE;
	}
	
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	return 0;
} 
LRESULT CBizPhoneSettingDlg::OnChangePassOkNotify(WPARAM wParam,LPARAM lParam)
{
	UINT curState=wParam;

	switch(curState)
	{
	case CHANGE_PASS_END_STATE:
		ShowStatus("Successful PassWord is reseted!");
		MainContent.m_sOldPass = MainContent.m_sNewPass1 ;
		break;
	case CHANGE_PASS_END_WRITE_STATE:
		ShowStatus("Successful to save new PassWord!");	
		//m_btnChangePass.EnableWindow(); 
		break;
	default:
		break;
	}		
	
	// 只有当全部操作结束之后再判断是否开始下一个操作
	if (CHANGE_PASS_END_WRITE_STATE == curState)
	{
		// 当设定了更改密码并且下载的标志的时候
		if (m_bChangePwdAndSaveSetting)
		{
			// 接下去继续执行原定的操作
			OnSaveModuleSetting(0,0);
			
			// 将标志位复位
			m_bChangePwdAndSaveSetting = FALSE;
		}
		else{
			// Enable所有的操作按钮
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
		}
		
	}
	return 0;
} 
LRESULT CBizPhoneSettingDlg::OnRecoverDefaultSettingOkNotify(WPARAM wParam,LPARAM lParam)
{
	UINT curState=wParam;
	switch(curState)
	{
	case RECOVER_DEFAULT_SETTING_END_STATE:
		ShowStatus("Successful to recover default setting!");
		break;
	case RECOVER_DEFAULT_SETTING_END_WRITE_STATE:
		ShowStatus("Successful to stop recover default writing!");
		//m_btnRecoverDefaultSetting.EnableWindow();
		break;
	}
	
	// 只有当全部操作结束之后再判断是否开始下一个操作
	if (RECOVER_DEFAULT_SETTING_END_WRITE_STATE == curState)
	{
		// Enable所有的操作按钮
		(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
			SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
		PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	}
	return 0;
} 

void CBizPhoneSettingDlg::OnTimer(UINT nIDEvent) 
{
	
	if(nIDEvent == DETECTTIMER) 
	{
		ShowStatus("system id running...");
		KillTimer(DETECTTIMER);
		
	}
	// 2007.06.08 add by hqr to Open/Close Comm
	else if(nIDEvent == OPENCOMMTIMER)
	{// 打开串口
		if(MainSerial.OpenConnection() == FALSE)
		{
			ShowStatus("Fail to open comm!", FALSE);
			
			KillTimer(OPENCOMMTIMER);
			
			// Enable所有的操作按钮
			MessageBox(_T("Fail to open comm!"), SYS_NAME, MB_OK|MB_ICONERROR);
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			//会造成死循环？
			//SetTimer(DETECTTIMER, DETECTTIME, NULL);				
		}
		else
		{
			ShowStatus("comm is contacted!");
			
			if(MainContent.m_pFIFOThread != NULL)
			{				
				MainContent.m_pFIFOThread->PostThreadMessage(WM_CLEAR_SEND_DATA_NOTIFY,0,0);
				Sleep(1000);				
			}			
			if(!(MainContent.bIfHaveInit)){
				MainContent.bIfHaveInit = TRUE;
			}
			
			KillTimer(OPENCOMMTIMER);
			
			extern short AfterOpenComm_afterActivateCommandTye;
			
			if (AfterOpenComm_ReadPara == AfterOpenComm_afterActivateCommandTye){
				// 要求激活串口
				PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
				MainSerial.afterActivateCommandTye = MainSerial.READDATA_CMD_DATAPAGE;
				
			}else if (AfterOpenComm_WritePara == AfterOpenComm_afterActivateCommandTye){
				
				PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
				MainSerial.afterActivateCommandTye = MainSerial.WRITEDATA_CMD_DATAPAGE;
				
			}else if (AfterOpenComm_afterActivateCommandTye == AfterOpenComm_ChangpassW){
				
				PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
				MainSerial.afterActivateCommandTye = MainSerial.CHANDEPASSWORD_CMD_DATAPAGE;
				
			}else if (AfterOpenComm_afterActivateCommandTye == AfterOpenComm_ChangPwLoad){
				
				PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
				MainSerial.afterActivateCommandTye = MainSerial.CHANGEANDDOWNLOAD_CMD_DATAPAGE;
				
			}else if (AfterOpenComm_afterActivateCommandTye == AfterOpenComm_ProduceLoad){
				
				PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
				MainSerial.afterActivateCommandTye = MainSerial.PRODUCTDOWNLOAD_CMD_DATAPAGE;
				
			}else if (AfterOpenComm_afterActivateCommandTye == AfterOpenComm_SaveDefault){
				
				PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
				MainSerial.afterActivateCommandTye = MainSerial.SETDEFAULTSETTING_CMD_DATAPAGE;
				
			}else if (AfterOpenComm_afterActivateCommandTye == AfterOpenComm_RecoverDefault){
				
				PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
				MainSerial.afterActivateCommandTye = MainSerial.RECOVERDEFAULTSETTING_CMD_DATAPAGE;
				
			}else{
				
				MessageBox(_T("UnKown Operation Type!"),SYS_NAME, MB_OK|MB_ICONSTOP);
				SetTimer(CLOSECOMMTIMER, COMMOPERATETIME, NULL);
				
			}
			
			
		} 
		/////////////////////////////////////////////
	}
	else if(nIDEvent == CLOSECOMMTIMER)
	{// 关闭串口
		ShowStatus("Comm is closed!");
		MainSerial.CloseConnection();
		KillTimer(CLOSECOMMTIMER);
		//return;
	}
	// 2007.06.08 add by hqr to Open/Close Comm
	else if(nIDEvent == SENDINGOVERTIMER)
	{
		// Predefine var in the section
		// 从界面的数据写到特定的文件去，然后通知线程发送到模块去
		switch( MainSerial.afterActivateCommandTye) {
		case MainSerial.READDATA_CMD_DATAPAGE:
			OnReadSetting();
			break;
			
		case MainSerial.WRITEDATA_CMD_DATAPAGE:            
			
			if(!MainContent.m_bDisplayOnceTime)
			{
				PostMessage(WM_MSG_SAVE_MODULE_SETTING_NOTIFY);
				
				KillTimer(SENDINGOVERTIMER);
			}
			break;
			
		case MainSerial.CHANDEPASSWORD_CMD_DATAPAGE:
			PostMessage(WM_MSG_CHANGE_PWD_NOTIFY);
			KillTimer(SENDINGOVERTIMER);
			break;
			
		case MainSerial.CHANGEANDDOWNLOAD_CMD_DATAPAGE:
			PostMessage(WM_MSG_CHANGE_PWD_NOTIFY);
			KillTimer(SENDINGOVERTIMER);
			break;
			
		case MainSerial.PRODUCTDOWNLOAD_CMD_DATAPAGE:
			PostMessage(WM_MSG_CHANGE_PWD_NOTIFY);
			KillTimer(SENDINGOVERTIMER);
			break;
			
		case MainSerial.SETDEFAULTSETTING_CMD_DATAPAGE:
			PostMessage(WM_MSG_SAVE_MODULE_DEFAULT_NOTIFY);
			KillTimer(SENDINGOVERTIMER);
			break;
			
		case MainSerial.RECOVERDEFAULTSETTING_CMD_DATAPAGE:
			PostMessage(WM_MSG_RECOVER_MODULE_DEFAULT_NOTIFY);
			KillTimer(SENDINGOVERTIMER);
			break;
			
		default:
			break;
		}
		
		
		/////////////////////////////////////////////
	}
	else if(nIDEvent == MONITORTIMER) 
	{
		KillTimer(MONITORTIMER);
		
		ASSERT(MainContent.m_pCommThread);
		MainContent.m_pCommThread->PostThreadMessage(
			WM_CLEARSTATE_NOTIFY, 
			0,
			0);
		// delete the sending data
		SetEvent(MainContent.m_hPostSendingEvent);
		MainContent.m_pFIFOThread->PostThreadMessage(
			WM_SENDFINISH_NOTIFY,
			FIFOLOST,
			0);
		Sleep(20);
		// 监控整条AT命令发送交互的时间
		ShowStatus("Task timeout!", FALSE);
		
#if defined(USE_WAIT_WND)
		if (m_dlgWaitMsg.IsWindowVisible())
		{
			m_dlgWaitMsg.ShowWindow(FALSE);
			Invalidate();
		}
#endif
		
		// Enable所有的操作按钮
		(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
			SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
		
		
		//监视超时则认为是找不到终端，重新启动查找定时器
		
		PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
		
		//MainSerial.CloseConnection();
		//SetTimer(DETECTTIMER, DETECTTIME, NULL);
	}	// TODO: Add your message handler code here and/or call default
	
	CStackDialog::OnTimer(nIDEvent);
}

////////////////////////////////////////////////////////////
long CBizPhoneSettingDlg::OnStartSendingTimerNotify(WPARAM wParam,LPARAM lParam)
{
	SetTimer(SENDINGOVERTIMER, SENDINGTIME, NULL);	
	return 0;
}
long CBizPhoneSettingDlg::OnStopSendingTimerNotify(WPARAM wParam,LPARAM lParam)
{
	KillTimer(SENDINGOVERTIMER);	
	return 0;
}
/////////////////////////////////////////////////////////////


// 2007.06.07 add by hqr to Open/Close Comm
long CBizPhoneSettingDlg::OnOpenCommConnectionNotify(WPARAM wParam,LPARAM lParam)
{
	SetTimer(OPENCOMMTIMER, COMMOPERATETIME, NULL);	
	return 0;
}
long CBizPhoneSettingDlg::OnCloseCommConnectionNotify(WPARAM wParam,LPARAM lParam)
{
	SetTimer(CLOSECOMMTIMER, COMMOPERATETIME, NULL);	
	return 0;
}
// 2007.06.07 add by hqr to Open/Close Comm


// 2008.06.01 add by hqr to lock/unlock page
LRESULT CBizPhoneSettingDlg::OnLockPoroPageNotify(WPARAM wParam,LPARAM lParam)
{
	BOOL bEnable = TRUE & wParam;
	EnablePoroPage(bEnable);

	if (bEnable)
	{
		RefreshPage();
	}

	return S_OK;
}
// 2008.06.01 add by hqr to lock/unlock page


LRESULT CBizPhoneSettingDlg::OnTreeControlNotify(WPARAM wParam,LPARAM lParam)
{

	BOOL bEnable = TRUE & wParam ;

#if !defined(FOR_CLIENT)
	bEnable &= (!theApp.m_bIsAdmin && (UM_SAVE == theApp.m_bUseMode));
#endif

	m_ctrTree.EnableWindow(bEnable);
	return S_OK;
}

LRESULT CBizPhoneSettingDlg::OnStartMonitorTimerNotify(WPARAM wParam,LPARAM lParam)
{
	MainContent.m_bDisplayOnceTime  = FALSE;
	
	SetTimer(MONITORTIMER, MainContent.m_nMonitorDelay, NULL);	
	return S_OK;
}

LRESULT CBizPhoneSettingDlg::OnStopMonitorTimerNotify(WPARAM wParam,LPARAM lParam)
{
	KillTimer(MONITORTIMER);		
	return S_OK;
}

void CBizPhoneSettingDlg::OnWriteSetting(void) 
{
	
	MainContent.m_bDisplayOnceTime = TRUE;
	if (MainContent.m_sOldPass.IsEmpty())
	{
		
		CInputPassDlg2 dlg;
		if(dlg.DoModal()==IDOK){
			MainContent.m_bDisplayOnceTime  = FALSE;
			MainContent.m_sOldPass =dlg.m_sOldPass ;
			
		}else{
			
			dlg.DestroyWindow();
			
			MainContent.m_bDisplayOnceTime  = FALSE;
			
			OnStopSendingTimerNotify(0, 0);
			
#if defined(USE_WAIT_WND)
			if (m_dlgWaitMsg.IsWindowVisible())
			{
				m_dlgWaitMsg.ShowWindow(FALSE);
				Invalidate();
			}
#endif
			
			
			//			((CCommThread*)MainContent.m_pCommThread)->CommThreadForceExit();
			
			// Enable所有的操作按钮
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			return;
		}
		
		/////////////////////////////////////////
		//m_bDisplayOnceTime = FALSE;
		/////////////////////////////////////////
	}
	CIniFile m_ParaFile((MainContent.m_sWorkPath)+CString(PARAFILE));
	CStringArray m_paraInfo;
	m_paraInfo.RemoveAll(); 
	m_ParaFile.ReadSection(ATCMD_SECTION,m_paraInfo);
	if(m_paraInfo.GetSize() > 0)
	{
		ShowStatus("Send the writing Task...");
		//ShowStatus2("");
		//m_btnWriteSetting.EnableWindow(false);
		//发送写参数任务
		MainContent.m_pMainSendData              = (SendData *)malloc(sizeof(SendData));
		MainContent.m_pMainSendData->senddata    = malloc(sizeof(""));    
		MainContent.m_pMainSendData->commandtype = WRITE_TOTAL_SETTING_STATE_CMD;
		MainContent.m_pFIFOThread->PostThreadMessage(WM_SENDNEWFIFO_NOTIFY,	
			                                         (unsigned int)MainContent.m_pMainSendData, 
			                                         0);
		
#if defined(USE_WAIT_WND)
		// show the status wait msg dialog
		if (!m_dlgWaitMsg.IsWindowVisible())
		{
			m_dlgWaitMsg.CenterWindow();
			m_dlgWaitMsg.ShowWindow(SW_SHOW);
		}
#endif
	}
	else
	{
		ShowStatus("there is nothing to save!", FALSE);
		
	}
	
	
}

void CBizPhoneSettingDlg::OnWriteDefaultSetting(void) 
{
	if (MainContent.m_sOldPass.IsEmpty())
	{
		CInputPassDlg2 dlg;
		if(dlg.DoModal()==IDOK){
			MainContent.m_sOldPass =dlg.m_sOldPass ;
			
		}else{
			dlg.DestroyWindow();
			// Enable所有的操作按钮
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			return;
		}
	}
	
	ShowStatus("send save default task...");
	
	//发送保存为出厂设置任务...
	MainContent.m_pMainSendData = (SendData *)malloc(sizeof(SendData));
	MainContent.m_pMainSendData->senddata = malloc(sizeof(""));    
	MainContent.m_pMainSendData->commandtype = SAVE_TO_DEFAULT_SETTING_STATE_CMD;
	MainContent.m_pFIFOThread->PostThreadMessage(WM_SENDNEWFIFO_NOTIFY,
		(unsigned int)MainContent.m_pMainSendData,	0);	
}

void CBizPhoneSettingDlg::OnChangePass(void)
{
	// TODO: Add your control notification handler code here
	//ShowParaFile();
	
	ShowStatus("send chang password task...");
	
	//发送修改密码任务...
	MainContent.m_pMainSendData = (SendData *)malloc(sizeof(SendData));
	MainContent.m_pMainSendData->senddata = malloc(sizeof(""));    
	MainContent.m_pMainSendData->commandtype = CHANGE_PASS_STATE_CMD;
	MainContent.m_pFIFOThread->PostThreadMessage(WM_SENDNEWFIFO_NOTIFY,
		(unsigned int)MainContent.m_pMainSendData,	0);	
}

void CBizPhoneSettingDlg::OnRecoverDefaultSetting(void) 
{
	if (MainContent.m_sOldPass.IsEmpty())
	{
		CInputPassDlg2 dlg;
		if(dlg.DoModal()==IDOK){
			MainContent.m_sOldPass =dlg.m_sOldPass ;
			
		}else{
			dlg.DestroyWindow();
			// Enable所有的操作按钮
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			return;
		}
	}
	ShowStatus("send recover default task...");
	//ShowStatus2("");
	//m_btnRecoverDefaultSetting.EnableWindow(false);
	//发送恢复默认设置任务...
	MainContent.m_pMainSendData = (SendData *)malloc(sizeof(SendData));
	MainContent.m_pMainSendData->senddata = malloc(sizeof(""));    
	MainContent.m_pMainSendData->commandtype = RECOVER_DEFAULT_SETTING_STATE_CMD;
	MainContent.m_pFIFOThread->PostThreadMessage(WM_SENDNEWFIFO_NOTIFY,
		(unsigned int)MainContent.m_pMainSendData, 0);		
}

int CBizPhoneSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStackDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnLoadFromUserIniFile(WPARAM wParam, LPARAM lParam)
{
	// 接受到消息，要求根据全局变量重新刷新显示
	ASSERT(!m_strExchangeFileName.IsEmpty());
	
	LoadIniFile(m_strExchangeFileName, TRUE);
	
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	return 0;
}

BOOL CBizPhoneSettingDlg::RefreshPage(void)
{
	// 将保存在全局变量中的数据体现到各个page中
	CStackPage* pStackPage = NULL;
	CStackPageHelperList::iterator it;
	
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) 
	{
		pStackPage = it->GetPage();
		pStackPage->OnRefreshPage();
	}

	if (!theApp.m_bEnablePage)
	{
		EnablePoroPage(theApp.m_bEnablePage);
	}
	
	// 然后，开文件，并将数据写入脚本文件
	return TRUE;
}

void CBizPhoneSettingDlg::OnShowPageState(CompareIniState eState)
{
	CStackPage* pStackPage = NULL;
	CStackPageHelperList::iterator it;
	
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) 
	{
		pStackPage = it->GetPage();
		pStackPage->OnShowPage(eState);
	}
	return;
}

void CBizPhoneSettingDlg::EnablePoroPage(BOOL bEnable)
{
	CStackPage* pStackPage = NULL;
	CStackPageHelperList::iterator it;
	
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) 
	{
		pStackPage = it->GetPage();
		pStackPage->OnLockPage(bEnable);
	}

	if (!theApp.m_bIsAdmin && (UM_SAVE == theApp.m_bUseMode))
	{
		m_ctrTree.EnableWindow(bEnable);
	}
}

LRESULT CBizPhoneSettingDlg::OnSaveToUserIniFile(WPARAM wParam, LPARAM lParam)
{
	// 接受到消息，要求根据全局变量重新刷新显示
	ASSERT(!m_strExchangeFileName.IsEmpty());
	
	if (CloseTest())
	{
		if (SaveData())
		{
			if (!DeleteThisFile(m_strExchangeFileName))
				ShowStatus(_T("this script has been in existence and can't be deleted, system stop!"), FALSE);
			SaveIniFile(m_strExchangeFileName, TRUE);
			m_bDataChanged = FALSE;
		}
	}
	
	// Enable所有的操作按钮
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	return 0;
}
// Huang ye Feb. 21 2006  for activate COMM
LRESULT CBizPhoneSettingDlg::OnActivateCommPort(WPARAM wParam, LPARAM lParam)
{
	// 读出模块中的数据
	OnActivateComm();
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnLoadModuleSetting(WPARAM wParam, LPARAM lParam)
{
	// 读出模块中的数据
	OnReadSetting();
	return 0;
}

void CBizPhoneSettingDlg::LoadFromModule(void)
{
	// 从模块写的特定文件中读出数据，并刷新界面
	CString strFileName = MainContent.m_sWorkPath + STRING_PARAFILE_NAME;
	
	LoadIniFile(strFileName, FALSE);
}

LRESULT CBizPhoneSettingDlg::OnSaveModuleSetting(WPARAM wParam, LPARAM lParam)
{
	// 从界面的数据写到特定的文件去，然后通知线程发送到模块去
	CString strFileName = MainContent.m_sWorkPath + STRING_PARAFILE_NAME;

	
	BOOL bAction = FALSE;
	if (CloseTest())
	{
		if (SaveData())
		{
			if (!DeleteThisFile(strFileName))
				ShowStatus(_T("can't delete the temporarily file, system stop!"), FALSE);
			SaveIniFile(strFileName, FALSE);
			OnWriteSetting();
			bAction = TRUE;
		}
	}
	
	if (!bAction)
	{
		// Enable所有的操作按钮
		(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
			SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	}
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnSaveModuleDefault(WPARAM wParam, LPARAM lParam)
{
	OnWriteDefaultSetting();
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnRecoverModuleDefault(WPARAM wParam, LPARAM lParam)
{
	OnRecoverDefaultSetting();
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnChangePassword(WPARAM wParam, LPARAM lParam)
{
	OnChangePass();
	return 0;
}

BOOL CBizPhoneSettingDlg::DeleteThisFile(CString strFile)
{
	if (DeleteFile(strFile))
	{
		// delete success
		return TRUE;
	}
	else
	{
		// delete failed
		int nErrorCode = GetLastError();
		
		if (ERROR_FILE_NOT_FOUND == nErrorCode)
		{
			// file does not exist
			return TRUE;
		}
		else if (ERROR_ACCESS_DENIED == nErrorCode)
		{
			// file is read only
			
			// set it to read-writeable, if failes return FALSE
			if (!SetFileAttributes(strFile, FILE_ATTRIBUTE_NORMAL)) return FALSE;
			
			// now delete it again
			if (!DeleteFile(strFile))
				return FALSE;
			else
				return TRUE;
		}
		else
			return FALSE;
	}
}

#ifdef USE_SERIAL_CODE
void CBizPhoneSettingDlg::GetHardwareID(DWORD& dwHDSerial)
{
	DWORD dwIDESerial;
	GetVolumeInformation("C:\\",NULL,NULL,&dwIDESerial,NULL,NULL,NULL,NULL); 
	dwHDSerial = dwIDESerial;
}

void CBizPhoneSettingDlg::GetHardwareID(CString& strHDSerial)
{
	CString strHardDiskSerial(_T(""));
	
	DWORD dwIDESerial;
	GetVolumeInformation("C:\\",NULL,NULL,&dwIDESerial,NULL,NULL,NULL,NULL); 
	
	strHardDiskSerial.Format(_T("%X"), dwIDESerial);
	strHDSerial = strHardDiskSerial;
}

void CBizPhoneSettingDlg::QuitSoftware(UINT nExitCode1, DWORD dExitCode2)
{
	CString strError;
	if (nExitCode1 == 0)
	{
		strError = _T("您将退出软件的运行！");
	}
	else if (nExitCode1 == 1)
	{
		strError.Format(_T("您已达到最大运行次数 - %d次，本软件将退出！"), dExitCode2);
	}
	else
		EndDialog(999);
	
	AfxMessageBox(strError, MB_ICONSTOP);
	EndDialog(nExitCode1);
}

void CBizPhoneSettingDlg::RunSoftware(void)
{
	//	AfxMessageBox(_T("Run Now"));
}

BOOL CBizPhoneSettingDlg::LoadMaxRunNumFromRegisterInfo(DWORD& dwMaxRun, CString strSerial)
{
	CString strTimes;
	strTimes = strSerial.Right(4);
	if (!ProcessAccordingGivenString(strTimes, STRING_SERIAL_CODE_FILTER_TIMES, FALSE))
		return FALSE;
	
	if (! XtoD(dwMaxRun, strTimes))
		return FALSE;
	
	return TRUE;
}

CString CBizPhoneSettingDlg::LoadSerialCodeFromRegistry(void)
{
	CString strSerial("");
	
	CWinApp* pApp = AfxGetApp();
	strSerial = pApp->GetProfileString(STRING_PROFILE_SECTION_REGISTER, STRING_PROFILE_ENTRY_SERIALCODE);
	
	return strSerial;
}

void CBizPhoneSettingDlg::SaveSerialCodeIntoRegistry(CString strSerial)
{
	// 写入注册表项
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(STRING_PROFILE_SECTION_REGISTER, STRING_PROFILE_ENTRY_SERIALCODE, strSerial);
}

DWORD CBizPhoneSettingDlg::LoadRunNumberFromRegistry(void)
{
	CString strProbation("");
	
	CWinApp* pApp = AfxGetApp();
	strProbation = pApp->GetProfileString(STRING_PROFILE_SECTION_REGISTER, STRING_PROFILE_ENTRY_PROBATION);
	
	if (strProbation.IsEmpty())
		return 0;
	
	ProcessAccordingGivenString(strProbation, STRING_RUNTIME_CODE_FILTER, FALSE);
	
	return atol(strProbation);
}

void CBizPhoneSettingDlg::SaveRunNumberIntoRegistry(DWORD dRunNumber)
{
	CString strProbation("");
	strProbation.Format("%d", dRunNumber);
	while (strProbation.GetLength()<4)
	{
		strProbation.Insert(0,'0');
	}
	
	ProcessAccordingGivenString(strProbation, STRING_RUNTIME_CODE_FILTER);
	//	for (int i=0; i< chARRAY.GetLength(); i++)
	//	{
	//		strProbation.SetAt( i, chARRAY[i] + strNumber[i] - '0');
	//	}
	
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(STRING_PROFILE_SECTION_REGISTER, STRING_PROFILE_ENTRY_PROBATION, strProbation);
}

CString CBizPhoneSettingDlg::FormatCustomStringFromHardwareID()
{
	CString chARRAY(STRING_CUSTOM_CODE_FILTER), chARRAYPre(STRING_CUSTOM_CODE_FILTER_PRE), chARRAYPos(STRING_CUSTOM_CODE_FILTER_POS),strPre,strMid,strPos, strAll;
	
	DWORD dwIDESerial, dwPre, dwPos;
	GetHardwareID(dwIDESerial);
	dwPre = (dwIDESerial & 0xffff0000)/0x10000;
	dwPos = (dwIDESerial & 0xffff);
	
	strPre.Format(_T("%04X"), dwPre&dwPos);
	strPos.Format(_T("%04X"), dwPre|dwPos);
	strMid.Format(_T("%04X"), dwIDESerial);
	
	ProcessAccordingGivenString(strPre, chARRAYPre);
	ProcessAccordingGivenString(strPos, chARRAYPos);
	ProcessAccordingGivenString(strMid, chARRAY);
	
	strAll.Format(_T("%s-%s-%s"), strPre, strMid, strPos);
	strAll.Insert(9,'-');
	
	return strAll;
}

BOOL CBizPhoneSettingDlg::ProcessAccordingGivenString(CString& strObject, CString strGiven, BOOL bAdd)
{
	ASSERT(strObject.GetLength() == strGiven.GetLength());
	
	UINT d1, d2, d;
	for (int i=0; i<strGiven.GetLength(); i++)
	{
		if ((strObject[i]<= '9')&&(strObject[i]>= '0'))
			d1 = strObject[i] - '0';
		else if ((strObject[i]<= 'Z')&&(strObject[i]>= 'A'))
			d1 = strObject[i] - 'A' + 10;
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}
		
		if ((strGiven[i]<= '9')&&(strGiven[i]>= '0'))
			d2 = strGiven[i] - '0';
		else if ((strGiven[i]<= 'Z')&&(strGiven[i]>= 'A'))
			d2 = strGiven[i] - 'A' + 10;
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}
		
		if (!bAdd)
		{
			if (d1-d2<0)
			{
				ASSERT(FALSE);
				return FALSE;
			}
			d = d1-d2;
		}
		else
			d = d1+d2;
		
		
		if (d<10)
			strObject.SetAt(i, d+'0');
		else
			strObject.SetAt(i,d-10+'A');
	}
	return TRUE;
}


BOOL CBizPhoneSettingDlg::VerifyInputedSerialCode(CString strInputedSerial, BOOL bFirst)
{
	strInputedSerial.MakeUpper();
	
	if (strInputedSerial.IsEmpty())
	{
		AfxMessageBox(_T("输入的注册码不得为空！请重新输入！"), MB_ICONERROR);
		return FALSE;
	}
	
	strInputedSerial.Remove('-');
	if (strInputedSerial.GetLength()!=20)
	{
		AfxMessageBox(_T("输入的客户码不正确！请重新输入！"), MB_ICONERROR);
		return FALSE;
	}
	
	// 得到去除连词号的最后字串
	CString strPre, strPos, strMid, strTimes, strPre1, strPos1;
	strPre = strInputedSerial.Left(4);
	strMid = strInputedSerial.Mid(4,8);
	strPos = strInputedSerial.Mid(12,4);
	strTimes = strInputedSerial.Right(4);
	
	if (!ProcessAccordingGivenString(strPre, STRING_SERIAL_CODE_FILTER_PRE, FALSE))
		return FALSE;
	
	if (!ProcessAccordingGivenString(strPos, STRING_SERIAL_CODE_FILTER_POS, FALSE))
		return FALSE;
	
	if (!ProcessAccordingGivenString(strMid, STRING_SERIAL_CODE_FILTER, FALSE))
		return FALSE;
	
	if (!ProcessAccordingGivenString(strTimes, STRING_SERIAL_CODE_FILTER_TIMES, FALSE))
		return FALSE;
	
	DWORD dwIDESerial, dwPre, dwPos, dwTimes;
	if (! XtoD(dwIDESerial, strMid))
		return FALSE;
	
	if (! XtoD(dwTimes, strTimes))
		return FALSE;
	
	dwPre = (dwIDESerial & 0xffff0000)/0x10000;
	dwPos = (dwIDESerial & 0xffff);
	
	strPre1.Format(_T("%04X"), dwPre&dwTimes);
	strPos1.Format(_T("%04X"), dwPre|dwTimes);
	
	if ((strPre1 == strPre) && (strPos1 == strPos))
	{
		if (bFirst)
		{
			CString strInfo;
			strInfo.Format(_T("您已注册本软件！限制使用次数为（%d）次！"), dwTimes);
			AfxMessageBox(strInfo);
		}
		return TRUE;
	}
	
	AfxMessageBox(_T("输入的注册码不正确！请重新输入！"), MB_ICONERROR);
	return FALSE;
}

BOOL CBizPhoneSettingDlg::XtoD(DWORD& dValue, CString strMid)
{
	dValue = 0;
	int N = strMid.GetLength();
	
	for (int i= 0; i< N; i++)
	{
		DWORD one = 0;
		TCHAR ch = strMid[i];
		
		if (!((ch<= '9')&&(ch>= '0') || (ch<= 'Z')&&(ch>= 'A')))
			return FALSE;
		
		if (ch>='A')
			one = 10 + ch -'A';
		else
			one = ch -'0';
		
		dValue = dValue*16 + one;
	}
	
	return TRUE;
}

void CBizPhoneSettingDlg::AppForceExit(void)
{
	OnDestroy();
	exit(0);
}
#endif

CompareIniState IniParaCmp(CString szFileOne, CString szFileTwo, CString szSecName)
{/* 完全相等比较 */
	CIniFile IniFileOne(szFileOne),
		     IniFileTwo(szFileTwo);

	CStringArray  arrayIdentOne, arrayIdentTwo;

	int nIdentSizeOne = 0,
		nIdentSizeTwo = 0;

	CString szValOne  = _T(""),
		    szValTwo  = _T("");

	IniFileOne.ReadSection(szSecName, arrayIdentOne);
	IniFileTwo.ReadSection(szSecName, arrayIdentTwo);

	nIdentSizeOne = arrayIdentOne.GetSize();
	nIdentSizeTwo = arrayIdentTwo.GetSize();

	if (nIdentSizeOne != nIdentSizeTwo)
		return CIS_DIF;

	if (0 == nIdentSizeOne)
		return CIS_SAME;

	for (nIdentSizeTwo=0; nIdentSizeTwo<nIdentSizeOne; nIdentSizeTwo++)
	{
		szValOne = IniFileOne.ReadString(szSecName, arrayIdentOne.GetAt(nIdentSizeTwo), _T("Val1"));
		szValTwo = IniFileTwo.ReadString(szSecName, arrayIdentOne.GetAt(nIdentSizeTwo), _T("Val2"));
		if (szValOne != szValTwo)
		{
			return CIS_DIF;
		}
	}

	return CIS_SAME;
}

CompareIniState IniParaIcmp(CString szFileOne, CString szFileTwo, CString szSecName)
{/* 主要是对[PARA]这部分作特殊化处理 [CLEAR_EMERGENCY_SETTING, INT_OPERATION_NAME_CMD+CLEAR_OPN_CFG]*/
	return CIS_SAME;
}

BOOL CompareParaIni(CString szFileOne, CString szFileTwo)
{
	_ASSERTE(szFileOne.GetLength() && szFileTwo.GetLength());

	CIniFile IniFileOne(szFileOne),
		     IniFileTwo(szFileTwo);

	CStringArray  arraySecsOne,  arraySecsTwo,
		          arrayIdentOne, arrayIdentTwo,
				  arrayIdentMax;

	CString       szValOne,      szValTwo;

	int indexSecs     = 0,
		indexIdent    = 0,
		nSecsSizeOne  = 0,
		nSecsSizeTwo  = 0,
		nIdentSizeOne = 0,
		nIdentSizeTwo = 0,
		nIdentSizeMax = 0;

	IniFileOne.ReadSections(arraySecsOne);
	IniFileTwo.ReadSections(arraySecsTwo);

	nSecsSizeOne = arraySecsOne.GetSize();
	nSecsSizeTwo = arraySecsTwo.GetSize();

    if (nSecsSizeOne != nSecsSizeTwo)
	{// Secs 数量不等
		return FALSE;
	}

	for (indexSecs=0; indexSecs<nSecsSizeOne; indexSecs++)
	{
		arrayIdentOne.RemoveAll();
		arrayIdentTwo.RemoveAll();
        // 读取同一个名节下的所有子项
		IniFileOne.ReadSection(arraySecsOne.GetAt(indexSecs), arrayIdentOne);
		IniFileTwo.ReadSection(arraySecsOne.GetAt(indexSecs), arrayIdentTwo);

		// 子项数量必须相等
		nIdentSizeOne = arrayIdentOne.GetSize();
		nIdentSizeTwo = arrayIdentTwo.GetSize();

		if (nIdentSizeOne > nIdentSizeTwo)
		{/* 以arrayIdentOne为参照对象作对比 */
			arrayIdentMax.Copy(arrayIdentOne);
			nIdentSizeMax = nIdentSizeOne;
		}
		else
		{
			arrayIdentMax.Copy(arrayIdentTwo);
			nIdentSizeMax = nIdentSizeTwo;
		}

		// 每一个子项的值也要相等
		for (indexIdent=0; indexIdent<nIdentSizeMax; indexIdent++)
		{
			szValOne = IniFileOne.ReadString(arraySecsOne.GetAt(indexSecs), arrayIdentMax.GetAt(indexIdent), _T(""));
			szValTwo = IniFileTwo.ReadString(arraySecsOne.GetAt(indexSecs), arrayIdentMax.GetAt(indexIdent), _T(""));

			if (szValOne != szValTwo)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}
