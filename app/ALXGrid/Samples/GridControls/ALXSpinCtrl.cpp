// ALXSpinCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
/*
Автор:              Долгачев А.В.
Координаты:         alxsoft@gazinter.net
*/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ALXSpinCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALXSpinCtrl
CALXSpinCtrl::CALXSpinCtrl()
{
	m_bModifyed = FALSE;
}

CALXSpinCtrl::~CALXSpinCtrl()
{
}


BEGIN_MESSAGE_MAP(CALXSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CALXSpinCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltaPos)
	ON_COMMAND(EN_CHANGE, OnChangeEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CALXSpinCtrl::ActivateCtrl(int x, int y, int cx, int cy)
{
	int cx1 = max(0,(cy-5)); 
	int x1 = max(x+2, x+2 + cx-3 - cx1);
	MoveWindow(x1,y+2,cx1,cy-3);
	m_EditCtrl.MoveWindow(x+2,y+2,x1-(x+2),cy-3);

	EnableWindow(TRUE);
	m_EditCtrl.EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	m_EditCtrl.ShowWindow(SW_SHOW);
	CSpinButtonCtrl::SetFocus();
	m_EditCtrl.SetFocus();
	m_EditCtrl.SetSel(0,-1);

	m_EditCtrl.m_hWndSpin = m_hWnd;
	return TRUE;
}

BOOL CALXSpinCtrl::DeactivateCtrl()
{
	m_EditCtrl.EnableWindow(FALSE);
	EnableWindow(FALSE);
	m_EditCtrl.ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	m_EditCtrl.MoveWindow(0,0,0,0);
	MoveWindow(0,0,0,0);
	return TRUE;
}

void CALXSpinCtrl::SetData(CELL_DATA CtrlData)
{
	int pos = atoi(CtrlData.m_strText);
	SetPos(pos);
	CtrlData.m_strText.Format("%d",pos);
	m_EditCtrl.SetData(CtrlData);
	m_EditCtrl.SetSel(0,-1);
	m_bModifyed = FALSE;
	return;
}

CELL_DATA CALXSpinCtrl::GetCellData()
{
	return m_EditCtrl.GetCellData();
}

BOOL CALXSpinCtrl::IsModifyed()
{
	return m_bModifyed;
}

BOOL CALXSpinCtrl::IsActive()
{
	return IsWindowEnabled();
}

BOOL CALXSpinCtrl::DestroyCtrl()
{
	BOOL bResult = m_EditCtrl.DestroyWindow();
	bResult = bResult & DestroyWindow();
	return bResult;
}

BOOL CALXSpinCtrl::CreateCtrl(DWORD dwStyle, CWnd *pParentWnd, UINT nID)
{
	BOOL bResult;
	CRect rectCtrl(0,0,0,0);

	bResult = m_EditCtrl.Create(WS_CHILD | ES_RIGHT,rectCtrl,pParentWnd,nID);
	if(bResult)
		bResult = Create(dwStyle,rectCtrl,pParentWnd,nID);
	return bResult;
}

void CALXSpinCtrl::SetFontCtrl(CFont *pFont, BOOL bRedraw)
{
	m_EditCtrl.SetFont(pFont,bRedraw);
	SetFont(pFont,bRedraw);
}

BOOL CALXSpinCtrl::EnableCtrl(BOOL bEnable)
{
	BOOL bResult = m_EditCtrl.EnableWindow(bEnable);
	bResult = bResult & EnableWindow(bEnable);
	return bResult;
}

void CALXSpinCtrl::SetModify(BOOL bModify)
{
	m_bModifyed = bModify;
}

void CALXSpinCtrl::SendLButtonDown(CPoint point)
{
//	SendMessage(WM_LBUTTONDOWN,MK_LBUTTON,MAKELPARAM(point.x,point.y));
}

/////////////////////////////////////////////////////////////////////////////
// CALXSpinCtrl message handlers

void CALXSpinCtrl::OnDeltaPos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	pNMUpDown->iPos;
	CString strData;
	int low, upp;
	GetRange( low, upp);

	strData.Format("%d", min(upp,max(low,pNMUpDown->iPos + pNMUpDown->iDelta)));
	m_EditCtrl.SetWindowText(strData);
	m_bModifyed = TRUE;
	*pResult = 0;
}

void CALXSpinCtrl::OnChangeEdit() 
{
	int low, upp, pos;
	CString strData;
	GetRange( low, upp);

	m_EditCtrl.GetWindowText(strData);
	pos = atoi(strData);

	if(pos > upp)
	{
		pos = upp;
		strData.Format("%d", pos);
		m_EditCtrl.SetWindowText(strData);
	}
	else if(pos < low)
	{
		pos = low;
		strData.Format("%d", pos);
		m_EditCtrl.SetWindowText(strData);
	}

	if(pos != GetPos())
	{
		SetPos(pos);
		m_bModifyed = TRUE;
	}
}

void CALXSpinCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar >= VK_0 && nChar <= VK_9)
		m_EditCtrl.PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
	else
		CSpinButtonCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
