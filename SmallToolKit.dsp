# Microsoft Developer Studio Project File - Name="SmallToolKit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SmallToolKit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SmallToolKit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SmallToolKit.mak" CFG="SmallToolKit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SmallToolKit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SmallToolKit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SmallToolKit - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ./cryptlib/cryptlib.lib /subsystem:windows /pdb:none /machine:I386 /nodefaultlib:"msvcprt.lib"
# SUBTRACT LINK32 /nologo /map /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=upx release\SmallToolKit.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SmallToolKit - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ./cryptlib/cryptlibd.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcprtd.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SmallToolKit - Win32 Release"
# Name "SmallToolKit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CWaitDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PrimeTable.cpp
# End Source File
# Begin Source File

SOURCE=.\PropChkSum.cpp
# End Source File
# Begin Source File

SOURCE=.\PropCode.cpp
# End Source File
# Begin Source File

SOURCE=.\PropData.cpp
# End Source File
# Begin Source File

SOURCE=.\PropFSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\PropNet.cpp
# End Source File
# Begin Source File

SOURCE=.\PropNum.cpp
# End Source File
# Begin Source File

SOURCE=.\PropRadix.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallToolKit.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallToolKit.rc
# End Source File
# Begin Source File

SOURCE=.\SmallToolKitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CWaitDialog.h
# End Source File
# Begin Source File

SOURCE=.\PrimeTable.h
# End Source File
# Begin Source File

SOURCE=.\PropChkSum.h
# End Source File
# Begin Source File

SOURCE=.\PropCode.h
# End Source File
# Begin Source File

SOURCE=.\PropData.h
# End Source File
# Begin Source File

SOURCE=.\PropFSplit.h
# End Source File
# Begin Source File

SOURCE=.\PropNet.h
# End Source File
# Begin Source File

SOURCE=.\PropNum.h
# End Source File
# Begin Source File

SOURCE=.\PropRadix.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SmallToolKit.h
# End Source File
# Begin Source File

SOURCE=.\SmallToolKitDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\1058190.gif
# End Source File
# Begin Source File

SOURCE=.\res\SmallToolKit.ico
# End Source File
# Begin Source File

SOURCE=.\res\SmallToolKit.rc2
# End Source File
# Begin Source File

SOURCE=.\tool.ico
# End Source File
# End Group
# Begin Group "core"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=.\core\code.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\core\cvt.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\core\file.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\core\math.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\core\prime.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\core\sock.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\core\eval.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
