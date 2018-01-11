// BizPhoneSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "AboutDlg.h"

#ifdef USE_SERIAL_CODE
#include "DlgSerial.h"
#endif//USE_SERIAL_CODE
#include "SplashWnd.h"
#include ".\bizphonesettingdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <assert.h>
// Page Header
#include <.\LablePg530ImeiDlg.h>
#include <.\LablePg530CartonDlg.h>
#include <.\LablePhoneAbsolutedPackDlg.h>
/////////////////////////////////////////////////////////////////////////////
// CBizPhoneSettingDlg dialog

CBizPhoneSettingDlg::CBizPhoneSettingDlg(CWnd* pParent /*=NULL*/)
: CStackDialog(IDC_SETTING_SHEET_PLACEHOLDER, CBizPhoneSettingDlg::IDD, pParent)
{	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}                                                                 

void CBizPhoneSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CStackDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBizPhoneSettingDlg)
	DDX_Control(pDX, IDC_PIC_GIF_LOGO, m_ctrGifLogo);
	DDX_Control(pDX, ID_SETTING_SHEET_BTN_EXIT, m_btExit);
	DDX_Control(pDX, IDC_SETTING_CTRL_TREE, m_ctrTree);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CURRENT_STATUS, m_st_status);
	DDX_Control(pDX, IDC_STATIC_TIPS_STATUS, m_st_tips);
	DDX_Control(pDX, ID_SETTING_SHEET_BTN_LOCK, m_btLock);
}

BEGIN_MESSAGE_MAP(CBizPhoneSettingDlg, CStackDialog)
//{{AFX_MSG_MAP(CBizPhoneSettingDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_CREATE()
//}}AFX_MSG_MAP
ON_NOTIFY(TVN_SELCHANGED, IDC_SETTING_CTRL_TREE, OnSelchangedOptionTree)
ON_BN_CLICKED(ID_SETTING_SHEET_BTN_EXIT, OnBnClickedSettingSheetBtnExit)
ON_MESSAGE(WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY,    NotifyShowStatus)
ON_BN_CLICKED(ID_SETTING_SHEET_BTN_LOCK, OnBnClickedSettingSheetBtnLock)
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
#ifndef _VK_RETURN_EXIT_
BOOL CBizPhoneSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message){//键盘消息
		if (VK_RETURN == pMsg->wParam){// 回车键
			return TRUE;
		}else{// if (VK_RETURN == pMsg->wParam){
			return CDialog::PreTranslateMessage(pMsg); 
		}// if (VK_RETURN == pMsg->wParam){}else{}
	}else{// if (WM_KEYDOWN == pMsg->message){
		return CDialog::PreTranslateMessage(pMsg); 
	}// if (WM_KEYDOWN == pMsg->message){}else{}
}
#endif//_VK_RETURN_EXIT_
BOOL CBizPhoneSettingDlg::OnInitDialog()
{
	CStackDialog::OnInitDialog();
	
	// Create and show the splash screen.
	//CSplashWnd::ShowSplashScreen(1000, IDR_JPG_SPLASH, this, FALSE);
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
	
	
	// ----------------------------------------------------------------------
	// 下面开始自定义的初始化过程
	// ----------------------------------------------------------------------
	m_strTypeName[0] = _T("PG530");
	m_strTypeName[1] = _T("Packaging");
	

	m_strSubPg530[0] = _T("Contents & Imei");
	m_strSubPg530[1] = _T("Carton box");
	
	
	// Set the tooltip string of CButtonXP button
	m_btExit.SetToolTipText(IDS_DLG_COMMON_CLOSE);
	
	m_ctrTree.DeleteAllItems();

	// 先增加机种的父亲结点
	for (int i=0; i<INT_NUM_OF_PHONE_TYPE; i++)
		m_hTypeRoot[i]  = m_ctrTree.InsertItem(m_strTypeName[i]);

	// 再增加商务电话的属性结点
	for (i=0; i<INI_NUM_OF_PG530_LABLE; i++)
		m_hSubPg530[i]  = m_ctrTree.InsertItem(m_strSubPg530[i], m_hTypeRoot[0],  TVI_LAST);

	//m_ctrTree.Expand(m_hTypeRoot[0], TVE_EXPAND);

	
	// AddPage并且将该page的指针保留下来
	AddPage(IDD_PG530_IMEI);
	AddPage(IDD_PG530_CARTONBOX);

	AddPage(IDD_PHONE_ABSOLUTED_PACK);
	
	// You can not remove this line of code or there will be errors
	//SetPage(IDD_PHONE_ABSOLUTED_PACK);

	// 由于设置了m_ctrTree的Show Selection Always的属性
	// 如果没有设定一个选择项，系统会自动选定第一个
	m_ctrTree.SelectItem(m_hTypeRoot[1]);

	
	// to show the logo gif picture
	if (m_ctrGifLogo.Load(MAKEINTRESOURCE(IDR_GIF_LOGO),_T("IMAGE")))
		m_ctrGifLogo.Draw();
	
	
	GetDlgItem(IDC_PIC_GIF_LOGO)->ShowWindow(TRUE);

	m_st_status.textColor(RGB(0,0,255));
	m_strSubPg530[1].Format("打印设置[%s, %s]",theApp.m_szLpt, theApp.m_szShare);
	m_st_status.SetWindowText(m_strSubPg530[1]);
	

	m_st_tips.SetWindowText(_T("状态提示："));
	m_st_tips.textColor(RGB(0,0,255));
	m_btExit.SetWindowText(_T("退 出"));
	m_btExit.SetToolTipText(_T("同时保存设置"));

	if (theApp.m_bLockPage){
		m_btLock.SetWindowText(_T("解 锁"));
		m_btLock.SetToolTipText(_T("解锁面页设置"));
	}else{
		m_btLock.SetWindowText(_T("锁 定"));
		m_btLock.SetToolTipText(_T("锁定面页设置"));
	}
	LockPageSet(theApp.m_bLockPage);

#ifdef ME_DEBUG
	SetWindowText(_T("Desay Zaber105SL300 Lable Printer D1.0.5 Debug 版本"));
#else
	SetWindowText(_T("Desay Zaber105SL300 Lable Printer D1.0.5"));
#endif

	/* 禁用标题栏的关闭按钮 */
	CMenu* pMenu = this->GetSystemMenu(FALSE);
	pMenu->ModifyMenu(SC_CLOSE,  MF_BYCOMMAND|MF_GRAYED);
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}
// ==========================================================================
// virtual function which Must be implemented
// Create a dialog object based on dialog resource id
CStackPage *CBizPhoneSettingDlg::CreatePage(UINT nId)
{
	switch (nId) 
	{
	case IDD_PG530_IMEI:
		return new CLablePg530ImeiDlg(this);

	case IDD_PG530_CARTONBOX:
		return new CLablePg530CartonDlg(this);

	case IDD_PHONE_ABSOLUTED_PACK:
		return new CLablePhoneAbsolutedPackDlg(this);
		
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
	if (bActivated){
		switch (nId){
		case IDD_PG530_IMEI:
			break;
		case IDD_PG530_CARTONBOX:
			break;
		case IDD_PHONE_ABSOLUTED_PACK:
			break;
		}		
	}else{
	}
}

void CBizPhoneSettingDlg::OnSelchangedOptionTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	HTREEITEM htiNow = pNMTreeView->itemNew.hItem;
	
	if (htiNow == m_hSubPg530[0]){// PG530 标签名称 IMEI
		SetPage(IDD_PG530_IMEI);
		return;
	}

	if (htiNow == m_hSubPg530[1]){// PG530 标签名称 卡通箱子
		SetPage(IDD_PG530_CARTONBOX);
		return;
	}

	if (htiNow == m_hTypeRoot[1]){// 装箱
		SetPage(IDD_PHONE_ABSOLUTED_PACK);
		return;
	}
	
	*pResult = 0;
}

void CBizPhoneSettingDlg::OnOK() 
{
	extern BOOL bThereIsThreadRunning;
	if (bThereIsThreadRunning){
		MessageBox(_T("后台线程正在工作，不能够退出!"), _T("Zaber Printer"), MB_OK|MB_ICONSTOP);
		return;
	}
	CStackDialog::OnOK();
}

void CBizPhoneSettingDlg::OnCancel() 
{
	extern BOOL bThereIsThreadRunning;
	if (bThereIsThreadRunning){
		MessageBox(_T("后台线程正在工作，不能够退出!"), _T("Zaber Printer"), MB_OK|MB_ICONSTOP);
		return;
	}
	CStackDialog::OnCancel();
}

void CBizPhoneSettingDlg::OnClose() 
{
	extern BOOL bThereIsThreadRunning;
	if (bThereIsThreadRunning){
		MessageBox(_T("后台线程正在工作，不能够退出!"), _T("Zaber Printer"), MB_OK|MB_ICONSTOP);
		return;
	}
	CStackDialog::OnClose();
}



BOOL CBizPhoneSettingDlg::DestroyWindow() 
{
	return CStackDialog::DestroyWindow();
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


int CBizPhoneSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStackDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
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
	MessageBox(strError);

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
		MessageBox(_T("输入的注册码不得为空！请重新输入！"));
		return FALSE;
	}
	
	strInputedSerial.Remove('-');
	if (strInputedSerial.GetLength()!=20)
	{
		MessageBox(_T("输入的客户码不正确！请重新输入！"));
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
			MessageBox(strInfo);
		}
		return TRUE;
	}


	MessageBox(_T("输入的注册码不正确！请重新输入！"));

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

#endif
void CBizPhoneSettingDlg::OnBnClickedSettingSheetBtnExit()
{
	// TODO: Add your control notification handler code here
	extern BOOL bThereIsThreadRunning;
	if (bThereIsThreadRunning){
		MessageBox(_T("后台线程正在工作，不能够退出!"), _T("Zaber Printer"), MB_OK|MB_ICONSTOP);
		return;
	}

	// 保存设置
	SavePageSet();

	CStackDialog::OnOK();
}
//*******************************************************************************//

void CBizPhoneSettingDlg::SavePageSet(void)
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
		pStackPage->OnSaveData();		
	}
	return;
}

void CBizPhoneSettingDlg::LockPageSet(BOOL bLock)
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
		pStackPage->OnLockPage(bLock);		
	}
	return;
}

LRESULT CBizPhoneSettingDlg::NotifyShowStatus(WPARAM bNoError,LPARAM lpShowText)
{
	CString szVal = _T("");

	szVal.Format("打印设置[%s, %s]  -- %s",theApp.m_szLpt, theApp.m_szShare, theApp.m_szStatus);

	m_st_status.SetWindowText(szVal);

	if (!bNoError){

		if (IDOK == MessageBox(theApp.m_szStatus, _T("Zaber Printer"), MB_OK|MB_ICONSTOP)){

			szVal.Format("打印设置[%s, %s]",theApp.m_szLpt, theApp.m_szShare);

			m_st_status.SetWindowText(szVal);
		}
	}

	return S_OK;
}

void CBizPhoneSettingDlg::OnBnClickedSettingSheetBtnLock()
{
	// TODO: Add your control notification handler code here
	theApp.m_bLockPage = !theApp.m_bLockPage;
    LockPageSet(theApp.m_bLockPage);
	if (theApp.m_bLockPage){
		m_btLock.SetWindowText(_T("解 锁"));
		m_btLock.SetToolTipText(_T("解锁面页设置"));
	}else{
		m_btLock.SetWindowText(_T("锁 定"));
		m_btLock.SetToolTipText(_T("锁定面页设置"));
	}
}

//*******************************************************************************//


BOOL IsNumberChar(const char Src_Ch)
{
	if ((Src_Ch > '9')||(Src_Ch < '0'))
		return FALSE;
	return TRUE;
}

BOOL IsHexChar(const char Src_Ch)
{
	if (!((Src_Ch >= '0')&&(Src_Ch <= '9'))
		&& !((Src_Ch >= 'a')&&(Src_Ch <= 'f'))
		&& !((Src_Ch >= 'A')&&(Src_Ch <= 'F')))
		return FALSE;
	return TRUE;
}

char GetHexChar(int cVal)
{
	switch(abs(cVal)%16){
		case 0x00:return 0x30;
		case 0x01:return 0x31;
		case 0x02:return 0x32;
		case 0x03:return 0x33;
		case 0x04:return 0x34;
		case 0x05:return 0x35;
		case 0x06:return 0x36;
		case 0x07:return 0x37;
		case 0x08:return 0x38;
		case 0x09:return 0x39;
		case 0x0A:return 0x41;
		case 0x0B:return 0x42;
		case 0x0C:return 0x43;
		case 0x0D:return 0x44;
		case 0x0E:return 0x45;
		case 0x0F:return 0x46;
	}
	return 0x30;
}

BOOL IsAlphaChar(const char Src_Ch)
{
	if (((Src_Ch >= 'A')&&(Src_Ch <= 'Z'))||
		((Src_Ch >= 'a')&&(Src_Ch <= 'z')))
		return TRUE;
	return FALSE;
}

BOOL IsAlnumChar(const char Src_Ch)
{
	if (IsAlphaChar(Src_Ch) || IsNumberChar(Src_Ch))
		return TRUE;
	return FALSE;
}

BOOL IsEmptyOrAlnumString(const char *pSrc)
{
	for (int i=0; i<(int)strlen(pSrc); i++)
		if (!(isprint(pSrc[i])))
			return FALSE;

	return TRUE;
}

BOOL IsNumberString(CString &Src_string)  // 0 - 9
{
	if(Src_string.IsEmpty()) 
		return FALSE;
	else
	{
		int nLength = Src_string.GetLength();
		char ch;
		for(int i=0; i<nLength; i++)
		{
			ch = Src_string.GetAt(i);
			if ((ch > '9')||(ch < '0'))	return FALSE;
		}
	}
	return TRUE;
}


BOOL IsHexString(CString &Src_string)
{
	if(Src_string.IsEmpty()) 
		return FALSE;
	else
	{
		int nLength = Src_string.GetLength();
		char ch;
		for(int i=0; i<nLength; i++)
		{
			ch = Src_string.GetAt(i);
			if (   !((ch >= '0')&&(ch <= '9'))
				 && !((ch >= 'a')&&(ch <= 'f'))
				 && !((ch >= 'A')&&(ch <= 'F'))
				)
				return FALSE;
		}
	}
	return TRUE;
}

BOOL IsAlnumString(CString &Src_string)
{
	int i, len = 0;
	if (Src_string.IsEmpty()){
		return FALSE;
	}

	len = Src_string.GetLength();
	for (i=0; i<len; i++)
	{
		if (!IsAlnumChar(Src_string.GetAt(i))){
			return FALSE;
		}
	}

	return TRUE;
	
}

BOOL IsPrintString(CString &Src_string)
{
	int i, len = 0;
	if (Src_string.IsEmpty()){
		return FALSE;
	}

	len = Src_string.GetLength();
	for (i=0; i<len; i++)
	{
		if (!isprint(Src_string.GetAt(i))){
			return FALSE;
		}
	}
	return TRUE;
}

BOOL IsEmptyOrNumberString(CString &Src_string)
{
	if (Src_string.IsEmpty()){

		return TRUE;

	}

	return  IsNumberString(Src_string);
}

BOOL IsEmptyOrHexString(CString &Src_string)
{
	if (Src_string.IsEmpty()){

		return TRUE;

	}

	return IsHexString(Src_string);
}

BOOL IsPhoneString(CString &Src_string)
{
	if(Src_string.IsEmpty()) 
		return FALSE;
	else
	{
		int nLength = Src_string.GetLength();
		char ch;
		for(int i=0; i<nLength; i++)
		{
			ch = Src_string.GetAt(i);
			if (   !((ch >= '0')&&(ch <= '9'))
				 && (ch != '*')
				 && (ch != '#')   )
				return FALSE;
		}
	}
	return TRUE;
}


BOOL IsEmptyOrPhoneString(CString &Src_string)
{
	if (Src_string.IsEmpty()){

		return TRUE;

	}

	return IsPhoneString(Src_string);

}

void NumStrAutoAdd(char *IO, short increment)
{
	int LEN = (int)strlen(IO);
	assert(LEN > 0);
	assert(LEN < 256);
	char  ZERO = '0';
	short Array[256];
	short Carry = 0;
	strrev(IO);
	for (int i=0; i<LEN; i++)
		Array[i] = IO[i] - ZERO;
	Array[0] = Array[0] + increment;
	Carry    = Array[0]/10;
	Array[0] = Array[0]%10;
	for (i=1; i<LEN; i++){
		Array[i] += Carry;
		Carry    = Array[i]/10;
		Array[i] = Array[i]%10;
	}
	for (i=0; i<LEN; i++)
		IO[i] = ZERO + Array[i];
	strrev(IO);
	return;
}

BOOL GetPostfixString(const char *Src_String, CString &szPostfix, int cTagChar, int bNoSpace)
{
	int nPos = 0;

	CString szSrc(Src_String);

	if (bNoSpace){
		szSrc.Remove(0x20);
	}

	szPostfix.Empty();

	nPos = szSrc.Find(cTagChar);

	if (-1 == nPos){
		return FALSE;
	}

	szPostfix = szSrc.Mid(nPos + 1);

	return TRUE;
}

// 把格式为：Str1-Str2-Str3-Str4-...-Strn 的字符串，存放进数组，返回数组宽度
int PutStringIntoCStringArray(IN const char *szFormatString, OUT CStringArray &szAnsIdArray, IN int cTagChar)
{
	CString szItem = _T("");
	CString szSrc  = szFormatString;
	int nPos = 0;

	szAnsIdArray.RemoveAll();

	nPos  = szSrc.Find(cTagChar);

	if ((-1 == nPos) && (szSrc.GetLength() == 0)){
		return 0;
	}//

	// 去除开头的分隔符号
	while(*szFormatString == cTagChar) szFormatString++;

	szSrc  = szFormatString;

	while(szSrc.GetLength() > 0){
		nPos   = szSrc.Find(cTagChar);
		if (0 == nPos){
			szSrc = szSrc.Right(szSrc.GetLength() - 1);
			continue;
		}
		if(-1 == nPos){
			szAnsIdArray.Add(szSrc);
			break;
		}
		szItem = szSrc.Left(nPos);
		szSrc  = szSrc.Mid(nPos + 1);
		if (!szItem.IsEmpty()){szAnsIdArray.Add(szItem);}
	}//while(szSrc.GetLength() > 0){}

	return szAnsIdArray.GetSize();
}

void PrintfCStringArrayToString(CString &szString, CStringArray& szPrintfStringArray, IN int cTagChar)
{
	szString.Empty();

	int nSize = 0;

	for (nSize=0; nSize<szPrintfStringArray.GetSize(); nSize++)
	{
		szString += szPrintfStringArray.GetAt(nSize);
		szString += (char)cTagChar;
	}

	if (szString.GetLength() > 0){
		szString = szString.Left(szString.GetLength() - 1);
	}

	return;
}

int FillStringIntoComboBox(CComboBox *pCComboBox, CStringArray &szStringArray)
{
	ASSERT(pCComboBox);

	int nIndex = 0;

	while (pCComboBox->GetCount()){
		pCComboBox->DeleteString(0);
	}

	for (nIndex=0; nIndex<szStringArray.GetSize(); nIndex++)
		pCComboBox->AddString(szStringArray.GetAt(nIndex));

	return pCComboBox->GetCount();
}

int GetComboBoxItemToStringArray(CStringArray &szStringArray, CComboBox *pComboBox)
{
	szStringArray.RemoveAll();

	int nIndex = 0;

	CString  szItem = _T("");

	for (nIndex=0; nIndex<pComboBox->GetCount(); nIndex++){

		pComboBox->GetLBText(nIndex, szItem);

		szStringArray.Add(szItem);	
	}

	return szStringArray.GetSize();
}

// 清空下拉列表
int ClearComboBoxItem(CComboBox *pComboBox)
{
	int iItem = 0;
	while (pComboBox->GetCount()){
		pComboBox->DeleteString(0);
		iItem++;
	}
	return iItem;
}
