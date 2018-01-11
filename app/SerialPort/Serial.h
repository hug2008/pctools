// Serial.h: interface for the CSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIAL_H__7522BCB7_A45C_4ADC_9F97_22DFE84F05BF__INCLUDED_)
#define AFX_SERIAL_H__7522BCB7_A45C_4ADC_9F97_22DFE84F05BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DATA_READ_TIMEOUT	      100   // Milliseconds
#define STATUS_CHECK_TIMEOUT      100   // Milliseconds

typedef enum WmSerialMsgTag
{
	WM_COMM_READ,
	WM_COMM_WRITE
} WmSerialMsg;

class CSerial  
{
public:
	CSerial();
	virtual ~CSerial();
public:	
	bool SendData(char szData[],int len);
	bool ReadData(char szData[],int&len);
	BOOL Close();
	BOOL Open(HWND hWnd,UINT Msg,UINT nCom, DWORD dwBaudRate,
					BYTE byByteSize,BYTE byParity,BYTE byStopBits);
private:
	bool	m_bOpened;
	HANDLE	m_hComm;
	UINT	m_myId;

	HANDLE  m_hThreadReadData;
	HANDLE  m_hThreadAnalysis;
	
	HANDLE  m_hEvStartRead;
	HANDLE  m_hEvStopRead;
	
	HANDLE  m_hEvStopAnalysis;
	HANDLE  m_hEvStartReadFile;

	HANDLE  m_hEvRecvFinish;
	HANDLE  m_hEvAnalysisFinish;
	HANDLE  m_hEvReadFileFinish;

	HANDLE  m_hEvDataComing;
	HANDLE  m_hEvEventComing;

	HANDLE  m_hThreadSendData;
	HANDLE  m_hEvSendDataFinish;
	HANDLE  m_hEvSendDataStart;

	HWND	m_hParentWnd;
	UINT	m_uMessage;
private:
	static	UINT SendDataProc(LPVOID lpParam);
	void	ClearSysReadBufferContent();
	void	NotifyWriteData(void *lpdata);
	void	NotifyReadData();
	void	Init();
	void	UnInit();
	void	CloseAndCleanHandle(HANDLE&handle);
	static	UINT ReadEventProc(LPVOID lpParam);
	static	UINT AnalysisEventProc(LPVOID lpParam);
	static	UINT ReadDataProc(LPVOID lpParam);
};

#endif // !defined(AFX_SERIAL_H__7522BCB7_A45C_4ADC_9F97_22DFE84F05BF__INCLUDED_)
