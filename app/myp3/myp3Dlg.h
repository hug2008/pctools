// myp3Dlg.h : header file
//

#if !defined(AFX_MYP3DLG_H__E663E064_B4B5_422F_A3C4_A19BDB7251AB__INCLUDED_)
#define AFX_MYP3DLG_H__E663E064_B4B5_422F_A3C4_A19BDB7251AB__INCLUDED_
#include "Record.h"
#include "Play.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WAV_FILE_NAME	"Test.wav"

/////////////////////////////////////////////////////////////////////////////
// CMyp3Dlg dialog

DWORD WINAPI ThreadProc(
  LPVOID lpParameter   // thread data
);


class CMyp3Dlg : public CDialog
{
// Construction
public:
	
	CRecord rec;
	CMyp3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMyp3Dlg)
	enum { IDD = IDD_MYP3_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyp3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyp3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonRecStart();
	afx_msg void OnButtonRecStop();
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonSuspend();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonContinue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYP3DLG_H__E663E064_B4B5_422F_A3C4_A19BDB7251AB__INCLUDED_)
