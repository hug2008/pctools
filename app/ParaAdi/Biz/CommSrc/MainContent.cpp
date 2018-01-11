// MainContent.cpp : implementation file
//

#include "stdafx.h"
#include "MainContent.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainContent

CMainContent::CMainContent()
{
	m_nMonitorDelay=MONITORDETECT;
	m_sWorkPath=GetWorkPath();
	bIfIsWinNt=TRUE;
	m_sOldPass="";
	bIfHaveInit=FALSE;
	m_pFIFOThread=NULL;
	m_pCommThread=NULL;

}

CMainContent::~CMainContent()
{
	
}


BEGIN_MESSAGE_MAP(CMainContent, CStatic)
	//{{AFX_MSG_MAP(CMainContent)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainContent message handlers



CString CMainContent::GetWorkPath()
{
   CString strPath="";
   GetModuleFileName(NULL,strPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
   return GetPathFromStr(strPath);
}

CString CMainContent::GetPathFromStr(CString s)
{
   CString tempStr,strPath="";   
   tempStr=s;
   
   int pos=0;   
   while(tempStr.Find("\\")!=-1){
	   pos=tempStr.Find("\\");
	   strPath+=tempStr.Left(pos+1);
	   tempStr=tempStr.Right(tempStr.GetLength()-pos-1);
   }	
   return strPath;
}
