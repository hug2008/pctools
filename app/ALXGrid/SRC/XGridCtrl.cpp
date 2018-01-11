// XGridCtrl.cpp : implementation of the CXGridCtrl class
//

#include "stdafx.h"
#include "XGridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridView

IMPLEMENT_DYNCREATE(CXGridCtrl, CALXGridCtrl)

BEGIN_MESSAGE_MAP(CXGridCtrl, CALXGridCtrl)
	//{{AFX_MSG_MAP(CXGridCtrl)
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView construction/destruction

CXGridCtrl::CXGridCtrl()
{
	m_bEnableCellEdit = FALSE;

	// �������������� ����� �������� ������
	m_MetricsCells.m_ActiveCellColor.m_crBackgrnd	= ::GetSysColor(COLOR_HIGHLIGHT);
	m_MetricsCells.m_ActiveCellColor.m_crText		= ::GetSysColor(COLOR_HIGHLIGHTTEXT);

	// �������������� ���� � ����� ����� ��������� �������� ������
	SetFocusRect(::GetSysColor(COLOR_HIGHLIGHTTEXT),::GetSysColor(COLOR_HIGHLIGHT),FALSE);
}

CXGridCtrl::~CXGridCtrl()
{
}

// ����������� ��������. ������� ���������� ��������� �� ������� ���������� �������.
CALXCellCtrl* CXGridCtrl::GetCellCtrl(int nCol, int nRow)
{
	// ����� ������� � ����� ��������������
	if(m_bEnableCellEdit)
		return CALXGridCtrl::GetCellCtrl(nCol, nRow);
	else
	{
		if(m_HeaderInfo.m_Cols[nCol].m_CtrlInfo.m_nType == GA_BUTTONCTRL)
			return CALXGridCtrl::GetCellCtrl(nCol, nRow);
	}

	return NULL;
}

void CXGridCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	SaveModifyed(); // ��������� ���������

	m_bEnableCellEdit = FALSE;

	CALXGridCtrl::OnSetFocus(pOldWnd);
}



void CXGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// ������� ��� ������� �����: ������ ����� ��� ����������� �������
	if(point.y <= GetHeadRowHeight() * GetHeadRowCount() || point.x <= GetFrozenColWidth() * GetFrozenColCount())
		CALXGridCtrl::OnLButtonDblClk(nFlags, point);
	else
	{
		m_bEnableCellEdit = TRUE;
		CALXGrid::OnLButtonDown(nFlags, point);
	}
}

void CXGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// ���� ������� ������� �� ����� ��������� � ����������� �� �������
	if( nChar == VK_TAB || nChar == VK_HOME || nChar == VK_END || nChar == VK_UP || nChar == VK_DOWN ||
		nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_PRIOR || nChar == VK_NEXT ||
		nChar == VK_CONTROL || nChar == VK_SHIFT )
	{
		if(m_bEnableCellEdit)
		{
			if(SaveModifyed(FALSE))
				CALXGrid::OnKeyDown(nChar, nRepCnt, nFlags);
		}
		else
			CALXGrid::OnKeyDown(nChar, nRepCnt, nFlags);

		return;
	}
	else
	{
		m_bEnableCellEdit = TRUE;

		CALXCellCtrl* pCellCtrl = m_HeaderInfo.m_Cols[GetActiveCol()].m_CtrlInfo.m_pColCtrl;
		if(pCellCtrl == NULL)
			return;

		// �������� ������ �� ������ � ��. ����������
		pCellCtrl->SetData(GetCellData(GetActiveCol(),GetActiveRow()));
		// ������������ ��. ����������
		ActivateCtrl(m_nActiveCol, m_nActiveRow);

		if(nChar != VK_RETURN)
		{
			// �������� ��� ������� ������� � ������� ����������
			switch(m_HeaderInfo.m_Cols[GetActiveCol()].m_CtrlInfo.m_nType)
			{
			case GA_EDITCTRL:
				{
					CALXEditCtrl* pEditCtrl = (CALXEditCtrl*)pCellCtrl;
					if(pEditCtrl != NULL)
						pEditCtrl->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
					break;
				}
			case GA_BUTTONCTRL:
				{
					CALXButtonCtrl* pButtonCtrl = (CALXButtonCtrl*)pCellCtrl;
					if(pButtonCtrl != NULL)
						pButtonCtrl->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
					break;
				}
			case GA_COMBOBOXCTRL:
				{
					CALXComboBoxCtrl* pComboCtrl = (CALXComboBoxCtrl*)pCellCtrl;
					if(pComboCtrl != NULL)
						pComboCtrl->PostMessage(WM_KEYDOWN,MAKEWPARAM(nChar,0),MAKELPARAM(nRepCnt,nFlags));
					break;
				}
			}

		}
	}
}

/* �������������� ��� ��������� ������������ ���������
   ���������� �. �. ���������� ���� ����� ���� ����������,
   � ����� ��������� ��������� ������������� ���� �����
   ���������, �������� �� ��� ��������� WM_KEYDOWN �����
   ���� �������������� � ������� OnKeyDown.				*/
BOOL CXGridCtrl::PreTranslateMessage(MSG* pMsg) 
{	
	// ���� ������ ������� 'ENTER'
	if(pMsg->message==WM_KEYDOWN && LOWORD(pMsg->wParam) == VK_RETURN)
	{
		if(m_bEnableCellEdit)
		{
			if(SaveModifyed(FALSE))
				return CALXGridCtrl::PreTranslateMessage(pMsg);
		}
		else
			CXGridCtrl::OnKeyDown(LOWORD(pMsg->wParam),LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));				
	}
	else
		return CALXGridCtrl::PreTranslateMessage(pMsg);

	return TRUE;
}
