#include "stdafx.h"
#include "barcode.h"
#include <string.h>
#include <WinSpool.h>

//调用动态连接库Fnthex32.dll中函数"GETFONTHEX"的远指针
typedef int (PASCAL *MYPROC)(LPCSTR, LPCSTR, int, int, int, int, int, LPSTR);


//打印端口句柄
HANDLE hLpt;

//打印设备控制句柄
HANDLE hPrint;

//编码方式
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


//打开打印端口
BOOL  OpenLpt(const char *lptport)
{//默认为"LPT1"
/*	COMMTIMEOUTS timeouts = {	// 串口超时控制参数
		100,					// 读字符间隔超时时间: 100 ms
		1,						// 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
		500,					// 基本的(额外的)读超时时间: 500 ms
		1,						// 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
		100};
*/
	hLpt = CreateFile(lptport,                           // 打印设备
		              GENERIC_WRITE,	                 // 写方式
					  NULL,				                 // 共享方式：独占
					  NULL,			                     // 默认的安全描述符
					  OPEN_EXISTING,	                 // 创建方式
					  NULL,				                 // 不需设置文件属性
					  NULL);			                 // 不需参照模板文件
	if(hLpt == INVALID_HANDLE_VALUE)
		return FALSE;
//	SetCommTimeouts(hLpt, &timeouts);
	return TRUE;
}

//关闭打印端口
BOOL  CloseLpt(void)
{
	return CloseHandle(hLpt);
}

//把buf中长度为nLength的字符串送到打印端口
int   WriteLpt(const char *buf, int nLength)
{
	DWORD dwNumWrite;	// 并口发出的数据长度
    //写端口
	WriteFile(hLpt, buf, (DWORD)nLength, &dwNumWrite, NULL);
	//返回实际写的数据长度
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

int CreateBACodeItemALine(char *cmd,                       // OUT 编译生成的打印命令
						  char *Codestr,                   // IN  第一个条码串           如：13800138000
						  char *note,                      // IN  第一个条码串的说明串   如：TELNUM:
						  ZPLFONT font,                    // IN  条码解释字符串的字体
						  BARBY   BY,                      // IN  条码模块的设置参数
						  int  FO_X,                       // IN  起始横坐标（In dots）
						  int  FO_Y,                       // IN  起始纵坐标（In dots）
						  BOOL OverBarcode,                // IN  解释字符串在条码的上(TRUE)/下(FALSE)
						  int nShare)                      // IN  打印份数	
{
	FILE *fp;
	fp = fopen("Code93ALine_3.inf", "wb");
	if (NULL == fp){
		AfxMessageBox("不能够建立文件B3PrintCmd.inf");
		return 0;
	}
	char cmdformat[128] = "";
	int Y = FO_Y;//纵轴的坐标(像素)
	
	if (OverBarcode)
	{
		//打印形如 note1:Codestr1 的字符串
		sprintf(cmdformat, "^XA^LH10,10^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note,Codestr);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印Codestr1的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}
	else
	{
		//打印Codestr1的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		sprintf(cmdformat,"^XA^LH10,10^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note1:Codestr1 的字符串
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
//采用Code 93 编码打印三个连续的条码，说明和条码数据同一行
int CreateBACodeItemALine(char *cmd,                       // OUT 编译生成的打印命令
						  char *Codestr1,                  // IN  第一个条码串           如：13800138000
						  char *note1,                     // IN  第一个条码串的说明串   如：TELNUM:
						  char *Codestr2,                  // IN  第二个条码串           如：012345678901234
						  char *note2,                     // IN  第二个条码串的说明串   如：IMEI:
						  char *Codestr3,                  // IN  第三个条码串           如：01234567890123456789
						  char *note3,                     // IN  第三个条码串的说明串   如：CCID:
						  ZPLFONT font,                    // IN  条码解释字符串的字体
						  BARBY   BY,                      // IN  条码模块的设置参数
						  int  FO_X,                       // IN  起始横坐标（In dots）
						  int  FO_Y,                       // IN  起始纵坐标（In dots）
						  BOOL OverBarcode,                // IN  解释字符串在条码的上(TRUE)/下(FALSE)
						  int nShare)                      // IN  打印份数
{
	FILE *fp;
	fp = fopen("Code93ALine_3.inf", "wb");
	if (NULL == fp){
		AfxMessageBox("不能够建立文件B3PrintCmd.inf");
		return 0;
	}
	char cmdformat[128] = "";
	int Y = FO_Y;//纵轴的坐标(像素)


	if (OverBarcode)
	{
		//打印形如 note1:Codestr1 的字符串
		sprintf(cmdformat, "^XA^LH10,10^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印Codestr1的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//打印形如 note2:Codestr2 的字符串
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印Codestr2的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//打印形如 note3:Codestr3 的字符串
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note3,Codestr3);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印Codestr3的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr3);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}
	else
	{
		//打印Codestr1的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		sprintf(cmdformat,"^XA^LH10,10^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note1:Codestr1 的字符串
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		
		//打印Codestr2的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note2:Codestr2 的字符串
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//打印Codestr3的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr3);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note3:Codestr3 的字符串
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
//采用Code 93 编码打印三个在一行的条码（用与装箱的标签）
int CreatePackingItem(char      *cmd,                      // OUT 编译生成的打印命令
					  char      *pTelnum,                  // IN  电话号码字符串
					  char      *pImei,                    // IN  IMEI字符串
					  char      *pCcid,                    // IN  CCID字符串
					  BARBY     barby,                     // IN  条码模块设置结构
					  XY        xy,                        // IN  起始坐标点
					  short     nMMspace,                  // IN  条码间隔
					  short     nShare)                    // IN  打印份数
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
//		MessageBox(NULL, "不能够建立文件Packingitem.inf", "提示", MB_OK|MB_ICONERROR);
//		return 0;
//	}
	strcpy(cmd,   "");

	//打印电话号码
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

	//打印IMEI号
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

	//打印CCID号
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

//采用Code 93 编码打印两个在一行的条码（用与装箱的标签）
int CreatePackingItem(char      *cmd,                      // OUT 编译生成的打印命令
					  char      *pImei,                    // IN  IMEI字符串
					  char      *pCcid,                    // IN  CCID字符串
					  BARBY     barby,                     // IN  条码模块设置结构
					  XY        xy,                        // IN  起始坐标点
					  short     nMMspace,                  // IN  条码间隔
					  short     nShare)                    // IN  打印份数
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
//		MessageBox(NULL, "不能够建立文件Packingitem.inf", "提示", MB_OK|MB_ICONERROR);
//		return 0;
//	}
	strcpy(cmd,   "");
	//打印IMEI号
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

	//打印CCID号
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
