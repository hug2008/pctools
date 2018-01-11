#if !defined(__BARCODE_H__)
#define __BARCODE_H__


#include <string>
using namespace std;

//�����ӡ�����ù����п��ܳ��ֵļ������
#define    ERR_OPEN_LPT            (0x00)    //�򿪴�ӡ�˿�ʧ��
#define    ERR_CLOSE_LPT           (0x01)    //�رմ�ӡ�˿�ʧ��
#define    ERR_CMD_EMPTY           (0x02)    //��ӡ����Ϊ��
#define    ERR_WRITE_LOSE_DATA     (0x03)    //д��ӡ�˿�ʱ��ʧ����
#define    SUCCEED_LPT             (0x04)    //��ӡ�ɹ�



//����һ������ṹ
typedef struct _XY
{
	int x_axis;
	int y_axis;
	void XY(){x_axis = 30; y_axis = 30;};
}XY, *LPXY;

//����һ�����ֵĴ�ӡ�ṹ
typedef struct  _CHINESEZPL
{
	std::string    strChinese;     //Ҫ������ַ���
	std::string    strFontName;    //���������磺"����"
	int            intOrient;      //���巽���
	int            intHeight;      //����߶�
	int            intWidth;       //������
	BOOL           boolBold;       //�������
	BOOL           boolItalic;     //����б��
	XY             xyStart;        //��ʼ�������
	short          shortMx;        //x�᷽��Ŵ���[1, 10]
	short          shortMy;        //y�᷽��Ŵ���[1, 10]
}CHINESEZPL, *LPCHINESEZPL;

//����һ������Ľṹ��
typedef struct _ZPLFONT
{
	char name;            //��������ȡֵΪ����{'0','1','2',...,'9','A','B','C',...,'Z'}֮һ
	char Orientation;     //��б�ȣ�ȡֵΪ����{'N','R','I','B'}֮һ
	int  height;          //�ַ��߶ȣ�In dots�� ȡֵ[10, 32000]
	int  width;           //�ַ���ȣ�In dots�� ȡֵ[10, 32000]
}ZPLFONT;


//����һ����������ṹ
typedef struct _BARBY
{
	short width;             //ģ����(����),ȡֵ��Χ[1, 10]
	float width_ratio;       //������խ���Ŀ�ȱ���(0.1),ȡֵ��Χ[2.0, 3.0]
	int   heighet;           //����ĸ߶�(����),ȡֵ��Χ[1, 32000]
}BARBY;


BOOL   OpenLpt(char *lptport = "LPT1");
BOOL   CloseLpt(void);
int    WriteLpt(char *buf, int nLength);
SHORT  ExecutePrintCmdInLpt(char *PrintCmd, char *lptport = "LPT1");



int    GetChineseZplCode(LPCHINESEZPL lpchinesezpl,  char *outcode);

//���ɾ��λ���ֱ�ߵĴ�ӡ����
int CreateGraphicBoxCmd(char  *cmd,                   // OUT �������ɵĴ�ӡ����
				    	int   FO_X = 80,              // IN  ��ʼ�����꣨In dots��
					    int   FO_Y = 80,              // IN  ��ʼ�����꣨In dots��
					    int   Box_Width = 200,        // IN  ���εĿ�ȣ�In dots��[1, 32000]
					    int   Box_Height = 200,       // IN  ���εĸ߶ȣ�In dots��[1, 32000]
					    int   Line_thickness = 10,    // IN  ���ε������ֶȣ�In dots��[1, 32000]
					    BOOL  LineColor = TRUE,       // IN  ��������ɫTRUE(black)/FALSE(white)
					    short rounding = 0);          // IN  ���α߽ǵ�ƽ����[0, 8]

//����Code 93 �����ӡһ�����룬˵������������ͬһ��
int CreateBACodeItemALine(char *cmd,                       // OUT �������ɵĴ�ӡ����
						  char *Codestr,                   // IN  ��һ�����봮           �磺13800138000
						  char *note,                      // IN  ��һ�����봮��˵����   �磺TELNUM:
						  ZPLFONT font,                    // IN  ��������ַ���������
						  BARBY   BY,                      // IN  ����ģ������ò���
						  int  FO_X = 50,                  // IN  ��ʼ�����꣨In dots��
						  int  FO_Y = 50,                  // IN  ��ʼ�����꣨In dots��
						  BOOL OverBarcode = TRUE,         // IN  �����ַ������������(TRUE)/��(FALSE)
						  int nShare = 1);                 // IN  ��ӡ����	

//����Code 93 �����ӡ�������������룬˵������������ͬһ�У���������һ��
int CreateBACodeItemALine(char *cmd,                       // OUT �������ɵĴ�ӡ����
						  char *Codestr1,                  // IN  �ڶ������봮           �磺012345678901234
						  char *note1,                     // IN  �ڶ������봮��˵����   �磺IMEI:
						  char *Codestr2,                  // IN  ���������봮           �磺01234567890123456789
						  char *note2,                     // IN  ���������봮��˵����   �磺CCID:
						  ZPLFONT font,                    // IN  ��������ַ���������
						  BARBY   BY,                      // IN  ����ģ������ò���
						  int  FO_X = 50,                  // IN  ��ʼ�����꣨In dots��
						  int  FO_Y = 50,                  // IN  ��ʼ�����꣨In dots��
						  BOOL OverBarcode = TRUE,         // IN  �����ַ������������(TRUE)/��(FALSE)
						  int nShare = 1);                 // IN  ��ӡ����	


//����Code 93 �����ӡ�������������룬˵������������ͬһ�У���������һ��
int CreateBACodeItemALine(char *cmd,                       // OUT �������ɵĴ�ӡ����
						  char *Codestr1,                  // IN  ��һ�����봮           �磺13800138000
						  char *note1,                     // IN  ��һ�����봮��˵����   �磺TELNUM:
						  char *Codestr2,                  // IN  �ڶ������봮           �磺012345678901234
						  char *note2,                     // IN  �ڶ������봮��˵����   �磺IMEI:
						  char *Codestr3,                  // IN  ���������봮           �磺01234567890123456789
						  char *note3,                     // IN  ���������봮��˵����   �磺CCID:
						  ZPLFONT font,                    // IN  ��������ַ���������
						  BARBY   BY,                      // IN  ����ģ������ò���
						  int  FO_X = 50,                  // IN  ��ʼ�����꣨In dots��
						  int  FO_Y = 50,                  // IN  ��ʼ�����꣨In dots��
						  BOOL OverBarcode = TRUE,         // IN  �����ַ������������(TRUE)/��(FALSE)
						  int nShare = 1);                 // IN  ��ӡ����	

//*****************************************************************************************************

//����Code 93 �����ӡ������һ�е����루����װ��ı�ǩ��
int CreatePackingItem(char      *cmd,                      // OUT �������ɵĴ�ӡ����
					  char      *pTelnum,                  // IN  �绰�����ַ���
					  char      *pImei,                    // IN  IMEI�ַ���
					  char      *pCcid,                    // IN  CCID�ַ���
					  BARBY     barby,                     // IN  ����ģ�����ýṹ
					  XY        xy,                        // IN  ��ʼ�����
					  short     nShare = 1);               // IN  ��ӡ����
#endif//__BARCODE_H__
