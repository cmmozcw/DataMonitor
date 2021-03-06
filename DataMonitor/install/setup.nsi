# Auto-generated by EclipseNSIS Script Wizard
# 2012-12-19 12:42:46

Name 测井地面系统

# General Symbol Definitions
!define REGKEY "SOFTWARE\$(^Name)"
!define VERSION 1.14.9.4
!define COMPANY 力擎数控
!define URL ""

# MUI Symbol Definitions
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_STARTMENUPAGE_REGISTRY_ROOT HKLM
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_REGISTRY_KEY ${REGKEY}
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME StartMenuGroup
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "力擎数控"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

# Included files
!include Sections.nsh
!include MUI2.nsh

# Variables
Var StartMenuGroup

# Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuGroup
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE SimpChinese

# Installer attributes
OutFile setup.exe
InstallDir $PROGRAMFILES\力擎数控
CRCCheck on
XPStyle on
ShowInstDetails show
VIProductVersion 4.0.4.828
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} ProductName 测井地面系统
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} ProductVersion "${VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} CompanyName "${COMPANY}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} FileVersion "${VERSION}"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} FileDescription ""
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} LegalCopyright ""
InstallDirRegKey HKLM "${REGKEY}" Path
ShowUninstDetails show

# Installer sections
Section -Main SEC0000
    SetShellVarContext all
    SetOutPath $INSTDIR
    SetOverwrite ifnewer
    File ..\Release\DataMonitor.exe
    File ..\Release\PlotEditor.exe
    CreateDirectory $INSTDIR\Config
    CreateDirectory $INSTDIR\Data
    CreateDirectory $INSTDIR\Jobs
    CreateDirectory $INSTDIR\Log
    CreateDirectory $INSTDIR\Temp
    CreateDirectory $INSTDIR\Tools
#    SetOutPath $INSTDIR\Config
#    File /r Config\*.ini
#    SetOutPath $INSTDIR\DeviceInterface
#    File /r DeviceInterface\*.exe
#    File /r DeviceInterface\*.dll
#    File /r DeviceInterface\*.apk
#    SetOutPath $INSTDIR\Skins
#    File /r Skins\*.png
    SetOutPath $SMPROGRAMS\$StartMenuGroup
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\测井地面系统.lnk" $INSTDIR\DataMonitor.exe
    WriteRegStr HKLM "${REGKEY}\Components" Main 1
SectionEnd

Section -post SEC0001
    SetShellVarContext all
    WriteRegStr HKLM "${REGKEY}" Path $INSTDIR
    SetOutPath $INSTDIR
    WriteUninstaller $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    SetOutPath $SMPROGRAMS\$StartMenuGroup
    CreateShortcut "$SMPROGRAMS\$StartMenuGroup\$(^UninstallLink).lnk" $INSTDIR\uninstall.exe
    !insertmacro MUI_STARTMENU_WRITE_END
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayName "$(^Name)"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayVersion "${VERSION}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" Publisher "${COMPANY}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" DisplayIcon $INSTDIR\uninstall.exe
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" UninstallString $INSTDIR\uninstall.exe
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoModify 1
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" NoRepair 1
SectionEnd

# Macro for selecting uninstaller sections
!macro SELECT_UNSECTION SECTION_NAME UNSECTION_ID
    Push $R0
    ReadRegStr $R0 HKLM "${REGKEY}\Components" "${SECTION_NAME}"
    StrCmp $R0 1 0 next${UNSECTION_ID}
    !insertmacro SelectSection "${UNSECTION_ID}"
    GoTo done${UNSECTION_ID}
next${UNSECTION_ID}:
    !insertmacro UnselectSection "${UNSECTION_ID}"
done${UNSECTION_ID}:
    Pop $R0
!macroend

# Uninstaller sections
Section /o -un.Main UNSEC0000
    SetShellVarContext all
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\测井地面系统.lnk"
    RmDir /r /REBOOTOK $INSTDIR\Config
    RmDir /r /REBOOTOK $INSTDIR\Data
    RmDir /r /REBOOTOK $INSTDIR\Jobs
    RmDir /r /REBOOTOK $INSTDIR\Log
    RmDir /r /REBOOTOK $INSTDIR\Temp
    RmDir /r /REBOOTOK $INSTDIR\Tools
    Delete /REBOOTOK $INSTDIR\DataMonitor.exe
    Delete /REBOOTOK $INSTDIR\PlotEditor.exe
    DeleteRegValue HKLM "${REGKEY}\Components" Main
SectionEnd

Section -un.post UNSEC0001
    SetShellVarContext all
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
    Delete /REBOOTOK "$SMPROGRAMS\$StartMenuGroup\$(^UninstallLink).lnk"
    Delete /REBOOTOK $INSTDIR\uninstall.exe
    DeleteRegValue HKLM "${REGKEY}" StartMenuGroup
    DeleteRegValue HKLM "${REGKEY}" Path
    DeleteRegKey /IfEmpty HKLM "${REGKEY}\Components"
    DeleteRegKey /IfEmpty HKLM "${REGKEY}"
    RmDir /REBOOTOK $SMPROGRAMS\$StartMenuGroup
    RmDir /REBOOTOK $INSTDIR
SectionEnd

# Installer functions
Function .onInit
    InitPluginsDir
FunctionEnd

# Uninstaller functions
Function un.onInit
    ReadRegStr $INSTDIR HKLM "${REGKEY}" Path
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuGroup
    !insertmacro SELECT_UNSECTION Main ${UNSEC0000}
FunctionEnd

# Installer Language Strings
# TODO Update the Language Strings with the appropriate translations.

LangString ^UninstallLink ${LANG_SIMPCHINESE} "卸载 $(^Name)"
