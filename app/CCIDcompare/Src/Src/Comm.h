#if !defined(__COMM_H__)
#define __COMM_H__

BOOL OpenComm(char pPort = 1,         //端口号
			  int nspeed=115200,       //波特率
			  int nParity=NOPARITY,      //校验位
			  int nByteSize=8,           //数据位
			  int nStopBits=ONESTOPBIT); //停止位
BOOL CloseComm(void);
int ReadComm(void* pData, int nLength);
int WriteComm(void* pData, int nLength,int delay);
int SendAT_GetModenReply(void *SendAT, void *Reply);
BOOL gsminit(void);
int GetRfIMEI(char *imei);
int GetRfCCID(char *ccid);
#endif // !defined(COMM_H__)