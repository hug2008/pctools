// GridCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"

#define PRN_XMARGIN		5
#define PRN_YMARGIN		5
#define PRN_PORTRAIT_MIN_PAGE_WIDHT		797
#define PRN_LANDSCAPE_MIN_PAGE_WIDHT	1127

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl

IMPLEMENT_DYNCREATE(CGridCtrl, CXGridCtrl)

BEGIN_MESSAGE_MAP(CGridCtrl, CXGridCtrl)
	//{{AFX_MSG_MAP(CGridCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGridCtrl::CGridCtrl()
{
	CString strTmp;
	strTmp.LoadString(ID_COL_DEBIT);
	DefineColCtrl(AddCol(115, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_DEBIT), GA_EDITCTRL, WS_CHILD | ES_RIGHT);
	strTmp.LoadString(ID_COL_CREDIT);
	DefineColCtrl(AddCol(115, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_CREDIT), GA_EDITCTRL, WS_CHILD | ES_RIGHT);
	strTmp.LoadString(ID_COL_SALDO);
	DefineColCtrl(AddCol(120, strTmp, ACFF_RIGHT, AHFF_CENTER, 0, 0, ID_COL_SALDO), GA_CELLCTRL);

	for(int i = 0; i < 20; i++)
	{
		RECORD_INFO rc_Info;
		rc_Info.m_Dedet.SetCurrency(i*20,0); 
		rc_Info.m_Credit.SetCurrency(i*10,0); 
		m_RowInfo.Add(rc_Info);
	}

	SetGridRowCount(m_RowInfo.GetSize());
}

CGridCtrl::~CGridCtrl()
{
	RemoveAllCol();
}

CELL_DATA CGridCtrl::GetCellData(int nCol, int nRow)
{
	CELL_DATA CellData = CALXGrid::GetCellData(nCol,nRow);

	switch(GetCellCtrlID(nCol,nRow))
	{
	case ID_COL_DEBIT:
		CellData.m_strText = m_RowInfo[nRow].m_Dedet.Format();
		break;
	case ID_COL_CREDIT:
		CellData.m_strText = m_RowInfo[nRow].m_Credit.Format();
		break;
	case ID_COL_SALDO:
		CellData.m_strText = (m_RowInfo[nRow].m_Dedet - m_RowInfo[nRow].m_Credit).Format();
		break;
	}
	return CellData;
}

BOOL CGridCtrl::OnSaveCellData(int nCol, int nRow)
{
	CALXCellCtrl* pCellCtrl = GetCellCtrl(nCol,nRow);
	if(pCellCtrl != NULL)
	{
		CELL_DATA Data = pCellCtrl->GetCellData();
		switch(GetCellCtrlID(nCol,nRow))
		{
		case ID_COL_DEBIT:
			m_RowInfo[nRow].m_Dedet.ParseCurrency(Data.m_strText);
			break;
		case ID_COL_CREDIT:
			m_RowInfo[nRow].m_Credit.ParseCurrency(Data.m_strText);
			break;
		}
		InvalidateCell(2,nRow);
	}
	return TRUE;
}

void CGridCtrl::Print()
{
	// get default print info
	CPrintInfo printInfo;
	ASSERT(printInfo.m_pPD != NULL);    // must be set

	if (OnPreparePrinting(&printInfo))
	{
		// hDC must be set (did you remember to call DoPreparePrinting?)
		ASSERT(printInfo.m_pPD->m_pd.hDC != NULL);

		// gather file to print to if print-to-file selected
		CString strOutput;
		if (printInfo.m_pPD->m_pd.Flags & PD_PRINTTOFILE && !printInfo.m_bDocObject)
		{
			// construct CFileDialog for browsing
			CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
			CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
			CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
			CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
			CFileDialog dlg(FALSE, strDef, strPrintDef,
				OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
			dlg.m_ofn.lpstrTitle = strCaption;

			if (dlg.DoModal() != IDOK)
				return;

			// set output device to resulting path name
			strOutput = dlg.GetPathName();
		}

		// set up document info and start the document printing process
		CString strTitle;
		GetParentFrame()->GetWindowText(strTitle);
		if (strTitle.GetLength() > 31)
			strTitle.ReleaseBuffer(31);
		DOCINFO docInfo;
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = strTitle;
		CString strPortName;
		int nFormatID;
		if (strOutput.IsEmpty())
		{
			docInfo.lpszOutput = NULL;
			strPortName = printInfo.m_pPD->GetPortName();
			nFormatID = AFX_IDS_PRINTONPORT;
		}
		else
		{
			docInfo.lpszOutput = strOutput;
			nFormatID = AFX_IDS_PRINTTOFILE;
		}

		// setup the printing DC
		CDC dcPrint;
		if (!printInfo.m_bDocObject)
		{
			dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
			dcPrint.m_bPrinting = TRUE;
		}
		OnBeginPrinting(&dcPrint, &printInfo);


		// disable main window while printing & init printing status dialog
		AfxGetMainWnd()->EnableWindow(FALSE);

		CString strTemp;
		AfxFormatString1(strTemp, nFormatID, strPortName);

		// start document printing process
		if (!printInfo.m_bDocObject && dcPrint.StartDoc(&docInfo) == SP_ERROR)
		{
			// enable main window before proceeding
			AfxGetMainWnd()->EnableWindow(TRUE);

			// cleanup and show error message
			OnEndPrinting(&dcPrint, &printInfo);
			dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
			AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
			return;
		}

		// Guarantee values are in the valid range
		UINT nEndPage = printInfo.GetToPage();
		UINT nStartPage = printInfo.GetFromPage();

		if (nEndPage < printInfo.GetMinPage())
			nEndPage = printInfo.GetMinPage();
		if (nEndPage > printInfo.GetMaxPage())
			nEndPage = printInfo.GetMaxPage();

		if (nStartPage < printInfo.GetMinPage())
			nStartPage = printInfo.GetMinPage();
		if (nStartPage > printInfo.GetMaxPage())
			nStartPage = printInfo.GetMaxPage();

		int nStep = (nEndPage >= nStartPage) ? 1 : -1;
		nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

		VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));

		// If it's a doc object, we don't loop page-by-page
		// because doc objects don't support that kind of levity.

		BOOL bError = FALSE;
		if (printInfo.m_bDocObject)
		{
			OnPrepareDC(&dcPrint, &printInfo);
			OnPrint(&dcPrint, &printInfo);
		}
		else
		{
			// begin page printing loop
			for (printInfo.m_nCurPage = nStartPage;
				printInfo.m_nCurPage != nEndPage; printInfo.m_nCurPage += nStep)
			{
				OnPrepareDC(&dcPrint, &printInfo);

				// check for end of print
				if (!printInfo.m_bContinuePrinting)
					break;

				// set up drawing rect to entire page (in logical coordinates)
				printInfo.m_rectDraw.SetRect(0, 0,
					dcPrint.GetDeviceCaps(HORZRES),
					dcPrint.GetDeviceCaps(VERTRES));
				dcPrint.DPtoLP(&printInfo.m_rectDraw);

				// attempt to start the current page
				if (dcPrint.StartPage() < 0)
				{
					bError = TRUE;
					break;
				}

				ASSERT(printInfo.m_bContinuePrinting);

				// page successfully started, so now render the page
				OnPrint(&dcPrint, &printInfo);
				if (dcPrint.EndPage() < 0)
				{
					bError = TRUE;
					break;
				}
			}
		}

		// cleanup document printing process
		if (!printInfo.m_bDocObject)
		{
			if (!bError)
				dcPrint.EndDoc();
			else
				dcPrint.AbortDoc();
		}

		AfxGetMainWnd()->EnableWindow();    // enable main window

		OnEndPrinting(&dcPrint, &printInfo);    // clean up after printing

		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
	}
}

BOOL CGridCtrl::DoPreparePrinting(CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;

	// don't prompt the user if we're doing print preview, printing directly,
	// or printing via IPrint and have been instructed not to ask

	CWinApp* pApp = AfxGetApp();
	if (pInfo->m_bPreview || pInfo->m_bDirect ||
		(pInfo->m_bDocObject && !(pInfo->m_dwFlags & PRINTFLAG_PROMPTUSER)))
	{
		if (pInfo->m_pPD->m_pd.hDC == NULL)
		{
			// if no printer set then, get default printer DC and create DC without calling
			//   print dialog.
			if (!pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd))
			{
				// bring up dialog to alert the user they need to install a printer.
				if (!pInfo->m_bDocObject || (pInfo->m_dwFlags & PRINTFLAG_MAYBOTHERUSER))
					if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
						return FALSE;
			}

			if (pInfo->m_pPD->m_pd.hDC == NULL)
			{
				// call CreatePrinterDC if DC was not created by above
				if (pInfo->m_pPD->CreatePrinterDC() == NULL)
					return FALSE;
			}
		}

		// set up From and To page range from Min and Max
		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();
	}
	else
	{
		// otherwise, bring up the print dialog and allow user to change things
		// preset From-To range same as Min-Max range
		pInfo->m_pPD->m_pd.nFromPage = (WORD)pInfo->GetMinPage();
		pInfo->m_pPD->m_pd.nToPage = (WORD)pInfo->GetMaxPage();

		if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
			return FALSE;       // do not print
	}

	ASSERT(pInfo->m_pPD != NULL);
	ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);
	if (pInfo->m_pPD->m_pd.hDC == NULL)
		return FALSE;

	pInfo->m_nNumPreviewPages = pApp->m_nNumPreviewPages;
	VERIFY(pInfo->m_strPageDesc.LoadString(AFX_IDS_PREVIEWPAGEDESC));
	return TRUE;
}

/* ���������� � ������ */
BOOL CGridCtrl::OnPreparePrinting(CPrintInfo* pInfo)
{
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_pPD != NULL);

	m_nPageWight = PRN_XMARGIN * 2; // ������ ������ 1 � ����� 1
	// ���������� ������ �����
	for(int i = 0; i < GetHeadColCount(); i++)
		m_nPageWight = m_nPageWight + GetHeadColWidth(i);

	if (pInfo->m_pPD->m_pd.nMinPage > pInfo->m_pPD->m_pd.nMaxPage)
		pInfo->m_pPD->m_pd.nMaxPage = pInfo->m_pPD->m_pd.nMinPage;

	CGridApp* pApp = (CGridApp*) AfxGetApp();

	// ������������ ��� ������� �� ���������� �� '�������' ��������
	if(m_nPageWight >= PRN_PORTRAIT_MIN_PAGE_WIDHT)
	{
		// don't prompt the user if we're doing print preview, printing directly,
		// or printing via IPrint and have been instructed not to ask
		if (!(pInfo->m_dwFlags & PRINTFLAG_PROMPTUSER))
		{
			if (pInfo->m_pPD->m_pd.hDC == NULL)
			{
				// if no printer set then, get default printer DC and create DC without calling
				//   print dialog.
				pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd);
			}
		}
	}

	// ��������� ������ �������� �� ������
	if(pApp->GetPageOrientation() & DMORIENT_LANDSCAPE)
	{
		if(m_nPageWight < PRN_LANDSCAPE_MIN_PAGE_WIDHT)
			m_nPageWight = PRN_LANDSCAPE_MIN_PAGE_WIDHT;
		m_nPageHight = m_nPageWight*210/297;
	}
	else
	{
		if(m_nPageWight < PRN_PORTRAIT_MIN_PAGE_WIDHT)
			m_nPageWight = PRN_PORTRAIT_MIN_PAGE_WIDHT;
		m_nPageHight = m_nPageWight*297/210;
	}

	// ��������� ������� ����� ���������� �� ��������
	m_nRowInPage = (m_nPageHight - PRN_YMARGIN*2 - GetHeadRowCount()*GetHeadRowHeight())/GetGridRowHeight();
	
	// ���������� ���������� �������
	pInfo->SetMaxPage(GetGridRowCount()/m_nRowInPage + (GetGridRowCount()%m_nRowInPage > 0 ? 1 : 0));

	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGridCtrl::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
    if(!pCurrentDC || !pDC) 
		return;

    // Get the page sizes (physical and logical)
    m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	
    CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
    CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));

    m_LogicalPageSize.cx = ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx;

	if(m_PaperSize.cx > m_nPageWight * PaperPixelsPerInch.cx / ScreenPixelsPerInch.cx)
	{
	    m_LogicalPageSize.cy = ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy;
	    m_nPageHight = m_LogicalPageSize.cy;
	}
	else
	{
		m_nPageHight = m_nPageWight*m_PaperSize.cy/m_PaperSize.cx;

	    m_LogicalPageSize.cx = m_nPageWight;
		m_LogicalPageSize.cy = m_nPageHight;
	}

	m_nRowInPage = (m_nPageHight - PRN_YMARGIN*2 - GetHeadRowCount()*GetHeadRowHeight())/GetGridRowHeight();

	pInfo->SetMaxPage(GetGridRowCount()/m_nRowInPage + (GetGridRowCount()%m_nRowInPage > 0 ? 1 : 0));

	ReleaseDC(pCurrentDC);
}

/* ������� �� ������ */
void CGridCtrl::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// ������ ������� ��� ����� ������� ���������� �� ������
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_LogicalPageSize);
	pDC->SetViewportExt(m_PaperSize);
	pDC->SetWindowOrg(0, 0);

	PrintHead(pDC, pInfo);
	PrintGridPage(pDC, pInfo);
}

void CGridCtrl::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/* �������� ��������� ������� */
void CGridCtrl::PrintHead(CDC* pDC, CPrintInfo* pInfo)
{
	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(GetHeadFont());
	int OldBkMode =	pDC->SetBkMode(TRANSPARENT);

	// ��������� ������ ������ � ������� �����������,
	for (int nRow = 0, y = PRN_YMARGIN; nRow < GetHeadRowCount(); y += GetHeadRowHeight(), nRow++)
		// ��������� ������ ������� � ������� �����������
		for (int nCol = 0, x = PRN_XMARGIN; nCol < GetHeadColCount(nRow); x = x + GetHeadColWidth(nCol,nRow), nCol++)
			PrintHeadCell(pDC,nCol,nRow,x,y);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OldBkMode);
}

/* �������� ������� �� ��������� �������� */
void CGridCtrl::PrintGridPage(CDC* pDC, CPrintInfo* pInfo)
{
	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(GetGridFont());
	int OldBkMode =	pDC->SetBkMode(TRANSPARENT);
	UINT OldAlign = pDC->GetTextAlign(); 
	pDC->SetTextAlign(TA_LEFT | TA_TOP | TA_NOUPDATECP);

	int y = GetHeadRowCount()*GetHeadRowHeight()+PRN_YMARGIN;

	int nFirstRow = (pInfo->m_nCurPage-1)*m_nRowInPage;
	int nLastRow = min(GetGridRowCount(),(int)(pInfo->m_nCurPage*m_nRowInPage));

	// ��������� ������ ������ � ������� �����������,
	for (int nRow = nFirstRow; nRow < nLastRow; y += GetGridRowHeight(), nRow++)
		// ��������� ������ ������ � ������� �����������
		for (int nCol = 0, x = PRN_XMARGIN; nCol < GetHeadColCount(); x = x + GetHeadColWidth(nCol), nCol++)
			PrintGridCell(pDC,nCol,nRow,x,y);

	if(pOldFont != NULL)
		pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OldBkMode);
	pDC->SetTextAlign(OldAlign);
}

/* ������ ������ ����� */
void CGridCtrl::PrintHeadCell(CDC* pDC, int nCol, int nRow, int x, int y)
{
	CRect rectCell(x,y,x+GetHeadColWidth(nCol,nRow)+1,y+GetHeadRowHeight()+1);
	COLORREF clrCellFrame = RGB(0,0,0);

	// ������� �����
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,1,1);

	CBrush brushBackground;
	// ����������� ������
	if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE)))
		pDC->FillRect(&rectCell, &brushBackground);
	rectCell.DeflateRect(2,2,1,0);
	// �������� ����� � ����� 
	HEADCELL_INFO CellInfo = GetHeadCellInfo(nCol,nRow);
	// ������� �����
	pDC->DrawText(CellInfo.m_strText, rectCell, CellInfo.m_nFormat);
}

/* ������ ������ ����� */
void CGridCtrl::PrintGridCell(CDC* pDC, int nCol, int nRow, int x, int y)
{
	CELL_INFO CellInfo;
	CELL_DATA CellData;
	CRect rectCell(x,y,x+GetHeadColWidth(nCol)+1,y+GetHeadRowHeight()+1);
	COLORREF clrCellFrame = RGB(0,0,0);

	// ������� �����
	pDC->Draw3dRect(rectCell,clrCellFrame,clrCellFrame);
	rectCell.DeflateRect(1,1,1,1);

	CBrush brushBackground;
	// ����������� ������
	if(brushBackground.CreateSolidBrush(::GetSysColor(COLOR_WINDOW)))
		pDC->FillRect(&rectCell, &brushBackground);
	rectCell.DeflateRect(2,2,1,0);

	// �������� ���������� � ���� ������
	CellInfo = GetCellInfo(nCol,nRow);
	// �������� ���������� � ���������� ������
	CellData = GetCellData(nCol,nRow);

	rectCell.DeflateRect(1,1,1,1); // ��������� ������� ���������

	if(CellInfo.m_nTypeCtrl > 0)
		// ���� ����, ������ ������� ����������
		DrawCellCtrl(pDC,rectCell,CellInfo,CellData);
	else
		// ������� �����
		PrintText(pDC,CellData.m_strText, rectCell, CellInfo.m_nFormat);
}

/* ������ ������ � ������. ������������ ����� ��� ������
   ������ ����������� �� ������� ���� �� ��������� ���������
   ������� (������� �� ��������� ��������).                   */
void CGridCtrl::PrintText(CDC *pDC, CString &str, CRect& rect, UINT nFormat)
{
	// ���� ������������ �� �� ������� ���� � ���� ����� �� ����������
	if(!(nFormat & DT_RIGHT) || pDC->GetTextExtent(str).cx > rect.Width())
	{
		pDC->DrawText(str,rect,nFormat);
		return;
	}

	UINT OldAlign = pDC->GetTextAlign();

	pDC->SetTextAlign(TA_RIGHT | TA_TOP);

	pDC->TextOut(rect.right,rect.top,str);

	pDC->SetTextAlign(OldAlign);
}

void CGridCtrl::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);
	UNUSED(pDC); // unused in release builds

	// Default to one page printing if doc length not known
	if (pInfo != NULL)
		pInfo->m_bContinuePrinting =
			(pInfo->GetMaxPage() != 0xffff || (pInfo->m_nCurPage == 1));
}



