// myp3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "myp3.h"
#include "myp3Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyp3Dlg dialog

CPlay play;
CWnd* pwnd;
HANDLE hthread;
bool playstop=false;

DWORD WINAPI ThreadProc(
  LPVOID lpParameter   // thread data
)
{
	while(1)
	{
		Sleep(1500);
		if(playstop==true)
			return 0;
		if(play.GetCount()==0)
		{
			play.stop();
			pwnd->GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(TRUE);
			pwnd->GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(FALSE);
			pwnd->GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
			pwnd->GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(FALSE);
			pwnd->GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
			pwnd->GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
			return 0;
		}
	}
	return 0;
}




CMyp3Dlg::CMyp3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyp3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyp3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyp3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyp3Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyp3Dlg, CDialog)
	//{{AFX_MSG_MAP(CMyp3Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REC_START, OnButtonRecStart)
	ON_BN_CLICKED(IDC_BUTTON_REC_STOP, OnButtonRecStop)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_SUSPEND, OnButtonSuspend)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnButtonContinue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyp3Dlg message handlers

BOOL CMyp3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);

	pwnd=this;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyp3Dlg::OnPaint() 
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
HCURSOR CMyp3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyp3Dlg::OnButtonRecStart() 
{
	// TODO: Add your control notification handler code here
	rec.SetDlgPoint(GetDlgItem(IDC_ST));
	rec.Initwaveformat();
	rec.Record(WAV_FILE_NAME);
	GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
	
}

void CMyp3Dlg::OnButtonRecStop() 
{
	// TODO: Add your control notification handler code here

	rec.Stop();
	GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);

	
}

void CMyp3Dlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	play.setwindow(GetDlgItem(IDC_ST));
	play.readfile(WAV_FILE_NAME);
	play.Initformat();
	play.play();
	playstop=false;
	hthread=CreateThread(0,0,(LPTHREAD_START_ROUTINE)ThreadProc,0,0,0);
	CloseHandle(hthread);

	GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
}


void CMyp3Dlg::OnButtonSuspend() 
{
	// TODO: Add your control notification handler code here
	play.pause();
	GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(TRUE);
}

void CMyp3Dlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	TRACE("\n%d\n",play.GetCount());
	play.stop();
	playstop=true;
	GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
}

void CMyp3Dlg::OnButtonContinue() 
{
	// TODO: Add your control notification handler code here
	play.jixu();
	GetDlgItem(IDC_BUTTON_REC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REC_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SUSPEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CONTINUE)->EnableWindow(FALSE);
}
