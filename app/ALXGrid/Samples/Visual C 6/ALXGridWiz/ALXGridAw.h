#if !defined(AFX_ALXGRIDAW_H__09AA4886_AFD0_11D4_8A40_005004555789__INCLUDED_)
#define AFX_ALXGRIDAW_H__09AA4886_AFD0_11D4_8A40_005004555789__INCLUDED_

// ALXGridaw.h : header file
//
#include "Names.h"

class CDialogChooser;

// All function calls made by mfcapwz.dll to this custom AppWizard (except for
//  GetCustomAppWizClass-- see ALXGrid.cpp) are through this class.  You may
//  choose to override more of the CCustomAppWiz virtual functions here to
//  further specialize the behavior of this custom AppWizard.
class CALXGridAppWiz : public CCustomAppWiz
{
public:
	virtual CAppWizStepDlg* Next(CAppWizStepDlg* pDlg);
	virtual CAppWizStepDlg* Back(CAppWizStepDlg* pDlg);
		
	virtual void InitCustomAppWiz();
	virtual void ExitCustomAppWiz();
	virtual void CustomizeProject(IBuildProject* pProject);

	virtual void ProcessTemplate( LPCTSTR lpszInput, DWORD dwSize,
                                      OutputStream* pOutput);

	void GetClassInfo(void);

	CNamesArray m_Clas;
	
	CString m_strGridConstr, m_strGridRes, m_strGridStr;

protected:
	CDialogChooser* m_pChooser;
private:
	CString m_strTypeProject;
	BOOL	m_bNoDocView;

	void GetAdvClassInfo(int ind);
	void RedifineClass();
};

// This declares the one instance of the CALXGridAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global ALXAppWz.  (Its definition is in ALXGridaw.cpp.)
extern CALXGridAppWiz ALXAppWz;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALXGRIDAW_H__09AA4886_AFD0_11D4_8A40_005004555789__INCLUDED_)
