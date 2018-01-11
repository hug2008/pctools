// Record.h: interface for the CRecord class.
//
////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORD_H__60718690_CBE0_4613_A517_1B1C47A771CA__INCLUDED_)
#define AFX_RECORD_H__60718690_CBE0_4613_A517_1B1C47A771CA__INCLUDED_
#include <mmsystem.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void CALLBACK waveInProc(
  HWAVEIN hwi,       
  UINT uMsg,         
  DWORD dwInstance,  
  DWORD dwParam1,    
  DWORD dwParam2     
);


class CRecord  
{
public:
	void SetDlgPoint(CWnd* p);
	void Stop();
	void Initwavehdr(char *FileName_p);
	void Record(char *FileName_p);
	void Initwaveformat();
	CRecord();
	virtual ~CRecord();

private:
	CWnd* pdlg;
	char* ch2;
	char* ch1;
	WAVEHDR m_wavehdr2;
	WAVEHDR m_wavehdr1;
	HWAVEIN m_hwavein;
	MMRESULT mmresult;
	WAVEFORMATEX m_waveformat;
};

#endif // !defined(AFX_RECORD_H__60718690_CBE0_4613_A517_1B1C47A771CA__INCLUDED_)
