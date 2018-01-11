// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
	b_InRect = false;
	b_Flag = false;
	m_ForeColor = RGB(0,0,0);		//������ɫ
	m_BkColor = RGB(203,231,242);	//����ɫ
	m_LockForeColor = GetSysColor(COLOR_GRAYTEXT);	//������ť��������ɫ
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton message handlers

void CMyButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	//���ð�ť����Ϊ�Ի�ʽ
	ModifyStyle( 0, BS_OWNERDRAW );		
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	m_ButRect = lpDrawItemStruct->rcItem;		//��ȡ��ť�ߴ�
	GetWindowText( m_strText );					//��ȡ��ť�ı�
	CPoint m_ptCentre = m_ButRect.CenterPoint();	//��ť���ĵ�
	CSize Extent = pDC->GetTextExtent( m_strText );	//���ı��ߴ�
	//�����ı�����
	m_textPt = CPoint( m_ptCentre.x - Extent.cx/2,m_ptCentre.y - Extent.cy/2 );
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );

	if( !(::GetWindowLong(m_hWnd,GWL_STYLE) & WS_DISABLED) )
	{
		if( !b_Flag )
		{
			NormalButton( pDC );	//��������ť
		}
		else
		{
			PassButton( pDC );		//����꾭��ʱ�İ�ť
		}
	}
	else
	{
		LockButton( pDC );			//�������İ�ť
	}

	pDC->RestoreDC( nSavedDC );
}

//����ƶ���Ϣ����
void CMyButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButton::OnMouseMove(nFlags, point);

	if( !b_InRect || GetCapture()!=this )	//�����밴ť
	{
		b_InRect = true;
		SetCapture();
		b_Flag = true;
		Invalidate();		//�ػ水ť
	}
	else
	{
		CRect rc;
		this->GetClientRect( &rc );
		if ( !rc.PtInRect(point) )			//����뿪��ť
		{
			b_InRect = false;
			ReleaseCapture();
			b_Flag = false;
			Invalidate();		//�ػ水ť
		}
	}
}

//��갴����Ϣ����
void CMyButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	b_Flag = false;
	if (GetFocus()!=this)
	{
		this->SetFocus();
	}
	CButton::OnLButtonDown( nFlags, point );
	Invalidate();		//�ػ水ť
}

//��굯����Ϣ����
void CMyButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	b_Flag = true;
	if (GetFocus()!=this)
	{
		this->SetFocus();
	}
	CButton::OnLButtonUp( nFlags, point );
	Invalidate();		//�ػ水ť
}

////////////////////////////////////////////////////////////////////////
//���ư�ť��

//��������ť
void CMyButton::NormalButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( m_BkColor );
	pDC->SelectObject( &Brush );
	pDC->RoundRect(&m_ButRect,CPoint(10, 10));		//��Բ�Ǿ��ε����

	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(208,235,249));
	pDC->SelectObject( &Pen );
	pDC->RoundRect(m_ButRect.left+2, m_ButRect.top+2,
		m_ButRect.right-2, m_ButRect.bottom-2, 10, 10);	//��Բ�Ǿ��ε��ڱ�

	pDC->SetTextColor( m_ForeColor );		//������
	pDC->SetBkMode( TRANSPARENT );
	pDC->TextOut( m_textPt.x, m_textPt.y, m_strText );
}

//����꾭��ʱ�İ�ť
void CMyButton::PassButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( m_BkColor );
	pDC->SelectObject( &Brush );
	pDC->RoundRect(&m_ButRect,CPoint(10, 10));		//��Բ�Ǿ��ε����

	CPen Pen;
	Pen.CreatePen(PS_SOLID, 2, RGB(255,208,100) );
	pDC->SelectObject( &Pen );
	pDC->RoundRect(m_ButRect.left+2, m_ButRect.top+2,
		m_ButRect.right-2, m_ButRect.bottom-2, 10, 10);	//��Բ�Ǿ��ε��ڱ�

	pDC->SetTextColor( m_ForeColor );		//������
	pDC->SetBkMode( TRANSPARENT );
	pDC->TextOut( m_textPt.x, m_textPt.y, m_strText );
}

//�������İ�ť
void CMyButton::LockButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( m_BkColor );
	pDC->SelectObject( &Brush );
	pDC->RoundRect(&m_ButRect,CPoint(10, 10));		//��Բ�Ǿ��ε����

	pDC->SetBkMode( TRANSPARENT );			//������
	pDC->SetTextColor( m_LockForeColor );
	pDC->TextOut( m_textPt.x, m_textPt.y, m_strText );
}

////////////////////////////////////////////////////////////////////////
//�ӿں���

//�����ı���ɫ
void CMyButton::SetForeColor(COLORREF color)
{
	m_ForeColor = color;
}

//���ñ�����ɫ
void CMyButton::SetBkColor(COLORREF color)
{
	m_BkColor = color;
}
