# Microsoft Developer Studio Project File - Name="libxsldbg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libxsldbg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libxsldbg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libxsldbg.mak" CFG="libxsldbg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libxsldbg - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libxsldbg - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libxsldbg - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "libxsldbg"
# PROP Intermediate_Dir "libxsldbg"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "HAVE_INCLUDE_FIX" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../src" /D "WIN32" /D "HAVE_INCLUDE_FIX" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libxsldbg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "libxsldbg"
# PROP Intermediate_Dir "libxsldbg"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "HAVE_INCLUDE_FIX" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /Od /I "..\..\src" /I "\libxslt" /I "..\..\..\libxslt\include" /I "..\..\..\libxml2\include" /I "..\..\..\iconv\include" /D "WIN32" /D "HAVE_INCLUDE_FIX" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /I /libxslt" /I /libxml2/include" /I /iconv/include" /GZ " " " /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libxsldbg - Win32 Release"
# Name "libxsldbg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\libxsldbg\arraylist.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\breakpoint.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\breakpoint_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\callstack.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\debug.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\debugXSL.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\file_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\files.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\files_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\help_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\nodeview_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\option_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\options.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\options_win32.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\os_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\param_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\search.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\search_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\template_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\trace_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\utils.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\variable_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbg.c
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgthread.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\libxsldbg\arraylist.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\breakpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\callstack.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\cmds.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\debug.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\debugXSL.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\files.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\help.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\options.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\search.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\utils.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\win32conf.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbg.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgio.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgmsg.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgnotifier.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgthread.h
# End Source File
# Begin Source File

SOURCE=..\..\src\libxsldbg\xsldbgwin32conf.h
# End Source File
# End Group
# End Target
# End Project
