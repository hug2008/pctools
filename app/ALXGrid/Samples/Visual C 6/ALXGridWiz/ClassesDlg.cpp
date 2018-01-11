// ClassDlg.cpp : implementation file
// CG: This file was added by the AppWizard Tools component

#include "stdafx.h"
#include "ALXGridwz.h"
#include "ClassesDlg.h"
#include "WizUtil.h"
#include "ALXGridaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassesDlg dialog


CClassesDlg::CClassesDlg()
	: CAppWizStepDlg(CClassesDlg::IDD)
{
	m_bmpFlag.LoadBitmap(IDB_FLAG);
	m_bmpFon.LoadBitmap(IDB_FON);

	//{{AFX_DATA_INIT(CClassesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iSelOld = 0;
	m_bValidate = TRUE;
}

void CClassesDlg::DDV_FileName(CDataExchange* pDX, CString& s,
	LPCTSTR pszExtension)
{
	if (!pDX->m_bSaveAndValidate || !m_bValidate)
		return;

	if (!IsValidFileName(s))
	{
		AfxMessageBox(CG_IDS_INVALID_FILENAME);
		pDX->Fail();
	}

	if (pszExtension == NULL)
		return;

	// make sure the suffix is ok
	TCHAR szExt[_MAX_EXT];
	_tsplitpath(s, NULL, NULL, NULL, szExt);
	if (szExt[0] == _T('\0'))
	{
		// no extension - add it
		s += pszExtension;
	}
	else if (_tcsicmp(szExt, pszExtension) != 0)
	{
		CString strMessage;
		strMessage.Format(CG_IDS_INVALID_EXTENSION, pszExtension);
		AfxMessageBox(strMessage);
		pDX->Fail();
	}
}

void CClassesDlg::DDV_ClassName(CDataExchange* pDX, CString& s)
{
	if (!pDX->m_bSaveAndValidate || !m_bValidate)
		return;

	if (!IsValidSymbol(s))
	{
		AfxMessageBox(CG_IDS_INVALID_CLASS);
		pDX->Fail();
	}
}

void CClassesDlg::DoDataExchange(CDataExchange* pDX)
{
	CAppWizStepDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassesDlg)
	DDX_Control(pDX, CG_IDC_CLASS_LIST, m_classList);
	//}}AFX_DATA_MAP

	int nCount = ALXAppWz.m_Clas.GetSize();
	if (nCount > 0)
	{
		CNames* pNames = &ALXAppWz.m_Clas[m_iSelOld];

		DDX_Text(pDX, CG_IDC_CLASS_NAME, pNames->m_strClass);
		DDV_ClassName(pDX, pNames->m_strClass);

		DDX_Text(pDX, CG_IDC_HFILE, pNames->m_strHFile);
		DDV_FileName(pDX, pNames->m_strHFile, _T(".h"));

		DDX_Text(pDX, CG_IDC_IFILE, pNames->m_strIFile);
		DDV_FileName(pDX, pNames->m_strIFile, _T(".cpp"));

		DDX_Text(pDX, CG_IDC_BASE_CLASS, pNames->m_strBaseClass);

		if (pDX->m_bSaveAndValidate && m_bValidate)
		{
			// Make sure we don't have duplicate class & file names
			for (int iOther = 0; iOther < nCount; iOther++)
			{
				if (iOther == m_iSelOld)
					continue;
				CNames* pOtherNames = &ALXAppWz.m_Clas[iOther];

				if (pNames->m_strClass == pOtherNames->m_strClass)
				{
					AfxMessageBox(CG_IDS_DUPLICATE_CLASS);
					pDX->PrepareEditCtrl(CG_IDC_CLASS_NAME);
					pDX->Fail();
				}
				if (!pNames->m_strHFile.CompareNoCase(pOtherNames->m_strHFile))
				{
					AfxMessageBox(CG_IDS_DUPLICATE_FILE);
					pDX->PrepareEditCtrl(CG_IDC_HFILE);
					pDX->Fail();
				}
				if (!pNames->m_strIFile.CompareNoCase(pOtherNames->m_strIFile))
				{
					AfxMessageBox(CG_IDS_DUPLICATE_FILE);
					pDX->PrepareEditCtrl(CG_IDC_IFILE);
					pDX->Fail();
				}
			}
		}
	}
}

// This is called whenever the user presses Next, Back, or Finish with this step
// present.  Do all validation & data exchange from the dialog in this function.
BOOL CClassesDlg::OnDismiss()
{
	if (!UpdateData(TRUE))
		return FALSE;

	// Define macros for all the names
/*	int nCount = ALXAppWz.m_Clas.GetSize();
	for (int i=0; i < nCount; i++)
		ALXAppWz.m_Clas[i].DefineMacros();
*/	
	return TRUE;	// return FALSE if the dialog shouldn't be dismissed
}


BEGIN_MESSAGE_MAP(CClassesDlg, CAppWizStepDlg)
	//{{AFX_MSG_MAP(CClassesDlg)
	ON_LBN_SELCHANGE(CG_IDC_CLASS_LIST, OnSelectClass)
	ON_EN_CHANGE(CG_IDC_CLASS_NAME, OnChangeClassName)
	ON_EN_CHANGE(CG_IDC_HFILE, OnChangeHFile)
	ON_EN_CHANGE(CG_IDC_IFILE, OnChangeIFile)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CClassesDlg message handlers


void CClassesDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CAppWizStepDlg::OnShowWindow(bShow, nStatus);
	if (!bShow)
		return;

	ALXAppWz.GetClassInfo();

	// Dialog initialization-- initialize contents of classes list

	UpdateData(FALSE);	// To get m_classList
	m_classList.SetRedraw(FALSE);
	m_classList.ResetContent();
	int nCount = ALXAppWz.m_Clas.GetSize();
	for (int i = 0; i < nCount; i++)
		m_classList.AddString(ALXAppWz.m_Clas[i].m_strClass);
	m_classList.SetCurSel(m_iSelOld = 0);
	m_classList.SetRedraw(TRUE);
	UpdateData(FALSE);	// To update edit controls with first class
}

void CClassesDlg::OnSelectClass()
{
	// get current selection
	int iSel = m_classList.GetCurSel();
	ASSERT(iSel >= 0);
	if (m_iSelOld == iSel)
		return;

	// get class ID associated with that item
	if (!UpdateData(TRUE))	// save old class
	{
		// restore to old selection
		m_classList.SetCurSel(m_iSelOld);
		return;
	}

	// class name may have changed
	CString oldName;
	m_classList.GetText(m_iSelOld, oldName);
	CNames* pNames = &ALXAppWz.m_Clas[m_iSelOld];
	if (pNames->m_strClass != oldName)
	{
		m_classList.DeleteString(m_iSelOld);
		m_classList.InsertString(m_iSelOld, pNames->m_strClass);
		m_classList.SetCurSel(iSel);
	}

	m_iSelOld = iSel;
	UpdateData(FALSE);	// re-fill data with new class
}

void CClassesDlg::WriteValues()
{
	m_bValidate = FALSE;
	UpdateData(TRUE);
	m_bValidate = TRUE;
}

void CClassesDlg::OnChangeClassName() 
{
	// Copy values in edit controls to the Names struct
	WriteValues();

	// Update filenames from class name
	ALXAppWz.m_Clas[m_iSelOld].UpdateFiles();

	// Display new filenames
	UpdateData(FALSE);
}

void CClassesDlg::OnChangeHFile() 
{
	ALXAppWz.m_Clas[m_iSelOld].m_bTrackHFile = FALSE;
}

void CClassesDlg::OnChangeIFile() 
{
	ALXAppWz.m_Clas[m_iSelOld].m_bTrackIFile = FALSE;
}

void CClassesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(rect);

	// Create a pattern brush from the bitmap.
	CBrush brush;
	CSize sizeFlag;
	BITMAP bmflag;
	brush.CreatePatternBrush(&m_bmpFon);
	m_bmpFlag.GetBitmap(&bmflag);
	sizeFlag.cx = bmflag.bmWidth;
	sizeFlag.cy = bmflag.bmHeight;

	// Select the brush into a device context, and draw.
	CBrush* pOldBrush = (CBrush*)dc.SelectObject(&brush);
	dc.FillRect(&CRect(0, 0, bmflag.bmWidth + 17, rect.Height()),&brush); 	
	// Restore the original brush.
	dc.SelectObject(pOldBrush);


	dc.DrawState(0,sizeFlag,m_bmpFlag,DSS_NORMAL);
	
	CRect rect2(bmflag.bmWidth + 17, 0,bmflag.bmWidth + 17 + 2, rect.Height()+1);
	dc.Draw3dRect(rect2,::GetSysColor(COLOR_BTNSHADOW) , ::GetSysColor(COLOR_WINDOW));
}

