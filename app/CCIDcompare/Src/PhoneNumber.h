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
