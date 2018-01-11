// CCIDcompareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CCIDcompare.h"
#include "CCIDcompareDlg.h"
#include ".\ccidcomparedlg.h"


#include "CompareDlg.h"
#include "about.h"
#include ".\res\Version.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCCIDcompareDlg::CCCIDcompareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCCIDcompareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_port     = theApp.M_STRPORT;

	m_rate     = theApp.M_STRRATE;

	M_STRDBSRC = theApp.M_STRDBSRC;
}

void CCCIDcompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlport);
	DDX_CBString(pDX, IDC_COMBO1, m_port);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlrate);
	DDX_CBString(pDX, IDC_COMBO2, m_rate);
	DDX_Control(pDX, IDOK, m_button);
	DDX_Text(pDX, IDC_EDIT2, M_STRDBSRC);
}

BEGIN_MESSAGE_MAP(CCCIDcompareDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CCCIDcompareDlg ��Ϣ�������

BOOL CCCIDcompareDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	int i;
	CString str;
	for(i=1; i<10; i++){
		if (i<10){
			str.Format("COM%d", i);
			m_ctrlport.AddString(str);
		}else{
			str.Format("COM%d", i);
			m_ctrlport.AddString(str);
		}
	}

	m_ctrlrate.AddString("28800");
	m_ctrlrate.AddString("9600");
	m_ctrlrate.AddString("19200");
	m_ctrlrate.AddString("38400");
	m_ctrlrate.AddString("56000");
	m_ctrlrate.AddString("115200");
    
	this->SetWindowText("�˿ں�ѡ��");
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CCCIDcompareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;

		dlgAbout.m_szVer.Format(_T("%s%s"), STR_APP_NAME, STRFILEVER);
		dlgAbout.m_szBuildTime.Format(_T("%s%s %s"), STR_BUILD_TIME, __DATE__, __TIME__);

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

void CCCIDcompareDlg::OnPaint() 
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
HCURSOR CCCIDcompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCCIDcompareDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CCompareDlg CompareDlg;
	
	theApp.M_STRPORT = m_port;
	theApp.M_STRRATE = m_rate;

	SetWindowText(_T("����һ���ǩ��ӡ..."));

	PostMessage(SW_MINIMIZE);

	CompareDlg.DoModal();

	CCCIDcompareDlg::OnOK();
}

void CCCIDcompareDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CFileDialog  dlg(TRUE, 
		             NULL,
					 NULL, 
					 OFN_HIDEREADONLY,
					 "Access Files (*.mdb)|*.mdb||",
					 AfxGetMainWnd()); 

	if (IDOK == dlg.DoModal()){
		M_STRDBSRC = dlg.GetPathName();
		theApp.M_STRDBSRC = M_STRDBSRC;
	}else{
	}
;
	CDialog::UpdateData(FALSE);
}
