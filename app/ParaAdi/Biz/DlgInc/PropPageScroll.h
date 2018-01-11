#if !defined(PROPPAGE_SCROLL_H)// && !defined(NO_NXT)

#include "stackdialog.h"

#include "comboboxxp.h"

#include "PhoneNumber.h"

#include "BizPhoneSettingDlg.h"

#define   MAX_SCROLLNUM_LEN           (12)
#define   BASE_PAGE_HEAD              (13)

#define   MAX_NXTMENU_TITLE_LEN       (10)
#define   MAX_NXTSCROLL_MATTER_LEN    (19)
#define   MAX_NXTSCROLL_AD_LEN        (10)

// CPropPageScroll dialog

class CPropPageScroll : public CStackPage
{
public:
	CPropPageScroll(CWnd* pParent = NULL);   // standard constructor
	
	BOOL OnCloseTest(void);
	void OnSaveData(void);
	void OnRefreshPage(void);
	void OnLockPage(BOOL bLock);
	
protected:
	CBizPhoneSettingDlg  *m_pParentWnd;
	
	// Dialog Data
	enum { IDD = IDD_PROPPAGE_SCROLL };
	
protected:
	//{{AFX_DATA(CPropPageScroll)
	CString         m_strScroll_IssuanceNum01;
	CString         m_strScroll_IssuanceNum02;
	CString         m_strScroll_IssuanceNum03;
	CString         m_strScroll_IssuanceNum04;
	CString         m_strScroll_IssuanceNum05;
	CString         m_strScroll_IssuanceNum06;
	CString         m_strScroll_IssuanceNum07;
	CString         m_strScroll_HotLine;
	CString         m_strScroll_NxtNum;
	CString         m_strScroll_MobileSec;
	CString         m_strScroll_NxtTitle;
	CString         m_strScroll_ScrollMatter;
	CString         m_strScroll_ScrollAd;
	//}}AFX_DATA
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
};
#endif