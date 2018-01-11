// E:\CvsRoot\ZebraPrinter\SRC\PAGE\QcsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include ".\QcsDlg.h"


// CQcsDlg dialog

IMPLEMENT_DYNAMIC(CQcsDlg, CDialog)
CQcsDlg::CQcsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQcsDlg::IDD, pParent)
	, m_sBoxId(_T(""))
	, m_sModel(_T(""))
	, m_sEdition(_T(""))
	, m_sReq(_T(""))
	, m_sOfm(_T(""))
	, m_sDate(_T("YYYY-MM-DD"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CQcsDlg::~CQcsDlg()
{
}

void CQcsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_QCS_BOXID, m_eBoxId);
	DDX_Text(pDX, IDC_EDIT_QCS_BOXID, m_sBoxId);
	DDX_Control(pDX, IDC_EDIT_QCS_MODEL, m_eModel);
	DDX_Text(pDX, IDC_EDIT_QCS_MODEL, m_sModel);
	DDX_Control(pDX, IDC_EDIT_QCS_EDITION, m_eEdition);
	DDX_Text(pDX, IDC_EDIT_QCS_EDITION, m_sEdition);
	DDX_Control(pDX, IDC_EDIT_QCS_REQ, m_eReq);
	DDX_Text(pDX, IDC_EDIT_QCS_REQ, m_sReq);
	DDX_Control(pDX, IDC_EDIT_QCS_OFM, m_eOfm);
	DDX_Text(pDX, IDC_EDIT_QCS_OFM, m_sOfm);
	DDX_Control(pDX, IDC_EDIT_QCS_DATE, m_eDate);
	DDX_Text(pDX, IDC_EDIT_QCS_DATE, m_sDate);
}


BEGIN_MESSAGE_MAP(CQcsDlg, CDialog)
END_MESSAGE_MAP()

#ifndef _VK_RETURN_EXIT_
BOOL CQcsDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message){//������Ϣ
		if (VK_RETURN == pMsg->wParam){// �س���
			return TRUE;
		}else{// if (VK_RETURN == pMsg->wParam){
			return CDialog::PreTranslateMessage(pMsg); 
		}// if (VK_RETURN == pMsg->wParam){}else{}
	}else{// if (WM_KEYDOWN == pMsg->message){
		return CDialog::PreTranslateMessage(pMsg); 
	}// if (WM_KEYDOWN == pMsg->message){}else{}
}
#endif//_VK_RETURN_EXIT_
// CQcsDlg message handlers
BOOL CQcsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_eBoxId.SetToolTipText(_T("��������Ҫ��ѯ�����,��������!"));
	m_eModel.SetToolTipText(_T("��������Ҫ��ѯ���ͺ�,��������!"));
	m_eEdition.SetToolTipText(_T("��������Ҫ��ѯ������,��������!"));
	m_eReq.SetToolTipText(_T("��������Ҫ��ѯ���ͼ��֪ͨ��,��������!"));
	m_eOfm.SetToolTipText(_T("��������Ҫ��ѯ�Ĺ���,��������!"));
	m_eDate.SetToolTipText(_T("��������Ҫ��ѯ������(��ʽ:YYYY-MM-DD),��������!"));


	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;
}