#if !defined(AFX_MYBUTTON_H__F56003FF_3CB5_4AFB_A601_A4097DA61B3E__INCLUDED_)
#define AFX_MYBUTTON_H__F56003FF_3CB5_4AFB_A601_A4097DA61B3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

class CMyButton : public CButton
{
// Construction
public:
	CMyButton();

private:
	BOOL    b_Flag;			//��ť״̬��false-������true-��ǰ��
	BOOL    b_InRect;		//�������־
	CString m_strText;		//��ť����
	COLORREF m_ForeColor;	//�ı���ɫ
	COLORREF m_BkColor;		//����ɫ
	COLORREF m_LockForeColor;	//������ť��������ɫ
	CRect   m_ButRect;		//��ť�ߴ�
	CPoint  m_textPt;		//�������꣨���Ͻǣ�

	void   NormalButton(CDC *pDC);		//�������İ�ť
	void   PassButton(CDC *pDC);		//����꾭��ʱ�İ�ť
	void   LockButton(CDC *pDC);		//�������İ�ť

// Attributes
public:
	void  SetForeColor(COLORREF color);	//�����ı���ɫ
	void  SetBkColor(COLORREF color);	//���ñ�����ɫ

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__F56003FF_3CB5_4AFB_A601_A4097DA61B3E__INCLUDED_)
