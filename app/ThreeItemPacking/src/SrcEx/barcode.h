#if !defined(__BARCODE_H__)
#define __BARCODE_H__


#include <string>
using namespace std;

//定义打印机调用过程中可能出现的几种情况
#define    ERR_OPEN_LPT            (0x00)    //打开打印端口失败
#define    ERR_CLOSE_LPT           (0x01)    //关闭打印端口失败
#define    ERR_CMD_EMPTY           (0x02)    //打印命令为空
#define    ERR_WRITE_LOSE_DATA     (0x03)    //写打印端口时丢失数据
#define    SUCCEED_LPT             (0x04)    //打印成功

//定义几个数据长度的单位
#define  QUARTER_KILO     (256)
#define  HALF_KILO        (QUARTER_KILO*2)
#define  MAX_MEMORG_LEN   (HALF_KILO*2)
#define  ONE_KILO          MAX_MEMORG_LEN


//定义前缀命令
#define PrintInitCmd      ("^XA~TA000~JSN^LT0^MMT^MTT^PON^PMN^JMA^MD26^JUS^LRN^XZ")
//定义一毫米的像素
#define  MMDOTSA          (12)
#define  MMDOTSB          (MMDOTSA/2)
#define  ACHDOTSA         (18)
#define  ACHDOTSB         (ACHDOTSA*2)

//定义一个坐标结构
typedef struct _XY
{
	int x_axis;
	int y_axis;
	void XY(){x_axis = 0; y_axis = 0;};
}XY, *LPXY;

//定义一个汉字的ZPL编码结构
typedef struct  _CHINESEZPL
{
	LPCSTR         lpChinese;      //要编码的字符串
	LPCSTR         lpFontName;     //字体名，如："宋体"
	int            intOrient;      //字体方向角
	int            intHeight;      //字体高度
	int            intWidth;       //字体宽度
	BOOL           boolBold;       //字体粗体
	BOOL           boolItalic;     //字体斜体
	short          xmf;            //X轴方向放大倍数
	short          ymf;            //Y轴方向放大倍数
	XY             xy;             //打印位置
}CHINESEZPL, *LPCHINESEZPL;

//定义一个字体的结构体
typedef struct _ZPLFONT
{
	char name;            //字体名，取值为集合{'0','1','2',...,'9','A','B','C',...,'Z'}之一
	char Orientation;     //倾斜度，取值为集合{'N','R','I','B'}之一
	int  height;          //字符高度（In dots） 取值[10, 32000]
	int  width;           //字符宽度（In dots） 取值[10, 32000]
}ZPLFONT;


//定义一个条码参数结构
typedef struct _BARBY
{
	short width;             //模块宽度(像素),取值范围[1, 10]
	float width_ratio;       //宽条到窄条的宽度比率(0.1),取值范围[2.0, 3.0]
	int   heighet;           //条码的高度(像素),取值范围[1, 32000]
}BARBY;


BOOL   OpenLpt(char *lptport = "LPT1");
BOOL   CloseLpt(void);
int    WriteLpt(char *buf, int nLength);
SHORT  ExecutePrintCmdInLpt(char *PrintCmd, char *lptport = "LPT1");




//中文字转换成ZPLⅡ打印代码
int    GetChineseZplCode(LPCHINESEZPL lpchinesezpl,  LPSTR lpoutcode);

//采用Code 93 编码打印一个条码，说明和条码数据同一行
int CreateBACodeItemALine(char *cmd,                       // OUT 编译生成的打印命令
						  char *Codestr,                   // IN  第一个条码串           如：13800138000
						  char *note,                      // IN  第一个条码串的说明串   如：TELNUM:
						  ZPLFONT font,                    // IN  条码解释字符串的字体
						  BARBY   BY,                      // IN  条码模块的设置参数
						  int  FO_X = 50,                  // IN  起始横坐标（In dots）
						  int  FO_Y = 50,                  // IN  起始纵坐标（In dots）
						  BOOL OverBarcode = TRUE,         // IN  解释字符串在条码的上(TRUE)/下(FALSE)
						  int nShare = 1);                 // IN  打印份数	


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
						  int  FO_X = 50,                  // IN  起始横坐标（In dots）
						  int  FO_Y = 50,                  // IN  起始纵坐标（In dots）
						  BOOL OverBarcode = TRUE,         // IN  解释字符串在条码的上(TRUE)/下(FALSE)
						  int nShare = 1);                 // IN  打印份数	

//采用Code 93 编码打印三个在一行的条码（用与装箱的标签）
int CreatePackingItem(char      *cmd,                      // OUT 编译生成的打印命令
					  char      *pTelnum,                  // IN  电话号码字符串
					  char      *pImei,                    // IN  IMEI字符串
					  char      *pCcid,                    // IN  CCID字符串
					  BARBY     barby,                     // IN  条码模块设置结构
					  XY        xy,                        // IN  起始坐标点
					  short     nMMspace,                  // IN  条码间隔
					  short     nShare = 1);               // IN  打印份数

//采用Code 93 编码打印两个在一行的条码（用与装箱的标签）
int CreatePackingItem(char      *cmd,                      // OUT 编译生成的打印命令
					  char      *pImei,                    // IN  IMEI字符串
					  char      *pCcid,                    // IN  CCID字符串
					  BARBY     barby,                     // IN  条码模块设置结构
					  XY        xy,                        // IN  起始坐标点
					  short     nMMspace,                  // IN  条码间隔
					  short     nShare = 1);               // IN  打印份数

#endif//__BARCODE_H__
