 /***************************************************************************
 *
 *                         <商务电话管理系统>
 *
 *               <Copyright 2003-2005 SIMCOM.COM 软件部>
 *
 *****************************************************************************
 *  模块名	    ：PhoneNumber
 *  文件名	    ：PhoneNumber.h
 *  相关文件	：PhoneNumber.CPP
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

#if !defined(AFX_PHONENUMBER_H__9838E400_FE37_4398_97D8_4729286BDF7D__INCLUDED_)
#define AFX_PHONENUMBER_H__9838E400_FE37_4398_97D8_4729286BDF7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhoneNumber.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumber window

class CPhoneNumber : public CString
{
// Construction
public:
	CPhoneNumber();
	CPhoneNumber(const CString& str);
	virtual ~CPhoneNumber();

	CPhoneNumber& operator=( const CString& str );


	BOOL IsHexNumber();			// only 1-9 and A to F or a to f
	BOOL IsPhoneNumber();		// only 1-9
	
	BOOL IsPhoneString();		// only 1-9 and * and #
	BOOL IsEmptyOrPhoneString();// empty or only 1-9 and * and #
	
	BOOL IsNumberAlpha();		// only 1-9 and a-z and A-Z
	BOOL IsEmptyOrNumberAlpha();// empty or only 1-9 and a-z and A-Z

// Attributes
public:

// Operations
public:

// Implementation
public:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHONENUMBER_H__9838E400_FE37_4398_97D8_4729286BDF7D__INCLUDED_)
