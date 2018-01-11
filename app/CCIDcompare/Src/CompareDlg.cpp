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
	CString szVer(_T("DESAYELE  GSM����һ�廰��У���ӡ-V"));
	//CString strAboutMenu;

	CDialog::OnInitDialog();

	szVer += STRFILEVER;

	SetWindowText(szVer);

	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//�������ݿ�
	sprintf(OpenLink, "Provider=Microsoft.Jet.OLEDB.4.0; Data Source = %s", theApp.M_STRDBSRC);
	if (!m_adodb.Open(OpenLink)){
		MessageBox("�޷��������ݿ⣡", "����", MB_OK|MB_ICONERROR);
		OnCancel();
		exit(0);
		return FALSE;
	}

	{
		m_CtrlAotu.AddString("1����ťУ��");
		m_CtrlAotu.AddString("2������У��");
		if (M_STRCHECTYPE == _T("TRUE")){
			m_CtrlAotu.SetCurSel(1);			
		}else{
			m_CtrlAotu.SetCurSel(0);
		}
		m_CtrlAotu.EnableWindow(FALSE);
		
		m_CtrlSimtype.AddString("1���ſ�");
		m_CtrlSimtype.AddString("2���׿�");
		m_CtrlSimtype.SetCurSel(0);
		m_CtrlSimtype.EnableWindow(FALSE);
		m_typeprint = 1;
	}

	//m_edit.SubclassDlgItem(IDC_EDIT2, this);
	m_edit.bkColor(RGB(18, 190, 233));
	m_edit.setFont(28);
#ifndef  UNICOM_CCID_19
	m_edit.SetWindowText(_T("ֻ��Ҫ��������������ɨ�����ֻ�����,\r\n������ʾ�������\r\n\n��ɫ��ʾOK   ��ɫ��ʾ����"));
#else
	m_edit.SetWindowText(_T("ֻ��Ҫ��������������ɨ�����ֻ�����,\r\n������ʾ�������\r\n\n��ɫ��ʾOK   ��ɫ��ʾ����\r\n       ��GSM169(B)V60��\r\n      ��CCIDֻУ��ǰ19λ��"));
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

		//����һ���ַ�����ר�����ڻ�ȡ��������Ϣ
		char telnum[12] = "",
			 modenccid[21] = "",
			 modenimei[16] = "";
		char strSQL[255] = "";
		CString Report(""),Value("");
		BOOL bCOM = OpenComm(CStrPortTranInt(theApp.M_STRPORT),CStrBateTranInt(theApp.M_STRRATE));
        
		if (!bCOM){
				Report.Format(_T("�򿪴���ʧ�ܣ��޷�У�飡\r\nPORT = %s, RATE = %s"), theApp.M_STRPORT, theApp.M_STRRATE);				
				MessageBox(Report, _T("��ʾ"), MB_OK|MB_ICONSTOP);
				m_edit.SetWindowText("���󣺴򿪴���ʧ�ܣ��޷�У�飡");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				goto EXIT;
			}

		if (!gsminit()){
			m_edit.SetWindowText("����:����δ׼���ã�\r\n���:�Ӻô����ߡ������");
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		strcpy(telnum, m_input);
		sprintf(strSQL, 
			    "SELECT HANDLED FROM DataCompare WHERE TELNUM = '%s'", //SQL���
				m_input);
		m_adodb.ExecuteQueryValue(strSQL, Value);
		if(Value == "TRUE"){
			m_edit.SetWindowText("����:�ú����Ѿ�У���!\r\n���:��ɨ����һ��");
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			goto EXIT;
		}
		strcpy(strSQL, "");
		sprintf(strSQL, 
			    "SELECT CCID FROM DataCompare WHERE TELNUM = '%s'", //SQL���
				m_input);
		m_adodb.ExecuteQueryValue(strSQL, Value);
		strcpy(strSQL, "");
		if(Value.IsEmpty())
		{//
			m_edit.SetWindowText("����:���ݿ���û���ҵ���������ֻ�����\r\n\n���:��ȷ�����ݿ����Ƿ���ڴ˺���");
			m_edit.bkColor(RGB(255, 0, 0));
		}
		else if (20 != Value.GetLength())
		{//
			m_edit.SetWindowText("����:���ܹ���ȡ���ֻ�������ƥ���CCID��\r\n���:��ȷ�����ݿ����Ƿ�������ɨ����ֻ�������ƥ���CCID��");
			m_edit.bkColor(RGB(255, 0, 0));
			//MessageBox("���ܹ���ȡ���ֻ�������ƥ���CCID�ţ�", "����", MB_OK);
		}
		else
		{//			

			if (20 != GetRfCCID(modenccid)){				
				m_edit.SetWindowText("����:δ�ܹ��ӻ����л�ȡCCID\r\n\n���:�Ӻô����ߡ������");
				//MessageBox(modenccid);
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("δ�ܹ��ӻ����л�ȡCCID��", "����", MB_OK);
				goto EXIT;//δ�ܹ��ӻ����л�ȡCCID
			}
#ifndef  UNICOM_CCID_19
			if (Value != modenccid){
				m_edit.SetWindowText("����:������CCID�������ݿ��еĲ���ͬ!\r\n\n���:��ȷ�����ݿ�������Ƿ��д�");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("������CCID�������ݿ��еĲ�ƥ�䣡", "����", MB_OK);
				goto EXIT;//������CCID�������ݿ��еĲ�ƥ��
			}
#else//��ͨCCID�Ŷ�ֻҪУ��ǰ19λ
			szCcid19 = modenccid;
			if (Value.Left(19) != szCcid19.Left(19)){
				m_edit.SetWindowText("����:������CCID�ŵ�ǰ19λ�����ݿ��е�ǰ19λ����ͬ!\r\n\r\n���:��ȷ�����ݿ�������Ƿ��д�");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("������CCID�������ݿ��еĲ�ƥ�䣡", "����", MB_OK);
				goto EXIT;//������CCID�������ݿ��еĲ�ƥ��
			}
#endif//UNICOM_CCID_19
			if (15 != GetRfIMEI(modenimei)){
				m_edit.SetWindowText("����:δ�ܹ��ӻ����л�ȡIMEI\r\n���:�Ӻô����ߡ������");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("δ�ܹ��ӻ����л�ȡIMEI��", "����", MB_OK);
				goto EXIT;//δ�ܹ��ӻ����л�ȡIMEI
			}

			if (!IsAlphaString(modenimei)){
				m_edit.SetWindowText("����:��ȡ��IMEI��������\r\n���:�Ӻô����ߡ������");
				m_edit.bkColor(RGB(255, 0, 0));
				CloseComm();
				//MessageBox("δ�ܹ��ӻ����л�ȡIMEI��", "����", MB_OK);
				goto EXIT;//δ�ܹ��ӻ����л�ȡIMEI
			}

			{//������ݿ����Ƿ��Ѿ����ڴ�IMEI��
				Value.Empty();
				sprintf(strSQL, 
					"SELECT CCID FROM DataCompare WHERE IMEI = '%s'", //SQL���
					modenimei);
				m_adodb.ExecuteQueryValue(strSQL, Value);
				if (Value.GetLength() == 20)
				{
					Report.Format("����:���ݿ����Ѿ����ڸ�IMEI�ţ�%s��\r\n���:�޷�У��˻�����", modenimei);
					m_edit.SetWindowText(Report);
					m_edit.bkColor(RGB(255, 0, 0));
					CloseComm();
					goto EXIT;
				}
#ifndef  UNICOM_CCID_19
				sprintf(strSQL, 
					"SELECT SERIAL FROM DataCompare WHERE CCID = '%s'", //SQL���
					modenccid);
#else
				sprintf(strSQL, 
					"SELECT SERIAL FROM DataCompare WHERE CCID LIKE '%s_'", //SQL���
					szCcid19.Left(19));
#endif//UNICOM_CCID_19
				m_adodb.ExecuteQueryValue(strSQL, Value);
                
				if (Value.GetLength() == 0)
				{
#ifndef UNICOM_CCID_19
					Report.Format("����:���ݿ���û�д�CCID�ţ�%s��\r\n���:�޷�У��˻�����", modenccid);
#else
					Report.Format("����:���ݿ���û�д�CCID�ţ�%s_��\r\n���:�޷�У��˻�����", szCcid19.Left(19));
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
			        "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID = '%s'", //SQL���
					modenimei, m_strbuyer, m_input, modenccid);
#else
			sprintf(strSQL, 
			        "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID LIKE '%s_'", //SQL���
					modenimei, m_strbuyer, m_input, szCcid19.Left(19));
#endif//UNICOM_CCID_19
			m_adodb.Execute(strSQL);
			//����Code 93 �����ӡ�������������룬˵������������ͬһ��
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
	//����һ���ַ�����ר�����ڻ�ȡ��������Ϣ
	char telnum[16] = "",
		 modenccid[25] = "",
		 modenimei[18] = "";
	char strSQL[512] = "";
	CString  Report,SqlValue("");

	BOOL bCOM = OpenComm(CStrPortTranInt(theApp.M_STRPORT),CStrBateTranInt(theApp.M_STRRATE));

	if (!bCOM){
		Report.Format(_T("�򿪴���ʧ�ܣ�\r\nPORT = %s, RATE = %s"), theApp.M_STRPORT, theApp.M_STRRATE);
		MessageBox(Report, _T("��ʾ"), MB_OK|MB_ICONSTOP);
		return;
	}

	if (!gsminit()){
		m_edit.SetWindowText("����:����δ׼���ã�\r\n���:�Ӻô����ߡ������");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (20 != GetRfCCID(modenccid))
	{//���ȶ�ȡ������CCID
		m_edit.SetWindowText("����:δ�ܹ��ӻ����л�ȡCCID\r\n\n���:�Ӻô����ߡ������");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (!IsAlphaString(modenccid)){
		m_edit.SetWindowText("����:��ȡCCID�Ŵ���\r\n�����ȷ�������Ĵ��ڷ���OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}
#ifdef UNICOM_CCID_19
	szCcid19 = modenccid;
#endif//UNICOM_CCID_19
	if (15 != GetRfIMEI(modenimei))
	{
		m_edit.SetWindowText("����:δ�ܹ��ӻ����л�ȡIMEI\r\n���:�Ӻô����ߡ������");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (!IsAlphaString(modenimei)){
		m_edit.SetWindowText("����:��ȡIMEI�Ŵ���\r\n�����ȷ�������Ĵ��ڷ���OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	{//������ݿ����Ƿ��Ѿ����ڴ�IMEI��
		SqlValue.Empty();
		sprintf(strSQL, 
			"SELECT CCID FROM DataCompare WHERE IMEI = '%s'", //SQL���
			modenimei);
		m_adodb.ExecuteQueryValue(strSQL, SqlValue);
		if (SqlValue.GetLength() == 20)
		{
			Report.Format("����:���ݿ����Ѿ����ڸ�IMEI�ţ�%s��\r\n���:�޷�У��˻�����", modenimei);
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			SqlValue.Empty();
			CloseComm();
			return;
		}
#ifndef UNICOM_CCID_19
		sprintf(strSQL, 
			"SELECT SERIAL FROM DataCompare WHERE CCID = '%s'", //SQL���
			modenccid);
#else
		sprintf(strSQL, 
			"SELECT SERIAL FROM DataCompare WHERE CCID LIKE '%s_'", //SQL���
			szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_adodb.ExecuteQueryValue(strSQL, SqlValue);

		if (SqlValue.GetLength() == 0)
		{
#ifndef UNICOM_CCID_19
			Report.Format("����:���ݿ���û�д�CCID�ţ�%s��\r\n���:�޷�У��˻�����", modenccid);
#else
			Report.Format("����:���ݿ���û�д�CCID(19)�ţ�%s_��\r\n���:�޷�У��˻�����", szCcid19.Left(19));
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
	{//���տ�������������
	case NORMALTYPE_CAR://���ſ������
		if (20 != (int)strlen(modenccid)){
			Report.Format("���󣺶�����CCID�ų��������޷�У�飡\r\n\n���������У��");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		if (!IsAlphaString(modenccid)){
			Report.Format("���󣺶�����CCID�Ÿ�ʽ�����޷�У�飡\r\n\n���������У��");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
#ifndef UNICOM_CCID_19
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID = '%s'", //SQL����ѯ��CCIDƥ���TELNUM
				modenccid);
#else
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID LIKE '%s_'", //SQL����ѯ��CCIDƥ���TELNUM
				szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_adodb.ExecuteQueryValue(strSQL,  SqlValue);
		if (SqlValue.GetLength() != 11)
		{
#ifndef UNICOM_CCID_19
			Report.Format("����δ�ܹ������ݿ��л�ȡ�뻰��CCID��ƥ���TELNUM\r\n���������ÿ��Ƿ�Ϊ�׿��������ݿ����Ƿ���ڸ�CCID��:%s",
				modenccid);
#else
			Report.Format("����δ�ܹ������ݿ��л�ȡ�뻰��CCID��ƥ���TELNUM\r\n���������ÿ��Ƿ�Ϊ�׿��������ݿ����Ƿ���ڸ�CCID��:%s_",
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
			Report.Format("���󣺳�������\r\n\n������������ݿ�����CCID��[%s]ƥ���TELNUM�Ƿ�Ϊ����", 
				modenccid);
#else
			Report.Format("���󣺳�������\r\n\r\n������������ݿ�����CCID��[%s_] ƥ���TELNUM�Ƿ�Ϊ����", 
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
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID = '%s'", //SQL���
				    modenimei, m_strbuyer, SqlValue, modenccid);
#else
			sprintf(strSQL,
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s', BUYER = '%s' WHERE TELNUM = '%s' AND CCID LIKE '%s_'", //SQL���
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
			//����Code 93 �����ӡ�������������룬˵������������ͬһ��
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
	case NON_NORMALTYPE_CAR://�޺ſ������
#ifndef UNICOM_CCID_19
		sprintf(strSQL,
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s',BUYER = '%s' WHERE CCID = '%s'", //SQL���
				    modenimei, m_strbuyer, modenccid);
		m_adodb.Execute(strSQL);
			Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s",modenimei,modenccid);
#else
		sprintf(strSQL,
			    "UPDATE DataCompare SET HANDLED = 1,IMEI = '%s',BUYER = '%s' WHERE CCID LIKE '%s_'", //SQL���
				    modenimei, m_strbuyer, szCcid19.Left(19));
		m_adodb.Execute(strSQL);
		Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s_",modenimei,szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_edit.SetWindowText(Report);
		m_edit.bkColor(RGB(0, 255, 0));
		//����Code 93 �����ӡ�������������룬˵������������ͬһ��
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
		MessageBox("���ش����޷�ʶ��Ŀ����Ͳ���", "��ʾ", MB_OK|MB_ICONERROR);
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
		this->GetDlgItem(IDC_BUTTON3)->SetWindowText("��������");
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
				this->GetDlgItem(IDC_BUTTON3)->SetWindowText("��������");
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
				MessageBox("�������", "��ʾ", MB_OK|MB_ICONERROR);
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
		MessageBox("���ش���û�иô�ӡѡ��", "��ʾ", MB_OK|MB_ICONERROR);
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
		MessageBox("���ش���û�иÿ�����ѡ��", "��ʾ", MB_OK|MB_ICONERROR);
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
			MessageBox(_T("���������Ǵ� 1 ��ʼ�ģ�"), _T("��ʾ"), MB_OK|MB_ICONSTOP);
			return;
		}else if (CcidCol == 0){
			MessageBox(_T("���������Ǵ� 1 ��ʼ�ģ�"), _T("��ʾ"), MB_OK|MB_ICONSTOP);
			return;
		}else if (TelCol == CcidCol){
			MessageBox(_T("�绰�����CCID���ǲ��ܹ���ͬһ�еģ�"), _T("��ʾ"), MB_OK|MB_ICONSTOP);
			return;
		}else if (TelCol + CcidCol == 0){
			MessageBox(_T("���������Ǵ���������"), _T("��ʾ"), MB_OK|MB_ICONSTOP);
			return;
		}else{
			CSpreadSheet SS(pathname, sheetname, 0);
			SS.ReadCell(Value, TelCol, row++); // ��ȡ�绰���� ��һ��
			SS.ReadCell(Value, TelCol, row);   // ��ȡ�绰���� �ڶ���
			while(!Value.IsEmpty())
			{
				errmsg      = Value;
				PreserveLen = PreserveString(errmsg, 0);
				Value  = errmsg.Left(11);
				if (Value.GetLength() != 11){
					errmsg.Format("�޷�ʶ��˸�ʽ�ĵ绰���룺%s PreserveLen = %d [�У�%d���У�%d]", Value, PreserveLen, row, TelCol);
					MessageBox(errmsg, _T("��������ʧ��"), MB_OK|MB_ICONSTOP);
					return;
				}
				//Value  = errmsg;

				SS.ReadCell(ccid, CcidCol, row);
				errmsg      = ccid;
				PreserveLen = PreserveString(errmsg, 4);
				ccid        = errmsg.Left(20);
				if (ccid.GetLength() != 20){
					errmsg.Format("�޷�ʶ��˸�ʽ��CCID�ţ�%s [�У�%d���У�%d]", ccid, row, CcidCol);
					MessageBox(errmsg, _T("��������ʧ��"), MB_OK|MB_ICONSTOP);
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
		errmsg.Format("�ɹ����� %d ����¼��", row-2);
		MessageBox(errmsg, _T("��ʾ"), MB_OK);
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
	{//�����"OK"��
		string.Format("��ȷ���µı�����[%s] \r\n������Դ�� \r\n[%s] \r\n��û���ظ���?", dlg.m_strnewtablename, theApp.M_STRDBSRC);
		if (IDYES == MessageBox(string, _T("ѯ��"), MB_YESNO|MB_ICONQUESTION))
		{//�����"YES"��
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
				if (dlg.m_bcCheck){                                      //��
					switch(dlg.m_iSelCheck){                             //��
						case 0:                                          //��
							condition = condition + " HANDLED = FALSE";  //��
							break;                                       //ѡ
						case 1:                                          //��
							condition = condition + " HANDLED = TRUE";   //��
							break;                                       //��
						default:                                         //��
							condition = condition + " HANDLED = TRUE";   //��
							break;                                       //֯
					}                                                    //һ
				}                                                        //��
				if (dlg.m_bcBuyer){                                      // S
					condition = condition + " AND BUYER = '";            // Q
					condition = condition + dlg.m_strbcBuyer;            // L
					condition = condition + "'";                         //��
				}                                                        //ѯ
				if (dlg.m_bcArea){                                       //��
					condition = condition + " AND AREA = '";             //��
					condition = condition + dlg.m_strbcArea;             //
					condition = condition + "'";                         //
				}                                                        //
				if (dlg.m_bcBuyer||dlg.m_bcArea||dlg.m_bcCheck){
					sql = sql + condition; 
				}else{
				}
				//*****************************************************************
				//����һ���±����ݿ�
				if (FALSE == m_adodb.Execute(sql)){
					MessageBox(_T("��������ʧ�ܣ�����ָ���ı��������ݿ�����û���ظ���"), _T(" ������ʾ"), MB_OK|MB_ICONSTOP);
				}else{
					string.Format("�ɹ�������Դ��\r\n[%s]\r\n�����ӱ��[%s]", theApp.M_STRDBSRC, dlg.m_strnewtablename);
					MessageBox(string, _T(" ������ʾ"), MB_OK);
				}
				fprintf(fp, "%s\r", sql);
		}//�����"YES"��
	}//�����"OK"��
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
	//����һ���ַ�����ר�����ڻ�ȡ��������Ϣ
	char telnum[16] = "",
		 modenccid[25] = "",
		 modenimei[18] = "";
	char strSQL[512] = "";
	CString  Report,SqlValue("");

	BOOL bCOM = OpenComm(CStrPortTranInt(theApp.M_STRPORT),CStrBateTranInt(theApp.M_STRRATE));

	if (!bCOM){
		Report.Format(_T("�򿪴���ʧ�ܣ�\r\nPORT = %s, RATE = %s"), theApp.M_STRPORT, theApp.M_STRRATE);
		MessageBox(Report, _T("��ʾ"), MB_OK|MB_ICONSTOP);
		return;
	}

	if (!gsminit()){
		m_edit.SetWindowText("����:����δ׼���ã�\r\n���:�Ӻô����ߡ������");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (20 != GetRfCCID(modenccid))
	{//���ȶ�ȡ������CCID
		m_edit.SetWindowText("����:δ�ܹ��ӻ����л�ȡCCID\r\n\n���:�Ӻô����ߡ������");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}
#ifdef UNICOM_CCID_19
	szCcid19 = modenccid;
#endif//UNICOM_CCID_19
	if (!IsAlphaString(modenccid)){
		m_edit.SetWindowText("����:��ȡCCID�Ŵ���\r\n�����ȷ�������Ĵ��ڷ���OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (15 != GetRfIMEI(modenimei))
	{
		m_edit.SetWindowText("����:δ�ܹ��ӻ����л�ȡIMEI\r\n���:�Ӻô����ߡ������");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	if (!IsAlphaString(modenimei)){
		m_edit.SetWindowText("����:��ȡIMEI�Ŵ���\r\n�����ȷ�������Ĵ��ڷ���OK");
		m_edit.bkColor(RGB(255, 0, 0));
		CloseComm();
		return;
	}

	switch(m_typecar)
	{//���տ�������������
	case NORMALTYPE_CAR://���ſ������
		if (20 != (int)strlen(modenccid)){
			Report.Format("���󣺶�����CCID�ų��������޷�У�飡\r\n\n���������У��");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
		if (!IsAlphaString(modenccid)){
			Report.Format("���󣺶�����CCID�Ÿ�ʽ�����޷�У�飡\r\n\n���������У��");
			m_edit.SetWindowText(Report);
			m_edit.bkColor(RGB(255, 0, 0));
			CloseComm();
			return;
		}
#ifndef UNICOM_CCID_19
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID = '%s'", //SQL����ѯ��CCIDƥ���TELNUM
				modenccid);
#else		
		sprintf(strSQL,
			    "SELECT TELNUM FROM DataCompare WHERE CCID LIKE '%s_'", //SQL����ѯ��CCIDƥ���TELNUM
				szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_adodb.ExecuteQueryValue(strSQL,  SqlValue);
		if (SqlValue.GetLength() != 11)
		{
#ifndef UNICOM_CCID_19
			Report.Format("����δ�ܹ������ݿ��л�ȡ�뻰��CCID��ƥ���TELNUM\r\n���������ÿ��Ƿ�Ϊ�׿��������ݿ����Ƿ���ڸ�CCID��:%s", 
				modenccid);
#else
			Report.Format("����δ�ܹ������ݿ��л�ȡ�뻰��CCID��ƥ���TELNUM\r\n���������ÿ��Ƿ�Ϊ�׿��������ݿ����Ƿ���ڸ�CCID��:%s_", 
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
			Report.Format("���󣺳�������\r\n������������ݿ�����CCID��[%s]ƥ���TELNUM�Ƿ�Ϊ����", 
				modenccid);
#else
			Report.Format("���󣺳�������\r\n������������ݿ�����CCID��[%s_]ƥ���TELNUM�Ƿ�Ϊ����", 
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
	case NON_NORMALTYPE_CAR://�޺ſ������
#ifndef UNICOM_CCID_19
		Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s",modenimei,modenccid);
#else
		szCcid19 = modenccid;
		Report.Format("TELNUM:00000000000\r\nIMEI:%s\r\nCCID:%s_",modenimei,szCcid19.Left(19));
#endif//UNICOM_CCID_19
		m_edit.SetWindowText(Report);
		m_edit.bkColor(RGB(0, 255, 0));
			//����Code 93 �����ӡ�������������룬˵������������ͬһ��
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
		MessageBox("���ش����޷�ʶ��Ŀ����Ͳ���", "��ʾ", MB_OK|MB_ICONERROR);
		CloseComm();
		break;
	}
	CDialog::UpdateData(FALSE);
}

void CCompareDlg::OnBnClickedButtonInput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgInvalidCardPrint DlgInvalidCardPrint;

	// ��ӡ���겿��
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
