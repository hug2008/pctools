#if !defined(__COMM_H__)
#define __COMM_H__

BOOL OpenComm(char pPort = 1,         //�˿ں�
			  int nspeed=115200,       //������
			  int nParity=NOPARITY,      //У��λ
			  int nByteSize=8,           //����λ
			  int nStopBits=ONESTOPBIT); //ֹͣλ
BOOL CloseComm(void);
int ReadComm(void* pData, int nLength);
int WriteComm(void* pData, int nLength,int delay);
int SendAT_GetModenReply(void *SendAT, void *Reply);
BOOL gsminit(void);
int GetRfIMEI(char *imei);
int GetRfCCID(char *ccid);
#endif // !defined(COMM_H__)