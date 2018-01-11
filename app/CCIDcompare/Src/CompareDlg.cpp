// CompareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CCIDcompare.h"
#include "CompareDlg.h"
#include "PasswordDlg.h"
#include "ImportDialog.h"
#include "OutportDlg.h"
#include ".\comparedlg.h"
#include "DlgInvalidCardPrint.h"
#include ".\res\Version.h"
#include "about.h"
// CCompareDlg dialog


IMPLEMENT_DYNAMIC(CCompareDlg, CDialog)
CCompareDlg::CCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompareDlg::IDD, pParent)
	, m_input(_T(""))
{
	buf = (char *) malloc(sizeof(char)*512);
	OpenLink = (char *) malloc(sizeof(char)*768);
	GetFileRow("config.ini", buf, 2);
	font.name = buf[0];
	GetFileRow("config.ini", buf, 3);
	font.Orientation = buf[0];
	GetFileRow("config.ini", buf, 4);	
	font.height = atol(buf);	
	GetFileRow("config.ini", buf, 5);
	font.width = atol(buf);
	GetFileRow("config.ini", buf, 6);
	X_coordinate = atol(buf);
	GetFileRow("config.ini", buf, 7);
	Y_coordinate = atol(buf);
	GetFileRow("config.ini", buf, 9);
	if(NULL == strstr(buf, "TRUE"))
	{
		OverBarcode = FALSE;
	}
	else
	{
		OverBarcode = TRUE;
	}
	GetFileRow("config.ini", buf, 10);
	nShare = atol(buf);
	GetFileRow("config.ini", buf, 13);
	by.width = (short)atol(buf);
	GetFileRow("config.ini", buf, 15);
	by.width_ratio = (float)atof(buf);
	GetFileRow("config.ini", buf, 17);
	by.heighet = atol(buf);
#ifdef  UNICOM_CCID_19
	szCcid19 = _T("");
	GetFileRow("config.ini", buf, 26);
	cNumCard = buf[0];
	GetFileRow("config.ini", buf, 28);
	cFreeCard = buf[0];
#endif//UNICOM_CCID_19
	GetFileRow("config.ini", buf, 36);
	m_bLock     = TRUE;
	m_typeprint = 0;

	m_strbuyer     = theApp.M_STRBUYER;
	M_STRCARDTYPE  = theApp.M_STRCARDTYPE;
	M_STRCHECTYPE  = theApp.M_STRCHECTYPE;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCompareDlg::~CCompareDlg()
{
	free(buf);
	free(OpenLink);
	m_adodb.Close();
}

void CCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_input);
	DDV_MaxChars(pDX, m_input, 11);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
	DDX_Control(pDX, IDC_COMBO1, m_CtrlAotu);
	DDX_Control(pDX, IDC_COMBO3, m_CtrlSimtype);
	DDX_Text(pDX, IDC_EDIT3, m_strbuyer);
	DDX_Control(pDX, IDC_BUTTON_INPUT, m_btInput);
}


BEGIN_MESSAGE_MAP(CCompareDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchangeCombo3)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_EN_CHANGE(IDC_EDIT3, OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CCompareDlg::OnBnClickedButtonInput)
END_MESSAGE_MAP()


// CCompareDlg message handlers


void CCompareDlg::PrintTest()
{
	char PrintCmd[512] = "";

	int strlen = 0;

	strlen = CreateBACodeItemALine(PrintCmd,
		"13800138000",
		"TELNUM",
		"012345678901234",
		"IMEI",
		"01234567890123456789",
		"CCID",
		font,
		by,
		X_coordinate,
		Y_coordinate,
		OverBarcode,
		nShare);
	ExecutePrintCmdInLpt(PrintCmd);

	return;
}

BOOL  CCompareDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) 
		return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE; 
	else 
		return CDialog::PreTranslateMessage(pMsg);

}

BOOL CCompareDlg::OnInitDialog()
{
	CString szVer(_T("DESAYELE  GSM机卡一体话机校验打印-V"));
	//CString strAboutMenu;

	CDialog::OnInitDialog();

	szVer += STRFILEVER;

	SetWindowText(szVer);

	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//连接数据库
	sprintf(OpenLink, "Provider=Microsoft.Jet.OLEDB.4.0; Data Source = %s", theApp.M_STRDBSRC);
	if (!m_adodb.Open(OpenLink)){
		MessageBox("无法连接数据库！", "错误", MB_OK|MB_ICONERROR);
		OnCancel();
		exit(0);
		return FALSE;
	}

	{
		m_CtrlAotu.AddString("1、按钮校验");
		m_CtrlAotu.AddString("2、触发校验");
		if (M_STRCHECTYPE == _T("TRUE")){
			m_CtrlAotu.SetCurSel(1);			
		}else{
			m_CtrlAotu.SetCurSel(0);
		}
		m_CtrlAotu.EnableWindow(FALSE);
		
		m_CtrlSimtype.AddString("1、号卡");
		m_CtrlSimtype.AddString("2、白卡");
		m_CtrlSimtype.SetCurSel(0);
		m_CtrlSimtype.EnableWindow(FALSE);
		m_typeprint = 1;
	}

	//m_edit.SubclassDlgItem(IDC_EDIT2, this);
	m_edit.bkColor(RGB(18, 190, 233));
	m_edit.setFont(28);
#ifndef  UNICOM_CCID_19
	m_edit.SetWindowText(_T("只需要在上面的输入框内扫描入手机号码,\r\n本框显示操作结果\r\n\n绿色表示OK   红色表示错误"));
#else
	m_edit.SetWindowText(_T("只需要在上面的输入框内扫描入手机号码,\r\n本框显示操作结果\r\n\n绿色表示OK   红色表示错误\r\n       ※GSM169(B)V60※\r\n      ※CCID只校验前19位※"));
#endif//UNICOM_CCID_19

	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_INPUT)->EnableWindow(FALSE);

	m_ctrlbuyer.SubclassDlgItem(IDC_EDIT3, this);
	m_ctrlbuyer.textColor(RGB(255, 0, 0));
	m_ctrlbuyer.setFont(28);
	m_ctrlbuyer.EnableWindow(FALSE);

	if (M_STRCARDTYPE == _T("FALSE")){
		m_typecar   = NON_NORMALTYPE_CAR;
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		m_CtrlSimtype.SetCurSel(NON_NORMALTYPE_CAR);
		m_CtrlAotu.SetCurSel(NON_NORMALTYPE_PRINT);
	}else{
		m_typecar   = NORMALTYPE_CAR;
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		m_CtrlSimtype.SetCurSel(NORMALTYPE_CAR);
		m_CtrlAotu.SetCurSel(NORMALTYPE_PRINT);
	}

	GetDlgItem(IDC_BUTTON6)->ShowWindow(FALSE);
	
	return TRUE;
}

void CCompareDlg::OnEnChangeEdit1()
{
	// TODO:  Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	while(11 == m_input.GetLength())
	{
		char  PrintCmd[HALF_KILO*2] = "";
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

		//定义一个字符串，专门用于获取话机的信息
		char telnum[12] = "",
			 modenccid[21] = "",
			 modenimei[16] = "";
		char strSQL[255] = "";
		CString Report(""),Value("");
		BOOL bCOM = OpenComm(CStrPortTranInt(theApp.M_STRPORT),CStrBateTranInt(theApp.M_STRRATE));
        
		if (!bCOM){
				Report.Format(_T("打开串口失败，无法校验！\r\nPORT = %s, RATE = %s"), theApp.M_STRPORT, theApp.M_STRRATE);				
				MessageBox(Report, _T("提示"), MB_OK|MB_ICONSTOP);
				m_edit.SetWindowText("错误：打开串口失败，无法校验！");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				goto EXIT;
			}

		if (!gsminit()){
			m_edit.SetWindowText("错误:串口未准备好！\r\n解决:接好串口线、激活话机");
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		strcpy(telnum, m_input);
		sprintf(strSQL, 
			    "SELECT HANDLED FROM DataCompare WHERE TELNUM = '%s'", //SQL语句
				m_input);
		m_adodb.ExecuteQueryValue(strSQL, Value);
		if(Value == "TRUE"){
			m_edit.SetWindowText("错误:该号码已经校验过!\r\n解决:请扫描下一个");
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			goto EXIT;
		}
		strcpy(strSQL, "");
		sprintf(strSQL, 
			    "SELECT CCID FROM DataCompare WHERE TELNUM = '%s'", //SQL语句
				m_input);
		m_adodb.ExecuteQueryValue(strSQL, Value);
		strcpy(strSQL, "");
		if(Value.IsEmpty())
		{//
			m_edit.SetWindowText("错误:数据库中没有找到刚输入的手机号码\r\n\n解决:请确认数据库中是否存在此号码");
			m_edit.bkColor(RGB(255, 0, 0));
		}
		else if (20 != Value.GetLength())
		{//
			m_edit.SetWindowText("错误:不能够获取与手机号码相匹配的CCID号\r\n解决:请确认数据库中是否存在与刚扫入的手机号码相匹配的CCID号");
			m_edit.bkColor(RGB(255, 0, 0));
			//MessageBox("不能够获取与手机号码相匹配的CCID号！", "错误", MB_OK);
		}
		else
		{//			

			if (20 != GetRfCCID(modenccid)){				
				m_edit.SetWindowText("错误:未能够从话机中获取CCID\r\n\n解决:接好串口线、激活话机");
				//MessageBox(modenccid);
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("未能够从话机中获取CCID！", "错误", MB_OK);
				goto EXIT;//未能够从话机中获取CCID
			}
#ifndef  UNICOM_CCID_19
			if (Value != modenccid){
				m_edit.SetWindowText("错误:话机的CCID号与数据库中的不相同!\r\n\n解决:请确认数据库的数据是否有错");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("话机的CCID号与数据库中的不匹配！", "错误", MB_OK);
				goto EXIT;//话机的CCID号与数据库中的不匹配
			}
#else//联通CCID号段只要校验前19位
			szCcid19 = modenccid;
			if (Value.Left(19) != szCcid19.Left(19)){
				m_edit.SetWindowText("错误:话机的CCID号的前19位与数据库中的前19位不相同!\r\n\r\n解决:请确认数据库的数据是否有错");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("话机的CCID号与数据库中的不匹配！", "错误", MB_OK);
				goto EXIT;//话机的CCID号与数据库中的不匹配
			}
#endif//UNICOM_CCID_19
			if (15 != GetRfIMEI(modenimei)){
				m_edit.SetWindowText("错误:未能够从话机中获取IMEI\r\n解决:接好串口线、激活话机");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("未能够从话机中获取IMEI！", "错误", MB_OK);
				goto EXIT;//未能够从话机中获取IMEI
			}

			if (!IsAlphaString(modenimei)){
				m_edit.SetWindowText("错误:获取的IMEI号有乱码\r\n解决:接好串口线、激活话机");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("未能够从话机中获取IMEI！", "错误", MB_OK);
				goto EXIT;//未能够从话机中获取IMEI
			}

			{//检测数据库中是否已经存在此IMEI号
				Value.Empty();
				sprintf(strSQL, 
					"SELECT CCID FROM DataCompare WHERE IMEI = '%s'", //SQL语句
					modenimei);
				m_adodb.ExecuteQueryValue(strSQL, Value);
				if (Value.GetLength() == 20)
				{
					Report.Format("错误:数据库中已经存在该IMEI号：%s，\r\n解决:无法校验此话机！", modenimei);
					m_edit.SetWindowText(Report);
					m_edit.bkColor(RGB(255, 0, 0));
					CloseComm();
					goto EXIT;
				}
#ifndef  UNICOM_CCID_19
				sprintf(strSQL, 
					"SELECT SERIAL FROM DataCompare WHERE CCID = '%s'", //SQL语句
					modenccid);
#else
				sprintf(strSQL, 
					"SELECT SERIAL FROM DataCompare WHERE CCID LIKE '%s_'", //SQL语句
					szCcid19.Left(19));
#endif//UNICOM_CCID_19
				m_adodb.ExecuteQueryValue(strSQL, Value);
                
				if (Value.GetLength() == 0)
				{
#ifndef UNICOM_CCID_19
					Report.Format("错误:数据库中没有此CCID号：%s，\r\n解决:无法校验此话机！", modenccid);
#else
					Report.Format("错误:数据库中没有此CCID号：%s_，\r\n解决:无法校验此话机！", szCcid19.Left(19));
#endif//UNICOM_CCID_19
					m_edit.SetWindowText(Report);
					m_edit.bkColor(RGB(255, 0, 0));
					Value.Empty();
					CloseComm();
					return;
				}                
				Value.Empty();
			}
#ifndef  UNICOM_CCID_19
			sprintf(strSQL, 
			        "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID = '%s'", //SQL语句
					modenimei, m_strbuyer, m_input, modenccid);
#else
			sprintf(strSQL, 
			        "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID LIKE '%s_'", //SQL语句
					modenimei, m_strbuyer, m_input, szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_adodb.Execute(strSQL);
			//采用Code 93 编码打印三个连续的条码，说明和条码数据同一行
#ifdef UNICOM_CCID_19
			modenccid[19] = cNumCard;
#endif//UNICOM_CCID_19
			CreateBACodeItemALine(PrintCmd,
                    		      telnum,
								  "TELNUM",
								  modenimei,
								  "IMEI",
								  modenccid,
								  "CCID",
								  font,
								  by,
								  X_coordinate,
								  Y_coordinate,
								  OverBarcode,
								  nShare);

			ExecutePrintCmdInLpt(PrintCmd, "LPT1");
			Report.Format("TELNUM:%s\r\nIMEI:%s\r\nCCID:%s",telnum,modenimei,modenccid);
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(0, 255, 0));
		}
EXIT:	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		m_input.Empty();		
		GetDlgItem(IDC_EDIT1)->SetFocus();
		CloseComm();
		break;
	}
	CDialog::UpdateData(FALSE);
	CloseComm();
}

void CCompareDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	char  PrintCmd[HALF_KILO*3] = "";    
	//定义一个字符串，专门用于获取话机的信息
	char telnum[16] = "",
		 modenccid[25] = "",
		 modenimei[18] = "";
	char strSQL[512] = "";
	CString  Report,SqlValue("");

	BOOL bCOM = OpenComm(CStrPortTranInt(theApp.M_STRPORT),CStrBateTranInt(theApp.M_STRRATE));

	if (!bCOM){
		Report.Format(_T("打开串口失败！\r\nPORT = %s, RATE = %s"), theApp.M_STRPORT, theApp.M_STRRATE);
		MessageBox(Report, _T("提示"), MB_OK|MB_ICONSTOP);
		return;
	}

	if (!gsminit()){
		m_edit.SetWindowText("错误:串口未准备好！\r\n解决:接好串口线、激活话机");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (20 != GetRfCCID(modenccid))
	{//首先读取话机的CCID
		m_edit.SetWindowText("错误:未能够从话机中获取CCID\r\n\n解决:接好串口线、激活话机");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (!IsAlphaString(modenccid)){
		m_edit.SetWindowText("错误:读取CCID号错误！\r\n解决：确保话机的串口返回OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}
#ifdef UNICOM_CCID_19
	szCcid19 = modenccid;
#endif//UNICOM_CCID_19
	if (15 != GetRfIMEI(modenimei))
	{
		m_edit.SetWindowText("错误:未能够从话机中获取IMEI\r\n解决:接好串口线、激活话机");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (!IsAlphaString(modenimei)){
		m_edit.SetWindowText("错误:读取IMEI号错误！\r\n解决：确保话机的串口返回OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	{//检测数据库中是否已经存在此IMEI号
		SqlValue.Empty();
		sprintf(strSQL, 
			"SELECT CCID FROM DataCompare WHERE IMEI = '%s'", //SQL语句
			modenimei);
		m_adodb.ExecuteQueryValue(strSQL, SqlValue);
		if (SqlValue.GetLength() == 20)
		{
			Report.Format("错误:数据库中已经存在该IMEI号：%s，\r\n解决:无法校验此话机！", modenimei);
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			SqlValue.Empty();
			CloseComm();
			return;
		}
#ifndef UNICOM_CCID_19
		sprintf(strSQL, 
			"SELECT SERIAL FROM DataCompare WHERE CCID = '%s'", //SQL语句
			modenccid);
#else
		sprintf(strSQL, 
			"SELECT SERIAL FROM DataCompare WHERE CCID LIKE '%s_'", //SQL语句
			szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_adodb.ExecuteQueryValue(strSQL, SqlValue);

		if (SqlValue.GetLength() == 0)
		{
#ifndef UNICOM_CCID_19
			Report.Format("错误:数据库中没有此CCID号：%s，\r\n解决:无法校验此话机！", modenccid);
#else
			Report.Format("错误:数据库中没有此CCID(19)号：%s_，\r\n解决:无法校验此话机！", szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			SqlValue.Empty();
			CloseComm();
			return;
		}
		
		SqlValue.Empty();
	}
	switch(m_typecar)
	{//按照卡的类型做操作
	case NORMALTYPE_CAR://带号卡的情况
		if (20 != (int)strlen(modenccid)){
			Report.Format("错误：读出的CCID号长度有误，无法校验！\r\n\n解决：重新校验");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		if (!IsAlphaString(modenccid)){
			Report.Format("错误：读出的CCID号格式有误，无法校验！\r\n\n解决：重新校验");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
#ifndef UNICOM_CCID_19
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID = '%s'", //SQL语句查询与CCID匹配的TELNUM
				modenccid);
#else
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID LIKE '%s_'", //SQL语句查询与CCID匹配的TELNUM
				szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_adodb.ExecuteQueryValue(strSQL,  SqlValue);
		if (SqlValue.GetLength() != 11)
		{
#ifndef UNICOM_CCID_19
			Report.Format("错误：未能够从数据库中获取与话机CCID号匹配的TELNUM\r\n解决：请检查该卡是否为白卡，或数据库中是否存在该CCID号:%s",
				modenccid);
#else
			Report.Format("错误：未能够从数据库中获取与话机CCID号匹配的TELNUM\r\n解决：请检查该卡是否为白卡，或数据库中是否存在该CCID号:%s_",
				szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		else if (!CStrIsNumber(SqlValue))
		{
#ifndef UNICOM_CCID_19
			Report.Format("错误：出现乱码\r\n\n解决：请检查数据库中与CCID号[%s]匹配的TELNUM是否为乱码", 
				modenccid);
#else
			Report.Format("错误：出现乱码\r\n\r\n解决：请检查数据库中与CCID号[%s_] 匹配的TELNUM是否为乱码", 
				szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		else
		{
#ifndef UNICOM_CCID_19
			sprintf(strSQL,
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID = '%s'", //SQL语句
				    modenimei, m_strbuyer, SqlValue, modenccid);
#else
			sprintf(strSQL,
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID LIKE '%s_'", //SQL语句
				    modenimei, m_strbuyer, SqlValue, szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_adodb.Execute(strSQL);
#ifndef UNICOM_CCID_19
			Report.Format("TELNUM:%s\r\nIMEI:%s\r\nCCID:%s",
				SqlValue,modenimei,modenccid);
#else
			Report.Format("TELNUM:%s\r\nIMEI:%s\r\nCCID:%s_",
				SqlValue,modenimei,szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(0, 255, 0));
			//采用Code 93 编码打印三个连续的条码，说明和条码数据同一行
			strcpy(telnum, SqlValue);
#ifdef UNICOM_CCID_19
			modenccid[19] = cNumCard;
#endif//UNICOM_CCID_19
			CreateBACodeItemALine(PrintCmd,
                    		      telnum,
								  "TELNUM",
								  modenimei,
								  "IMEI",
								  modenccid,
								  "CCID",
								  font,
								  by,
								  X_coordinate,
								  Y_coordinate,
								  OverBarcode,
								  nShare);
			ExecutePrintCmdInLpt(PrintCmd, "LPT1");
		}
		CloseComm();
		break;
	case NON_NORMALTYPE_CAR://无号卡的情况
#ifndef UNICOM_CCID_19
		sprintf(strSQL,
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s',BUYER = '%s' WHERE CCID = '%s'", //SQL语句
				    modenimei, m_strbuyer, modenccid);
		m_adodb.Execute(strSQL);
			Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s",modenimei,modenccid);
#else
		sprintf(strSQL,
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s',BUYER = '%s' WHERE CCID LIKE '%s_'", //SQL语句
				    modenimei, m_strbuyer, szCcid19.Left(19));
		m_adodb.Execute(strSQL);
		Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s_",modenimei,szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_edit.SetWindowText(Report);
		m_edit.bkColor(RGB(0, 255, 0));
		//采用Code 93 编码打印两个连续的条码，说明和条码数据同一行
#ifdef UNICOM_CCID_19
		modenccid[19] = cFreeCard;
#endif//UNICOM_CCID_19
		CreateBACodeItemALine(PrintCmd,
			                  modenimei,
							  "IMEI",
							  modenccid,
							  "CCID",
							  font,
							  by,
							  X_coordinate,
							  Y_coordinate + font.height + 12,
							  OverBarcode,
							  nShare);
		ExecutePrintCmdInLpt(PrintCmd, "LPT1");
		CloseComm();
		break;
	default:
		MessageBox("严重错误：无法识别的卡类型参数", "提示", MB_OK|MB_ICONERROR);
		CloseComm();
		break;
	}
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CPasswordDlg  PasswordDlg;
	if (m_bLock == FALSE)
	{
		m_bLock = !m_bLock;
		this->GetDlgItem(IDC_BUTTON3)->SetWindowText("解锁配置");
		this->GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_INPUT)->EnableWindow(FALSE);
		m_CtrlAotu.EnableWindow(FALSE);
		m_CtrlSimtype.EnableWindow(FALSE);
		m_ctrlbuyer.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON6)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->ShowWindow(TRUE);
	}
	else
	{
		if (PasswordDlg.DoModal() == IDOK)
		{
			PasswordDlg.m_strpw.MakeUpper();
			if (PasswordDlg.m_strpw == "DESAYELE"||
				PasswordDlg.m_strpw == "ERROR")
			{
				m_bLock = !m_bLock;
				this->GetDlgItem(IDC_BUTTON3)->SetWindowText("锁定配置");
				this->GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
				this->GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
				this->GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
				this->GetDlgItem(IDC_COMBO3)->EnableWindow(TRUE);
				m_CtrlAotu.EnableWindow(TRUE);
				m_CtrlSimtype.EnableWindow(TRUE);
				m_ctrlbuyer.EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON6)->ShowWindow(TRUE);
				GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
				GetDlgItem(IDC_BUTTON_INPUT)->EnableWindow(TRUE);
			}
			else
			{
				MessageBox("密码错误！", "提示", MB_OK|MB_ICONERROR);
			}
		}		
	}
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	m_typeprint = m_CtrlAotu.GetCurSel();
	switch(m_typeprint)
	{
	case NORMALTYPE_PRINT:
		theApp.M_STRCHECTYPE = _T("TRUE");
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		break;
	case NON_NORMALTYPE_PRINT:
		theApp.M_STRCHECTYPE = _T("FALSE");
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		break;
	default:
		MessageBox("严重错误：没有该打印选项", "提示", MB_OK|MB_ICONERROR);
		break;
	}
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnCbnSelchangeCombo3()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	m_typecar = m_CtrlSimtype.GetCurSel();
	switch(m_typecar)
	{
	case NORMALTYPE_CAR:
		theApp.M_STRCARDTYPE = _T("TRUE");
		m_CtrlAotu.EnableWindow(TRUE);
		if (m_typeprint == NORMALTYPE_PRINT)
		{
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
			m_CtrlAotu.SetCurSel(NORMALTYPE_PRINT);
		}
		else
		{
			GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
			m_CtrlAotu.SetCurSel(NON_NORMALTYPE_PRINT);
		}
		break;
	case NON_NORMALTYPE_CAR:
		theApp.M_STRCARDTYPE = _T("FALSE");
		m_CtrlAotu.SetCurSel(0);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		m_CtrlAotu.EnableWindow(FALSE);
		break;
	default:
		MessageBox("严重错误：没有该卡类型选项", "提示", MB_OK|MB_ICONERROR);
		break;
	}
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnBnClickedButton5()
{//IN
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CString  pathname  = _T(""),
		     sheetname = _T("");
	char    *pSQL = (char *)malloc(sizeof(char) * 512);
	CString ccid,
		    Value,
			errmsg;
	CImportDialog  dlg;
	if (IDOK == dlg.DoModal())
	{
		int  row     = 1;
		int  PreserveLen = 0;
		int  TelCol  = dlg.M_INTTELCOL;
		int  CcidCol = dlg.M_INTCCIDCOL; 
		pathname  = dlg.m_strpathname;
		sheetname = dlg.m_strSheetname;
		if (TelCol == 0){
			MessageBox(_T("列数必须是从 1 开始的！"), _T("提示"), MB_OK|MB_ICONSTOP);
			return;
		}else if (CcidCol == 0){
			MessageBox(_T("列数必须是从 1 开始的！"), _T("提示"), MB_OK|MB_ICONSTOP);
			return;
		}else if (TelCol == CcidCol){
			MessageBox(_T("电话号码和CCID号是不能够在同一列的！"), _T("提示"), MB_OK|MB_ICONSTOP);
			return;
		}else if (TelCol + CcidCol == 0){
			MessageBox(_T("列数必须是从是正数！"), _T("提示"), MB_OK|MB_ICONSTOP);
			return;
		}else{
			CSpreadSheet SS(pathname, sheetname, 0);
			SS.ReadCell(Value, TelCol, row++); // 读取电话号码 第一行
			SS.ReadCell(Value, TelCol, row);   // 读取电话号码 第二行
			while(!Value.IsEmpty())
			{
				errmsg      = Value;
				PreserveLen = PreserveString(errmsg, 0);
				Value  = errmsg.Left(11);
				if (Value.GetLength() != 11){
					errmsg.Format("无法识别此格式的电话号码：%s PreserveLen = %d [行：%d，列：%d]", Value, PreserveLen, row, TelCol);
					MessageBox(errmsg, _T("导入数据失败"), MB_OK|MB_ICONSTOP);
					return;
				}
				//Value  = errmsg;

				SS.ReadCell(ccid, CcidCol, row);
				errmsg      = ccid;
				PreserveLen = PreserveString(errmsg, 4);
				ccid        = errmsg.Left(20);
				if (ccid.GetLength() != 20){
					errmsg.Format("无法识别此格式的CCID号：%s [行：%d，列：%d]", ccid, row, CcidCol);
					MessageBox(errmsg, _T("导入数据失败"), MB_OK|MB_ICONSTOP);
					return;
				}
				//ccid  = errmsg;

				sprintf(pSQL, "INSERT INTO DataCompare (CCID, TELNUM, AREA) \
							  VALUES('%s', '%s', '%s')", ccid.Left(20), Value.Left(11), sheetname);
				m_adodb.Execute(pSQL);
				row++;
				Value.Empty();
				SS.ReadCell(Value, TelCol, row);
			}
		}
		errmsg.Format("成功导入 %d 条记录！", row-2);
		MessageBox(errmsg, _T("提示"), MB_OK);
	}
	free(pSQL);	
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnBnClickedButton4()
{//OUT
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CString   string = _T("");
	COutportDlg dlg;
	FILE *fp;
	fp = fopen("SQL.TXT", "w");
	CString outfilename = _T(""),
		    filenameEx  = _T(""),
		    sql         = "SELECT ",
			condition   = "WHERE";
	if (IDOK == dlg.DoModal())
	{//点击了"OK"键
		string.Format("你确定新的表名：[%s] \r\n在数据源： \r\n[%s] \r\n中没有重复吗?", dlg.m_strnewtablename, theApp.M_STRDBSRC);
		if (IDYES == MessageBox(string, _T("询问"), MB_YESNO|MB_ICONQUESTION))
		{//点击了"YES"键
			sql = sql + " SERIAL";               
				if (dlg.m_bTelnum){     
					sql = sql + ", TELNUM";
				}
				if (dlg.m_bImei){
					sql = sql + ", IMEI"; 
				} 
				if (dlg.m_bCcid){
					sql = sql + ", CCID"; 
				}
				if (dlg.m_bBuyer){
					sql = sql + ", BUYER";
				}
				if (dlg.m_bArea){
					sql = sql + ", AREA";
				}
				//**************************************************************
				sql = sql + " INTO "; 
				sql = sql + dlg.m_strnewtablename;
				sql = sql + " FROM  DataCompare ";
				if (dlg.m_bcCheck){                                      //根
					switch(dlg.m_iSelCheck){                             //据
						case 0:                                          //导
							condition = condition + " HANDLED = FALSE";  //出
							break;                                       //选
						case 1:                                          //项
							condition = condition + " HANDLED = TRUE";   //对
							break;                                       //话
						default:                                         //框
							condition = condition + " HANDLED = TRUE";   //组
							break;                                       //织
					}                                                    //一
				}                                                        //条
				if (dlg.m_bcBuyer){                                      // S
					condition = condition + " AND BUYER = '";            // Q
					condition = condition + dlg.m_strbcBuyer;            // L
					condition = condition + "'";                         //查
				}                                                        //询
				if (dlg.m_bcArea){                                       //语
					condition = condition + " AND AREA = '";             //句
					condition = condition + dlg.m_strbcArea;             //
					condition = condition + "'";                         //
				}                                                        //
				if (dlg.m_bcBuyer||dlg.m_bcArea||dlg.m_bcCheck){
					sql = sql + condition; 
				}else{
				}
				//*****************************************************************
				//增加一个新表到数据库
				if (FALSE == m_adodb.Execute(sql)){
					MessageBox(_T("导出数据失败，请检查指定的表名在数据库中有没有重复！"), _T(" 导出提示"), MB_OK|MB_ICONSTOP);
				}else{
					string.Format("成功在数据源：\r\n[%s]\r\n中增加表格[%s]", theApp.M_STRDBSRC, dlg.m_strnewtablename);
					MessageBox(string, _T(" 导出提示"), MB_OK);
				}
				fprintf(fp, "%s\r", sql);
		}//点击了"YES"键
	}//点击了"OK"键
	fclose(fp);
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnEnChangeEdit3()
{
	CDialog::UpdateData(TRUE);
	theApp.M_STRBUYER = m_strbuyer;
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	char  PrintCmd[HALF_KILO*3] = "";    
	//定义一个字符串，专门用于获取话机的信息
	char telnum[16] = "",
		 modenccid[25] = "",
		 modenimei[18] = "";
	char strSQL[512] = "";
	CString  Report,SqlValue("");

	BOOL bCOM = OpenComm(CStrPortTranInt(theApp.M_STRPORT),CStrBateTranInt(theApp.M_STRRATE));

	if (!bCOM){
		Report.Format(_T("打开串口失败！\r\nPORT = %s, RATE = %s"), theApp.M_STRPORT, theApp.M_STRRATE);
		MessageBox(Report, _T("提示"), MB_OK|MB_ICONSTOP);
		return;
	}

	if (!gsminit()){
		m_edit.SetWindowText("错误:串口未准备好！\r\n解决:接好串口线、激活话机");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (20 != GetRfCCID(modenccid))
	{//首先读取话机的CCID
		m_edit.SetWindowText("错误:未能够从话机中获取CCID\r\n\n解决:接好串口线、激活话机");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}
#ifdef UNICOM_CCID_19
	szCcid19 = modenccid;
#endif//UNICOM_CCID_19
	if (!IsAlphaString(modenccid)){
		m_edit.SetWindowText("错误:读取CCID号错误！\r\n解决：确保话机的串口返回OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (15 != GetRfIMEI(modenimei))
	{
		m_edit.SetWindowText("错误:未能够从话机中获取IMEI\r\n解决:接好串口线、激活话机");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (!IsAlphaString(modenimei)){
		m_edit.SetWindowText("错误:读取IMEI号错误！\r\n解决：确保话机的串口返回OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	switch(m_typecar)
	{//按照卡的类型做操作
	case NORMALTYPE_CAR://带号卡的情况
		if (20 != (int)strlen(modenccid)){
			Report.Format("错误：读出的CCID号长度有误，无法校验！\r\n\n解决：重新校验");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		if (!IsAlphaString(modenccid)){
			Report.Format("错误：读出的CCID号格式有误，无法校验！\r\n\n解决：重新校验");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
#ifndef UNICOM_CCID_19
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID = '%s'", //SQL语句查询与CCID匹配的TELNUM
				modenccid);
#else		
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID LIKE '%s_'", //SQL语句查询与CCID匹配的TELNUM
				szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_adodb.ExecuteQueryValue(strSQL,  SqlValue);
		if (SqlValue.GetLength() != 11)
		{
#ifndef UNICOM_CCID_19
			Report.Format("错误：未能够从数据库中获取与话机CCID号匹配的TELNUM\r\n解决：请检查该卡是否为白卡，或数据库中是否存在该CCID号:%s", 
				modenccid);
#else
			Report.Format("错误：未能够从数据库中获取与话机CCID号匹配的TELNUM\r\n解决：请检查该卡是否为白卡，或数据库中是否存在该CCID号:%s_", 
				szCcid19.Left(19));
#endif//ifndef UNICOM_CCID_19
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		else if (!CStrIsNumber(SqlValue))
		{
#ifndef UNICOM_CCID_19
			Report.Format("错误：出现乱码\r\n解决：请检查数据库中与CCID号[%s]匹配的TELNUM是否为乱码", 
				modenccid);
#else
			Report.Format("错误：出现乱码\r\n解决：请检查数据库中与CCID号[%s_]匹配的TELNUM是否为乱码", 
				szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		else
		{
#ifndef UNICOM_CCID_19
			Report.Format("TELNUM:%s\r\nIMEI:%s\r\nCCID:%s",
                                              SqlValue, modenimei, modenccid);
#else
			Report.Format("TELNUM:%s\r\nIMEI:%s\r\nCCID:%s_",
				                              SqlValue, modenimei, szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(0, 255, 0));
			strcpy(telnum, SqlValue);
#ifdef UNICOM_CCID_19
			modenccid[19] = cNumCard;
#endif//UNICOM_CCID_19
			CreateBACodeItemALine(PrintCmd,
                    		      telnum,
								  "TELNUM",
								  modenimei,
								  "IMEI",
								  modenccid,
								  "CCID",
								  font,
								  by,
								  X_coordinate,
								  Y_coordinate,
								  OverBarcode,
								  nShare);
			ExecutePrintCmdInLpt(PrintCmd, "LPT1");
		}
		CloseComm();
		break;
	case NON_NORMALTYPE_CAR://无号卡的情况
#ifndef UNICOM_CCID_19
		Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s",modenimei,modenccid);
#else
		szCcid19 = modenccid;
		Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s_",modenimei,szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_edit.SetWindowText(Report);
		m_edit.bkColor(RGB(0, 255, 0));
			//采用Code 93 编码打印两个连续的条码，说明和条码数据同一行
#ifdef UNICOM_CCID_19
		modenccid[19] = cFreeCard;
#endif//UNICOM_CCID_19
		CreateBACodeItemALine(PrintCmd,
			                  modenimei,
							  "IMEI",
							  modenccid,
							  "CCID",
							  font,
							  by,
							  X_coordinate,
							  Y_coordinate + font.height + 12,
							  OverBarcode,
							  nShare);
		ExecutePrintCmdInLpt(PrintCmd, "LPT1");
		CloseComm();
		break;
	default:
		MessageBox("严重错误：无法识别的卡类型参数", "提示", MB_OK|MB_ICONERROR);
		CloseComm();
		break;
	}
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnBnClickedButtonInput()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgInvalidCardPrint DlgInvalidCardPrint;

	// 打印坐标部分
	DlgInvalidCardPrint.m_font         = font;
	DlgInvalidCardPrint.m_by           = by;
	DlgInvalidCardPrint.m_X_coordinate = X_coordinate;
	DlgInvalidCardPrint.m_Y_coordinate = Y_coordinate;
	DlgInvalidCardPrint.m_OverBarcode  = OverBarcode;
	DlgInvalidCardPrint.m_nShare       = nShare;
	DlgInvalidCardPrint.m_pAdodb       = &m_adodb;
	DlgInvalidCardPrint.m_szBuyer      = m_strbuyer;

	DlgInvalidCardPrint.DoModal();
}
