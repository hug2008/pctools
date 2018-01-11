#if !defined(ALX_SPLITTERWND_H_INCLUDED_)
#define ALX_SPLITTERWND_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ALXSplitterWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CALXSplitterWnd

class CALXSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CALXSplitterWnd)

friend class CALXGridView;

// Construction, Destruction
public:
	CALXSplitterWnd();
	virtual ~CALXSplitterWnd();

	virtual BOOL CreateView(int row, int col, CRuntimeClass* pViewClass,
			SIZE sizeInit, CCreateContext* pContext);

	// for customizing DYNAMIC_SPLIT behavior
	virtual void DeleteView(int row, int col);
	virtual BOOL SplitRow(int cyBefore);
	virtual BOOL SplitColumn(int cxBefore);
	virtual void DeleteRow(int rowDelete);
	virtual void DeleteColumn(int colDelete);

protected:
	BOOL OnMouseWheel(CView* pView, UINT fFlags, short zDelta, CPoint point);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:

protected:
	//{{AFX_MSG(CALXSplitterWnd)
	afx_msg LRESULT OnActGridColRowChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSelGridColRowRange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridColWidthChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaveChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGridRowCountChanged(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ALX_SPLITTERWND_H_INCLUDED_)
