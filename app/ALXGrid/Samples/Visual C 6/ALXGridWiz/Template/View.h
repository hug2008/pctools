// $$view_hfile$$.h : interface of the $$VIEW_CLASS$$ class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
#define $$FILE_NAME_SYMBOL$$_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

$$IF(CXGridView)
#include "XGridView.h"
$$ENDIF

$$IF(CONTAINER || CONTAINER_SERVER)
class $$CNTRITEM_CLASS$$;
$$ENDIF
$$IF(CALXRecordView || CALXDaoRecordView || CALXOleDBRecordView)
class $$RECSET_CLASS$$;
$$ENDIF

class $$VIEW_CLASS$$ : public $$VIEW_BASE_CLASS$$
{
protected: // create from serialization only
	$$VIEW_CLASS$$();
	DECLARE_DYNCREATE($$VIEW_CLASS$$)
$$IF(CALXRecordView || CALXDaoRecordView || CALXOleDBRecordView)

public:
	$$RECSET_CLASS$$* m_pSet;
$$ENDIF

// Attributes
public:
	$$DOC_CLASS$$* GetDocument();
$$IF(CONTAINER || CONTAINER_SERVER)
$$IF(VERBOSE)
	// m_pSelection holds the selection to the current $$CNTRITEM_CLASS$$.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not $$CNTRITEM_CLASS$$ objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
$$ENDIF //VERBOSE
	$$CNTRITEM_CLASS$$* m_pSelection;
$$ENDIF //CONTAINERS

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL($$VIEW_CLASS$$)
	public:
$$IF(CALXRecordView)
	virtual CRecordset* OnGetRecordset();
$$ENDIF
$$IF(CALXDaoRecordView)
	virtual CDaoRecordset* OnGetRecordset();
$$ENDIF
$$IF(CALXOleDBRecordView)
	virtual CRowset* OnGetRowset();
$$ENDIF
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
$$IF(PRINT)
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
$$IF(ACTIVE_DOC_CONTAINER)
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
$$ENDIF //ACTIVE_DOC_CONTAINER
$$ENDIF //PRINT
$$IF(CONTAINER || CONTAINER_SERVER)
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
$$ENDIF //CONTAINER || CONTAINER_SERVER
	//}}AFX_VIRTUAL

$$IF(CXGridView)
	virtual CELL_DATA GetCellData(int nCol, int nRow);
	virtual BOOL OnSaveCellData(int nCol, int nRow);
$$ENDIF //CXGridView

// Implementation
public:
	virtual ~$$VIEW_CLASS$$();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG($$VIEW_CLASS$$)
$$IF(VERBOSE)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
$$ENDIF
$$IF(CONTAINER || CONTAINER_SERVER)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
$$ENDIF //CONTAINER || CONTAINER_SERVER
$$IF(MINI_SERVER || FULL_SERVER || CONTAINER_SERVER)
	afx_msg void OnCancelEditSrvr();
$$ENDIF //SERVERS
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in $$view_ifile$$.cpp
inline $$DOC_CLASS$$* $$VIEW_CLASS$$::GetDocument()
   { return ($$DOC_CLASS$$*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// $$INSERT_LOCATION_COMMENT$$

#endif // !defined($$FILE_NAME_SYMBOL$$_INCLUDED_)
