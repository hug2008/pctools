#pragma once
#include "afxwin.h"
#include <editex.hxx>
// CQcsDlg dialog

class CQcsDlg : public CDialog
{
	DECLARE_DYNAMIC(CQcsDlg)

public:
	CQcsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CQcsDlg();
	HICON                m_hIcon;

// Dialog Data
	enum { IDD = IDD_PACKING_SELETE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
#ifndef _VK_RETURN_EXIT_
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#endif//_VK_RETURN_EXIT_
	DECLARE_MESSAGE_MAP()
public:
	CEditEx m_eBoxId;
	CString m_sBoxId;
	CEditEx m_eModel;
	CString m_sModel;
	CEditEx m_eEdition;
	CString m_sEdition;
	CEditEx m_eReq;
	CString m_sReq;
	CEditEx m_eOfm;
	CString m_sOfm;
	CEditEx m_eDate;
	CString m_sDate;
};
