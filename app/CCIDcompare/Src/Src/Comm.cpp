#include "stdafx.h"
#include "Comm.h"
#include "assert.h"
#include "myglobals.h"
#include <string>

// 串口设备句柄
HANDLE hComm;

//缓冲区定义
//CRITICAL_SECTION m_csLock;

//串口名称
const char *ComPortStr[] = {
	"ERR",
	"COM1",
	"COM2",
	"COM3",
	"COM4",
	"COM5",
	"COM6",
	"COM7",
	"COM8",
	"COM9",
	"COM10",
	"COM11",
	"COM12",
	"COM13",
	"COM14",
	"COM15"
	};
//线程控制句柄
HANDLE hThread;



// 打开串口
// 输入: pPort - 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者
//       nBaudRate - 波特率
//       nParity - 奇偶校验
//       nByteSize - 数据字节宽度
//       nStopBits - 停止位
BOOL OpenComm(char Port, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	DCB dcb;		// 串口控制块
	COMMTIMEOUTS timeouts = {	// 串口超时控制参数
		5000,				// 读字符间隔超时时间: 5000 ms/5 s
		1,					// 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
		500,				// 基本的(额外的)读超时时间: 500 ms
		1,					// 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
		100};				// 基本的(额外的)写超时时间: 100 ms
	if ((Port < 1) || (Port > 9))
	{
		return FALSE;
	}
	else
	{
		hComm = CreateFile(ComPortStr[(int) Port],	// 串口名称或设备路径
			GENERIC_READ | GENERIC_WRITE,	// 读写方式
			0,				// 共享方式：独占
			NULL,			// 默认的安全描述符
			OPEN_EXISTING,	// 创建方式
			0,				// 不需设置文件属性
			NULL);			// 不需参照模板文件
		if(hComm == INVALID_HANDLE_VALUE)
		{
			return FALSE;// 打开串口失败
		}
		else
		{
			GetCommState(hComm, &dcb);		// 取DCB 
			//********  以下四项由登陆框获取 *********
			dcb.BaudRate = nBaudRate;	
			dcb.Parity = nParity;
			dcb.StopBits = nStopBits;
			dcb.ByteSize = nByteSize;
			//*********  以下五项自行设置 *********
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			dcb.fDsrSensitivity = FALSE;
			dcb.fOutxCtsFlow = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fParity = FALSE;
			dcb.ByteSize = nByteSize;
			//**********************************
			
			SetCommState(hComm, &dcb);		// 设置DCB
			
			SetupComm(hComm, 4096, 1024);	// 设置输入输出缓冲区大小
			
			SetCommTimeouts(hComm, &timeouts);	// 设置超时
			
			return TRUE;// 打开串口成功
		}
	}	
}
// 关闭串口
BOOL CloseComm()
{
	return CloseHandle(hComm);
}

// 写串口
// 输入: pData - 待写的数据缓冲区指针
//       nLength - 待写的数据长度
// 返回: 实际写入的数据长度
int WriteComm(void* pData, int nLength,int delay)
{
	DWORD dwNumWrite;	// 串口发出的数据长度

	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);

	Sleep(delay);		//歇会

	return (int)dwNumWrite;
}
// 读串口
// 输入: pData - 待读的数据缓冲区指针
//       nLength - 待读的最大数据长度
// 返回: 实际读出的数据长度
int ReadComm(void* pData, int nLength)
{
	DWORD dwNumRead;	// 串口收到的数据长度

	ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);
	
	return (int)dwNumRead;
}

int SendAT_GetModenReply(void *SendAT, void *Reply)
{
	assert(NULL != SendAT);
	char cmd[200] = "";
	strcpy((char *)Reply, "");
	strcpy(cmd, (char *)SendAT);
	//检查里面有没有"\r"
	if (NULL == strstr(cmd, "\r"))
		strcat(cmd, "\r\n\0");
	WriteComm(cmd, (int)strlen(cmd), 10);
	ReadComm(Reply, 200);
	return (int)strlen((char *)Reply);
}

BOOL gsminit(void)
{
	char ans[256] = "";
	int i = 0;
	while (i < 200){
		WriteComm("AT\r", 3, 1);
		ReadComm(ans, 100);
		if (NULL != strstr(ans, "OK")){
			break;
		}
		i++;
	}//while (i < 200){}
	if (i == 200){
		return FALSE;
	}
	SendAT_GetModenReply("ATE1\r", ans);
	return TRUE;
}

int GetRfIMEI(char *imei)
{
	char ans[256] = "";
	int i, j = 0;
	SendAT_GetModenReply("AT+CGSN\r", ans);
	if (NULL ==strstr(ans, "OK")){
		strcpy(imei, "ERROR");
		return 0;
	}
	else if (NULL != strstr(ans, "+SIND"))
	{
		strcpy(imei, "ERROR");
		return 0;
	}
	else if(NULL == strstr(ans, "+CGSN"))
	{
		strcpy(imei, "ERROR");
		return 0;
	}
	else
	{
		strcpy(imei, "");
		for (i=0; i<(int)strlen(ans)-15; i++){
			if (CharIsNum(ans[i]))
				break;
		}
		if (i < ((int)strlen(ans)-15))
		{
			for(j=0; j<15; j++)
			{
				imei[j] = ans[i++];
				if (!IsAlphaChar(imei[j])){
					imei[j] = '\0';
					return 0;
				}
			}				
			imei[j] = '\0';
		}
	}
	return j;
}

int GetRfCCID(char *ccid)
{
	char ans[256] = "";
	int i, j = 0;
	SendAT_GetModenReply("AT+CCID\r", ans);
	if (NULL == strstr(ans, "OK"))
	{
		strcpy(ccid, "ERROR");
		return 0;
	}
	else if (NULL != strstr(ans, "+SIND"))
	{
		strcpy(ccid, "ERROR");
		return 0;
	}
	else if (NULL == strstr(ans ,"+CCID"))
	{
		strcpy(ccid, "ERROR");
		return 0;
	}
	else
	{
		strcpy(ccid, "");
		for (i=0; i<(int)strlen(ans)-20; i++){
			if (CharIsNum(ans[i])){
				break;
			}
		}
		if (i < ((int)strlen(ans)-20))
		{
			for(j=0; j<20; j++)
			{
				ccid[j] = ans[i++];
				if (!IsAlphaChar(ccid[j]))
				{
					ccid[j] = '\0';
					return 0;
				}
			}
			ccid[j] = '\0';
		}		
	}
	return j;
}