// Play.cpp: implementation of the CPlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myp3.h"
#include "Play.h"

#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int x = 0;
int count = 0;
int n = 0;

short oldmax = 0;
short oldmin = 0;

char* ch[1024];

const unsigned long len = 512;
unsigned long length = 0;

void CALLBACK waveOutProc(
						  HWAVEOUT hwo,
						  UINT uMsg,
						  DWORD dwInstance,
						  DWORD dwParam1,
						  DWORD dwParam2
						  )
{
	if(uMsg==WOM_DONE)
	{

		CWnd* pwnd=(CWnd*)dwInstance;
		CRect rect;
		CDC* pDC;
		pDC=pwnd->GetDC();
		pwnd->GetClientRect(rect);
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(255,25,255));
		CPen ypen;
		ypen.CreatePen(PS_SOLID,1,RGB(255,255,0));
		CPen bpen;
		bpen.CreatePen(PS_SOLID,1,RGB(155,155,155));


		WAVEHDR* phdr=(WAVEHDR*)dwParam1;
		short int *p=(short int*)phdr->lpData;
		unsigned long l=phdr->dwBufferLength/2;
		short min=0;
		short max=2;
		static int y=rect.Height()/2;
		x=x+1;
		for(unsigned long i=0;i<l;i++)
		{
			if(min>p[i]/200)
				min=p[i]/200;
			if(max<p[i]/200)
				max=p[i]/200;
		}

		pDC->SelectObject(&pen);
		pDC->MoveTo(x,0);
		pDC->LineTo(x,rect.Height());
		if(x>0)
		{
			pDC->SelectObject(&bpen);
			pDC->MoveTo(x-1,0);
			pDC->LineTo(x-1,oldmin+y);
			pDC->MoveTo(x-1,rect.Height());
			pDC->LineTo(x-1,oldmax+y-1);
			pDC->SelectObject(&ypen);
			pDC->MoveTo(x-1,oldmin+y);
			pDC->LineTo(x-1,oldmax+y);
		}
		oldmin=min;
		oldmax=max;

		pen.DeleteObject();
		bpen.DeleteObject();
		ypen.DeleteObject();
		ReleaseDC(pwnd->m_hWnd,pDC->m_hDC);
		n--;
	}
	if(uMsg==WOM_CLOSE)
	{
		CWnd* pwnd=(CWnd*)dwInstance;
		CRect rect;
		CDC* pDC;
		pDC=pwnd->GetDC();
		pwnd->GetClientRect(rect);
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(255,25,255));
		CPen ypen;
		ypen.CreatePen(PS_SOLID,1,RGB(255,255,0));
		CPen bpen;
		bpen.CreatePen(PS_SOLID,1,RGB(155,155,155));

		pDC->SelectObject(&bpen);
		pDC->MoveTo(x,0);
		pDC->LineTo(x,oldmin+rect.Height()/2);
		pDC->MoveTo(x,rect.Height());
		pDC->LineTo(x,oldmax+rect.Height()/2-1);
		pDC->SelectObject(&ypen);
		pDC->MoveTo(x,oldmin+rect.Height()/2);
		pDC->LineTo(x,oldmax+rect.Height()/2);
		pen.DeleteObject();
		bpen.DeleteObject();
		ypen.DeleteObject();
		ReleaseDC(pwnd->m_hWnd,pDC->m_hDC);
	}
}

CPlay::CPlay()
{
	m_fp = NULL;
}

CPlay::~CPlay()
{
	if	(NULL != m_fp)
	{
		fclose(m_fp);
	}
}

void CPlay::readfile(char *FileName_p)
{
	unsigned long dw=0;
	unsigned short di=0;
	
	m_fp = fopen(FileName_p,"rb");
	
	if(NULL == m_fp) AfxMessageBox("error");
	fread(&dw,4,1,m_fp);
	if(dw!=1179011410)
	{
		AfxMessageBox("riff error!");
		return ;
	}
	fread(&dw,4,1,m_fp);
	fread(&dw,4,1,m_fp);
	if(dw!=1163280727)
	{
		AfxMessageBox("wave error");
		return ;
	}

	fread(&dw,4,1,m_fp);
	if(dw!=544501094)
	{
		AfxMessageBox("fmt error");
		return ;
	}
	fread(&dw,4,1,m_fp);
	if(dw!=16)
	{
		AfxMessageBox("chklen error");
		return;
	}

	fread(&di,2,1,m_fp);
	if(di!=1)
	{
		AfxMessageBox("fmttag error");
		return ;
	}

	fread(&di,2,1,m_fp);
	if(di!=1)
	{
		AfxMessageBox("channel error");
		return ;
	}

	fread(&dw,4,1,m_fp);
	if(dw!=16000)
	{
		AfxMessageBox("Sample rate error");
		return ;
	}
	fread(&dw,4,1,m_fp);
	if(dw!=32000)
	{
		AfxMessageBox("dwavgbytesrate error");
		return ;
	}
	fread(&di,2,1,m_fp);
	if(di!=2)
	{
		AfxMessageBox("blockAlign error");
		return ;
	}
	fread(&di,2,1,m_fp);
	if(di!=16)
	{
		AfxMessageBox("wbitspersample error");
		return;
	}
	fread(&dw,4,1,m_fp);
	if(dw!=1635017060)
	{
		AfxMessageBox("data error");
		return ;
	}

}

void CPlay::Initformat()
{
	m_format.wFormatTag			=	WAVE_FORMAT_PCM;
	m_format.nSamplesPerSec		=	16000;
	m_format.nChannels			=	1;
	m_format.wBitsPerSample		=	16;
	m_format.nBlockAlign		=	m_format.nChannels*m_format.wBitsPerSample/8;
	m_format.nAvgBytesPerSec	=	m_format.nBlockAlign*m_format.nSamplesPerSec;
	m_format.cbSize				=	0;
}

void CPlay::Initwavehdr()
{
	for(int i=0;i<1024;i++)
		ch[i]=0;
	count=0;
	unsigned long dw;
	fread(&dw,4,1,m_fp);
	if(dw>0)
	{
		for(int i=0;i<1024;i++)
		{
			if(dw==0)break;
			count++;
			if(dw>len)
			{
				ch[i]=new char[len];
				if(ch[i]==0)AfxMessageBox("new failed");
				fread(ch[i],1,len,m_fp);
				dw=dw-len;

				m_wavehdr[i].lpData=ch[i];
				m_wavehdr[i].dwBufferLength=len;
				m_wavehdr[i].dwBytesRecorded=0;
				m_wavehdr[i].dwUser=0;
				m_wavehdr[i].dwFlags=0;
				m_wavehdr[i].dwLoops=0;
				m_wavehdr[i].lpNext=0;
				m_wavehdr[i].reserved=0;
			}
			else
			{
				ch[i]=new char[dw-4];
				fread(ch[i],1,dw-4,m_fp);

				m_wavehdr[i].lpData=ch[i];
				m_wavehdr[i].dwBufferLength=dw-4;
				m_wavehdr[i].dwBytesRecorded=0;
				m_wavehdr[i].dwUser=0;
				m_wavehdr[i].dwFlags=0;
				m_wavehdr[i].dwLoops=0;
				m_wavehdr[i].lpNext=0;
				m_wavehdr[i].reserved=0;
				dw=dw-dw;
			}
		}
	}

	fclose(m_fp);
}

void CPlay::play()
{
	x=0;
	mmret=waveOutOpen(&m_hwaveout,WAVE_MAPPER,&m_format,(DWORD)waveOutProc,(DWORD)pdlg,CALLBACK_FUNCTION);
	if(mmret!=MMSYSERR_NOERROR)
	{
		AfxMessageBox("open failed!");
		return ;
	}

	this->Initwavehdr();
	n=count;

	for(int i=0;i<1024;i++)
	{
		if(ch[i]==0)
			break;
		mmret=waveOutPrepareHeader(m_hwaveout,&m_wavehdr[i],sizeof(WAVEHDR));
		if(mmret!=0)
			AfxMessageBox("prepare failed!");
		mmret=waveOutWrite(m_hwaveout,&m_wavehdr[i],sizeof(WAVEHDR));
		if(mmret!=0)
			AfxMessageBox("write failed!");

	}	
}

void CPlay::pause()
{
	mmret=waveOutPause(m_hwaveout);

}

void CPlay::stop()
{
	int i=0;
	mmret=waveOutReset(m_hwaveout);
	if(mmret!=0)
		AfxMessageBox("00000000");
	for(i=0;i<1024;i++)
	{
		if(ch[i]==0) break;

		mmret=waveOutUnprepareHeader(m_hwaveout,&m_wavehdr[i],sizeof(WAVEHDR));
		if(mmret!=MMSYSERR_NOERROR)
		{
			AfxMessageBox("unprepare failed!");
			return ;
		}

	}
	mmret=waveOutClose(m_hwaveout);
	if(mmret!=MMSYSERR_NOERROR)
	{
		AfxMessageBox("close failed!");
		return ;
	}
	for(i=0;i<1024;i++)
	{
		if(ch[i]==0) break;
		delete[] ch[i];
	}

}

void CPlay::jixu()
{
	waveOutRestart(m_hwaveout);
}

void CPlay::setwindow(CWnd *pwnd)
{
	pdlg = pwnd;
}

int CPlay::GetCount() const
{
	return n;
}
