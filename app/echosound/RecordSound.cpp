// RecordSound.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include "record.h"
#include "RecordSound.h"
#include "PlaySound.h"
#include "WriteSoundFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecordSound

IMPLEMENT_DYNCREATE(CRecordSound, CWinThread)


#define MAXINPUTBUFFERS 25

CRecordSound::CRecordSound(int iHertz)
{
    m_nInputBuffers = 0;
    m_nMaxInputBuffers = MAXINPUTBUFFERS;
    memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
    m_WaveFormatEx.wFormatTag 		= WAVE_FORMAT_PCM;
    m_WaveFormatEx.nChannels 		= 1;
    m_WaveFormatEx.wBitsPerSample 	= 16;
    m_WaveFormatEx.cbSize 			= 0;
    m_WaveFormatEx.nSamplesPerSec 	= iHertz;
    m_WaveFormatEx.nBlockAlign 		= (m_WaveFormatEx.wBitsPerSample/8) * m_WaveFormatEx.nChannels;
    m_WaveFormatEx.nAvgBytesPerSec	= m_WaveFormatEx.nBlockAlign*m_WaveFormatEx.nSamplesPerSec;//m_WaveFormatEx.nSamplesPerSec * (m_WaveFormatEx.wBitsPerSample/8);

    m_bRecording 					= FALSE;
    m_Player 						= NULL;
    m_Writer 						= NULL;
}






CRecordSound::~CRecordSound()
{
}

BOOL CRecordSound::InitInstance()
{
    // TODO:  perform and per-thread initialization here
    return TRUE;
}

int CRecordSound::ExitInstance()
{
    // TODO:  perform any per-thread cleanup here
    return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRecordSound, CWinThread)
    //{{AFX_MSG_MAP(CRecordSound)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
    ON_THREAD_MESSAGE(WM_RECORDSOUND_STARTRECORDING, OnStartRecording)
    ON_THREAD_MESSAGE(WM_RECORDSOUND_STOPRECORDING, OnStopRecording)
    ON_THREAD_MESSAGE(WM_RECORDSOUND_SOUNDPLAYER, OnPtrSoundPlayer)
    ON_THREAD_MESSAGE(WM_RECORDSOUND_ENDTHREAD, OnEndThread)
    ON_THREAD_MESSAGE(WM_RECORDSOUND_WRITERTHREAD,OnPtrSoundWriter)
    ON_THREAD_MESSAGE(MM_WIM_DATA, OnSoundData)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CRecordSound message handlers
WM_ON_MSG CRecordSound::OnPtrSoundPlayer(WPARAM wParam, LPARAM lParam)
{
    m_Player = (CPlaySound*) lParam;

    return WM_ON_OK;
}
WM_ON_MSG CRecordSound::OnStartRecording(WPARAM wParam, LPARAM lParam)
{
    DWORD mmReturn = 0;

    if(m_bRecording)
        return WM_ON_ERR;

    if(!m_bRecording)
    {
        // open wavein device
        MMRESULT mmReturn = ::waveInOpen(&m_hRecord, WAVE_MAPPER,
                                         &m_WaveFormatEx, ::GetCurrentThreadId(), 0, CALLBACK_THREAD);
        if(mmReturn)
        {
            char errorbuffer[MAX_PATH]  = {0};
            char errorbuffer1[MAX_PATH] = {0};
            waveInGetErrorText(mmReturn,
                               errorbuffer,
                               MAX_PATH);
            sprintf(errorbuffer1,"WAVEIN:%x:%s",mmReturn,errorbuffer);

            AfxMessageBox(errorbuffer1);

        }
		else
        {
            for(int i=0; i < m_nMaxInputBuffers; i++)
            {
                LPWAVEHDR lpHdr = CreateWaveHeader();
                mmReturn = ::waveInPrepareHeader(m_hRecord,lpHdr, sizeof(WAVEHDR));
                mmReturn = ::waveInAddBuffer(m_hRecord, lpHdr, sizeof(WAVEHDR));
                m_nInputBuffers++;
            }
            mmReturn = ::waveInStart(m_hRecord);
            if(mmReturn)
            {
                char errorbuffer[MAX_PATH]  = {0};
                char errorbuffer1[MAX_PATH] = {0};
                waveInGetErrorText(mmReturn,
                                   errorbuffer,
                                   MAX_PATH);
                sprintf(errorbuffer1,"WAVEIN:%x:%s",mmReturn,errorbuffer);

                AfxMessageBox(errorbuffer1);
            }
			else
            {
                m_bRecording = TRUE;
                if(m_Player)
                    m_Player->PostThreadMessage(WM_PLAYSOUND_STARTPLAYING, 0, 0);
                if(m_Writer)
                {
                    PWRITESOUNDFILE pwsf= (PWRITESOUNDFILE) new WRITESOUNDFILE;
                    ZeroMemory(pwsf,sizeof(WRITESOUNDFILE));
                    strcpy((char*)pwsf->lpszFileName,  (char*)lParam);
                    memcpy(&pwsf->waveFormatEx,&m_WaveFormatEx,sizeof(m_WaveFormatEx));
                    m_Writer->PostThreadMessage(WM_WRITESOUNDFILE_FILENAME,0,(LPARAM)pwsf);
                }

                return WM_ON_OK;
            }
        }
    }
    return WM_ON_OK;
}
WM_ON_MSG CRecordSound::OnStopRecording(WPARAM wParam, LPARAM lParam)
{
    MMRESULT mmReturn = 0;
    if(!m_bRecording)
        return WM_ON_ERR;

    if(m_bRecording)
    {
        mmReturn = ::waveInStop(m_hRecord);
        if(!mmReturn)
            mmReturn = ::waveInReset(m_hRecord);
        if(!mmReturn)
            m_bRecording = FALSE;
        Sleep(500);
        if(!mmReturn)
            mmReturn = ::waveInClose(m_hRecord);
        if(m_Player)
            m_Player->PostThreadMessage(WM_PLAYSOUND_STOPPLAYING, 0, lParam);

        if(m_Writer)
            m_Writer->PostThreadMessage(WM_WRITESOUNDFILE_CLOSEFILE, 0, lParam);

        return WM_ON_OK;
    }
    return WM_ON_OK;
}
WM_ON_MSG CRecordSound::OnSoundData(WPARAM wParam, LPARAM lParam)
{
    LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;

    if(lpHdr)
    {
        short int * lpInt = (short int*) lpHdr->lpData;
        DWORD cbRecorded = lpHdr->dwBytesRecorded;
        ::waveInUnprepareHeader(m_hRecord, lpHdr, sizeof(WAVEHDR));

        ProcessSoundData(lpInt, cbRecorded/sizeof(short int));
        if(m_Writer)
        {
            WAVEHDR* pWriteHdr = new WAVEHDR;
            if(!pWriteHdr)
                return WM_ON_ERR;
            memcpy(pWriteHdr,lpHdr,sizeof(WAVEHDR));
            BYTE * pSound = new BYTE[lpHdr->dwBufferLength];
            if(!pSound)
            {
                delete pWriteHdr;
                return WM_ON_ERR;
            }
            memcpy(pSound,lpHdr->lpData,lpHdr->dwBufferLength);
            pWriteHdr->lpData = (char*)pSound;
            m_Writer->PostThreadMessage(WM_WRITESOUNDFILE_WRITEBLOCK,GetCurrentThreadId(),(LPARAM) pWriteHdr);
        }


        if(m_Player)
        {
            m_Player->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK, GetCurrentThreadId(),(LPARAM) lpHdr);
        }
        else
        {
            delete lpInt;
            delete lpHdr;
        }
        char debugbuffer[256];
        sprintf(debugbuffer, "SOUND BUFFER returned: %d\n",cbRecorded);
        TRACE(debugbuffer);
        if(m_bRecording)
        {
            LPWAVEHDR lpHdr = CreateWaveHeader();
            ::waveInPrepareHeader(m_hRecord,lpHdr, sizeof(WAVEHDR));
            ::waveInAddBuffer(m_hRecord, lpHdr, sizeof(WAVEHDR));
            m_nInputBuffers++;
        }
    }


    return WM_ON_OK;
}
WM_ON_MSG CRecordSound::OnEndThread(WPARAM wParam, LPARAM lParam)
{
    if(m_bRecording)
    {
        OnStopRecording(0, 0);
    }
    ::PostQuitMessage(0);
    return WM_ON_OK;
}
WM_ON_MSG CRecordSound::OnPtrSoundWriter(WPARAM wParam, LPARAM lParam)
{
    m_Writer = (CWriteSoundFile*) lParam;
    return WM_ON_OK;
}

LPWAVEHDR CRecordSound::CreateWaveHeader()
{
    LPWAVEHDR lpHdr = new WAVEHDR;
    ZeroMemory(lpHdr, sizeof(WAVEHDR));
    BYTE* lpByte = new BYTE[(m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES)];

    lpHdr->lpData = (char *) lpByte;
    lpHdr->dwBufferLength = (m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES);
    return lpHdr;

}

void CRecordSound::ProcessSoundData(short int* sound, DWORD dwSamples)
{
}
