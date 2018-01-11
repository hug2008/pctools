// recordDlg.h : header file
//

#if !defined(AFX_RECORDDLG_H__5260C011_03B2_11D2_A421_FC4B2C882A60__INCLUDED_)
#define AFX_RECORDDLG_H__5260C011_03B2_11D2_A421_FC4B2C882A60__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "RecordSound.h"
#include "PlaySound.h"

/////////////////////////////////////////////////////////////////////////////
// CRecordDlg dialog

class CRecordDlg : public CDialog
{
// Construction
public:
	CRecordDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRecordDlg)
	enum { IDD = IDD_RECORD_DIALOG };
	CButton	m_btRecord;
	//}}AFX_DATA
	CButton m_Train;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRecordDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonPlay();
	//}}AFX_MSG
	afx_msg void OnTrain();

	afx_msg LRESULT onWavRecEndNotify(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
	BOOL 	m_bRecording;
public:
	CRecordSound* m_RecordThread;
	CPlaySound* m_PlayThread;
	CPlayMMSound* m_PlayMMSound;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDDLG_H__5260C011_03B2_11D2_A421_FC4B2C882A60__INCLUDED_)
