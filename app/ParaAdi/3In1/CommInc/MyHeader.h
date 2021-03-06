#if !defined(AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)
#define AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#define     STRING_PAGELIMIT_NAME           "PAGELIMITOPTION"
#define		STRING_SECTION1_NAME			"PARA"
#define		STRING_SECTION2_NAME			"ATCMD"


#define		STRING_PARAFILE_NAME		    "parafile.ini"

//几个自定义消息
// for system
#define WM_SYSTEMTRAY                      WM_USER + 2
#define WM_SENDHIGH_NOTIFY                 WM_USER + 3
#define WM_SENDLOW_NOTIFY	               WM_USER + 4
#define WM_RECEIVEHIGH_NOTIFY              WM_USER + 5
#define WM_SENDNEWFIFO_NOTIFY              WM_USER + 6
#define WM_SENDCOMM_NOTIFY                 WM_USER + 7
#define WM_CLEARSTATE_NOTIFY               WM_USER + 8
#define WM_SENDFINISH_NOTIFY               WM_USER + 9
#define WM_COMMMANAGE_NOTIFY               WM_USER + 10
#define WM_RECEIVE_SERIALDATA_NOTIFY       WM_USER + 11
#define WM_RECEIVE_END_NOTIFY              WM_USER + 12
#define WM_START_MONITOR_TIMER_NOTIFY      WM_USER + 13
#define WM_STOP_MONITOR_TIMER_NOTIFY       WM_USER + 14
#define WM_OPEN_SERIAL_CONNECT_NOTIFY      WM_USER + 15
#define WM_CLOSE_SERIAL_CONNECT_NOTIFY     WM_USER + 16
#define WM_START_SEND_ATCOMMAND_NOTIFY     WM_USER + 17
// added by Huang ye for activate commport
#define WM_START_SENDING_TIMER_NOTIFY      WM_USER + 18
#define WM_STOP_SENDING_TIMER_NOTIFY       WM_USER + 19
// added by Huang ye for activate commport

// 2007.06.07 add by hqr to Open/Close Comm
#define WM_OPEN_COMM_CONNECTION_NOTIFY    WM_USER + 20
#define WM_CLOSE_COMM_CONNECTION_NOTIFY   WM_USER + 21
// 2007.06.07 add by hqr to Open/Close Comm

#define WM_READ_PARA_ERR_NOTIFY            WM_USER + 40
#define WM_READ_PARA_OK_NOTIFY             WM_USER + 41
#define WM_READ_PARA_STATE_NOTIFY          WM_USER + 42
#define WM_WRITE_PARA_ERR_NOTIFY           WM_USER + 43
#define WM_WRITE_PARA_OK_NOTIFY            WM_USER + 44
#define WM_WRITE_PARA_STATE_NOTIFY		   WM_USER + 45
#define WM_WRITE_TO_DEFAULT_SETTING_ERR_NOTIFY WM_USER +46
#define WM_WRITE_TO_DEFAULT_SETTING_OK_NOTIFY  WM_USER +47
#define WM_RECOVER_DEFAULT_SETTING_ERR_NOTIFY  WM_USER + 49
#define WM_RECOVER_DEFAULT_SETTING_OK_NOTIFY   WM_USER + 50

#define WM_CHANGE_PASS_ERR_NOTIFY              WM_USER + 52
#define WM_CHANGE_PASS_OK_NOTIFY               WM_USER + 53

#define WM_LOCK_PORO_PAGE_NOTIFY               WM_USER + 54

//for worknote
#define WM_DETECT_ONLINE_END_NOTIFY		   WM_USER + 141
#define WM_CLEAR_SEND_DATA_NOTIFY          WM_USER + 142

#define		WM_MSG_EDIT_END_NOTIFY						WM_USER+ 100

#define		WM_MSG_LOAD_USER_INI_NOTIFY					WM_USER+ 200
#define		WM_MSG_SAVE_USER_INI_NOTIFY					WM_USER+ 201

#define		WM_MSG_LOAD_MODULE_SETTING_NOTIFY			WM_USER+ 202
#define		WM_MSG_SAVE_MODULE_SETTING_NOTIFY			WM_USER+ 203

#define		WM_MSG_SAVE_MODULE_DEFAULT_NOTIFY			WM_USER+ 204
#define		WM_MSG_RECOVER_MODULE_DEFAULT_NOTIFY		WM_USER+ 205

#define		WM_MSG_CHANGE_PWD_NOTIFY					WM_USER+ 206
#define		WM_MSG_OUTPUT_OPERATION_MESSAGE_NOTIFY		WM_USER+ 207
#define		WM_MSG_ENABLE_OPERATION_BUTTOM_NOTIFY		WM_USER+ 208

#define		WM_MSG_ACTIVE_THIS_PAGE_NOTIFY				WM_USER+ 209
// added by Huang ye for activate commport
#define     WM_MSG_ACTIVATE_COMMPORT_NOTIFY             WM_USER+ 210
// added by Huang ye for activate commport


#define		WM_MSG_MAIN_WND_TREE_CONTROL_NOTIFY	        WM_USER+ 211


#endif // !defined(AFX_MYHEADER_H__FBE910CF_DAE2_4D1A_AEF0_E4A9BA7D6DC0__INCLUDED_)
