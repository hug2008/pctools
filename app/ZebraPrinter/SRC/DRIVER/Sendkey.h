// Sendkey.h: interface for the CSendkey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENDKEY_H__42567959_2D46_4CA9_82FC_AFF748D0CA37__INCLUDED_)
#define AFX_SENDKEY_H__42567959_2D46_4CA9_82FC_AFF748D0CA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//Spec keys code
#define    TAB            0x02		
#define    CAPSLOCK       0x03
#define    SHIFT          0x04
#define    CTRL           0x05
#define    BSPACE         0x06
#define    ENTER          0x07
#define    ALT            0x08
#define    WINDOW         0x09
#define    LEFT           0x0A
#define    RIGHT          0x0B
#define    UP             0x0C
#define    DOWN           0x0D
#define    ESC            0x0E
#define    PUP            0x0F
#define    PDN            0x10
#define    HOME           0x11
#define    END            0x12
#define    INS            0x13
#define    DEL            0x14
#define    WWW            0x15


struct KEYDEF
{
	char cNormal;		//键值
	char cShifted;		//系统键值
};

class CSendkey  
{
public:
	void Send(KEYDEF *keydef);
	void Sendit(CString msg);
	CSendkey();
	virtual ~CSendkey();
	KEYDEF *sts;
	
};

#endif // !defined(AFX_SENDKEY_H__42567959_2D46_4CA9_82FC_AFF748D0CA37__INCLUDED_)
