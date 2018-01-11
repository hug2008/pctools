// $$frame_ifile$$.cpp : implementation of the $$FRAME_CLASS$$ class
//

#include "stdafx.h"
#include "$$root$$.h"

#include "$$frame_hfile$$.h"
$$IF(PROJTYPE_SDI)
$$IF(PROJTYPE_EXPLORER)
#include "$$treeview_hfile$$.h"
#include "$$view_hfile$$.h"
$$ENDIF
$$ENDIF

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$

$$IF(PROJTYPE_MDI || NODOCVIEW)
IMPLEMENT_DYNAMIC($$FRAME_CLASS$$, $$FRAME_BASE_CLASS$$)
$$ELSE
IMPLEMENT_DYNCREATE($$FRAME_CLASS$$, $$FRAME_BASE_CLASS$$)
$$ENDIF

BEGIN_MESSAGE_MAP($$FRAME_CLASS$$, $$FRAME_BASE_CLASS$$)
	//{{AFX_MSG_MAP($$FRAME_CLASS$$)
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
$$ENDIF
$$IF(TOOLBAR || STATUSBAR)
	ON_WM_CREATE()
$$ENDIF //TOOLBAR || STATUSBAR
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
	ON_WM_SETFOCUS()
$$ENDIF 
$$ENDIF 
	//}}AFX_MSG_MAP
$$IF(HELP)
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, $$FRAME_BASE_CLASS$$::OnHelpFinder)
	ON_COMMAND(ID_HELP, $$FRAME_BASE_CLASS$$::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, $$FRAME_BASE_CLASS$$::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, $$FRAME_BASE_CLASS$$::OnHelpFinder)
$$ENDIF
END_MESSAGE_MAP()
$$IF(STATUSBAR)

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
$$IF(DO_KANA)
	ID_INDICATOR_KANA,
$$ENDIF	// DO_KANA
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
$$ENDIF //STATUSBAR

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$ construction/destruction

$$FRAME_CLASS$$::$$FRAME_CLASS$$()
{
$$IF(VERBOSE)
	// TODO: add member initialization code here
	
$$ENDIF
}

$$FRAME_CLASS$$::~$$FRAME_CLASS$$()
{
}
$$IF(TOOLBAR || STATUSBAR || REBAR)

int $$FRAME_CLASS$$::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if ($$FRAME_BASE_CLASS$$::OnCreate(lpCreateStruct) == -1)
		return -1;
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
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
$$ENDIF 
$$ENDIF //SDI + NODOCVIEW
$$IF(TOOLBAR)
	
$$IF(REBAR)
	if (!m_wndToolBar.CreateEx(this) ||
$$ELSE
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
$$ENDIF //REBAR
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
$$IF(REBAR)
$$IF( CONTAINER || CONTAINER_SERVER)
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
$$ENDIF // ANY CONTAINER
	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
$$IF( CONTAINER || CONTAINER_SERVER)
	m_wndReBar.SetBarStyle(m_wndReBar.GetBarStyle() & ~CBRS_HIDE_INPLACE);
$$ENDIF// ANY CONTAINER
$$ENDIF //REBAR
$$ENDIF //TOOLBAR
$$IF(STATUSBAR)

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
$$ENDIF //STATUSBAR
$$IF(TOOLBAR)

$$IF(REBAR)
$$IF(VERBOSE)
	// TODO: Remove this if you don't want tool tips
$$ENDIF
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
$$IF(MINI_SERVER || FULL_SERVER || CONTAINER_SERVER)
	m_wndDlgBar.SetBarStyle(m_wndDlgBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);
$$ENDIF//MINI_SERVER || FULL_SERVER || CONTAINER_SERVER
$$ELSE
$$IF(VERBOSE)
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
$$ENDIF
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
$$ENDIF //REBAR
$$ENDIF //TOOLBAR	

	return 0;
}
$$ENDIF //TOOLBAR || STATUSBAR
$$IF(PROJTYPE_SDI)
$$IF(SPLITTER_SDI || PROJTYPE_EXPLORER )

BOOL $$FRAME_CLASS$$::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
$$IF(PROJTYPE_EXPLORER)
$$IF(SPLITTER_SDI)
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
	return TRUE;
$$ELSE // Not SPLITTER_SDI
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS($$TREEVIEW_CLASS$$), CSize(100, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS($$VIEW_CLASS$$), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	return TRUE;
$$ENDIF // SPLITTER_SDI  
$$ELSE // Not PROJTYPE_EXPLORER
	return m_wndSplitter.Create(this,
$$IF(VERBOSE)
		2, 2,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
$$ELSE //!VERBOSE
		2, 2,
		CSize(10, 10),
$$ENDIF //VERBOSE
		pContext);
$$ENDIF // PROJTYPE_EXPLORER
}
$$ENDIF //SPLITTER || EXPLORER
$$ENDIF //SDI

BOOL $$FRAME_CLASS$$::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !$$FRAME_BASE_CLASS$$::PreCreateWindow(cs) )
		return FALSE;
$$IF(VERBOSE)
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

$$ENDIF //VERBOSE
$$IF(FRAME_STYLES)
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		$$FRAME_STYLE_FLAGS$$;

$$ENDIF //FRAME_STYLES
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
$$ENDIF 
$$ENDIF //SDI + NODOCVIEW
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$ diagnostics

#ifdef _DEBUG
void $$FRAME_CLASS$$::AssertValid() const
{
	$$FRAME_BASE_CLASS$$::AssertValid();
}

void $$FRAME_CLASS$$::Dump(CDumpContext& dc) const
{
	$$FRAME_BASE_CLASS$$::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// $$FRAME_CLASS$$ message handlers
$$IF(PROJTYPE_SDI)
$$IF(NODOCVIEW)
void $$FRAME_CLASS$$::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL $$FRAME_CLASS$$::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
$$ENDIF 
$$ENDIF //SDI + NODOCVIEW

