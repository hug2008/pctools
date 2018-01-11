// record.h : main header file for the RECORD application
//

#if !defined(AFX_RECORD_H__5260C00F_03B2_11D2_A421_FC4B2C882A60__INCLUDED_)
#define AFX_RECORD_H__5260C00F_03B2_11D2_A421_FC4B2C882A60__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "RecordSound.h"
#include "PlaySound.h"
#include "PlayMMSound.h"



/////////////////////////////////////////////////////////////////////////////
// CRecordApp:
// See record.cpp for the implementation of this class
//

class CRecordApp : public CWinApp
{
public:
	CRecordApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	virtual int ExitInstance();

// Implementation

	//{{AFX_MSG(CRecordApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	protected:
		CRecordSound* m_pRecordSound;
		CPlaySound* m_pPlaySound;
		CWriteSoundFile* m_pWriteSound;
		CPlayMMSound* m_pPlayMMSound;
		
	protected:
		BOOL InitRecording();
		BOOL InitPlaying();
		BOOL InitWriting();
		BOOL InitPlayMMSound();
		

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORD_H__5260C00F_03B2_11D2_A421_FC4B2C882A60__INCLUDED_)
