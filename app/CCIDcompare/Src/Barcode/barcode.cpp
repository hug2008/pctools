#include "stdafx.h"
#include "barcode.h"
#include <string.h>
#include <WinSpool.h>

//调用动态连接库Fnthex32.dll中函数"GETFONTHEX"的远指针
typedef int (FAR __stdcall*MYWENT)(std::string, std::string, int, int, int, int, int, char *);


//打印端口句柄
HANDLE hLpt;
//命令的前缀
char Prefixion[] = "^XA~TA000~JSN^LT0^MMT^MTT^PON^PMN^JMA^MD26^JUS^LRN^XZ";

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
BOOL  OpenLpt(char *lptport)
{//默认为"LPT1"
	hLpt = CreateFile(lptport,                           // 打印设备
		              GENERIC_WRITE,	                 // 写方式
					  NULL,				                 // 共享方式：独占
					  NULL,			                     // 默认的安全描述符
					  OPEN_EXISTING,	                 // 创建方式
					  NULL,				                 // 不需设置文件属性
					  NULL);			                 // 不需参照模板文件
	if(hLpt == INVALID_HANDLE_VALUE)
		return FALSE;
	return TRUE;
}

//关闭打印端口
BOOL  CloseLpt(void)
{
	return CloseHandle(hLpt);
}

//把buf中长度为nLength的字符串送到打印端口
int   WriteLpt(char *buf, int nLength)
{
	DWORD dwNumWrite;	// 并口发出的数据长度
    //写端口
	WriteFile(hLpt, buf, (DWORD)nLength, &dwNumWrite, NULL);
	//返回实际写的数据长度
	return (int)dwNumWrite;
}

SHORT ExecutePrintCmdInLpt(char *PrintCmd, char *lptport)
{
	BOOL result;
	char *cmd = (char *)malloc(sizeof(char)*HALF_KILO*10);
	if (NULL == cmd){
		MessageBox(NULL, "Can't get buf from stack！", "ERROR", MB_OK|MB_ICONERROR);
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


int CreateGraphicBoxCmd(char  *cmd,                   // OUT 编译生成的打印命令
			  		    int   FO_X,                   // IN  起始横坐标（In dots）
					    int   FO_Y,                   // IN  起始纵坐标（In dots）
					    int   Box_Width,              // IN  矩形的宽度（In dots）[1, 32000]
					    int   Box_Height,             // IN  矩形的高度（In dots）[1, 32000]
					    int   Line_thickness,         // IN  矩形的线条粗度（In dots）[1, 32000]
					    BOOL  LineColor,              // IN  线条的颜色TRUE(black)/FALSE(white)
					    short rounding)               // IN  矩形边角的平滑度[0, 8]
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
	fp = fopen("Code933.nfo", "wb");
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


//采用Code 93 编码打印两个连续的条码，说明和条码数据同一行，条码另起一行
int CreateBACodeItemALine(char *cmd,                       // OUT 编译生成的打印命令
						  char *Codestr1,                  // IN  第二个条码串           如：012345678901234
						  char *note1,                     // IN  第二个条码串的说明串   如：IMEI:
						  char *Codestr2,                  // IN  第三个条码串           如：01234567890123456789
						  char *note2,                     // IN  第三个条码串的说明串   如：CCID:
						  ZPLFONT font,                    // IN  条码解释字符串的字体
						  BARBY   BY,                      // IN  条码模块的设置参数
						  int  FO_X,                       // IN  起始横坐标（In dots）
						  int  FO_Y,                       // IN  起始纵坐标（In dots）
						  BOOL OverBarcode,                // IN  解释字符串在条码的上(TRUE)/下(FALSE)
						  int nShare)                      // IN  打印份数	
{
	FILE *fp;
	fp = fopen("Code932.nfo", "wb");
	if (NULL == fp){
		AfxMessageBox("不能够建立文件BAPrintCmd.inf");
		return 0;
	}
	char cmdformat[128] = "";
	int Y = FO_Y;//纵轴的坐标(像素)
	strcpy(cmd,  "");
	Y = Y+font.height+3;
	Y = Y + BY.heighet + 3;

	if (OverBarcode)
	{
		//打印形如 note1:Codestr1 的字符串
		sprintf(cmdformat, "^XA^LH10,10^FO%d,%d^A%c,%c,%d,%d^FDTELNUM:^FS",
			FO_X,Y-BY.heighet-36,font.name,font.Orientation,font.height,font.width);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		sprintf(cmdformat, "^LH10,10^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印Codestr1的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//打印形如 note2:Codestr2 的字符串
		Y = Y + BY.heighet + 24;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印Codestr2的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
	}
	else
	{
		//打印Codestr1的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		sprintf(cmdformat,"^XA^LH10,10^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr1);
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note1:Codestr1 的字符串
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		
		//打印Codestr2的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+24;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note2:Codestr2 的字符串
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
	fp = fopen("Code933.nfo", "wb");
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
		Y = Y + BY.heighet + 24;
		// 用于HLP200点打印机而设置
		//Y = Y + BY.heighet + 18;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印Codestr2的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height + 3;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//打印形如 note3:Codestr3 的字符串
		Y = Y + BY.heighet + 24;
		// 用于HLP200点打印机而设置
		//Y = Y + BY.heighet + 18;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note3,Codestr3);
		strcat(cmd, cmdformat);
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
		strcpy(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note1:Codestr1 的字符串
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note1,Codestr1);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		
		//打印Codestr2的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+24;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note2:Codestr2 的字符串
		Y = Y + BY.heighet + 3;
		sprintf(cmdformat, "^FO%d,%d^A%c,%c,%d,%d^FD%s:%s^FS",
			FO_X,Y,font.name,font.Orientation,font.height,font.width,note2,Codestr2);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);

		//打印Codestr3的条码命令^BYw,r,h^BAo,h,f,g,e^FD13800138000^FS
		Y = Y+font.height+24;
		sprintf(cmdformat,"^FO%d,%d^BY%d,%3.2f,%d^BAN,,N,N,N^FD%s^FS",
			FO_X, Y, BY.width, BY.width_ratio, BY.heighet ,Codestr3);
		strcat(cmd, cmdformat);
		fprintf(fp, "%s\r", cmdformat);
		//打印形如 note3:Codestr3 的字符串
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
//采用Code 93 编码打印三个在一行的条码（用与装箱的标签）
int CreatePackingItem(char      *cmd,                      // OUT 编译生成的打印命令
					  char      *pTelnum,                  // IN  电话号码字符串
					  char      *pImei,                    // IN  IMEI字符串
					  char      *pCcid,                    // IN  CCID字符串
					  BARBY     barby,                     // IN  条码模块设置结构
					  XY        xy,                        // IN  起始坐标点
					  short     nShare)                    // IN  打印份数
{
	int   Xaxis         = xy.x_axis;
	char  cmdformat[128] = "";
	FILE *fp;
	fp = fopen("Packingitem.inf", "wb");
	if (NULL == fp){
		MessageBox(NULL, "不能够建立文件Packingitem.inf", "提示", MB_OK|MB_ICONERROR);
		return 0;
	}
	strcpy(cmd,   "");

	//打印电话号码
	sprintf(cmdformat,"^XA^FO%d,%d^BY%d,%3.2f,%d^BAN,,Y,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.heighet, barby.width_ratio, barby.heighet, pTelnum);
	strcat(cmd,  cmdformat);
	sprintf(cmdformat, "%s\r", cmdformat);

	//打印IMEI号
	Xaxis = Xaxis + 626;
	sprintf(cmdformat,"^XA^FO%d,%d^BY%d,%3.2f,%d^BAN,,Y,N,N^FD%s^FS",
		Xaxis, xy.y_axis, barby.heighet, barby.width_ratio, barby.heighet, pImei);
	strcat(cmd,  cmdformat);
	sprintf(cmdformat, "%s\r", cmdformat);

	//打印CCID号
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






