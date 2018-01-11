// StackDialog.cpp: implementation of the CStackDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StackDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//=============================================================================
// Helper structure to maintain information about each page.
//=============================================================================

CStackPageHelper::CStackPageHelper()
{
	m_pDialog = NULL;
	m_bCreated = FALSE;
	m_bActive = FALSE;
}

CStackPageHelper::CStackPageHelper(const CStackPageHelper& obj)
{ 
	*this = obj;
}

CStackPageHelper::~CStackPageHelper()
{
}

CStackPageHelper& CStackPageHelper::operator=(const CStackPageHelper& obj)
{
	m_nDialogID = obj.m_nDialogID;
	m_pDialog	= obj.m_pDialog;
	m_dwHelpID	= obj.m_dwHelpID;
	m_bCreated	= obj.m_bCreated;
	m_bActive	= obj.m_bActive;
	m_dwData1	= obj.m_dwData1;
	m_dwData2	= obj.m_dwData2;
	return *this;
}

// Returns a pointer to child dialog associated with this page
CStackPage *CStackPageHelper::GetPage(void)
{
	return m_pDialog;
}

// Returns TRUE if the child dialog associated with this page is currently
// selected, visible and focused
BOOL CStackPageHelper::IsActive(void)
{
	return m_bActive;
}

// Activate this page and associated child dialog
void CStackPageHelper::Activate(void)
{
	m_pParent->SetPage(m_nDialogID);
}

// Deactivate this page and associated child dialog
void CStackPageHelper::Deactivate(void)
{
	m_pParent->SetPage();
}


//=============================================================================
// class CStackPage
//=============================================================================

BEGIN_MESSAGE_MAP(CStackPage, CDialog)
	//{{AFX_MSG_MAP(CStackPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CStackPage::CStackPage(LPCTSTR lpszTemplateName, CWnd* pParent)
	:CDialog(lpszTemplateName,pParent)
{
}

CStackPage::CStackPage(UINT nIDTemplate, CWnd* pParent)
	:CDialog(nIDTemplate,pParent)
{
}

CStackPage::~CStackPage()
{
}

void CStackPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStackPage)
	//}}AFX_DATA_MAP
}

void CStackPage::OnOK()
{
}

void CStackPage::OnCancel()
{
}

BOOL CStackPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DWORD style = GetStyle();
	ASSERT((style & WS_CHILD) != 0);
	ASSERT((style & WS_BORDER) == 0);
	ASSERT((style & WS_DISABLED) != 0);
	
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// Called when a page is displayed
void CStackPage::OnSetActive(void)
{
}

// Called before a page becomes invisible
void CStackPage::OnKillActive(void)
{
}

// Called after the page is created
void CStackPage::OnCreatePage(void)
{
}

// Called before the page is destroyed
void CStackPage::OnDestroyPage(void)
{
}

// Called when store the data inputed in this page
BOOL CStackPage::OnCloseTest(void)
{
	return TRUE;
}

// Called when store the data inputed in this page
void CStackPage::OnSaveData(void)
{
}

// Called after the data have been imported outside
void CStackPage::OnRefreshPage(void)
{
}

// Called to lock or unlock the setting page
void CStackPage::OnLockPage(BOOL bLock)
{
}

void CStackPage::OnShowPage(int nState)
{
}
//=============================================================================
// class CStackDialog
//=============================================================================

BEGIN_MESSAGE_MAP(CStackDialog, CDialog)
	//{{AFX_MSG_MAP(CStackDialog)
		ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CStackDialog::CStackDialog(UINT nPlaceholder, LPCTSTR lpszTemplateName, CWnd* pParent)
						   :CDialog(lpszTemplateName,pParent)
{
	init(nPlaceholder);
}

CStackDialog::CStackDialog(UINT nPlaceholder, UINT nIDTemplate, CWnd* pParent)
						   :CDialog(nIDTemplate,pParent)
{
	init(nPlaceholder);
}

CStackDialog::~CStackDialog()
{
}

void CStackDialog::init(UINT placeholder)
{
	m_nPlaceholderID = placeholder;
}

void CStackDialog::flush(void)
{
	CStackPageHelper *pPage;
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		pPage = &(*it);
		destroyPageDialog(pPage);
	}
	m_PageList.clear();
}

void CStackDialog::destroyPageDialog(CStackPageHelper *pPage)
{
	if (pPage->m_bCreated) {
		pPage->m_bCreated = FALSE;
		ASSERT(pPage->m_pDialog != NULL);
		pPage->m_pDialog->OnKillActive();
		pPage->m_pDialog->OnDestroyPage();
		pPage->m_pDialog->DestroyWindow();
		delete pPage->m_pDialog;
		pPage->m_pDialog = NULL;
	}
}

CStackPageHelper *CStackDialog::getFirst(void)
{
	CStackPageHelperList::iterator it = m_PageList.begin();
	if (it != m_PageList.end())
		return &(*it);
	return NULL;
}

CStackPageHelper *CStackDialog::getCurrent(void)
{
	CStackPageHelper *pPage = NULL;
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		if (it->m_bActive) {
			pPage = &(*it);
			break;
		}
	}
	return pPage;
}

CStackPageHelper *CStackDialog::getNext(void)
{
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		if (it->m_bActive) {
			it ++;
			if (it != m_PageList.end())
				return &(*it);
			return getFirst();
		}
	}
	return getFirst();
}

void CStackDialog::activatePage(CStackPageHelper *pPage)
{
	ASSERT(pPage != NULL);
	ASSERT(pPage->m_pDialog != NULL);
	ASSERT(pPage->m_bCreated != FALSE);
	CStackPage *pDialog = pPage->m_pDialog;
	// Show and update window
	pDialog->ShowWindow(SW_SHOW);
	pDialog->InvalidateRect(NULL);
	pDialog->UpdateWindow();
	pDialog->OnSetActive();
	pPage->m_bActive = TRUE;
	OnPageChanged(pPage->m_nDialogID, TRUE);
}

void CStackDialog::deactivatePage(CStackPageHelper *pPage)
{
	ASSERT(pPage != NULL);
	ASSERT(pPage->m_pDialog != NULL);
	ASSERT(pPage->m_bCreated != FALSE);
	CStackPage *pDialog = pPage->m_pDialog;
	pDialog->OnKillActive();
	pDialog->ShowWindow(SW_HIDE);
	pPage->m_bActive = FALSE;
	OnPageChanged(pPage->m_nDialogID, FALSE);
}

BOOL CStackDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ModifyStyleEx (0, WS_EX_CONTROLPARENT);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStackDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStackDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CStackDialog::OnDestroy() 
{
	flush();
	CDialog::OnDestroy();
}

// Add a new page (associated child dialog with resource nId) to the 
// stacked dialog. Returns a pointer to the internal helper object that 
// maintains the state of the page
CStackPageHelper *CStackDialog::AddPage(UINT nId, DWORD dwHelpId,
										DWORD dwData1, DWORD dwData2) 
{
	CStackPageHelper page;
	page.m_nDialogID = nId;
	page.m_dwHelpID = dwHelpId;
	page.m_dwData1 = dwData1;
	page.m_dwData2 = dwData2;
	
	CStackPageHelperList::iterator it;
	it = m_PageList.insert(m_PageList.end(), page);
	CStackPageHelper *pPage = &(*it);
	
	pPage->m_pParent = this;
	
	pPage->m_pDialog = CreatePage(pPage->m_nDialogID);
	ASSERT(pPage->m_pDialog != NULL);
	pPage->m_pDialog->Create(pPage->m_nDialogID,this);
	ASSERT(IsWindow(pPage->m_pDialog->m_hWnd) != FALSE);
	
	CRect rect;
	CWnd *pWnd = GetDlgItem(m_nPlaceholderID);
	ASSERT(pWnd != NULL);
	ASSERT(IsWindow(pWnd->m_hWnd) != FALSE);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	pPage->m_pDialog->SetWindowPos(NULL, rect.left, rect.top, 0, 0, 
		SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	pPage->m_pDialog->EnableWindow(TRUE);
	pPage->m_pDialog->OnCreatePage();
	pPage->m_bCreated = TRUE;
	
	return pPage;
}

// Delete page identified with a pointer to its helper object
void CStackDialog::DelPage(CStackPageHelper *pPage)
{
	ASSERT(pPage != NULL);
	destroyPageDialog(pPage);
	
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		if (pPage == &(*it)) {
			m_PageList.erase(it);
			break;
		}
	}
}

// Activate no page (actually deactivates the active page)
void CStackDialog::SetPage(void)
{
	CStackPageHelper *pOldPage = NULL;
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		if ((pOldPage == NULL) && it->m_bActive)
			pOldPage = &(*it);
	}
	if (pOldPage)
		deactivatePage(pOldPage);
}

// Activate the page with the specified child dialog resource
void CStackDialog::SetPage(UINT nId)
{
	CStackPageHelper *pOldPage = NULL, *pNewPage = NULL;
	
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		if ((pNewPage == NULL) && (it->m_nDialogID == nId))
			pNewPage = &(*it);
		if ((pOldPage == NULL) && it->m_bActive)
			pOldPage = &(*it);
	}
	
	if (pOldPage)
		deactivatePage(pOldPage);
	if (pNewPage)
		activatePage(pNewPage);
}

// Set first page as active page
void CStackDialog::SetFirstPage(void)
{
	CStackPageHelper *pPage = getFirst();
	if (pPage)
		SetPage(pPage->m_nDialogID);
}

// Set next page as active page
void CStackDialog::SetNextPage(void)
{
	CStackPageHelper *pPage = getNext();
	if (pPage)
		SetPage(pPage->m_nDialogID);
}

// Get helper object associated with the currently active page
CStackPageHelper *CStackDialog::GetPage(void)
{
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		if (it->m_bActive)
			return &(*it);
	}
	return NULL;
}

// Get helper object associated with the specified child resource id
CStackPageHelper *CStackDialog::GetPage(UINT nId)
{
	CStackPageHelperList::iterator it;
	for (it = m_PageList.begin(); it != m_PageList.end(); it ++) {
		if (it->m_nDialogID == nId)
			return &(*it);
	}
	return NULL;
}