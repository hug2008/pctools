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
#include "PropPageSys.h"
#include "PropPageLocal.h"
#include "PropPageDirectTable.h"
#include "PropPageForbidTable.h"
#if defined(ENABLE_MOBILE_HEAD)
#include "PropPageHeader.h"
#endif
#include "PropPageLocalMobileSect.h"
#include "PropPageRefuseTable.h"
#include "PropPagePinSim.h"
#include "PropPageLock.h"

#if defined(LOCK_LAC)
#include "PropPageLac.h"
#endif

#include "PropPageIP.h"
#include "PropPageMic.h"
#include "PropPageOutput.h"
#include "PropPageData.h"
#include "PropPageFee.h"
#include "PropPageFeeSect.h"
#include "PropPageLocalPhone.h"

#ifdef    SIM_ENCRYPT_COMPLETE_SOLUTIONS
#include "PropPageEncrypt.h"
#endif

#include "StringCheckEncode.h"


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


#define FEE_FILE_YYMMDDHHMMSS  _T("010101000000")/*2001-01-01 00:00:00 */

/////////////////////////////////////////////////////////////////////////////
// CBizPhoneSettingDlg dialog
CSerialComm MainSerial;
CMainContent MainContent;

PropTreeNote pageNode[PAGE_TOTAL] = 
{
	PROP_TREE_NOTE_INIT(PROPPAGE_SYS,          IDD_PROPPAGE_1_SYS,                    "System"),
	PROP_TREE_NOTE_INIT(PROPPAGE_PINSIM,       IDD_PROPPAGE_8_PIN_SIM,                "Pin&Sim"),
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCK,         IDD_PROPPAGE_9_LOCK,                   "Lock"),
	PROP_TREE_NOTE_INIT(PROPPAGE_DIRECT,       IDD_PROPPAGE_3_DIRECT_TABLE,           "Hot"),
	PROP_TREE_NOTE_INIT(PROPPAGE_FORBID,       IDD_PROPPAGE_4_FORBID_TABLE,           "Forbidden"),
	PROP_TREE_NOTE_INIT(PROPPAGE_REFUSE,       IDD_PROPPAGE_7_REFUSE_TABLE,           "Restricted"),
#if defined(ENABLE_MOBILE_HEAD)
	PROP_TREE_NOTE_INIT(PROPPAGE_MOBILE_HEADER,IDD_PROPPAGE_MOBILE_HEADER,            "Mobile Header"),
#endif
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCAL_MOBILE, IDD_PROPPAGE_5_LOCAL_MOBILESECT,       "Local Mobile"),

#if defined(ENABLE_AREA_CODE)
	PROP_TREE_NOTE_INIT(PROPPAGE_AREA_CODE,    IDD_PROPPAGE_2_LOCAL,                  "Area Code"),
#endif

#if defined(LOCK_LAC)
	PROP_TREE_NOTE_INIT(PROPPAGE_LOCK_LAC,     IDD_PROPPAGE_LOCK_LAC,                  "Lock Lac"),
#endif
	PROP_TREE_NOTE_INIT(PROPPAGE_IP,           IDD_PROPPAGE_A_IP,                     "IP Dial"),
	PROP_TREE_NOTE_INIT(PROPPAGE_MIC,          IDD_PROPPAGE_C_MIC,                    "MIC"),
	PROP_TREE_NOTE_INIT(PROPPAGE_FEE,          IDD_PROPPAGE_H_FEE,                    "Rate table"),
	PROP_TREE_NOTE_INIT(PROPPAGE_FEESECT,      IDD_PROPPAGE_I_FEESECT,                "Rate rule"),
	PROP_TREE_NOTE_INIT(PROPPAGE_SWITCH,       IDD_PROPPAGE_G_LOCAL_PHONE,            "Rate flag"),
#ifdef SIM_ENCRYPT_COMPLETE_SOLUTIONS
	PROP_TREE_NOTE_INIT(PROPPAGE_ENCRYPT,      IDD_PROPPAGE_16_ENCRYPTED_SOLUTION,    "Encrypt"),
#endif
	PROP_TREE_NOTE_INIT(PROPPAGE_OUTPUT,       IDD_PROPPAGE_X_OUTPUT,                 "Info"),
	PROP_TREE_NOTE_INIT(PROPPAGE_DATA,         IDD_PROPPAGE_B_DATA,                   "Admin")
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
CBizPhoneSettingDlg::CBizPhoneSettingDlg(CWnd* pParent /*=NULL*/)
: CStackDialog(IDC_SETTING_SHEET_PLACEHOLDER, CBizPhoneSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBizPhoneSettingDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	
	/*����FIFO�߳�*/
	MainContent.m_pFIFOThread = AfxBeginThread(RUNTIME_CLASS(CSendDataFIFO));
	
	/*����COMM�߳�*/
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
	// �鿴�Ƿ�ע���ˣ�Yes->2, No->1��
	//
	// 1��ûע����Ļ����ͼ������custom string��Ҫ������ע���룬������Ļ����˳�����
	//    Ȼ����֤�����룬��֤��ͨ���Ļ����˳����У�Ȼ��д��֤�뵽ע����ע���������һ��3
	//
	// 2����ע����Ļ�������ע������֤ע�����е�ע���룬��ȷ�Ļ�������һ��3������ȷ���˳�����
	//////////////////////////////////////////////////////////////////////////
	
	
	//	// ���Ӳ�����к�
	//	CString strHardwareID = GetHardwareID();
	
	CString strRegistryItem = LoadSerialCodeFromRegistry();
	if (strRegistryItem.IsEmpty())
	{
		// ��֧ 1 ûע����Ļ�
		
		// �������custom string
		CString strCustomString;
		strCustomString = FormatCustomStringFromHardwareID();
		
		// Ҫ������ע����
		BOOL bFlag = FALSE;
		CDlgSerial dlg;
		dlg.m_strCustomID = strCustomString;
		
		while(!bFlag)
		{
			dlg.m_strInputedSerial = "";
			if (IDCANCEL == dlg.DoModal())
			{
				QuitSoftware(0);	// ������Ļ����˳�����
				return FALSE;
			}
			
			// ��֤������
			if (VerifyInputedSerialCode(dlg.m_strInputedSerial, TRUE))
				// ��֤��ͨ���Ļ���Ҫ���ٴ���������
				bFlag = TRUE;
		}
		
		// д��֤�뵽ע����ע����
		strRegistryItem = dlg.m_strInputedSerial;
		SaveSerialCodeIntoRegistry(strRegistryItem);
	}
	else
	{
		// ��ע����Ļ�
		
		// ����ע�����
		strRegistryItem;
		
		// ��֤ע�����е�ע����
		if (!VerifyInputedSerialCode(strRegistryItem))
		{
			// ��֤��ͨ���Ļ���Ҫ���ٴ���������
			
			// �������custom string
			CString strCustomString;
			strCustomString = FormatCustomStringFromHardwareID();
			
			// Ҫ������ע����
			BOOL bFlag = FALSE;
			CDlgSerial dlg;
			dlg.m_strCustomID = strCustomString;
			
			while(!bFlag)
			{
				if (IDCANCEL == dlg.DoModal())
				{
					QuitSoftware(0);	// ������Ļ����˳�����
					return FALSE;
				}
				
				// ��֤������
				if (VerifyInputedSerialCode(dlg.m_strInputedSerial))
					// ��֤��ͨ���Ļ���Ҫ���ٴ���������
					bFlag = TRUE;
			}
			
			// д��֤�뵽ע����ע����
			strRegistryItem = dlg.m_strInputedSerial;
			SaveSerialCodeIntoRegistry(strRegistryItem);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ���� 3�� ��֤���еĴ��� ������>N->4, ����<=N ->5��
	//
	// 3����ע����ж��������˵Ĵ�������ע�����ж���������д������Ƚ϶���
	//
	// 4��֪ͨ�����˿������е����������˳�����
	//
	// 5�����������ۼ�Ȼ�����ע���Ȼ��ʼ���г���
	//////////////////////////////////////////////////////////////////////////
	
	// ������һ��3
	DWORD dRunNumber = 0;
	dRunNumber = LoadRunNumberFromRegistry();
	
	DWORD dRunMaxNum = 0;
	LoadMaxRunNumFromRegisterInfo(dRunMaxNum, strRegistryItem);
	
	
	if (dRunNumber >= dRunMaxNum)
	{
		// 4��֪ͨ�����˿������е����������˳�����
		QuitSoftware(1, dRunMaxNum);
		return FALSE;
	}
	else
	{
		// 5�����������ۼ�Ȼ�����ע���Ȼ��ʼ���г���
		dRunNumber++;
		SaveRunNumberIntoRegistry(dRunNumber);
	}
	
	RunSoftware();
#endif
	
	MainSerial.initialize(this);
	SetTimer(DETECTTIMER, FIRSTTTIME, NULL);
	
	
	// ----------------------------------------------------------------------
	// ���濪ʼ�Զ���ĳ�ʼ������
	// ----------------------------------------------------------------------
	
	m_bDataChanged = FALSE;		   // ��ʼ���������������ݡ�Ϊ��
	
	// Set the tooltip string of CButtonXP button
	m_btExit.SetToolTipText(IDS_DLG_COMMON_CLOSE);
	
	m_ctrTree.DeleteAllItems();

	int i = 0;
	CString szVerInfo =  _T("");

	/* ��������һ����ʼ���ļ�� */
	for (i=0; i<PAGE_TOTAL; i++)
	{
		if (i != pageNode[i].pageId)
		{
			szVerInfo.Format(_T("pageNode[%d].pageId = %d"), i, pageNode[i].pageId);
			MessageBox(szVerInfo, SYS_NAME, MB_ICONSTOP);
			return TRUE;
		}
	}
	if (PROPPAGE_LOCAL_MOBILE > PROPPAGE_FEE)
	{
		szVerInfo.Format(_T("PROPPAGE_LOCAL_MOBILE > PROPPAGE_FEE"));
		MessageBox(szVerInfo, SYS_NAME, MB_ICONSTOP);
		return TRUE;
	}

	if (PROPPAGE_FEE > PROPPAGE_FEESECT)
	{
		szVerInfo.Format(_T("PROPPAGE_FEE > PROPPAGE_FEESECT"));
		MessageBox(szVerInfo, SYS_NAME, MB_ICONSTOP);
		return TRUE;
	}
	/* ��������һ����ʼ���ļ�� */
	
	for (i=0; i<PAGE_TOTAL; i++)
	{
		m_htiRoot[i]  = m_ctrTree.InsertItem(pageNode[i].szName);
		AddPage(pageNode[i].nSrcId);
	}
	
	m_ctrTree.Expand(m_htiRoot[0], TVE_EXPAND);
	
	GetDlgItem(IDC_PIC_GIF_LOGO)->ShowWindow(FALSE);
#if defined(USE_WAIT_WND)
	m_dlgWaitMsg.Create(IDD_WAIT_DIALOG);
#endif
	
	// ����������m_ctrTree��Show Selection Always������
	// ���û���趨һ��ѡ���ϵͳ���Զ�ѡ����һ��
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

#if defined(ENABLE_MOBILE_HEAD)
	case IDD_PROPPAGE_MOBILE_HEADER:
		return new CPropPageHeader(this);
#endif
		
	case IDD_PROPPAGE_5_LOCAL_MOBILESECT:
		return new CPropPageLocalMobileSect(this);
		
	case IDD_PROPPAGE_7_REFUSE_TABLE:
		return new CPropPageRefuseTable(this);
		
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
		
#ifdef SIM_ENCRYPT_COMPLETE_SOLUTIONS
	case IDD_PROPPAGE_16_ENCRYPTED_SOLUTION:
		return new CPropPageEncrypt(this);	
#endif
		
	case IDD_PROPPAGE_X_OUTPUT:
		return new CPropPageOutput(this);

	case IDD_PROPPAGE_H_FEE:
		return new CPropPageFee(this);

	case IDD_PROPPAGE_I_FEESECT:
		return new CPropPageFeeSect(this);
		break;

	case IDD_PROPPAGE_G_LOCAL_PHONE:
		return new CPropPageLocalPhone(this);

#if defined(LOCK_LAC)
	case IDD_PROPPAGE_LOCK_LAC:
		return new CPropPageLac(this);
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

#if defined(ENABLE_MOBILE_HEAD)
		case IDD_PROPPAGE_MOBILE_HEADER:
			break;
#endif
			
		case IDD_PROPPAGE_5_LOCAL_MOBILESECT:
			break;
			
		case IDD_PROPPAGE_7_REFUSE_TABLE:
			break;
			
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
			
#ifdef   SIM_ENCRYPT_COMPLETE_SOLUTIONS
		case IDD_PROPPAGE_16_ENCRYPTED_SOLUTION:
			break;
#endif //SIM_ENCRYPT_COMPLETE_SOLUTIONS
		case IDD_PROPPAGE_X_OUTPUT:
			break;

		case IDD_PROPPAGE_H_FEE:
			break;

		case IDD_PROPPAGE_I_FEESECT:
			break;

		case IDD_PROPPAGE_G_LOCAL_PHONE:
			break;

#if defined(LOCK_LAC)
		case IDD_PROPPAGE_LOCK_LAC:
			break;
#endif
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
	// �ڹرոöԻ���֮ǰ������֤�Ƿ�Ϸ��رգ�Ҳ���ǵ�ǰ�Ƿ��ڶ�д
	if (0 == ((CCommThread*)MainContent.m_pCommThread)->commThreadState)
	{
		// ��������������ı䣬��Ҫ���Ǳ���
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
	//		MessageBox(_T("���ڶ�д���ݣ���ʱ�����˳�ϵͳ��"),SYS_NAME,MB_ICONSTOP);
}

BOOL CBizPhoneSettingDlg::DestroyWindow() 
{
#if defined(USE_WAIT_WND)
	// ����WaitMsg Dialog
	if (m_dlgWaitMsg.GetSafeHwnd())
		m_dlgWaitMsg.DestroyWindow();
#endif
	
	return CStackDialog::DestroyWindow();
}

void CBizPhoneSettingDlg::OnDestroy() 
{
	CStackDialog::OnDestroy();
	
	// ����ȫ�ֱ������������̵߳Ĳ���Ӧ���ڴ�������֮�����
	
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
	// ֻ�е����ִ����ʱ�򣬲�ֱ��ѡ�г����ҳ��
	m_ctrTree.SelectItem(m_htiRoot[nPage]);
	return 0;
}

BOOL CBizPhoneSettingDlg::CloseTest(void)
{
	int nPage = 0;
	CStackPage* pStackPage = NULL;
	
	// ���浱ǰ�ҳ��
	CStackPageHelper *pPageCurrent = getCurrent();
	
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it++, nPage++) 
	{
		// ע�͸���䣬��������֤������ȷ�Ե�ʱ��Ҫ��ͣ���ڸ���
		// ҳ��֮���л��������
		// m_ctrTree.SelectItem(m_htiRoot[nPage++]);			// ѡ�и�ҳ
		
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
	// ������ҳ�����ݱ��浽ȫ�ֱ�����
	CStackPage* pStackPage = NULL;
	CStackPageHelperList::iterator it;
	
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) 
	{
		pStackPage = it->GetPage();
		pStackPage->OnSaveData();
	}
	
	// Ȼ�󣬿��ļ�����������д��ű��ļ�
	return TRUE;
}

void CBizPhoneSettingDlg::LoadIniFile(CString strLoadFile, BOOL bFromIni)
{
	// ����INI�ļ�
	ASSERT(!strLoadFile.IsEmpty()); 

	CIniFile m_fIniFile(strLoadFile);
	int    Index = 0;
	unsigned int nCommand = 0;

	BOOL   bHasNoneUsefulIni = FALSE;

	if (strLoadFile.IsEmpty())
	{
		bHasNoneUsefulIni = TRUE;
		theApp.m_szIniInput.Empty();
		CString strInfo = _T("This is not valid a script!");
		MessageBox(strInfo,SYS_NAME,MB_ICONSTOP);
		ShowStatus(strInfo, FALSE);
		return;
	}


	CStringArray strCommandAry;
	m_fIniFile.ReadSection(STRING_SECTION1_NAME, strCommandAry);
	int nCommandNum = strCommandAry.GetSize();

	CStringArray strATCommandAry;
	m_fIniFile.ReadSection(STRING_SECTION2_NAME, strATCommandAry);
	int nATCommandNum = strATCommandAry.GetSize();

	CString strCommand   = _T(""),
	        strValue     = _T(""),
			strATCommand = _T(""),
			strATValue   = _T("");

	
	if (nCommandNum + nATCommandNum ==0 )
	{
		// ����������ļ�Ϊ�գ������ļ�����
		bHasNoneUsefulIni = TRUE;
		theApp.m_szIniInput.Empty();
		CString strInfo = _T("There is nothing valid could be matched in the script!");
		MessageBox(strInfo,SYS_NAME,MB_ICONSTOP);
		ShowStatus(strInfo, FALSE);
		return;
	}
	/*
	if (bFromIni) 
	{
		bHasNoneUsefulIni = TRUE;
		theApp.m_szIniInput.Empty();
		CString strInfo = _T("This is not valid a script!");
		MessageBox(strInfo,SYS_NAME,MB_ICONSTOP);
		ShowStatus(strInfo, FALSE);
		return;
	}
	*/
	/************************************************************************/
	/*    ���һ��ԭ�е�����                                                */
	/************************************************************************/
	Reset3In1PhSetting();

	/*  PARA ����   */
	for (Index=0; Index<nCommandNum; Index++)
	{
		strCommand = strCommandAry[Index];
		strValue   = m_fIniFile.ReadString(STRING_SECTION1_NAME, strCommand, "");
		nCommand   = atoi(strCommand);

#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
		/*      ���ܿ���������Կ                                                */
		if ((nCommand >= CMD_SIM_ENCRYPT_KEY1_RW) && (nCommand <= CMD_SIM_ENCRYPT_KEY5_RW))
		{
			szEncryptConfigMainSimMpkKey689[nCommand - CMD_SIM_ENCRYPT_KEY1_RW] = strValue;
			continue;
		}
#endif

		/*           PHONE_HOT_NUMBER_SETTING  ֱ���Ŷβ���                     */
		if ((nCommand >= CMD_HOT_NUMBER_STR_RW) && (nCommand <= CMD_HOT_NUMBER_END_RW))
		{
			szHotNum2001[nCommand - CMD_HOT_NUMBER_STR_RW] = strValue;
			continue;
		}

		/*      PHONE_BARRING_NUMBER_SETTING   �����Ŷβ���                     */
		if ((nCommand >= CMD_BARRING_NUMBER_STR_RW) && (nCommand <= CMD_BARRING_NUMBER_END_RW))
		{
			szBarringCallNum3001[nCommand - CMD_BARRING_NUMBER_STR_RW] = strValue;
			continue;
		}

		/*      PHONE_LOCAL_NUMBER_HEADER_SETTING      ���غ���β���           */
		if ((nCommand >= CMD_LOCALPHONE_HEADER_STR_RW) && (nCommand <= CMD_LOCALPHONE_HEADER_END_RW))
		{
			szLocalPhoneNumberHead[nCommand - CMD_LOCALPHONE_HEADER_STR_RW] = strValue;
			continue;
		}

		/*        PHONE_LIMIT_INCOMING_NUMBER_SETTING  ���ƺ��벿��             */
		if ((nCommand >= CMD_LIMITINCOMING_HEADER_STR_RW) && (nCommand <= CMD_LIMITINCOMING_HEADER_END_RW))
		{
			szLimitIncomingCallHead5001[nCommand - CMD_LIMITINCOMING_HEADER_STR_RW] = strValue;
			continue;
		}
		
#if defined(LOCK_LAC)
		/*      PHONE_LOCK_CELL_SETTING   ��С������ Lac                         */
		if ((nCommand >= CMD_LOCK_CELL_LAC_INFO_STR_RW) && (nCommand <= CMD_LOCK_CELL_LAC_INFO_END_RW))
		{
			szLacInf8006[nCommand - CMD_LOCK_CELL_LAC_INFO_STR_RW] = strValue;
			continue;
		}
#endif

		/*     PHONE_IPHEDER_SETTING  Ip ���Ź��򲿷� ���������                */
		if ((nCommand >= CMD_IP_SETTING_STR_RW) && (nCommand <= CMD_IP_SETTING_END_RW))
		{
			szMajoripFramIpSetting[nCommand - CMD_IP_SETTING_STR_RW] = strValue;
			continue;
		}

		/*      PHONE_INT_SPEED_DIAL_SETTING     �����ٲ�����                   */
#if defined(INTELLIGENT_SPEED_DIAL)
		if ((nCommand >= CMD_INTELLIGENT_SPEED_DIAL_STR_RW) && (nCommand <= CMD_INTELLIGENT_SPEED_DIAL_END_RW))
		{
			szIntSpeedDialSetting[nCommand - CMD_INTELLIGENT_SPEED_DIAL_STR_RW] = strValue;
			continue;
		}
#endif

		/*    PHONE_SPECIAL_DIAL_SETTING     ���Ⲧ�Ų�����                     */
#if defined(SPECIAL_DIAL)
		if ((nCommand >= CMD_SPECIAL_DIAL_STR_RW) && (nCommand <= CMD_SPECIAL_DIAL_END_RW))
		{
			szSpecialDialSetting[nCommand - CMD_SPECIAL_DIAL_STR_RW] = strValue;
			continue;
		}
#endif

		/*      �����ʱ�����,�շѹ���������                                     */
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
		if ((nCommand >= CMD_MAJOR_RATE_SETTING_STR_RW) && (nCommand <= CMD_MAJOR_RATE_SETTING_END_RW))
		{
			szMajorRateFramRateSetting12001[nCommand - CMD_MAJOR_RATE_SETTING_STR_RW] = strValue;
			continue;
		}

		if ((nCommand >= CMD_MAJOR_RATE_HEADER_TYPE_STR_RW) && (nCommand <= CMD_MAJOR_RATE_HEADER_TYPE_END_RW))
		{
			szMajorRateFramRateHeadSetting[nCommand - CMD_MAJOR_RATE_HEADER_TYPE_STR_RW] = strValue;
			continue;
		}

#if defined(VG_MINOR_RATE)
		if ((nCommand >= CMD_MINOR_RATE_SETTING_STR_RW) && (nCommand <= CMD_MINOR_RATE_SETTING_END_RW))
		{
			szMinorRateFramRateSetting12171[nCommand - CMD_MINOR_RATE_SETTING_STR_RW] = strValue;
			continue;
		}

		if ((nCommand >= CMD_MINOR_RATE_HEADER_TYPE_STR_RW) && (nCommand <= CMD_MINOR_RATE_HEADER_TYPE_END_RW))
		{
			szMinorRateFramRateHeadSetting[nCommand - CMD_MINOR_RATE_HEADER_TYPE_STR_RW] = strValue;
			continue;
		}
#endif
#endif

		/************************************************************************/
		/*        PHONE_MOBILE_HEAD_SETTING �ƶ��绰�Ŷ�ͷ                      */
		/************************************************************************/
#if defined(ENABLE_MOBILE_HEAD)
		if ((nCommand >= CMD_MOBILE_HEAD_STR_RW) && (nCommand <= CMD_MOBILE_HEAD_END_RW))
		{
			szMobileHeader14001[nCommand - CMD_MOBILE_HEAD_STR_RW] = strValue;
			continue;
		}
#endif

		switch (nCommand)
		{
		case CMD_DIAL_WAITING_TIME_RW:
			szDialWaitingTime02 = strValue;
			break;

		case CMD_HOW_TO_USE_CITY_NUMBER_RW:
			szHowToUseCityNumber03 = strValue;
			break;

		case CMD_SMS_CENTER_NUMBER_RW:
			szMscNum04 = strValue;
			break;

		case CMD_BARING_OTHER_CITY_NUMBER_RW:
			szBaringOtherCityNum05 = strValue;
			break;

		case CMD_SMS_RECEIVE_NUMBER_RW:
			szMscReceiveNum06 = strValue;
			break;

		case CMD_DISPLAY_ORIGINAL_NUMBER_RW:
			szDisplayOriginalNumber07 = strValue;
			break;

#if defined(CELL_LEVEL)
		case CMD_CELL_LEVEL_QUALITY_R:
			szCurrentCellLevelQuality08 = strValue;
			break;
#endif
			
		case CMD_CLIENT_ID_RW:
			szClientID09 = strValue;
			break;

		case CMD_LOCAL_NUMBER_ACTIVE_RW:
			szLocalNumberActive11 = strValue;
			break;

#if defined(ICCID_BUILD)
		case CMD_IMEI_CCID_INFO_R:
			szImeiCcid12 = strValue;
			break;
#endif

		case CMD_SIM_ENCRYPT_FACTORYID_RW:
			szEncryptConfigfactoryId17 = strValue;
			break;

		case CMD_SIM_ENCRYPT_COMPATIBLE_NORMAL_RW:
			szEncryptConfigBCompatibleWithNormalSim18 = strValue;
			break;

		case CMD_SIM_ENCRYPT_SOLUTION_RW:
			szEncryptConfigSimEncryptSolution19 = strValue;
			break;

		case CMD_SIM_ENCRYPT_EDITION_ID_RW:
			szEncryptConfigEditionId31 = strValue;
			break;

		case CMD_SWITCH_INTELLIGENT_DAIL_RW:
			szIntelligentDial26 = strValue;
			break;

		case CMD_SWITCH_AOTU_LOCK_RW:
			szAutoLock27 = strValue;
			break;

		case CMD_SWITCH_TOLL_INDICTION_RW:
			szTollIndiction28 = strValue;
			break;

		case CMD_SWITCH_SPECIAL_APP_RW:
			szSpecialApplication29 = strValue;
			break;

		case CMD_USER_TAG_RW:
			szUserTag30 = strValue;
			break;

		case CMD_CITY_NUMBER_RW:
			szCityNum1000 = strValue;
			break;

		case CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW:
			szAddZeroIfOtherCity4000 = strValue;
			break;

		case CMD_LIMIT_ALL_INCOMING_CALL_RW:
			szLimitAllIncomingCall5000 = strValue;
			break;

		case CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW:
			 szIsHandsetAutoLockPinEnable6000 = strValue;
			break;

		case CMD_PIN_SETTING_PIN_LEN_RW:
			 szPinLen6001 = strValue;
			break;

		case CMD_PIN_SETTING_INIT_PIN_RW:
			 szInitPin6002 = strValue;
			break;

		case CMD_PIN_SETTING_ENCRYPT_PIN_RW:
			 szEncryptPin6003 = strValue;
			break;

		case CMD_PIN_SETTING_FIX_PIN_RW:
			 szFixPin6004 = strValue;
			break;

		case CMD_PIN_SETTING_USE_ENCRYPT_PIN_RW:
			 szUseEncryptPinFlag6005 = strValue;
			break;

		case CMD_LOCK_SIM_BAND_TO_HANDSET_RW:
			 szIsHandsetCanBeBindedToASim7000 = strValue;
			break;

		case CMD_LOCK_SIM_LOCKED_IMSI_RW:
			 szLockedImsi7001 = strValue;
			break;

		case CMD_LOCK_SIM_SP_LOCK_ENABLE_RW:
			 szIsServiceProviderLockEnable7002 = strValue;
			break;

		case CMD_LOCK_SIM_LOCKED_SERVICE_PROVIDER_RW:
			 szLockedServiceProvider7003 = strValue;
			break;

		case CMD_LOCK_CELL_LOCK_ENABLE_RW:
			 szIsCellLockEnable8000 = strValue;
			break;

		case CMD_LOCK_CELL_LOCKED_CELL_NUM_RW:
			 szLockedCellNum8001 = strValue;
			break;

		case CMD_LOCK_CELL_AUTO_LOCK_ENABLE_RW:
			 szIsCellAutoLockEnable8002 = strValue;
			break;

		case CMD_LOCK_CELL_INFO_RW:
			 szCellInf8003 = strValue;
			break;

#if defined(LOCK_LAC)
        case CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW:
			 szIsLacLockEnable8004 = strValue;
			break;

		case CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW:
			 szLockedLacNum8005 = strValue;
			break;
#endif

#if defined(ENABLE_SESSION_TOLL_FUNCTION)
		case CMD_MAJOR_RATE_SETTING_START_TIME_RW:
			 szMajorRateFramStartTime12026 = strValue;
			break;

		case CMD_MAJOR_RATE_SETTING_FILE_ID_RW:
			 szMajorRateFramID12027 = strValue;
			break;

		case CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R:
			 szMajorRateFramRateHeadCount12028 = strValue;
			break;

		case CMD_MAJOR_RATE_SETTING_RATE_COUNT_R:
			 szMajorRateFramRateCount12029 = strValue;
			break;

		case CMD_MAJOR_RATE_SETTING_FRAM_FLAG_R:
			 szMajorRateFramFramFlag12030 = strValue;
			break;

		case CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R:
			 szMajorRateFramRateVerify12800 = strValue;
			break;

#if defined(VG_MINOR_RATE)
		case CMD_MINOR_RATE_SETTING_START_TIME_RW:
			 szMinorRateFramStartTime12196 = strValue;
			break;

		case CMD_MINOR_RATE_SETTING_FILE_ID_RW:
			 szMinorRateFramID12197 = strValue;
			break;

		case CMD_MINOR_RATE_SETTING_HEAD_COUNT_R:
			 szMinorRateFramRateHeadCount12198 = strValue;
			break;

		case CMD_MINOR_RATE_SETTING_RATE_COUNT_R:
			 szMinorRateFramRateCount12199 = strValue;
			break;

		case CMD_MINOR_RATE_SETTING_FRAM_FLAG_R:
			 szMinorRateFramFramFlag12200 = strValue;
			break;

		case CMD_MINOR_RATE_DOWNLOAD_VERIFY_CODE_R:
			 szMinorRateFramRateVerify12970 = strValue;
			break;
#endif
#endif

		/*           ���������������,������                    */
		case CMD_HOT_NUMBER_CLEAR_ALL_W:
		case CMD_BARRING_NUMBER_CLEAR_ALL_W:
		case CMD_LOCALPHONE_NUMBER_CLEAR_ALL_W:
		case CMD_IP_SETTING_CLEAR_ALL_W:
#if defined(INTELLIGENT_SPEED_DIAL)
		case CMD_INTELLIGENT_SPEED_DIAL_CLEAR_ALL_W:
#endif
#if defined(SPECIAL_DIAL)
		case CMD_ISPECIAL_DIAL_CLEAR_ALL_W:
#endif
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
		case CMD_RATE_SETTING_CLEAR_ALL_W:
#endif
#if defined(ENABLE_MOBILE_HEAD)
		case CMD_MOBILE_HEAD_CLEAR_ALL_W:
#endif
			break;
		/*           ���������������,������                    */

		default:
			ASSERT(FALSE);
			break;
		}

	}/*  PARA ����   */

	/*  ATCMD ����  */
	for (Index=0; Index<nATCommandNum; Index++)
	{
		strATCommand = strATCommandAry[Index];
		strATValue   = m_fIniFile.ReadString(STRING_SECTION2_NAME, strATCommand, "");
		// --------------------------------------------------------
		// MIC
		// --------------------------------------------------------

		if (strATCommand == STR_CMIC0_CMD)					//1	"AT+CMIC0" // ������ͨ����˷�����
		{
			// strATValue ��Ϊ "0,2" 
			ASSERT( strATValue.Left(1)== "0" );
			m_strMIC_CMIC0_nCMICMain = strATValue.Mid(strATValue.Find(',')+1);
		}
		else if (strATCommand == STR_CMIC1_CMD)				//2	"AT+CMIC1" // ���ø���ͨ����˷�����
		{
			// strATValue ��Ϊ "1,2" 
			ASSERT( strATValue.Left(1)== "1" );
			m_strMIC_CMIC1_nCMICAux = strATValue.Mid(strATValue.Find(',')+1);
		}
#if defined(R16_ATC)
		else if (strATCommand == STR_SIDET0_CMD)				//3	"AT+SIDET0" // ��������������
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
		else if (strATCommand == STR_SIDET1_CMD)				//3	"AT+SIDET1" // ���ø���������
		{
			ASSERT( strATValue.Left(1) == "1" );
			m_strMIC_SIDET_AuxGain  = strATValue.Mid(strATValue.Find(',') + 1);
		}
#else
		else if (strATCommand == STR_SIDET_CMD)				//3	"AT+SIDET" // ���ò�������
		{
			// strATValue ��Ϊ "4096" 
			m_strMIC_SIDET_Gain = strATValue;
		}
#endif
		else if (strATCommand == STR_ECHO0_CMD)				//4	"AT+ECHO0" // ������ͨ���ز�����
		{
			// strATValue ��Ϊ "0,0,0,0" 
			ASSERT( strATValue.Right(1)== "0" );

			// ������ͨ�� - �ز����� - �������ϵ��
			int nPos = strATValue.Find(',');
			m_strMIC_ECHO0_Vox = strATValue.Left(nPos);

			// ������ͨ�� - �ز����� - ��˷���ֵ
			int nNext = strATValue.Find(',', nPos+1);
			m_strMIC_ECHO0_Min = strATValue.Mid(nPos+1, nNext-nPos-1);
			nPos = nNext;

			// ������ͨ�� - �ز����� - �ز����Ƴ���
			nNext = strATValue.Find(',', nPos+1);
			m_strMIC_ECHO0_Samp = strATValue.Mid(nPos+1, nNext-nPos-1);
		}
		else if (strATCommand == STR_ECHO1_CMD)				//5	"AT+ECHO1" // ���ø���ͨ���ز�����
		{
			// strATValue ��Ϊ "0,0,0,1" 
			ASSERT( strATValue.Right(1)== "1" );

			// ���ø���ͨ�� - �ز����� - �������ϵ��
			int nPos = strATValue.Find(',');
			m_strMIC_ECHO1_Vox = strATValue.Left(nPos);

			// ���ø���ͨ�� - �ز����� - ��˷���ֵ
			int nNext = strATValue.Find(',', nPos+1);
			m_strMIC_ECHO1_Min = strATValue.Mid(nPos+1, nNext-nPos-1);
			nPos = nNext;

			// ���ø���ͨ�� - �ز����� - �ز����Ƴ���
			nNext = strATValue.Find(',', nPos+1);
			m_strMIC_ECHO1_Samp = strATValue.Mid(nPos+1, nNext-nPos-1);
		}
		else if (strATCommand == STR_CTRS_CMD)				//6	"AT+CTRS" // ���ø���ͨ����˷�����
		{
			// strATValue ��Ϊ "10" 
			m_strSYS_LCD_Contrast = strATValue;
		}
		else
		{
			ASSERT(FALSE);	// some thing error happened here
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
	
	// ˢ�����е�ҳ��
	RefreshPage();
	
	// ��������Ѿ��޸Ĺ���
	m_bDataChanged = TRUE;
}


void CBizPhoneSettingDlg::SaveIniFile(CString strSaveFile, BOOL bFromIni)
{
	ASSERT(!strSaveFile.IsEmpty()); 
	unsigned int i             = 0,
		         IndexNotEmpty = 0;
	
	// д��INI�ļ�
	CIniFile m_fIniFile(strSaveFile);
	CString strCommand(""), strValue("");

	/*       �ٲ���ʱ                                                       */
	strCommand.Format(_T("%d"), CMD_DIAL_WAITING_TIME_RW);
	strValue.Format(_T("\"%s\""), szDialWaitingTime02);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue); 

	/*       ��������ʹ�÷�ʽ                                               */
	strCommand.Format(_T("%d"), CMD_HOW_TO_USE_CITY_NUMBER_RW); 
	strValue.Format(_T("\"%s\""), szHowToUseCityNumber03);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue); 


	/*       �������ĺ���                                                   */
	strCommand.Format(_T("%d"), CMD_SMS_CENTER_NUMBER_RW); 
	strValue.Format(_T("\"%s\""), szMscNum04);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue); 


	/*       �Ƿ��ֹ����;�绰                                           */
	strCommand.Format(_T("%d"), CMD_BARING_OTHER_CITY_NUMBER_RW); 
	strValue.Format(_T("\"%s\""), szBaringOtherCityNum05);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

#if defined(XMSTART_NET_AND_COMMGR_ENABLE)
	/*       ���Ž������ĺ���                                               */
	strCommand.Format(_T("%d"), CMD_SMS_RECEIVE_NUMBER_RW); 
	strValue.Format(_T("\"%s\""), szMscReceiveNum06);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ��ʾԭʼ����                                                    */
	strCommand.Format(_T("%d"), CMD_DISPLAY_ORIGINAL_NUMBER_RW); 
	strValue.Format(_T("\"%s\""), szDisplayOriginalNumber07);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      С���ź�ǿ��������Ϣ    CMD_CELL_LEVEL_QUALITY_R ֻ��           */

	/*      ������ID��Ϣ                                                    */
	strCommand.Format(_T("%d"), CMD_CLIENT_ID_RW); 
	strValue.Format(_T("\"%s\""), szClientID09);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif

	/*      �����ֻ��Ŷ��Ƿ���Ч                                            */
	strCommand.Format(_T("%d"), CMD_LOCAL_NUMBER_ACTIVE_RW); 
	strValue.Format(_T("\"%s\""), szLocalNumberActive11);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      Imei��Ccid������Ϣ          CMD_IMEI_CCID_INFO_R                */

#if defined(SIM_ENCRYPT_COMPLETE_SOLUTIONS)
	/*      ���ܿ�����                                                      */
	strCommand.Format("%d", CMD_SIM_ENCRYPT_SOLUTION_RW);
	strValue.Format(_T("\"%s\""), szEncryptConfigSimEncryptSolution19);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	if (13 == GetEncryptSolution(szEncryptConfigSimEncryptSolution19))
	{
		/*      Sim������ID��Ϣ                                                 */
		strCommand.Format("%d", CMD_SIM_ENCRYPT_FACTORYID_RW);
		strValue.Format(_T("\"%s\""), szEncryptConfigfactoryId17);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

		/*      �Ƿ������ͨSim��                                               */
		strCommand.Format("%d", CMD_SIM_ENCRYPT_COMPATIBLE_NORMAL_RW);
		strValue.Format(_T("\"%s\""), szEncryptConfigBCompatibleWithNormalSim18);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

		/*      ���ܿ���������Կ                                                */
		for (i=0; i<5; i++)
		{
			strCommand.Format("%d", CMD_SIM_ENCRYPT_KEY1_RW + i);
			strValue.Format(_T("\"%s\""), szEncryptConfigMainSimMpkKey689[i]);
			m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
		}

		/*      SIM�汾��Ϣ                                                     */
		strCommand.Format("%d", CMD_SIM_ENCRYPT_EDITION_ID_RW);
		strValue.Format(_T("\"%s\""), szEncryptConfigEditionId31);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	}
#endif
	
	

#if defined(XMSTART_3IN1_NET_MANAGER)
	/*      ���ܲ��ſ���                                                    */
	strCommand.Format("%d", CMD_SWITCH_INTELLIGENT_DAIL_RW);
	strValue.Format(_T("\"%s\""), szIntelligentDial26);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      �Զ���������                                                    */
	strCommand.Format("%d", CMD_SWITCH_AOTU_LOCK_RW);
	strValue.Format(_T("\"%s\""), szAutoLock27);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      �շ���ʾ������                                                  */
	strCommand.Format("%d", CMD_SWITCH_TOLL_INDICTION_RW);
	strValue.Format(_T("\"%s\""), szTollIndiction28);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ����Ӧ�ÿ���                                                    */
	strCommand.Format("%d", CMD_SWITCH_SPECIAL_APP_RW);
	strValue.Format(_T("\"%s\""), szSpecialApplication29);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      �û���ʶ��Ϣ                                                    */
	strCommand.Format("%d", CMD_USER_TAG_RW);
	strValue.Format(_T("\"%s\""), szUserTag30);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif


	/*      ��������                                                        */
	strCommand.Format("%d", CMD_CITY_NUMBER_RW);
	strValue.Format(_T("\"%s\""), szCityNum1000);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);



	/*        ֱ���Ŷβ��� ������ٱ���ǿ�                                 */
	/*      �������ֱ���Ŷ�                                                */
	strCommand.Format("%d", CMD_HOT_NUMBER_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	IndexNotEmpty = 0;
	for (i=0; i<NUM_HOT_NUMBER_TOTAL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_HOT_NUMBER_STR_RW + i);
		strValue.Format(_T("\"%s\""), szHotNum2001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#else
		if (szHotNum2001[i].IsEmpty())
		{
			continue;
		}
		strCommand.Format("%d", CMD_HOT_NUMBER_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szHotNum2001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif
	}


	/*             �����Ŷβ���     ������ٱ���ǿ�                        */
	/*      ������н����Ŷ�                                                */
	strCommand.Format("%d", CMD_BARRING_NUMBER_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	IndexNotEmpty = 0;
	for (i=0; i<NUM_BARRING_NUMBER_TOTAL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_BARRING_NUMBER_STR_RW + i);
		strValue.Format(_T("\"%s\""), szBarringCallNum3001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#else
		if (szBarringCallNum3001[i].IsEmpty())
		{
			continue;
		}
		strCommand.Format("%d", CMD_BARRING_NUMBER_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szBarringCallNum3001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif
	}




	/*      PHONE_LOCAL_NUMBER_HEADER_SETTING      ���غ���β���           */
	/*      ����;�Զ��� '0'                                                */
	strCommand.Format("%d", CMD_ADD_ZERO_OHTER_CITY_NUMBER_RW);
	strValue.Format(_T("\"%s\""),  szAddZeroIfOtherCity4000);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ������б��غŶ�                                                */
	strCommand.Format("%d", CMD_LOCALPHONE_NUMBER_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	IndexNotEmpty = 0;
	for (i=0; i<NUM_LOCALPHONE_HEADER_TOTAL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_LOCALPHONE_HEADER_STR_RW + i);
		strValue.Format(_T("\"%s\""), szLocalPhoneNumberHead[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#else
		if (szLocalPhoneNumberHead[i].IsEmpty())
		{
			continue;
		}
		strCommand.Format("%d", CMD_LOCALPHONE_HEADER_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szLocalPhoneNumberHead[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif
	}




	/*                       ���ƺ��벿��                                   */
	/*      �������к���                                                    */
	strCommand.Format("%d", CMD_LIMIT_ALL_INCOMING_CALL_RW);
	strValue.Format(_T("\"%s\""),  szLimitAllIncomingCall5000);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ���ƺ���Ŷ�,��ʼ��ַ                                           */
	for (i=0; i<NUM_LIMITINCOMING_HEADER_TOTAL; i++)
	{
		strCommand.Format("%d", CMD_LIMITINCOMING_HEADER_STR_RW + i);
		strValue.Format(_T("\"%s\""), szLimitIncomingCallHead5001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	}
	


	/************************************************************************/
	/*                    Pin������                                         */
	/************************************************************************/
	/*      �����Զ���PIN��                                                 */
	strCommand.Format("%d",  CMD_PIN_SETTING_AUTO_LOCK_PIN_ENABLE_RW);
	strValue.Format(_T("\"%s\""), szIsHandsetAutoLockPinEnable6000);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ����PIN�볤��                                                   */
	strCommand.Format("%d",  CMD_PIN_SETTING_PIN_LEN_RW);
	strValue.Format(_T("\"%s\""), szPinLen6001);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ���ó�ʼPIN��                                                   */
	strCommand.Format("%d",  CMD_PIN_SETTING_INIT_PIN_RW);
	strValue.Format(_T("\"%s\""), szInitPin6002);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	// ----------------------------------------------------------
	// 2003.12.23 ���ݷ������Ľ��飬���6004�̶�PIN��Ϊ��
	// ���۹̶�PIN�봰���Ƿ�enable��
	// ���������·����������ݣ�
	// ----------------------------------------------------------
	if (!szFixPin6004.IsEmpty())
	{
		/*      ���ù̶�PIN��                                                   */
		strCommand.Format("%d",  CMD_PIN_SETTING_FIX_PIN_RW);
		strValue.Format(_T("\"%s\""), szFixPin6004);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	}
	
	/*      �Ƿ�ʹ�����PIN��                                               */
	strCommand.Format("%d",  CMD_PIN_SETTING_USE_ENCRYPT_PIN_RW);
	strValue.Format(_T("\"%s\""), szUseEncryptPinFlag6005);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	/*      ������SIM������                                               */
	strCommand.Format("%d",  CMD_LOCK_SIM_BAND_TO_HANDSET_RW);
	strValue.Format(_T("\"%s\""), szIsHandsetCanBeBindedToASim7000);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ������SIM����(imsi)                                           */
	strCommand.Format("%d",  CMD_LOCK_SIM_LOCKED_IMSI_RW);
	strValue.Format(_T("\"%s\""), szLockedImsi7001);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      �����Ƿ�������Ӫ��                                              */
	strCommand.Format("%d",  CMD_LOCK_SIM_SP_LOCK_ENABLE_RW);
	strValue.Format(_T("\"%s\""), szIsServiceProviderLockEnable7002);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ����������Ӫ�̺�                                                */
	strCommand.Format("%d",  CMD_LOCK_SIM_LOCKED_SERVICE_PROVIDER_RW);
	strValue.Format(_T("\"%s\""), szLockedServiceProvider7003);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	/*      �Ƿ���С��                                                      */
	strCommand.Format("%d",  CMD_LOCK_CELL_LOCK_ENABLE_RW);
	strValue.Format(_T("\"%s\""), szIsCellLockEnable8000);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	/*      ����С������Ŀ                                                  */
	strCommand.Format("%d",  CMD_LOCK_CELL_LOCKED_CELL_NUM_RW);
	strValue.Format(_T("\"%s\""), szLockedCellNum8001);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      �Ƿ��Զ���С��                                                  */
	strCommand.Format("%d",  CMD_LOCK_CELL_AUTO_LOCK_ENABLE_RW);
	strValue.Format(_T("\"%s\""), szIsCellAutoLockEnable8002);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      �Ѿ�������С����                                                */
	strCommand.Format("%d",  CMD_LOCK_CELL_INFO_RW);
	strValue.Format(_T("\"%s\""), szCellInf8003);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


#if defined(LOCK_LAC)
    /*      �Ƿ�������                                                      */
    strCommand.Format("%d",  CMD_LOCK_CELL_LAC_LOCK_ENABLE_RW);
	strValue.Format(_T("\"%s\""), szIsLacLockEnable8004);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	/*      ������������Ŀ                                                  */
	strCommand.Format("%d",  CMD_LOCK_CELL_LAC_LOCKED_CELL_NUM_RW);
	strValue.Format(_T("\"%s\""), szLockedLacNum8005);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	/* 24��ָ�����600��Lac��  */
	for (i=0; i<LAC_DOZEN_COUNT_AT_TOTAL; i++)
	{
		strCommand.Format("%d",  CMD_LOCK_CELL_LAC_INFO_STR_RW + i);
		strValue.Format(_T("\"%s\""), szLacInf8006[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	}
	
#endif


	/************************************************************************/
	/*                 Ip ���Ź��򲿷�   ���������                         */
	/************************************************************************/
	/*      �������Ip ���Ź���                                             */
	strCommand.Format("%d", CMD_IP_SETTING_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	

	IndexNotEmpty = 0;
	for (i=0; i<NUM_IP_HEADER_TATOL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_IP_SETTING_STR_RW + i);
		strValue.Format(_T("\"%s\""), szMajoripFramIpSetting[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#else
		if (IsIpSettingItemEmpty(szMajoripFramIpSetting[i]))
		{
			continue;
		}
		strCommand.Format("%d", CMD_IP_SETTING_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szMajoripFramIpSetting[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif
	}


	/************************************************************************/
	/*                       �����ٲ��������ò���                           */
	/************************************************************************/
#if defined(INTELLIGENT_SPEED_DIAL)
	/*      ������������ٲ���������                                        */
	strCommand.Format("%d", CMD_INTELLIGENT_SPEED_DIAL_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	IndexNotEmpty = 0;
	for (i=0; i<NUM_INT_SPEED_DIAL_TOTAL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_INTELLIGENT_SPEED_DIAL_STR_RW + i);
		strValue.Format(_T("\"%s\""), szIntSpeedDialSetting[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#else
		if (szIntSpeedDialSetting[i].IsEmpty())
		{
			continue;
		}
		strCommand.Format("%d", CMD_INTELLIGENT_SPEED_DIAL_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szIntSpeedDialSetting[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif
	}	
#endif


	/************************************************************************/
	/*                       ���Ⲧ�Ų�������                               */
	/************************************************************************/
#if defined(SPECIAL_DIAL)
	/*      ����������Ⲧ�Ų�������                                        */
	strCommand.Format("%d", CMD_ISPECIAL_DIAL_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	IndexNotEmpty = 0;
	for (i=0; i<NUM_SPECIAL_DIAL_TOTAL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_SPECIAL_DIAL_STR_RW + i);
		strValue.Format(_T("\"%s\""), szSpecialDialSetting[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#else
		if (szSpecialDialSetting[i].IsEmpty())
		{
			continue;
		}
		strCommand.Format("%d", CMD_SPECIAL_DIAL_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szSpecialDialSetting[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif
	}	
#endif

	/************************************************************************/
	/*                             ���ʱ����ò���                           */
	/************************************************************************/
	/*      ������з�������                                                */
	strCommand.Format("%d", CMD_RATE_SETTING_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	/*      ���ʱ�����,�շѹ���������                                       */
	IndexNotEmpty = 0;
	for (i=0; i<NUM_RATE_SETTING_TABLE_TOTAL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_MAJOR_RATE_SETTING_STR_RW + i);
		strValue.Format(_T("\"%s\""), szMajorRateFramRateSetting12001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#else
		if (IsRateTableItemEmpty(szMajorRateFramRateSetting12001[i]))
		{
			continue;
		}
		strCommand.Format("%d", CMD_MAJOR_RATE_SETTING_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szMajorRateFramRateSetting12001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
#endif
	}

	/*      �����ʱ�����,���ʺŶ���ʼ��ַ                                   */
	IndexNotEmpty = 0;
	for (i=0; i<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; i++)
	{
		if (IsRateRuleItemEmpty(szMajorRateFramRateHeadSetting[i]))
		{
			continue;
		}
		strCommand.Format("%d", CMD_MAJOR_RATE_HEADER_TYPE_STR_RW + IndexNotEmpty++);
		strValue.Format(_T("\"%s\""), szMajorRateFramRateHeadSetting[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	}


	/*      �����ʱ�����,����ʱ��                                           */
	strCommand.Format("%d", CMD_MAJOR_RATE_SETTING_START_TIME_RW);
	strValue.Format(_T("\"%s\""), szMajorRateFramStartTime12026);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	/*      �����ʱ�����, �����ļ� Id                                       */
	strCommand.Format("%d", CMD_MAJOR_RATE_SETTING_FILE_ID_RW);
	strValue.Format(_T("\"%s\""), szMajorRateFramID12027);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);



	/************************************************************************/
	/*      �����ʱ�����, �����ļ�֡���                                    */
	/*      ���ڴ���д���ļ�֡��ǵĽӿ�Ϊ12028,�����ȡ���ʺŶμ�¼����    */
	/*      �Ľӿ�һ��,���ҷ��ʺŶμ�¼������û��д�ӿڵ�,�����ڴ�ֱ��ʹ    */
	/*      ���ʺŶμ�¼������������д���ļ�֡��ǵĽӿ�12028����(���ݾ�    */
	/*      ����Ҫ��)                                                       */
	/************************************************************************/
	/*      �����ʱ�����, �����ļ�֡���                                    */
	strCommand.Format("%d", CMD_MAJOR_RATE_SETTING_HEAD_COUNT_R);
	strValue.Format(_T("\"%s\""), szMajorRateFramRateHeadCount12028);
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);


	/*      �����ʱ�����,�����ļ�У����                                     */
	/*strCommand.Format("%d", CMD_MAJOR_RATE_DOWNLOAD_VERIFY_CODE_R);
	strValue.Format(_T("\"%s\""), MakeFeeVerifyCode());
	if (10 == strValue.GetLength())
	{
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	}
	*/



	/************************************************************************/
	/*        PHONE_MOBILE_HEAD_SETTING �ƶ��绰�Ŷ�ͷ                      */
	/************************************************************************/
#if defined(ENABLE_MOBILE_HEAD)
	IndexNotEmpty = 0;
	/*      ��������ƶ��绰�Ŷ�ͷ                                          */
	strCommand.Format("%d", CMD_MOBILE_HEAD_CLEAR_ALL_W);
	strValue.Format(_T("\"%s\""), "CLEAR");
	m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);

	for (i=0; i<MOBILE_HEAD_TOTAL; i++)
	{
#if defined(FULL_LIST)
		strCommand.Format("%d", CMD_MOBILE_HEAD_STR_RW + i);
#else
		if (szMobileHeader14001[i].IsEmpty())
		{
			continue;
		}
		strCommand.Format("%d", CMD_MOBILE_HEAD_STR_RW + IndexNotEmpty++);
#endif
		strValue.Format(_T("\"%s\""), szMobileHeader14001[i]);
		m_fIniFile.WriteString(STRING_SECTION1_NAME, strCommand, strValue);
	}
#endif



	/************************************************************************/
	/*        ATCMD ���ֵ�ATָ���ʽ                                        */
	/************************************************************************/
	
	CString strATValue;
	
	// AT+CMIC0=0,2
	strATValue = m_strMIC_CMIC0_nCMICMain;		//1	"CMIC0" // ������ͨ����˷�����
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_CMIC0_CMD, "0," + strATValue); 
	
	// AT+CMIC1=1,2
	strATValue = m_strMIC_CMIC1_nCMICAux;		//2	"CMIC1" // ���ø���ͨ����˷�����
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_CMIC1_CMD, "1," + strATValue); 
	
#if defined(R16_ATC)
#if defined(REVERSE_SIDET)
	strATValue = m_strMIC_SIDET_AuxGain;			//3	"SIDET1" // ���ø���������
	strATValue = _T("1,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET1_CMD, strATValue); 

	strATValue = m_strMIC_SIDET_MainGain;			//3	"SIDET0" // ��������������
	strATValue = _T("0,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET0_CMD, strATValue);
#else
	strATValue = m_strMIC_SIDET_MainGain;			//3	"SIDET0" // ��������������
	strATValue = _T("0,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET0_CMD, strATValue); 
	
	strATValue = m_strMIC_SIDET_AuxGain;			//3	"SIDET1" // ���ø���������
	strATValue = _T("1,") + strATValue;
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET1_CMD, strATValue); 
#endif
#else
	// AT+SIDET=4096
	strATValue = m_strMIC_SIDET_Gain;			//3	"SIDET" // ���ò�������
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_SIDET_CMD, strATValue); 
#endif
	
	// AT+ECHO0=0,0,0,0							//4	"ECHO0" // ������ͨ���ز�����
	strATValue.Format("%s,%s,%s,0", m_strMIC_ECHO0_Vox, m_strMIC_ECHO0_Min, m_strMIC_ECHO0_Samp);
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_ECHO0_CMD, strATValue); 
	
	// AT+ECHO1=0,0,0,1							//5	"ECHO1" // ���ø���ͨ���ز�����
	strATValue.Format("%s,%s,%s,1", m_strMIC_ECHO1_Vox, m_strMIC_ECHO1_Min, m_strMIC_ECHO1_Samp);
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_ECHO1_CMD, strATValue); 
	
	// AT+CMIC0=0,2
	strATValue = m_strSYS_LCD_Contrast;		//1	"CMIC0" // ������ͨ����˷�����
	m_fIniFile.WriteString(STRING_SECTION2_NAME, STR_CTRS_CMD, strATValue); 
	// ����������ҳ

	if (bFromIni) 
	{
		CString strInfo; 
		strInfo.Format(_T("successful to export script[%s]..."), strSaveFile.Mid(strSaveFile.ReverseFind('\\')+1));
		ShowStatus(strInfo);
	}
	
	// �������Ϊ�����޸Ĺ�
	m_bDataChanged = FALSE;
}


BOOL CBizPhoneSettingDlg::CompareIniSrcAndReadBack(CString szIniSrc, CString ReadBack)
{
	return CompareParaIni(szIniSrc, ReadBack);
}

LRESULT CBizPhoneSettingDlg::OnReceiveLowNotify(WPARAM wParam, LPARAM lParam)
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
	
	/* �յ�����Ϣ��> �ı�ʾ�� */
	static BOOL  ReceSMFlag   = FALSE;
    
	// �жϽ��ջ��������������ݵ������ֱ�ӷ���
	if(!MainSerial.m_bConnected || (wParam & EV_RXCHAR) != EV_RXCHAR)
	{
		SetEvent(MainSerial.m_hPostMsgEvent);  //��������һ��WM_COMMNOTIFY��Ϣ
		return 0L;
	}
	
	// �Ӵ��ڽ��ջ������н����ݶ���
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
LRESULT CBizPhoneSettingDlg::OnReceiveHighNotify(WPARAM wParam, LPARAM lParam)
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
// MainFrame�յ��ײ㷢���Ŀ�ʼ����ĳ������AT�������Ϣָʾ
LRESULT CBizPhoneSettingDlg::OnStartSendAtcommandNotify(WPARAM wParam, 
													 LPARAM lParam)
{
	// �õ����ڷ��͵���������
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
	ShowStatus(_T("�޷�ɾ����ʱ�ļ���������ֹ��"), FALSE);
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
#ifdef SIM_ENCRYPT_COMPLETE_SOLUTIONS
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
	UINT curState  = wParam;
	UINT curCmd    = lParam;
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

	case ACTIVATE_COMMPORT_BEING:			//������ʱʱ��
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
	
	// Enable���еĲ�����ť
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	return 0;
}

LRESULT CBizPhoneSettingDlg::OnReadParaOkNotify(WPARAM wParam, LPARAM lParam)
{
	ShowStatus("Load all parameter successful!");	
	LoadFromModule();
	//m_btnReadSetting.EnableWindow();
#if defined(USE_WAIT_WND)
	if (m_dlgWaitMsg.IsWindowVisible())
	{
		m_dlgWaitMsg.ShowWindow(FALSE);
		Invalidate();
	}
#endif
	// Enable���еĲ�����ť
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
	
#if !defined(FOR_CLIENT)
	{
		// �Ƚϲ����ļ�
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
	
	// ���趨�˸������벢�����صı�־��ʱ��
	if (m_bChangePwdAndSaveSettingAndWriteDefault)
	{
		// ����־λ��λ
		m_bChangePwdAndSaveSettingAndWriteDefault = FALSE;
	}
	
	// Enable���еĲ�����ť
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
	case WRITE_TOTAL_SETTING_END_WRITE_STATE:
		ShowStatus("Successful to save all parameter!");
		//m_btnWriteSetting.EnableWindow();
		break;
	case WRITE_TOTAL_ATCMD_END_WRITE_STATE:
		ShowStatus("Successful to save all AT Cmd!");
		break;
	}
	
	// ֻ�е�ȫ����������֮�����ж��Ƿ�ʼ��һ������
	if (WRITE_TOTAL_ATCMD_END_WRITE_STATE == curState)
	{
		// ��λ���ݱ����־λ
		m_bDataChanged = FALSE;
#if defined(USE_WAIT_WND)
		// �ر���ʾ��Ϣ�ĶԻ���
		if (m_dlgWaitMsg.IsWindowVisible())
		{
			m_dlgWaitMsg.ShowWindow(FALSE);
			Invalidate();
		}
#endif
		
		// ���趨�����ز��Ҵ�Ϊ�������õı�־��ʱ��
		if (m_bChangePwdAndSaveSettingAndWriteDefault)
		{
			// ����ȥ����ִ��ԭ���Ĳ���
			OnSaveModuleDefault(0,0);
			
			// ����־λ��λ
			m_bChangePwdAndSaveSettingAndWriteDefault = FALSE;
		}
		else
		{
			// Enable���еĲ�����ť
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
	
	// �����output page'd log listctrl
	//	if (m_pPageOutput)
	//		m_pPageOutput->OutputMessage(str, bNoError);
	
	CString* pStr = new CString;
	*pStr = str;
	
	// �����output page'd log listctrl
	// ���Ȼ��OutPut page��ָ��
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
	
	// ��������Ի����STATIC
	pWnd->SetWindowText(str);
#if defined(USE_WAIT_WND)
	// ������ȴ��Ի��� ���ߵ��������ĶԻ���
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
	
	// Enable���еĲ�����ť
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
	
	// ֻ�е�ȫ����������֮�����ж��Ƿ�ʼ��һ������
	if (WRITE_TO_FACTORY_END_STATE == curState)
	{
		// Enable���еĲ�����ť
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
	
	// Enable���еĲ�����ť
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
	
	// ���趨�˸������벢�����صı�־��ʱ��
	if (m_bChangePwdAndSaveSetting)
	{
		// ����־λ��λ
		m_bChangePwdAndSaveSetting = FALSE;
	}
	
	// Enable���еĲ�����ť
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
	
	// ֻ�е�ȫ����������֮�����ж��Ƿ�ʼ��һ������
	if (CHANGE_PASS_END_WRITE_STATE == curState)
	{
		// ���趨�˸������벢�����صı�־��ʱ��
		if (m_bChangePwdAndSaveSetting)
		{
			// ����ȥ����ִ��ԭ���Ĳ���
			OnSaveModuleSetting(0,0);
			
			// ����־λ��λ
			m_bChangePwdAndSaveSetting = FALSE;
		}
		else{
			// Enable���еĲ�����ť
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
	
	// ֻ�е�ȫ����������֮�����ж��Ƿ�ʼ��һ������
	if (RECOVER_DEFAULT_SETTING_END_WRITE_STATE == curState)
	{
		// Enable���еĲ�����ť
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
	{// �򿪴���
		if(MainSerial.OpenConnection() == FALSE)
		{
			ShowStatus("Fail to open comm!", FALSE);
			
			KillTimer(OPENCOMMTIMER);
			
			// Enable���еĲ�����ť
			MessageBox(_T("Fail to open comm!"), SYS_NAME, MB_OK|MB_ICONERROR);
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			//�������ѭ����
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
				// Ҫ�󼤻��
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
	{// �رմ���
		ShowStatus("Comm is closed!");
		MainSerial.CloseConnection();
		KillTimer(CLOSECOMMTIMER);
		//return;
	}
	// 2007.06.08 add by hqr to Open/Close Comm
	else if(nIDEvent == SENDINGOVERTIMER)
	{
		// Predefine var in the section
		// �ӽ��������д���ض����ļ�ȥ��Ȼ��֪ͨ�̷߳��͵�ģ��ȥ
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
		// �������AT����ͽ�����ʱ��
		ShowStatus("Task timeout!", FALSE);
#if defined(USE_WAIT_WND)
		if (m_dlgWaitMsg.IsWindowVisible())
		{
			m_dlgWaitMsg.ShowWindow(FALSE);
			Invalidate();
		}
#endif
		// Enable���еĲ�����ť
		(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
			SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
		
		
		//���ӳ�ʱ����Ϊ���Ҳ����նˣ������������Ҷ�ʱ��
		
		PostMessage(WM_CLOSE_COMM_CONNECTION_NOTIFY);
		
		//MainSerial.CloseConnection();
		//SetTimer(DETECTTIMER, DETECTTIME, NULL);
	}	// TODO: Add your message handler code here and/or call default
	
	CStackDialog::OnTimer(nIDEvent);
}

////////////////////////////////////////////////////////////
LRESULT CBizPhoneSettingDlg::OnStartSendingTimerNotify(WPARAM wParam,LPARAM lParam)
{
	SetTimer(SENDINGOVERTIMER, SENDINGTIME, NULL);	
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnStopSendingTimerNotify(WPARAM wParam,LPARAM lParam)
{
	KillTimer(SENDINGOVERTIMER);	
	return 0;
}
/////////////////////////////////////////////////////////////


// 2007.06.07 add by hqr to Open/Close Comm
LRESULT CBizPhoneSettingDlg::OnOpenCommConnectionNotify(WPARAM wParam,LPARAM lParam)
{
	SetTimer(OPENCOMMTIMER, COMMOPERATETIME, NULL);	
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnCloseCommConnectionNotify(WPARAM wParam,LPARAM lParam)
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
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnStopMonitorTimerNotify(WPARAM wParam,LPARAM lParam)
{
	KillTimer(MONITORTIMER);		
	return 0;
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
			
			// Enable���еĲ�����ť
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
		//����д��������
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
			// Enable���еĲ�����ť
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			return;
		}
	}
	
	ShowStatus("send save default task...");
	
	//���ͱ���Ϊ������������...
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
	
	//�����޸���������...
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
			// Enable���еĲ�����ť
			(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
				SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
			return;
		}
	}
	ShowStatus("send recover default task...");
	//ShowStatus2("");
	//m_btnRecoverDefaultSetting.EnableWindow(false);
	//���ͻָ�Ĭ����������...
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
	// ���ܵ���Ϣ��Ҫ�����ȫ�ֱ�������ˢ����ʾ
	ASSERT(!m_strExchangeFileName.IsEmpty());
	
	LoadIniFile(m_strExchangeFileName, TRUE);
	
	// Enable���еĲ�����ť
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	return 0;
}

BOOL CBizPhoneSettingDlg::RefreshPage(void)
{
	// ��������ȫ�ֱ����е��������ֵ�����page��
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
	
	// Ȼ�󣬿��ļ�����������д��ű��ļ�
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
	// ���ܵ���Ϣ��Ҫ�����ȫ�ֱ�������ˢ����ʾ
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
	
	// Enable���еĲ�����ť
	(GetPage(IDD_PROPPAGE_B_DATA)->GetPage())->
		SendMessage(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY,1, 0);
	return 0;
}
// Huang ye Feb. 21 2006  for activate COMM
LRESULT CBizPhoneSettingDlg::OnActivateCommPort(WPARAM wParam, LPARAM lParam)
{
	// ����ģ���е�����
	OnActivateComm();
	return 0;
}
LRESULT CBizPhoneSettingDlg::OnLoadModuleSetting(WPARAM wParam, LPARAM lParam)
{
	// ����ģ���е�����
	OnReadSetting();
	return 0;
}

void CBizPhoneSettingDlg::LoadFromModule(void)
{
	// ��ģ��д���ض��ļ��ж������ݣ���ˢ�½���
	CString strFileName = MainContent.m_sWorkPath + STRING_PARAFILE_NAME;
	
	LoadIniFile(strFileName, FALSE);
}

LRESULT CBizPhoneSettingDlg::OnSaveModuleSetting(WPARAM wParam, LPARAM lParam)
{
	// �ӽ��������д���ض����ļ�ȥ��Ȼ��֪ͨ�̷߳��͵�ģ��ȥ
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
		// Enable���еĲ�����ť
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

void CBizPhoneSettingDlg::Reset3In1PhSetting(void)
{
	int  Index = 0;
	/************************************************************************/
	/*                  PHONE_NORMAL_SETTING                                */
	/************************************************************************/
	szDialWaitingTime02.Empty();
	szHowToUseCityNumber03.Empty();
	szMscNum04.Empty();
	szBaringOtherCityNum05.Empty();
	szMscReceiveNum06.Empty();
	szDisplayOriginalNumber07.Empty();
#if defined(CELL_LEVEL)
	szCurrentCellLevelQuality08.Empty();
#endif
	szClientID09.Empty();
	szLocalNumberActive11.Empty();
#if defined(ICCID_BUILD)
	szImeiCcid12.Empty();
#endif
	szEncryptConfigfactoryId17.Empty();
	szEncryptConfigBCompatibleWithNormalSim18.Empty();
	szEncryptConfigSimEncryptSolution19.Empty();
	szEncryptConfigEditionId31.Empty();
	for (Index=0; Index<5; Index++)
	{
		szEncryptConfigMainSimMpkKey689[Index].Empty();
	}
	szIntelligentDial26.Empty();
	szAutoLock27.Empty();
	szTollIndiction28.Empty();
	szSpecialApplication29.Empty();
	szUserTag30.Empty();

	/************************************************************************/
	/*            PHONE_CITY_NUMBER_SETTING �������Ų���                    */
	/************************************************************************/
	szCityNum1000.Empty();

	/************************************************************************/
	/*           PHONE_HOT_NUMBER_SETTING  ֱ���Ŷβ���                     */
	/************************************************************************/
	for (Index=0; Index<NUM_HOT_NUMBER_TOTAL; Index++)
	{
		szHotNum2001[Index].Empty();
	}

	/************************************************************************/
	/*      PHONE_BARRING_NUMBER_SETTING   �����Ŷβ���                     */
	/************************************************************************/
	for (Index=0; Index<NUM_BARRING_NUMBER_TOTAL; Index++)
	{
		szBarringCallNum3001[Index].Empty();
	}

	/************************************************************************/
	/*      PHONE_LOCAL_NUMBER_HEADER_SETTING      ���غ���β���           */
	/************************************************************************/
	szAddZeroIfOtherCity4000.Empty();
	for (Index=0; Index<NUM_LOCALPHONE_HEADER_TOTAL; Index++)
	{
		szLocalPhoneNumberHead[Index].Empty();
	}

	/************************************************************************/
	/*        PHONE_LIMIT_INCOMING_NUMBER_SETTING  ���ƺ��벿��             */
	/************************************************************************/
	szLimitAllIncomingCall5000.Empty();
	for (Index=0; Index<NUM_LIMITINCOMING_HEADER_TOTAL; Index++)
	{
		szLimitIncomingCallHead5001[Index].Empty();
	}

	/************************************************************************/
	/*       PHONE_PIN_SETTING     Pin������                                */
	/************************************************************************/
	szIsHandsetAutoLockPinEnable6000.Empty();/*�����Զ���PIN��*/
	szPinLen6001.Empty();/*����PIN�볤��*/
	szInitPin6002.Empty();/*���ó�ʼPIN��*/
	szEncryptPin6003.Empty();/*���PIN��*/
	szFixPin6004.Empty();/*�̶�PIN��*/
	szUseEncryptPinFlag6005.Empty();/*�Ƿ�ʹ�����PIN��*/

	/************************************************************************/
	/*      PHONE_LOCK_SIM_SETTING           ������������Ӫ������           */
	/************************************************************************/
	szIsHandsetCanBeBindedToASim7000.Empty();/*���û�����SIM��*/
	szLockedImsi7001.Empty();/*�����󶨵�SIM����*/
	szIsServiceProviderLockEnable7002.Empty();/*�Ƿ�������Ӫ��*/
	szLockedServiceProvider7003.Empty();/*��������Ӫ��*/


	/************************************************************************/
	/*      PHONE_LOCK_CELL_SETTING   ��С������                            */
	/************************************************************************/
	szIsCellLockEnable8000.Empty();/*�Ƿ���С��*/
	szLockedCellNum8001.Empty();/*������С����*/
	szIsCellAutoLockEnable8002.Empty();/*�Ƿ��Զ���С��*/
	szCellInf8003.Empty();/*�Ѿ�������С����*/
#if defined(LOCK_LAC)
	szIsLacLockEnable8004.Empty();/*�Ƿ���lac*/
	szLockedLacNum8005.Empty();/*������Lac������*/
	/* LAC_DOZEN_COUNT_AT_TOTAL * LAC_DOZEN_ONE_AT_TOTAL * 4 */
	for (Index=0; Index<LAC_DOZEN_COUNT_AT_TOTAL; Index++)
	{/* ��������Lac�� */
		szLacInf8006[Index].Empty();
	}
#endif
	/************************************************************************/
	/*     PHONE_IPHEDER_SETTING  Ip ���Ź��򲿷� ���������                */
	/************************************************************************/
	for (Index=0; Index<NUM_IP_HEADER_TATOL; Index++)
	{
		szMajoripFramIpSetting[Index].Empty();
	}

	/************************************************************************/
	/*      PHONE_INT_SPEED_DIAL_SETTING     �����ٲ�����                   */
	/************************************************************************/
#if defined(INTELLIGENT_SPEED_DIAL)
	for (Index=0; Index<NUM_INT_SPEED_DIAL_TOTAL; Index++)
	{
		szIntSpeedDialSetting[Index].Empty();
	}
#endif


	/************************************************************************/
	/*    PHONE_SPECIAL_DIAL_SETTING     ���Ⲧ�Ų�����                     */
	/************************************************************************/
#if defined(SPECIAL_DIAL)
	for (Index=0; Index<NUM_INT_SPEED_DIAL_TOTAL; Index++)
	{
		szSpecialDialSetting[Index].Empty();
	}
#endif

	/************************************************************************/
	/*      PHONE_CURRENT_TOLL_RATE_SETTING ���ʱ����ò���                  */
	/************************************************************************/
#if defined(ENABLE_SESSION_TOLL_FUNCTION)
	/*      �����ʱ�����,�շѹ���������                                     */
	for (Index=0; Index<NUM_RATE_SETTING_TABLE_TOTAL; Index++)
	{
		szMajorRateFramRateSetting12001[Index].Empty();
	}
	/*      �����ʱ�����,����ʱ��                                           */
	szMajorRateFramStartTime12026.Empty();
	/*      �����ʱ�����, �����ļ� Id                                       */
	szMajorRateFramID12027.Empty();
	/*      �����ʱ�����, ���ʺŶμ�¼����                                  */
	szMajorRateFramRateHeadCount12028.Empty();
	/*      �����ʱ�����, ���ʱ��¼����                                    */
	szMajorRateFramRateCount12029.Empty();
	/*      �����ʱ�����, �����ļ�֡���                                    */
	szMajorRateFramFramFlag12030.Empty();
	/*      �����ʱ�����,���ʺŶ���ʼ��ַ                                   */
	for (Index=0; Index<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; Index++)
	{
		szMajorRateFramRateHeadSetting[Index].Empty();
	}
	/*      �����ʱ�����,�����ļ�У����                                     */
	szMajorRateFramRateVerify12800.Empty();

#if defined(VG_MINOR_RATE)
	/*      �η��ʱ�����,�շѹ���������                                     */
	for (Index=0; Index<NUM_RATE_SETTING_TABLE_TOTAL; Index++)
	{
		szMinorRateFramRateSetting12171[Index].Empty();
	}
	/*      �η��ʱ�����,����ʱ��                                           */
	szMinorRateFramStartTime12196.Empty();
	/*      �η��ʱ�����, �����ļ� Id                                       */
	szMinorRateFramID12197.Empty();
	/*      �η��ʱ�����, ���ʺŶμ�¼����                                  */
	szMinorRateFramRateHeadCount12198.Empty();
	/*      �η��ʱ�����, ���ʱ��¼����                                    */
	szMinorRateFramRateCount12199.Empty();
	/*      �η��ʱ�����, �����ļ�֡���                                    */
	szMinorRateFramFramFlag12200.Empty();
	/*      �η��ʱ�����,���ʺŶ���ʼ��ַ                                   */
	for (Index=0; Index<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; Index++)
	{
		szMinorRateFramRateHeadSetting[Index].Empty();
	}
	/*      �η��ʱ�����,�����ļ�У����                                     */
	szMinorRateFramRateVerify12970.Empty();

#endif
#endif

	/************************************************************************/
	/*        PHONE_MOBILE_HEAD_SETTING �ƶ��绰�Ŷ�ͷ                      */
	/************************************************************************/
#if defined(ENABLE_MOBILE_HEAD)
	for (Index=0; Index<MOBILE_HEAD_TOTAL; Index++)
	{
		szMobileHeader14001[Index].Empty();
	}
#endif
}

#if defined(ENABLE_SESSION_TOLL_FUNCTION)
CString CBizPhoneSettingDlg::MakeFeeVerifyCode(void)
{
	CString szFeeFile    = _T(""),
		    szFeeItem    = _T(""),
		    szVerifyCode = _T("");

	CStringArray  szAryFeeTab,
		          szAryFeeHeader;

	char    szFeeFmt[50] = "";

	BOOL    bFeeTan      = FALSE;

	int Index            = 0;

	szAryFeeTab.RemoveAll();
	for (Index=0; Index<NUM_RATE_SETTING_TABLE_TOTAL; Index++)
	{
		if (IsRateTableItemEmpty(szMajorRateFramRateSetting12001[Index]))
		{
			continue;
		}
		szAryFeeTab.Add(szMajorRateFramRateSetting12001[Index]);
	}


	szAryFeeHeader.RemoveAll();
	for (Index=0; Index<NUM_RATE_SETTING_HEADER_TYPE_TOTAL; Index++)
	{
		if (IsRateRuleItemEmpty(szMajorRateFramRateHeadSetting[Index]))
		{
			continue;
		}
		szAryFeeHeader.Add(szMajorRateFramRateHeadSetting[Index]);
	}

	szFeeFile.Empty();
	/* ���ʱ��ID��(2) */
	szFeeItem.Format("%02X", atoi(szMajorRateFramID12027));
	szFeeFile += szFeeItem;

	/* ����ʱ��  YYMMDDHHMMSS */
	szFeeFile += FEE_FILE_YYMMDDHHMMSS;

	/* ����ʱ��(2) */
	szFeeItem.Format("%02X", atoi(szMajorRateFramStartTime12026));
	szFeeFile += szFeeItem;

	/* �� 4 �ֽ�,û���κ�����; "0000"(4) */
	szFeeFile += _T("0000");

	/* ���ʼ�¼������25(4) */
	szFeeItem.Format("%04X", NUM_RATE_SETTING_TABLE_TOTAL);
	szFeeFile += szFeeItem;

	/* 25���ķ��ʼ�¼(N*22) */
	for (Index=0; Index<NUM_RATE_SETTING_TABLE_TOTAL; Index++)
	{
		if (Index < szAryFeeTab.GetSize())
		{
			memset(szFeeFmt, 0x00, sizeof(szFeeFmt));
			bFeeTan = TransformChargeTable_CommTo3In1(szFeeFmt, szAryFeeTab.GetAt(Index));
			ASSERT(bFeeTan);
			if (!bFeeTan)
			{
				return szVerifyCode;
			}
			szFeeFile += szFeeFmt;
		} 
		else
		{
			szFeeFile += _T("0000000000000000000000");
		}
	}

	/* ����Э��,��� 22 �� "0",û���κ���������� */
	szFeeFile += _T("0000000000000000000000");

	/* ���ʺŶμ�¼����M,(4) m<=127 */
	szFeeItem.Format("%04X", szAryFeeHeader.GetSize());
	szFeeFile += szFeeItem;

	for (Index=0; Index<szAryFeeHeader.GetSize(); Index++)
	{
		memset(szFeeFmt, 0x00, sizeof(szFeeFmt));
		bFeeTan = TransformChargeHeader_CommTo3In1(szFeeFmt, szAryFeeHeader.GetAt(Index));
		ASSERT(bFeeTan);
		if (!bFeeTan)
		{
			return szVerifyCode;
		}
		szFeeFile += szFeeFmt;
	}

	/* �ܰ�У����  */
	memset(szFeeFmt, 0x00, sizeof(szFeeFmt));
	bFeeTan = RateFramRateVerifyCode(szFeeFmt, szFeeFile);
	ASSERT(bFeeTan && (8 == strlen(szFeeFmt)));
	if (!bFeeTan || (8 != strlen(szFeeFmt)))
	{
		return szVerifyCode;
	}

	szVerifyCode = szFeeFmt;

	return szVerifyCode;
}
#endif

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
		strError = _T("�����˳���������У�");
	}
	else if (nExitCode1 == 1)
	{
		strError.Format(_T("���Ѵﵽ������д��� - %d�Σ���������˳���"), dExitCode2);
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
	// д��ע�����
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
		AfxMessageBox(_T("�����ע���벻��Ϊ�գ����������룡"), MB_ICONERROR);
		return FALSE;
	}
	
	strInputedSerial.Remove('-');
	if (strInputedSerial.GetLength()!=20)
	{
		AfxMessageBox(_T("����Ŀͻ��벻��ȷ�����������룡"), MB_ICONERROR);
		return FALSE;
	}
	
	// �õ�ȥ�����ʺŵ�����ִ�
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
			strInfo.Format(_T("����ע�᱾���������ʹ�ô���Ϊ��%d���Σ�"), dwTimes);
			AfxMessageBox(strInfo);
		}
		return TRUE;
	}
	
	AfxMessageBox(_T("�����ע���벻��ȷ�����������룡"), MB_ICONERROR);
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
{/* ��ȫ��ȱȽ� */
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
{/* ��Ҫ�Ƕ�[PARA]�ⲿ�������⻯���� [CLEAR_EMERGENCY_SETTING, INT_OPERATION_NAME_CMD+CLEAR_OPN_CFG]*/
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
	{// Secs ��������
		return FALSE;
	}

	for (indexSecs=0; indexSecs<nSecsSizeOne; indexSecs++)
	{
		arrayIdentOne.RemoveAll();
		arrayIdentTwo.RemoveAll();
        // ��ȡͬһ�������µ���������
		IniFileOne.ReadSection(arraySecsOne.GetAt(indexSecs), arrayIdentOne);
		IniFileTwo.ReadSection(arraySecsOne.GetAt(indexSecs), arrayIdentTwo);

		// ���������������
		nIdentSizeOne = arrayIdentOne.GetSize();
		nIdentSizeTwo = arrayIdentTwo.GetSize();

		if (nIdentSizeOne > nIdentSizeTwo)
		{/* ��arrayIdentOneΪ���ն������Ա� */
			arrayIdentMax.Copy(arrayIdentOne);
			nIdentSizeMax = nIdentSizeOne;
		}
		else
		{
			arrayIdentMax.Copy(arrayIdentTwo);
			nIdentSizeMax = nIdentSizeTwo;
		}

		// ÿһ�������ֵҲҪ���
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



/*  ��������ļ����ܰ�У����  */
BOOL RateFramRateVerifyCode(char *Out_CheckCode, const char *In_HexField)
{
	unsigned short SrcLen = (unsigned short)strlen(In_HexField);
	unsigned char  *ucp   = (unsigned char *)malloc(sizeof(unsigned char)*(SrcLen/2 + 1));
	unsigned short Sum    = 0;
	unsigned short i      = 0;

	if (!IsHexString(In_HexField)||
		(SrcLen == 0)            ||
		(SrcLen%2 == 1))
	{
		free(ucp);
		return FALSE;
	}
	
	if (!HexStringToUcharString(ucp, In_HexField))
	{
		free(ucp);
		return FALSE;
	}
	
	while (i<SrcLen/2){Sum += (unsigned short)ucp[i++];}
	
	for (i=1;i<SrcLen/2;){ucp[0] ^= ucp[i++];}
	
	sprintf(Out_CheckCode, "%04X%02X00", Sum, ucp[0]);
	Out_CheckCode[8] = '\0';
	free(ucp);
	return TRUE;
}