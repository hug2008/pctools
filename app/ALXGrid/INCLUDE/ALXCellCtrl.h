// ALXCellCtrl.h : header file
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ALX_CELLCTRL_H_INCLUDED_)
#define ALX_CELLCTRL_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALXGridDefinitions.h"

// ������� �������� � ����� ������ �����
BOOL SetClipboardText(LPCTSTR pcsz);
// ������� �������� �� ������ ������ �����
BOOL GetClipboardText(CString& s);

/////////////////////////////////////////////////////////////////////////////
// CALXCellCtrl

class CALXCellCtrl
{
// Construction, Destruction
public:
	CALXCellCtrl();
	~CALXCellCtrl();

	//�����
	CALXFocusRect* m_pFocusRect;

// Attributes
protected:
	CELL_DATA m_Data;

// Operations
public:
	// ���������� ����� ����� ��� ������� ����� ������ ����
	virtual void SendLButtonDown(UINT nFlags, CPoint point);
	// ��������� ������
	virtual void SetFontCtrl( CFont* pFont, BOOL bRedraw = TRUE );
	// ��������������� ���� �����������
	virtual void SetModify(BOOL bModify);
	// ������ ������� ���������� ���������/�����������
	virtual BOOL EnableCtrl(BOOL bEnable = TRUE);
	// ������� ���� �������� ����������
	virtual BOOL CreateCtrl(DWORD dwStyle, CWnd* pParentWnd, UINT nID);
	// ������������ ������� ����������
	virtual BOOL ActivateCtrl(int x, int y, int cx, int cy);
	// �������������� ������� ����������
	virtual BOOL DeactivateCtrl();
	// ���������� ����� ����� ��� �������� ������ � ������
	virtual void SetData(CELL_DATA CtrlData);
	// ���������� ����� ����� ��� ��������� ������ �� ������
	virtual CELL_DATA GetCellData();
	// ���������� ���� �� ������� ���������
	virtual BOOL IsModifyed();
	// ���������� ��������� �� ������� ������
	virtual BOOL OnValidate();
	// ��������� ���� �������� ����������
	virtual BOOL DestroyCtrl();
	// ���������� ������� �� ���� �������� ����������
	virtual BOOL IsActive();

	// ��������������� ��������� �� ��������� ����� "����� �����"
	void SetFocusRect(CALXFocusRect* pFocusRect);

// ������� ��� ������ � ������� ������
	virtual BOOL IsCanCut();
	virtual BOOL IsCanCopy();
	virtual BOOL IsCanPaste();
	virtual BOOL IsCanUndo();
	virtual void Cut();
	virtual void Copy();
	virtual void Paste();
	virtual void Undo();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_CELLCTRL_H_INCLUDED_)
