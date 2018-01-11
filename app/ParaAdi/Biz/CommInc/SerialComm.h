#if !defined(AFX_SERIALCOMM_H__8F0BCE2E_71FB_4DFC_ADE8_7E054B7FA982__INCLUDED_)
#define AFX_SERIALCOMM_H__8F0BCE2E_71FB_4DFC_ADE8_7E054B7FA982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialComm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSerialComm window

//#include "MainContent.h"

#include "PARADEFINE.H"
class CSerialComm : public CStatic
{
// Construction
public:
	CSerialComm();

// Attributes
public:
	volatile HANDLE m_hCom;
	BOOL            m_bConnected;
	char		    m_sPort[MAXLENGTH];
	CWinThread*     m_pMonitorThread;
	volatile HANDLE m_hPostMsgEvent;
	volatile HANDLE m_hWriteSemaphore;
	volatile HWND   m_hMainViewWnd;
	CWinThread*     m_pWriteThread;
	OVERLAPPED      m_osRead, m_osWrite;
	CWnd*           m_hMainWnd;
	BOOL            m_bDataRxBufferFree;
	SerialSend      m_sendingdata;
	HANDLE          m_hMutex;

	typedef struct BufferTag
	{
		int  length;                        
		char data[MAXRECEIVECHARNUM];
	}Buffer;
	// added by Huang ye for activate commport
	typedef enum AfterActivateCommandTypeTag
	{
		READDATA_CMD_DATAPAGE,
		WRITEDATA_CMD_DATAPAGE,
		CHANDEPASSWORD_CMD_DATAPAGE,
		CHANGEANDDOWNLOAD_CMD_DATAPAGE,
		PRODUCTDOWNLOAD_CMD_DATAPAGE,
		SETDEFAULTSETTING_CMD_DATAPAGE,
		RECOVERDEFAULTSETTING_CMD_DATAPAGE
	}AfterActivateCommandType;
	
	
	AfterActivateCommandType   afterActivateCommandTye;
	// added by Huang ye for activate commport
	
	Buffer ReceiveBuffer[2];
	
	// added by Huang ye for activate commport
	BOOL         bActivated;
	BOOL         bDeleteBufferATCommand;
	// added by Huang ye for activate commport

	
	Buffer *m_pDataRxBufferInUseVar;
    Buffer *m_pDataRxBufferUnUseVar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialComm)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void  StartToSend(char * string, int length);
	void  SwapDataRxBuffers (void);
	DWORD ReadComm (char *buf,DWORD dwLength);
	void  initialize(CWnd* cwnd);
	DWORD WriteComm(char *buf,DWORD dwLength);
	void  CloseConnection();
	BOOL  ConfigConnection();
	BOOL  OpenConnection();
	virtual ~CSerialComm();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSerialComm)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALCOMM_H__8F0BCE2E_71FB_4DFC_ADE8_7E054B7FA982__INCLUDED_)
