# Microsoft Developer Studio Project File - Name="netwibtest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=netwibtest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netwibtest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netwibtest.mak" CFG="netwibtest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netwibtest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "netwibtest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netwibtest - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /O2 /GX /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /GX /I "..\..\..\..\..\src\netwib-src\src\port\windows\iphlpapi" /I "..\..\..\..\..\src\netwib-src\src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 
# ADD LINK32 advapi32.lib wsock32.lib netapi32.lib rpcrt4.lib /nologo /subsystem:console /machine:I386 /out:"../../../netwibtest.exe" 

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W4 /WX /Od /GZ /Gm /ZI /GX /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MTd /W4 /WX /Od /GZ /Gm /ZI /GX /I "..\..\..\..\..\src\netwib-src\src\port\windows\iphlpapi" /I "..\..\..\..\..\src\netwib-src\src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /debug /pdbtype:sept
# ADD LINK32 advapi32.lib wsock32.lib netapi32.lib rpcrt4.lib /nologo /subsystem:console /machine:I386 /out:"../../../netwibtest.exe" /debug /pdbtype:sept

!ENDIF 

# Begin Target

# Name "netwibtest - Win32 Release"
# Name "netwibtest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "dat"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\c.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\c2.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\ptr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\buf.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\bufpool.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\bufenc.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\bufdec.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\fmt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\checksum.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\regexp.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\ring.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\ringi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\hash.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\hashi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\tlv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\array.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\dat\uint64.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# End Group
# Begin Group "sys"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\time.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\rand.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\path.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\pathname.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\pathstat.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\dirname.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\filename.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\dir.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\thread.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\threadmut.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\threadrwl.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\threadcond.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\threadtsd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\threadlist.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\io.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\ionew.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\iousual.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\wait.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\waitlist.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\file.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\fd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\stream.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\kbd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\kbdchar.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\kbdint.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\kbdbuf.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\kbdpress.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\screen.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\bufenc.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\bufdec.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\disp.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\record.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\beep.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\global.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\init.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\error.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\internal.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\unix.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\sys\windows.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# End Group
# Begin Group "net"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\device.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\ip.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\eth.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\port.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\ips.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\ipsi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\eths.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\ethsi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\ports.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\portsi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\conf.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\confdev.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\confip.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\confarp.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\confrout.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\sock.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\sniff.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\net\spoof.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# End Group
# Begin Group "pkt"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\linkhdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\iphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\ip4opt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\ip4opts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\ip6ext.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\ip6exts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\arphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\udphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\tcphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\tcpopt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\tcpopts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\ip64bits.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\icmp4.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\icmp6.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\icmp6nd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\layer.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\packet.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\conv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\pkt\ipfrag.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# End Group
# Begin Group "shw"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\array.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\linkhdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\iphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\ip4opt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\ip4opts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\ip6ext.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\ip6exts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\arphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\udphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\tcphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\tcpopt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\tcpopts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\icmp4.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\icmp6.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\icmp6nd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\netwib\shw\packet.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# End Group
# Begin Group "test"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\src\netwibtest.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_c.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_c2.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_ptr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_buf.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_bufpool.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_bufenc.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_bufdec.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_fmt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_checksum.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_regexp.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_ring.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_ringi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_hash.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_hashi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_tlv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_array.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_dat_uint64.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_time.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_rand.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_path.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_pathname.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_pathstat.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_dirname.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_filename.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_dir.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_thread.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_threadmut.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_threadrwl.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_threadcond.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_threadtsd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_threadlist.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_io.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_ionew.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_iousual.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_wait.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_waitlist.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_file.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_fd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_stream.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_kbd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_kbdchar.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_kbdint.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_kbdbuf.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_kbdpress.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_screen.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_bufenc.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_bufdec.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_disp.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_record.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_beep.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_global.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_init.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_error.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_internal.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_unix.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_sys_windows.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_device.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_ip.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_eth.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_port.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_ips.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_ipsi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_eths.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_ethsi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_ports.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_portsi.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_conf.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_confdev.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_confip.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_confarp.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_confrout.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_sock.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_sniff.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_net_spoof.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_linkhdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_iphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_ip4opt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_ip4opts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_ip6ext.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_ip6exts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_arphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_udphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_tcphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_tcpopt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_tcpopts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_ip64bits.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_icmp4.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_icmp6.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_icmp6nd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_layer.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_packet.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_conv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_pkt_ipfrag.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_priv.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_debug.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_array.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_linkhdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_iphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_ip4opt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_ip4opts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_ip6ext.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_ip6exts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_arphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_udphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_tcphdr.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_tcpopt.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_tcpopts.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_icmp4.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_icmp6.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_icmp6nd.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_shw_packet.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\src\netwibtest_util.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\test"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\test"

!ENDIF 

# End Source File
# End Group
# Begin Group "ext"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\src\netwib-src\src\port\windows\regex\regex.c"

!IF  "$(CFG)" == "netwibtest - Win32 Release"

# PROP Intermediate_Dir "Release\ext"

!ELSEIF  "$(CFG)" == "netwibtest - Win32 Debug"

# PROP Intermediate_Dir "Debug\ext"

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
