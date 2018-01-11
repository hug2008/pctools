// Sendkey.cpp: implementation of the CSendkey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sendkey.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//模拟用户键盘输入的类
CSendkey::CSendkey()
{

}

CSendkey::~CSendkey()
{
	
}
//根据参数模拟用户按键
void CSendkey::Send(KEYDEF * keydef)
{
	//没有普通键被按
	if(keydef->cNormal == 0x00)
	{
		int vk;
		BOOL uptoo = TRUE;
		if(keydef->cShifted > 0x6F) //is a function key
		{
			vk = keydef->cShifted;
		}
		else
		{
			switch(keydef->cShifted)
			{
			case TAB:    // 0x02
				vk = VkKeyScan(0x09) & 0xFF;
				break;
			case CAPSLOCK:    // 0x03
				vk = VK_CAPITAL;
				break;
			case SHIFT:    // 0x04
				vk = VK_SHIFT;
				if(GetKeyState(vk) & 0xF000) return;
				uptoo = FALSE;
				break;
			case CTRL:    // 0x05
				vk = VK_CONTROL;
				if(GetKeyState(vk) & 0xF000) return;
				uptoo = FALSE;
				break;
			case BSPACE:    // 0x06
				vk = VK_BACK;
				break;
			case ENTER:    // 0x07
				vk = VkKeyScan(0x0D) & 0xFF;
				break;
			case ALT:    // 0x08
				vk = VK_MENU;
				break;
			case WINDOW:    // 0x9
				vk = VK_LWIN;
				break;
			case LEFT:    // 0x0A
				vk = VK_LEFT;
				break;
			case RIGHT:    // 0x0B
				vk = VK_RIGHT;
				break;
			case UP:    // 0x0C
				vk = VK_UP;
				break;
			case DOWN:    // 0x0D
				vk = VK_DOWN;
				break;
			case PUP: //0x0F
				vk = VK_PRIOR;
				break;
			case PDN: //0x10
				vk = VK_NEXT;
				break;
			case HOME: //0x11
				vk = VK_HOME;
				break;
			case END: //0x12
				vk = VK_END;
				break;
			case INS: //0x13
				vk = VK_INSERT;
				break;
			case DEL: //0x14
				vk = VK_DELETE;
				break;
			case WWW:
				KEYDEF key;
				key.cNormal = key.cShifted = 'H';
				Send(&key);
				key.cNormal = key.cShifted = 'T';
				Send(&key);
				Send(&key);
				key.cNormal = key.cShifted = 'P';
				Send(&key);
				key.cNormal = key.cShifted = ':';
				Send(&key);
				key.cNormal = key.cShifted = '/';
				Send(&key);
				Send(&key);
				key.cNormal = key.cShifted = 'W';
				Send(&key);
				Send(&key);
				Send(&key);
				vk = VkKeyScan('.') & 0xFF;
				break;
			default:
				ASSERT(FALSE);
				break;
			}
		}
		keybd_event(vk,0,0,0);
		if(uptoo)
		{
			keybd_event(vk,0,KEYEVENTF_KEYUP,0);
		}
	}
	//有普通键被按下
	else
	{
		char pChar = keydef->cNormal;
		SHORT ks = VkKeyScan(pChar);
		BYTE key = ks & 0xFF;
		
		keybd_event(key,0,0,0);
		keybd_event(key,0,KEYEVENTF_KEYUP,0);
		
		//turn off the control and shift if they were down
		//如果"control"和"shift"键是被按下的，则让其弹起
		unsigned char vk = VK_SHIFT;
		if(GetKeyState(vk) & 0xF000)
		{
			keybd_event(vk,0,KEYEVENTF_KEYUP,0);
		}
		vk = VK_CONTROL;
		if(GetKeyState(vk) & 0xF000) 
		{
			keybd_event(vk,0,KEYEVENTF_KEYUP,0);
		}
	}
	
	
}
//将传递的 字符串 模拟用户按键
void CSendkey::Sendit(CString msg)
{
	sts=new KEYDEF;
	char chartosend;
	int vk=VK_SHIFT;
	keybd_event(vk,0,KEYEVENTF_KEYUP,0);
	for (int i=0;i<msg.GetLength();i++)
	{
		chartosend=msg.GetAt(i);
		if ((chartosend=='\\')&&(i<msg.GetLength()-1))
		{
			if (msg.GetAt(i+1)=='n')
			{
				sts->cNormal=0x00;
				sts->cShifted=ENTER;
				Send(sts);
				i++;
				continue;
			}
			else
			{
				sts->cNormal=sts->cShifted=chartosend;
				Send(sts);
			}
		}
		else if ((chartosend>='A'&&chartosend<='Z')||(chartosend>='!'&&chartosend<='+')||(chartosend>='{'&&chartosend<='~')||(chartosend=='+')||(chartosend=='_')||(chartosend=='<')||(chartosend=='>')||(chartosend=='@')||(chartosend=='?')||(chartosend==':')||(chartosend=='^'))
		{
			sts->cNormal=0x00;
			sts->cShifted=SHIFT;
			Send(sts);
			sts->cNormal=sts->cShifted=chartosend;
			Send(sts);
			
		}
		else 
		{
			sts->cNormal=sts->cShifted=chartosend;
			Send(sts);
		}
	}
	
	delete sts;
	return;
	
	
}

