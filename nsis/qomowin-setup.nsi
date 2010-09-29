;--------------------------------
;Include
  !include "MUI2.nsh"

;--------------------------------
;Define
  !define PRODUCT_PACKAGE "@PACKAGE@"
  !define PRODUCT_VERSION "@VERSION@"
  !define VERSION_DOT "@WINVERSION@"
  !define MUI_ICON "${NSISDIR}\\Contrib\Graphics\Icons\orange-install.ico"
  !define MUI_UNICON "${NSISDIR}\\Contrib\Graphics\Icons\orange-uninstall.ico"

;--------------------------------
;General
  Name "${PRODUCT_PACKAGE}"
  OutFile "${PRODUCT_PACKAGE}-${PRODUCT_VERSION}.exe"

  InstallDir "$PROGRAMFILES\${PRODUCT_PACKAGE}"

  InstallDirRegKey HKCU "Software\${PRODUCT_PACKAGE}" ""
  RequestExecutionLevel admin
  SetOverwrite on
  SetCompress auto
  SetCompressor /SOLID lzma
  ; 设置数据块优化
  SetDatablockOptimize on
  ; 设置数据写入时间
  SetDateSave on
  BrandingText /TRIMCENTER "http://www.linux-ren.org"

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
  !insertmacro MUI_PAGE_COMPONENTS
  ;!insertmacro MUI_PAGE_DIRECTORY
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
;Version Information

  VIProductVersion "${VERSION_DOT}"
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "ProductName" $(_lang_productname)
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "Comments" $(_lang_comments)
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "CompanyName" $(_lang_companyname)
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "LegalTrademarks" $(_lang_legaltrademarks)
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "LegalCopyright" $(_lang_legalcopyright)
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "FileDescription" ${_lang_filedescription}
  VIAddVersionKey /LANG=${LANG_ENGLISH}     "FileVersion" "${PRODUCT_VERSION}"
;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Sections

Section "Core Section" SecCore

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
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

  ;Create shortcuts

  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut "$DESKTOP\qomowin.lnk" "$INSTDIR\qomowin.exe" \
	  "" "" "" SW_SHOWNORMAL "" $(_lang_Qomowin)
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\qomowin.lnk" "$INSTDIR\qomowin.exe" \
	  "" "" "" SW_SHOWNORMAL "" $(_lang_Qomowin)
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe" \
	  "" "" "" SW_SHOWNORMAL ""  $(_lang_Uninstall)

  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;--------------------------------
;Installer Functions

;Function .onInit
;
;  !insertmacro MUI_LANGDLL_DISPLAY
;
;FunctionEnd

;--------------------------------
;Descriptions

  ;USE A LANGUAGE STRING IF YOU WANT YOUR DESCRIPTIONS TO BE LANGAUGE SPECIFIC

  ;Language strings

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} $(_lang_SecCore)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir /r "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\qomowin.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\${PRODUCT_PACKAGE}"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd
