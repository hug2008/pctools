// ALXGrid.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "ALXGridwz.h"
#include "ALXGridaw.h"

#ifdef _PSEUDO_DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE ALXGridDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("ALXGRID.AWX Initializing!\n");
		
		// Extension DLL one-time initialization
		AfxInitExtensionModule(ALXGridDLL, hInstance);

		// Insert this DLL into the resource chain
		new CDynLinkLibrary(ALXGridDLL);

		// Register this custom AppWizard with MFCAPWZ.DLL
		SetCustomAppWizClass(&ALXAppWz);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("ALXGRID.AWX Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(ALXGridDLL);
	}
	return 1;   // ok
}
