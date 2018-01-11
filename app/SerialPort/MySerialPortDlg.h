// MySerialPortDlg.h : header file
//

#if !defined(AFX_MYSERIALPORTDLG_H__0DB91B6C_4710_426A_B610_6367F7AE19AD__INCLUDED_)
#define AFX_MYSERIALPORTDLG_H__0DB91B6C_4710_426A_B610_6367F7AE19AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMySerialPortDlg dialog
#include "Serial.h"
#define	WM_COMM_MESSAGE WM_USER+1


class CMySerialPortDlg : public CDialog
{
// Construction
public:
	CMySerialPortDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMySerialPortDlg)
	enum { IDD = IDD_MYSERIALPORT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySerialPortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMySerialPortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTestButton();
	afx_msg void OnCloseButton();
	afx_msg void OnSendButton();
	//}}AFX_MSG
	afx_msg LRESULT OnCommMessage(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void ShowMessage(CString strMessage);
	CSerial serial;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSERIALPORTDLG_H__0DB91B6C_4710_426A_B610_6367F7AE19AD__INCLUDED_)
