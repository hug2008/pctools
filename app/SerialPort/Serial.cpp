// Serial.cpp: implementation of the CSerial class.
// download by http://www.codefans.net
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Serial.h"

#include "DataStruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUm_NEW
#endif

//全局变量，暂时存储数据用的
static DATA_GROUP	gDataRecv;
static DATA_GROUP	gDataSend;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerial::CSerial()
{
	Init();
}

CSerial::~CSerial()
{
	UnInit();
}
/******************************************************************************
*功能：初始化
*说明：
*参数：
*返回：
******************************************************************************/
void CSerial::Init()
{
	m_hParentWnd		= NULL;
	m_uMessage			= 0;

	m_bOpened			=  false;
	m_hComm				= INVALID_HANDLE_VALUE;

	m_hThreadReadData	= CreateEvent(0, true, 0, _T(""));
	m_hThreadAnalysis 	= CreateEvent(0, true, 0, _T(""));
	
	m_hEvStartRead		= CreateEvent(0, true, 0, _T(""));
	m_hEvStopRead		= CreateEvent(0, true, 0, _T(""));
	
	m_hEvStopAnalysis	= CreateEvent(0, true, 0, _T(""));
	m_hEvStartReadFile	= CreateEvent(0, true, 0, _T(""));

	m_hEvRecvFinish		= CreateEvent(0, true, 0, _T(""));
	m_hEvAnalysisFinish	= CreateEvent(0, true, 0, _T(""));
	m_hEvReadFileFinish	= CreateEvent(0, true, 0, _T(""));

	m_hEvDataComing		= CreateEvent(0, true, 0, _T(""));
	m_hEvEventComing	= CreateEvent(0, true, 0, _T(""));

	m_hThreadSendData	= CreateEvent(0, true, 0, _T(""));
	m_hEvSendDataFinish	= CreateEvent(0, true, 0, _T(""));
	m_hEvSendDataStart	= CreateEvent(0, true, 0, _T(""));
}
/******************************************************************************
*功能：释放资源
*说明：
*参数：
*返回：
******************************************************************************/
void CSerial::UnInit()
{
	Close();

	//关闭一直存活的事件对象
	CloseAndCleanHandle(m_hEvStartRead);
	CloseAndCleanHandle(m_hThreadReadData);
	CloseAndCleanHandle(m_hThreadAnalysis);
	CloseAndCleanHandle(m_hEvStopRead);
	CloseAndCleanHandle(m_hEvStopAnalysis);
	CloseAndCleanHandle(m_hEvStartReadFile);
	CloseAndCleanHandle(m_hEvRecvFinish);
	CloseAndCleanHandle(m_hEvAnalysisFinish);
	CloseAndCleanHandle(m_hEvReadFileFinish);
	CloseAndCleanHandle(m_hEvDataComing);
	CloseAndCleanHandle(m_hEvEventComing);

	CloseAndCleanHandle(m_hThreadSendData);
	CloseAndCleanHandle(m_hEvSendDataFinish);
	CloseAndCleanHandle(m_hEvSendDataStart);
}
/******************************************************************************
*功能：关闭串口
*说明：
*参数：
*返回：
******************************************************************************/
BOOL CSerial::Close()
{
	if(!m_bOpened)
	{
		return TRUE;
	}

	m_bOpened = false;

	HANDLE handle[4] = {0};

	handle[0] = m_hEvRecvFinish;
	handle[1] = m_hEvAnalysisFinish;
	handle[2] = m_hEvReadFileFinish;
	handle[3] = m_hEvSendDataFinish;

	SetEvent(m_hEvStopRead);
	SetEvent(m_hThreadSendData);

	::WaitForMultipleObjects(4,	handle,	TRUE,	INFINITE);

	TRACE("已经关闭了四个线程！\n");

	Sleep(100);

	CloseAndCleanHandle(m_hComm);

	//关闭串口时，要重置一下这个信号
	ResetEvent(m_hEvStartRead);
	ResetEvent(m_hThreadReadData);
	ResetEvent(m_hThreadAnalysis);
	ResetEvent(m_hEvStopRead);
	ResetEvent(m_hEvStopAnalysis);
	ResetEvent(m_hEvStartReadFile);
	ResetEvent(m_hEvRecvFinish);
	ResetEvent(m_hEvAnalysisFinish);
	ResetEvent(m_hEvReadFileFinish);
	ResetEvent(m_hEvDataComing);
	ResetEvent(m_hEvEventComing);

	ResetEvent(m_hThreadSendData);
	ResetEvent(m_hEvSendDataFinish);
	ResetEvent(m_hEvSendDataStart);

	return TRUE;
}
/******************************************************************************
	*功能：关闭并非法化句柄
	*说明：
	*参数：
	*返回：
******************************************************************************/
void CSerial::CloseAndCleanHandle(HANDLE &handle)
{
	ASSERT(handle);

	if(handle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	BOOL bRet = CloseHandle(handle);

	if(!bRet)
	{
		TRACE0( "close handle fail!\n" );
		ASSERT(0);
	}
	else
	{
		handle  =  INVALID_HANDLE_VALUE;
	}
}
/******************************************************************************
	*功能：打开串口
	*说明：
	*参数：	nCom		：打开哪一个串口--------1,2
		dwBaudRate	：波特率----------------9600?
		byByteSize	：数据位----------------8
		byParity	：奇偶较验位------------0
		byStopBits	：停止位----------------1
	*返回： 如果之前已经打开，TRUE，但参数不是最新设置的，而是上次打开串口时的数值；
	如果之前没有打开串口：
	成功，TRUE，失败，FALSE
******************************************************************************/
BOOL CSerial::Open(HWND hWnd,
				   UINT Msg,
				   UINT nCom,
				   DWORD dwBaudRate,
				   BYTE byByteSize,
				   BYTE byParity,
				   BYTE byStopBits)
{
	if(m_bOpened)
	{
		return TRUE;
	}

	m_hParentWnd = hWnd;
	m_uMessage 	 = Msg;
	m_myId		 = nCom;

	CString szPortName("COM%d",nCom);

	try
	{
		//打开串口
		m_hComm  =  ::CreateFile(
			szPortName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			0,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			0);
		if(m_hComm == INVALID_HANDLE_VALUE)
		{
			TRACE("open com port failed!\n");
			CloseAndCleanHandle(m_hComm);
			return FALSE;
		}
		TRACE("open com port success!\n");

		//设置过滤事件
		if(!::SetCommMask(m_hComm,EV_RXCHAR | EV_ERR))//| EV_TXEMPTY 
		{
			TRACE("setcommmask fail!\n");
			CloseAndCleanHandle(m_hComm);
			return FALSE;
		}
		TRACE("SetCommMask success!\n");

		//设置DCB结构
		DCB dcb;
		memset(&dcb,0,sizeof(DCB));

		if(!::GetCommState(m_hComm,&dcb))
		{
			TRACE("GetCommState(m_hComm,&dcb) failed!\n");
			CloseAndCleanHandle(m_hComm);
			return FALSE;
		}
		TRACE("GetCommState success!\n");

		dcb.BaudRate	 =  dwBaudRate;
		dcb.ByteSize	 =  byByteSize;
		dcb.Parity		 =  byParity;
		if(byStopBits == 1)
		{
			dcb.StopBits  =  ONESTOPBIT;
		}
		else if(byStopBits == 2)
		{
			dcb.StopBits  =  TWOSTOPBITS;
		}
		else
		{
			dcb.StopBits  =  ONE5STOPBITS;
		}
		
		if(!::SetCommState(m_hComm,&dcb))
		{
			TRACE("SetCommState(m_hComm,&dcb)) failed!\n");
			CloseAndCleanHandle(m_hComm);
			return FALSE;
		}
		TRACE("SetCommState success!\n");

		//设置超时,我准备用事件判断接收到单个字符，所以超时这样设了
		COMMTIMEOUTS timeouts;
		memset(&timeouts,0,sizeof(COMMTIMEOUTS));
		timeouts.ReadIntervalTimeout		 =  MAXDWORD;
		timeouts.ReadTotalTimeoutConstant	 =  0;
		timeouts.ReadTotalTimeoutMultiplier	 =  0;
		timeouts.WriteTotalTimeoutConstant	 =  0;
		timeouts.WriteTotalTimeoutMultiplier =  0;

		if(!::SetCommTimeouts(m_hComm,&timeouts))
		{
			TRACE("SetCommTimeouts(m_hComm,&timeouts) failed!\n");
			CloseAndCleanHandle(m_hComm);
			return FALSE;
		}
		TRACE("SetCommTimeouts success!\n");


		AfxBeginThread(ReadEventProc,(LPVOID)this);
		AfxBeginThread(AnalysisEventProc,(LPVOID)this);
		AfxBeginThread(ReadDataProc,(LPVOID)this);
		AfxBeginThread(SendDataProc,(LPVOID)this);

		//首先启动读取数据线程，其它线程会依次被它启动
		//退出时，先退出接收事件线程，然后，事件分析线程，最后是数据接收线程
		SetEvent(m_hThreadReadData);	

		m_bOpened  =  TRUE;
	}
	catch(...)
	{
		ASSERT(0);
		CloseAndCleanHandle(m_hComm);
	}

	return TRUE;
}
/******************************************************************************
	*功能：读取数据线程
	*说明：	每接收到一个字符，都会有事件发出。这个线程把接收到的第个字符消息，都
			发送给消息分析线程。
	*参数：lpParam：即：CSerial类对象本身
	*返回：0
******************************************************************************/
UINT CSerial::ReadEventProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	bool bQuit = false;

	DWORD dwEventMask = 0;

	//接收到消息重叠
	OVERLAPPED ov = {0};	

	//读取串口事件、退出线程信号
	HANDLE mask_handle[2] = {0};

	//检查有没有消息的标志量
	DWORD dwRet;
	BOOL  bRet;
	bool  bHasMessage   = false;
	bool  bMaskOverlapping  =  false;

	//错误处理
	DWORD dwErrors = 0;
	COMSTAT comstate;
	
	memset(&ov,0,sizeof(OVERLAPPED));
	ov.hEvent	 =  CreateEvent(0, true, 0, _T("ReadEventProc"));
	
	mask_handle[0]  =  pSerial->m_hEvStopRead;
	mask_handle[1]  =  ov.hEvent;
	
	memset(&comstate,0,sizeof(COMSTAT));

	//等等待启动信号,数据处理线程启动后，就会通知接收事件消息线程启动
	::WaitForSingleObject(pSerial->m_hEvStartRead,INFINITE);
	TRACE("get event thread start!\n");

	//清空系统缓冲区
	pSerial->ClearSysReadBufferContent();

	while(!bQuit)
	{
		dwEventMask = 0;
		bRet = ::WaitCommEvent(pSerial->m_hComm,&dwEventMask,&ov);
		if(bRet)	//正好得到一个串口消息
		{
			bHasMessage = true;
		}
		else//没有得到串口消息,看看是否在进行重叠操作吧
		{
			if(GetLastError() == ERROR_IO_PENDING)//正在进行重叠操作...
			{
				bMaskOverlapping = true;//正在进行重叠操作,直接跳出，到下边的处理代码
				TRACE("已经进入到了重叠操作，errorcode = [%d][%d]\n",GetLastError(),dwEventMask);
			}
			else
			{
				TRACE("WaitCommEvent发生错误！errorcode = [%d][%d]\n",GetLastError(),dwEventMask);
				continue;
			}
		}
		if(bMaskOverlapping)	//如果是在进行叠操作,就等待...
		{
			bMaskOverlapping = false;
			dwRet = ::WaitForMultipleObjects(2, mask_handle, false,	INFINITE);
			switch(dwRet-WAIT_OBJECT_0)
			{
			case 0://退出线程
				{
					bQuit = true;
				}
				break;
			case 1://接收到消息
				{
					ResetEvent(ov.hEvent);//恢复串口事件信号
					bRet = ::GetOverlappedResult(pSerial->m_hComm,&ov,&dwErrors,false);
					if(bRet)
					{
						bHasMessage = true;
					}
				}
				break;
			default://未知信号
				{
				}
				break;
			}
		}
		if(bHasMessage)//最终，如果有消息到来
		{
			bHasMessage = false;
			
			//有消息到来，并且成功得到了消息
			if(EV_ERR  ==  (dwEventMask & EV_ERR) )	//如果发生了错误，就只处理错误
			{
				//发生错误了，要能够恢复才行
				::ClearCommError(pSerial->m_hComm,&dwErrors,&comstate);
				TRACE("出现了错误！\n");
			}
			
			else if(EV_RXCHAR  ==  (dwEventMask & EV_RXCHAR) )	//读取数据
			{
				SetEvent(pSerial->m_hEvEventComing);
			}
		}
	}

	pSerial->CloseAndCleanHandle(ov.hEvent);

	TRACE("get event thread end!\n");
	
	//通知接收数据线程关闭
	SetEvent(pSerial->m_hEvStopAnalysis);
	SetEvent(pSerial->m_hEvRecvFinish);
	
	return 0;
}
/******************************************************************************
*功能：分析是否成功接收到一个数据包
*说明：每接收到一个字符，都会有一个消息发送过来。根据两个字符间是否超时来判断是否成功接收到一个数据包
*参数：
*返回：
******************************************************************************/
UINT CSerial::AnalysisEventProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	bool bQuit = false;

	//保存WaitForMultipleObjects返回值
	DWORD dwRet;

	//用来判断之前是否有过字符消息，两个字符消息之间是否超时
	bool bHasMessageBefore		  =  false;
	
	//读取串口事件、退出线程信号
	HANDLE handle[2] = {0};

	
	handle[0]  =  pSerial->m_hEvStopAnalysis;
	handle[1]  =  pSerial->m_hEvEventComing;

	::WaitForSingleObject(pSerial->m_hThreadAnalysis,INFINITE);

	TRACE("analysis event thread start!\n");

	SetEvent(pSerial->m_hEvStartRead);//启动接收事件线程

	while(!bQuit)
	{
		dwRet = ::WaitForMultipleObjects(2,	handle,	false,	INFINITE);

		switch(dwRet-WAIT_OBJECT_0)
		{
		case 0://退出
			{
				bQuit = true;
			}
			continue;
		case 1://数据到来,进入下一级循环
			{
				TRACE0("一个数据包的第一个字符到达！\n");
				ResetEvent(handle[1]);
				if(!bHasMessageBefore)bHasMessageBefore = true;
			}
			break;
		default:
			{
				ResetEvent(handle[0]);
				ResetEvent(handle[1]);
			}
			continue;
		}
		while(!bQuit && bHasMessageBefore )
		{
			dwRet = ::WaitForMultipleObjects(2,handle,false,DATA_READ_TIMEOUT);
			if(WAIT_TIMEOUT == dwRet)//成功接收到一个数据包
			{
				if(bHasMessageBefore)
				{
					TRACE0("完整地接收到了一个数据包！通知数据接收线程进行接收！\n");
					bHasMessageBefore = false;
					SetEvent(pSerial->m_hEvDataComing);//通知数据接收线程接收数据
				}
				continue;
			}
			switch(dwRet-WAIT_OBJECT_0)
			{
			case 0://退出
				{
					bQuit = true;
				}
				break;
			case 1://数据到来
				{
					//TRACE0("事件分析线程接收到了一个字符！\n");
					ResetEvent(handle[1]);
				}
				break;
			default://出错了
				{
					ResetEvent(handle[0]);
					ResetEvent(handle[1]);
				}
				break;
			}
		}
	}

	TRACE("analysis event thread end\n");
	SetEvent(pSerial->m_hEvStartReadFile);
	SetEvent(pSerial->m_hEvAnalysisFinish);
	
	return 1;
}
/******************************************************************************
*功能：读取数据线程
*说明：	成功收到一个数据包后，这个线程会得到消息。开始读取这个数据包
成功读取一包数据后，会把数据放到g_read_data_group中，同时向调用者发出一个消息
此消息由调用者初始化时传入.
*参数：lpParam：即：CSerial类对象本身
*返回：0
******************************************************************************/
UINT CSerial::ReadDataProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	bool bQuit = false;

	DWORD dwEventMask = 0;

	//接收到重叠消息
	OVERLAPPED ov;

	//想要读取的字节数
	BOOL  bRet = FALSE;
	DWORD dwBytesWantRead = 0;
	DWORD dwBytesRead = 0;
	char  temp_buf[2*MAX_COMM_BUF_LEN] = {0};
	bool bReadOverlapping  =  false;
	bool bReadDataSuccess  =  false;

	//想知道系统读缓冲区里共有多少字节
	DWORD   dwErrors = 0;//需要这个参数，但是根本没有使用它的返回值，我们不关心它的返回值.
	COMSTAT comstate;

	//保存WaitForMultipleObjects返回值
	DWORD dwRet;

	//读取串口事件、退出线程信号
	HANDLE event_handle[2] = {0};
	HANDLE read_handle[2] = {0};


	memset(&ov,0,sizeof(OVERLAPPED));
	ov.hEvent	 =  CreateEvent(0, true, 0, _T("ReadDataProc"));
	
	event_handle[0]  =  pSerial->m_hEvStartReadFile;
	event_handle[1]  =  pSerial->m_hEvDataComing;
	
	read_handle[0]  =  pSerial->m_hEvStartReadFile;
	read_handle[1]  =  ov.hEvent;

	
	memset(&comstate,0,sizeof(COMSTAT));

	::WaitForSingleObject(pSerial->m_hThreadReadData,INFINITE);

	TRACE("read data thread start\n");

	SetEvent(pSerial->m_hThreadAnalysis);//启动分析线程

	while(!bQuit)
	{
		dwRet = ::WaitForMultipleObjects(2,	event_handle,	false,	INFINITE);
		switch(dwRet-WAIT_OBJECT_0)
		{
		case 0://退出
			{
				bQuit = true;
			}
			break;
		case 1://数据到来
			{
				ResetEvent(event_handle[1]);//每次都是先恢复信号
				//得到总计有多少字节在系统接收缓冲区里
				bRet = ::ClearCommError(pSerial->m_hComm,&dwErrors,&comstate);
				if(!bRet)
				{
					continue;//失败就丢弃！
				}
				dwBytesWantRead = comstate.cbInQue;//得到在系统读缓冲区中共有多少字节。
				bRet = ::ReadFile(pSerial->m_hComm,temp_buf,dwBytesWantRead,&dwBytesRead,&ov);
				if(bRet)	//成功读取数据
				{
					bReadDataSuccess = true;
				}
				else		//看看是否开始了重叠操作?
				{
					if(GetLastError() == ERROR_IO_PENDING)
					{
						bReadOverlapping = true;
					}
					else//发生错误
					{
						TRACE("ReadFile(pSerial->m_hComm,temp_buf,dwBytesWantRead,&dwBytesRead,&ov);\n");
						continue;
					}
				}
				if(bReadOverlapping)
				{
					bReadOverlapping = false;
					dwRet = ::WaitForMultipleObjects(2,read_handle,false,DATA_READ_TIMEOUT);
					if(dwRet == WAIT_TIMEOUT)
					{
						continue;//如果读取数据超时，就认为是错误数据，丢弃！
					}
					switch(dwRet-WAIT_OBJECT_0)
					{
					case 0://退出
						{
							bQuit = true;
						}
						break;
					case 1://读数据重叠操作完成
						{
							//							ResetEvent(read_handle[1]);//恢复读取数据信号
							//需要判断是否成功读取了数据
							bRet = ::GetOverlappedResult(pSerial->m_hComm,&ov,&dwBytesRead,false);
							if(!bRet)//读重叠操作失败！
							{
								TRACE0("读重叠操作失败！\n");
							}
							else
							{
								bReadDataSuccess = true;//成功读取了数据
							}
						}
						break;
					default:
						{
						}
						break;
					}
				}
				if(bReadDataSuccess)//如果成功读取了数据
				{
					bReadDataSuccess = false;//恢复状态
					if(dwBytesWantRead == dwBytesRead)//如果没有少读取数据字节数，才是真正的正确读取了数据
					{
						LPDATA lpdata = gDataRecv.GetBlank();

						//如果读取的字节数超过了MAX_COMM_BUF_LEN-1,则只能接收到MAX_COMM_BUF_LEN-1个字节了
						lpdata->m_n_len = (dwBytesRead > MAX_COMM_BUF_LEN-1) ? (MAX_COMM_BUF_LEN-1) : dwBytesRead;
						memcpy(lpdata->m_sz_buf, temp_buf, lpdata->m_n_len);
						lpdata->m_sz_buf[lpdata->m_n_len] = 0;
						pSerial->NotifyReadData();

						TRACE("数据接收线程接收到了一个数据包！len = [%d]\n",lpdata->m_n_len);
					}
					else
					{
						TRACE0("少读取数据字节数\n");
					}
				}
				continue;//成功读取了数据
			}
			break;
		default:
			{
			}
			break;
		}	
	}
	pSerial->CloseAndCleanHandle(ov.hEvent);

	TRACE("read data thread end\n");
	
	SetEvent(pSerial->m_hEvReadFileFinish);

	return 2;
}
/******************************************************************************
*功能：通知调用都，成功读取了一个数据包
*说明：
*参数：
*返回：
******************************************************************************/
void CSerial::NotifyReadData()
{
	::PostMessage(m_hParentWnd,m_uMessage,WM_COMM_READ,0);
}
/******************************************************************************
*功能：通知调用都，成功发送了一个数据包
*说明：
*参数：
*返回：
******************************************************************************/
void CSerial::NotifyWriteData(void*lpdata)
{
	::PostMessage(m_hParentWnd,m_uMessage,WM_COMM_WRITE,(LPARAM)lpdata);
}

void CSerial::ClearSysReadBufferContent()
{
	int nRet;
	DWORD dwErrors = 0;
	COMSTAT comstate;
	memset(&comstate,0,sizeof(COMSTAT));

	nRet = ::ClearCommError(m_hComm,&dwErrors,&comstate);
	if(!nRet)return;

	char buf[4096];
	DWORD dwBytesRead;
	::ReadFile(m_hComm,buf,comstate.cbInQue,&dwBytesRead,NULL);

	TRACE("已经清空了系统接收缓冲区！len = [%ld]\n",comstate.cbInQue);

	//也清空一下保存数据的缓冲区
	gDataRecv.PutToBlank();
	gDataSend.PutToBlank();
}

UINT CSerial::SendDataProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	//一些返回值
	BOOL  bRet;
	DWORD dwError = 0;
	DWORD dwRet;

	//重叠结构
	OVERLAPPED ov;
	memset(&ov,0,sizeof(OVERLAPPED));
	ov.hEvent = CreateEvent(0, true, 0, _T(""));

	HANDLE event_handle[2],handle[2];
	event_handle[0] = pSerial->m_hThreadSendData;
	event_handle[1] = pSerial->m_hEvSendDataStart;
	
	handle[0] = pSerial->m_hThreadSendData;
	handle[1] = ov.hEvent;

	//错误处理用的
	COMSTAT comstate;
	memset(&comstate,0,sizeof(COMSTAT));

	//是否开始了重叠操作？
	bool bWantToSendSomething = false;
	bool bHasOverlapped = false;
	bool bHasFinishedOverlapped = false;

	//一共写了多少字节？
	DWORD dwBytesWrite = 0;

	bool bQuit = false;

	TRACE0("send data thread start!!!!!\n");

	while(!bQuit)
	{
		dwRet = ::WaitForMultipleObjects(2,event_handle,false,INFINITE);
		switch(dwRet-WAIT_OBJECT_0)
		{
		case 0:
			{
				bQuit = true;
			}
			continue;
		case 1:
			{
				bWantToSendSomething = true;
			}
			break;
		default:
			{
			}
			continue;
		}
		if(!bWantToSendSomething)continue;
		bWantToSendSomething = false;
		//得到要发送的数据结点
		LPDATA lpdata = gDataSend.GetNext();

		//一直把所有需要发送的数据全都发送完
		while(lpdata != NULL)
		{
			bRet = ::WriteFile(pSerial->m_hComm,lpdata->m_sz_buf,lpdata->m_n_len,&dwBytesWrite,&ov);

			if(!bRet)
			{
				if(GetLastError() == ERROR_IO_PENDING)
				{
					bHasOverlapped = true;
				}
				else//出错处理，必须，否则所有IO操作都会被锁死!
				{
					::ClearCommError(pSerial->m_hComm,&dwError,&comstate);
				}
			}
			if(bHasOverlapped)
			{
				bHasOverlapped = false;
				dwRet = ::WaitForMultipleObjects(2,handle,false,INFINITE);
				switch(dwRet - WAIT_OBJECT_0)
				{
				case 0://退出
					{
						bQuit = true;
					}
					continue;
				case 1://数据到来
					{
						TRACE0("发送数据重叠操作完成\n");
						bHasFinishedOverlapped = true;
					}
					break;
				default://出错了
					{
					}
					continue;
				}
			}
			if(bHasFinishedOverlapped)
			{
				bHasFinishedOverlapped = false;
				bRet = ::GetOverlappedResult(pSerial->m_hComm,&ov,&dwBytesWrite,false);
				if(bRet)
				{
					TRACE("重叠操作完成！发送了[%d]字节数据\n",dwBytesWrite);
					pSerial->NotifyWriteData(lpdata->m_sz_buf);
				}
				else
				{
					TRACE("没有得到重叠操作结果！\n");
				}
			}
			//再取没有发送的数据
			lpdata = gDataSend.GetNext();
		}
		//恢复发送数据的信号，以便再接收发送数据的消息
		ResetEvent(event_handle[1]);
	}

	pSerial->CloseAndCleanHandle(ov.hEvent);
	TRACE0("send data thread end!\n");
	SetEvent(pSerial->m_hEvSendDataFinish);

	return 3;
}

bool CSerial::ReadData(char szData[], int &len)
{
	LPDATA lpdata = gDataRecv.GetNext();

	if(NULL == lpdata)
	{
		return false;
	}

	len = len>lpdata->m_n_len+1 ? lpdata->m_n_len +1 : len;

	memcpy(szData,lpdata->m_sz_buf,len);

	szData[len] = 0;

	return true;
}

bool CSerial::SendData(char szData[], int len)
{
	LPDATA lpdata = gDataSend.GetBlank();

	lpdata->m_n_len = len;

	memcpy(lpdata->m_sz_buf,szData,len);

	lpdata->m_sz_buf[len] = 0;

	SetEvent(m_hEvSendDataStart);

	return true;
}
