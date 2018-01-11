#if !defined(__MYGLOBALS_H__)
#define __MYGLOBALS_H__

#include <time.h>
#include <string.h>
#include <stdio.h>         /*          NULL         */
#include <time.h>          /*   ctime, asctime      */
#include <assert.h>
#include <io.h> 
#include <windows.h>

BOOL CharIsNum(char ch);

BOOL CharIsAlpha(char ch);

BOOL CharIsAlNum(char ch);

BOOL CStrIsNumber(CString str);

BOOL CStrIsAlpha(CString str);

BOOL CStrIsNumAlpha(CString str);

BOOL CStrIsHexChar(CString str);

void NumStrAutoAdd(char *IO, short increment = 1);

int CharToInt(char ch = '1');

int CStrPortTranInt(CString  Port = _T("COM1"));

int CStrBateTranInt(CString  Bate = _T("115200"));

int CStrParityTranInt(CString Parity = _T("NONE"));

int CStrStopbitsTranInt(CString Stopbit = _T("ONE"));

int CStrDatabitsTranInt(CString Databit = _T("8"));

void GetSysTime(char *systime, char *sysdate);

int  SignchPositionInStr(char *Srcstr, char Signch = '=', int nserial = 1);

BOOL strpositioncpy(char *Src, char *Dst, int strat, int end);

BOOL GetFileRow(char *finename, char *Value, int nRow = 1);

BOOL IntCheckInBound(int becheck, int upperlimit, int lowerlimit);

#endif//(__MYGLOBALS_H__)