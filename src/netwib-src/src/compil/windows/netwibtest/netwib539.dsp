# Microsoft Developer Studio Project File - Name="netwib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=netwib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netwib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netwib.mak" CFG="netwib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netwib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "netwib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netwib - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /O2 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NETWIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O2 /GX /I "..\..\..\port\windows\iphlpapi" /I "..\..\..\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NETWIB_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 
# ADD LINK32 advapi32.lib wsock32.lib netapi32.lib rpcrt4.lib /nologo /dll /machine:I386 
# SUBTRACT LINK32 /nodefaultlib /verbose /profile /pdb:none /map 
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\netwib539.lib ..\..\..\..\..\netwib-bin_windows\lib	copy Release\netwib539.dll ..\..\..\..\..\netwib-bin_windows\dll	copy Release\netwib539.dll ..\..\..\..\..\..\test\netwox
# End Special Build Tool

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W4 /WX /Od /GZ /Gm /ZI /GX /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NETWIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /WX /Od /GZ /Gm /ZI /GX /I "..\..\..\port\windows\iphlpapi" /I "..\..\..\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NETWIB_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /debug /pdbtype:sept
# ADD LINK32 advapi32.lib wsock32.lib netapi32.lib rpcrt4.lib /nologo /dll /machine:I386 /debug /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib 
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\netwib539.lib ..\..\..\..\..\netwib-bin_windows\lib	copy Debug\netwib539.dll ..\..\..\..\..\netwib-bin_windows\dll	copy Debug\netwib539.dll ..\..\..\..\..\..\test\netwox
# End Special Build Tool

!ENDIF

# Begin Target

# Name "netwib - Win32 Release"
# Name "netwib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "dat"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\netwib\dat\priv.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\debug.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\c.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\c2.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\ptr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\buf.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\bufpool.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\bufenc.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\bufdec.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\fmt.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\checksum.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\regexp.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\ring.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\ringi.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\hash.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\hashi.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\tlv.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\array.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\dat\uint64.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\dat"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\dat"

!ENDIF 

# End Source File
# End Group
# Begin Group "sys"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\netwib\sys\priv.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\debug.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\time.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\rand.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\path.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\pathname.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\pathstat.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\dirname.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\filename.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\dir.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\thread.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\threadmut.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\threadrwl.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\threadcond.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\threadtsd.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\threadlist.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\io.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\ionew.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\iousual.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\wait.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\waitlist.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\file.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\fd.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\stream.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\kbd.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\kbdchar.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\kbdint.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\kbdbuf.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\kbdpress.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\screen.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\bufenc.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\bufdec.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\disp.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\record.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\beep.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\global.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\init.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\error.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\internal.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\unix.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\sys\windows.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\sys"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\sys"

!ENDIF 

# End Source File
# End Group
# Begin Group "net"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\netwib\net\priv.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\debug.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\device.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\ip.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\eth.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\port.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\ips.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\ipsi.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\eths.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\ethsi.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\ports.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\portsi.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\conf.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\confdev.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\confip.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\confarp.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\confrout.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\sock.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\sniff.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\net\spoof.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\net"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\net"

!ENDIF 

# End Source File
# End Group
# Begin Group "pkt"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\netwib\pkt\priv.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\debug.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\linkhdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\iphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\ip4opt.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\ip4opts.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\ip6ext.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\ip6exts.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\arphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\udphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\tcphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\tcpopt.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\tcpopts.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\ip64bits.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\icmp4.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\icmp6.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\icmp6nd.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\layer.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\packet.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\conv.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\pkt\ipfrag.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\pkt"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\pkt"

!ENDIF 

# End Source File
# End Group
# Begin Group "shw"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\netwib\shw\priv.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\debug.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\array.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\linkhdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\iphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\ip4opt.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\ip4opts.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\ip6ext.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\ip6exts.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\arphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\udphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\tcphdr.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\tcpopt.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\tcpopts.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\icmp4.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\icmp6.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\icmp6nd.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\..\..\netwib\shw\packet.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\shw"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\shw"

!ENDIF 

# End Source File
# End Group
# Begin Group "ext"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\port\windows\regex\regex.c"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\ext"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

# PROP Intermediate_Dir "Debug\ext"

!ENDIF 

# End Source File
# Begin Source File

SOURCE="..\def\netwib.def"

!IF  "$(CFG)" == "netwib - Win32 Release"

# PROP Intermediate_Dir "Release\ext"

!ELSEIF  "$(CFG)" == "netwib - Win32 Debug"

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
