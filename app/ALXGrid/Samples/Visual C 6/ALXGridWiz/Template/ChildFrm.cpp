// $$child_frame_ifile$$.cpp : implementation of the $$CHILD_FRAME_CLASS$$ class
//

#include "stdafx.h"
#include "$$root$$.h"

#include "$$child_frame_hfile$$.h"
$$IF(PROJTYPE_EXPLORER)
#include "$$treeview_hfile$$.h"
#include "$$view_hfile$$.h"
$$ENDIF

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// $$CHILD_FRAME_CLASS$$

IMPLEMENT_DYNCREATE($$CHILD_FRAME_CLASS$$, $$CHILD_FRAME_BASE_CLASS$$)

BEGIN_MESSAGE_MAP($$CHILD_FRAME_CLASS$$, $$CHILD_FRAME_BASE_CLASS$$)
	//{{AFX_MSG_MAP($$CHILD_FRAME_CLASS$$)
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
$$ENDIF
$$IF(NODOCVIEW)
		ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
		ON_WM_SETFOCUS()
		ON_WM_CREATE()
$$ENDIF
$$IF( REBAR )
$$IF(MINI_SERVER || FULL_SERVER || CONTAINER_SERVER)
		ON_WM_CREATE()
$$ENDIF // (ANY SERVER)
$$ENDIF // REBAR
	//}}AFX_MSG_MAP
$$IF(PROJTYPE_EXPLORER)
$$IF(CListView)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
$$ENDIF
$$ENDIF
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// $$CHILD_FRAME_CLASS$$ construction/destruction

$$CHILD_FRAME_CLASS$$::$$CHILD_FRAME_CLASS$$()
{
$$IF(VERBOSE)
	// TODO: add member initialization code here
	
$$ENDIF
}

$$CHILD_FRAME_CLASS$$::~$$CHILD_FRAME_CLASS$$()
{
}
$$IF(SPLITTER_MDI || PROJTYPE_EXPLORER)

BOOL $$CHILD_FRAME_CLASS$$::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
$$IF(PROJTYPE_EXPLORER)
$$IF(SPLITTER_MDI)
	// create splitter window
	if (!m_wndSSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSSplitter.CreateView(0, 0, RUNTIME_CLASS($$TREEVIEW_CLASS$$), CSize(100, 100), pContext))
	{
		m_wndSSplitter.DestroyWindow();
		return FALSE;
	}

	pContext->m_pNewViewClass = RUNTIME_CLASS($$VIEW_CLASS$$);

	if(!m_wndSplitter.Create(&m_wndSSplitter, 2, 2, CSize(10,10), pContext, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT,m_wndSSplitter.IdFromRowCol(0,1)))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
$$ELSE
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS($$TREEVIEW_CLASS$$), CSize(100, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS($$VIEW_CLASS$$), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
$$ENDIF
	return TRUE;
$$ELSE //!PROJTYPE_EXPLORER
	return m_wndSplitter.Create( this,
$$IF(VERBOSE)
		2, 2,                 // TODO: adjust the number of rows, columns
		CSize( 10, 10 ),      // TODO: adjust the minimum pane size
$$ELSE //!VERBOSE
		2, 2,
		CSize( 10, 10 ),
$$ENDIF //VERBOSE
		pContext );
$$ENDIF //PROJTYPE_EXPLORER
}
$$ENDIF //SPLITTER || EXPLORER

BOOL $$CHILD_FRAME_CLASS$$::PreCreateWindow(CREATESTRUCT& cs)
{
$$IF(VERBOSE)
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

$$ENDIF //VERBOSE
	if( !$$CHILD_FRAME_BASE_CLASS$$::PreCreateWindow(cs) )
		return FALSE;

$$IF(CHILD_FRAME_STYLES)
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE$$CHILD_FRAME_STYLE_FLAGS$$;

$$ENDIF //CHILD_FRAME_STYLES
$$IF(NODOCVIEW)
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
$$ENDIF
	return TRUE;
}

$$IF(CHILD_FRAME_MAXIMIZED || CHILD_FRAME_MINIMIZED)
void $$CHILD_FRAME_CLASS$$::ActivateFrame(int nCmdShow)
{
$$IF(VERBOSE)
	// TODO: Modify this function to change how the frame is activated.

$$ENDIF //VERBOSE
$$IF(CHILD_FRAME_MAXIMIZED)
	nCmdShow = SW_SHOWMAXIMIZED;
$$ENDIF //CHILD_FRAME_MAXIMIZED
$$IF(CHILD_FRAME_MINIMIZED)
	nCmdShow = SW_SHOWMINIMIZED;
$$ENDIF //CHILD_FRAME_MINIMIZED
	$$CHILD_FRAME_BASE_CLASS$$::ActivateFrame(nCmdShow);
}
$$ENDIF //CHILD_FRAME_MAXIMIZED || CHILD_FRAME_MINIMIZED


/////////////////////////////////////////////////////////////////////////////
// $$CHILD_FRAME_CLASS$$ diagnostics

#ifdef _DEBUG
void $$CHILD_FRAME_CLASS$$::AssertValid() const
{
	$$CHILD_FRAME_BASE_CLASS$$::AssertValid();
}

void $$CHILD_FRAME_CLASS$$::Dump(CDumpContext& dc) const
{
	$$CHILD_FRAME_BASE_CLASS$$::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// $$CHILD_FRAME_CLASS$$ message handlers

$$IF(NODOCVIEW)
void $$CHILD_FRAME_CLASS$$::OnFileClose() 
{
$$IF(VERBOSE)
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.
$$ENDIF

	SendMessage(WM_CLOSE);
}

int $$CHILD_FRAME_CLASS$$::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
$$IF(CXGridCtrl)
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL| WS_TABSTOP, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
$$ELSE
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
$$ENDIF
	return 0;
}

void $$CHILD_FRAME_CLASS$$::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL $$CHILD_FRAME_CLASS$$::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// otherwise, do default handling
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

$$ENDIF
