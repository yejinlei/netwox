# Microsoft Developer Studio Project File - Name="netwox539" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=netwox539 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netwox539.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netwox539.mak" CFG="netwox539 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netwox539 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "netwox539 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\..\..\netwib-bin_windows\include" /I "..\..\.." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 advapi32.lib netwib539.lib /nologo /subsystem:console /machine:I386 /libpath:"..\..\..\..\..\netwib-bin_windows\lib"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /I "..\..\..\..\..\netwib-bin_windows\include" /I "..\..\.." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 advapi32.lib netwib539.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\netwib-bin_windows\lib"

!ENDIF 

# Begin Target

# Name "netwox539 - Win32 Release"
# Name "netwox539 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "tools"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\tools\000000.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000001.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000002.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000003.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000004.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000005.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000006.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000007.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000008.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000009.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000010.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000011.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000012.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000013.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000014.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000015.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000016.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000017.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000018.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000019.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000020.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000021.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000022.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000023.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000024.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000025.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000026.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000027.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000028.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000029.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000030.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000031.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000032.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000033.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000034.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000035.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000036.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000037.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000038.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000039.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000040.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000041.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000042.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000043.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000044.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000045.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000046.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000047.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000048.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000049.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000050.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000051.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000052.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000053.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000054.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000055.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000056.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000057.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000058.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000059.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000060.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000061.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000062.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000063.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000064.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000065.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000066.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000067.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000068.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000069.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000070.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000071.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000072.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000073.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000074.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000075.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000076.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000077.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000078.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000079.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000080.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000081.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000082.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000083.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000084.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000085.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000086.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000087.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000088.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000089.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000090.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000091.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000092.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000093.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000094.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000095.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000096.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000097.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000098.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000099.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000100.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000101.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000102.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000103.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000104.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000105.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000106.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000107.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000108.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000109.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000110.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000111.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000112.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000113.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000114.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000115.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000116.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000117.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000118.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000119.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000120.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000121.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000122.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000123.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000124.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000125.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000126.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000127.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000128.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000129.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000130.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000131.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000132.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000133.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000134.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000135.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000136.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000137.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000138.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000139.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000140.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000141.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000142.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000143.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000144.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000145.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000146.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000147.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000148.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000149.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000150.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000151.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000152.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000153.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000154.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000155.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000156.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000157.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000158.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000159.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000160.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000161.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000162.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000163.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000164.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000165.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000166.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000167.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000168.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000169.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000170.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000171.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000172.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000173.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000174.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000175.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000176.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000177.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000178.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000179.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000180.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000181.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000182.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000183.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000184.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000185.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000186.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000187.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000188.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000189.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000190.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000191.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000192.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000193.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000194.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000195.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000196.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000197.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000198.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000199.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000200.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000201.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000202.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000203.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000204.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000205.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000206.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000207.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000208.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000209.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000210.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000211.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000212.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000213.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000214.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000215.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000216.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000217.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000218.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000219.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000220.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000221.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000222.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\tools\000223.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/tools"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/tools"

!ENDIF 

# End Source File
# End Group
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\modules\alive.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\brute.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\checksum.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\cmdline.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\conffile.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\date.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dnscache.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\err.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\expect.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\fraction.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\icmp6nd.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\icmperr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\io.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ip4opts.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\losepriv.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\mail.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\mailex.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\passwordgene.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\pcapfilt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\perf.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ping.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\pkt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\pktshow.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\record.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\relay.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\remadmtcp.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\sample.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\scale.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\scan.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\search.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snispo.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\sock.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\spooffrag.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\syslog.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\time.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tracert.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tracertdisc.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\txthdr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\txtproto.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\unicode.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\whois.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\wordlist.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\asn1\asn1data.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\asn1\asn1oid.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\asn1\asn1pktber.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\asn1\asn1show.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\asn1\asn1tag.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\buf\bufarray.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\buf\bufarraystore.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\buf\buf.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\buf\bufstore.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\cryptohash.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\des.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\hmac.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\md2.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\md.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\ripemd.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\sha1.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\sha224256.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\sha384512.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\crypto\xor.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dhcp4\dhcp4hdr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dhcp4\dhcp4opt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dhcp4\dhcp4opts.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dhcp4\dhcp4pkt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dns.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnsfield.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnshdr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnspkt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnspktex.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnsrdata.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnsrr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnsrrring.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\dns\dnsshow.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ftp\ftpcli.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ftp\ftpclient.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ftp\ftpcmd.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ftp\ftpreply.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ftp\ftpser.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ftp\ftpserver.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\html\html.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\html\htmlfile.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\html\htmltag.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpbody.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpcli.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpclictx.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpclireqhdrs.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpcliresphdrs.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httphdr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpserctx.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpserreqhdrs.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\http\httpserresphdrs.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ident\identclient.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ident\identserver.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\irc\irc.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\irc\ircses.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\mime\mime.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\mime\mimetypes.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\nntp\nntp.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\nntp\nntpses.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ntp\ntphdr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\ntp\ntppkt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\rules\rules.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\rules\rulesfile.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\rules\rulesre.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\netbiosname.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcli.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmd.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdcmn.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdsearch.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdtsc2.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdtsc2q.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdtsc2r.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdtscntq.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdtscntr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdtscq.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbcmdtscr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smberr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbmsg.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbmsghdr.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbnaked.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbnbtss.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbser83.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbser.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbsercase.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbsercmd.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbsercmdsearch.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbsercmdtsc2.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbsercmdtsc.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbsercmdtscnt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbserfid.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbserglob.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbserjail.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbserver.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\smbtest.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\winauthlm.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\winauthntlmv1.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smb\winauthntlmv2.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smtp\smtp.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\smtp\smtpserver.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmp3.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmparg.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmpex.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmpmd5.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmpmsg.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmppdu.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmppkt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmpvb.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\snmp\snmpvbl.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\sockv\sockv.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\sockv\sockvcom.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\sockv\sockvtcp.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\sockv\sockvudp.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\sockv\vstack.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\telnet\telnet.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\telnet\telnetserver.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\telnet\telnetses.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tftp\tftpex.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tftp\tftppkt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tool\arg.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tool\toolarg.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tool\toolargstore.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tool\tool.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\tool\tooltree.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\url\url.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\url\urlcanon.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\url\urldown.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\url\urllocalbdd.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\url\urllocal.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\webspider\webspider.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\webspider\webspidercf.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\webspider\webspidermt.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\webspider\webspiderurl1.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\modules\webspider\webspiderurl2.c

!IF  "$(CFG)" == "netwox539 - Win32 Release"

# PROP Intermediate_Dir "Release/modules"

!ELSEIF  "$(CFG)" == "netwox539 - Win32 Debug"

# PROP Intermediate_Dir "Debug/modules"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\netwox.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\ico\icon.rc
# End Source File
# End Group
# End Target
# End Project
