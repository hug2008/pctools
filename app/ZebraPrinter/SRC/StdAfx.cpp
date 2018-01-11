// stdafx.cpp : source file that includes just the standard includes
//	BizPhoneManager.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information
#include "stdafx.h"

void  SetRadioCheck(CDialog *hDlg, int nRadioId, BOOL bRadioCheck)
{
	((CButton*)hDlg->GetDlgItem(nRadioId))->SetCheck(bRadioCheck);
}

void  EnableDlgItemWindow(CDialog *hDlg, int nDlgItemId, BOOL bEnabde)
{
	hDlg->GetDlgItem(nDlgItemId)->EnableWindow(bEnabde);
}


BOOL  IsRadioCheck(CDialog *hDlg, int nRadioId)
{
	return ((CButton*)hDlg->GetDlgItem(nRadioId))->GetCheck();
}