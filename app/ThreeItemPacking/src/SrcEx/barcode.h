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

//���弸�����ݳ��ȵĵ�λ
#define  QUARTER_KILO     (256)
#define  HALF_KILO        (QUARTER_KILO*2)
#define  MAX_MEMORG_LEN   (HALF_KILO*2)
#define  ONE_KILO          MAX_MEMORG_LEN


//����ǰ׺����
#define PrintInitCmd      ("^XA~TA000~JSN^LT0^MMT^MTT^PON^PMN^JMA^MD26^JUS^LRN^XZ")
//����һ���׵�����
#define  MMDOTSA          (12)
#define  MMDOTSB          (MMDOTSA/2)
#define  ACHDOTSA         (18)
#define  ACHDOTSB         (ACHDOTSA*2)

//����һ������ṹ
typedef struct _XY
{
	int x_axis;
	int y_axis;
	void XY(){x_axis = 0; y_axis = 0;};
}XY, *LPXY;

//����һ�����ֵ�ZPL����ṹ
typedef struct  _CHINESEZPL
{
	LPCSTR         lpChinese;      //Ҫ������ַ���
	LPCSTR         lpFontName;     //���������磺"����"
	int            intOrient;      //���巽���
	int            intHeight;      //����߶�
	int            intWidth;       //������
	BOOL           boolBold;       //�������
	BOOL           boolItalic;     //����б��
	short          xmf;            //X�᷽��Ŵ���
	short          ymf;            //Y�᷽��Ŵ���
	XY             xy;             //��ӡλ��
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




//������ת����ZPL���ӡ����
int    GetChineseZplCode(LPCHINESEZPL lpchinesezpl,  LPSTR lpoutcode);

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
						  int  FO_X = 50,                  // IN  ��ʼ�����꣨In dots��
						  int  FO_Y = 50,                  // IN  ��ʼ�����꣨In dots��
						  BOOL OverBarcode = TRUE,         // IN  �����ַ������������(TRUE)/��(FALSE)
						  int nShare = 1);                 // IN  ��ӡ����	

//����Code 93 �����ӡ������һ�е����루����װ��ı�ǩ��
int CreatePackingItem(char      *cmd,                      // OUT �������ɵĴ�ӡ����
					  char      *pTelnum,                  // IN  �绰�����ַ���
					  char      *pImei,                    // IN  IMEI�ַ���
					  char      *pCcid,                    // IN  CCID�ַ���
					  BARBY     barby,                     // IN  ����ģ�����ýṹ
					  XY        xy,                        // IN  ��ʼ�����
					  short     nMMspace,                  // IN  ������
					  short     nShare = 1);               // IN  ��ӡ����

//����Code 93 �����ӡ������һ�е����루����װ��ı�ǩ��
int CreatePackingItem(char      *cmd,                      // OUT �������ɵĴ�ӡ����
					  char      *pImei,                    // IN  IMEI�ַ���
					  char      *pCcid,                    // IN  CCID�ַ���
					  BARBY     barby,                     // IN  ����ģ�����ýṹ
					  XY        xy,                        // IN  ��ʼ�����
					  short     nMMspace,                  // IN  ������
					  short     nShare = 1);               // IN  ��ӡ����

#endif//__BARCODE_H__
