# Microsoft Developer Studio Project File - Name="corona" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=corona - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "corona.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "corona.mak" CFG="corona - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "corona - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "corona - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "corona - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /O2 /D "NDEBUG" /D for="if (0) ; else for" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "corona - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /ZI /Od /D "_DEBUG" /D for="if (0) ; else for" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CORONA_EXPORTS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "corona - Win32 Release"
# Name "corona - Win32 Debug"
# Begin Group "files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Corona.cpp
# End Source File
# Begin Source File

SOURCE=.\corona.def
# End Source File
# Begin Source File

SOURCE=.\corona.h
# End Source File
# Begin Source File

SOURCE=.\DefaultFileSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DefaultFileSystem.h
# End Source File
# Begin Source File

SOURCE=.\JPEG.cpp
# End Source File
# Begin Source File

SOURCE=.\Open.h
# End Source File
# Begin Source File

SOURCE=.\PCX.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleImage.h
# End Source File
# Begin Source File

SOURCE=.\Utility.h
# End Source File
# End Group
# Begin Group "libjpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\jpeg-6b\jcapimin.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcapistd.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jccoefct.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jccolor.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcdctmgr.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jchuff.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcinit.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcmainct.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcmarker.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcmaster.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcomapi.c"
# End Source File
# Begin Source File

SOURCE="..\third-party\sphere\jconfig.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcparam.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcphuff.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcprepct.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jcsample.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jctrans.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdapimin.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdapistd.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdatadst.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdatasrc.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdcoefct.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdcolor.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jddctmgr.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdhuff.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdinput.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdmainct.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdmarker.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdmaster.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdmerge.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdphuff.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdpostct.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdsample.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jdtrans.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jerror.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jfdctflt.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jfdctfst.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jfdctint.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jidctflt.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jidctfst.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jidctint.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jidctred.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jmemansi.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jmemmgr.c"
# End Source File
# Begin Source File

SOURCE="..\third-party\sphere\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE="..\third-party\sphere\jpeglib.h"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jquant1.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jquant2.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\jutils.c"
# End Source File
# Begin Source File

SOURCE=".\jpeg-6b\transupp.c"
# End Source File
# End Group
# End Target
# End Project
