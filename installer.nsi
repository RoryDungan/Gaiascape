!include "FileFunc.nsh"
!include "LogicLib.nsh"
!include "MUI2.nsh"

RequestExecutionLevel admin

InstallDir "$PROGRAMFILES\Gaiascape"
#LicenseData "license.txt"
Name "Gaiascape"
Icon "media\icon.ico"
OutFile "Gaiascape-setup.exe"

!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
#Page license
#Page directory
#Page instfiles

!define APPNAME "Gaiascape"
!define DESCRIPTION "A short description goes here"


!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
        MessageBox mb_iconstop "Administrator rights required!"
        SetErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        Quit
${EndIf}
!macroend
 
Function .onInit
	SetShellVarContext all
	!insertmacro VerifyUserIsAdmin
FunctionEnd
 
Section "install"
	# Copy files...
	SetOutPath $INSTDIR
	File Gaiascape.exe
	File lib\libgcc_s_dw2-1.dll
	File lib\libstdc++-6.dll
	File lib\libwinpthread-1.dll
	File C:\Qt\4.8.6\bin\QtCore4.dll
	File C:\Qt\4.8.6\bin\QtGui4.dll
	File C:\Qt\4.8.6\bin\QtOpenGL4.dll
	File lib\OgreMain.dll
	File lib\OgrePaging.dll
	File lib\OgreTerrain.dll
	File lib\libboost_date_time-mgw48-mt-1_60.dll
	File lib\libboost_system-mgw48-mt-1_60.dll
	File lib\libboost_thread-mgw48-mt-1_60.dll
	File media\icon.ico
	File plugins.cfg
	SetOutPath $INSTDIR\plugins
	File plugins\RenderSystem_GL.dll
	File plugins\Plugin_CgProgramManager.dll
	File plugins\cg.dll
	SetOutPath $INSTDIR\media\models
	File media\models\sphere.mesh
	SetOutPath $INSTDIR\media\textures
	File media\textures\clouds.jpg
	File media\textures\terrain.png
	SetOutPath $INSTDIR\media\textures\nvidia
	File media\textures\nvidia\dirt_grayrocky_diffusespecular.dds
	File media\textures\nvidia\dirt_grayrocky_normalheight.dds
	File media\textures\nvidia\grass_green-01_diffusespecular.dds
	File media\textures\nvidia\grass_green-01_normalheight.dds
	File media\textures\nvidia\growth_weirdfungus-03_diffusespecular.dds
	File media\textures\nvidia\growth_weirdfungus-03_normalheight.dds
	File media\textures\nvidia\TextureUsageAgreement.txt
	
	# Create shortcuts and uninstaller
    SetOutPath "$INSTDIR"
	CreateShortCut "$SMPROGRAMS\Gaiascape.lnk" "$INSTDIR\Gaiascape.exe"

	WriteUninstaller $INSTDIR\uninstaller.exe
	
	# Write registry settings to add the program to the Add/Remove programs list in the Windows control panel
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstaller.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstaller.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$\"$INSTDIR\icon.ico$\""
	${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
	IntFmt $0 "0x%08X" $0
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "EstimatedSize" "$0"
SectionEnd

# Uninstaller
 
function un.onInit
	SetShellVarContext all
 
	#Verify the uninstaller - last chance to back out
	MessageBox MB_OKCANCEL "Permanantly remove ${APPNAME}?" IDOK next
		Abort
	next:
	!insertmacro VerifyUserIsAdmin
functionEnd

Section "uninstall"
	
	Delete $INSTDIR\Gaiascape.exe
	Delete $INSTDIR\libgcc_s_dw2-1.dll
	Delete $INSTDIR\libstdc++-6.dll
	Delete $INSTDIR\libwinpthread-1.dll
	Delete $INSTDIR\QtCore4.dll
	Delete $INSTDIR\QtGui4.dll
	Delete $INSTDIR\QtOpenGL4.dll
	Delete $INSTDIR\OgreMain.dll
	Delete $INSTDIR\OgrePaging.dll
	Delete $INSTDIR\OgreTerrain.dll
	Delete $INSTDIR\libboost_date_time-mgw48-mt-1_60.dll
	Delete $INSTDIR\libboost_thread-mgw48-mt-1_60.dll
	Delete $INSTDIR\libboost_system-mgw48-mt-1_60.dll
	Delete $INSTDIR\icon.ico
	Delete $INSTDIR\plugins.cfg
	Delete $INSTDIR\Plugins\RenderSystem_GL.dll
	Delete $INSTDIR\Plugins\Plugin_CgProgramManager.dll
	Delete $INSTDIR\Plugins\cg.dll
	Delete $INSTDIR\media\models\sphere.mesh
	Delete $INSTDIR\media\textures\clouds.jpg
	Delete $INSTDIR\media\textures\terrain.png
	Delete $INSTDIR\media\textures\nvidia\dirt_grayrocky_diffusespecular.dds
	Delete $INSTDIR\media\textures\nvidia\dirt_grayrocky_normalheight.dds
	Delete $INSTDIR\media\textures\nvidia\grass_green-01_diffusespecular.dds
	Delete $INSTDIR\media\textures\nvidia\grass_green-01_normalheight.dds
	Delete $INSTDIR\media\textures\nvidia\growth_weirdfungus-03_diffusespecular.dds
	Delete $INSTDIR\media\textures\nvidia\growth_weirdfungus-03_normalheight.dds
	Delete $INSTDIR\media\textures\nvidia\TextureUsageAgreement.txt
	Delete $SMPROGRAMS\Gaiascape.lnk
	RmDir $INSTDIR\media\textures\nvidia
	RmDir $INSTDIR\media\textures
	RmDir $INSTDIR\media\models
	Rmdir $INSTDIR\media
	RmDir $INSTDIR\plugins
	
	Delete $INSTDIR\uninstaller.exe
	RmDir $INSTDIR 
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
SectionEnd
