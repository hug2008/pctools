// DlgInvalidCardPrint.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CCIDcompare.h"
#include "DlgInvalidCardPrint.h"


// CDlgInvalidCardPrint �Ի���

IMPLEMENT_DYNAMIC(CDlgInvalidCardPrint, CDialog)

CDlgInvalidCardPrint::CDlgInvalidCardPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInvalidCardPrint::IDD, pParent)
	, m_szTel(_T(""))
	, m_szImei(_T(""))
	, m_szCcid(_T(""))
	, m_szBuyer(_T(""))
	, m_szArea(_T("����"))
{
	m_pAdodb = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDlgInvalidCardPrint::~CDlgInvalidCardPrint()
{
	m_pAdodb = NULL;
}

void CDlgInvalidCardPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INVALID_TEL,  m_edTel);
	DDX_Control(pDX, IDC_EDIT_INVALID_IMEI, m_edImei);
	DDX_Control(pDX, IDC_EDIT_INVALID_CCID, m_edCcid);
	DDX_Control(pDX, IDC_BT_INVALID_PRINT,  m_btPrint);

	DDX_Text(pDX, IDC_EDIT_INVALID_TEL,     m_szTel);
	DDX_Text(pDX, IDC_EDIT_INVALID_IMEI,    m_szImei);
	DDX_Text(pDX, IDC_EDIT_INVALID_CCID,    m_szCcid);
}


BEGIN_MESSAGE_MAP(CDlgInvalidCardPrint, CDialog)
	ON_BN_CLICKED(IDC_BT_INVALID_PRINT, &CDlgInvalidCardPrint::OnBnClickedBtInvalidPrint)
	ON_EN_CHANGE(IDC_EDIT_INVALID_TEL, &CDlgInvalidCardPrint::OnEnChangeEditInvalidTel)
	ON_EN_CHANGE(IDC_EDIT_INVALID_IMEI, &CDlgInvalidCardPrint::OnEnChangeEditInvalidImei)
	ON_EN_CHANGE(IDC_EDIT_INVALID_CCID, &CDlgInvalidCardPrint::OnEnChangeEditInvalidCcid)
END_MESSAGE_MAP()


// CDlgInvalidCardPrint ��Ϣ�������

BOOL  CDlgInvalidCardPrint::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) 
		return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE; 
	else 
		return CDialog::PreTranslateMessage(pMsg);

}

BOOL CDlgInvalidCardPrint::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	if (m_pAdodb && m_pAdodb->IsOpen())
	{
		SetWindowText(_T("���������ӡ (��У��) ���ݿ�������..."));
	}
	else
	{
		SetWindowText(_T("���������ӡ (��У��) ���ݿ�δ����..."));
	}	

	return TRUE;
}
BOOL  CDlgInvalidCardPrint::IsValidSetting(void)
{
	m_edTel.GetWindowText(m_szTel);
	m_edImei.GetWindowText(m_szImei);
	m_edCcid.GetWindowText(m_szCcid);

	m_szImei.MakeUpper();
	m_szCcid.MakeUpper();

	if (m_szTel.IsEmpty() && m_szImei.IsEmpty() && m_szCcid.IsEmpty())
	{
		MessageBox(_T("û����Ч����!"), _T("����"), MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	// �ֻ�������Ч�Լ��
	if (m_szTel.GetLength() > 0)
	{
		if (!m_szTel.IsPhoneNumber())
		{
			MessageBox(_T("Telnum ����Ҫ������!"), _T("����"), MB_OK|MB_ICONSTOP);
			m_edTel.SetFocus();
			m_edTel.SetSel(0, -1);
			return FALSE;
		}

		if (11 != m_szTel.GetLength())
		{
			MessageBox(_T("Telnum ����Ҫ�� 11 λ,��������!"), _T("����"), MB_OK|MB_ICONSTOP);
			m_edTel.SetFocus();
			m_edTel.SetSel(0, -1);
			return FALSE;
		}
	}

	// Imei ��Ч�Լ�� 15 λ Hex
	if ( (15 != m_szImei.GetLength()) && !m_szImei.IsHexNumber())
	{
		MessageBox(_T("Imei ����Ҫ�� 15 λ hex ��!"), _T("����"), MB_OK|MB_ICONSTOP);
		m_edImei.SetFocus();
		m_edImei.SetSel(0, -1);
		return FALSE;
	}

	// Ccid ��Ч�Լ�� 20 λ Hex
#if defined(_UNICOM_CCID_19_)
	if ( (20 != m_szCcid.GetLength()) && (19 != m_szCcid.GetLength()) && !m_szCcid.IsHexNumber())
#else
	if ( (20 != m_szCcid.GetLength()) && !m_szCcid.IsHexNumber())
#endif
	{
		MessageBox(_T("Ccid ����Ҫ�� 19����20 λ hex ��!"), _T("����"), MB_OK|MB_ICONSTOP);
		m_edCcid.SetFocus();
		m_edCcid.SetSel(0, -1);
		return FALSE;
	}

	return TRUE;
}

void  CDlgInvalidCardPrint::EnableEditId(BOOL bEnable)
{
	m_edTel.EnableWindow(bEnable);
	m_edImei.EnableWindow(bEnable);
	m_edCcid.EnableWindow(bEnable);
	m_btPrint.EnableWindow(bEnable);
}

void  CDlgInvalidCardPrint::StoreItemToDb(void)
{//INSERT INTO InValidCard ( IMEI, CCID, TELNUM, BUYER, AREA ) VALUES ('123456789012345', '12345678901234567890', '12345678901', 'NC007', '����')
	CString  szSql = _T("INSERT INTO InValidCard ( IMEI, CCID, TELNUM, BUYER, AREA ) VALUES ('");
	//char szSql[1024] = "";
	if (m_pAdodb && m_pAdodb->IsOpen())
	{

		szSql += m_szImei;
		szSql += _T("', '");
		szSql += m_szCcid;
		szSql += _T("', '");
		szSql += m_szTel;
		szSql += _T("', '");
		szSql += m_szBuyer;
		szSql += _T("', '");
		szSql += m_szArea;
		szSql += _T("')");
		//MessageBox(m_szImei);
		//MessageBox(m_szCcid);
		//MessageBox(m_szTel);
		//MessageBox(m_szBuyer);
		//MessageBox(m_szArea);
		//MessageBox(szSql);

		m_pAdodb->Execute(szSql);
	}
}

void  CDlgInvalidCardPrint::ResetEditId(void)
{
	m_szTel.Empty();
	m_szImei.Empty();
	m_szCcid.Empty();

	//m_edTel.SetWindowText(m_szTel);
	//m_edImei.SetWindowText(m_szImei);
	//m_edCcid.SetWindowText(m_szCcid);

	m_edTel.SetFocus();
	m_edTel.SetSel(0, -1);
	CDialog::UpdateData(FALSE);
}
#if 0
void CDlgInvalidCardPrint::OnBnClickedBtInvalidPrint()
{
	StoreItemToDb();
}
#else
void CDlgInvalidCardPrint::OnBnClickedBtInvalidPrint()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	char  PrintCmd[HALF_KILO*2] = {0};
	char  szTel[12]             = {0};
	char  szImei[16]            = {0};
	char  szCcid[21]            = {0};

	CDialog::UpdateData(TRUE);
	if (!IsValidSetting())
	{
		return;
	}

	EnableEditId(FALSE);

	strncpy_s(szTel,  12, m_szTel,  m_szTel.GetLength());
	strncpy_s(szImei, 16, m_szImei, m_szImei.GetLength());
	strncpy_s(szCcid, 21, m_szCcid, m_szCcid.GetLength());

	CreateBACodeItemALine(PrintCmd,
		                  szTel,
						  "TELNUM",
						  szImei,
						  "IMEI",
						  szCcid,
						  "CCID",
						  m_font,
						  m_by,
						  m_X_coordinate,
						  m_Y_coordinate,
						  m_OverBarcode,
						  m_nShare);

	ExecutePrintCmdInLpt(PrintCmd, "LPT1");

	StoreItemToDb();

	EnableEditId(TRUE);

	ResetEditId();
}
#endif

void CDlgInvalidCardPrint::OnEnChangeEditInvalidTel()
{
	CDialog::UpdateData(TRUE);
	if (m_szTel.GetLength() > 10)
	{
		m_edImei.SetFocus();
		m_edImei.SetSel(0, -1);
	}
}

void CDlgInvalidCardPrint::OnEnChangeEditInvalidImei()
{
	CDialog::UpdateData(TRUE);
	if (m_szImei.GetLength() > 14)
	{
		m_edCcid.SetFocus();
		m_edCcid.SetSel(0, -1);
	}
}

void CDlgInvalidCardPrint::OnEnChangeEditInvalidCcid()
{
	CDialog::UpdateData(TRUE);
	if (m_szCcid.GetLength() > 19)
	{
		m_btPrint.SetFocus();
	}
}
