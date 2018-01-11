; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRecordDlg
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "record.h"

ClassCount=9
Class1=CRecordApp
Class2=CRecordDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CRecordSound
Class5=CPlaySound
Class6=CWriteSoundFile
Class7=CMoveCursor
Class8=CProcessSoundBlock
Resource3=IDD_TRAIN
Class9=CPlayMMSound
Resource4=IDD_RECORD_DIALOG

[CLS:CRecordApp]
Type=0
HeaderFile=record.h
ImplementationFile=record.cpp
Filter=N

[CLS:CRecordDlg]
Type=0
HeaderFile=recordDlg.h
ImplementationFile=recordDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BUTTON2

[CLS:CAboutDlg]
Type=0
HeaderFile=recordDlg.h
ImplementationFile=recordDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_RECORD_DIALOG]
Type=1
Class=CRecordDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_STATIC_ZEROCROSSINGS,static,1342308352
Control5=IDC_BUTTON2,button,1342242816

[CLS:CRecordSound]
Type=0
HeaderFile=RecordSound.h
ImplementationFile=RecordSound.cpp
BaseClass=CWinThread
Filter=N
LastObject=CRecordSound

[CLS:CPlaySound]
Type=0
HeaderFile=PlaySound.h
ImplementationFile=PlaySound.cpp
BaseClass=CWinThread
Filter=N
LastObject=CPlaySound

[CLS:CWriteSoundFile]
Type=0
HeaderFile=WriteSoundFile.h
ImplementationFile=WriteSoundFile.cpp
BaseClass=CWinThread
Filter=N
LastObject=CWriteSoundFile

[CLS:CMoveCursor]
Type=0
HeaderFile=MoveCursor.h
ImplementationFile=MoveCursor.cpp
BaseClass=CWinThread
Filter=N
LastObject=CMoveCursor

[CLS:CProcessSoundBlock]
Type=0
HeaderFile=ProcessSoundBlock.h
ImplementationFile=ProcessSoundBlock.cpp
BaseClass=CWinThread
Filter=N
LastObject=CProcessSoundBlock

[DLG:IDD_TRAIN]
Type=1
Class=?
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_UP,button,1342242816
Control4=IDC_DOWN,button,1342242816
Control5=IDC_RIGHT,button,1342242816
Control6=IDC_LEFT,button,1342242816
Control7=IDC_CLICK,button,1342242816
Control8=IDC_STOPTRAINING,button,1342242816

[CLS:CPlayMMSound]
Type=0
HeaderFile=PlayMMSound.h
ImplementationFile=PlayMMSound.cpp
BaseClass=CWinThread
Filter=N
LastObject=CPlayMMSound

