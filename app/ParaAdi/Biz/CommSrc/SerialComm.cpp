// SerialComm.cpp : implementation file
//

#include "stdafx.h"
#include "SerialComm.h"

#include "MainContent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if !defined(SYS_NAME)
#if defined(R16_ATC)
#define SYS_NAME  _T("BinPhoneManagerR16")
#else
#define SYS_NAME  _T("BinPhoneManagerR10")
#endif
#endif

UINT MonitorProc(LPVOID pParam);
UINT CommWriteProc(LPVOID pParam);

/////////////////////////////////////////////////////////////////////////////
// CSerialComm
extern CSerialComm MainSerial;
extern CMainContent MainContent;
CSerialComm::CSerialComm()
{
	m_bConnected        = FALSE;
	memset(m_sPort, '\0', MAXLENGTH);
	m_pMonitorThread    = NULL;
	m_pWriteThread      = NULL;
	m_bDataRxBufferFree = TRUE;


	// added by Huang ye for activate commport
	bActivated          = FALSE;
	bDeleteBufferATCommand = TRUE;
	// added by Huang ye for activate commport
	m_pDataRxBufferInUseVar = &ReceiveBuffer[0];
    m_pDataRxBufferUnUseVar = &ReceiveBuffer[1];
	m_pDataRxBufferInUseVar ->length = 0;
	m_pDataRxBufferUnUseVar ->length = 0;

	m_hWriteSemaphore = CreateSemaphore(
		NULL,  // pointer to security attributes
		0,     // initial count
		1,     // maximum count
		NULL); // pointer to semaphore-object name

	m_hMutex = CreateMutex(NULL, FALSE, NULL);

	//ΪWM_COMMNOTIFY��Ϣ�����¼�����,�ֹ�����,���Ի�Ϊ���źŵ�
	if((m_hPostMsgEvent = CreateEvent(NULL, TRUE, TRUE, NULL)) == NULL)
	{
		MessageBox("Create m_hPostMsgEvent event object failing!", SYS_NAME, MB_ICONSTOP);
	}

	memset(&m_osRead, 0, sizeof(OVERLAPPED));
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));

	//Ϊ���ö������¼������ֹ����ã����Ի�Ϊ���źŵ�
	if((m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)
	{
		MessageBox("Create read event object failing!", SYS_NAME, MB_ICONSTOP);
	}

	//Ϊ����д�����¼������ֹ����ã����Ի�Ϊ���źŵ�
	if((m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)
	{
		MessageBox("Create write event object failing!", SYS_NAME, MB_ICONSTOP);
	}	
}

CSerialComm::~CSerialComm()
{
	if(m_bConnected)
		CloseConnection();

	if(m_hPostMsgEvent)
		CloseHandle(m_hPostMsgEvent);
	if(m_osRead.hEvent)
		CloseHandle(m_osRead.hEvent);
	if(m_osWrite.hEvent)
		CloseHandle(m_osWrite.hEvent);

	//// CS added to avoid resource leaks
	if(m_hWriteSemaphore)
		CloseHandle(m_hWriteSemaphore);
	if(m_hMutex)
		CloseHandle(m_hMutex);
}


BEGIN_MESSAGE_MAP(CSerialComm, CStatic)
	//{{AFX_MSG_MAP(CSerialComm)
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialComm message handlers
/*�򿪴��ڡ������ʧ�ܣ�����FALSE,���򷵻�TRUE*/
BOOL CSerialComm::OpenConnection()
{
	extern CMainContent MainContent;
	COMMTIMEOUTS        TimeOuts;

	if(m_bConnected) 
	{
		return TRUE;
	}
	CString ComPort;
	ComPort=MainContent.m_SelComPort;
	if(ComPort.IsEmpty())
	{
		return FALSE;
	}

	ComPort=CString("\\\\.\\")+ComPort;
	/*��ȷִ�к�TR����*/
	m_hCom = CreateFile(ComPort, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, /*FILE_ATTRIBUTE_NORMAL |*/ 
		FILE_FLAG_OVERLAPPED, NULL);   //modified by cyh at 03-04-18

	if(m_hCom == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	SetupComm(m_hCom, MAXRECEIVECHARNUM, MAXSENDCHARNUM);
	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | 
		PURGE_TXCLEAR | PURGE_RXCLEAR);
	// Ϊ�����USBģ�´��ڵĳ�������EV_RING�źţ�����ȡ�����ж�����
	//SetCommMask(m_hCom, EV_RXCHAR | EV_TXEMPTY | EV_CTS | 
	//	EV_BREAK | EV_DSR | EV_ERR | EV_RING | EV_RLSD);
	SetCommMask(m_hCom, EV_RXCHAR | EV_TXEMPTY | EV_CTS | 
		EV_BREAK | EV_DSR | EV_ERR | EV_RLSD);
	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | 
		PURGE_TXCLEAR | PURGE_RXCLEAR);

	TimeOuts.ReadIntervalTimeout        = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant   = 0;
	
	TimeOuts.WriteTotalTimeoutMultiplier = 50;
	TimeOuts.WriteTotalTimeoutConstant   = 2000;
	SetCommTimeouts(m_hCom, &TimeOuts);

	if(ConfigConnection())
	{				
		m_pMonitorThread = AfxBeginThread(MonitorProc, this, 
			THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);

		if(m_pMonitorThread == NULL)
		{
			CloseHandle(m_hCom);
			return FALSE;
		}
		else
		{
			m_bConnected = TRUE;
			EscapeCommFunction(m_hCom,SETDTR);
			EscapeCommFunction(m_hCom,SETRTS);
			m_pMonitorThread->ResumeThread();
			//COMSTAT     ComStat;
	        //DWORD       dwErrorFlags;
			//ClearCommError(MainSerial.m_hCom, &dwErrorFlags, &ComStat);
			
		
		}
	}
	else
	{
		CloseHandle(m_hCom);
		return FALSE;
	}
	return TRUE;
}

BOOL CSerialComm::ConfigConnection()
{
	DCB dcb;

	if(!GetCommState(m_hCom, &dcb))
		return FALSE;

	dcb.fBinary  = TRUE;

//	dcb.BaudRate = CONNECTBAUD;

	dcb.BaudRate = atol(MainContent.m_strBaudRate);

	dcb.ByteSize = CONNECTDATABITS;
	dcb.fParity  = FALSE;//TRUE; modified by cyh at 03-04-18
	switch(CONNECTPARITY)
	{
	case 0: dcb.Parity = NOPARITY;
		break;
	case 1: dcb.Parity = EVENPARITY;
		break;
	case 2: dcb.Parity = ODDPARITY;
		break;
	default:;
	}

	switch(CONNECTSTOPBITS)
	{
	case 0: dcb.StopBits = ONESTOPBIT;
		break;
	case 1: dcb.StopBits = ONE5STOPBITS;
		break;
	case 2: dcb.StopBits = TWOSTOPBITS;
		break;
	default:;
	}
	/*added by cyh at 09-08*/
	 
	dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
	/*added by cyh at 09-08*/

	//Ϊ������������ģ������USBģ�´��ڵ����ʱ�������޷�������
	/*
	dcb.fOutxCtsFlow = CONNECTFLOWCTRL == 1;
	dcb.fRtsControl  = CONNECTFLOWCTRL == 1?
		RTS_CONTROL_HANDSHAKE:RTS_CONTROL_ENABLE;

	dcb.fInX = dcb.fOutX = CONNECTFLOWCTRL == 2;
	dcb.XonChar  = XON;
	dcb.XoffChar = XOFF;
	dcb.XonLim   = 50;
	dcb.XoffLim  = 50;
	*/
	return SetCommState(m_hCom, &dcb);
}

UINT MonitorProc(LPVOID pParam)
{
	OVERLAPPED  os;
	DWORD       dwMask, dwTrans, ModemStat;
	COMSTAT     ComStat;
	DWORD       dwErrorFlags;
	
	memset(&os, 0, sizeof(OVERLAPPED));
	os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(os.hEvent == NULL)
	{
		MessageBox(NULL, "Create overlappped event Error!", SYS_NAME, MB_ICONSTOP);
		return (UINT)-1;
	}
	while(MainSerial.m_bConnected)
	{
		ClearCommError(MainSerial.m_hCom, &dwErrorFlags, 
			&ComStat);
		if(ComStat.cbInQue)    // ��ѯ���뻺�������Ƿ����ַ�
		{
			WaitForSingleObject(MainSerial.m_hPostMsgEvent, 
				INFINITE);
			ResetEvent(MainSerial.m_hPostMsgEvent);

			MainSerial.m_hMainWnd->PostMessage(
				WM_COMMNOTIFY, 
				EV_RXCHAR,
				0);
			continue;
		}

		if(!ComStat.cbOutQue)  // ��ѯ����������Ƿ�Ϊ��
		{
			ReleaseSemaphore(MainSerial.m_hWriteSemaphore, 1, NULL);
		}
		
		dwMask    = 0;
		ModemStat = 0;
		if(!WaitCommEvent(MainSerial.m_hCom, &dwMask, &os))
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				if(MainContent.bIfIsWinNt)
					GetOverlappedResult(MainSerial.m_hCom, &os, 
					&dwTrans, TRUE);
				else
					GetOverlappedResult(MainSerial.m_hCom, &os, 
					&dwTrans, FALSE);
			}
			else
			{
				CloseHandle(os.hEvent);
				return (UINT)-1;
			}
		}
		if((dwMask & EV_CTS)==EV_CTS)
		{
			if(GetCommModemStatus(MainSerial.m_hCom, &ModemStat))
			{
				if((ModemStat & MS_CTS_ON) == MS_CTS_ON)
				{
					//MainSerial.ModemCt106On="���ڿ�����CTS (CT106)�ź�ΪON!";
					//PostMessage(MainSerial.m_hTermWnd, WM_ECHOSTRING, 0, MODEMCT106ON);
				}
				else
				{
					//MainSerial.ModemCt106Off = "���ڿ�����CTS (CT106)�ź�ΪOFF!";
					//PostMessage(MainSerial.m_hTermWnd,WM_ECHOSTRING,0,MODEMCT106OFF);
				}
			}
		}

		if((dwMask & EV_DSR) == EV_DSR)
		{
			if(GetCommModemStatus(MainSerial.m_hCom, &ModemStat))
			{
				if((ModemStat & MS_DSR_ON) == MS_DSR_ON)
				{
					//MainSerial.ModemCt107On="���ڿ�����DSR (CT107)�ź�ΪON!";
					//PostMessage(MainSerial.m_hTermWnd,WM_ECHOSTRING,0,MODEMCT107ON);
				}
				else
				{
					//MainSerial.ModemCt107Off="���ڿ�����DSR (CT107)�ź�ΪOFF!";
					//PostMessage(MainSerial.m_hTermWnd,WM_ECHOSTRING,0,MODEMCT107OFF);
				}
			}
		}

		if((dwMask & EV_ERR)==EV_ERR)
		{
			//MainSerial.ModemError = "����һ�����ݴ������!";
			//PostMessage(MainSerial.m_hTermWnd,WM_ECHOSTRING,0,MODEMERROR);
		}

		if((dwMask & EV_RING)==EV_RING)
		{
			if(GetCommModemStatus(MainSerial.m_hCom, &ModemStat))
			{
				if((ModemStat & MS_RING_ON) == MS_RING_ON)
				{
					//MainSerial.ModemCt125On="���ڿ�����CI (CT125)�ź�ΪON!";
					//PostMessage(MainSerial.m_hTermWnd,WM_ECHOSTRING,0,MODEMCT125ON);
				}
				else
				{
					//MainSerial.ModemCt125Off = "���ڿ�����CI (CT125)�ź�ΪOFF!";
					//PostMessage(MainSerial.m_hTermWnd, WM_ECHOSTRING,0,MODEMCT125OFF);
				}
			}
		}

		if((dwMask & EV_RLSD) == EV_RLSD)
		{
			if(GetCommModemStatus(MainSerial.m_hCom, &ModemStat))
			{
				if((ModemStat & MS_RLSD_ON) == MS_RLSD_ON)
				{
					//MainSerial.ModemCt109On = "���ڿ�����DCD (CT109)�ź�ΪON!";
					//PostMessage(MainSerial.m_hTermWnd,WM_ECHOSTRING,0,MODEMCT109ON);
				}
				else
				{
					//MainSerial.ModemCt109Off = "���ڿ�����DCD (CT109)�ź�ΪOFF!";
					//PostMessage(MainSerial.m_hTermWnd,WM_ECHOSTRING,0,MODEMCT109OFF);
				}
			}
		}
		if((dwMask & EV_BREAK)==EV_BREAK)
		{
			//MainSerial.ModemBreak="���ڷ���BREAK�ź�!";
			//PostMessage(MainSerial.m_hTermWnd, WM_ECHOSTRING, 0,MODEMBREAK);
		}
	}
	CloseHandle(os.hEvent);
	return 0;
}

void CSerialComm::CloseConnection()
{
	if(!m_bConnected)
		return;

	SetEvent(m_hPostMsgEvent);  // �ѽ��ջ���Ĳ������ݽ�����
	//SetCommMask(m_hCom, 0);

	// ��������ʱ����ǿ���ж�
	//WaitForSingleObject(m_pThread->m_hThread,INFINITE);
	PurgeComm(m_hCom, PURGE_TXABORT | PURGE_RXABORT | 
		PURGE_TXCLEAR | PURGE_RXCLEAR);

	m_bConnected     = FALSE;
	if(m_pMonitorThread!=NULL){
		TerminateThread(m_pMonitorThread->m_hThread,0);
	}
	m_pMonitorThread = NULL;
	
	CloseHandle(m_hCom);
	m_hCom = NULL;
	Sleep(50);
}

DWORD CSerialComm::WriteComm(char *buf, DWORD dwLength)
{
	BOOL    fState;
	DWORD   length = dwLength;
	COMSTAT ComStat;
	DWORD   dwErrorFlags;

	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	fState = WriteFile(m_hCom, buf, length, &length, &m_osWrite);
    
	if(!fState)
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			if(MainContent.bIfIsWinNt)
				GetOverlappedResult(m_hCom, &m_osWrite, &length, TRUE);
			else
				GetOverlappedResult(m_hCom, &m_osWrite, &length, FALSE);
		}
		else
			length=0;
	}
	
	return length;
}

void CSerialComm::initialize(CWnd *cwnd)
{
	m_hMainWnd = cwnd;	
}

DWORD CSerialComm::ReadComm(char *buf, DWORD dwLength)
{
	DWORD   length = 0;
	COMSTAT ComStat;
	DWORD   dwErrorFlags;

	ClearCommError(m_hCom, &dwErrorFlags, &ComStat);
	length = min(dwLength, ComStat.cbInQue);
	ReadFile(m_hCom, buf, length, &length, &m_osRead);

	return length;
}

void CSerialComm::SwapDataRxBuffers()
{
	Buffer *temp;
    temp = m_pDataRxBufferUnUseVar;
    m_pDataRxBufferUnUseVar = m_pDataRxBufferInUseVar;
    m_pDataRxBufferInUseVar = temp;
}

void CSerialComm::StartToSend(char *string, int length)
{
	DWORD dw;

	// Wait forever for the mutex to become signaled.
	dw = WaitForSingleObject(m_hMutex, INFINITE);

	if(dw == WAIT_OBJECT_0)
	{
		// Mutex became signaled.
		m_sendingdata.sendlength = length; 
		m_sendingdata.senddata   = string;

		m_pWriteThread = AfxBeginThread(
			CommWriteProc, 
			&m_sendingdata, 
			THREAD_PRIORITY_NORMAL, 
			0,
			0,
			NULL);		
	}
}
UINT CommWriteProc(LPVOID pParam)
{
	char *TempFilep;
	SerialSend *sending = (SerialSend *)pParam;

	TempFilep = sending->senddata;
    while(sending->sendlength)
	{
		WaitForSingleObject(MainSerial.m_hWriteSemaphore, 
			INFINITE);

		if(sending->sendlength > MAXSENDCHARNUM)
		{
			sending->sendlength -= MAXSENDCHARNUM;
			MainSerial.WriteComm(TempFilep, MAXSENDCHARNUM);
			TempFilep += MAXSENDCHARNUM;
		}
		else
		{
			MainSerial.WriteComm(TempFilep, sending->sendlength);
			sending->sendlength = 0;
		}
	}

	// ��������Ҫȷ��Ӳ���������е������Ѿ��������
	WaitForSingleObject(MainSerial.m_hWriteSemaphore, INFINITE);
	ReleaseSemaphore(MainSerial.m_hWriteSemaphore, 1, NULL);

	// ֪ͨ���ڷ����̣߳���ǰ�������Ѿ��������
	ReleaseMutex(MainSerial.m_hMutex);

	return 0;
}
