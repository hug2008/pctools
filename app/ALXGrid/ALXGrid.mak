# Microsoft Developer Studio Generated NMAKE File, Based on ALXGrid.dsp
!IF "$(CFG)" == ""
CFG=ALXGrid - Win32 Debug static
!MESSAGE No configuration specified. Defaulting to ALXGrid - Win32 Debug static.
!ENDIF 

!IF "$(CFG)" != "ALXGrid - Win32 Release" && "$(CFG)" != "ALXGrid - Win32 Debug" && "$(CFG)" != "ALXGrid - Win32 Pseudo_Debug" && "$(CFG)" != "ALXGrid - Win32 Release static" && "$(CFG)" != "ALXGrid - Win32 Debug static"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ALXGrid.mak" CFG="ALXGrid - Win32 Debug static"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ALXGrid - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ALXGrid - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ALXGrid - Win32 Pseudo_Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ALXGrid - Win32 Release static" (based on "Win32 (x86) Static Library")
!MESSAGE "ALXGrid - Win32 Debug static" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ALXGrid - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\Lib\ALXGridR.lib"


CLEAN :
	-@erase "$(INTDIR)\ALXButtonCtrl.obj"
	-@erase "$(INTDIR)\ALXCellCtrl.obj"
	-@erase "$(INTDIR)\ALXCellMetrics.obj"
	-@erase "$(INTDIR)\ALXComboBoxCtrl.obj"
	-@erase "$(INTDIR)\ALXEditCtrl.obj"
	-@erase "$(INTDIR)\ALXGrid.obj"
	-@erase "$(INTDIR)\ALXGrid.pch"
	-@erase "$(INTDIR)\ALXGridCore.obj"
	-@erase "$(INTDIR)\ALXGridCtrl.obj"
	-@erase "$(INTDIR)\ALXGridDefinitions.obj"
	-@erase "$(INTDIR)\ALXGridView.obj"
	-@erase "$(INTDIR)\ALXSplitterWnd.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\Lib\ALXGridR.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\ALXGrid.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ALXGrid.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"Lib\ALXGridR.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ALXButtonCtrl.obj" \
	"$(INTDIR)\ALXCellCtrl.obj" \
	"$(INTDIR)\ALXCellMetrics.obj" \
	"$(INTDIR)\ALXComboBoxCtrl.obj" \
	"$(INTDIR)\ALXEditCtrl.obj" \
	"$(INTDIR)\ALXGrid.obj" \
	"$(INTDIR)\ALXGridCore.obj" \
	"$(INTDIR)\ALXGridCtrl.obj" \
	"$(INTDIR)\ALXGridDefinitions.obj" \
	"$(INTDIR)\ALXGridView.obj" \
	"$(INTDIR)\ALXSplitterWnd.obj" \
	"$(INTDIR)\StdAfx.obj"

".\Lib\ALXGridR.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\Lib\ALXGridD.lib"


CLEAN :
	-@erase "$(INTDIR)\ALXButtonCtrl.obj"
	-@erase "$(INTDIR)\ALXCellCtrl.obj"
	-@erase "$(INTDIR)\ALXCellMetrics.obj"
	-@erase "$(INTDIR)\ALXComboBoxCtrl.obj"
	-@erase "$(INTDIR)\ALXEditCtrl.obj"
	-@erase "$(INTDIR)\ALXGrid.obj"
	-@erase "$(INTDIR)\ALXGrid.pch"
	-@erase "$(INTDIR)\ALXGridCore.obj"
	-@erase "$(INTDIR)\ALXGridCtrl.obj"
	-@erase "$(INTDIR)\ALXGridDefinitions.obj"
	-@erase "$(INTDIR)\ALXGridView.obj"
	-@erase "$(INTDIR)\ALXSplitterWnd.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\Lib\ALXGridD.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\ALXGrid.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ALXGrid.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"Lib\ALXGridD.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ALXButtonCtrl.obj" \
	"$(INTDIR)\ALXCellCtrl.obj" \
	"$(INTDIR)\ALXCellMetrics.obj" \
	"$(INTDIR)\ALXComboBoxCtrl.obj" \
	"$(INTDIR)\ALXEditCtrl.obj" \
	"$(INTDIR)\ALXGrid.obj" \
	"$(INTDIR)\ALXGridCore.obj" \
	"$(INTDIR)\ALXGridCtrl.obj" \
	"$(INTDIR)\ALXGridDefinitions.obj" \
	"$(INTDIR)\ALXGridView.obj" \
	"$(INTDIR)\ALXSplitterWnd.obj" \
	"$(INTDIR)\StdAfx.obj"

".\Lib\ALXGridD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Pseudo_Debug"

OUTDIR=.\Pseudo_Debug
INTDIR=.\Pseudo_Debug

ALL : ".\Lib\ALXGridR.lib"


CLEAN :
	-@erase "$(INTDIR)\ALXButtonCtrl.obj"
	-@erase "$(INTDIR)\ALXCellCtrl.obj"
	-@erase "$(INTDIR)\ALXCellMetrics.obj"
	-@erase "$(INTDIR)\ALXComboBoxCtrl.obj"
	-@erase "$(INTDIR)\ALXEditCtrl.obj"
	-@erase "$(INTDIR)\ALXGrid.obj"
	-@erase "$(INTDIR)\ALXGrid.pch"
	-@erase "$(INTDIR)\ALXGridCore.obj"
	-@erase "$(INTDIR)\ALXGridCtrl.obj"
	-@erase "$(INTDIR)\ALXGridDefinitions.obj"
	-@erase "$(INTDIR)\ALXGridView.obj"
	-@erase "$(INTDIR)\ALXSplitterWnd.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\Lib\ALXGridR.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ALXGrid.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ALXGrid.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"Lib\ALXGridR.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ALXButtonCtrl.obj" \
	"$(INTDIR)\ALXCellCtrl.obj" \
	"$(INTDIR)\ALXCellMetrics.obj" \
	"$(INTDIR)\ALXComboBoxCtrl.obj" \
	"$(INTDIR)\ALXEditCtrl.obj" \
	"$(INTDIR)\ALXGrid.obj" \
	"$(INTDIR)\ALXGridCore.obj" \
	"$(INTDIR)\ALXGridCtrl.obj" \
	"$(INTDIR)\ALXGridDefinitions.obj" \
	"$(INTDIR)\ALXGridView.obj" \
	"$(INTDIR)\ALXSplitterWnd.obj" \
	"$(INTDIR)\StdAfx.obj"

".\Lib\ALXGridR.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Release static"

OUTDIR=.\Release_Static
INTDIR=.\Release_Static

ALL : ".\Lib\ALXGridRs.lib"


CLEAN :
	-@erase "$(INTDIR)\ALXButtonCtrl.obj"
	-@erase "$(INTDIR)\ALXCellCtrl.obj"
	-@erase "$(INTDIR)\ALXCellMetrics.obj"
	-@erase "$(INTDIR)\ALXComboBoxCtrl.obj"
	-@erase "$(INTDIR)\ALXEditCtrl.obj"
	-@erase "$(INTDIR)\ALXGrid.obj"
	-@erase "$(INTDIR)\ALXGrid.pch"
	-@erase "$(INTDIR)\ALXGridCore.obj"
	-@erase "$(INTDIR)\ALXGridCtrl.obj"
	-@erase "$(INTDIR)\ALXGridDefinitions.obj"
	-@erase "$(INTDIR)\ALXGridView.obj"
	-@erase "$(INTDIR)\ALXSplitterWnd.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\Lib\ALXGridRs.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ALXGrid.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ALXGrid.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"Lib\ALXGridRs.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ALXButtonCtrl.obj" \
	"$(INTDIR)\ALXCellCtrl.obj" \
	"$(INTDIR)\ALXCellMetrics.obj" \
	"$(INTDIR)\ALXComboBoxCtrl.obj" \
	"$(INTDIR)\ALXEditCtrl.obj" \
	"$(INTDIR)\ALXGrid.obj" \
	"$(INTDIR)\ALXGridCore.obj" \
	"$(INTDIR)\ALXGridCtrl.obj" \
	"$(INTDIR)\ALXGridDefinitions.obj" \
	"$(INTDIR)\ALXGridView.obj" \
	"$(INTDIR)\ALXSplitterWnd.obj" \
	"$(INTDIR)\StdAfx.obj"

".\Lib\ALXGridRs.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Debug static"

OUTDIR=.\Debug_Static
INTDIR=.\Debug_Static

ALL : ".\Lib\ALXGridDs.lib"


CLEAN :
	-@erase "$(INTDIR)\ALXButtonCtrl.obj"
	-@erase "$(INTDIR)\ALXCellCtrl.obj"
	-@erase "$(INTDIR)\ALXCellMetrics.obj"
	-@erase "$(INTDIR)\ALXComboBoxCtrl.obj"
	-@erase "$(INTDIR)\ALXEditCtrl.obj"
	-@erase "$(INTDIR)\ALXGrid.obj"
	-@erase "$(INTDIR)\ALXGrid.pch"
	-@erase "$(INTDIR)\ALXGridCore.obj"
	-@erase "$(INTDIR)\ALXGridCtrl.obj"
	-@erase "$(INTDIR)\ALXGridDefinitions.obj"
	-@erase "$(INTDIR)\ALXGridView.obj"
	-@erase "$(INTDIR)\ALXSplitterWnd.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\Lib\ALXGridDs.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ALXGrid.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ALXGrid.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"Lib\ALXGridDs.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ALXButtonCtrl.obj" \
	"$(INTDIR)\ALXCellCtrl.obj" \
	"$(INTDIR)\ALXCellMetrics.obj" \
	"$(INTDIR)\ALXComboBoxCtrl.obj" \
	"$(INTDIR)\ALXEditCtrl.obj" \
	"$(INTDIR)\ALXGrid.obj" \
	"$(INTDIR)\ALXGridCore.obj" \
	"$(INTDIR)\ALXGridCtrl.obj" \
	"$(INTDIR)\ALXGridDefinitions.obj" \
	"$(INTDIR)\ALXGridView.obj" \
	"$(INTDIR)\ALXSplitterWnd.obj" \
	"$(INTDIR)\StdAfx.obj"

".\Lib\ALXGridDs.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ALXGrid.dep")
!INCLUDE "ALXGrid.dep"
!ELSE 
!MESSAGE Warning: cannot find "ALXGrid.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ALXGrid - Win32 Release" || "$(CFG)" == "ALXGrid - Win32 Debug" || "$(CFG)" == "ALXGrid - Win32 Pseudo_Debug" || "$(CFG)" == "ALXGrid - Win32 Release static" || "$(CFG)" == "ALXGrid - Win32 Debug static"
SOURCE=.\Src\ALXButtonCtrl.cpp

"$(INTDIR)\ALXButtonCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXCellCtrl.cpp

"$(INTDIR)\ALXCellCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXCellMetrics.cpp

"$(INTDIR)\ALXCellMetrics.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXComboBoxCtrl.cpp

"$(INTDIR)\ALXComboBoxCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXEditCtrl.cpp

"$(INTDIR)\ALXEditCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXGrid.cpp

"$(INTDIR)\ALXGrid.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXGridCore.cpp

"$(INTDIR)\ALXGridCore.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXGridCtrl.cpp

"$(INTDIR)\ALXGridCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXGridDefinitions.cpp

"$(INTDIR)\ALXGridDefinitions.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXGridView.cpp

"$(INTDIR)\ALXGridView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\Src\ALXSplitterWnd.cpp

"$(INTDIR)\ALXSplitterWnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\ALXGrid.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ALXGrid - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\ALXGrid.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ALXGrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Fp"$(INTDIR)\ALXGrid.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ALXGrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Pseudo_Debug"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\ALXGrid.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ALXGrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Release static"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ALXGrid.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ALXGrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Debug static"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\ALXGrid.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\ALXGrid.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

