#include "stdafx.h"
#include "barcode.h"
#include <string.h>
#include <WinSpool.h>

//���ö�̬���ӿ�Fnthex32.dll�к���"GETFONTHEX"��Զָ��
typedef int (PASCAL *MYPROC)(LPCSTR, LPCSTR, int, int, int, int, int, LPSTR);


//��ӡ�˿ھ��
HANDLE hLpt;

//��ӡ�豸���ƾ��
HANDLE hPrint;

//���뷽ʽ
const char *Coder[] = {
	"^B1",//Code 11
	"^B2",//Interleaved 2 of 5
	"^B3",//Code 39
	"^B4",//Code 49
	"^B5",//Planet Code
	"^B7",//PDF417 
	"^B8",//EAN-8
	"^B9",//UPC-E
	"^BA",//Code 93
	"^BB",//CODABLOCK
	"^BC",//Code 128
	"^BD",//UPS MaxiCode
	"^BE",//EAN-13
	"^BF",//Micro-PDF417
	"^BI",//Industrial 2 of 5
	"^BJ",//Standard 2 of 5
	"^BK",//ANSI Codabar
	"^BL",//LOGMARS
	"^BM",//MSI
	"^BO",//Aztec
	"^BP",//Plessey
	"^BQ",//QR
	"^BR",//RSS
	"^BS",//UPC/EAN
	"^BT",//TLC39
	"^BU",//UPC-A
	"^BX",//Data Matrix
	"^BZ" //POSTNET
	};


//�򿪴�ӡ�˿�
BOOL  OpenLpt(const char *lptport)
{//Ĭ��Ϊ"LPT1"
/*	COMMTIMEOUTS timeouts = {	// ���ڳ�ʱ���Ʋ���
		100,					// ���ַ������ʱʱ��: 100 ms
		1,						// ������ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		500,					// ������(�����)����ʱʱ��: 500 ms
		1,						// д����ʱÿ�ַ���ʱ��: 1 ms (n���ַ��ܹ�Ϊn ms)
		100};
*/
	hLpt = CreateFile(lptport,                           // ��ӡ�豸
		              GENERIC_WRITE,	                 // д��ʽ
					  NULL,				                 // ����ʽ����ռ
					  NULL,			                     // Ĭ�ϵİ�ȫ������
					  OPEN_EXISTING,	                 // ������ʽ
					  NULL,				                 // ���������ļ�����
					  NULL);			                 // �������ģ���ļ�
	if(hLpt == INVALID_HANDLE_VALUE)
		return FALSE;
//	SetCommTimeouts(hLpt, &timeouts);
	return TRUE;
}

//�رմ�ӡ�˿�
BOOL  CloseLpt(void)
{
	return CloseHandle(hLpt);
}

//��buf�г���ΪnLength���ַ����͵���ӡ�˿�
int   WriteLpt(const char *buf, int nLength)
{
	DWORD dwNumWrite;	// ���ڷ��������ݳ���
    //д�˿�
	WriteFile(hLpt, buf, (DWORD)nLength, &dwNumWrite, NULL);
	//����ʵ��д�����ݳ���
	return (int)dwNumWrite;
}

SHORT ExecutePrintCmdInLpt(const char *PrintCmd, const char *lptport)
{
	BOOL result;
	result = OpenLpt(lptport);
	if (!result){
		return ERR_OPEN_LPT;
	}
	if ((int)strlen(PrintCmd) > WriteLpt(PrintCmd, (int)strlen(PrintCmd))){
		return ERR_WRITE_LOSE_DATA;
	}
	result = CloseLpt();
	if (!result){
		return ERR_CLOSE_LPT;
	}
	return SUCCEED_LPT;

}

BOOL IsZplDirectValidString(const char *pSrc)
{
	while ((*pSrc++ < 0x20) && (*pSrc > 0x8f)){
		return FALSE;
	}
	return TRUE;
}


int GetChineseZplCode(LPCHINESEZPL lpchinesezpl,  LPSTR lpoutcode)
{
	HINSTANCE  hFnthex32;
	MYPROC     myproc;
	int        outlen = 0;
	char       *mat   = (char *) malloc(sizeof(char)*100); 
	hFnthex32 = ::LoadLibrary("Fnthex32.dll");
	if(hFnthex32 == NULL){
		MessageBox(NULL, "Can't open Fnthex32.dll file!", "ERROR", MB_OK|MB_ICONHAND);
		return -1;
	}	

	myproc = (MYPROC)GetProcAddress(hFnthex32, "GETFONTHEX");
	if (myproc == NULL){
		MessageBox(NULL, "Can't GetProcAddress of GETFONTHEX!", "ERROR", MB_OK|MB_ICONHAND);
		return -2;
	}

	memset(lpoutcode,  0x00, sizeof(lpoutcode));

	outlen = myproc(lpchinesezpl->lpChinese, lpchinesezpl->lpFontName,
		            lpchinesezpl->intOrient, lpchinesezpl->intHeight, 
					lpchinesezpl->intWidth,  lpchinesezpl->boolBold,
					lpchinesezpl->boolItalic,lpoutcode);
	sprintf(mat, "^FO%d,%d^XGOUTSTR01,%d,%d^FS", lpchinesezpl->xy.x_axis, lpchinesezpl->xy.y_axis,
		lpchinesezpl->xmf, lpchinesezpl->ymf);
	strcat(lpoutcode, mat);
	outlen = outlen + (int)strlen(mat);
    ::FreeLibrary(hFnthex32);
	free(mat);
	return  outlen;
}

int CreateBACodeItemALine(char *cmd,                       // OUT �������ɵĴ�ӡ����
						  char *Codestr,                   // IN  ��һ�����봮           �磺13800138000
						  char *note,                      // IN  ��һ�����봮��˵����   �磺TELNUM:
						  ZPLFONT font,                    // IN  ��������ַ���������
						  BARBY   BY,                      // IN  ����ģ������ò���
						  int  FO_X,                       // IN  ��ʼ�����꣨In dots��
						  int  FO_Y,                       // IN  ��ʼ�����꣨In dots��
						  BOOL OverBarcode,                // IN  �����ַ������������(TRUE)/��(FALSE)
						  int nShare)                      // IN  ��ӡ����	
{
	FILE *fp;
	fp = fopen("Code93ALine_3.inf", "wb");
	if (NULL == fp){
		AfxMessageBox("���ܹ������ļ�B3PrintCmd.inf");
		return 0;
	}
	char cmdformat[128] = "";
	int Y = FO_Y;//���������(����)
	
	if (OverBarcode)
	{
		//��ӡ���� note1:Codestr1 ���ַ���
		sprintf(cmdformat, "^XA^LH10,10^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note,Codestr);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡCodestr1����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}
	else
	{
		//��ӡCodestr1����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		sprintf(cmdformat,"^XA^LH10,10^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note1:Codestr1 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note,Codestr);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}

	sprintf(cmdformat, "^PQ%d^XZ\n", nShare);
	strcat(cmd, cmdformat);
	fprintf(fp, "%s", cmdformat);

	fclose(fp);
	return (int)strlen(cmd);
}
//����Code 93 �����ӡ�������������룬˵������������ͬһ��
int CreateBACodeItemALine(char *cmd,                       // OUT �������ɵĴ�ӡ����
						  char *Codestr1,                  // IN  ��һ�����봮           �磺13800138000
						  char *note1,                     // IN  ��һ�����봮��˵����   �磺TELNUM:
						  char *Codestr2,                  // IN  �ڶ������봮           �磺012345678901234
						  char *note2,                     // IN  �ڶ������봮��˵����   �磺IMEI:
						  char *Codestr3,                  // IN  ���������봮           �磺01234567890123456789
						  char *note3,                     // IN  ���������봮��˵����   �磺CCID:
						  ZPLFONT font,                    // IN  ��������ַ���������
						  BARBY   BY,                      // IN  ����ģ������ò���
						  int  FO_X,                       // IN  ��ʼ�����꣨In dots��
						  int  FO_Y,                       // IN  ��ʼ�����꣨In dots��
						  BOOL OverBarcode,                // IN  �����ַ������������(TRUE)/��(FALSE)
						  int nShare)                      // IN  ��ӡ����
{
	FILE *fp;
	fp = fopen("Code93ALine_3.inf", "wb");
	if (NULL == fp){
		AfxMessageBox("���ܹ������ļ�B3PrintCmd.inf");
		return 0;
	}
	char cmdformat[128] = "";
	int Y = FO_Y;//���������(����)


	if (OverBarcode)
	{
		//��ӡ���� note1:Codestr1 ���ַ���
		sprintf(cmdformat, "^XA^LH10,10^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡCodestr1����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//��ӡ���� note2:Codestr2 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡCodestr2����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//��ӡ���� note3:Codestr3 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note3,Codestr3);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡCodestr3����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr3);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}
	else
	{
		//��ӡCodestr1����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		sprintf(cmdformat,"^XA^LH10,10^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note1:Codestr1 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		
		//��ӡCodestr2����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note2:Codestr2 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//��ӡCodestr3����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr3);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note3:Codestr3 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note3,Codestr3);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}

	sprintf(cmdformat, "^PQ%d^XZ\n", nShare);
	strcat(cmd, cmdformat);
	fprintf(fp, "%s", cmdformat);

	fclose(fp);
	return (int)strlen(cmd);
}



//***********************************************************************************
//����Code 93 �����ӡ������һ�е����루����װ��ı�ǩ��
int CreatePackingItem(char      *cmd,                      // OUT �������ɵĴ�ӡ����
					  char      *pTelnum,                  // IN  �绰�����ַ���
					  char      *pImei,                    // IN  IMEI�ַ���
					  char      *pCcid,                    // IN  CCID�ַ���
					  BARBY     barby,                     // IN  ����ģ�����ýṹ
					  XY        xy,                        // IN  ��ʼ�����
					  short     nMMspace,                  // IN  ������
					  short     nShare)                    // IN  ��ӡ����
{
	int   Xaxis         = xy.x_axis;
	int   lendots1      = (int)strlen(pTelnum)*ACHDOTSA,
		  lendots2      = (int)strlen(pImei)*ACHDOTSA,
		  lendots3      = (int)strlen(pCcid)*ACHDOTSA;
	int   SpaceDots     = nMMspace*MMDOTSA;
	char  cmdformat[256] = "";
	XY    xystr;
//	FILE *fp;
//	fp = fopen("Packingitem.inf", "wb");
//	if (NULL == fp){
//		MessageBox(NULL, "���ܹ������ļ�Packingitem.inf", "��ʾ", MB_OK|MB_ICONERROR);
//		return 0;
//	}
	strcpy(cmd,   "");

	//��ӡ�绰����
//	sprintf(cmdformat,"^XA^FO%d,%d^BY%d,%3.2f,%d^BAN,,Y,N,N^FD%s^FS",
	sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.width, barby.width_ratio, barby.heighet, pTelnum);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);
	xystr.x_axis = Xaxis + 24 + (lendots1 - ((int)strlen(pTelnum)*MMDOTSA))/2;
	xystr.y_axis = xy.y_axis + barby.heighet + 4;
	sprintf(cmdformat, "^FO%d,%d^AAN,15^FD%s^FS",xystr.x_axis, xystr.y_axis, pTelnum);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);

	//��ӡIMEI��
	Xaxis = Xaxis + lendots1 + SpaceDots;
	sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.width, barby.width_ratio, barby.heighet, pImei);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);
	xystr.x_axis = Xaxis + 24 + (lendots2 - ((int)strlen(pImei)*MMDOTSA))/2;
	xystr.y_axis = xy.y_axis + barby.heighet + 4;
	sprintf(cmdformat, "^FO%d,%d^AAN,15^FD%s^FS",xystr.x_axis, xystr.y_axis, pImei);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);

	//��ӡCCID��
	Xaxis = Xaxis + lendots2 + SpaceDots;
	sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.width, barby.width_ratio, barby.heighet, pCcid);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);
	xystr.x_axis = Xaxis + 24 + (lendots3 - ((int)strlen(pCcid)*MMDOTSA))/2;
	xystr.y_axis = xy.y_axis + barby.heighet + 4;
	sprintf(cmdformat, "^FO%d,%d^AAN,15^FD%s^FS",xystr.x_axis, xystr.y_axis, pCcid);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);

//	sprintf(cmdformat, "^PQ%d^XZ\n", nShare);
//	strcat(cmd, cmdformat);
//	fprintf(fp, "%s", cmdformat);

//	free(fp);
	return (int)strlen(cmd);
}

//����Code 93 �����ӡ������һ�е����루����װ��ı�ǩ��
int CreatePackingItem(char      *cmd,                      // OUT �������ɵĴ�ӡ����
					  char      *pImei,                    // IN  IMEI�ַ���
					  char      *pCcid,                    // IN  CCID�ַ���
					  BARBY     barby,                     // IN  ����ģ�����ýṹ
					  XY        xy,                        // IN  ��ʼ�����
					  short     nMMspace,                  // IN  ������
					  short     nShare)                    // IN  ��ӡ����
{
	int   Xaxis         = xy.x_axis;
	int   lendots1      = (int)strlen(pImei)*ACHDOTSA,
		  lendots2      = (int)strlen(pCcid)*ACHDOTSA;
	int   SpaceDots     = nMMspace*MMDOTSA;
	char  cmdformat[256] = "";
	XY    xystr;
//	FILE *fp;
//	fp = fopen("Packingitem.inf", "wb");
//	if (NULL == fp){
//		MessageBox(NULL, "���ܹ������ļ�Packingitem.inf", "��ʾ", MB_OK|MB_ICONERROR);
//		return 0;
//	}
	strcpy(cmd,   "");
	//��ӡIMEI��
//	sprintf(cmdformat,"^XA^FO%d,%d^BY%d,%3.2f,%d^BAN,,Y,N,N^FD%s^FS",
	sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.width, barby.width_ratio, barby.heighet, pImei);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);
	xystr.x_axis = Xaxis + 24 + (lendots1 - ((int)strlen(pImei)*MMDOTSA))/2;
	xystr.y_axis = xy.y_axis + barby.heighet + 4;
	sprintf(cmdformat, "^FO%d,%d^AAN,15^FD%s^FS",xystr.x_axis, xystr.y_axis, pImei);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);

	//��ӡCCID��
	Xaxis = Xaxis + lendots1 + SpaceDots;
	sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.width, barby.width_ratio, barby.heighet, pCcid);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);
	xystr.x_axis = Xaxis + 24 + (lendots2 - ((int)strlen(pCcid)*MMDOTSA))/2;
	xystr.y_axis = xy.y_axis + barby.heighet + 4;
	sprintf(cmdformat, "^FO%d,%d^AAN,15^FD%s^FS",xystr.x_axis, xystr.y_axis, pCcid);
	strcat(cmd,  cmdformat);
//	fprintf(fp, "%s", cmdformat);

//	sprintf(cmdformat, "^PQ%d^XZ\n", nShare);
//	strcat(cmd, cmdformat);
//	fprintf(fp, "%s", cmdformat);

//	free(fp);
	return (int)strlen(cmd);
}


unsigned long loadfile(const char *szFile, char *outbuf)
{
	unsigned long SIZE = 0;
	char row[1024*128] = "";

	FILE *fp = fopen(szFile,  "r");
	
	if (NULL == fp){
		return SIZE;
	}
	memset(outbuf, 0x00, sizeof(outbuf));

	while(!feof(fp)){
		memset(row, 0x00, 1024*128);
		fscanf(fp, "%s", row);
		strcat(outbuf, row);
		//*outbuf++ = (const char)getc(fp);
		SIZE++;
	}
	fclose(fp);
	return SIZE;
}
