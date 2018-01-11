// Play.h: interface for the CPlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAY_H__8A651822_D712_4DB7_920B_DD7F806B2F4D__INCLUDED_)
#define AFX_PLAY_H__8A651822_D712_4DB7_920B_DD7F806B2F4D__INCLUDED_
#include <mmsystem.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void CALLBACK waveOutProc(
  HWAVEOUT hwo,
  UINT uMsg,
  DWORD dwInstance,
  DWORD dwParam1,
  DWORD dwParam2
);

class CPlay  
{
public:
	int GetCount()const;
	void setwindow(CWnd* pwnd);
	void jixu();
	void stop();
	void pause();
	void play();
	void Initwavehdr();
	void Initformat();
	void readfile(char* FileName_p);
	CPlay();
	virtual ~CPlay();

private:
	CWnd* pdlg;
	MMRESULT mmret;
	WAVEHDR m_wavehdr[1024];
	WAVEFORMATEX m_format;
	HWAVEOUT m_hwaveout;

	FILE *m_fp;

};

#endif // !defined(AFX_PLAY_H__8A651822_D712_4DB7_920B_DD7F806B2F4D__INCLUDED_)
