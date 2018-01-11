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

//ȫ�ֱ�������ʱ�洢�����õ�
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
*���ܣ���ʼ��
*˵����
*������
*���أ�
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
*���ܣ��ͷ���Դ
*˵����
*������
*���أ�
******************************************************************************/
void CSerial::UnInit()
{
	Close();

	//�ر�һֱ�����¼�����
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
*���ܣ��رմ���
*˵����
*������
*���أ�
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

	TRACE("�Ѿ��ر����ĸ��̣߳�\n");

	Sleep(100);

	CloseAndCleanHandle(m_hComm);

	//�رմ���ʱ��Ҫ����һ������ź�
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
	*���ܣ��رղ��Ƿ������
	*˵����
	*������
	*���أ�
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
	*���ܣ��򿪴���
	*˵����
	*������	nCom		������һ������--------1,2
		dwBaudRate	��������----------------9600?
		byByteSize	������λ----------------8
		byParity	����ż����λ------------0
		byStopBits	��ֹͣλ----------------1
	*���أ� ���֮ǰ�Ѿ��򿪣�TRUE�������������������õģ������ϴδ򿪴���ʱ����ֵ��
	���֮ǰû�д򿪴��ڣ�
	�ɹ���TRUE��ʧ�ܣ�FALSE
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
		//�򿪴���
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

		//���ù����¼�
		if(!::SetCommMask(m_hComm,EV_RXCHAR | EV_ERR))//| EV_TXEMPTY 
		{
			TRACE("setcommmask fail!\n");
			CloseAndCleanHandle(m_hComm);
			return FALSE;
		}
		TRACE("SetCommMask success!\n");

		//����DCB�ṹ
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

		//���ó�ʱ,��׼�����¼��жϽ��յ������ַ������Գ�ʱ��������
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

		//����������ȡ�����̣߳������̻߳����α�������
		//�˳�ʱ�����˳������¼��̣߳�Ȼ���¼������̣߳���������ݽ����߳�
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
	*���ܣ���ȡ�����߳�
	*˵����	ÿ���յ�һ���ַ����������¼�����������̰߳ѽ��յ��ĵڸ��ַ���Ϣ����
			���͸���Ϣ�����̡߳�
	*������lpParam������CSerial�������
	*���أ�0
******************************************************************************/
UINT CSerial::ReadEventProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	bool bQuit = false;

	DWORD dwEventMask = 0;

	//���յ���Ϣ�ص�
	OVERLAPPED ov = {0};	

	//��ȡ�����¼����˳��߳��ź�
	HANDLE mask_handle[2] = {0};

	//�����û����Ϣ�ı�־��
	DWORD dwRet;
	BOOL  bRet;
	bool  bHasMessage   = false;
	bool  bMaskOverlapping  =  false;

	//������
	DWORD dwErrors = 0;
	COMSTAT comstate;
	
	memset(&ov,0,sizeof(OVERLAPPED));
	ov.hEvent	 =  CreateEvent(0, true, 0, _T("ReadEventProc"));
	
	mask_handle[0]  =  pSerial->m_hEvStopRead;
	mask_handle[1]  =  ov.hEvent;
	
	memset(&comstate,0,sizeof(COMSTAT));

	//�ȵȴ������ź�,���ݴ����߳������󣬾ͻ�֪ͨ�����¼���Ϣ�߳�����
	::WaitForSingleObject(pSerial->m_hEvStartRead,INFINITE);
	TRACE("get event thread start!\n");

	//���ϵͳ������
	pSerial->ClearSysReadBufferContent();

	while(!bQuit)
	{
		dwEventMask = 0;
		bRet = ::WaitCommEvent(pSerial->m_hComm,&dwEventMask,&ov);
		if(bRet)	//���õõ�һ��������Ϣ
		{
			bHasMessage = true;
		}
		else//û�еõ�������Ϣ,�����Ƿ��ڽ����ص�������
		{
			if(GetLastError() == ERROR_IO_PENDING)//���ڽ����ص�����...
			{
				bMaskOverlapping = true;//���ڽ����ص�����,ֱ�����������±ߵĴ������
				TRACE("�Ѿ����뵽���ص�������errorcode = [%d][%d]\n",GetLastError(),dwEventMask);
			}
			else
			{
				TRACE("WaitCommEvent��������errorcode = [%d][%d]\n",GetLastError(),dwEventMask);
				continue;
			}
		}
		if(bMaskOverlapping)	//������ڽ��е�����,�͵ȴ�...
		{
			bMaskOverlapping = false;
			dwRet = ::WaitForMultipleObjects(2, mask_handle, false,	INFINITE);
			switch(dwRet-WAIT_OBJECT_0)
			{
			case 0://�˳��߳�
				{
					bQuit = true;
				}
				break;
			case 1://���յ���Ϣ
				{
					ResetEvent(ov.hEvent);//�ָ������¼��ź�
					bRet = ::GetOverlappedResult(pSerial->m_hComm,&ov,&dwErrors,false);
					if(bRet)
					{
						bHasMessage = true;
					}
				}
				break;
			default://δ֪�ź�
				{
				}
				break;
			}
		}
		if(bHasMessage)//���գ��������Ϣ����
		{
			bHasMessage = false;
			
			//����Ϣ���������ҳɹ��õ�����Ϣ
			if(EV_ERR  ==  (dwEventMask & EV_ERR) )	//��������˴��󣬾�ֻ�������
			{
				//���������ˣ�Ҫ�ܹ��ָ�����
				::ClearCommError(pSerial->m_hComm,&dwErrors,&comstate);
				TRACE("�����˴���\n");
			}
			
			else if(EV_RXCHAR  ==  (dwEventMask & EV_RXCHAR) )	//��ȡ����
			{
				SetEvent(pSerial->m_hEvEventComing);
			}
		}
	}

	pSerial->CloseAndCleanHandle(ov.hEvent);

	TRACE("get event thread end!\n");
	
	//֪ͨ���������̹߳ر�
	SetEvent(pSerial->m_hEvStopAnalysis);
	SetEvent(pSerial->m_hEvRecvFinish);
	
	return 0;
}
/******************************************************************************
*���ܣ������Ƿ�ɹ����յ�һ�����ݰ�
*˵����ÿ���յ�һ���ַ���������һ����Ϣ���͹��������������ַ����Ƿ�ʱ���ж��Ƿ�ɹ����յ�һ�����ݰ�
*������
*���أ�
******************************************************************************/
UINT CSerial::AnalysisEventProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	bool bQuit = false;

	//����WaitForMultipleObjects����ֵ
	DWORD dwRet;

	//�����ж�֮ǰ�Ƿ��й��ַ���Ϣ�������ַ���Ϣ֮���Ƿ�ʱ
	bool bHasMessageBefore		  =  false;
	
	//��ȡ�����¼����˳��߳��ź�
	HANDLE handle[2] = {0};

	
	handle[0]  =  pSerial->m_hEvStopAnalysis;
	handle[1]  =  pSerial->m_hEvEventComing;

	::WaitForSingleObject(pSerial->m_hThreadAnalysis,INFINITE);

	TRACE("analysis event thread start!\n");

	SetEvent(pSerial->m_hEvStartRead);//���������¼��߳�

	while(!bQuit)
	{
		dwRet = ::WaitForMultipleObjects(2,	handle,	false,	INFINITE);

		switch(dwRet-WAIT_OBJECT_0)
		{
		case 0://�˳�
			{
				bQuit = true;
			}
			continue;
		case 1://���ݵ���,������һ��ѭ��
			{
				TRACE0("һ�����ݰ��ĵ�һ���ַ����\n");
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
			if(WAIT_TIMEOUT == dwRet)//�ɹ����յ�һ�����ݰ�
			{
				if(bHasMessageBefore)
				{
					TRACE0("�����ؽ��յ���һ�����ݰ���֪ͨ���ݽ����߳̽��н��գ�\n");
					bHasMessageBefore = false;
					SetEvent(pSerial->m_hEvDataComing);//֪ͨ���ݽ����߳̽�������
				}
				continue;
			}
			switch(dwRet-WAIT_OBJECT_0)
			{
			case 0://�˳�
				{
					bQuit = true;
				}
				break;
			case 1://���ݵ���
				{
					//TRACE0("�¼������߳̽��յ���һ���ַ���\n");
					ResetEvent(handle[1]);
				}
				break;
			default://������
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
*���ܣ���ȡ�����߳�
*˵����	�ɹ��յ�һ�����ݰ�������̻߳�õ���Ϣ����ʼ��ȡ������ݰ�
�ɹ���ȡһ�����ݺ󣬻�����ݷŵ�g_read_data_group�У�ͬʱ������߷���һ����Ϣ
����Ϣ�ɵ����߳�ʼ��ʱ����.
*������lpParam������CSerial�������
*���أ�0
******************************************************************************/
UINT CSerial::ReadDataProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	bool bQuit = false;

	DWORD dwEventMask = 0;

	//���յ��ص���Ϣ
	OVERLAPPED ov;

	//��Ҫ��ȡ���ֽ���
	BOOL  bRet = FALSE;
	DWORD dwBytesWantRead = 0;
	DWORD dwBytesRead = 0;
	char  temp_buf[2*MAX_COMM_BUF_LEN] = {0};
	bool bReadOverlapping  =  false;
	bool bReadDataSuccess  =  false;

	//��֪��ϵͳ���������ﹲ�ж����ֽ�
	DWORD   dwErrors = 0;//��Ҫ������������Ǹ���û��ʹ�����ķ���ֵ�����ǲ��������ķ���ֵ.
	COMSTAT comstate;

	//����WaitForMultipleObjects����ֵ
	DWORD dwRet;

	//��ȡ�����¼����˳��߳��ź�
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

	SetEvent(pSerial->m_hThreadAnalysis);//���������߳�

	while(!bQuit)
	{
		dwRet = ::WaitForMultipleObjects(2,	event_handle,	false,	INFINITE);
		switch(dwRet-WAIT_OBJECT_0)
		{
		case 0://�˳�
			{
				bQuit = true;
			}
			break;
		case 1://���ݵ���
			{
				ResetEvent(event_handle[1]);//ÿ�ζ����Ȼָ��ź�
				//�õ��ܼ��ж����ֽ���ϵͳ���ջ�������
				bRet = ::ClearCommError(pSerial->m_hComm,&dwErrors,&comstate);
				if(!bRet)
				{
					continue;//ʧ�ܾͶ�����
				}
				dwBytesWantRead = comstate.cbInQue;//�õ���ϵͳ���������й��ж����ֽڡ�
				bRet = ::ReadFile(pSerial->m_hComm,temp_buf,dwBytesWantRead,&dwBytesRead,&ov);
				if(bRet)	//�ɹ���ȡ����
				{
					bReadDataSuccess = true;
				}
				else		//�����Ƿ�ʼ���ص�����?
				{
					if(GetLastError() == ERROR_IO_PENDING)
					{
						bReadOverlapping = true;
					}
					else//��������
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
						continue;//�����ȡ���ݳ�ʱ������Ϊ�Ǵ������ݣ�������
					}
					switch(dwRet-WAIT_OBJECT_0)
					{
					case 0://�˳�
						{
							bQuit = true;
						}
						break;
					case 1://�������ص��������
						{
							//							ResetEvent(read_handle[1]);//�ָ���ȡ�����ź�
							//��Ҫ�ж��Ƿ�ɹ���ȡ������
							bRet = ::GetOverlappedResult(pSerial->m_hComm,&ov,&dwBytesRead,false);
							if(!bRet)//���ص�����ʧ�ܣ�
							{
								TRACE0("���ص�����ʧ�ܣ�\n");
							}
							else
							{
								bReadDataSuccess = true;//�ɹ���ȡ������
							}
						}
						break;
					default:
						{
						}
						break;
					}
				}
				if(bReadDataSuccess)//����ɹ���ȡ������
				{
					bReadDataSuccess = false;//�ָ�״̬
					if(dwBytesWantRead == dwBytesRead)//���û���ٶ�ȡ�����ֽ�����������������ȷ��ȡ������
					{
						LPDATA lpdata = gDataRecv.GetBlank();

						//�����ȡ���ֽ���������MAX_COMM_BUF_LEN-1,��ֻ�ܽ��յ�MAX_COMM_BUF_LEN-1���ֽ���
						lpdata->m_n_len = (dwBytesRead > MAX_COMM_BUF_LEN-1) ? (MAX_COMM_BUF_LEN-1) : dwBytesRead;
						memcpy(lpdata->m_sz_buf, temp_buf, lpdata->m_n_len);
						lpdata->m_sz_buf[lpdata->m_n_len] = 0;
						pSerial->NotifyReadData();

						TRACE("���ݽ����߳̽��յ���һ�����ݰ���len = [%d]\n",lpdata->m_n_len);
					}
					else
					{
						TRACE0("�ٶ�ȡ�����ֽ���\n");
					}
				}
				continue;//�ɹ���ȡ������
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
*���ܣ�֪ͨ���ö����ɹ���ȡ��һ�����ݰ�
*˵����
*������
*���أ�
******************************************************************************/
void CSerial::NotifyReadData()
{
	::PostMessage(m_hParentWnd,m_uMessage,WM_COMM_READ,0);
}
/******************************************************************************
*���ܣ�֪ͨ���ö����ɹ�������һ�����ݰ�
*˵����
*������
*���أ�
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

	TRACE("�Ѿ������ϵͳ���ջ�������len = [%ld]\n",comstate.cbInQue);

	//Ҳ���һ�±������ݵĻ�����
	gDataRecv.PutToBlank();
	gDataSend.PutToBlank();
}

UINT CSerial::SendDataProc(LPVOID lpParam)
{
	CSerial * pSerial  =  (CSerial *)lpParam;

	//һЩ����ֵ
	BOOL  bRet;
	DWORD dwError = 0;
	DWORD dwRet;

	//�ص��ṹ
	OVERLAPPED ov;
	memset(&ov,0,sizeof(OVERLAPPED));
	ov.hEvent = CreateEvent(0, true, 0, _T(""));

	HANDLE event_handle[2],handle[2];
	event_handle[0] = pSerial->m_hThreadSendData;
	event_handle[1] = pSerial->m_hEvSendDataStart;
	
	handle[0] = pSerial->m_hThreadSendData;
	handle[1] = ov.hEvent;

	//�������õ�
	COMSTAT comstate;
	memset(&comstate,0,sizeof(COMSTAT));

	//�Ƿ�ʼ���ص�������
	bool bWantToSendSomething = false;
	bool bHasOverlapped = false;
	bool bHasFinishedOverlapped = false;

	//һ��д�˶����ֽڣ�
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
		//�õ�Ҫ���͵����ݽ��
		LPDATA lpdata = gDataSend.GetNext();

		//һֱ��������Ҫ���͵�����ȫ��������
		while(lpdata != NULL)
		{
			bRet = ::WriteFile(pSerial->m_hComm,lpdata->m_sz_buf,lpdata->m_n_len,&dwBytesWrite,&ov);

			if(!bRet)
			{
				if(GetLastError() == ERROR_IO_PENDING)
				{
					bHasOverlapped = true;
				}
				else//���������룬��������IO�������ᱻ����!
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
				case 0://�˳�
					{
						bQuit = true;
					}
					continue;
				case 1://���ݵ���
					{
						TRACE0("���������ص��������\n");
						bHasFinishedOverlapped = true;
					}
					break;
				default://������
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
					TRACE("�ص�������ɣ�������[%d]�ֽ�����\n",dwBytesWrite);
					pSerial->NotifyWriteData(lpdata->m_sz_buf);
				}
				else
				{
					TRACE("û�еõ��ص����������\n");
				}
			}
			//��ȡû�з��͵�����
			lpdata = gDataSend.GetNext();
		}
		//�ָ��������ݵ��źţ��Ա��ٽ��շ������ݵ���Ϣ
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
