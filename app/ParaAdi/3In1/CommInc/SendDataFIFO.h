#if !defined(AFX_SENDDATAFIFO_H__5375C53B_CE3B_4BF2_80FC_BEDA9C5D136A__INCLUDED_)
#define AFX_SENDDATAFIFO_H__5375C53B_CE3B_4BF2_80FC_BEDA9C5D136A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendDataFIFO.h : header file
//

#include "MyHeader.h"
#include "ParaDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CSendDataFIFO thread

class CSendDataFIFO : public CWinThread
{
	DECLARE_DYNCREATE(CSendDataFIFO)
protected:
	CSendDataFIFO();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	SendFIFO * m_pHeadFIFO;
	void OnSendFinishNotify(WPARAM wParam, LPARAM lParam);
	SendFIFO * DeleteRecord(SendFIFO *head);
	SendFIFO * InsertRecord(SendFIFO *head, SendFIFO *record);
	void OnSendNewFIFONotify(WPARAM wParam, LPARAM lParam);
	void OnClearSendDataNotify(WPARAM wParam,LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendDataFIFO)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSendDataFIFO();

	// Generated message map functions
	//{{AFX_MSG(CSendDataFIFO)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	// 用来记录当前发送线程中是否有数据在发送
	BOOL m_bIsSendingData;
	BOOL IsCurrentVaildCommandType(CommandType commandtype);
	BOOL IsForbidRepeatCommandType(CommandType commandtype);
	BOOL IsExistSameAtCommand(CommandType commandtype);
	void ClearHeadFIFO();//add by wfx 2003.05.20 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDDATAFIFO_H__5375C53B_CE3B_4BF2_80FC_BEDA9C5D136A__INCLUDED_)
