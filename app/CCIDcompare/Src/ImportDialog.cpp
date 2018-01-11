// ImportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CCIDcompare.h"
#include "ImportDialog.h"
#include ".\importdialog.h"


// CImportDialog dialog

IMPLEMENT_DYNAMIC(CImportDialog, CDialog)
CImportDialog::CImportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CImportDialog::IDD, pParent)
	, m_strpathname(_T("D:\\\\.xls"))
	, m_strSheetname(_T("新都"))
	, M_INTTELCOL(1)
	, M_INTCCIDCOL(2)
{
}

CImportDialog::~CImportDialog()
{
}

void CImportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strpathname);
	DDX_Text(pDX, IDC_EDIT2, m_strSheetname);
	DDX_Text(pDX, IDC_EDIT3, M_INTTELCOL);
	DDV_MinMaxInt(pDX, M_INTTELCOL, 1, 100);
	DDX_Text(pDX, IDC_EDIT6, M_INTCCIDCOL);
	DDV_MinMaxInt(pDX, M_INTCCIDCOL, 1, 100);
}


BEGIN_MESSAGE_MAP(CImportDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CImportDialog message handlers
BOOL CImportDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText("  指定数据源路径");

	return TRUE;
}
void CImportDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CDialog::UpdateData(TRUE);
	CFileDialog dlg(TRUE,
		            NULL,
					NULL, 
					OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					"Excel Files (*.xls)|*.xls||",
					AfxGetMainWnd());
	if (dlg.DoModal() == IDOK){
		m_strpathname = dlg.GetPathName();
	}else{
		AfxMessageBox("Can't get this Excel's file", MB_OK|MB_ICONERROR);
	}
	CDialog::UpdateData(FALSE);
}
