#if !defined(AFX_WRITESOUNDFILE_H__A0EDF320_057F_11D2_A421_E60ECC112860__INCLUDED_)
#define AFX_WRITESOUNDFILE_H__A0EDF320_057F_11D2_A421_E60ECC112860__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WriteSoundFile.h : header file
//
#include "msgType.h"
#include <mmsystem.h>

#include <vector>


typedef struct waveSwingTag
{
	short		crest;
	short		trough;
}waveSwing;


#define WM_WRITESOUNDFILE_FILENAME		WM_USER+700
#define WM_WRITESOUNDFILE_WRITEBLOCK	WM_USER+701
#define WM_WRITESOUNDFILE_CLOSEFILE		WM_USER+702
#define WM_WRITESOUNDFILE_ENDTHREAD		WM_USER+703
#define WM_WRITESOUNDFILE_NOTIFY		WM_USER+704

typedef struct writesoundfile_tag {
	char lpszFileName[MAX_PATH];
	WAVEFORMATEX waveFormatEx;
	TCHAR buffer[100];
} WRITESOUNDFILE, *PWRITESOUNDFILE;

/////////////////////////////////////////////////////////////////////////////
// CWriteSoundFile thread

class CWriteSoundFile : public CWinThread
{
	DECLARE_DYNCREATE(CWriteSoundFile)
public:
	CWriteSoundFile();
protected:
	           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWriteSoundFile)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWriteSoundFile();

	// Generated message map functions
	//{{AFX_MSG(CWriteSoundFile)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg WM_ON_MSG CreateWaveFile(WPARAM wParam, LPARAM lParam);
	afx_msg WM_ON_MSG WriteToSoundFile(WPARAM wParam, LPARAM lParam);
	afx_msg WM_ON_MSG CloseSoundFile(WPARAM wParam, LPARAM lParam);
	afx_msg WM_ON_MSG OnEndThread(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	HMMIO m_hFile;
	MMCKINFO m_MMCKInfoData;
	MMCKINFO m_MMCKInfoParent;
	MMCKINFO m_MMCKInfoChild;

	
protected:
	std::vector<waveSwing> 	m_waveSwing;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITESOUNDFILE_H__A0EDF320_057F_11D2_A421_E60ECC112860__INCLUDED_)
