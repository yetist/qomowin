;--------------------------------
;Include
  !include "MUI2.nsh"

;--------------------------------
;Define
  !define PRODUCT_PACKAGE "@PACKAGE@"
  !define PRODUCT_VERSION "@VERSION@"
  !define VERSION_DOT "@WINVERSION@"
  !define HOME_URL "http://www.linux-ren.org"
  !define GIT_URL "http://git.linux-ren.org/qomowin"
  !define MUI_ICON "${NSISDIR}\\Contrib\Graphics\Icons\orange-install.ico"
  !define MUI_UNICON "${NSISDIR}\\Contrib\Graphics\Icons\orange-uninstall.ico"

;--------------------------------
;General
  Name "QomoWin"
  OutFile "${PRODUCT_PACKAGE}-${PRODUCT_VERSION}.exe"
  InstallDir "$PROGRAMFILES\${PRODUCT_PACKAGE}"
  InstallDirRegKey HKCU "Software\${PRODUCT_PACKAGE}" ""
  RequestExecutionLevel admin

  SetOverwrite on
  SetCompress auto
  SetCompressor /SOLID lzma
  SetDatablockOptimize on
  SetDateSave on
  BrandingText /TRIMCENTER "Powered by http://www.linux-ren.org "
;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings
  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\${PRODUCT_PACKAGE}" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !include "mylang.nsh"

;--------------------------------
;Reserve Files
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Version Information

  VIProductVersion "${VERSION_DOT}"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "ProductName" "QomoWin"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "Comments" "Qomo Installer"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "CompanyName" "Linux Ren Open Source Community"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "LegalTrademarks" "trademarks"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "LegalCopyright" "Copyright (r) Linux Ren Open Source Community"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "FileDescription" "Qomo Linux installer for Windows"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "FileVersion" "${PRODUCT_VERSION}"

  VIAddVersionKey /LANG=${LANG_SIMPCHINESE}     "ProductName" "QomoWin"
  VIAddVersionKey /LANG=${LANG_SIMPCHINESE}     "Comments" "Qomo Linux 安装程序"
  VIAddVersionKey /LANG=${LANG_SIMPCHINESE}     "CompanyName" "Linux人开源社区"
  VIAddVersionKey /LANG=${LANG_SIMPCHINESE}     "LegalTrademarks" "trademarks"
  VIAddVersionKey /LANG=${LANG_SIMPCHINESE}     "LegalCopyright" "Copyright (r) Linux人开源社区"
  VIAddVersionKey /LANG=${LANG_SIMPCHINESE}     "FileDescription" "Qomo Linux 安装程序"
  VIAddVersionKey /LANG=${LANG_SIMPCHINESE}     "FileVersion" "${PRODUCT_VERSION}"

;--------------------------------
;Installer Sections

Section

  SectionIn RO 

  SetOutPath $INSTDIR\winboot
  File "winboot\qomoldr"
  File "winboot\qomoldr.mbr"

  SetOutPath "$INSTDIR"
  File "qomowin.exe"
  File /r bin boot locale

  ;Store installation folder
  WriteRegStr HKCU "Software\${PRODUCT_PACKAGE}" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

  ;Create shortcuts
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut "$DESKTOP\$(_lang_qomowin).lnk" "$INSTDIR\qomowin.exe" \
	  "" "" "" SW_SHOWNORMAL "" $(_lang_qomowin_desc)
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\$(_lang_qomowin).lnk" "$INSTDIR\qomowin.exe" \
	  "" "" "" SW_SHOWNORMAL "" $(_lang_qomowin_desc)
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\$(_lang_uninstall).lnk" "$INSTDIR\uninstall.exe" \
	  "" "" "" SW_SHOWNORMAL ""  $(_lang_uninstall_desc)

  !insertmacro MUI_STARTMENU_WRITE_END

  ; Add uninstall information to Add/Remove Programs
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "DisplayName" "QomoWin"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "DisplayIcon" "$INSTDIR\qomowin.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "Publisher" "Linux Ren Open Source Comminuty"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "UrlInfoAbout" "${HOME_URL}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "HelpLink" "${HOME_URL}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "UrlUpdateInfo" "${GIT_URL}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "Readme" "$INSTDIR\README.html"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}" \
	  "UninstallString" "$INSTDIR\uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\uninstall.exe"

  RMDir /r "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$DESKTOP\$(_lang_qomowin).lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\$(_lang_qomowin).lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\$(_lang_uninstall).lnk"

  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\${PRODUCT_PACKAGE}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_PACKAGE}"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

Function .onInstSuccess
  MessageBox MB_YESNO $(_lang_runitnow) IDNO skip
  Exec "$INSTDIR\qomowin.exe"
  skip:
FunctionEnd

