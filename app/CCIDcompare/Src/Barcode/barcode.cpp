#include "stdafx.h"
#include "barcode.h"
#include <string.h>
#include <WinSpool.h>

//���ö�̬���ӿ�Fnthex32.dll�к���"GETFONTHEX"��Զָ��
typedef int (FAR __stdcall*MYWENT)(std::string, std::string, int, int, int, int, int, char *);


//��ӡ�˿ھ��
HANDLE hLpt;
//�����ǰ׺
char Prefixion[] = "^XA~TA000~JSN^LT0^MMT^MTT^PON^PMN^JMA^MD26^JUS^LRN^XZ";

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
BOOL  OpenLpt(char *lptport)
{//Ĭ��Ϊ"LPT1"
	hLpt = CreateFile(lptport,                           // ��ӡ�豸
		              GENERIC_WRITE,	                 // д��ʽ
					  NULL,				                 // ����ʽ����ռ
					  NULL,			                     // Ĭ�ϵİ�ȫ������
					  OPEN_EXISTING,	                 // ������ʽ
					  NULL,				                 // ���������ļ�����
					  NULL);			                 // �������ģ���ļ�
	if(hLpt == INVALID_HANDLE_VALUE)
		return FALSE;
	return TRUE;
}

//�رմ�ӡ�˿�
BOOL  CloseLpt(void)
{
	return CloseHandle(hLpt);
}

//��buf�г���ΪnLength���ַ����͵���ӡ�˿�
int   WriteLpt(char *buf, int nLength)
{
	DWORD dwNumWrite;	// ���ڷ��������ݳ���
    //д�˿�
	WriteFile(hLpt, buf, (DWORD)nLength, &dwNumWrite, NULL);
	//����ʵ��д�����ݳ���
	return (int)dwNumWrite;
}

SHORT ExecutePrintCmdInLpt(char *PrintCmd, char *lptport)
{
	BOOL result;
	char *cmd = (char *)malloc(sizeof(char)*HALF_KILO*10);
	if (NULL == cmd){
		MessageBox(NULL, "Can't get buf from stack��", "ERROR", MB_OK|MB_ICONERROR);
		return  ERR_CMD_EMPTY;
	}
	strcpy(cmd,  Prefixion);
	strcat(cmd,  PrintCmd);
	result = OpenLpt(lptport);
	if (!result){
		free(cmd);
		return ERR_OPEN_LPT;
	}
	if ((int)strlen(PrintCmd) > WriteLpt(PrintCmd, (int)strlen(PrintCmd))){
		free(cmd);
		return ERR_WRITE_LOSE_DATA;
	}
	result = CloseLpt();
	if (!result){
		free(cmd);
		return ERR_CLOSE_LPT;
	}
	free(cmd);
	return SUCCEED_LPT;

}

int GetChineseZplCode(LPCHINESEZPL lpchinesezpl,  char *outcode)
{
	HINSTANCE  hFnthex32;
	MYWENT     myproc;
	char       *format = (char *)malloc(sizeof(char) * 50);
	hFnthex32 = ::LoadLibrary("Fnthex32.dll");
	if(hFnthex32 == NULL){
		MessageBox(NULL, "Can't open Fnthex32.dll file!", "ERROR", MB_OK|MB_ICONHAND);
		return 0;
	}

	myproc = (MYWENT)GetProcAddress(hFnthex32,"GETFONTHEX");
	if (myproc == NULL){
		MessageBox(NULL, "Can't GetProcAddress of go!", "ERROR", MB_OK|MB_ICONHAND);
		return -1;
	}

	myproc(lpchinesezpl->strChinese, 
		   lpchinesezpl->strFontName, 
		   lpchinesezpl->intOrient,
		   lpchinesezpl->intHeight,  
		   lpchinesezpl->intWidth,     
		   lpchinesezpl->boolBold,		
		   lpchinesezpl->boolItalic, 
		   outcode);
	::FreeLibrary(hFnthex32);

	sprintf(format, "^FO%d,%d^XGOUTSTR01,%d,%d^FS", \
		lpchinesezpl->xyStart.x_axis,
		lpchinesezpl->xyStart.y_axis,
		lpchinesezpl->shortMx,
		lpchinesezpl->shortMy);

	strcat(outcode, format);
	free(format);

	return (int)strlen(outcode);
}


int CreateGraphicBoxCmd(char  *cmd,                   // OUT �������ɵĴ�ӡ����
			  		    int   FO_X,                   // IN  ��ʼ�����꣨In dots��
					    int   FO_Y,                   // IN  ��ʼ�����꣨In dots��
					    int   Box_Width,              // IN  ���εĿ�ȣ�In dots��[1, 32000]
					    int   Box_Height,             // IN  ���εĸ߶ȣ�In dots��[1, 32000]
					    int   Line_thickness,         // IN  ���ε������ֶȣ�In dots��[1, 32000]
					    BOOL  LineColor,              // IN  ��������ɫTRUE(black)/FALSE(white)
					    short rounding)               // IN  ���α߽ǵ�ƽ����[0, 8]
{
	char cmdformat[100] = "";

#if LineColor
	char color = 'B';
#else
	char color = 'W';
#endif

	sprintf(cmdformat, "^XA^IA6^XZ^XA^LH10,10^FO%d,%d^GB%d,%d,%d,%c,%d^FS^XZ",
		               FO_X,FO_Y,Box_Width,Box_Height,Line_thickness,color,rounding%9);

	strcpy(cmd, cmdformat);

	return (int)strlen(cmd);
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
	fp = fopen("Code933.nfo", "wb");
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


//����Code 93 �����ӡ�������������룬˵������������ͬһ�У���������һ��
int CreateBACodeItemALine(char *cmd,                       // OUT �������ɵĴ�ӡ����
						  char *Codestr1,                  // IN  �ڶ������봮           �磺012345678901234
						  char *note1,                     // IN  �ڶ������봮��˵����   �磺IMEI:
						  char *Codestr2,                  // IN  ���������봮           �磺01234567890123456789
						  char *note2,                     // IN  ���������봮��˵����   �磺CCID:
						  ZPLFONT font,                    // IN  ��������ַ���������
						  BARBY   BY,                      // IN  ����ģ������ò���
						  int  FO_X,                       // IN  ��ʼ�����꣨In dots��
						  int  FO_Y,                       // IN  ��ʼ�����꣨In dots��
						  BOOL OverBarcode,                // IN  �����ַ������������(TRUE)/��(FALSE)
						  int nShare)                      // IN  ��ӡ����	
{
	FILE *fp;
	fp = fopen("Code932.nfo", "wb");
	if (NULL == fp){
		AfxMessageBox("���ܹ������ļ�BAPrintCmd.inf");
		return 0;
	}
	char cmdformat[128] = "";
	int Y = FO_Y;//���������(����)
	strcpy(cmd,  "");
	Y = Y+font.height+3;
	Y = Y + BY.heighet + 3;

	if (OverBarcode)
	{
		//��ӡ���� note1:Codestr1 ���ַ���
		sprintf(cmdformat, "^XA^LH10,10^FO%d,%d^A%c,%c,%d,%d^FDTELNUM:^FS",
			FO_X,Y-BY.heighet-36,font.name,font.Orientation,font.height,font.width);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		sprintf(cmdformat, "^LH10,10^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡCodestr1����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//��ӡ���� note2:Codestr2 ���ַ���
		Y = Y + BY.heighet + 24;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡCodestr2����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}
	else
	{
		//��ӡCodestr1����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		sprintf(cmdformat,"^XA^LH10,10^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note1:Codestr1 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		
		//��ӡCodestr2����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+24;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note2:Codestr2 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcat(cmd, cmdformat);
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
	fp = fopen("Code933.nfo", "wb");
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
		Y = Y + BY.heighet + 24;
		// ����HLP200���ӡ��������
		//Y = Y + BY.heighet + 18;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡCodestr2����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height + 3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//��ӡ���� note3:Codestr3 ���ַ���
		Y = Y + BY.heighet + 24;
		// ����HLP200���ӡ��������
		//Y = Y + BY.heighet + 18;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note3,Codestr3);
		strcat(cmd, cmdformat);
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
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note1:Codestr1 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		
		//��ӡCodestr2����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+24;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note2:Codestr2 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//��ӡCodestr3����������^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+24;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr3);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//��ӡ���� note3:Codestr3 ���ַ���
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note3,Codestr3);
		strcat(cmd, cmdformat);
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
					  short     nShare)                    // IN  ��ӡ����
{
	int   Xaxis         = xy.x_axis;
	char  cmdformat[128] = "";
	FILE *fp;
	fp = fopen("Packingitem.inf", "wb");
	if (NULL == fp){
		MessageBox(NULL, "���ܹ������ļ�Packingitem.inf", "��ʾ", MB_OK|MB_ICONERROR);
		return 0;
	}
	strcpy(cmd,   "");

	//��ӡ�绰����
	sprintf(cmdformat,"^XA^FO%d,%d^BY%d,%3.2f,%d^BAN,,Y,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.heighet, barby.width_ratio, barby.heighet, pTelnum);
	strcat(cmd,  cmdformat);
	sprintf(cmdformat, "%s\r", cmdformat);

	//��ӡIMEI��
	Xaxis = Xaxis + 626;
	sprintf(cmdformat,"^XA^FO%d,%d^BY%d,%3.2f,%d^BAN,,Y,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.heighet, barby.width_ratio, barby.heighet, pImei);
	strcat(cmd,  cmdformat);
	sprintf(cmdformat, "%s\r", cmdformat);

	//��ӡCCID��
	Xaxis = Xaxis + 844;
	sprintf(cmdformat,"^XA^FO%d,%d^BY%d,%3.2f,%d^BAN,,Y,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.heighet, barby.width_ratio, barby.heighet, pImei);
	strcat(cmd,  cmdformat);
	sprintf(cmdformat, "%s\r", cmdformat);

	sprintf(cmdformat, "^PQ%d^XZ\n", nShare);
	strcat(cmd, cmdformat);
	fprintf(fp, "%s", cmdformat);

	free(fp);
	return (int)strlen(cmd);
}






