#if !defined(__STRINGCHECKENCODE_H__)
#define __STRINGCHECKENCODE_H__


void ucharToHexchar(unsigned char Src_Onech, char *pDst_TwoHexchar);

void uintToHexchar(unsigned int Src_uint, char *pDst_FourHexchar);

int HexcharToInt(char Hexchar);

bool StrIsHexchars(char *pSrc_Hexstring);

int  HexstrToInt(char *pSrc_Hexstring);

unsigned char HexcharToUchar(const char *pSrc_TwoHexchar);

//string Operate
BOOL ReverseBytes(char *str_mul4);

void MakeUpperString(char *pSrc);

//Unicode Encode,Decode
int AscEncodeHexUcs2(const char *pAscSrc,  char *pUnicodeHexDst);

int HexUcs2DecodeAsc(const char *pHexSrc, char *pAscDst);

//string check

BOOL IsNumberString(CString &Src_string);  // 0 - 9

BOOL IsEmptyOrNumberString(CString &Src_string);// empty or only 1-9

BOOL IsHexString(CString &Src_string);     // 0 - 9, a - f, A - F

BOOL IsEmptyOrHexString(CString &Src_string);// empty or only 0 - 9, a - f, A - F

BOOL IsPhoneString(CString &Src_string);		// only 1-9 and * and #

BOOL IsEmptyOrPhoneString(CString &Src_string); // empty or only 1-9 and * and #


#endif // !defined(__STRINGCHECKENCODE_H__)