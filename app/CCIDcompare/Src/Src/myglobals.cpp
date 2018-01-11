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

BOOL IsLowerChar(char ch)
{
	if ((ch >= 'a') && (ch <= 'z'))
		return TRUE;
	return FALSE;
}

BOOL IsUpperChar(char ch)
{
	if ((ch >= 'A') && (ch <= 'Z'))
		return TRUE;
	return FALSE;
}

BOOL IsChar(char ch)
{
	if (IsUpperChar(ch)){
		return TRUE;
	}else if (IsLowerChar(ch)){
		return TRUE;
	}else{
		return FALSE;
	}
}

BOOL IsAlphaChar(char ch)
{
	if ((ch >= '0') && (ch <= '9')){
		return TRUE;
	}else if ((ch >= 'A') && (ch <= 'Z')){
		return TRUE;
	}else if ((ch >= 'a') && (ch <= 'z')){
		return TRUE;
	}else{
		return FALSE;
	}
}


BOOL IsHexChar(const char ch)
{
	return ((ch >= '0') && (ch <= '9')) || ((ch >= 'A') && (ch <= 'F')) || (ch >= 'a') && (ch <= 'f');
}

BOOL IsHexStr(const char *Str_p)
{
	if ((NULL == Str_p) || ('\0' == *Str_p))
	{
		return FALSE;
	}

	while (*Str_p)
	{
		if (!IsHexChar(*Str_p))
		{
			return FALSE;
		}
		Str_p++;
	}

	return TRUE;
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

//*****  检查一个字符串是否只包含数字或者字母
BOOL IsAlphaString(CString string)
{
	for (int i=0; i<(int)string.GetLength(); i++){
		if (string.GetAt(i)<'0')
			return FALSE;
		if ((string.GetAt(i)<'A') && (string.GetAt(i) > '9'))
			return FALSE;
		if ((string.GetAt(i)<'a') && (string.GetAt(i) > 'Z'))
			return FALSE;
		if (string.GetAt(i)>'z')
			return FALSE;
	}
	return TRUE;
}

// 在源串(Src_string)中删除Del_string所包含的字符，返回删除后
// 源串(Src_string)的长度
int ModifyString(CString &Src_string, CString Del_string)
{
	for (int i=0; i<Del_string.GetLength(); i++){
		Src_string.Remove(Del_string.GetAt(i));
		if (0 == Src_string.GetLength()){
			return 0;
		}
	}
	return Src_string.GetLength();
}

/*
     保留字符串
	 参数说明：
	    Src_string      ----         源串

		Preserve_Style  ----         保留样式

		                             0 ： 仅保留数字
									 1 ： 仅保留大写字母
									 2 ： 仅保留小写字母
									 3 ： 仅保留字母
									 4 ： 仅保留Alpha字符
	 返回值   -- 整形
	             保留操作后字符串的长度

	 返回 -1 说明参数 Preserve_Style 错误；

*/
int PreserveString(CString &Src_string, char Preserve_Style)
{
	int  i    = 0;
	int Style = (int)Preserve_Style;
	char  removech = 0;
	switch(Style){
		case 0x00:
			for (i=0; i<Src_string.GetLength(); i++){
				if (!CharIsNum(Src_string.GetAt(i))){
					removech = Src_string.GetAt(i);
					Src_string.Remove(removech);
					i = 0;
				}
			}
			break;
		case 0x01:
			for (i=0; i<Src_string.GetLength(); i++){
				if (!IsUpperChar(Src_string.GetAt(i))){
					removech = Src_string.GetAt(i);
					Src_string.Remove(removech);
					i = 0;
				}
			}
			break;
		case 0x02:
			for (i=0; i<Src_string.GetLength(); i++){
				if (!IsLowerChar(Src_string.GetAt(i))){
					removech = Src_string.GetAt(i);
					Src_string.Remove(removech);
					i = 0;
				}
			}
			break;
		case 0x03:
			for (i=0; i<Src_string.GetLength(); i++){
				if (!IsChar(Src_string.GetAt(i))){
					removech = Src_string.GetAt(i);
					Src_string.Remove(removech);
					i = 0;
				}
			}
			break;
		case 0x04:
			for (i=0; i<Src_string.GetLength(); i++){
				if (!IsAlphaChar(Src_string.GetAt(i))){
					removech = Src_string.GetAt(i);
					Src_string.Remove(removech);
					i = 0;
				}
			}
			break;
		default:
			return (-1);
			break;
	}
	return Src_string.GetLength();
}


//*******  列表框的字符数据转换整数
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
	return atoi(Port.Mid(3));
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
		i        = 0,
		len = (int)strlen(Srcstr);
	if (NULL == strstr(Srcstr, &Signch)) return position; //处
	if (nserial > len) return position;                   //理
	for (i=0; i<len; i++)                             //异
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