# Microsoft Developer Studio Project File - Name="XML4CParser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XML4CParser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XML4CParser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XML4CParser.mak" CFG="XML4CParser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XML4CParser - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XML4CParser - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XML4CParser - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XML4CPARSER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /Za /W3 /GX /O2 /Ob2 /I "..\..\..\..\..\src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PROJ_XMLPARSER" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib IXUtil.Lib /nologo /base:"0x11000000" /version:3.0 /dll /map /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Release/IXCore.dll" /libpath:"..\..\..\..\..\Build\Win32\VC6\Release"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "XML4CParser - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XML4CPARSER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /Za /W3 /Gm /GX /Zi /Od /I "..\..\..\..\..\src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "PROJ_XMLPARSER" /FD /GZ /c
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
# ADD LINK32 kernel32.lib user32.lib IXUtil.Lib /nologo /base:"0x11000000" /version:3.0 /dll /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Debug/IXCore.dll" /libpath:"..\..\..\..\..\Build\Win32\VC6\Debug"
# SUBTRACT LINK32 /profile /map

!ENDIF 

# Begin Target

# Name "XML4CParser - Win32 Release"
# Name "XML4CParser - Win32 Debug"
# Begin Group "Framework Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\LocalFileInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\MemBufInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\StdInInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\URLInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBufferMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLElementDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLEntityDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLFormatter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRecognizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidator.cpp
# End Source File
# End Group
# Begin Group "Framework Header Files"

# PROP Default_Filter "hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\LocalFileInputSource.Hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\StdInInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\URLInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDef.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBuffer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBufferMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLDocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLElementDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLEntityDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLEntityHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLErrorCodes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLErrorReporter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLPScanToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRecognizer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRefInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidityCodes.hpp
# End Source File
# End Group
# Begin Group "Internal Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ElemStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLReader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner2.cpp
# End Source File
# End Group
# Begin Group "Internal Header Files"

# PROP Default_Filter "hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\CharTypeTables.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ElemStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\EndOfEntityException.Hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLReader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner.hpp
# End Source File
# End Group
# End Target
# End Project
