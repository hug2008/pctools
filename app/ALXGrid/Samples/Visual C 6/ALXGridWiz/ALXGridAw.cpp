// ALXGridaw.cpp : implementation file
//

#include "stdafx.h"
#include "ALXGridwz.h"
#include "ALXGridaw.h"
#include "chooser.h"
#include "WizUtil.h"

#import "C:\Program Files\Microsoft Visual Studio\Common\MsDev98\Bin\IDE\DevBld.Pkg" 

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// This is called immediately after the custom AppWizard is loaded.  Initialize
//  the state of the custom AppWizard here.
void CALXGridAppWiz::InitCustomAppWiz()
{
	// Create a new dialog chooser; CDialogChooser's constructor initializes
	//  its internal array with pointers to the steps.
	m_pChooser = new CDialogChooser;

	// At first, we don't know the total number of steps, since there are two
	//  possible "tracks" (MDI/SDI app and dialog-based app).
	SetNumberOfSteps(-1);

	// Inform AppWizard of the languages we support
	SetSupportedLanguages(_T("English [USA] (APPWZENU.DLL);0x40904b0"));
}

// This is called just before the custom AppWizard is unloaded.
void CALXGridAppWiz::ExitCustomAppWiz()
{
	// Deallocate memory used for the dialog chooser
	ASSERT(m_pChooser != NULL);
	delete m_pChooser;
	m_pChooser = NULL;

	// TODO: Add code here to deallocate resources used by the custom AppWizard
}

// This is called when the user clicks "Create..." on the New Project dialog
//  or "Next" on one of the custom AppWizard's steps.
CAppWizStepDlg* CALXGridAppWiz::Next(CAppWizStepDlg* pDlg)
{
	// Delegate to the dialog chooser
	return m_pChooser->Next(pDlg);
}

// This is called when the user clicks "Back" on one of the custom
//  AppWizard's steps.
CAppWizStepDlg* CALXGridAppWiz::Back(CAppWizStepDlg* pDlg)
{
	// Delegate to the dialog chooser
	return m_pChooser->Back(pDlg);
}

void CALXGridAppWiz::CustomizeProject(IBuildProject* pProject)
{

//   if(IsMacroDefine(_T("PROJTYPE_DLG")))
//		return;

   using namespace DSProjectSystem;

      long lNumConfigs;
      IConfigurationsPtr pConfigs;
      IBuildProjectPtr pProj;
      // Needed to convert IBuildProject to the DSProjectSystem namespace
      pProj.Attach((DSProjectSystem::IBuildProject*)pProject, true);

      pProj->get_Configurations(&pConfigs);
      pConfigs->get_Count(&lNumConfigs);
      //Get each individual configuration
      for (long j = 1 ; j < lNumConfigs+1 ; j++)
      {
         _bstr_t varName;
         _bstr_t varTool;
         _bstr_t varSwitch;
         IConfigurationPtr pConfig;
         _variant_t varj = j;

         pConfig = pConfigs->Item(varj);

		 CString strTmp;
		 strTmp = (LPTSTR)pConfig->GetName();
		 if(strTmp.Find(_T("Debug")) == -1)
		 {
			 if(IsMacroDefine(_T("MFCDLL")))
				varSwitch = _T("ALXGridR.lib");
			 else
				varSwitch = _T("ALXGridRs.lib");
		 }
		 else
		 {
			 if(IsMacroDefine(_T("MFCDLL")))
				varSwitch = _T("ALXGridD.lib");
			 else
				varSwitch = _T("ALXGridDs.lib");
		 }

         varTool = _T("link.exe");
         pConfig->AddToolSettings(varTool, varSwitch, varj);

//		 varSwitch = _T("/i \"") + m_strGridPath +"\"";
//       varTool = _T("rc.exe");
//       pConfig->AddToolSettings(varTool, varSwitch, varj);
	  }
      
}

void CALXGridAppWiz::ProcessTemplate( LPCTSTR lpszInput, DWORD dwSize,
                                      OutputStream* pOutput )
{
	if((IsMacroDefine(_T("NODOCVIEW")) && !m_bNoDocView) || !IsMacroDefine(m_strTypeProject))
		GetClassInfo();
	CString strTmp;
	if(IsMacroDefine(_T("PROJTYPE_DLG")))
	{
		RedifineClass();
		m_Clas[2].DefineMacros();
	}
	else
	{
		RedifineClass();


		UndefMacro(_T("CListView"));
		UndefMacro(_T("CFormView"));
		if(IsMacroDefine(_T("CRecordView")))
		{
			UndefMacro(_T("CRecordView"));
			DefineBoolMacro(_T("CALXRecordView"), TRUE);
		}

		if(IsMacroDefine(_T("CDaoRecordView")))
		{
			UndefMacro(_T("CDaoRecordView"));
			DefineBoolMacro(_T("CALXDaoRecordView"), TRUE);
		}

		if(IsMacroDefine(_T("COleDBRecordView")))
		{
			UndefMacro(_T("COleDBRecordView"));
			DefineBoolMacro(_T("CALXOleDBRecordView"), TRUE);
		}
	}

	DefineStringMacro(_T("GRID_CONSTR"), m_strGridConstr);
	DefineStringMacro(_T("GRID_ID"), m_strGridRes);
	DefineStringMacro(_T("GRID_STR"),m_strGridStr);

/*
	CStdioFile m_FileLST;
	if(!m_FileLST.Open("C:\\TEMP\\Vars.TXT",CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		MessageBox(NULL,"Не удается открыть файл:\nC:\\TEMP\\Vars.TXT",
			"Ошибка записи",MB_OK|MB_ICONSTOP);
	}
	else
	{
		POSITION pos = m_Dictionary.GetStartPosition();
		CString key, value;
		for( pos = m_Dictionary.GetStartPosition(); pos != NULL; )
		{
			m_Dictionary.GetNextAssoc( pos, key, value );
			m_FileLST.WriteString(key+"\t\t"+value+"\n");
		}
	}
	m_FileLST.Close();
*/
   CCustomAppWiz::ProcessTemplate( lpszInput, dwSize, pOutput );
} 
// Here we define one instance of the CALXGridAppWiz class.  You can access
//  m_Dictionary and any other public members of this class through the
//  global ALXAppWz.
CALXGridAppWiz ALXAppWz;


void CALXGridAppWiz::RedifineClass()
{
	int nCount = m_Clas.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		if(IsMacroDefine(m_Clas[i].m_strMacro +_T("_CLASS")))
			m_Clas[i].DefineMacros();
	}
}

void CALXGridAppWiz::GetClassInfo()
{
	CString strDef;
	m_Dictionary.Lookup(_T("Safe_root"), strDef);
	m_Clas.RemoveAll( );

	if(IsMacroDefine(_T("PROJTYPE_MDI")))
	{
		m_strTypeProject = _T("PROJTYPE_MDI");

		if(IsMacroDefine(_T("NODOCVIEW")))
		{
			m_bNoDocView = TRUE;

			m_Clas.SetSize(4);
			m_Clas[0].Init(_T("WNDVIEW"),_T("View"),_T("CXGridCtrl"));
			m_Clas[1].Init(_T("APP"),_T("App"),_T("CWinApp"));
			m_Clas[2].Init(_T("FRAME"),_T("MainFrame"),_T("CMDIFrameWnd"), FALSE,_T("MainFrm"));
			m_Clas[3].Init(_T("CHILD_FRAME"),_T("ChildFrame"),_T("CMDIChildWnd"),FALSE,_T("ChildFrm"));
		}
		else
		{
			m_bNoDocView = FALSE;

			m_Clas.SetSize(11);
			m_Clas[0].Init(_T("VIEW"),_T("View"),_T("CXGridView"));
			m_Clas[1].Init(_T("APP"),_T("App"),_T("CWinApp"));
			m_Clas[2].Init(_T("FRAME"),_T("MainFrame"),_T("CMDIFrameWnd"),FALSE,_T("MainFrm"));
			m_Clas[3].Init(_T("CHILD_FRAME"),_T("ChildFrame"),_T("CMDIChildWnd"),FALSE,_T("ChildFrm"));
			m_Clas[4].Init(_T("DOC"),_T("Doc"),_T("CDocument"));

			GetAdvClassInfo(5);
		}
	}
	else if (IsMacroDefine(_T("PROJTYPE_SDI")))
	{
		m_strTypeProject = _T("PROJTYPE_SDI");

		if(IsMacroDefine(_T("NODOCVIEW")))
		{
			m_bNoDocView = TRUE;

			m_Clas.SetSize(3);
			m_Clas[0].Init(_T("WNDVIEW"),_T("View"),_T("CXGridCtrl"));
			m_Clas[1].Init(_T("APP"),_T("App"),_T("CWinApp"));
			m_Clas[2].Init(_T("FRAME"),_T("MainFrame"),_T("CFrameWnd"),FALSE,_T("MainFrm"));
		}
		else
		{
			m_bNoDocView = FALSE;

			m_Clas.SetSize(10);
			m_Clas[0].Init(_T("VIEW"),_T("View"),_T("CXGridView"));
			m_Clas[1].Init(_T("APP"),_T("App"),_T("CWinApp"));
			m_Clas[2].Init(_T("FRAME"),_T("MainFrame"),_T("CFrameWnd"),FALSE,_T("MainFrm"));
			m_Clas[3].Init(_T("DOC"),_T("Doc"),_T("CDocument"));

			GetAdvClassInfo(4);
		}
	}
	else
	{
		m_strTypeProject = _T("PROJTYPE_DLG");
		m_bNoDocView = FALSE;

		m_Clas.SetSize(3);
		m_Clas[0].Init(_T("APP"),_T("App"),_T("CWinApp"));
		m_Clas[1].Init(_T("DLG"),_T("Dlg"),_T("CDialog"));
		m_Clas[2].Init(_T("WNDVIEW"),_T("Ctrl"),_T("CXGridCtrl"));
	}
}

void CALXGridAppWiz::GetAdvClassInfo(int ind)
{
	CString strTemp, strDef;
	m_Dictionary.Lookup(_T("Safe_root"), strDef);

// RecordSet
	if(IsMacroDefine(_T("DAO")))
	{
		m_Clas[ind].Init(_T("RECSET"),_T("Set"),_T("CDaoRecorset"));
		ind++;
	}
	else if(IsMacroDefine(_T("OLEDB")))
	{
		m_Clas[ind].Init(_T("RECSET"),_T("Set"),_T("CCommand"));
		ind++;
	}
	else if(!IsMacroDefine(_T("DB_NO_DETECT")))
	{
		m_Clas[ind].Init(_T("RECSET"),_T("Set"),_T("CRecordset"));
		ind++;
	}

// OLE
	if(IsMacroDefine(_T("CONTAINER_SERVER")))
	{
		// Server
		if(IsMacroDefine(_T("ACTIVE_DOC_SERVER")))
			m_Clas[ind].Init(_T("SRVRITEM"),_T("SrvrItem"),_T("CDocObjectServerItem"),TRUE,_T("SrvrItem"));
		else
			m_Clas[ind].Init(_T("SRVRITEM"),_T("SrvrItem"),_T("COleServerItem"),TRUE,_T("SrvrItem"));

		ind++;

		// Container
		if(IsMacroDefine(_T("ACTIVE_DOC_CONTAINER")))
			m_Clas[ind].Init(_T("CNTRITEM"),_T("CntrItem"),_T("COleDocObjectItem"),TRUE,_T("CntrItem"));
		else
			m_Clas[ind].Init(_T("CNTRITEM"),_T("CntrItem"),_T("COleClientItem"),TRUE,_T("CntrItem"));

		ind++;

		//InPlaceFrame
		if(IsMacroDefine(_T("ACTIVE_DOC_SERVER")))
			m_Clas[ind].Init(_T("IPFRAME"), _T("InPlaceFrame"),_T("COleDocIPFrameWnd"),FALSE,_T("IpFrame"));
		else
			m_Clas[ind].Init(_T("IPFRAME"), _T("InPlaceFrame"),_T("COleIPFrameWnd"),FALSE,_T("IpFrame"));

		ind++;
	}
	else if(IsMacroDefine(_T("FULL_SERVER")))
	{
		if(IsMacroDefine(_T("ACTIVE_DOC_SERVER")))
		{
			// Server
			m_Clas[ind].Init(_T("SRVRITEM"),_T("SrvrItem"),_T("CDocObjectServerItem"),TRUE,_T("SrvrItem"));

			ind++;

			//InPlaceFrame
			m_Clas[ind].Init(_T("IPFRAME"), _T("InPlaceFrame"),_T("COleDocIPFrameWnd"),FALSE,_T("IpFrame"));

			ind++;
		}
		else
		{
			// Server
			m_Clas[ind].Init(_T("SRVRITEM"),_T("SrvrItem"),_T("COleServerItem"),TRUE,_T("SrvrItem"));

			ind++;
	
			//InPlaceFrame
			m_Clas[ind].Init(_T("IPFRAME"), _T("InPlaceFrame"),_T("COleIPFrameWnd"),FALSE,_T("IpFrame"));

			ind++;
		}
	}
	else if(IsMacroDefine(_T("MINI_SERVER")))
	{
		if(IsMacroDefine(_T("ACTIVE_DOC_SERVER")))
		{
			// Server
			m_Clas[ind].Init(_T("SRVRITEM"),_T("SrvrItem"),_T("CDocObjectServerItem"),TRUE,_T("SrvrItem"));

			ind++;

			//InPlaceFrame
			m_Clas[ind].Init(_T("IPFRAME"), _T("InPlaceFrame"),_T("COleDocIPFrameWnd"),FALSE,_T("IpFrame"));

			ind++;
		}
		else
		{
			// Server
			m_Clas[ind].Init(_T("SRVRITEM"),_T("SrvrItem"),_T("COleServerItem"),TRUE,_T("SrvrItem"));

			ind++;

			//InPlaceFrame
			m_Clas[ind].Init(_T("IPFRAME"), _T("InPlaceFrame"),_T("COleIPFrameWnd"),FALSE,_T("IpFrame"));

			ind++;
		}
	}
	else if(IsMacroDefine(_T("CONTAINER")))
	{
		// Container
		if(IsMacroDefine(_T("ACTIVE_DOC_CONTAINER")))
			m_Clas[ind].Init(_T("CNTRITEM"),_T("CntrItem"),_T("COleDocObjectItem"),TRUE,_T("CntrItem"));
		else
			m_Clas[ind].Init(_T("CNTRITEM"),_T("CntrItem"),_T("COleClientItem"),TRUE,_T("CntrItem"));

		ind++;
	}

// Explorer
	if(IsMacroDefine(_T("PROJTYPE_EXPLORER")))
	{
		// LeftView
		m_Clas[ind].Init(_T("TREEVIEW"), _T("LeftView"),_T("CTreeView"), FALSE);

		ind++;
	}

	m_Clas.SetSize(ind);
}
