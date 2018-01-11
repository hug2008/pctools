#include "stdafx.h"
#include "Comm.h"
#include "assert.h"
#include "myglobals.h"
#include <string>

// �����豸���
HANDLE hComm;

//����������
//CRITICAL_SECTION m_csLock;

//��������
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
//�߳̿��ƾ��
HANDLE hThread;



// �򿪴���
// ����: pPort - �������ƻ��豸·��������"COM1"��"\\.\COM1"���ַ�ʽ�������ú���
//       nBaudRate - ������
//       nParity - ��żУ��
//       nByteSize - �����ֽڿ��
//       nStopBits - ֹͣλ
BOOL OpenComm(char Port, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	DCB dcb;		// ���ڿ��ƿ�
	COMMTIMEOUTS timeouts = {	// ���ڳ�ʱ���Ʋ���
		5000,				// ���ַ������ʱʱ��: 5000 ms/5 s
		1,					// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		500,				// ������(�����)����ʱʱ��: 500 ms
		1,					// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		100};				// ������(�����)д��ʱʱ��: 100 ms
	if ((Port < 1) || (Port > 9))
	{
		return FALSE;
	}
	else
	{
		hComm = CreateFile(ComPortStr[(int) Port],	// �������ƻ��豸·��
			GENERIC_READ | GENERIC_WRITE,	// ��д��ʽ
			0,				// ����ʽ����ռ
			NULL,			// Ĭ�ϵİ�ȫ������
			OPEN_EXISTING,	// ������ʽ
			0,				// ���������ļ�����
			NULL);			// �������ģ���ļ�
		if(hComm == INVALID_HANDLE_VALUE)
		{
			return FALSE;// �򿪴���ʧ��
		}
		else
		{
			GetCommState(hComm, &dcb);		// ȡDCB 
			//********  ���������ɵ�½���ȡ *********
			dcb.BaudRate = nBaudRate;	
			dcb.Parity = nParity;
			dcb.StopBits = nStopBits;
			dcb.ByteSize = nByteSize;
			//*********  ���������������� *********
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			dcb.fDsrSensitivity = FALSE;
			dcb.fOutxCtsFlow = FALSE;
			dcb.fOutxDsrFlow = FALSE;
			dcb.fParity = FALSE;
			dcb.ByteSize = nByteSize;
			//**********************************
			
			SetCommState(hComm, &dcb);		// ����DCB
			
			SetupComm(hComm, 4096, 1024);	// �������������������С
			
			SetCommTimeouts(hComm, &timeouts);	// ���ó�ʱ
			
			return TRUE;// �򿪴��ڳɹ�
		}
	}	
}
// �رմ���
BOOL CloseComm()
{
	return CloseHandle(hComm);
}

// д����
// ����: pData - ��д�����ݻ�����ָ��
//       nLength - ��д�����ݳ���
// ����: ʵ��д������ݳ���
int WriteComm(void* pData, int nLength,int delay)
{
	DWORD dwNumWrite;	// ���ڷ��������ݳ���

	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);

	Sleep(delay);		//Ъ��

	return (int)dwNumWrite;
}
// ������
// ����: pData - ���������ݻ�����ָ��
//       nLength - ������������ݳ���
// ����: ʵ�ʶ��������ݳ���
int ReadComm(void* pData, int nLength)
{
	DWORD dwNumRead;	// �����յ������ݳ���

	ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);
	
	return (int)dwNumRead;
}

int SendAT_GetModenReply(void *SendAT, void *Reply)
{
	assert(NULL != SendAT);
	char cmd[200] = "";
	strcpy((char *)Reply, "");
	strcpy(cmd, (char *)SendAT);
	//���������û��"\r"
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