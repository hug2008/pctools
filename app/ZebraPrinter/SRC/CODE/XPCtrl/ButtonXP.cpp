///////////////////////////////////////////////////////////////////////////////

//

// ButtonXP.cpp : implementation file

//

///////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"

#include "ButtonXP.h"

#include "XPDraw.h"



#ifdef _DEBUG

#define new DEBUG_NEW

#undef THIS_FILE

static char THIS_FILE[] = __FILE__;

#endif





////////////////////////////////////////////////////////////////////////////////

CButtonXP::CButtonXP ()

{
	
}



////////////////////////////////////////////////////////////////////////////////

CButtonXP::~CButtonXP ()

{
	
}



////////////////////////////////////////////////////////////////////////////////

void CButtonXP::PreSubclassWindow ()

{
	
    ModifyStyle (0, BS_OWNERDRAW);
	
    m_MouseMgr.Init (m_hWnd);
	
}



////////////////////////////////////////////////////////////////////////////////

void CButtonXP::DrawItem (LPDRAWITEMSTRUCT pDis)

{
	
    CBufferDC cDC (pDis->hDC);
	
    CRect rcItem (pDis->rcItem);
	
    HFONT hOldFont = (HFONT)cDC.SelectObject ((HFONT)::GetStockObject (DEFAULT_GUI_FONT));
	
    CPenDC pen (cDC);
	
    CBrushDC brush (cDC);
	
	
	
    if ( (pDis->itemState & ODS_FOCUS) ||
		
		LOWORD(::SendMessage (::GetParent (m_hWnd), DM_GETDEFID, 0, 0)) == GetDlgCtrlID() )
		
    {
		
        rcItem.DeflateRect (1, 1);
		
    }
	
    if ( (pDis->itemState & ODS_DISABLED) ||
		
		(!m_MouseMgr.MouseOver() && !(pDis->itemState & ODS_SELECTED)) )
		
    {
		
        pen.Color (::GetSysColor (COLOR_3DSHADOW));
		
        brush.Color (HLS_TRANSFORM (::GetSysColor (COLOR_3DFACE), -10, 0));
		
    }
	
    else
		
    {
		
        COLORREF crBorder = ::GetSysColor (COLOR_HIGHLIGHT);
		
		
		
        pen.Color (crBorder);
		
		
		
        if ( (pDis->itemState & ODS_SELECTED) )
			
        {
			
            brush.Color (HLS_TRANSFORM (crBorder, +50, -50));
			
            cDC.SetTextColor (RGB(240,240,240));
			
        }
		
        else
			
        {
			
            brush.Color (HLS_TRANSFORM (crBorder, +80, -66));
			
            cDC.SetTextColor (::GetSysColor (COLOR_BTNTEXT));
			
        }
		
    }
	
    if ( (pDis->itemState & ODS_DISABLED) )
		
    {
		
        cDC.SetTextColor (::GetSysColor (COLOR_GRAYTEXT));
		
    }
	
    else if ( (pDis->itemState & ODS_SELECTED) )
		
    {
		
        cDC.SetTextColor (RGB(240,240,240));
		
    }
	
    else if ( m_MouseMgr.MouseOver() )
		
    {
		
        cDC.SetTextColor (0);
		
    }
	
    else
		
    {
		
        cDC.SetTextColor (::GetSysColor (COLOR_BTNTEXT));
		
    }
	
    cDC.Rectangle (rcItem);
	
    cDC.SetBkMode (TRANSPARENT);
	
    cDC.DrawText (CWindowText (this), rcItem, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	
	
	
    if ( (pDis->itemState & ODS_FOCUS) )
		
    {
		
        rcItem.DeflateRect (3, 3);
		
        cDC.DrawFocusRect (rcItem);
		
    }
	
    cDC.SelectObject (hOldFont);
	
}



////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CButtonXP, CButton)

//{{AFX_MSG_MAP(CButtonXP)

ON_WM_MOUSEMOVE()

ON_WM_MOUSEOUT()

//}}AFX_MSG_MAP

END_MESSAGE_MAP()



////////////////////////////////////////////////////////////////////////////////

void CButtonXP::OnMouseMove (UINT, CPoint)

{
	
    Default();
	
    m_MouseMgr.OnMouseMove();
	
}



///////////////////////////////////////////////////////////////////////////////

void CButtonXP::OnMouseOut ()

{
	
    m_MouseMgr.OnMouseOut();
	
}



// Set the tooltip with a string ID

void CButtonXP::SetToolTipText(UINT nStrId, BOOL bActivate)

{
	
	// load string resource
	
	CString sTooltip;
	
	sTooltip.LoadString(nStrId);
	
	
	
	SetToolTipText(sTooltip, bActivate);
	
}



// Set the tooltip with a CString

void CButtonXP::SetToolTipText(CString sText, BOOL bActivate)

{
	
	// We cannot accept NULL pointer
	
	if (sText.IsEmpty()) return;
	
	
	
	// Initialize ToolTip
	
	InitToolTip();
	
	
	
	// If there is no tooltip defined then add it
	
	if (m_ToolTip.GetToolCount() == 0)
		
	{
		
		CRect rectBtn; 
		
		GetClientRect(rectBtn);
		
		m_ToolTip.AddTool(this, sText, rectBtn, 1);
		
	}
	
	
	
	// Set text for tooltip
	
	m_ToolTip.UpdateTipText(sText, this, 1);
	
	m_ToolTip.Activate(bActivate);
	
}



void CButtonXP::InitToolTip()

{
	
	if (m_ToolTip.m_hWnd == NULL)
		
	{
		
		// Create ToolTip control
		
		m_ToolTip.Create(this);
		
		// Create active
		
		m_ToolTip.Activate(TRUE);
		
	}
	
} // End of InitToolTip



BOOL CButtonXP::PreTranslateMessage(MSG* pMsg) 

{
	
	// TODO: Add your specialized code here and/or call the base class
	
	InitToolTip();
	
	m_ToolTip.RelayEvent(pMsg);		
	
	return CButton::PreTranslateMessage(pMsg);
	
}



