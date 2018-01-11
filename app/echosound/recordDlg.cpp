// recordDlg.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include "record.h"
#include "recordDlg.h"
#include "RecordSound.h"
#include "WriteSoundFile.h"
#include "PlayMMSound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordDlg dialog

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordDlg)
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_btRecord);
	//}}AFX_DATA_MAP
	
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_TRAIN, OnTrain)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_MESSAGE(WM_WRITESOUNDFILE_NOTIFY, onWavRecEndNotify)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordDlg message handlers

BOOL CRecordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.



	// TODO: Add extra initialization here



	m_bRecording = FALSE;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRecordDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRecordDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRecordDlg::OnTrain()
{
	
}


void CRecordDlg::OnButtonRecord() 
{
	UINT  numDev = 0;

	if(m_bRecording)
	{
		m_btRecord.SetWindowText("&Start Recording");
		m_RecordThread->PostThreadMessage(WM_RECORDSOUND_STOPRECORDING, 0, (LPARAM)this->m_hWnd);

		m_bRecording = FALSE;
	}
	else
	{
		numDev = waveInGetNumDevs();
		if (0 == numDev)
		{
			MessageBox(_T("没有录音设备,请检查是否已经插入耳麦!"), _T("错误"), MB_OK|MB_ICONERROR);
			return;
		}
		m_btRecord.SetWindowText("&Stop Recording");
		m_RecordThread->PostThreadMessage(WM_RECORDSOUND_STARTRECORDING, 0, (LPARAM)WAV_FILE_NAME);

		m_bRecording = TRUE;
	}
}



void CRecordDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	if(m_PlayMMSound)
	{
		m_PlayMMSound->PostThreadMessage(WM_PLAYMMSOUND_PLAYSOUNDPTR, 0,(LPARAM)m_PlayThread);
		m_PlayMMSound->PostThreadMessage(WM_PLAYMMSOUND_PLAYFILE, 0, (LPARAM)WAV_FILE_NAME);
	}
}


LRESULT CRecordDlg::onWavRecEndNotify(WPARAM wParam, LPARAM lParam)
{
	// 录音线程返回波峰(crest)/波谷(trough) 的均值给父窗口
	// 在此可以根据两者的和(crest + trough > 50)大于一个临界值来判断是否达标
	unsigned int lCrest  = (unsigned int)wParam;
	unsigned int lTrough = (unsigned int)lParam;

	char  szMsg[200] = {0};

	sprintf_s(szMsg, 200, "Crest = %ld, lTrough = %ld", lCrest, lTrough);

	MessageBox(szMsg, _T("WAVE"), MB_OK);

	return 0L;
}