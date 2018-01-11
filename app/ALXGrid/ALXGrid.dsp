# Microsoft Developer Studio Project File - Name="ALXGrid" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ALXGrid - Win32 Debug static
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ALXGrid.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ALXGrid - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\ALXGridR.lib"

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\\" /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Lib\ALXGridD.lib"

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Pseudo_Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ALXGrid___Win32_Pseudo_Debug"
# PROP BASE Intermediate_Dir "ALXGrid___Win32_Pseudo_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Pseudo_Debug"
# PROP Intermediate_Dir "Pseudo_Debug"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\ALXGridR.lib"
# ADD LIB32 /nologo /out:"Lib\ALXGridR.lib"

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Release static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ALXGrid___Win32_Release_static"
# PROP BASE Intermediate_Dir "ALXGrid___Win32_Release_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Static"
# PROP Intermediate_Dir "Release_Static"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\ALXGridR.lib"
# ADD LIB32 /nologo /out:"Lib\ALXGridRs.lib"

!ELSEIF  "$(CFG)" == "ALXGrid - Win32 Debug static"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ALXGrid___Win32_Debug_static"
# PROP BASE Intermediate_Dir "ALXGrid___Win32_Debug_static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Static"
# PROP Intermediate_Dir "Debug_Static"
# PROP Target_Dir ""
LINK32=link.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Lib\ALXGridD.lib"
# ADD LIB32 /nologo /out:"Lib\ALXGridDs.lib"

!ENDIF 

# Begin Target

# Name "ALXGrid - Win32 Release"
# Name "ALXGrid - Win32 Debug"
# Name "ALXGrid - Win32 Pseudo_Debug"
# Name "ALXGrid - Win32 Release static"
# Name "ALXGrid - Win32 Debug static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Src\ALXButtonCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXCellCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXCellMetrics.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXComboBoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\ALXGrid.rc
# End Source File
# Begin Source File

SOURCE=.\Src\ALXGridCore.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXGridDefinitions.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXGridView.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ALXSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SRC\XGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SRC\XGridView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Include\ALXButtonCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXCellCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXCellMetrics.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXComboBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXGrid.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXGrid.inl
# End Source File
# Begin Source File

SOURCE=.\Include\ALXGridCore.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXGridCore.inl
# End Source File
# Begin Source File

SOURCE=.\Include\ALXGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXGridDefinitions.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXGridView.h
# End Source File
# Begin Source File

SOURCE=.\Include\ALXSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\Include\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\XGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\XGridView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Grid.ico
# End Source File
# Begin Source File

SOURCE=.\res\GridDoc.ico
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\res\HResize.cur
# End Source File
# Begin Source File

SOURCE=.\res\HResize.cur
# End Source File
# Begin Source File

SOURCE=.\Src\res\HResize.cur
# End Source File
# Begin Source File

SOURCE=.\INCLUDE\res\icon1.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
