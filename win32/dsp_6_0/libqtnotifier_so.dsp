# Microsoft Developer Studio Project File - Name="libqtnotifier_so" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libqtnotifier_so - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libqtnotifier_so.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libqtnotifier_so.mak" CFG="libqtnotifier_so - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libqtnotifier_so - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libqtnotifier_so - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libqtnotifier_so - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBQTNOTIFIER_SO_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBQTNOTIFIER_SO_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "libqtnotifier_so - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libqtnotifier_so"
# PROP Intermediate_Dir "libqtnotifier_so"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBQTNOTIFIER_SO_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GR /ZI /Od /I "\qt\include" /I "..\..\src" /I "\libxslt" /I "..\..\..\libxslt\include" /I "..\..\..\libxml2\include" /I "..\..\..\iconv\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "QT_DLL" /D "UNICODE" /D "QT_THREAD_SUPPORT" /D "_USRDLL" /D "XSLDBG_SO_EXPORTS" /D "USE_XSLDBG_AS_THREAD" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib winmm.lib wsock32.lib imm32.lib wsock32.lib winmm.lib \qt\lib\qt-mt230nc.lib \qt\lib\qtmain.lib libxsldbg.lib libxml2.lib libexslt.lib libxslt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept /libpath:"..\..\..\xsldbg\win32\dsp\libxsldbg" /libpath:"\libxslt\win32\dsp\libexslt" /libpath:"\libxslt\win32\dsp\libxslt" /libpath:"..\..\..\libxslt\lib" /libpath:"..\..\..\libxml2\lib" /libpath:"..\..\..\iconv\lib"

!ENDIF 

# Begin Target

# Name "libqtnotifier_so - Win32 Release"
# Name "libqtnotifier_so - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\libqtnotifier\qtnotifier2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\libqtnotifier\xsldbgdebuggerbase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\libqtnotifier\xsldbgdebuggerbase.moc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\libqtnotifier\xsldbgevent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\libqtnotifier\xsldbgnotifier.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\libqtnotifier\xsldbgthread_win32.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\libxsldbg\qtnotifier2.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgdebuggerbase.h

!IF  "$(CFG)" == "libqtnotifier_so - Win32 Release"

!ELSEIF  "$(CFG)" == "libqtnotifier_so - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=..\..\src\libxsldbg\xsldbgdebuggerbase.h
InputName=xsldbgdebuggerbase

"..\..\src\libqtnotifier\$(InputName).moc.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	\qt\bin\moc ..\..\src\libxsldbg\$(InputName).h -o ..\..\src\libqtnotifier\$(InputName).moc.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgevent.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
