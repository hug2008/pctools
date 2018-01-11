; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=ZebraPrinter
AppVerName=ZebraPrinter_D1.0.5
AppPublisher=Desay Electronics Co.,LTD
AppPublisherURL=http://www.desayele.net/
AppSupportURL=http://www.desayele.net/
AppUpdatesURL=http://www.desayele.net/
DefaultDirName=DesayEle\ZebraPrinter
DefaultGroupName=ZebraPrinter
OutputBaseFilename=zpSetup
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "E:\CvsRoot\ZebraPrinter\bin\ZebraPrinter_D.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CvsRoot\ZebraPrinter\bin\mfc71d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CvsRoot\ZebraPrinter\bin\msvcp71d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CvsRoot\ZebraPrinter\bin\msvcr71d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CvsRoot\ZebraPrinter\bin\ZebraPrinter.mdb"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CvsRoot\ZebraPrinter\bin\ZebraPrinter User Manual.pdf"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\CvsRoot\ZebraPrinter\bin\SpImei.prn"; DestDir: "{app}"; Flags: ignoreversion
;Source: "E:\CvsRoot\ZebraPrinter\bin\EmEditor.rar"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\ZebraPrinter"; Filename: "{app}\ZebraPrinter_D.exe"
Name: "{group}\User's Manual"; Filename: "{app}\ZebraPrinter User Manual.pdf"
Name: "{group}\{cm:UninstallProgram,}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\ZebraPrinter"; Filename: "{app}\ZebraPrinter_D.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\ZebraPrinter"; Filename: "{app}\ZebraPrinter_D.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\ZebraPrinter_D.exe"; Description: "{cm:LaunchProgram,ZebraPrinter}"; Flags: nowait postinstall skipifsilent

