// WriteSoundFile.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#include "record.h"
#include "WriteSoundFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWriteSoundFile

IMPLEMENT_DYNCREATE(CWriteSoundFile, CWinThread)

CWriteSoundFile::CWriteSoundFile()
{
}

CWriteSoundFile::~CWriteSoundFile()
{
}

BOOL CWriteSoundFile::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CWriteSoundFile::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWriteSoundFile, CWinThread)
	//{{AFX_MSG_MAP(CWriteSoundFile)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_WRITESOUNDFILE_FILENAME, CreateWaveFile)
	ON_THREAD_MESSAGE(WM_WRITESOUNDFILE_WRITEBLOCK, WriteToSoundFile)
	ON_THREAD_MESSAGE(WM_WRITESOUNDFILE_CLOSEFILE, CloseSoundFile)
	ON_THREAD_MESSAGE(WM_RECORDSOUND_ENDTHREAD, OnEndThread)

END_MESSAGE_MAP()

WM_ON_MSG CWriteSoundFile::CreateWaveFile(WPARAM wParam, LPARAM lParam)
{
	PWRITESOUNDFILE pWriteSoundFile = (PWRITESOUNDFILE) lParam;
	int cbWaveFormatEx = sizeof(WAVEFORMATEX) + pWriteSoundFile->waveFormatEx.cbSize;

	m_hFile = ::mmioOpen(pWriteSoundFile->lpszFileName,NULL, MMIO_CREATE|MMIO_WRITE|MMIO_EXCLUSIVE | MMIO_ALLOCBUF);
	if(!m_hFile)
		return WM_ON_ERR;

	ZeroMemory(&m_MMCKInfoParent, sizeof(MMCKINFO));
	m_MMCKInfoParent.fccType = mmioFOURCC('W','A','V','E');

	MMRESULT mmResult =  ::mmioCreateChunk( m_hFile,&m_MMCKInfoParent,
							MMIO_CREATERIFF);
	
	ZeroMemory(&m_MMCKInfoChild, sizeof(MMCKINFO));
	m_MMCKInfoChild.ckid = mmioFOURCC('f','m','t',' ');
	m_MMCKInfoChild.cksize = cbWaveFormatEx;
	mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);
	mmResult = ::mmioWrite(m_hFile, (char*)&pWriteSoundFile->waveFormatEx, cbWaveFormatEx); 
	mmResult = ::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
	m_MMCKInfoChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);

	m_waveSwing.clear();

	return WM_ON_OK;

}
WM_ON_MSG CWriteSoundFile::WriteToSoundFile(WPARAM wParam, LPARAM lParam)
{
	LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;
	int cbLength = lpHdr->dwBufferLength;

	waveSwing	sWaveSwing	= {1, -1};
	DWORD		nIndex		= 0;
	short		*IntData_p	= NULL;
	short		sVal		= 0;

	if(lpHdr)
	{
		char *soundbuffer = (char*) lpHdr->lpData;
		if(m_hFile && soundbuffer)
			::mmioWrite(m_hFile, soundbuffer, cbLength);

		IntData_p = (short *)lpHdr->lpData;
		while (nIndex < (lpHdr->dwBytesRecorded / 2))
		{
			sVal = *IntData_p / 200;
			if (sWaveSwing.crest < sVal)
			{
				sWaveSwing.crest = sVal;
			}

			if (sWaveSwing.trough > sVal)
			{
				sWaveSwing.trough = sVal;
			}

			IntData_p++;
			nIndex++;
		}
		
		m_waveSwing.push_back(sWaveSwing);

		if(soundbuffer)
			delete (BYTE*) soundbuffer;
		if(lpHdr)
			delete lpHdr;
	}

	return WM_ON_OK;
}
WM_ON_MSG CWriteSoundFile::CloseSoundFile(WPARAM wParam, LPARAM lParam)
{
	unsigned int sum_crest  = 0;
	unsigned int sum_trough = 0;

	HWND  parentWnd	 = (HWND)lParam;

	if(m_hFile)
	{
		::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
		::mmioAscend(m_hFile, &m_MMCKInfoParent, 0);
		::mmioClose(m_hFile, 0);
		m_hFile = NULL;
	}
	
	for(std::vector<waveSwing>::iterator it=m_waveSwing.begin(); it!=m_waveSwing.end(); ++it)
	{
		sum_crest  += it->crest  * it->crest;
		sum_trough += it->trough * it->trough;
	}

	sum_crest	/= m_waveSwing.size();
	sum_trough	/= m_waveSwing.size();

	::PostMessage(parentWnd, WM_WRITESOUNDFILE_NOTIFY, (WPARAM)sum_crest, (LPARAM)sum_trough);
	
	return WM_ON_OK;
}
WM_ON_MSG CWriteSoundFile::OnEndThread(WPARAM wParam, LPARAM lParam)
{
	CloseSoundFile(0,0);
	::PostQuitMessage(0);
	return WM_ON_OK;
}
/////////////////////////////////////////////////////////////////////////////
// CWriteSoundFile message handlers
