# Microsoft Developer Studio Project File - Name="CoreTests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=CoreTests - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CoreTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CoreTests.mak" CFG="CoreTests - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CoreTests - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "CoreTests - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CoreTests - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\VC6\Release\Obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /Za /W3 /GX /O2 /Ob2 /I "..\..\..\..\..\src" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib IXUtil.Lib /nologo /version:2.4 /subsystem:console /map /machine:I386 /libpath:"..\..\..\..\..\Build\Win32\VC6\Release"

!ELSEIF  "$(CFG)" == "CoreTests - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\VC6\Debug\Obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\..\..\src" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib IXUtil.Lib /nologo /version:2.4 /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\..\Build\Win32\VC6\Debug"

!ENDIF 

# Begin Target

# Name "CoreTests - Win32 Release"
# Name "CoreTests - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_BitSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_CountedPointer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_RefArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_RefHashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_RefStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_RefVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_String.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_Transcoders.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_URL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_ValueArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_ValueStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests_ValueVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTestsMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\tests\UtilTests\CoreTests.hpp
# End Source File
# End Group
# End Target
# End Project
