// PropPageData.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "PropPageData.h"
#include ".\userveritydlg.h"
#include "UserVerify.h"
#include ".\VfManagerDlg.h"
#include "InputPassDlg2.h"
#include "IniFile.h"
#include <io.h>
#include ".\proppagedata.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STR_INPUT_TIPS   _T("Please import a para file!")
#define FILE_EXT_LOAD    _T("load")
#define FILE_EXT_SAVE    _T("save")
short  AfterOpenComm_afterActivateCommandTye = 0;

/////////////////////////////////////////////////////////////////////////////
// CPropPageData dialog

CPropPageData::CPropPageData(CWnd* pParent /*=NULL*/)
: CStackPage(CPropPageData::IDD, pParent)
, m_szIniInput(STR_INPUT_TIPS)
{
	//{{AFX_DATA_INIT(CPropPageData)
	m_strOldPwd = _T("");
	m_strNewPwd = _T("");
	m_strCnfPwd = _T("");
	if (theApp.m_bEnPassw == _T("FALSE")){
		m_bUseDefaultPwd = FALSE;
	}else{
		m_bUseDefaultPwd = TRUE;
	}	
	m_strDefaultPwd = theApp.m_szPassw;
	//}}AFX_DATA_INIT
}


void CPropPageData::DoDataExchange(CDataExchange* pDX)
{
	CStackPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageData)
	DDX_Control(pDX, IDC_PAGE_B_BTN_VIEW, m_btManager);
	//DDX_Control(pDX, IDC_PAGE_B_BTN_PWDCHG, m_btChangePwd);
	//DDX_Control(pDX, IDC_PAGE_B_BTN_PWDCHGANDSAVE, m_btChgPwdAndSave);
	DDX_Control(pDX, IDC_PAGE_B_BTN_PRODUCE, m_btProduce);
	DDX_Control(pDX, IDC_PAGE_B_BTN_LOADINI, m_btLoadIni);
	DDX_Control(pDX, IDC_PAGE_B_BTN_SAVEINI, m_btSaveIni);
	DDX_Control(pDX, IDC_PAGE_B_BTN_LOADPHONE, m_btLoadPhone);
	//DDX_Control(pDX, IDC_PAGE_B_BTN_SAVEPHONE, m_btSavePhone);
	DDX_Control(pDX, IDC_PAGE_B_BTN_PHONE2DEFAULT, m_btPhone2Default);
	DDX_Control(pDX, IDC_PAGE_B_BTN_DEFAULT2PHONE, m_btDefault2Phone);
	DDX_Text(pDX, IDC_PAGE_B_EDIT_OLDPWD, m_strOldPwd);
	DDV_MaxChars(pDX, m_strOldPwd, 8);
	DDX_Text(pDX, IDC_PAGE_B_EDIT_NEWPWD, m_strNewPwd);
	DDV_MaxChars(pDX, m_strNewPwd, 8);
	DDX_Text(pDX, IDC_PAGE_B_EDIT_CNFPWD, m_strCnfPwd);
	DDV_MaxChars(pDX, m_strCnfPwd, 8);
	DDX_Check(pDX, IDC_PAGE_B_CHECK_DEFAULTPWD, m_bUseDefaultPwd);
	DDX_Text(pDX, IDC_PAGE_B_EDIT_DEFAULTPWD, m_strDefaultPwd);
	DDV_MaxChars(pDX, m_strDefaultPwd, 8);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_ST_INI_INPUT, m_szIniInput);
	DDX_Control(pDX, IDC_ST_INI_INPUT, m_cStIniInput);
	DDX_Control(pDX, IDC_ST_STATE, m_stState);
}


BEGIN_MESSAGE_MAP(CPropPageData, CStackPage)
//{{AFX_MSG_MAP(CPropPageData)
ON_BN_CLICKED(IDC_PAGE_B_CHECK_DEFAULTPWD, OnPageCheckDefaultPwd)
//ON_BN_CLICKED(IDC_PAGE_B_BTN_PWDCHG, OnPageChangePwd)
//ON_BN_CLICKED(IDC_PAGE_B_BTN_PWDCHGANDSAVE, OnPageChangeAndSave)
ON_BN_CLICKED(IDC_PAGE_B_BTN_PRODUCE, OnPageProduce)
ON_BN_CLICKED(IDC_PAGE_B_BTN_LOADINI, OnPageLoadFromIni)
ON_BN_CLICKED(IDC_PAGE_B_BTN_SAVEINI, OnPageSaveToIni)
ON_BN_CLICKED(IDC_PAGE_B_BTN_LOADPHONE, OnPageLoadModuleSetting)
//ON_BN_CLICKED(IDC_PAGE_B_BTN_SAVEPHONE, OnPageSaveModuleSetting)
ON_BN_CLICKED(IDC_PAGE_B_BTN_SAVEDEFAULT, OnPageSaveDefault)
ON_BN_CLICKED(IDC_PAGE_B_BTN_RECOVERDEFAULT, OnPageRecoverDefault)
ON_BN_CLICKED(IDC_PAGE_B_BTN_VIEW, OnUserVerify)
ON_EN_CHANGE(IDC_PAGE_B_EDIT_DEFAULTPWD, OnEnChangePageBEditDefaultpwd)
//}}AFX_MSG_MAP
ON_MESSAGE(WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY, OnEnableOperationButtom)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropPageData message handlers

BOOL CPropPageData::OnInitDialog() 
{
	CStackPage::OnInitDialog();
	m_pParentWnd = (CBizPhoneSettingDlg*)GetParent();

	if (m_bUseDefaultPwd){
		GetDlgItem(IDC_PAGE_B_EDIT_DEFAULTPWD)->EnableWindow(TRUE);
	}else{
		GetDlgItem(IDC_PAGE_B_EDIT_DEFAULTPWD)->EnableWindow(FALSE);
	}	
	m_pParentWnd->m_bChangePwdAndSaveSetting = FALSE;
	
	// Set the tooltip string of CButtonXP button
	//m_btChangePwd.SetToolTipText(IDS_PAGE_CHANGE_PWD);
	//m_btChgPwdAndSave.SetToolTipText(IDS_PAGE_CHANGE_PWD_AND_DOWNLOAD);
	m_btProduce.SetToolTipText(IDS_PAGE_CHANGE_PWD_DOWN_DEFAULT);
	
	m_btLoadIni.SetToolTipText(IDS_PAGE_IMPORT_ALLDATA);
	m_btSaveIni.SetToolTipText(IDS_PAGE_EXPORT_ALLDATA);
	
	m_btLoadPhone.SetToolTipText(IDS_PAGE_LOAD_SETTING);
	//m_btSavePhone.SetToolTipText(IDS_PAGE_SAVE_SETTING);
	m_btPhone2Default.SetToolTipText(IDS_PAGE_SAVE_DEFAULT);
	m_btDefault2Phone.SetToolTipText(IDS_PAGE_RECORVER_DEFAULT);

	//m_btChangePwd.ShowWindow(theApp.m_bIsAdmin);
	//m_btChgPwdAndSave.ShowWindow(theApp.m_bIsAdmin);
	m_btPhone2Default.ShowWindow(theApp.m_bIsAdmin);
	m_btDefault2Phone.ShowWindow(theApp.m_bIsAdmin);
	//m_btSavePhone.ShowWindow(theApp.m_bIsAdmin);

	m_stState.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PAGE_B_BTN_SAVEDEFAULT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PAGE_B_BTN_RECOVERDEFAULT)->ShowWindow(SW_HIDE);

#if defined(FOR_CLIENT)
    GetDlgItem(IDC_PAGE_B_BTN_VIEW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_STATE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_PARA_FILE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ST_INI_INPUT)->ShowWindow(SW_HIDE);
#else
	if (theApp.m_bIsAdmin)
	{
		GetDlgItem(IDC_PAGE_B_BTN_VIEW)->SetWindowText("User Manager");
		m_btManager.SetToolTipText(IDS_PAGE_USER_MANAG);
	}
	else
	{
		GetDlgItem(IDC_PAGE_B_BTN_VIEW)->SetWindowText("UnLock");
		m_btManager.SetToolTipText(IDS_PAGE_LOCK_OR_NOT);
	}
#endif
	return TRUE;
}

LRESULT CPropPageData::OnEnableOperationButtom(WPARAM wParam, LPARAM lParam)
{
	BOOL bEnable = (BOOL)wParam;
	
	///#ifndef DISABLE_INPUT
	//m_btChangePwd.EnableWindow(bEnable);
	//m_btChgPwdAndSave.EnableWindow(bEnable);
	m_btProduce.EnableWindow(bEnable);
	
	m_btSaveIni.EnableWindow(bEnable && theApp.m_bEnablePage);
	m_btDefault2Phone.EnableWindow(bEnable);
	//#endif

	if (theApp.m_bIsAdmin)
	{
		m_btLoadPhone.EnableWindow(bEnable);
		m_btProduce.EnableWindow(bEnable);
	}
	else
	{
		m_btLoadPhone.EnableWindow((theApp.m_bUseMode == UM_LOAD) && bEnable);
		m_btProduce.EnableWindow((theApp.m_bUseMode == UM_SAVE) && bEnable);
	}
	
	m_btLoadIni.EnableWindow(bEnable && theApp.m_bEnablePage);
	
	//m_btLoadPhone.EnableWindow(bEnable);
	//m_btSavePhone.EnableWindow(bEnable);
	m_btPhone2Default.EnableWindow(bEnable);

	m_btManager.EnableWindow(bEnable);


	GetDlgItem(IDC_PAGE_B_EDIT_OLDPWD)->EnableWindow(bEnable);
	GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD)->EnableWindow(bEnable);
	GetDlgItem(IDC_PAGE_B_EDIT_CNFPWD)->EnableWindow(bEnable);
	GetDlgItem(IDC_PAGE_B_EDIT_DEFAULTPWD)->EnableWindow(bEnable);
	GetDlgItem(IDC_PAGE_B_CHECK_DEFAULTPWD)->EnableWindow(bEnable);

	/* 返回一个消息给回主窗口,禁用激活树控件  */
	::PostMessage(m_pParentWnd->m_hWnd, WM_MSG_MAIN_WND_TREE_CONTROL_NOTIFY, wParam, lParam);
	
	//	Invalidate();
	return 0;
}

void CPropPageData::OnPageLoadFromIni() 
{
	// load option data from our predifined data file
	// 打开的路径为本程序所在的路径
	char name[256];
	GetModuleFileName(NULL, name, 256);	
	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);

#if defined(FOR_CLIENT)
	// 导入脚本数据配置文件
	CFileDialog fDlg(TRUE,
              		_T("ini"),
					NULL,
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
					_T("Para Script File(*.ini)|*.ini|All File (*.*)|*.*||"),
					this);
#else
	CString szExtFile;
	
	if (UM_SAVE == theApp.m_bUseMode)
	{
		szExtFile.Format(_T("Para Script File(*.%s)|*.%s||"), FILE_EXT_SAVE, FILE_EXT_SAVE);
	}
	else if (UM_LOAD == theApp.m_bUseMode)
	{
		szExtFile.Format(_T("Para Script File(*.%s)|*.%s||"), FILE_EXT_LOAD, FILE_EXT_LOAD);
	}
	else
	{
		MessageBox(_T("Unkown usemode"), SYS_NAME, MB_ICONSTOP);
		ASSERT(0);
	}

	CFileDialog fDlg(TRUE,
                     _T("ini"),
					 NULL,
					 OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
					 szExtFile,
					 this);
#endif
	
	fDlg.m_ofn.lpstrInitialDir = path;
	
	// 改变标题：Open - means 导入脚本文件
	fDlg.m_ofn.lpstrTitle = _T("Import Para Script File:");
	
	CString strFileName, sError;
	if ( IDOK == fDlg.DoModal() )
	{
		// 依据得到的路径和文件名
		strFileName = fDlg.GetPathName();
		
		// 此处不再验证文件的有效性
		InfoSystemLoadFile(strFileName);
		
		// Disable all the buttom now
		OnEnableOperationButtom(0,0);
	}
}

void CPropPageData::OnPageSaveToIni() 
{
	// Save option data to our predifined data file
	
	// 导出脚本文件
	CFileDialog fDlg(	FALSE,
		_T("ini"),
		_T("config.ini"),
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Para Script File(*.ini)|*.ini|All File (*.*)|*.*||"),
		this
		);
	
	// 打开的路径为本程序所在的路径
	char name[256];
	GetModuleFileName(NULL, name, 256);
	
	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	fDlg.m_ofn.lpstrInitialDir = path;
	
	// 改变标题：Save - means 导出脚本文件
	fDlg.m_ofn.lpstrTitle = _T("Export Para Script File:");
	
	CString strFileName, sError;
	if (IDOK==fDlg.DoModal())
	{
		// 依据得到的路径和文件名
		strFileName = fDlg.GetPathName();
		
		// Disable all the buttom now
		OnEnableOperationButtom(0,0);
		
		// 此处不再验证文件的有效性
		InfoSystemSaveFile(strFileName);
	}
}

void CPropPageData::InfoSystemLoadFile(CString strLoadFileName)
{
	// post App dialog a message to force it refresh all the pages
	ASSERT(!strLoadFileName.IsEmpty());
	
	m_pParentWnd->m_strExchangeFileName = strLoadFileName;
	m_pParentWnd->PostMessage(WM_MSG_LOAD_USER_INI_NOTIFY);
}

void CPropPageData::InfoSystemSaveFile(CString strSaveFileName)
{
	// post App dialog a message to force it refresh all the pages
	ASSERT(!strSaveFileName.IsEmpty());
	
	m_pParentWnd->m_strExchangeFileName = strSaveFileName;
	m_pParentWnd->PostMessage(WM_MSG_SAVE_USER_INI_NOTIFY);
}

void CPropPageData::OnPageLoadModuleSetting() 
{
	//Mask for activate the comm port  huang ye 20060808
#if !defined(FOR_CLIENT)

	CString szMsg, szExtName;

	OnShowPage(CIS_NULL);

	theApp.m_szIniInput.Empty();

	m_eUseMode = UM_LOAD;

	if (m_szIniInput.IsEmpty() || (STR_INPUT_TIPS == m_szIniInput))
	{
		MessageBox(_T("Please import a para file,first!"), SYS_NAME, MB_OK|MB_ICONSTOP);
		return;
	}

	szExtName = m_szIniInput;
	szExtName = szExtName.Mid(szExtName.Find('.') + 1);
	szExtName.MakeLower();
	switch (m_eUseMode)
	{
	case UM_SAVE:
		if (szExtName != FILE_EXT_SAVE)
		{
			szMsg.Format("%s is invalid file for save!", m_szIniInput);
			MessageBox(szMsg, SYS_NAME, MB_ICONSTOP);
			return;
		}
		break;
	case UM_LOAD:
		if (szExtName != FILE_EXT_LOAD)
		{
			szMsg.Format("%s is invalid file for load!", m_szIniInput);
			MessageBox(szMsg, SYS_NAME, MB_ICONSTOP);
			return;
		}
		break;
	default:
		ASSERT(0);
		MessageBox(_T("FileExt no match"), SYS_NAME, MB_ICONSTOP);
		return;
		break;
	}

	// 检查该文件是否可读
	if (0 != _access(m_szIniInput, 0x04))
	{
		szMsg.Format(_T("Can't access this file[%s]!"), m_szIniInput);
		MessageBox(szMsg, _T("Error"), MB_OK|MB_ICONSTOP);
		m_szIniInput.Empty();
		theApp.m_szIniInput = m_szIniInput;
		OnRefreshPage();
		return;
	}
	// 赋值给全局变量
	theApp.m_szIniInput = m_szIniInput;
	OnRefreshPage();
#endif
	/*
	// added by Huang ye for activate commport
	m_pParentWnd->PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
	MainSerial.afterActivateCommandTye = MainSerial.READDATA_CMD_DATAPAGE;
	// added by Huang ye for activate commport
	*/
	//发送消息去打开串口
	m_pParentWnd->PostMessage(WM_OPEN_COMM_CONNECTION_NOTIFY);
	
	extern short AfterOpenComm_afterActivateCommandTye;
	AfterOpenComm_afterActivateCommandTye = AfterOpenComm_ReadPara;
	
	// Disable all the buttom now
	OnEnableOperationButtom(0,0);
}

void CPropPageData::OnPageSaveModuleSetting() 
{
	// enable/disable the edits
	UpdateData(TRUE);
	if (m_bUseDefaultPwd)
	{
		// transfer the pwd to MainDlg
		// MainContent had been defined to be a global variable
		MainContent.m_sOldPass = m_strDefaultPwd;
	}
	else
	{
		MainContent.m_sOldPass = "";
	}
	
	/*
	// added by Huang ye for activate commport
	m_pParentWnd->PostMessage(WM_MSG_ACTIVATE_COMMPORT_NOTIFY);
	
	  MainSerial.afterActivateCommandTye = MainSerial.WRITEDATA_CMD_DATAPAGE;
	  // added by Huang ye for activate commport
	*/
	
    //发送消息去打开串口
	m_pParentWnd->PostMessage(WM_OPEN_COMM_CONNECTION_NOTIFY);
	
	extern short AfterOpenComm_afterActivateCommandTye;
	AfterOpenComm_afterActivateCommandTye = AfterOpenComm_WritePara;
	
	OnEnableOperationButtom(0,0);
}

void CPropPageData::OnPageSaveDefault() 
{
	// enable/disable the edits
	UpdateData(TRUE);
	if (m_bUseDefaultPwd)
	{
		// transfer the pwd to MainDlg
		// MainContent had been defined to be a global variable
		MainContent.m_sOldPass = m_strDefaultPwd;
	}
	else
	{
		MainContent.m_sOldPass = "";
	}
	//m_pParentWnd->PostMessage(WM_MSG_SAVE_MODULE_DEFAULT_NOTIFY);
	
	//发送消息去打开串口
	m_pParentWnd->PostMessage(WM_OPEN_COMM_CONNECTION_NOTIFY);
	
	extern short AfterOpenComm_afterActivateCommandTye;
	AfterOpenComm_afterActivateCommandTye = AfterOpenComm_SaveDefault;
	
	// Disable all the buttom now
	OnEnableOperationButtom(0,0);
}

void CPropPageData::OnPageRecoverDefault() 
{
	// enable/disable the edits
	UpdateData(TRUE);
	if (m_bUseDefaultPwd)
	{
		// transfer the pwd to MainDlg
		// MainContent had been defined to be a global variable
		MainContent.m_sOldPass = m_strDefaultPwd;
	}
	else
	{
		MainContent.m_sOldPass = "";
	}
	//m_pParentWnd->PostMessage(WM_MSG_RECOVER_MODULE_DEFAULT_NOTIFY);
	//发送消息去打开串口
	m_pParentWnd->PostMessage(WM_OPEN_COMM_CONNECTION_NOTIFY);
	
	extern short AfterOpenComm_afterActivateCommandTye;
	AfterOpenComm_afterActivateCommandTye = AfterOpenComm_RecoverDefault;
	
	// Disable all the buttom now
	OnEnableOperationButtom(0,0);
}

void CPropPageData::OnPageCheckDefaultPwd() 
{
	// enable/disable the edits
	UpdateData(TRUE);
	GetDlgItem(IDC_PAGE_B_EDIT_DEFAULTPWD)->EnableWindow(m_bUseDefaultPwd);
	theApp.m_bEnPassw = m_bUseDefaultPwd ? _T("TRUE"): _T("FALSE");
	
	// 清空之
	MainContent.m_sOldPass = "";
}

BOOL CPropPageData::ValidatePassword(void) 
{
	// Get Input
	UpdateData(TRUE);
	
	if (m_strNewPwd=="")
	{
		MessageBox(_T("The new password could't be empty, input something, please!"),SYS_NAME,MB_ICONSTOP);
		((CEdit*)GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD)->SetFocus();
		return FALSE;
	}
	
	if (m_strNewPwd==m_strOldPwd)
	{
		MessageBox(_T("It is not allowed than the new one is as same as the old one!"),SYS_NAME,MB_ICONSTOP);
		((CEdit*)GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD)->SetFocus();
		return FALSE;
	}
	
	if (m_strNewPwd!=m_strCnfPwd)
	{
		MessageBox(_T("The confirm must match the new one!"),SYS_NAME,MB_ICONSTOP);
		((CEdit*)GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD))->SetSel(0,-1);
		GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD)->SetFocus();
		return FALSE;
	}
	
	MainContent.m_sOldPass = m_strOldPwd;
	MainContent.m_sNewPass1 = m_strNewPwd;
	MainContent.m_sNewPass2 = m_strCnfPwd;
	
	/*
	m_strOldPwd = "";
	m_strNewPwd = "";
	m_strCnfPwd = "";
	*/
	
	// empty three edit ctrl now
	// UpdateData(FALSE);
	return TRUE;
}

BOOL CPropPageData::isAllPasswordEmpty(void)
{
	UpdateData(TRUE);
	BOOL bRet =  m_strNewPwd.IsEmpty() && m_strOldPwd.IsEmpty() && m_strCnfPwd.IsEmpty();
	m_pParentWnd->m_bIsAllPasswordEmpty = bRet;
	return bRet;
}
/*
void CPropPageData::OnPageChangePwd() 
{
	if (ValidatePassword())
	{
		//m_pParentWnd->PostMessage(WM_MSG_CHANGE_PWD_NOTIFY);
		
		//发送消息去打开串口
		m_pParentWnd->PostMessage(WM_OPEN_COMM_CONNECTION_NOTIFY);
        
		extern short AfterOpenComm_afterActivateCommandTye;
		AfterOpenComm_afterActivateCommandTye = AfterOpenComm_ChangpassW;
		
		// Disable all the buttom now
		OnEnableOperationButtom(0,0);
	}
}


void CPropPageData::OnPageChangeAndSave() 
{
	// first we call this procedure to make pwd changed
	if (ValidatePassword())
	{
		m_pParentWnd->m_bChangePwdAndSaveSetting = TRUE;
		
		//m_pParentWnd->PostMessage(WM_MSG_CHANGE_PWD_NOTIFY);
		
		//发送消息去打开串口
		m_pParentWnd->PostMessage(WM_OPEN_COMM_CONNECTION_NOTIFY);
        
		extern short AfterOpenComm_afterActivateCommandTye;
		AfterOpenComm_afterActivateCommandTye = AfterOpenComm_ChangPwLoad;
		
		// Disable all the buttom now
		OnEnableOperationButtom(0,0);
	}
}
*/
void CPropPageData::OnPageProduce() 
{
	// first we call this procedure to make pwd changed
#if !defined(FOR_CLIENT)
	CString szMsg, szExtName;

	theApp.m_szIniInput.Empty();

	m_eUseMode = UM_SAVE;

	if (m_szIniInput.IsEmpty() || (STR_INPUT_TIPS == m_szIniInput))
	{
		MessageBox(_T("Please import a para file,first!"), _T("Error"), MB_OK|MB_ICONSTOP);
		return;
	}

	szExtName = m_szIniInput;
	szExtName = szExtName.Mid(szExtName.Find('.') + 1);
	szExtName.MakeLower();
	switch (m_eUseMode)
	{
	case UM_SAVE:
		if (szExtName != FILE_EXT_SAVE)
		{
			szMsg.Format("%s is invalid file for save!", m_szIniInput);
			MessageBox(szMsg, SYS_NAME, MB_ICONSTOP);
			return;
		}
		break;
	case UM_LOAD:
		if (szExtName != FILE_EXT_LOAD)
		{
			szMsg.Format("%s is invalid file for load!", m_szIniInput);
			MessageBox(szMsg, SYS_NAME, MB_ICONSTOP);
			return;
		}
		break;
	default:
		ASSERT(0);
		MessageBox(_T("FileExt no match"), SYS_NAME, MB_ICONSTOP);
		return;
		break;
	}

	// 检查该文件是否可读
	if (0 != _access(m_szIniInput, 0x04))
	{
		szMsg.Format(_T("Can't access this file[%s]!"), m_szIniInput);
		MessageBox(szMsg, _T("Error"), MB_OK|MB_ICONSTOP);
		m_szIniInput.Empty();
		theApp.m_szIniInput = m_szIniInput;
		OnRefreshPage();
		return;
	}
	// 赋值给全局变量
	theApp.m_szIniInput = m_szIniInput;
	OnRefreshPage();
#endif
	if (isAllPasswordEmpty())
	{
		m_pParentWnd->m_bChangePwdAndSaveSetting = TRUE;
		m_pParentWnd->m_bChangePwdAndSaveSettingAndWriteDefault = TRUE;
		OnPageSaveModuleSetting();
		return;
	}

	if (ValidatePassword())
	{
		m_pParentWnd->m_bChangePwdAndSaveSetting = TRUE;
		m_pParentWnd->m_bChangePwdAndSaveSettingAndWriteDefault = TRUE;
		//m_pParentWnd->PostMessage(WM_MSG_CHANGE_PWD_NOTIFY);
		
		//发送消息去打开串口
		m_pParentWnd->PostMessage(WM_OPEN_COMM_CONNECTION_NOTIFY);
        
		extern short AfterOpenComm_afterActivateCommandTye;
		AfterOpenComm_afterActivateCommandTye = AfterOpenComm_ProduceLoad;
		
		// Disable all the buttom now
		OnEnableOperationButtom(0,0);
	}
}

void CPropPageData::OnUserVerify(void) 
{
	// 保存的路径为本程序所在的路径
	char name[256];
	GetModuleFileName(NULL, name, 256);
	
	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);

#if 0

	CString fileName = path + "At.log";
	CString fileUE = _T("notepad.exe");
	
	ShellExecute(this->m_hWnd, "open", fileUE, fileName, NULL, SW_SHOWNORMAL);
#else

	CInputPassDlg2     PwDlg;
	CVfManagerDlg      ManagerDlg;

	CString szErrMsg   = _T("");

	if (theApp.m_bIsAdmin)
	{		
		ManagerDlg.DoModal();
	}
	else
	{
		if (!theApp.m_bEnablePage)
		{

			if (IDOK != PwDlg.DoModal())
				return;

			szErrMsg.LoadString(IDS_STRING_ADMIN_PASSW);

			if (szErrMsg != PwDlg.m_sOldPass)
			{
				szErrMsg.Format(_T("InValid PassWord[%s]！"), PwDlg.m_sOldPass);
				MessageBox(szErrMsg, _T("Err"), MB_OK|MB_ICONSTOP);
				return;
			}
			
		}
        
		theApp.m_bEnablePage = !theApp.m_bEnablePage;
		
		if (theApp.m_bEnablePage)
		{
			GetDlgItem(IDC_PAGE_B_BTN_VIEW)->SetWindowText(_T("Lock"));
		}
		else
		{
			GetDlgItem(IDC_PAGE_B_BTN_VIEW)->SetWindowText(_T("UnLock"));
		}
        
		::SendMessage(m_pParentWnd->m_hWnd, WM_LOCK_PORO_PAGE_NOTIFY, theApp.m_bEnablePage, 0);
	}

#endif

}

void CPropPageData::OnLockPage(BOOL bLock)
{
	m_btLoadIni.EnableWindow(bLock);
	m_btSaveIni.EnableWindow(bLock);
#if defined(FOR_CLIENT)
    m_btLoadPhone.EnableWindow(bLock);
	m_btProduce.EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_B_EDIT_OLDPWD)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD)->EnableWindow(bLock);
	GetDlgItem(IDC_PAGE_B_EDIT_CNFPWD)->EnableWindow(bLock);
#else
	m_btLoadPhone.EnableWindow((theApp.m_bUseMode == UM_LOAD) || bLock);
	m_btProduce.EnableWindow((theApp.m_bUseMode == UM_SAVE) || bLock);
	GetDlgItem(IDC_PAGE_B_EDIT_OLDPWD)->EnableWindow((theApp.m_bUseMode == UM_SAVE) && bLock);
	GetDlgItem(IDC_PAGE_B_EDIT_NEWPWD)->EnableWindow((theApp.m_bUseMode == UM_SAVE) && bLock);
	GetDlgItem(IDC_PAGE_B_EDIT_CNFPWD)->EnableWindow((theApp.m_bUseMode == UM_SAVE) && bLock);
#endif
}

void CPropPageData::OnRefreshPage(void)
{
#if !defined(FOR_CLIENT)
	m_szIniInput = theApp.m_szIniInput;
	if (m_szIniInput.IsEmpty()){
		m_szIniInput = STR_INPUT_TIPS;
	}
	m_cStIniInput.SetWindowText(m_szIniInput);
#endif
}

void CPropPageData::OnShowPage(int nState)
{
	CompareIniState eState = (CompareIniState)(nState % CIS_NUM);

	m_stState.ShowWindow(CIS_NULL != eState);
	m_stState.setFont(-22);
	
	switch (eState)
	{
	case CIS_READING:
		m_stState.bkColor(RGB(235, 235, 235));
		m_stState.textColor(RGB(0, 0, 255));
		m_stState.SetWindowText(_T("Reading"));
		break;

	case CIS_WRITING:
		m_stState.bkColor(RGB(235, 235, 235));
		m_stState.textColor(RGB(0, 0, 255));
		m_stState.SetWindowText(_T("Writing"));
		break;

	case CIS_SAME:
		m_stState.bkColor(RGB(0, 255, 0));
		m_stState.textColor(RGB(0, 0, 255));
		m_stState.SetWindowText(_T("Pass"));
		break;

	case CIS_DIF:
		m_stState.bkColor(RGB(255, 0, 0));
		m_stState.textColor(RGB(0, 0, 255));
		m_stState.SetWindowText(_T("Fail"));
		break;

	default:
		break;
	}
}

void CPropPageData::OnEnChangePageBEditDefaultpwd()
{
	CDialog::UpdateData(TRUE);
	theApp.m_szPassw = m_strDefaultPwd;
	CDialog::UpdateData(FALSE);
}