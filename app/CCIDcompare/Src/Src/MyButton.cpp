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
	m_ForeColor = RGB(0,0,0);		//文字颜色
	m_BkColor = RGB(203,231,242);	//背景色
	m_LockForeColor = GetSysColor(COLOR_GRAYTEXT);	//锁定按钮的文字颜色
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
	//设置按钮属性为自画式
	ModifyStyle( 0, BS_OWNERDRAW );		
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	m_ButRect = lpDrawItemStruct->rcItem;		//获取按钮尺寸
	GetWindowText( m_strText );					//获取按钮文本
	CPoint m_ptCentre = m_ButRect.CenterPoint();	//求按钮中心点
	CSize Extent = pDC->GetTextExtent( m_strText );	//求文本尺寸
	//设置文本坐标
	m_textPt = CPoint( m_ptCentre.x - Extent.cx/2,m_ptCentre.y - Extent.cy/2 );
	int nSavedDC = pDC->SaveDC();
	VERIFY( pDC );

	if( !(::GetWindowLong(m_hWnd,GWL_STYLE) & WS_DISABLED) )
	{
		if( !b_Flag )
		{
			NormalButton( pDC );	//画正常按钮
		}
		else
		{
			PassButton( pDC );		//画鼠标经过时的按钮
		}
	}
	else
	{
		LockButton( pDC );			//画锁定的按钮
	}

	pDC->RestoreDC( nSavedDC );
}

//鼠标移动消息函数
void CMyButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButton::OnMouseMove(nFlags, point);

	if( !b_InRect || GetCapture()!=this )	//鼠标进入按钮
	{
		b_InRect = true;
		SetCapture();
		b_Flag = true;
		Invalidate();		//重绘按钮
	}
	else
	{
		CRect rc;
		this->GetClientRect( &rc );
		if ( !rc.PtInRect(point) )			//鼠标离开按钮
		{
			b_InRect = false;
			ReleaseCapture();
			b_Flag = false;
			Invalidate();		//重绘按钮
		}
	}
}

//鼠标按下消息函数
void CMyButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	b_Flag = false;
	if (GetFocus()!=this)
	{
		this->SetFocus();
	}
	CButton::OnLButtonDown( nFlags, point );
	Invalidate();		//重绘按钮
}

//鼠标弹起消息函数
void CMyButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	b_Flag = true;
	if (GetFocus()!=this)
	{
		this->SetFocus();
	}
	CButton::OnLButtonUp( nFlags, point );
	Invalidate();		//重绘按钮
}

////////////////////////////////////////////////////////////////////////
//绘制按钮：

//画正常按钮
void CMyButton::NormalButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( m_BkColor );
	pDC->SelectObject( &Brush );
	pDC->RoundRect(&m_ButRect,CPoint(10, 10));		//画圆角矩形的外框

	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(208,235,249));
	pDC->SelectObject( &Pen );
	pDC->RoundRect(m_ButRect.left+2, m_ButRect.top+2,
		m_ButRect.right-2, m_ButRect.bottom-2, 10, 10);	//画圆角矩形的内边

	pDC->SetTextColor( m_ForeColor );		//画文字
	pDC->SetBkMode( TRANSPARENT );
	pDC->TextOut( m_textPt.x, m_textPt.y, m_strText );
}

//画鼠标经过时的按钮
void CMyButton::PassButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( m_BkColor );
	pDC->SelectObject( &Brush );
	pDC->RoundRect(&m_ButRect,CPoint(10, 10));		//画圆角矩形的外框

	CPen Pen;
	Pen.CreatePen(PS_SOLID, 2, RGB(255,208,100) );
	pDC->SelectObject( &Pen );
	pDC->RoundRect(m_ButRect.left+2, m_ButRect.top+2,
		m_ButRect.right-2, m_ButRect.bottom-2, 10, 10);	//画圆角矩形的内边

	pDC->SetTextColor( m_ForeColor );		//画文字
	pDC->SetBkMode( TRANSPARENT );
	pDC->TextOut( m_textPt.x, m_textPt.y, m_strText );
}

//画锁定的按钮
void CMyButton::LockButton(CDC *pDC)
{
	CBrush Brush;
	Brush.CreateSolidBrush( m_BkColor );
	pDC->SelectObject( &Brush );
	pDC->RoundRect(&m_ButRect,CPoint(10, 10));		//画圆角矩形的外框

	pDC->SetBkMode( TRANSPARENT );			//画文字
	pDC->SetTextColor( m_LockForeColor );
	pDC->TextOut( m_textPt.x, m_textPt.y, m_strText );
}

////////////////////////////////////////////////////////////////////////
//接口函数

//设置文本颜色
void CMyButton::SetForeColor(COLORREF color)
{
	m_ForeColor = color;
}

//设置背景颜色
void CMyButton::SetBkColor(COLORREF color)
{
	m_BkColor = color;
}
