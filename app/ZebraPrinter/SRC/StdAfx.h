// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#if !defined(AFX_STDAFX_H__A6BD1D9B_DAAD_4FE3_842A_1A5426CB5E15__INCLUDED_)
#define AFX_STDAFX_H__A6BD1D9B_DAAD_4FE3_842A_1A5426CB5E15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define zh_CN               (0x0804)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#include "XTrace.h"

void  SetRadioCheck(CDialog *hDlg, int nRadioId, BOOL bRadioCheck = FALSE);

void  EnableDlgItemWindow(CDialog *hDlg, int nDlgItemId, BOOL bEnabde = FALSE);

BOOL  IsRadioCheck(CDialog *hDlg, int nRadioId);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_STDAFX_H__A6BD1D9B_DAAD_4FE3_842A_1A5426CB5E15__INCLUDED_)
