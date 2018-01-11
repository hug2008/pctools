#if !defined(AFX_MAINCONTENT_H__3AB03A33_06B1_4F34_97E4_2D61240E4272__INCLUDED_)
#define AFX_MAINCONTENT_H__3AB03A33_06B1_4F34_97E4_2D61240E4272__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainContent.h : header file
//
#include "PARADEFINE.H"
/////////////////////////////////////////////////////////////////////////////
// CMainContent window

class CMainContent : public CStatic
{
// Construction
public:
	CMainContent();
public:
	
    CString m_SelComPort;
	CString m_strBaudRate;
	
	CWinThread * m_pFIFOThread;
	CWinThread * m_pCommThread;
	SendData   * m_pMainSendData;

	CString m_sOldPass;

	CString m_sNewPass1;
	CString m_sNewPass2;
// Attributes
public:
	// 用于同步独立收发线程发送数据事件对象
	volatile  HANDLE m_hPostSendingEvent;

	
	// 向移动终端发送数据包时监控定时器的时间大小
	int     m_nMonitorDelay;

	BOOL bIfIsWinNt;
	BOOL bIfHaveInit;
	BOOL        m_bDisplayOnceTime;

private:


// Operations
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainContent)
	//}}AFX_VIRTUAL

// Implementation
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainContent)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_sWorkPath;
	CString GetPathFromStr(CString s);
	CString GetWorkPath();
		
	virtual ~CMainContent();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainContent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
private:
	

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINCONTENT_H__3AB03A33_06B1_4F34_97E4_2D61240E4272__INCLUDED_)
