#include "stdafx.h"
#include "StringCheckEncode.h"
#include <lm.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
/*
* name:     ucharToHexchar()
*
* function: 将一个无符号字符转换成相应的两个16进制字符串
*
* parameter:
*           Src_Onech          -   一个无符号源字符
*
*           pDst_TwoHexchar    -   对应高低位的两个16进制字符串
*/
void ucharToHexchar(unsigned char ch, char *pDst_TwoHexchar)
{
	sprintf(pDst_TwoHexchar, "%x%x", (ch>>4), (ch&0x0F));
}



/*
* name:     uintToHexchar()
*
* function: 将一个无符号整数转换成相应的四个16进制字符串
*
* parameter:
*           Src_Oneuint        -   无符号整数
*
*           pDst_TwoHexchar    -   对应高低位的四个16进制字符串
*/
void uintToHexchar(unsigned int Src_uint, char *pDst_FourHexchar)
{
	sprintf(pDst_FourHexchar, "%x%x%x%x\0", \
		(Src_uint>>12)&0x0F, (Src_uint>>8)&0x0F, (Src_uint>>4)&0x0F, Src_uint&0x0F);
}


/*
* name:     HexcharToInt()
*
* function: 将一个16进制字符转换成整数
*
* parameter:
*           Hexchar          -   一个16进制源字符
*
*/
int HexcharToInt(char Hexchar)
{
	switch (Hexchar)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a':
	case 'A':
		return 0x0a;
	case 'b':
	case 'B':
		return 0x0b;
	case 'c':
	case 'C':
		return 0x0c;
	case 'd':
	case 'D':
		return 0x0d;
	case 'e':
	case 'E':
		return 0x0e;
	case 'f':
	case 'F':
		return 0x0f;
	default:
		return -1;
	}
	return -1;
}


/*
* name:     StrIsHexchars()
*
* function: 判断一个字符串是否为16进制串
*
* parameter:
*           pSrc_Hexstring  -  源字符串
*
*/
bool StrIsHexchars(char *pSrc_Hexstring)
{
	int len = (int)strlen(pSrc_Hexstring);
	char item;
	for (int i=0; i<len; i++)
	{
		item = pSrc_Hexstring[i];
		if (item<'0')
			return false;
		if ((item>'9')&&(item<'A'))
			return false;
		if ((item>'Z')&&(item<'a'))
			return false;
		if (item>'z')
			return false;
	}
	return true;
}



/*
* name:     HexstrToInt()
*
* function: 将16进制串转化成相应的整数
*
* parameter:
*           pSrc_Hexstring  -  源字符串
*
*/
int  HexstrToInt(char *pSrc_Hexstring)
{
	int len = (int)strlen(pSrc_Hexstring);
	int ans = -1, i;
	if (!StrIsHexchars(pSrc_Hexstring))
	{
		MessageBox(NULL, "The string is not a hex's string!", "ERROR", MB_OK);
		return ans;
	}
	ans = i = 0;
	while (i<len)
	{
		ans += HexcharToInt(pSrc_Hexstring[i])*(int)pow((float)16, (float)len-i-1);
		i++;
	}
	return ans;
}

/*
* name:     HexcharToUchar()
*
* function: 将两个16进制串转化成高低位对应的无符号字符
*
* parameter:
*           pSrc_TwoHexchar  -  源16进制字符串
*
*/
unsigned char HexcharToUchar(const char *pSrc_TwoHexchar)
{
	return (unsigned char)(HexcharToInt(pSrc_TwoHexchar[0])*16 \
		+ HexcharToInt(pSrc_TwoHexchar[1]));
}

BOOL IsHexChar(const char Src_Ch)
{
	if (!((Src_Ch >= '0')&&(Src_Ch <= '9'))
		&& !((Src_Ch >= 'a')&&(Src_Ch <= 'f'))
		&& !((Src_Ch >= 'A')&&(Src_Ch <= 'F')))
		return FALSE;
	return TRUE;
}


BOOL IsHexString(const char *Src_String)
{
	int i = 0;
	for (; i<(int)strlen(Src_String); i++)
	{
		if (IsHexChar(Src_String[i]))
			continue;
		return FALSE;
	}
	return TRUE;
}

unsigned char HexcharToUcharVal(char hexchar)
{
	switch (hexchar)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
	case 'a':
		return 10;
	case 'B':
	case 'b':
		return 11;
	case 'C':
	case 'c':
		return 12;
	case 'D':
	case 'd':
		return 13;
	case 'E':
	case 'e':
		return 14;
	case 'F':
	case 'f':
		return 15;
	default:
		return 0;
	}
}

unsigned char TwocharToUcharVal(const char hHexchar, const char lHexchar)
{
	if (!IsHexChar(hHexchar) || !IsHexChar(lHexchar))
	{
		return (0);
	}

	return HexcharToUcharVal(hHexchar)*16 + HexcharToUcharVal(lHexchar);
}

BOOL HexStringToUcharString(unsigned char *Dst, const char *SrcHex)
{
	int len = (int)strlen(SrcHex);
	int i   = 0;
	if (!IsHexString(SrcHex)||(len%2 == 1))
	{
		return FALSE;
	}

	for (;i<len;)
	{
		Dst[i/2] = TwocharToUcharVal(SrcHex[i], SrcHex[i+1]);
		i = i + 2;
	}
	return TRUE;
}

/*
* name:     ReverseBytes()
*
* function: 将16进制的Unicode编码串(UCS2编码)高低字节颠倒
*
* parameter:
*          str_mul4   -  源/目的; UCS2编码字符串，长度为4的倍数
*
*/
BOOL ReverseBytes(char *str_mul4)
{
	char temp;
	int len = (int)strlen(str_mul4);
	if (len%4 != 0)
	{
		MessageBox(NULL, "The Length of Src_string is not multiple of 4", "ERROR", MB_OK|MB_ICONERROR);
		return FALSE;
	}
	for (int i=0; i<len; i=i+4)
	{
		temp = str_mul4[i];

		str_mul4[i] = str_mul4[i+2];

		str_mul4[i+2] = temp;

		temp = str_mul4[i+1];

		str_mul4[i+1] = str_mul4[i+3];

		str_mul4[i+3] = temp;
	}
	return TRUE;
}

/*
* name:     MakeUpperString()
*
* function: 把字符串变成大写
*
* parameter:
*          pSrc   -  源/目的
*
*/
void MakeUpperString(char *pSrc)
{
	for (int i=0; i<(int)strlen(pSrc); i++)
	{
		if ((pSrc[i]>='a')&&(pSrc[i]<='z'))
		{
			pSrc[i] -= 32;
		}
	}
}



/*
* name:     AscEncodeHexUcs2()
*
* function: 将ASCII编码的字符串转换Unicode编码的16进制字符串
*
* parameter:
*          pAscSrc          -  源ASCII字符串
*          pUnicodeHexDst   -  uni对应的16进制字符串（Unicode编码数据）
*/
int AscEncodeHexUcs2(const char *pSrc, char *pHexDst)
{
	int i = 0,
		j = 0;
	char Itemhex[5] = "";
	//源串的长度
	int nSrcLength = (int)strlen(pSrc);
	//取得将pSrc转化成宽字符所需要的长度
	int nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, NULL, 0);
	//给宽字符指针分配内存
	wchar_t *uni = (wchar_t *) malloc(sizeof(wchar_t)*(nDstLength + 1));
	// 字符串-->UNICODE串
	nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, uni, nDstLength);
	//给中间变量分配内存
	unsigned char *pTemp = (unsigned char*) malloc(sizeof(unsigned char)*(nDstLength*2+1));
	// 高低字节对调，输出
	for (i=0; i<nDstLength; i++)
	{
		pTemp[j++] = uni[i] >> 8;		// 先输出高位字节
		pTemp[j++] = uni[i] & 0xff;	    // 后输出低位字节
	}
	//转换成16进制字符串
	strcpy(pHexDst, "");
	for (i=0; i<nDstLength*2; i++)
	{
		ucharToHexchar(pTemp[i], Itemhex);
		strupr(Itemhex);
		strcat(pHexDst, Itemhex);
	}
	free(uni);   //释放内存
	free(pTemp); //释放内存
	return (int)strlen(pHexDst);
}

/*
* name:     HexUcs2DecodeAsc()
*
* function: 将16进制的Unicode编码串(UCS2编码)转换成ASCII编码的字符串
*
* parameter:
*          pHexSrc      -  源, 某个ASCII串的16进制UCS2编码
*          pAscDst      -  目的，pHexSrc对应的ASCII串
*
*/
int HexUcs2DecodeAsc(const char *pHexSrc, char *pAscDst)
{
	//首先要把pHexSrc转换成wchar_t*, 长度为pHexSrc的1/4
	int lenSrc = (int) strlen(pHexSrc);
	// 迭代指针
	int i = 0, j = 0;
	wchar_t * pUni = (wchar_t *)malloc(sizeof(wchar_t)*(lenSrc + 1));
	// 用于存储两个连续的16进制字符
	char Hex[3] = "";
	// 无符号字符,一个byte,存放wchar_t的高、低位
	unsigned char uch = 0;
	while (i<lenSrc)
	{
		// 取连续两个16进制字符
		Hex[0] = pHexSrc[i++];
		Hex[1] = pHexSrc[i++];
		Hex[2] = '\0';
		// 转化成无符号字符
		uch = HexcharToUchar(Hex);
		// 作为wchar_t的高位字节
		pUni[j] = uch<<8;
		//  再取连续两个16进制字符
		Hex[0] = pHexSrc[i++];
		Hex[1] = pHexSrc[i++];
		Hex[2] = '\0';
		// 转化成无符号字符
		uch = HexcharToUchar(Hex);
		// 作为wchar_t的低位字节
		pUni[j] |= uch;
		// wchar_t指针递增1
		j++;
	}
	// wchar_t指针尾加一个结束符
	pUni[j] = L'\0';
	// wchar_t转换成ASCII
	//WcharToAsc(pUni, pAscDst);
	int nDstLength = 0;
	nDstLength = WideCharToMultiByte(CP_ACP,0,pUni,-1,NULL,0,NULL,NULL);
	nDstLength = WideCharToMultiByte(CP_ACP,0,pUni,-1,pAscDst,nDstLength,NULL,NULL);
	free(pUni);
	return nDstLength;
}


BOOL IsNumberString(CString &Src_string)  // 0 - 9
{
	if (Src_string.IsEmpty())
		return FALSE;
	else
	{
		int nLength = Src_string.GetLength(),
			i       = 0;
		char ch     = 0;
		for (i=0; i<nLength; i++)
		{
			ch = Src_string.GetAt(i);
			if ((ch > '9')||(ch < '0'))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}


BOOL IsHexString(CString &Src_string)
{
	if (Src_string.IsEmpty())
		return FALSE;
	else
	{
		int nLength = Src_string.GetLength();
		char ch;
		for (int i=0; i<nLength; i++)
		{
			ch = Src_string.GetAt(i);
			if (   !((ch >= '0')&&(ch <= '9'))
				&& !((ch >= 'a')&&(ch <= 'f'))
				&& !((ch >= 'A')&&(ch <= 'F'))
				)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL IsEmptyOrNumberString(CString &Src_string)
{
	if (Src_string.IsEmpty())
	{

		return TRUE;

	}

	return  IsNumberString(Src_string);
}

BOOL IsEmptyOrHexString(CString &Src_string)
{
	if (Src_string.IsEmpty())
	{

		return TRUE;

	}

	return IsHexString(Src_string);
}

BOOL IsPhoneString(CString &Src_string)
{
	if (Src_string.IsEmpty())
		return FALSE;
	else
	{
		int nLength = Src_string.GetLength();
		char ch;
		for (int i=0; i<nLength; i++)
		{
			ch = Src_string.GetAt(i);
			if (   !((ch >= '0')&&(ch <= '9'))
				&& (ch != '*')
				&& (ch != '#') )
			{
				return FALSE;
			}

		}
	}
	return TRUE;
}

BOOL IsEmptyOrPhoneString(CString &Src_string)
{
	if (Src_string.IsEmpty())
	{
		return TRUE;
	}

	return IsPhoneString(Src_string);

}

int  Getcharcountfromstring(const char *src_string, const char ch)
{
	int ncount = 0;
	while(*src_string){
		if (ch == *src_string++){ncount++;}
	}
	return ncount;
}

BOOL IsNumberChar(const char Src_Ch)
{
	if ((Src_Ch > '9')||(Src_Ch < '0'))
		return FALSE;
	return TRUE;
}

BOOL TrimString(char *Src, int nFixLen, int cFixchar)
{
	int i = 0;
	if ((nFixLen < 1) || (cFixchar < 1)){
		return FALSE;
	}
	/*
	if (sizeof(Src) < nFixLen){
	return FALSE;
	}

	if (sizeof(Src) > nFixLen) Src[nFixLen] = 0x00;
	*/

	if (nFixLen == strlen(Src)){
		return TRUE;
	}

	for (i=(int)strlen(Src); i<nFixLen; i++)
		Src[i] = (char)cFixchar;

	Src[nFixLen] = 0x00;

	return TRUE;
}

#if defined(ENABLE_SESSION_TOLL_FUNCTION)

BOOL AnalyzeItemChargeTable(char *szTime01,
							char *szCharge01,
							char *szTime02,
							char *szCharge02,
							char *szSCharge,
							const char *szCommFormItem)
{
	char *pT01 = szTime01;
	char *pC01 = szCharge01;
	char *pT02 = szTime02;
	char *pC02 = szCharge02;
	char *pSC  = szSCharge;
	int nLen   = (int)strlen(szCommFormItem);
	int i      = 0;

	if ((nLen > 20) || (nLen < 10)){
		return FALSE;
	}

	if ((szCommFormItem[0] == '*') || (szCommFormItem[nLen - 1] != '*')){
		return FALSE;
	}


	if (5 != Getcharcountfromstring(szCommFormItem, '*')){
		return FALSE;
	}


	for (i=0; i<(int)strlen(szCommFormItem); i++){
		if (szCommFormItem[i] == '*'){continue;}
		if (!IsNumberChar(szCommFormItem[i]))
			return FALSE;			
	}

	nLen = 0;	
	while(*szCommFormItem){
		if (*szCommFormItem == '*'){
			nLen++;
			szCommFormItem++;
			continue;
		}
		if (nLen == 0){
			*szTime01++ = *szCommFormItem++;
		}else if (nLen == 1){
			*szCharge01++ = *szCommFormItem++;
		}else if (nLen == 2){
			*szTime02++ = *szCommFormItem++;
		}else if (nLen == 3){
			*szCharge02++ = *szCommFormItem++;
		}else if (nLen == 4){
			*szSCharge++ = *szCommFormItem++;
		}else{
			*szTime01   = 0x00;
			*szCharge01 = 0x00;
			*szTime02   = 0x00;
			*szCharge02 = 0x00;
			*szSCharge  = 0x00;
		}
	}

	return (strlen(pT01) > 0)&&
		(strlen(pT01) < 5)&&
		(strlen(pC01) > 0)&&
		(strlen(pC01) < 5)&&
		(strlen(pT02) > 0)&&
		(strlen(pT02) < 5)&&
		(strlen(pC02) > 0)&&
		(strlen(pC02) < 5)&&
		(strlen(pSC)  > 0)&&
		(strlen(pSC)  < 5);
}

BOOL TransformChargeTable_CommTo3In1(char *sz3In1Form, const char *szCommFormItem)
{
	char szTime01[5]   = "", 
		szCharge01[5] = "", 
		szTime02[5]   = "", 
		szCharge02[5] = "", 
		szSCharge[5]  = "";
	if (!AnalyzeItemChargeTable(szTime01, szCharge01, 
		szTime02, szCharge02, szSCharge, szCommFormItem)){
			return FALSE;
		}
		sprintf(sz3In1Form, "%04X%04X%04X%04X%04X00", atoi(szTime01), atoi(szCharge01), 
			atoi(szTime02), atoi(szCharge02), atoi(szSCharge));
		return TRUE;
}

BOOL AnalyzeItemChargeHeader(char *szHeader,
							 char *szType,
							 const char *szCommForm)
{
	char *pH = szHeader;
	char *pT = szType;
	int nLen = (int)strlen(szCommForm);
	int i    = 0;
	if (2 != Getcharcountfromstring(szCommForm, '*')){
		return FALSE;
	}
	if ((nLen < 3) || (nLen > 17)){
		return FALSE;
	}
	if (szCommForm[nLen - 1] != '*'){
		return FALSE;
	}
	for (i=0; i<(int)strlen(szCommForm); i++){
		if (szCommForm[i] == '*'){continue;}
		if (!IsNumberChar(szCommForm[i]))
			return FALSE;			
	}
	if (*szCommForm == '*'){//无效的
		return FALSE;
	}
	nLen = 0;
	while(*szCommForm)
	{
		if (*szCommForm == '*') 
		{
			nLen++;
			szCommForm++;
			continue;
		}
		if (nLen == 0)
		{
			*szHeader++ = *szCommForm++;
		}
		else if (nLen == 1)
		{
			*szType++ = *szCommForm++;
		}
		else
		{
			*szHeader = 0x00;
			*szType   = 0x00;
			break;
		}
	}

	return (strlen(pH) > 0)&&
		(strlen(pH) < 13)&&
		(atoi(pT) > 1)&&
		(atoi(pT)  <= NUM_RATE_SETTING_TABLE_TOTAL);

}

BOOL TransformChargeHeader_CommTo3In1(char *sz3In1Form, const char *szCommFormItem)
{
	char szheader[15] = "";
	char sztype[5]    = "";
	char szItem[5]    = "";
	int  i            = 0;
	if (!AnalyzeItemChargeHeader(szheader, sztype, szCommFormItem)){
		return FALSE;
	}

	TrimString(szheader, 13, 0x00);

	for (i=0; i<13; i++){
		sprintf(szItem, "%02X", szheader[i]);

		strcat(sz3In1Form, szItem);
	}

	// 计费类型要根据填写的数据减 1
	sprintf(szItem, "%02X00", atoi(sztype) - 1);

	strcat(sz3In1Form, szItem);

	return (30 == strlen(sz3In1Form));
}
#endif