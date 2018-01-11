// ThreeItemPackingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ThreeItemPacking.h"
#include "ThreeItemPackingDlg.h"
#include ".\threeitempackingdlg.h"
#include "ModifItemDlg.h"
#include "LockDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CThreeItemPackingDlg 对话框



CThreeItemPackingDlg::CThreeItemPackingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThreeItemPackingDlg::IDD, pParent)
	, m_strTelnum(_T(""))
	, m_strImei(_T(""))
	, m_strCcid(_T(""))
	, m_strTicket(_T(""))
	, m_strComboAmount(_T(""))
	, m_strcommodel(_T(""))
	, m_strcomcolor(_T(""))
	, m_strcomedition(_T(""))
	, m_strcomamount(_T(""))
	, m_strcomnshare(_T(""))
	, m_streditboxnum(_T(""))
	, m_strBuyer(_T(""))
	, m_strkeymsg(_T(""))
	, m_boolCardType()
	, m_strcardtype(_T(""))
{
	m_nIndex            = 0;
	m_strconfigpathname = _T("");
	Mykey               =  new (KEYDEF);
	Mykey->cNormal      = 0x00;
	Mykey->cShifted     = TAB;
	MySendkey           =  new (CSendkey);
	m_bLock             =  FALSE;
	pdate               =  (char *)malloc(sizeof(char)*12);
	ptime               =  (char *)malloc(sizeof(char)*12);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CThreeItemPackingDlg::~CThreeItemPackingDlg()
{
	delete Mykey;
	delete MySendkey;
	free(pdate);
	free(ptime);
}

void CThreeItemPackingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_CtrlCommodel);
	DDX_Control(pDX, IDC_COMBO3, m_CtrlComcolor);
	DDX_Control(pDX, IDC_COMBO4, m_CtrlComedition);
	DDX_Control(pDX, IDC_COMBO5, m_CtrlComamount);
	DDX_Control(pDX, IDC_COMBO6, m_CtrlComnshare);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_strTelnum);
	DDX_Text(pDX, IDC_EDIT2, m_strImei);
	DDX_Text(pDX, IDC_EDIT3, m_strCcid);
	DDX_Text(pDX, IDC_EDIT4, m_strTicket);
	DDX_Text(pDX, IDC_EDIT8, m_streditboxnum);
	DDX_Text(pDX, IDC_EDIT9, m_strBuyer);
	DDX_CBString(pDX, IDC_COMBO2, m_strcommodel);
	DDX_CBString(pDX, IDC_COMBO3, m_strcomcolor);
	DDX_CBString(pDX, IDC_COMBO4, m_strcomedition);
	DDX_CBString(pDX, IDC_COMBO5, m_strcomamount);
	DDX_CBString(pDX, IDC_COMBO5, m_strComboAmount);
	DDX_CBString(pDX, IDC_COMBO6, m_strcomnshare);	
	DDX_Check(pDX, IDC_CHECK1, m_boolCardType);
}

BEGIN_MESSAGE_MAP(CThreeItemPackingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_Button1, OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_Button2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_Button3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EDIT9, OnEnChangeEdit9)
	ON_EN_CHANGE(IDC_EDIT8, OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT4, OnEnChangeEdit4)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeComboModel)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchangeComboColor)
	ON_CBN_SELCHANGE(IDC_COMBO4, OnCbnSelchangeComboZone)
	ON_CBN_SELCHANGE(IDC_COMBO5, OnCbnSelchangeComboAmount)
	ON_CBN_SELCHANGE(IDC_COMBO6, OnCbnSelchangeComboNshare)
END_MESSAGE_MAP()


// CThreeItemPackingDlg 消息处理程序

BOOL  CThreeItemPackingDlg::PreTranslateMessage(MSG* pMsg)
{
	char vkmsg,//键盘消息的值
		strquery[256]  = "";//SQL查询语句
	CString QueryValue = _T("");//数据库查询所得的值
	CString TableName  = m_boolCardType ? "PackingDataEx" : "PackingData"; 
	int listcount      = 0;
	if (WM_KEYDOWN == pMsg->message){//键盘消息
		//收集键盘动作
		vkmsg = (char)(0xFF&pMsg->wParam);
		if (CharIsAlNum(vkmsg)) 	m_strkeymsg = m_strkeymsg + vkmsg;
		if  (pMsg->hwnd ==((GetDlgItem(IDC_EDIT1))->m_hWnd)){//这个消息属于TELNUM			
			if (VK_RETURN == pMsg->wParam){
				GetDlgItem(IDC_EDIT1)->GetWindowText(m_strTelnum);
				//m_strTelnum = m_strkeymsg;
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strTelnum.GetLength(), m_strTelnum);
				if (m_strTelnum.GetLength() != 11){
					MessageBox("电话号码只能够是11位", "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT1, m_strTelnum);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (!CStrIsNumber(m_strTelnum)){
					MessageBox("电话号码只能够是数字", "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT1, m_strTelnum);
					m_strkeymsg.Empty();
					return FALSE;
				}

				if (ItemExistInList(m_strTelnum, 1)){
					EmptyItemSetFocus(IDC_EDIT1, m_strTelnum);
					m_strkeymsg.Empty();
					return FALSE;
				}

				sprintf(strquery, "SELECT SERIAL FROM %s WHERE TELNUM = '%s'" ,TableName, m_strTelnum);
				m_adoConnection.ExecuteQueryValue((LPCTSTR)strquery, QueryValue);
				if (!QueryValue.IsEmpty()){
					sprintf(strquery, "数据库(%s)中已经存在这个TELNUM：%s\nSERIAL = %s", TableName, m_strTelnum, QueryValue);
					MessageBox(strquery, "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT1, m_strTelnum);
					m_strkeymsg.Empty();
					return FALSE;
				}
				MySendkey->Send(Mykey);
				m_strkeymsg.Empty();
				QueryValue.Empty();
				return TRUE;
			}else{
				return CDialog::PreTranslateMessage(pMsg);
			}
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT2))->m_hWnd)){//这个消息属于IMEI
			
			if (VK_RETURN == pMsg->wParam){
				//m_strImei = m_strkeymsg;
				GetDlgItem(IDC_EDIT2)->GetWindowText(m_strImei);
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strImei.GetLength(), m_strImei);
				if (m_strImei.GetLength() != 15){
					MessageBox("IMEI号只能够是15位", "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT2, m_strImei);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (!CStrIsNumAlpha(m_strImei)){
					MessageBox("IMEI号只能够是数字或者字母", "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT2, m_strImei);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (ItemExistInList(m_strImei, 2)){
					EmptyItemSetFocus(IDC_EDIT1, m_strImei);
					m_strkeymsg.Empty();
					return FALSE;
				}
				sprintf(strquery, "SELECT SERIAL FROM %s WHERE IMEI = '%s'" ,TableName, m_strImei);
				m_adoConnection.ExecuteQueryValue((LPCTSTR)strquery, QueryValue);
				if (!QueryValue.IsEmpty()){
					sprintf(strquery, "数据库(%s)中已经存在这个IMEI：%s\nSERIAL = %s", TableName, m_strImei, QueryValue);
					MessageBox(strquery, "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT2, m_strImei);
					m_strkeymsg.Empty();
					return FALSE;
				}
				MySendkey->Send(Mykey);
				m_strkeymsg.Empty();
				return TRUE;
			}else{
				return CDialog::PreTranslateMessage(pMsg);
			}
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT3))->m_hWnd)){//这个消息属于CCID			
			if (VK_RETURN == pMsg->wParam){
				//m_strCcid = m_strkeymsg;
				GetDlgItem(IDC_EDIT3)->GetWindowText(m_strCcid);
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strCcid.GetLength(), m_strCcid);
				if (m_strCcid.GetLength() != 20){
					MessageBox("CCID号码只能够是20位", "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT3, m_strCcid);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (!CStrIsNumAlpha(m_strCcid)){
					MessageBox("CCID只能够是数字或者字母", "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT3, m_strCcid);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (ItemExistInList(m_strCcid, 3)){
					EmptyItemSetFocus(IDC_EDIT1, m_strCcid);
					m_strkeymsg.Empty();
					return FALSE;
				}
				sprintf(strquery, "SELECT SERIAL FROM %s WHERE CCID = '%s'",TableName, m_strCcid);
				m_adoConnection.ExecuteQueryValue((LPCTSTR)strquery, QueryValue);
				if (!QueryValue.IsEmpty()){
					sprintf(strquery, "数据库(%s)中已经存在这个CCIC号：%s\nSERIAL = %s", TableName, m_strCcid, QueryValue);
					MessageBox(strquery, "提示", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT3, m_strCcid);
					m_strkeymsg.Empty();
					return FALSE;
				}
				//检查编辑框数据的完整性
				if (CheckEditIntegrity() != 0){
					return FALSE;
				}
				listcount = GetListItemCount();
				if (listcount >= GetListAmountCount()){//达到了每箱的数量
					//Delete List Item
					DeleteListAllItems();
					AddItemToList(m_strTelnum, m_strImei, m_strCcid, &m_list);
					//BOXNUM Add 1
					UpdateNumEditAdd(m_streditboxnum, 1);
				}else if(listcount+1 == GetListAmountCount()){
					AddItemToList(m_strTelnum, m_strImei, m_strCcid, &m_list);
					//Print Lable
					PrintfListData();
					//Store List'data
					StoreListDataToAccess();
				}else{
					AddItemToList(m_strTelnum, m_strImei, m_strCcid, &m_list);
				}
				EmptyCEidtItemInList(m_boolCardType);
				MySendkey->Send(Mykey);
				m_strkeymsg.Empty();
				return TRUE;
			}else{
				return CDialog::PreTranslateMessage(pMsg);
			}
		}else{
			//这个消息属于其他的窗口元素
			return CDialog::PreTranslateMessage(pMsg); 
		}
	}else{		
		return CDialog::PreTranslateMessage(pMsg); 
	}
}


BOOL CThreeItemPackingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString str;

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	{
		//静态控件的初始化
		m_stcticket.SubclassDlgItem(IDC_STATIC06, this);
		m_stcticket.SetWindowText("送检查通知单：");
		m_stcticket.setFont(25);
		m_stcticket.textColor(RGB(0, 0, 255));

		m_CtrlTicket.SubclassDlgItem(IDC_EDIT4, this);
		m_CtrlTicket.setFont(25);
		m_CtrlTicket.textColor(RGB(0, 0, 255));
		m_CtrlTicket.LimitText(15);
		m_CtrlBuyer.SubclassDlgItem(IDC_EDIT9, this);
		m_CtrlBuyer.textColor(RGB(255, 0, 0));
		//m_CtrlBuyer.setFont(23);
		m_CtrlBuyer.LimitText(10);
	}

	{
		//按钮的初始化
		m_CtrlButtonLock.SubclassDlgItem(IDOK,  this);
		m_CtrlButtonLock.SetWindowText("锁 定 设 置");
		m_CtrlButtonPrint.SubclassDlgItem(IDC_Button2,  this);
		m_CtrlButtonPrint.SetWindowText("打 印");
		m_CtrlButtonDelete.SubclassDlgItem(IDC_Button1, this);
		m_CtrlButtonDelete.SetWindowText("清空列表");
		m_CtrlButtonStroe.SubclassDlgItem(IDC_Button3, this);
		m_CtrlButtonStroe.SetWindowText("存储数据");
		//隐藏取消按钮
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	}

	{
		//标题以及配置文件路径的初始化
		::GetCurrentDirectory(MAX_PATHLENGTH,m_strconfigpathname.GetBuffer(MAX_PATHLENGTH));
		m_strconfigpathname.ReleaseBuffer();
		m_strconfigpathname += "\\PackConfig.ini";
		str = GetCommandLine();
		str = str.Left(str.GetLength()-2);
		str = str.Right(str.GetLength()-1);
		//str ="PRINT WIDTH 105 08/12 MM+  " + str;
		//修改了标题栏版本号
		str = "装箱条型码打印-V1.0.3b   " + str;
		SetWindowText(str);
		str.Empty();
	}

	{
		//列表控件的初始化
		m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
		m_list.SetBkColor(RGB(247,247,255));
		m_list.SetTextColor(RGB(0,0,255));
		m_list.SetTextBkColor(RGB(247,247,255));
		m_list.InsertColumn(0, "序号", LVCFMT_CENTER, 40);
		m_list.InsertColumn(1, "TELNUM", LVCFMT_CENTER, 110);
		m_list.InsertColumn(2, "IMEI", LVCFMT_CENTER, 150);
		m_list.InsertColumn(3, "CCID", LVCFMT_CENTER, 180);
		/*
		m_list.InsertItem(0,"000");
		m_list.SetItemText(0,1,"00000000000");
		m_list.SetItemText(0,2,"000000000000000");
		m_list.SetItemText(0,3,"00000000000000000000");
		m_list.InsertItem(1,"001");
		m_list.SetItemText(1,1,"11111111111");
		m_list.SetItemText(1,2,"111111111111111");
		m_list.SetItemText(1,3,"11111111111111111111");
		m_list.InsertItem(2,"001");
		m_list.SetItemText(2,1,"22222222222");
		m_list.SetItemText(2,2,"222222222222222");
		m_list.SetItemText(2,3,"22222222222222222222");
		m_list.InsertItem(3,"003");
		m_list.SetItemText(3,1,"33333333333");
		m_list.SetItemText(3,2,"333333333333333");
		m_list.SetItemText(3,3,"33333333333333333333");
		m_list.InsertItem(4,"004");
		m_list.SetItemText(4,1,"44444444444");
		m_list.SetItemText(4,2,"444444444444444");
		m_list.SetItemText(4,3,"44444444444444444444");
		m_list.InsertItem(5,"005");
		m_list.SetItemText(5,1,"55555555555");
		m_list.SetItemText(5,2,"555555555555555");
		m_list.SetItemText(5,3,"55555555555555555555");
		m_list.InsertItem(6,"006");
		m_list.SetItemText(6,1,"66666666666");
		m_list.SetItemText(6,2,"666666666666666");
		m_list.SetItemText(6,3,"66666666666666666666");
		m_list.InsertItem(7,"007");
		m_list.SetItemText(7,1,"77777777777");
		m_list.SetItemText(7,2,"777777777777777");
		m_list.SetItemText(7,3,"77777777777777777777");
		m_list.InsertItem(8,"008");
		m_list.SetItemText(8,1,"88888888888");
		m_list.SetItemText(8,2,"888888888888888");
		m_list.SetItemText(8,3,"88888888888888888888");
		m_list.InsertItem(9,"009");
		m_list.SetItemText(9,1,"99999999999");
		m_list.SetItemText(9,2,"999999999999999");
		m_list.SetItemText(9,3,"99999999999999999999");
		*/
	}

	{
		m_CtrlTelnum.SubclassDlgItem(IDC_EDIT1, this);
		m_CtrlImei.SubclassDlgItem(IDC_EDIT2, this);
		m_CtrlCcid.SubclassDlgItem(IDC_EDIT3, this);
		//m_CtrlTelnum.LimitText(11);
		//m_CtrlImei.LimitText(15);
		//m_CtrlCcid.LimitText(20);
		m_CtrlTelnum.textColor(RGB(255, 0, 0));
		m_CtrlImei.textColor(RGB(255, 0, 0));
		m_CtrlCcid.textColor(RGB(255, 0, 0));
		m_CtrlBoxnum.SubclassDlgItem(IDC_EDIT8, this);
		m_CtrlBoxnum.LimitText(18);
		m_CtrlBoxnum.textColor(RGB(255, 0, 0));
	}

	{
		//CComboBox  控件的初始化
		char flag = ',';
		int  nculsel = 0;
		str.Empty();
		//型号：
		GetPrivateProfileString("MODEL", "MODELLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("MODEL", "MODELCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlCommodel, str, flag, nculsel);

		//颜色：
		GetPrivateProfileString("COLOR", "COLORLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("COLOR", "COLORCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComcolor, str, flag, nculsel);

		//版本：
		GetPrivateProfileString("EDITION", "EDITIONLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("EDITION", "EDITIONCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComedition, str, flag, nculsel);

		//数量(套)：
		GetPrivateProfileString("AMOUNT", "AMOUNTLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("AMOUNT", "AMOUNTCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComamount, str, flag, nculsel);

		//打印份数：
		GetPrivateProfileString("NSHARE", "NSHARELIST", "00", str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("NSHARE", "NSHARECURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComnshare, str, flag, nculsel);
	}

	{
		// 编辑控件初始化
		GetPrivateProfileString("OTHER", "BUYER","NC1234",str.GetBuffer(MAX_PATHLENGTH), 
			MAX_PATHLENGTH, m_strconfigpathname);
		str.ReleaseBuffer();
		m_strBuyer = str;
		m_CtrlBuyer.SetWindowText(m_strBuyer);

		GetPrivateProfileString("OTHER", "BOXNUM","123456789123456789",str.GetBuffer(MAX_PATHLENGTH), 
			MAX_PATHLENGTH, m_strconfigpathname);
		str.ReleaseBuffer();
		m_streditboxnum = str;
		m_CtrlBoxnum.SetWindowText(m_streditboxnum);

		GetPrivateProfileString("OTHER", "TICKET","HLP0123456789",str.GetBuffer(MAX_PATHLENGTH), 
			MAX_PATHLENGTH, m_strconfigpathname);
		str.ReleaseBuffer();
		m_strTicket = str;
		m_CtrlTicket.SetWindowText(m_strTicket);

		GetPrivateProfileString("OTHER", "CARDTYPE","FALSE",str.GetBuffer(MAX_PATHLENGTH), 
			MAX_PATHLENGTH, m_strconfigpathname);
		str.ReleaseBuffer();
		m_strcardtype = str;
		if (m_strcardtype == _T("FALSE")){
			m_boolCardType = FALSE;
			//((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(m_boolCardType);
		}else{
			m_boolCardType = TRUE;
			((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(m_boolCardType);
			m_strTelnum = "XXXXXXXXXXX";
			m_CtrlTelnum.SetWindowText(m_strTelnum);
			m_CtrlTelnum.EnableWindow(!m_boolCardType);
		}

	}

	{
		//连接数据库
		if(!m_adoConnection.ConnectAccess("LablePacking.mdb")){
			MessageBox(_T("连接数据库：LablePacking.mdb失败！"), _T("提示"), MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CThreeItemPackingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CThreeItemPackingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CThreeItemPackingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CThreeItemPackingDlg::ItemExistInList(CString str, int nSubItem)
{
	CDialog::UpdateData(TRUE);
	int i, nCount = m_list.GetItemCount();
	CString nItemstr;
	for (i=0; i<nCount; i++){
		nItemstr = m_list.GetItemText(i, nSubItem);
		if (nItemstr == str){//如果已经存在了这个记录
			nItemstr.Format("列表中已经存在这个记录\n序号：%s", m_list.GetItemText(i, 0));
			MessageBox(nItemstr);
			return TRUE;
		}
	}
	CDialog::UpdateData(FALSE);
	return FALSE;
}
void CThreeItemPackingDlg::EmptyItemSetFocus(int nId, CString &var)
{
	CDialog::UpdateData(TRUE);
	GetDlgItem(nId)->SetWindowText("");
	var.Empty();
	GetDlgItem(nId)->SetFocus();
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::EmptyCEidtItemInList(BOOL bTypeCard)
{
	CDialog::UpdateData(TRUE);
	GetDlgItem(IDC_EDIT2)->SetWindowText("");
	GetDlgItem(IDC_EDIT3)->SetWindowText("");
	m_strImei.Empty();
	m_strCcid.Empty();
	if (!bTypeCard){
		GetDlgItem(IDC_EDIT1)->SetWindowText("");
		m_strTelnum.Empty();
	}
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::EnableCEidtItemInList(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT3)->EnableWindow(bEnable);
	return;
}


void CThreeItemPackingDlg::EnableCComboxInList(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT8)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT9)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO2)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO3)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO4)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO5)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO6)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT4)->EnableWindow(bEnable);
	return;
}

SHORT CThreeItemPackingDlg::InitCComboBoxByDiscontinuousStr(CComboBox *VarCtrl, 
													   CString &DiscontinuousStr, 
													   char BrokenCh,  
													   int nCurSel)
{
	int Itemcount = VarCtrl->GetCount();
	CString temp, str = DiscontinuousStr;
	for (int i=0; i<Itemcount; i++){
		VarCtrl->DeleteString(0);
		VarCtrl->UpdateData();
	}
	i = 0;
	while(!str.IsEmpty())
	{
		i++;
		Itemcount = str.Find(BrokenCh);
		if (Itemcount > 0){
			temp = str;
			str = str.Left(Itemcount);
			VarCtrl->AddString(str);
			str = temp.Mid(Itemcount+1);
		}else{
			VarCtrl->AddString(str);
			break;
		}
	}
	VarCtrl->SetCurSel(nCurSel);
	return i;
}


void CThreeItemPackingDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CModifItemDlg   dlg;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	m_nIndex     = m_list.GetNextSelectedItem(pos);  // 得到项目索引
	if (m_list.GetItemText(m_nIndex, 0).IsEmpty()){
		AfxMessageBox("请选择非空条目!", MB_OK|MB_ICONWARNING);
		return;
	}
	
	dlg.m_strSerial = m_list.GetItemText(m_nIndex, 0);
	dlg.m_strTelnum = m_list.GetItemText(m_nIndex, 1);
	dlg.m_strImei   = m_list.GetItemText(m_nIndex, 2);
	dlg.m_strCcid   = m_list.GetItemText(m_nIndex, 3);

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strTelnum.GetLength() < 11){
			MessageBox("TELNUM 不够 11 位！", "提示", MB_OK|MB_ICONERROR);
			return;
		}else if (dlg.m_strImei.GetLength() < 15){
			MessageBox("IMEI 不够 15 位！", "提示", MB_OK|MB_ICONERROR);
			return;
		}else if (dlg.m_strCcid.GetLength() < 20){
			MessageBox("CCID 不够 20 位！", "提示", MB_OK|MB_ICONERROR);
			return;
		}else{
			m_list.SetItemText(m_nIndex, 1, dlg.m_strTelnum);
			m_list.SetItemText(m_nIndex, 2, dlg.m_strImei);
			m_list.SetItemText(m_nIndex, 3, dlg.m_strCcid);
		}		
	}
	*pResult = 0;
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CLockDlg  LockDlg;
	if (m_bLock == FALSE)
	{
		EnableCComboxInList(FALSE);
		m_CtrlButtonLock.SetWindowText("解 锁 配 置");
		m_bLock = !m_bLock;
	}
	else
	{
		if (IDOK == LockDlg.DoModal())
		{
			LockDlg.m_strpw.MakeUpper();
			if (("DESAYELE" == LockDlg.m_strpw)||
				("ERROR" == LockDlg.m_strpw))
			{
				EnableCComboxInList(TRUE);
				m_CtrlButtonLock.SetWindowText("锁 定 配 置");
				m_bLock = !m_bLock;
			}
			else
			{
				MessageBox("密码错误！", "提示", MB_OK|MB_ICONERROR);
			}
		}
	}
	CDialog::UpdateData(FALSE);

}

void CThreeItemPackingDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	if (m_list.GetItemCount() == 0) return;
	if (IDYES == MessageBox("该操作将会把列表框的所有条目删除\r\n       你确定删除吗？", "询问", MB_YESNO|MB_ICONASTERISK))
		m_list.DeleteAllItems();
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();	
}

int CThreeItemPackingDlg::PrintfListData(void)
{
	CDialog::UpdateData(TRUE);

	FILE *fp = fopen("PrintCmd.nfo", "w");

	char *cmdformat = (char *)malloc(sizeof(char)*ONE_KILO*2);
	char *PrintCmd  = (char *)malloc(sizeof(char)*(ONE_KILO*30));
	//列表控件的记录量
	int nCount      =  m_list.GetItemCount();
	//计算条码高度的参数，若列表的数据不够10条，按10条的规格计算
	int BARHEIGHET      =  (MMDOTSA*6);//(nCount < 10) ? (MMDOTSA*8):(MMDOTSA*6);
	int nMMspace        =   10;

	CString         strini;//用于读取配置文件的字段值

	XY              xy, 
		            xytemp;  //起始坐标结构,从配置文件读入
	ZPLFONT         zf;      //字体的打印结构,从配置文件读入

	BARBY           by;      //条码模块设置结构

	LPCHINESEZPL    lpchzpl = new CHINESEZPL; //汉字编码结构
	

	//三个字符串获取m_list列表的TELNUM, IMEI, CCID
	char Telnum[12] = "",
		 Imei[16]   = "",
		 Ccid[21]   = "";

	//检查数据的完整性
	if (m_strTicket.GetLength() < TicketLen)
	{
		MessageBox("请将送检通知单填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -1;
	}
	if (m_streditboxnum.GetLength() < 18)
	{
		MessageBox("请将箱号填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -2;
	}
	if (m_strBuyer.GetLength() < 6)
		{
		MessageBox("请将生产批次填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -3;
	}

	if (NULL == fp){
		MessageBox("无法建立文件：PrintCmd.cfg", "提示", MB_OK|MB_ICONERROR);
		return -4;
	}
	if (NULL == cmdformat){
		MessageBox("动态分配内存失败：2k！", "错误", MB_OK|MB_ICONERROR);
		return -5;
	}
	if (NULL == PrintCmd){
		MessageBox("动态分配内存失败：30k！", "错误", MB_OK|MB_ICONERROR);
		return -30;
	}

	xy.x_axis     = GetPrivateProfileInt("LABLECONFIG", "INITX",      0, m_strconfigpathname);
	xy.y_axis     = GetPrivateProfileInt("LABLECONFIG", "INITY",      0, m_strconfigpathname);
	xytemp.x_axis = xy.x_axis;
	xytemp.y_axis = xy.y_axis;
	zf.height     = GetPrivateProfileInt("LABLECONFIG", "FONTHEIGHT", 0, m_strconfigpathname);
	zf.width      = GetPrivateProfileInt("LABLECONFIG", "FONTWIDTH",  0, m_strconfigpathname);
	zf.name       = '0';
	zf.Orientation= 'N';
	by.heighet    = BARHEIGHET;
	by.width      = 2;
	by.width_ratio= 2.0;

	{//LPCHINESEZPL结构的初始化
		GetPrivateProfileString("CHINESEBARCODE", "FONTNAME", "宋体", strini.GetBuffer(MAX_PATHLENGTH),  MAX_PATHLENGTH,  m_strconfigpathname);
		strini.ReleaseBuffer();		
		lpchzpl->lpFontName = strini;
		lpchzpl->intHeight  = GetPrivateProfileInt("CHINESEBARCODE", "INTHEIGHT",  14, m_strconfigpathname);
		lpchzpl->intWidth   = GetPrivateProfileInt("CHINESEBARCODE", "INTWIDTH",    0, m_strconfigpathname);
		lpchzpl->intOrient  = GetPrivateProfileInt("CHINESEBARCODE", "INTORIENT",   0, m_strconfigpathname);
		lpchzpl->boolBold   = GetPrivateProfileInt("CHINESEBARCODE", "BOOLBOLD",    0, m_strconfigpathname);
		lpchzpl->boolItalic = GetPrivateProfileInt("CHINESEBARCODE", "BOOLITALIC",  0, m_strconfigpathname);
		lpchzpl->xmf        = GetPrivateProfileInt("CHINESEBARCODE", "XGMX",        1, m_strconfigpathname);
		lpchzpl->ymf        = GetPrivateProfileInt("CHINESEBARCODE", "XGMY",        1, m_strconfigpathname);
	}

	strcpy(PrintCmd,  "");
	//strcpy(PrintCmd,  PrintInitCmd);
	fprintf(fp, "%s\r",  PrintInitCmd);
	strcat(PrintCmd,  "^XA");
	fprintf(fp,  "^XA\r");

	{//打印箱号、代码和分隔横线
		//打印“箱 号：”
		lpchzpl->xy.x_axis = xy.x_axis + MMDOTSA;
		lpchzpl->xy.y_axis = xy.y_axis + MMDOTSA*5;
		lpchzpl->lpChinese = "箱 号：";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		xytemp.x_axis      = xy.x_axis + MMDOTSA*15;
		xytemp.y_axis      = 0;//xy.y_axis + MMDOTSA*18;
		//打印箱号的条码
		sprintf(cmdformat, "^FO%d,%d^BY2,2.00,%d^BAN,,N,N,N^FD%s^FS", 
			xytemp.x_axis, xytemp.y_axis+MMDOTSA*2, MMDOTSA*10, m_streditboxnum);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		sprintf(cmdformat, "^FO%d,%d^A0,N,%d, %d^FD%s^FS", 
			xytemp.x_axis + MMDOTSA*4, xytemp.y_axis + MMDOTSA*13, MMDOTSA*3, MMDOTSA*2, m_streditboxnum);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	
		//打印“代 码：”
		lpchzpl->xy.x_axis = xytemp.x_axis + MMDOTSA*40;
		lpchzpl->lpChinese = "代 码：";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印代码的条码
		xytemp.x_axis  =  lpchzpl->xy.x_axis + MMDOTSA*14;
		sprintf(cmdformat, "^FO%d,%d^BY2,2.00,%d^BAN,,N,N,N^FDDESAYELE^FS", 
			xytemp.x_axis, xytemp.y_axis+MMDOTSA*2, MMDOTSA*10);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		sprintf(cmdformat, "^FO%d,%d^A0,N,%d, %d^FDDESAYELE^FS", 
			xytemp.x_axis + MMDOTSA*2, xytemp.y_axis + MMDOTSA*13, MMDOTSA*3, MMDOTSA*2);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印一条分隔线
		xy.x_axis = MMDOTSA * 5;
		xy.y_axis = lpchzpl->xy.y_axis + MMDOTSA*12;
		sprintf(cmdformat, "^FO%d,%d^GB%d,0,5,B,0^FS",xy.x_axis, xy.y_axis, 110*MMDOTSA);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	}

	{//打印型号、颜色、区域、数量
		//打印“型 号：”字样
		lpchzpl->lpChinese  = "型  号：";
		lpchzpl->intHeight  = lpchzpl->intHeight/2;
		lpchzpl->xy.x_axis  = xy.x_axis + MMDOTSA*2;
		lpchzpl->xy.y_axis  = xy.y_axis + MMDOTSA*2;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印具体的型号条码
		xytemp.x_axis  =  lpchzpl->xy.x_axis;
		xytemp.y_axis  =  lpchzpl->xy.y_axis;
		xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA * 10;
		xy.y_axis = lpchzpl->xy.y_axis;
		sprintf(cmdformat,"^FO%d,%d^A0N,%d,%d^FD%s^FS", 
			xy.x_axis, xy.y_axis, lpchzpl->intHeight + MMDOTSA*3, lpchzpl->intHeight, m_strcommodel);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印“颜 色：”字样
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*60;
		lpchzpl->lpChinese = "颜 色：";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//打印具体的颜色
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*12;
		lpchzpl->lpChinese = m_strcomcolor;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印“区 域：”字样
		lpchzpl->xy.x_axis = xytemp.x_axis;
		lpchzpl->xy.y_axis = xytemp.y_axis + lpchzpl->intHeight + MMDOTSA*4;
		lpchzpl->lpChinese = "区  域：";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//打印具体的区域
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*10;
		lpchzpl->lpChinese = m_strcomedition;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印“数 量：”字样
		lpchzpl->xy.x_axis = xytemp.x_axis + MMDOTSA*60;
		lpchzpl->lpChinese = "数 量：";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//打印具体的数量
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*12;
		sprintf(cmdformat, "^FO%d,%d^A0N,%d,%d^FD%s^FS",
			lpchzpl->xy.x_axis, lpchzpl->xy.y_axis, lpchzpl->intHeight + MMDOTSA*3, lpchzpl->intHeight, this->m_strComboAmount);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//打印数量的条码
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*8;
		sprintf(cmdformat, "^FO%d,%d^BY2,2.00,%d^BAN,,N,N,N^FD%s^FS", 
			lpchzpl->xy.x_axis, lpchzpl->xy.y_axis, lpchzpl->intHeight + MMDOTSA*2, this->m_strComboAmount);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	}

	{//打印列表内的所以条码数据
#define    BARCODELEN     (MMDOTSA*6);
		if (nCount > 0){
			//没有数据就跳过
			xytemp.x_axis = xy.x_axis = MMDOTSA*8;
			xytemp.y_axis = xy.y_axis = lpchzpl->xy.y_axis + max(MMDOTSA, lpchzpl->intHeight) + MMDOTSA*4;
			if(m_boolCardType){//如果是白卡，则只打印IMEI、CCID
				xytemp.x_axis = xytemp.x_axis + MMDOTSA*5;
				nMMspace      = nMMspace + 5;
				for (m_nIndex=0; m_nIndex<nCount; m_nIndex++){//逐条打印
					strcpy(Imei,   m_list.GetItemText(m_nIndex, 2));
					strcpy(Ccid,   m_list.GetItemText(m_nIndex, 3));
					CreatePackingItem(cmdformat, Imei, Ccid, by, xytemp, nMMspace);
					strcat(PrintCmd,  cmdformat);
					fprintf(fp,  "%s\r", cmdformat);
					xytemp.y_axis = xytemp.y_axis + by.heighet + MMDOTSA*4;
				}
			}else{				
				for (m_nIndex=0; m_nIndex<nCount; m_nIndex++){//逐条打印
					strcpy(Telnum, m_list.GetItemText(m_nIndex, 1));
					strcpy(Imei,   m_list.GetItemText(m_nIndex, 2));
					strcpy(Ccid,   m_list.GetItemText(m_nIndex, 3));
					CreatePackingItem(cmdformat, Telnum, Imei, Ccid, by, xytemp, nMMspace);
					strcat(PrintCmd,  cmdformat);
					fprintf(fp,  "%s\r", cmdformat);
					xytemp.y_axis = xytemp.y_axis + by.heighet + MMDOTSA*4;
				}
			}			
		}
	}

	{//打印分隔横线和送检查通知单、Q/A：、日期
		//打印一条分隔线，距底12mm， 两端空2mm
		xy.x_axis = MMDOTSA * 5;
		xy.y_axis = (150 - 15) * MMDOTSA;
		sprintf(cmdformat, "^FO%d,%d^GB%d,0,5,B,0^FS", xy.x_axis, xy.y_axis, 110*MMDOTSA);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印“送检通知单：”
		lpchzpl->lpChinese   =  "送检通知单：";
		lpchzpl->intHeight   = lpchzpl->intHeight;
		lpchzpl->boolBold    = 1;
		lpchzpl->xy.x_axis   = xy.x_axis + MMDOTSA * 3;
		lpchzpl->xy.y_axis   = xy.y_axis + MMDOTSA * 2;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//打印送检通知单的具体数据
		xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA * 15;
		xy.y_axis = lpchzpl->xy.y_axis + MMDOTSA;
		sprintf(cmdformat,"^FO%d,%d^A0N,%d,%d^FD%s         Q/A:^FS", 
			xy.x_axis, xy.y_axis, lpchzpl->intHeight+20, lpchzpl->intHeight+10, m_strTicket);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//打印“日期：”字样
		lpchzpl->lpChinese   =  "日  期：";
		lpchzpl->xy.x_axis   =  lpchzpl->xy.x_axis + MMDOTSA*(65+3);
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//打印系统的日期
		xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA * 10;
		GetSysTime(ptime, pdate);
		sprintf(cmdformat,"^FO%d,%d^A0N,%d,%d^FD%s^FS", 
			xy.x_axis, xy.y_axis, lpchzpl->intHeight+20, lpchzpl->intHeight+10, pdate);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	}
   
	{//在这里处理打印分数及结束
		sprintf(cmdformat, "^PQ%d,0,1,Y", atoi(m_strcomnshare));
		strcat(PrintCmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		strcat(PrintCmd, "^XZ");
		fprintf(fp, "^XZ");
	}
	ExecutePrintCmdInLpt(PrintCmd, "LPT1");
	CDialog::UpdateData(FALSE);
	fclose(fp);
	free(cmdformat);
	free(PrintCmd);
	delete lpchzpl;
	return nCount;
}

int CThreeItemPackingDlg::GetListAmountCount(void)
{
	CDialog::UpdateData(TRUE);
	int count = atoi(m_strComboAmount);
	CDialog::UpdateData(FALSE);
	return count;
}

int CThreeItemPackingDlg::GetListItemCount(void)
{
	CDialog::UpdateData(TRUE);
	int count = this->m_list.GetItemCount();
	CDialog::UpdateData(FALSE);
	return count;
}

void  CThreeItemPackingDlg::AddItemToList(CString strimei, CString strccid, CListCtrl *list)
{
	CDialog::UpdateData(TRUE);
	int nCount = list->GetItemCount();
	CString strlistcount;
	strlistcount.Format("%d%d%d", (nCount+1)/100, (nCount+1)/10%10, (nCount+1)%10);
	list->InsertItem(nCount, strlistcount);
	list->SetItemText(nCount, 1, "XXXXXXXXXXX");
	list->SetItemText(nCount, 2, strimei);
	list->SetItemText(nCount, 3, strccid);
	CDialog::UpdateData(FALSE);
}

void  CThreeItemPackingDlg::AddItemToList(CString strtel, CString strimei, CString strccid, CListCtrl *list)
{
	CDialog::UpdateData(TRUE);
	int nCount = list->GetItemCount();
	CString strlistcount;
	strlistcount.Format("%d%d%d", (nCount+1)/100, (nCount+1)/10%10, (nCount+1)%10);
	list->InsertItem(nCount, strlistcount);
	list->SetItemText(nCount, 1, strtel);
	list->SetItemText(nCount, 2, strimei);
	list->SetItemText(nCount, 3, strccid);
	CDialog::UpdateData(FALSE);
}


int CThreeItemPackingDlg::CheckEditIntegrity(void)
{
	CDialog::UpdateData(TRUE);
	if (m_strTicket.GetLength() < TicketLen)
	{
		MessageBox("请将送检通知单填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -1;
	}
	if (m_streditboxnum.GetLength() < 18)
	{
		MessageBox("请将箱号填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -2;
	}
	if (m_strBuyer.GetLength() < 6)
	{
		MessageBox("请将生产批次填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -3;
	}
	if (m_strTelnum.GetLength() != 11){
		MessageBox("TELNUM 长度不对！", "提示", MB_OK|MB_ICONERROR);
		return -11;
	}
	if (m_strImei.GetLength() != 15){
		MessageBox("IMEI 长度不对！", "提示", MB_OK|MB_ICONERROR);
		return -15;
	}
	if (m_strCcid.GetLength() != 20){
		MessageBox("CCID 长度不对！", "提示", MB_OK|MB_ICONERROR);
		return -20;
	}
	CDialog::UpdateData(FALSE);
	return 0;
}


int CThreeItemPackingDlg::StoreListDataToAccess(void)
{
	CDialog::UpdateData(TRUE);
	int nCount = m_list.GetItemCount();
	m_nIndex = 0;
	char *strsql = (char *)malloc(sizeof(char) * HALF_KILO);
	char Telnum[12] = "",
		 Imei[16]   = "",
		 Ccid[21]   = "";
	CString TableName = m_boolCardType ? "PackingDataEx" : "PackingData"; 
	FILE *fp;
	fp = fopen("sqlcmd.nfo", "w");
	if (m_strTicket.GetLength() < TicketLen)
	{
		MessageBox("请将送检通知单填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -1;
	}
	if (m_streditboxnum.GetLength() < 18)
	{
		MessageBox("请将箱号填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -2;
	}
	if (m_strBuyer.GetLength() < 6)
		{
		MessageBox("请将生产批次填写完整！", "提示", MB_OK|MB_ICONERROR);
		return -3;
	}
	//获取系统的时间
	GetSysTime(ptime,  pdate);
	for (m_nIndex=0; m_nIndex<nCount; m_nIndex++)
	{
		strcpy(Telnum, m_list.GetItemText(m_nIndex, 1));
		strcpy(Imei,   m_list.GetItemText(m_nIndex, 2));
		strcpy(Ccid,   m_list.GetItemText(m_nIndex, 3));
		sprintf(strsql, "INSERT INTO %s (BOXNUM, BUYER, PHMODEL, PHCOLOR, TELNUM, IMEI, CCID, BOXAMOUNT, DATEDATE, CHECKTICKET, AREA) VALUES('%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",	
			TableName, m_streditboxnum, m_strBuyer, m_strcommodel, m_strcomcolor, Telnum, Imei, Ccid, m_strcomamount, pdate, m_strTicket, m_strcomedition);
		fprintf(fp, "%s\r", strsql);
		m_adoConnection.BeginTrans();
		m_adoConnection.Execute(strsql);
		m_adoConnection.CommitTrans();
	}
	CDialog::UpdateData(FALSE);
	free(strsql);
	fclose(fp);
	return nCount;
}

void CThreeItemPackingDlg::UpdateNumEditAdd(CString &strVar, int nStep)
{
	CDialog::UpdateData(TRUE);
	char buf[64] = "";
	strcpy(buf, strVar);
	NumStrAutoAdd(buf, nStep);
	strVar = buf;
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::DeleteListAllItems(void)
{
	CDialog::UpdateData(TRUE);
	m_list.DeleteAllItems();
	CDialog::UpdateData(FALSE);
}


void CThreeItemPackingDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	PrintfListData();
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	StoreListDataToAccess();
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	m_boolCardType  = !m_boolCardType;
	DeleteListAllItems();
	EmptyCEidtItemInList();
	GetDlgItem(IDC_EDIT1)->EnableWindow(!m_boolCardType);
	if (m_boolCardType){		
		EmptyItemSetFocus(IDC_EDIT2, m_strImei);
		m_strTelnum = "XXXXXXXXXXX";
		WritePrivateProfileString(_T("OTHER"), _T("CARDTYPE"), _T("TRUE"), m_strconfigpathname);
	}else{
		EmptyItemSetFocus(IDC_EDIT1, m_strTelnum);
		WritePrivateProfileString(_T("OTHER"), _T("CARDTYPE"), _T("FALSE"), m_strconfigpathname);
	}	
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnEnChangeEdit9()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	WritePrivateProfileString(_T("OTHER"), _T("BUYER"), m_strBuyer, m_strconfigpathname);
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnEnChangeEdit8()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	WritePrivateProfileString(_T("OTHER"), _T("BOXNUM"), m_streditboxnum, m_strconfigpathname);
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	WritePrivateProfileString(_T("OTHER"), _T("TICKET"), m_strTicket, m_strconfigpathname);
	CDialog::UpdateData(FALSE);
}

void CThreeItemPackingDlg::OnCbnSelchangeComboModel()
{
	// TODO: Add your control notification handler code here
	CString  str = _T("");
	str.Format("%d", m_CtrlCommodel.GetCurSel());
	WritePrivateProfileString(_T("MODEL"), _T("MODELCURSEL"), str, m_strconfigpathname);
	
}

void CThreeItemPackingDlg::OnCbnSelchangeComboColor()
{
	// TODO: Add your control notification handler code here
	CString  str = _T("");
	str.Format("%d", m_CtrlComcolor.GetCurSel());
	WritePrivateProfileString(_T("COLOR"), _T("COLORCURSEL"), str, m_strconfigpathname);
}

void CThreeItemPackingDlg::OnCbnSelchangeComboZone()
{
	// TODO: Add your control notification handler code here
	CString  str = _T("");
	str.Format("%d", m_CtrlComedition.GetCurSel());
	WritePrivateProfileString(_T("EDITION"), _T("EDITIONCURSEL"), str, m_strconfigpathname);
}

void CThreeItemPackingDlg::OnCbnSelchangeComboAmount()
{
	// TODO: Add your control notification handler code here
	CString  str = _T("");
	str.Format("%d", m_CtrlComamount.GetCurSel());
	WritePrivateProfileString(_T("AMOUNT"), _T("AMOUNTCURSEL"), str, m_strconfigpathname);
}

void CThreeItemPackingDlg::OnCbnSelchangeComboNshare()
{
	// TODO: Add your control notification handler code here
	CString  str = _T("");
	str.Format("%d", m_CtrlComnshare.GetCurSel());
	WritePrivateProfileString(_T("NSHARE"), _T("NSHARECURSEL"), str, m_strconfigpathname);
}
