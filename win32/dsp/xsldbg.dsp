# Microsoft Developer Studio Project File - Name="xsldbg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=xsldbg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xsldbg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xsldbg.mak" CFG="xsldbg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xsldbg - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "xsldbg - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xsldbg - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "xsldbg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "xsldbg"
# PROP Intermediate_Dir "xsldbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /Zi /Od /I "../.." /I "../../../gnome-xml/include" /I "../../../libxslt/" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "WITH_DEBUGGER" /FD /I /GZ /c
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libexslt.lib libxslt.lib libxml2.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"xsltproc/xsltproc.pdb" /debug /machine:I386 /pdbtype:sept /libpath:"../../../gnome-xsl/win32/dsp/libexslt_a" /libpath:"../../../gnome-xsl/win32/dsp//libxslt_a" /libpath:"../../../gnome-xml/win32/dsp/libxml2_a"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "xsldbg - Win32 Release"
# Name "xsldbg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=..\..\src\breakpoint_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\dbgarraylist.c
# End Source File
# Begin Source File

SOURCE=..\..\src\dbgbreakpoint.c
# End Source File
# Begin Source File

SOURCE=..\..\src\dbgcallstack.c
# End Source File
# Begin Source File

SOURCE=..\..\src\dbgmain.c
# End Source File
# Begin Source File

SOURCE=..\..\src\dbgsearch.c
# End Source File
# Begin Source File

SOURCE=..\..\src\debugXSL.c
# End Source File
# Begin Source File

SOURCE=..\..\src\files.c
# End Source File
# Begin Source File

SOURCE=..\..\src\help.c
# End Source File
# Begin Source File

SOURCE=..\..\src\nodeview_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\options.c
# End Source File
# Begin Source File

SOURCE=..\..\src\os_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\param_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\search_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\template_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\trace_cmds.c
# End Source File
# Begin Source File

SOURCE=..\..\src\xsldbg.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\..\src\arraylist.h
# End Source File
# Begin Source File

SOURCE=..\..\src\breakpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\breakpointInternals.h
# End Source File
# Begin Source File

SOURCE=..\..\src\debugXSL.h
# End Source File
# Begin Source File

SOURCE=..\..\src\files.h
# End Source File
# Begin Source File

SOURCE=..\..\src\help.h
# End Source File
# Begin Source File

SOURCE=..\..\src\options.h
# End Source File
# Begin Source File

SOURCE=..\..\src\win32conf.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xslbreakpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xslcallpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xsldbg.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xsldbgwin32conf.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xslsearch.h
# End Source File
# End Group
# End Target
# End Project
