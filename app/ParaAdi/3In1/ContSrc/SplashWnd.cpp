// SplashWnd.cpp : implementation file
//
// ©1998-2001 Codejock Software, All Rights Reserved.
// Based on the Visual C++ splash screen component.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SplashWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL        CSplashWnd::m_bShowSplashWnd;
CSplashWnd* CSplashWnd::m_pSplashWnd;

CSplashWnd::CSplashWnd()
{
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(m_pSplashWnd == this);
	m_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	m_bShowSplashWnd = bEnable;
}

BOOL CSplashWnd::ShowSplashScreen(UINT uTimeOut, CString strPic, CWnd* pParentWnd /*= NULL*/, BOOL bBmpORJpg /*= TRUE*/)
{
	ASSERT(uTimeOut && (!strPic.IsEmpty()));
	
	if (!m_bShowSplashWnd || m_pSplashWnd != NULL) {
		return FALSE;
	}

	// Allocate a new splash screen, and create the window.
	m_pSplashWnd = new CSplashWnd;

	m_pSplashWnd->m_bBMPorJPG = bBmpORJpg;
	if (bBmpORJpg)
	{
		// BMP here
		if (!m_pSplashWnd->m_bitmap.LoadBitmap(strPic)) {
			return FALSE;
		}

		BITMAP bm;
		m_pSplashWnd->m_bitmap.GetBitmap(&bm);

		CString strWndClass = AfxRegisterWndClass(0,
			AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		if (!m_pSplashWnd->CreateEx(0, strWndClass, NULL, WS_POPUP | WS_VISIBLE,
			0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL))
		{
			TRACE0("Failed to create splash screen.\n");
			delete m_pSplashWnd;
			return FALSE;
		}
	}
	else
	{
		// JPG here
		if (!m_pSplashWnd->m_picture.Load(strPic)) {
			return FALSE;
		}

		CSize size = m_pSplashWnd->m_picture.GetImageSize();

		CString strWndClass = AfxRegisterWndClass(0,
			AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		if (!m_pSplashWnd->CreateEx(0, strWndClass, NULL, WS_POPUP | WS_VISIBLE,
			0, 0, size.cx, size.cy, pParentWnd->GetSafeHwnd(), NULL))
		{
			TRACE0("Failed to create splash screen.\n");
			delete m_pSplashWnd;
			return FALSE;
		}
	}


	// Center the window.
	m_pSplashWnd->CenterWindow();
	m_pSplashWnd->UpdateWindow();

	// Set a timer to destroy the splash screen.
	m_pSplashWnd->SetTimer(1, uTimeOut, NULL);

	return TRUE;
}

BOOL CSplashWnd::ShowSplashScreen(UINT uTimeOut, UINT uPictureID, CWnd* pParentWnd /*= NULL*/, BOOL bBmpORJpg /*= TRUE*/)
{
	ASSERT(uTimeOut && uPictureID);
	
	if (!m_bShowSplashWnd || m_pSplashWnd != NULL) {
		return FALSE;
	}

	// Allocate a new splash screen, and create the window.
	m_pSplashWnd = new CSplashWnd;

	m_pSplashWnd->m_bBMPorJPG = bBmpORJpg;
	if (bBmpORJpg)
	{
		// BMP here
		if (!m_pSplashWnd->m_bitmap.LoadBitmap(uPictureID)) {
			return FALSE;
		}

		BITMAP bm;
		m_pSplashWnd->m_bitmap.GetBitmap(&bm);

		CString strWndClass = AfxRegisterWndClass(0,
			AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		if (!m_pSplashWnd->CreateEx(0, strWndClass, NULL, WS_POPUP | WS_VISIBLE,
			0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL))
		{
			TRACE0("Failed to create splash screen.\n");
			delete m_pSplashWnd;
			return FALSE;
		}
	}
	else
	{
		// JPG here
		if (!m_pSplashWnd->m_picture.Load(uPictureID)) {
			return FALSE;
		}

		CSize size = m_pSplashWnd->m_picture.GetImageSize();

		CString strWndClass = AfxRegisterWndClass(0,
			AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		if (!m_pSplashWnd->CreateEx(0, strWndClass, NULL, WS_POPUP | WS_VISIBLE,
			0, 0, size.cx, size.cy, pParentWnd->GetSafeHwnd(), NULL))
		{
			TRACE0("Failed to create splash screen.\n");
			delete m_pSplashWnd;
			return FALSE;
		}
	}


	// Center the window.
	m_pSplashWnd->CenterWindow();
	m_pSplashWnd->UpdateWindow();

	// Set a timer to destroy the splash screen.
	m_pSplashWnd->SetTimer(1, uTimeOut, NULL);

	return TRUE;
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (m_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		m_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();
}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC dcImage;
	if (dcImage.CreateCompatibleDC(&dc))
	{
		if (m_bBMPorJPG)
		{
			BITMAP bm;
			m_bitmap.GetBitmap(&bm);

			// Paint the image.
			CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
			dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
			dcImage.SelectObject(pOldBitmap);
		}
		else
		{
			CSize size = m_picture.GetImageSize();

			// Paint the image.
			m_picture.Render(&dc,CRect(0,0,size.cx,size.cy));
		}
	}
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}
