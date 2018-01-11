// .\SRC\COMM\SelComPortClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "SelComPortClient.h"


// CSelComPortClient �Ի���

IMPLEMENT_DYNAMIC(CSelComPortClient, CDialog)

CSelComPortClient::CSelComPortClient(CWnd* pParent /*=NULL*/)
	: CDialog(CSelComPortClient::IDD, pParent)
{
	m_ComPort = _T("COM1");
	m_strBaut = _T("115200");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSelComPortClient::~CSelComPortClient()
{
}

void CSelComPortClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPORT_CLIENT, m_cbPort);
	DDX_CBString(pDX, IDC_COMPORT_CLIENT, m_ComPort);

	DDX_Control(pDX, IDC_COMBATU_CLIENT, m_cbBaut);
	DDX_CBString(pDX, IDC_COMBATU_CLIENT, m_strBaut);
	
	DDV_MaxChars(pDX, m_ComPort, 15);
	DDX_Control(pDX, IDOK_CLIENT, m_btOK);
	DDX_Control(pDX, IDCANCEL_CLIENT, m_btCancel);
}


BEGIN_MESSAGE_MAP(CSelComPortClient, CDialog)
	ON_BN_CLICKED(IDOK_CLIENT, &CSelComPortClient::OnBnClickedClientOk)
	ON_BN_CLICKED(IDCANCEL_CLIENT, &CSelComPortClient::OnBnClickedClientCancel)
END_MESSAGE_MAP()


// CSelComPortClient ��Ϣ�������
BOOL CSelComPortClient::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_btOK    .SetToolTipText(_T("Selete and logon"));
	m_btCancel.SetToolTipText(_T("Exit this app"));

	SetWindowText(SYS_NAME);

	return TRUE;
}
void CSelComPortClient::OnBnClickedClientOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}

void CSelComPortClient::OnBnClickedClientCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}
