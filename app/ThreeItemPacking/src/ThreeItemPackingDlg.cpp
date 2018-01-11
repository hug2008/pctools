// ThreeItemPackingDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CThreeItemPackingDlg �Ի���



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


// CThreeItemPackingDlg ��Ϣ�������

BOOL  CThreeItemPackingDlg::PreTranslateMessage(MSG* pMsg)
{
	char vkmsg,//������Ϣ��ֵ
		strquery[256]  = "";//SQL��ѯ���
	CString QueryValue = _T("");//���ݿ��ѯ���õ�ֵ
	CString TableName  = m_boolCardType ? "PackingDataEx" : "PackingData"; 
	int listcount      = 0;
	if (WM_KEYDOWN == pMsg->message){//������Ϣ
		//�ռ����̶���
		vkmsg = (char)(0xFF&pMsg->wParam);
		if (CharIsAlNum(vkmsg)) 	m_strkeymsg = m_strkeymsg + vkmsg;
		if  (pMsg->hwnd ==((GetDlgItem(IDC_EDIT1))->m_hWnd)){//�����Ϣ����TELNUM			
			if (VK_RETURN == pMsg->wParam){
				GetDlgItem(IDC_EDIT1)->GetWindowText(m_strTelnum);
				//m_strTelnum = m_strkeymsg;
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strTelnum.GetLength(), m_strTelnum);
				if (m_strTelnum.GetLength() != 11){
					MessageBox("�绰����ֻ�ܹ���11λ", "��ʾ", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT1, m_strTelnum);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (!CStrIsNumber(m_strTelnum)){
					MessageBox("�绰����ֻ�ܹ�������", "��ʾ", MB_OK|MB_ICONERROR);
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
					sprintf(strquery, "���ݿ�(%s)���Ѿ��������TELNUM��%s\nSERIAL = %s", TableName, m_strTelnum, QueryValue);
					MessageBox(strquery, "��ʾ", MB_OK|MB_ICONERROR);
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
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT2))->m_hWnd)){//�����Ϣ����IMEI
			
			if (VK_RETURN == pMsg->wParam){
				//m_strImei = m_strkeymsg;
				GetDlgItem(IDC_EDIT2)->GetWindowText(m_strImei);
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strImei.GetLength(), m_strImei);
				if (m_strImei.GetLength() != 15){
					MessageBox("IMEI��ֻ�ܹ���15λ", "��ʾ", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT2, m_strImei);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (!CStrIsNumAlpha(m_strImei)){
					MessageBox("IMEI��ֻ�ܹ������ֻ�����ĸ", "��ʾ", MB_OK|MB_ICONERROR);
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
					sprintf(strquery, "���ݿ�(%s)���Ѿ��������IMEI��%s\nSERIAL = %s", TableName, m_strImei, QueryValue);
					MessageBox(strquery, "��ʾ", MB_OK|MB_ICONERROR);
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
		}else if (pMsg->hwnd ==((GetDlgItem(IDC_EDIT3))->m_hWnd)){//�����Ϣ����CCID			
			if (VK_RETURN == pMsg->wParam){
				//m_strCcid = m_strkeymsg;
				GetDlgItem(IDC_EDIT3)->GetWindowText(m_strCcid);
				m_strkeymsg.Format("Len = %d\nMsg = %s",m_strCcid.GetLength(), m_strCcid);
				if (m_strCcid.GetLength() != 20){
					MessageBox("CCID����ֻ�ܹ���20λ", "��ʾ", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT3, m_strCcid);
					m_strkeymsg.Empty();
					return FALSE;
				}
				if (!CStrIsNumAlpha(m_strCcid)){
					MessageBox("CCIDֻ�ܹ������ֻ�����ĸ", "��ʾ", MB_OK|MB_ICONERROR);
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
					sprintf(strquery, "���ݿ�(%s)���Ѿ��������CCIC�ţ�%s\nSERIAL = %s", TableName, m_strCcid, QueryValue);
					MessageBox(strquery, "��ʾ", MB_OK|MB_ICONERROR);
					//EmptyCEidtItemInList();
					EmptyItemSetFocus(IDC_EDIT3, m_strCcid);
					m_strkeymsg.Empty();
					return FALSE;
				}
				//���༭�����ݵ�������
				if (CheckEditIntegrity() != 0){
					return FALSE;
				}
				listcount = GetListItemCount();
				if (listcount >= GetListAmountCount()){//�ﵽ��ÿ�������
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
			//�����Ϣ���������Ĵ���Ԫ��
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

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	{
		//��̬�ؼ��ĳ�ʼ��
		m_stcticket.SubclassDlgItem(IDC_STATIC06, this);
		m_stcticket.SetWindowText("�ͼ��֪ͨ����");
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
		//��ť�ĳ�ʼ��
		m_CtrlButtonLock.SubclassDlgItem(IDOK,  this);
		m_CtrlButtonLock.SetWindowText("�� �� �� ��");
		m_CtrlButtonPrint.SubclassDlgItem(IDC_Button2,  this);
		m_CtrlButtonPrint.SetWindowText("�� ӡ");
		m_CtrlButtonDelete.SubclassDlgItem(IDC_Button1, this);
		m_CtrlButtonDelete.SetWindowText("����б�");
		m_CtrlButtonStroe.SubclassDlgItem(IDC_Button3, this);
		m_CtrlButtonStroe.SetWindowText("�洢����");
		//����ȡ����ť
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	}

	{
		//�����Լ������ļ�·���ĳ�ʼ��
		::GetCurrentDirectory(MAX_PATHLENGTH,m_strconfigpathname.GetBuffer(MAX_PATHLENGTH));
		m_strconfigpathname.ReleaseBuffer();
		m_strconfigpathname += "\\PackConfig.ini";
		str = GetCommandLine();
		str = str.Left(str.GetLength()-2);
		str = str.Right(str.GetLength()-1);
		//str ="PRINT WIDTH 105 08/12 MM+  " + str;
		//�޸��˱������汾��
		str = "װ���������ӡ-V1.0.3b   " + str;
		SetWindowText(str);
		str.Empty();
	}

	{
		//�б�ؼ��ĳ�ʼ��
		m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
		m_list.SetBkColor(RGB(247,247,255));
		m_list.SetTextColor(RGB(0,0,255));
		m_list.SetTextBkColor(RGB(247,247,255));
		m_list.InsertColumn(0, "���", LVCFMT_CENTER, 40);
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
		//CComboBox  �ؼ��ĳ�ʼ��
		char flag = ',';
		int  nculsel = 0;
		str.Empty();
		//�ͺţ�
		GetPrivateProfileString("MODEL", "MODELLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("MODEL", "MODELCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlCommodel, str, flag, nculsel);

		//��ɫ��
		GetPrivateProfileString("COLOR", "COLORLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("COLOR", "COLORCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComcolor, str, flag, nculsel);

		//�汾��
		GetPrivateProfileString("EDITION", "EDITIONLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("EDITION", "EDITIONCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComedition, str, flag, nculsel);

		//����(��)��
		GetPrivateProfileString("AMOUNT", "AMOUNTLIST","ERROR",str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("AMOUNT", "AMOUNTCURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComamount, str, flag, nculsel);

		//��ӡ������
		GetPrivateProfileString("NSHARE", "NSHARELIST", "00", str.GetBuffer(MAX_PATHLENGTH), MAX_PATHLENGTH, m_strconfigpathname);
		nculsel = GetPrivateProfileInt("NSHARE", "NSHARECURSEL", 0, m_strconfigpathname);
		str.ReleaseBuffer();
		InitCComboBoxByDiscontinuousStr(&m_CtrlComnshare, str, flag, nculsel);
	}

	{
		// �༭�ؼ���ʼ��
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
		//�������ݿ�
		if(!m_adoConnection.ConnectAccess("LablePacking.mdb")){
			MessageBox(_T("�������ݿ⣺LablePacking.mdbʧ�ܣ�"), _T("��ʾ"), MB_OK|MB_ICONSTOP);
			return FALSE;
		}
	}

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CThreeItemPackingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
		if (nItemstr == str){//����Ѿ������������¼
			nItemstr.Format("�б����Ѿ����������¼\n��ţ�%s", m_list.GetItemText(i, 0));
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
	m_nIndex     = m_list.GetNextSelectedItem(pos);  // �õ���Ŀ����
	if (m_list.GetItemText(m_nIndex, 0).IsEmpty()){
		AfxMessageBox("��ѡ��ǿ���Ŀ!", MB_OK|MB_ICONWARNING);
		return;
	}
	
	dlg.m_strSerial = m_list.GetItemText(m_nIndex, 0);
	dlg.m_strTelnum = m_list.GetItemText(m_nIndex, 1);
	dlg.m_strImei   = m_list.GetItemText(m_nIndex, 2);
	dlg.m_strCcid   = m_list.GetItemText(m_nIndex, 3);

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_strTelnum.GetLength() < 11){
			MessageBox("TELNUM ���� 11 λ��", "��ʾ", MB_OK|MB_ICONERROR);
			return;
		}else if (dlg.m_strImei.GetLength() < 15){
			MessageBox("IMEI ���� 15 λ��", "��ʾ", MB_OK|MB_ICONERROR);
			return;
		}else if (dlg.m_strCcid.GetLength() < 20){
			MessageBox("CCID ���� 20 λ��", "��ʾ", MB_OK|MB_ICONERROR);
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
		m_CtrlButtonLock.SetWindowText("�� �� �� ��");
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
				m_CtrlButtonLock.SetWindowText("�� �� �� ��");
				m_bLock = !m_bLock;
			}
			else
			{
				MessageBox("�������", "��ʾ", MB_OK|MB_ICONERROR);
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
	if (IDYES == MessageBox("�ò���������б���������Ŀɾ��\r\n       ��ȷ��ɾ����", "ѯ��", MB_YESNO|MB_ICONASTERISK))
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
	//�б�ؼ��ļ�¼��
	int nCount      =  m_list.GetItemCount();
	//��������߶ȵĲ��������б�����ݲ���10������10���Ĺ�����
	int BARHEIGHET      =  (MMDOTSA*6);//(nCount < 10) ? (MMDOTSA*8):(MMDOTSA*6);
	int nMMspace        =   10;

	CString         strini;//���ڶ�ȡ�����ļ����ֶ�ֵ

	XY              xy, 
		            xytemp;  //��ʼ����ṹ,�������ļ�����
	ZPLFONT         zf;      //����Ĵ�ӡ�ṹ,�������ļ�����

	BARBY           by;      //����ģ�����ýṹ

	LPCHINESEZPL    lpchzpl = new CHINESEZPL; //���ֱ���ṹ
	

	//�����ַ�����ȡm_list�б��TELNUM, IMEI, CCID
	char Telnum[12] = "",
		 Imei[16]   = "",
		 Ccid[21]   = "";

	//������ݵ�������
	if (m_strTicket.GetLength() < TicketLen)
	{
		MessageBox("�뽫�ͼ�֪ͨ����д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -1;
	}
	if (m_streditboxnum.GetLength() < 18)
	{
		MessageBox("�뽫�����д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -2;
	}
	if (m_strBuyer.GetLength() < 6)
		{
		MessageBox("�뽫����������д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -3;
	}

	if (NULL == fp){
		MessageBox("�޷������ļ���PrintCmd.cfg", "��ʾ", MB_OK|MB_ICONERROR);
		return -4;
	}
	if (NULL == cmdformat){
		MessageBox("��̬�����ڴ�ʧ�ܣ�2k��", "����", MB_OK|MB_ICONERROR);
		return -5;
	}
	if (NULL == PrintCmd){
		MessageBox("��̬�����ڴ�ʧ�ܣ�30k��", "����", MB_OK|MB_ICONERROR);
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

	{//LPCHINESEZPL�ṹ�ĳ�ʼ��
		GetPrivateProfileString("CHINESEBARCODE", "FONTNAME", "����", strini.GetBuffer(MAX_PATHLENGTH),  MAX_PATHLENGTH,  m_strconfigpathname);
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

	{//��ӡ��š�����ͷָ�����
		//��ӡ���� �ţ���
		lpchzpl->xy.x_axis = xy.x_axis + MMDOTSA;
		lpchzpl->xy.y_axis = xy.y_axis + MMDOTSA*5;
		lpchzpl->lpChinese = "�� �ţ�";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		xytemp.x_axis      = xy.x_axis + MMDOTSA*15;
		xytemp.y_axis      = 0;//xy.y_axis + MMDOTSA*18;
		//��ӡ��ŵ�����
		sprintf(cmdformat, "^FO%d,%d^BY2,2.00,%d^BAN,,N,N,N^FD%s^FS", 
			xytemp.x_axis, xytemp.y_axis+MMDOTSA*2, MMDOTSA*10, m_streditboxnum);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		sprintf(cmdformat, "^FO%d,%d^A0,N,%d, %d^FD%s^FS", 
			xytemp.x_axis + MMDOTSA*4, xytemp.y_axis + MMDOTSA*13, MMDOTSA*3, MMDOTSA*2, m_streditboxnum);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	
		//��ӡ���� �룺��
		lpchzpl->xy.x_axis = xytemp.x_axis + MMDOTSA*40;
		lpchzpl->lpChinese = "�� �룺";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡ���������
		xytemp.x_axis  =  lpchzpl->xy.x_axis + MMDOTSA*14;
		sprintf(cmdformat, "^FO%d,%d^BY2,2.00,%d^BAN,,N,N,N^FDDESAYELE^FS", 
			xytemp.x_axis, xytemp.y_axis+MMDOTSA*2, MMDOTSA*10);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		sprintf(cmdformat, "^FO%d,%d^A0,N,%d, %d^FDDESAYELE^FS", 
			xytemp.x_axis + MMDOTSA*2, xytemp.y_axis + MMDOTSA*13, MMDOTSA*3, MMDOTSA*2);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡһ���ָ���
		xy.x_axis = MMDOTSA * 5;
		xy.y_axis = lpchzpl->xy.y_axis + MMDOTSA*12;
		sprintf(cmdformat, "^FO%d,%d^GB%d,0,5,B,0^FS",xy.x_axis, xy.y_axis, 110*MMDOTSA);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	}

	{//��ӡ�ͺš���ɫ����������
		//��ӡ���� �ţ�������
		lpchzpl->lpChinese  = "��  �ţ�";
		lpchzpl->intHeight  = lpchzpl->intHeight/2;
		lpchzpl->xy.x_axis  = xy.x_axis + MMDOTSA*2;
		lpchzpl->xy.y_axis  = xy.y_axis + MMDOTSA*2;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡ������ͺ�����
		xytemp.x_axis  =  lpchzpl->xy.x_axis;
		xytemp.y_axis  =  lpchzpl->xy.y_axis;
		xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA * 10;
		xy.y_axis = lpchzpl->xy.y_axis;
		sprintf(cmdformat,"^FO%d,%d^A0N,%d,%d^FD%s^FS", 
			xy.x_axis, xy.y_axis, lpchzpl->intHeight + MMDOTSA*3, lpchzpl->intHeight, m_strcommodel);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡ���� ɫ��������
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*60;
		lpchzpl->lpChinese = "�� ɫ��";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//��ӡ�������ɫ
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*12;
		lpchzpl->lpChinese = m_strcomcolor;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡ���� �򣺡�����
		lpchzpl->xy.x_axis = xytemp.x_axis;
		lpchzpl->xy.y_axis = xytemp.y_axis + lpchzpl->intHeight + MMDOTSA*4;
		lpchzpl->lpChinese = "��  ��";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//��ӡ���������
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*10;
		lpchzpl->lpChinese = m_strcomedition;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡ���� ����������
		lpchzpl->xy.x_axis = xytemp.x_axis + MMDOTSA*60;
		lpchzpl->lpChinese = "�� ����";
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//��ӡ���������
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*12;
		sprintf(cmdformat, "^FO%d,%d^A0N,%d,%d^FD%s^FS",
			lpchzpl->xy.x_axis, lpchzpl->xy.y_axis, lpchzpl->intHeight + MMDOTSA*3, lpchzpl->intHeight, this->m_strComboAmount);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//��ӡ����������
		lpchzpl->xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA*8;
		sprintf(cmdformat, "^FO%d,%d^BY2,2.00,%d^BAN,,N,N,N^FD%s^FS", 
			lpchzpl->xy.x_axis, lpchzpl->xy.y_axis, lpchzpl->intHeight + MMDOTSA*2, this->m_strComboAmount);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	}

	{//��ӡ�б��ڵ�������������
#define    BARCODELEN     (MMDOTSA*6);
		if (nCount > 0){
			//û�����ݾ�����
			xytemp.x_axis = xy.x_axis = MMDOTSA*8;
			xytemp.y_axis = xy.y_axis = lpchzpl->xy.y_axis + max(MMDOTSA, lpchzpl->intHeight) + MMDOTSA*4;
			if(m_boolCardType){//����ǰ׿�����ֻ��ӡIMEI��CCID
				xytemp.x_axis = xytemp.x_axis + MMDOTSA*5;
				nMMspace      = nMMspace + 5;
				for (m_nIndex=0; m_nIndex<nCount; m_nIndex++){//������ӡ
					strcpy(Imei,   m_list.GetItemText(m_nIndex, 2));
					strcpy(Ccid,   m_list.GetItemText(m_nIndex, 3));
					CreatePackingItem(cmdformat, Imei, Ccid, by, xytemp, nMMspace);
					strcat(PrintCmd,  cmdformat);
					fprintf(fp,  "%s\r", cmdformat);
					xytemp.y_axis = xytemp.y_axis + by.heighet + MMDOTSA*4;
				}
			}else{				
				for (m_nIndex=0; m_nIndex<nCount; m_nIndex++){//������ӡ
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

	{//��ӡ�ָ����ߺ��ͼ��֪ͨ����Q/A��������
		//��ӡһ���ָ��ߣ����12mm�� ���˿�2mm
		xy.x_axis = MMDOTSA * 5;
		xy.y_axis = (150 - 15) * MMDOTSA;
		sprintf(cmdformat, "^FO%d,%d^GB%d,0,5,B,0^FS", xy.x_axis, xy.y_axis, 110*MMDOTSA);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡ���ͼ�֪ͨ������
		lpchzpl->lpChinese   =  "�ͼ�֪ͨ����";
		lpchzpl->intHeight   = lpchzpl->intHeight;
		lpchzpl->boolBold    = 1;
		lpchzpl->xy.x_axis   = xy.x_axis + MMDOTSA * 3;
		lpchzpl->xy.y_axis   = xy.y_axis + MMDOTSA * 2;
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//��ӡ�ͼ�֪ͨ���ľ�������
		xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA * 15;
		xy.y_axis = lpchzpl->xy.y_axis + MMDOTSA;
		sprintf(cmdformat,"^FO%d,%d^A0N,%d,%d^FD%s         Q/A:^FS", 
			xy.x_axis, xy.y_axis, lpchzpl->intHeight+20, lpchzpl->intHeight+10, m_strTicket);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);

		//��ӡ�����ڣ�������
		lpchzpl->lpChinese   =  "��  �ڣ�";
		lpchzpl->xy.x_axis   =  lpchzpl->xy.x_axis + MMDOTSA*(65+3);
		GetChineseZplCode(lpchzpl,   cmdformat);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
		//��ӡϵͳ������
		xy.x_axis = lpchzpl->xy.x_axis + MMDOTSA * 10;
		GetSysTime(ptime, pdate);
		sprintf(cmdformat,"^FO%d,%d^A0N,%d,%d^FD%s^FS", 
			xy.x_axis, xy.y_axis, lpchzpl->intHeight+20, lpchzpl->intHeight+10, pdate);
		strcat(PrintCmd,  cmdformat);
		fprintf(fp,  "%s\r", cmdformat);
	}
   
	{//�����ﴦ���ӡ����������
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
		MessageBox("�뽫�ͼ�֪ͨ����д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -1;
	}
	if (m_streditboxnum.GetLength() < 18)
	{
		MessageBox("�뽫�����д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -2;
	}
	if (m_strBuyer.GetLength() < 6)
	{
		MessageBox("�뽫����������д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -3;
	}
	if (m_strTelnum.GetLength() != 11){
		MessageBox("TELNUM ���Ȳ��ԣ�", "��ʾ", MB_OK|MB_ICONERROR);
		return -11;
	}
	if (m_strImei.GetLength() != 15){
		MessageBox("IMEI ���Ȳ��ԣ�", "��ʾ", MB_OK|MB_ICONERROR);
		return -15;
	}
	if (m_strCcid.GetLength() != 20){
		MessageBox("CCID ���Ȳ��ԣ�", "��ʾ", MB_OK|MB_ICONERROR);
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
		MessageBox("�뽫�ͼ�֪ͨ����д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -1;
	}
	if (m_streditboxnum.GetLength() < 18)
	{
		MessageBox("�뽫�����д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -2;
	}
	if (m_strBuyer.GetLength() < 6)
		{
		MessageBox("�뽫����������д������", "��ʾ", MB_OK|MB_ICONERROR);
		return -3;
	}
	//��ȡϵͳ��ʱ��
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
