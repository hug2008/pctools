///////////////////////////////////////////////////////////////////////////////

//

// XPTools.h

//

///////////////////////////////////////////////////////////////////////////////



#pragma once



///////////////////////////////////////////////////////////////////////////////

// Usefull macros

//

#define KEYDOWN(Key) ((GetKeyState(Key)&0x8000)!=0)

#define ON_WM_MOUSEOUT() \
{ WM_MOUSELEAVE, 0, 0, 0, AfxSig_vv, \
	(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnMouseOut },
///////////////////////////////////////////////////////////////////////////////
// Check if the specified window is child of a docked toolbar
bool ChildOfDockedToolbar (CWnd* pWnd);
	
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	
	class CClientRect : public CRect
		
	{
		
	public:
		
		CClientRect (HWND hWnd)
			
		{
			
			::GetClientRect (hWnd, this);
			
		};
		
		CClientRect (const CWnd* pWnd)
			
		{
			
			::GetClientRect (pWnd->GetSafeHwnd(), this);
			
		};
		
	};
	
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	
	class CWindowRect : public CRect
		
	{
		
	public:
		
		CWindowRect (HWND hWnd)
			
		{
			
			::GetWindowRect (hWnd, this);
			
		};
		
		CWindowRect (const CWnd* pWnd)
			
		{
			
			::GetWindowRect (pWnd->GetSafeHwnd(), this);
			
		};
		
	};
	
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	
	class CWindowText : public CString
		
	{
		
	public:
		
		CWindowText (HWND hWnd)
			
		{
			
			CWnd::FromHandle (hWnd)->GetWindowText (*this);
			
		};
		
		CWindowText (const CWnd* pWnd)
			
		{
			
			pWnd->GetWindowText (*this);
			
		};
		
	};
	
	
	
	
	
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	
#define MMS_PAINT   0x0001
	
#define MMS_NCPAINT 0x0002
	
	
	
	///////////////////
	
	class CMouseMgr		
	{		
	public:		
		CMouseMgr ();	
		
		
		void Init (HWND hWnd, WORD wFlags = MMS_PAINT);

		bool MouseOver () const;
		
		bool OnMouseMove (HWND hTrack = NULL);
		
		bool OnMouseOut (HWND hTrack = NULL);	
		
		
	protected:
		
		HWND m_hWnd;
		
		HWND m_hTrack;
		
		WORD m_wFlags;
		
		bool m_bOver;
		
	};
	
