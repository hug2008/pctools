#include "stdafx.h"
#include "myglobals.h"


BOOL CharIsNum(char ch)
{
	if(ch > '9')
		return FALSE;
	if(ch < '0')
		return FALSE;
	return TRUE;
}

BOOL CharIsAlpha(char ch)
{
	if ((ch >= 'a')&&(ch <= 'z'))
		return TRUE;
	if ((ch >= 'A')&&(ch <= 'Z'))
		return TRUE;
	return FALSE;
}

BOOL CharIsAlNum(char ch)
{
	if (CharIsNum(ch) || CharIsAlpha(ch))
		return TRUE;
	return FALSE;
}


//*****  检查一个字符串是否都是数字
BOOL CStrIsNumber(CString str)
{
	int i = 0;
	for (i=0; i<str.GetLength(); i++){
		if ((str.GetAt(i)>'9') || (str.GetAt(i)<'0')){
			return false;
		}
	}
	return true;
}

//检查一个字符串是否都是字母
BOOL CStrIsAlpha(CString str)
{
	int i;
	for (i=0; i<str.GetLength(); i++){
		if (!CharIsAlpha(str.GetAt(i)))
			return FALSE;
	}
	return TRUE;
}

//检查一个字符串是否只含有数字和字母
BOOL CStrIsNumAlpha(CString str)
{
	int i;
	for (i=0; i<str.GetLength(); i++){
		if (!CharIsAlNum(str.GetAt(i)))
			return FALSE;
	}
	return TRUE;
}

//检查一个字符串是否为16进制字符
BOOL CStrIsHexChar(CString str)
{
	int i;
	for (i=0; i<str.GetLength(); i++){
		if (CharIsNum(str.GetAt(i)))
			continue;
		if (str.GetAt(i) >= 'a' && str.GetAt(i) <= 'f')
			continue;
		if (str.GetAt(i) >= 'A' && str.GetAt(i) <= 'F')
			continue;
		return FALSE;
	}
	return TRUE;
}

//**** 数字串按照增量递增
void NumStrAutoAdd(char *IO, short increment)
{
	int LEN = (int)strlen(IO);
	assert(LEN > 0);
	assert(LEN < 256);
	char  ZERO = '0';
	short Array[256];
	short Carry = 0;
	strrev(IO);
	for (int i=0; i<LEN; i++)
		Array[i] = IO[i] - ZERO;
	Array[0] = Array[0] + increment;
	Carry    = Array[0]/10;
	Array[0] = Array[0]%10;
	for (i=1; i<LEN; i++){
		Array[i] += Carry;
		Carry    = Array[i]/10;
		Array[i] = Array[i]%10;
	}
	for (i=0; i<LEN; i++)
		IO[i] = ZERO + Array[i];
	strrev(IO);
	return;
}

//**************  列表框的字符数据转换整数   **********************
int CharToInt(char ch)
{
	switch(ch){
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	default:
		return -1;
		break;
	};
}

int CStrPortTranInt(CString  Port)
{
	ASSERT(!Port.IsEmpty());
	return CharToInt(Port.GetAt(3));
}

int CStrBateTranInt(CString  Bate)
{
	ASSERT(!Bate.IsEmpty());
	char *endptr,
		 p[10] = "";
	strcpy(p, Bate);
	return (int)strtol(p, &endptr, 10);
}

int CStrParityTranInt(CString Parity/* = _T("NONE")*/)
{
	ASSERT(!Parity.IsEmpty());
	if (Parity == "NONE")
		return NOPARITY/*0*/;
	else if ((Parity == "ODD"))
		return ODDPARITY/*1*/;
	else if ((Parity == "EVEN"))
		return EVENPARITY/*2*/;
	else
		return -1;
}

int CStrStopbitsTranInt(CString Stopbit/* = _T("ONE")*/)
{//ONE,TWO
	ASSERT(!Stopbit.IsEmpty());
	if (Stopbit == "ONE")
		return ONE5STOPBITS;//1;
	else if (Stopbit == "TWO")
		return TWOSTOPBITS;//2
	else
		return ONESTOPBIT;//0;
}

int CStrDatabitsTranInt(CString Databit/* = _T("8")*/)
{
	ASSERT(!Databit.IsEmpty());
	return CharToInt(Databit.GetAt(0));
}






//*************************************************************************************

void GetSysTime(char *systime, char *sysdate)
{
	time_t now;   

    now = time((time_t *)NULL); 

	struct tm *l_time;
      
    l_time = localtime(&now); 

	sprintf(sysdate, "%d/%d%d/%d%d", l_time->tm_year+1900,
		                            (l_time->tm_mon+1)/10,(l_time->tm_mon+1)%10,
									(l_time->tm_mday)/10, (l_time->tm_mday)%10);

	sprintf(systime, "%d%d:%d%d:%d%d",
		             l_time->tm_hour/10,l_time->tm_hour%10,
					 l_time->tm_min/10, l_time->tm_min%10,
					 l_time->tm_sec/10, l_time->tm_sec%10);

	return;
}

/*******************************************************************
/*******************************************************************
*
*  原形:    SignchPositionInStr()
*
*  功能:    获得源字符串中第nserial个标志字符Signch的位置
*
*  参数:    Srcstr - 源字符串指针,Signch - 目标字符,nserial - 第nserial个Signch
*
*  返回值:  第serial个标志字符Signch的位置(整型),异常则返回0
*
*******************************************************************/
int  SignchPositionInStr(char *Srcstr, char Signch, int nserial)
{
	int SignCount = 0, 
		position = 0,
		len = (int)strlen(Srcstr);
	if (NULL == strstr(Srcstr, &Signch)) return position; //处
	if (nserial > len) return position;                   //理
	for (int i=0; i<len; i++)                             //异
		if (Srcstr[i] == Signch)                          //常
			SignCount++;                                  //情
	if (nserial > SignCount) return position;             //况
	for (SignCount=i=0; i<len; i++)
	{
		if (Srcstr[i] == Signch)
		{
			SignCount++;
			position = i+1;
		}
		if (nserial == SignCount) break;
	}
	return position;
}

BOOL strpositioncpy(char *Src, char *Dst, int strat, int end)
{//拷贝Src下标从strat到end的内容到Dst
	int i, lenSrc;
	lenSrc = (int)strlen(Src);	
	if ((strat<0) || (end<0)){
		return false;
	}
	i = end-strat;
	if (i < 0) return false;
	if (lenSrc< i) return false;
	for (i=0; i<end-strat; i++){
		Dst[i] = Src[i+strat];
	}
	Dst[i] = '\0';
	return true;
}

BOOL GetFileRow(char *filename, char *Value, int nRow)
{
	FILE *fp;

	int index = 0;

	char temp[100] = "";

	fp = fopen(filename, "rb");

	if (NULL == fp){
		sprintf(temp, "Can't Open the fine: %s!", filename);
		AfxMessageBox(temp);
		return FALSE;
	}

	if (nRow <1) nRow = 1;

	while(index<nRow && !feof(fp))
	{
		fscanf(fp, "%s", Value);
		index++;
	}

	fclose(fp);

	index = SignchPositionInStr(Value);

	if (index == 0){
		AfxMessageBox(Value);
		strcpy(Value, "ERROR");
		AfxMessageBox(Value);
		return FALSE;
	}

	strpositioncpy(Value, temp, index, (int)strlen(Value));

	strcpy(Value, temp);

	return TRUE;

}

BOOL IntCheckInBound(int becheck, int lowerlimit, int upperlimit)
{
	if (becheck < lowerlimit)
		return FALSE;
	if (becheck > upperlimit)
		return FALSE;
	return TRUE;
}