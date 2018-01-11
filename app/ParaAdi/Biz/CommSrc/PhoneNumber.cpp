 /***************************************************************************
 *
 *                         <商务电话管理系统>
 *
 *               <Copyright 2003-2005 SIMCOM.COM 软件部>
 *
 *****************************************************************************
 *  模块名	    ：PhoneNumber
 *  文件名	    ：PhoneNumber.cpp
 *  相关文件	：PhoneNumber.h
 *  实现功能    ：实现对字符串是否是电话号码的校验
 *  作者		：陈胜
 *  版本		：V1.0
 *  时间		：2003/8/14
 *****************************************************************************
 *  修改记录:
 *
 *      日  期		版本		修改人		2003/8/14
 *      修改内容（包括修改的原因及函数）
 *
 *****************************************************************************/

// PhoneNumber.cpp : implementation file
//

#include "stdafx.h"
#include "bizphonemanager.h"
#include "PhoneNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumber

CPhoneNumber::CPhoneNumber()
{
}

CPhoneNumber::~CPhoneNumber()
{
}

CPhoneNumber::CPhoneNumber(const CString& str)
{
	this->Format("%s", str);
}

CPhoneNumber& CPhoneNumber::operator=(const CString& str)
{
	this->Format("%s", str);

	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumber message handlers

BOOL CPhoneNumber::IsPhoneNumber()
{
	if(this->IsEmpty()) 
		return FALSE;
	else
	{
		int nLength = this->GetLength();
		char ch;
		for(int i=0; i<nLength; i++)
		{
			ch = this->GetAt(i);
			if ((ch > '9')||(ch < '0'))	return FALSE;
		}
	}
	return TRUE;
}

BOOL CPhoneNumber::IsHexNumber()	// only 1-9 and A to F or a to f
{

	if(this->IsEmpty()) 
		return FALSE;
	else
	{
		int nLength = this->GetLength();
		char ch;
		for(int i=0; i<nLength; i++)
		{
			ch = this->GetAt(i);
			if (   !((ch >= '0')&&(ch <= '9'))
				 && !((ch >= 'a')&&(ch <= 'f'))
				 && !((ch >= 'A')&&(ch <= 'F'))
				)
				return FALSE;
		}
	}
	return TRUE;

}

BOOL CPhoneNumber::IsPhoneString()
{
	if(this->IsEmpty()) 
		return FALSE;
	else
	{
		int nLength = this->GetLength();
		char ch;
		for(int i=0; i<nLength; i++)
		{
			ch = this->GetAt(i);
			if (   !((ch >= '0')&&(ch <= '9'))
				 && (ch != '*')
				 && (ch != '#')   )
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CPhoneNumber::IsEmptyOrPhoneString()
{
	if(this->IsEmpty()) 
		return TRUE;
	else
		return IsPhoneString();
}

BOOL CPhoneNumber::IsNumberAlpha()
{
	if(this->IsEmpty()) 
		return FALSE;
	else
	{
		int nLength = this->GetLength();
		char ch;
		for(int i=0; i<nLength; i++)
		{
			ch = this->GetAt(i);
			if (   !((ch >= '0')&&(ch <= '9'))
				 && !((ch >= 'a')&&(ch <= 'z'))
				 && !((ch >= 'A')&&(ch <= 'Z'))
				)
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CPhoneNumber::IsEmptyOrNumberAlpha()
{
	if(this->IsEmpty()) 
		return TRUE;
	else
		return IsNumberAlpha();
}

