// CCIDcompareDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Src\\MyButton.h"
#include "Src\\myglobals.h"
#include "Src\\Comm.h"

// CCCIDcompareDlg �Ի���
class CCCIDcompareDlg : public CDialog
{
// ����
public:
	CCCIDcompareDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CCIDCOMPARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CString M_STRDBSRC;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox      m_ctrlport;
	CString        m_port;
	CComboBox      m_ctrlrate;
	CString        m_rate;
	CMyButton      m_button;
	afx_msg void OnBnClickedOk();	
	afx_msg void OnBnClickedButton1();
};
