// E:\CvsRoot\ParaDownload_Head\SRC\VfManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BizPhoneManager.h"
#include "VfManagerDlg.h"
#include "UserVerify.h"
#include <stdio.h>
#include <stdlib.h>
// CVfManagerDlg dialog

IMPLEMENT_DYNAMIC(CVfManagerDlg, CDialog)
CVfManagerDlg::CVfManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVfManagerDlg::IDD, pParent)
	, m_sUser(_T(""))
	, m_sPassw(_T(""))
{
}

CVfManagerDlg::~CVfManagerDlg()
{
}

void CVfManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VYM_LIST, m_list);
	DDX_Control(pDX, IDC_EDIT_VFM_USER, m_eUser);
	DDX_Text(pDX, IDC_EDIT_VFM_USER, m_sUser);
	DDX_Control(pDX, IDC_EDIT_VFM_PASSW, m_ePassw);
	DDX_Text(pDX, IDC_EDIT_VFM_PASSW, m_sPassw);
}


BEGIN_MESSAGE_MAP(CVfManagerDlg, CDialog)
//	ON_NOTIFY(HDN_ITEMCLICK, 0, OnHdnItemclickVymList)
	ON_BN_CLICKED(IDC_VFM_ADD, OnBnClickedVfmAdd)
	ON_BN_CLICKED(IDC_VFM_DEL, OnBnClickedVfmDel)
	ON_BN_CLICKED(IDC_VFM_LOAD, OnBnClickedVfmLoad)
	ON_NOTIFY(NM_CLICK, IDC_VYM_LIST, OnNMClickVymList)
END_MESSAGE_MAP()


// CVfManagerDlg message handlers
BOOL CVfManagerDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	// call EnableToolTips to enable tooltip display
	m_list.EnableToolTips(TRUE);

	InitList();

	char name[256];
	GetModuleFileName(NULL, name, 256);	
	CString path = name;
	path = path.Mid(0, path.ReverseFind('\\')+1);
	m_sVfFile = path + VERIFY_FILE;

	SetWindowText(_T("User Verity Manager"));

	loadVyFile(m_sVfFile);
	RefreshList();

	return TRUE;
}

void  CVfManagerDlg::loadVyFile(CString szFile)
{
	char szUserArray[2048][NAME_SIZE]   = {},
		 szPasswArray[2048][PASSW_SIZE] = {};

	CString newString                   = _T("");

	unsigned long  nSize                = 0,
		           nIndex               = 0;

	m_userArray.RemoveAll();
	m_passwArray.RemoveAll();

	nSize = loadAllUser(szUserArray, szPasswArray, szFile);

	for (nIndex=0; nIndex<nSize; nIndex++)
	{
		newString = szUserArray[nIndex];
		m_userArray.Add(newString);
		newString = szPasswArray[nIndex];
		m_passwArray.Add(newString);
	}
}

void  CVfManagerDlg::RefreshList(void)
{
	int Index = 0,
		Size  = 0;
	CString szItem = _T("");

	m_list.DeleteAllItems();
	Size = m_userArray.GetSize();

	for (Index=0; Index<Size; Index++){
		szItem.Format(_T(" %02d"), Index+1);
		m_list.InsertItem(Index, szItem);
		m_list.SetItemText(Index, 0x01, m_userArray.GetAt(Index));
		m_list.SetItemText(Index, 0x02, m_passwArray.GetAt(Index));
	}

	m_sUser  = _T("");
	m_eUser.SetWindowText(m_sUser);

	m_sPassw = _T("");
	m_ePassw.SetWindowText(m_sPassw);
}

void  CVfManagerDlg::InitList(void)
{
	CRect rect;

	TCHAR *lpszHeaders[] = {
		    _T("Index"),
		    _T("User"),
			_T("PassW")
	};

	m_list.GetWindowRect(&rect);
	int w = rect.Width();
	w = w - ::GetSystemMetrics(SM_CXVSCROLL) - 2;	// width of vertical scroll bar
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	lvcolumn.cx = 45;
	lvcolumn.pszText  = lpszHeaders[0];
	lvcolumn.iSubItem = 0;
	m_list.InsertColumn(0, &lvcolumn);

	lvcolumn.cx = 80;
	lvcolumn.pszText  = lpszHeaders[1];
	lvcolumn.iSubItem = 1;
	m_list.InsertColumn(1, &lvcolumn);

	lvcolumn.cx = w - (lvcolumn.cx + 45);
	lvcolumn.pszText  = lpszHeaders[2];
	lvcolumn.iSubItem = 2;
	m_list.InsertColumn(2, &lvcolumn);
}
//void CVfManagerDlg::OnHdnItemclickVymList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	int nItem = phdr->iItem;
//
//	m_sUser  = m_list.GetItemText(nItem, 0x01);
//	m_eUser.SetWindowText(m_sUser);
//
//	m_sPassw = m_list.GetItemText(nItem, 0x02);
//	m_ePassw.SetWindowText(m_sPassw);
//
//	*pResult = 0;
//}

void CVfManagerDlg::OnBnClickedVfmAdd()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);

	char szUser[NAME_SIZE] = "";
	char szPassW[PASSW_SIZE] = "";
	CString szErr = _T("");
	int nItem = 0;

	if ((m_sUser.GetLength() > NAME_SIZE) ||
		(m_sUser.GetLength() < 2))
	{
		MessageBox(_T("user's name length between 2 and 20!"), _T("Err"), MB_OK|MB_ICONSTOP);
		m_eUser.SetFocus();
		m_eUser.SetSel(0, -1);
		return;
	}

	if ((m_sPassw.GetLength() > PASSW_SIZE) ||
		(m_sPassw.GetLength() == 0))
	{
		MessageBox(_T("password length between 1 and 10!"), _T("Err"), MB_OK|MB_ICONSTOP);
		m_ePassw.SetFocus();
		m_ePassw.SetSel(0, -1);
		return;
	}

	strcpy(szUser,  m_sUser);
	strcpy(szPassW, m_sPassw);

	if (addVerifyNode(szUser, szPassW, m_sVfFile) < 1){
		szErr.Format("User = %s PassW = %s", szUser, szPassW);
		MessageBox(szErr, _T("Add Aborted"), MB_OK|MB_ICONSTOP);
		return;
	}

	nItem = m_list.GetItemCount();
	szErr.Format(" %02d", nItem+1);
	m_list.InsertItem(nItem, szErr);
	m_list.SetItemText(nItem, 0x01, szUser);
	m_list.SetItemText(nItem, 0x02, szPassW);
	szErr.Format("User = %s PassW = %s", szUser, szPassW);
	MessageBox(szErr, _T("Add OK"), MB_OK);
}

void CVfManagerDlg::OnBnClickedVfmDel()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	char szUser[NAME_SIZE] = "";
	char szPassW[PASSW_SIZE] = "";
	CString szErr = _T("");

	if ((m_sUser.GetLength() > NAME_SIZE) ||
		(m_sUser.GetLength() < 2))
	{
		MessageBox(_T("user's name length between 2 and 20!"), _T("Err"), MB_OK|MB_ICONSTOP);
		m_eUser.SetFocus();
		m_eUser.SetSel(0, -1);
		return;
	}

	if ((m_sPassw.GetLength() > PASSW_SIZE) ||
		(m_sPassw.GetLength() == 0))
	{
		MessageBox(_T("password length between 1 and 10!"), _T("Err"), MB_OK|MB_ICONSTOP);
		m_ePassw.SetFocus();
		m_ePassw.SetSel(0, -1);
		return;
	}

	strcpy(szUser,  m_sUser);
	strcpy(szPassW, m_sPassw);

	if (VE_OK == deleteVerifyNode(szUser, szPassW, m_sVfFile)){
		loadVyFile(m_sVfFile);
		RefreshList();
		szErr.Format("User = %s PassW = %s", szUser, szPassW);
		MessageBox(szErr, _T("Del OK"), MB_OK);
	}else{
		szErr.Format("User = %s PassW = %s", szUser, szPassW);
		MessageBox(szErr, _T("Del Aborted"), MB_OK|MB_ICONSTOP);
	}
}

void CVfManagerDlg::OnBnClickedVfmLoad()
{
	// TODO: Add your control notification handler code here
	loadVyFile(m_sVfFile);
	RefreshList();
}

void CVfManagerDlg::OnNMClickVymList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	int nItem = phdr->iItem;

	m_sUser  = m_list.GetItemText(nItem, 0x01);
	m_eUser.SetWindowText(m_sUser);

	m_sPassw = m_list.GetItemText(nItem, 0x02);
	m_ePassw.SetWindowText(m_sPassw);
	*pResult = 0;
}
