 /***************************************************************************
 *
 *                         <����绰����ϵͳ>
 *
 *               <Copyright 2003-2005 SIMCOM.COM �����>
 *
 *****************************************************************************
 *  ģ����	    ��PhoneNumber
 *  �ļ���	    ��PhoneNumber.cpp
 *  ����ļ�	��PhoneNumber.h
 *  ʵ�ֹ���    ��ʵ�ֶ��ַ����Ƿ��ǵ绰�����У��
 *  ����		����ʤ
 *  �汾		��V1.0
 *  ʱ��		��2003/8/14
 *****************************************************************************
 *  �޸ļ�¼:
 *
 *      ��  ��		�汾		�޸���		2003/8/14
 *      �޸����ݣ������޸ĵ�ԭ�򼰺�����
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

