// Record.cpp: implementation of the CRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myp3.h"
#include "Record.h"
#include <stdio.h>
#include <cmath>

#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const unsigned long len = 512;
static int x = 0;
bool isClose = false;

static FILE *g_pRecFile = NULL;


void CALLBACK waveInProc(HWAVEIN hwi,
						 UINT uMsg,
						 DWORD dwInstance,
						 DWORD dwParam1,
						 DWORD dwParam2)
{
    switch(uMsg)
    {
        case WIM_OPEN:
        {
        }
        break;
        case WIM_DATA:
        {
            WAVEHDR* p = (WAVEHDR*)dwParam1;
			
            short int* data = (short int*)p->lpData;
			
            unsigned long l = p->dwBytesRecorded / 2;
			
            for(unsigned int i=0; i<l; i++)
            {
                fwrite(&data[i], 2, 1, g_pRecFile);
            }

            short max = 1;
            short min = -1;
            CWnd* pwnd = (CWnd*)dwInstance;
            CRect rect;
            pwnd->GetClientRect(&rect);
            CDC* pDC = pwnd->GetDC();
            CPen pen;
            pen.CreatePen(PS_SOLID,1,RGB(255,255,0));
            CDC memDC;
            CBitmap bitmap;
            bitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
            memDC.CreateCompatibleDC(pDC);
            memDC.SelectObject(&bitmap);
            memDC.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);

            static int y=rect.Height()/2;
            //      TRACE("\n%d\n",x);
            //      TRACE("\n%d\n",y);
            if(x > rect.Width())
            {
                x = 0;
            }
            if(x == 0)
            {
                CBrush br(RGB(155,155,155));
                memDC.SelectObject(&br);
                memDC.FillRect(rect,&br);
            }
            x = x + 1;
            for(unsigned int c=0; c<l; c++)
            {
                memDC.SelectObject(&pen);

                if(max<data[c]/200)
                    max=data[c]/200;
				
                if(min>data[c]/200)
                    min=data[c]/200;
            }
            memDC.MoveTo(x, max+y);
            memDC.LineTo(x, min+y);
            pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

            pen.DeleteObject();
            bitmap.DeleteObject();
            ReleaseDC(pwnd->m_hWnd,memDC.m_hDC);
            ReleaseDC(pwnd->m_hWnd,pDC->m_hDC);



            if(isClose==false)
            {
                if(waveInUnprepareHeader(hwi,p,sizeof(WAVEHDR))!=MMSYSERR_NOERROR)
                {
                    AfxMessageBox("11111111");
                }
                if(waveInPrepareHeader(hwi,p,sizeof(WAVEHDR))!=MMSYSERR_NOERROR) AfxMessageBox("prepare failed!");
                if(waveInAddBuffer(hwi,p,sizeof(WAVEHDR))!=MMSYSERR_NOERROR) AfxMessageBox("add failed!");
            }
        }
        break;
        case WIM_CLOSE:
        {
        }
        break;
    }
}

CRecord::CRecord()
{
    ch1 = NULL;
    ch2 = NULL;
    ch1 = new char[len];
    ch2 = new char[len];

	g_pRecFile = NULL;
}

CRecord::~CRecord()
{
    delete[] ch1;
    delete[] ch2;
    ch1 = NULL;
    ch2 = NULL;

	if (NULL != g_pRecFile)
	{
		fclose(g_pRecFile);
		g_pRecFile = NULL;
	}
}

void CRecord::Initwaveformat()
{
    m_waveformat.wFormatTag 		= WAVE_FORMAT_PCM;
    m_waveformat.nSamplesPerSec 	= 16000;
    m_waveformat.nChannels			= 1;
    m_waveformat.wBitsPerSample		= 16;
    m_waveformat.nBlockAlign		= m_waveformat.nChannels*m_waveformat.wBitsPerSample/8;
    m_waveformat.nAvgBytesPerSec	= m_waveformat.nBlockAlign*m_waveformat.nSamplesPerSec;
    m_waveformat.cbSize				= 0;
}

void CRecord::Record(char *FileName_p)
{
    isClose = false;
    x 		= 0;
    mmresult=waveInOpen(&m_hwavein,
                        WAVE_MAPPER,
                        &m_waveformat,
                        (DWORD)&waveInProc,
                        (DWORD)pdlg,
                        CALLBACK_FUNCTION);
    if(mmresult != MMSYSERR_NOERROR)
    {
        AfxMessageBox("open failed!");
        return ;
    }
    this->Initwavehdr(FileName_p);
	
    mmresult = waveInPrepareHeader(m_hwavein,&m_wavehdr1,sizeof(WAVEHDR));
	
    if(mmresult != MMSYSERR_NOERROR)
    {
        AfxMessageBox("prepare buf1 failed!");
        return ;
    }

    mmresult = waveInAddBuffer(m_hwavein,&m_wavehdr1,sizeof(WAVEHDR));
    if(mmresult != MMSYSERR_NOERROR)
    {
        AfxMessageBox("add buf1 failed!");
        return ;
    }

    mmresult = waveInPrepareHeader(m_hwavein,&m_wavehdr2,sizeof(WAVEHDR));
    if(mmresult != MMSYSERR_NOERROR)
    {
        AfxMessageBox("prepare buf2 failed!");
        return ;
    }

    mmresult = waveInAddBuffer(m_hwavein,&m_wavehdr2,sizeof(WAVEHDR));
    if(mmresult!=MMSYSERR_NOERROR)
    {
        AfxMessageBox("add buf2 failed!");
        return ;
    }
	
    waveInStart(m_hwavein);
}

void CRecord::Initwavehdr(char *FileName_p)
{
    if(ch1==0)return ;
    if(ch2==0)return ;
    m_wavehdr1.lpData=ch1;
    m_wavehdr1.dwBufferLength=len;
    m_wavehdr1.dwBytesRecorded=0;
    m_wavehdr1.dwUser=0;
    m_wavehdr1.dwFlags=0;
    m_wavehdr1.dwLoops=0;
    m_wavehdr1.lpNext=0;
    m_wavehdr1.reserved=0;

    m_wavehdr2.lpData=ch2;
    m_wavehdr2.dwBufferLength=len;
    m_wavehdr2.dwBytesRecorded=0;
    m_wavehdr2.dwUser=0;
    m_wavehdr2.dwFlags=0;
    m_wavehdr2.dwLoops=0;
    m_wavehdr2.lpNext=0;
    m_wavehdr2.reserved=0;

    g_pRecFile = fopen(FileName_p,"wb");
    if(NULL == g_pRecFile) AfxMessageBox("open file failed!");

    unsigned long dw;
    unsigned short di;
    dw=1179011410;           //riff
    fwrite(&dw,4,1,g_pRecFile);
    fseek(g_pRecFile,8,SEEK_SET);
    dw=1163280727;         //wave
    fwrite(&dw,4,1,g_pRecFile);
    dw=544501094;          //fmt
    fwrite(&dw,4,1,g_pRecFile);
    dw=16;                   //chklen
    fwrite(&dw,4,1,g_pRecFile);
    di=1;                //fmttag
    fwrite(&di,2,1,g_pRecFile);
    di=1;                 //wchannel
    fwrite(&di,2,1,g_pRecFile);
    dw=16000;             //nSamplePerSec
    fwrite(&dw,4,1,g_pRecFile);
    dw=32000;               //dwAvgBytesRate
    fwrite(&dw,4,1,g_pRecFile);
    di=2;                   //nBlock
    fwrite(&di,2,1,g_pRecFile);
    di=16;                //wBitsPerSample
    fwrite(&di,2,1,g_pRecFile);
    dw=1635017060;                  //data
    fwrite(&dw,4,1,g_pRecFile);
    fseek(g_pRecFile,44,SEEK_SET);
}

void CRecord::Stop()
{
    isClose = true;
    if(waveInReset(m_hwavein) != MMSYSERR_NOERROR) AfxMessageBox("reset failed!");
    if(waveInUnprepareHeader(m_hwavein,&m_wavehdr1,sizeof(WAVEHDR)) != 0) AfxMessageBox("unprepare buf1 failed!");
    if(waveInUnprepareHeader(m_hwavein,&m_wavehdr2,sizeof(WAVEHDR)) != 0) AfxMessageBox("unprepare buf2 failed!");
    if(waveInClose(m_hwavein)!=0) AfxMessageBox("close failed!");
    unsigned long dw=ftell(g_pRecFile)-8;
    fseek(g_pRecFile,4,0);
    fwrite(&dw,4,1,g_pRecFile);
    dw = dw-32;
    fseek(g_pRecFile, 40, 0);
    fwrite(&dw, 4, 1, g_pRecFile);
    fflush(g_pRecFile);
    fclose(g_pRecFile);
	g_pRecFile = NULL;
}

void CRecord::SetDlgPoint(CWnd *p)
{
    this->pdlg = p;
}
