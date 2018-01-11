// CCIDcompareDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Src\\MyButton.h"
#include "Src\\myglobals.h"
#include "Src\\Comm.h"

// CCCIDcompareDlg 对话框
class CCCIDcompareDlg : public CDialog
{
// 构造
public:
	CCCIDcompareDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CCIDCOMPARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CString M_STRDBSRC;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
