# Microsoft Developer Studio Project File - Name="xml4com" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=xml4com - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xerces-com.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xerces-com.mak" CFG="xml4com - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xml4com - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xml4com - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xml4com - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xml4com - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xml4com - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "xml4com - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "xml4com - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Build\Win32\xml4com\Debug"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\xml4com\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "_DEBUG" /D "_MBCS" /D "_ATL_DEBUG_INTERFACES" /D "_ATL_DEBUG_QI" /D "_ATL_DEBUG_REFCOUNT" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib xerces-c_1.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Build\Win32\VC6\Debug"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\Debug
TargetPath=\xml-xerces\c\Build\Win32\xml4com\Debug\xerces-com.dll
InputPath=\xml-xerces\c\Build\Win32\xml4com\Debug\xerces-com.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xml4com - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Build\Win32\xml4com\DebugU"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\xml4com\DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib xerces-c_1.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\..\Build\Win32\VC6\Debug"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\DebugU
TargetPath=\xml-xerces\c\Build\Win32\xml4com\DebugU\xerces-com.dll
InputPath=\xml-xerces\c\Build\Win32\xml4com\DebugU\xerces-com.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xml4com - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Build\Win32\xml4com\ReleaseMinSize"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\xml4com\ReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_MBCS" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib xerces-c_1.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\..\..\..\Build\Win32\VC6\Release"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseMinSize
TargetPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseMinSize\xerces-com.dll
InputPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseMinSize\xerces-com.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xml4com - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Build\Win32\xml4com\ReleaseMinDependency"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\xml4com\ReleaseMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib xerces-c_1.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\..\..\..\Build\Win32\VC6\Release"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseMinDependency
TargetDir=\xml-xerces\c\Build\Win32\xml4com\ReleaseMinDependency
TargetPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseMinDependency\xerces-com.dll
InputPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseMinDependency\xerces-com.dll
SOURCE="$(InputPath)"

BuildCmds= \
	echo copy ..\..\..\..\..\Build\Win32\VC6\Release\xerces-c_1_2.dll $(TargetDir) \
	copy ..\..\..\..\..\Build\Win32\VC6\Release\xerces-c_1_2.dll $(TargetDir) \
	regsvr32 /s /c "$(TargetPath)" \
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" \
	

"$(TargetDir)\xerces-c_1_2.dll" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "xml4com - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinSize"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib xerces-c_1.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\..\..\..\Build\Win32\VC6\Release"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinSize
TargetPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseUMinSize\xerces-com.dll
InputPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseUMinSize\xerces-com.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "xml4com - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinDependency"
# PROP Intermediate_Dir ".\..\..\..\..\Build\Win32\xml4com\ReleaseUMinDependency"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib xerces-c_1.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinDependency
TargetPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseUMinDependency\xerces-com.dll
InputPath=\xml-xerces\c\Build\Win32\xml4com\ReleaseUMinDependency\xerces-com.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "xml4com - Win32 Debug"
# Name "xml4com - Win32 Unicode Debug"
# Name "xml4com - Win32 Release MinSize"
# Name "xml4com - Win32 Release MinDependency"
# Name "xml4com - Win32 Unicode Release MinSize"
# Name "xml4com - Win32 Unicode Release MinDependency"
# Begin Group "com"

# PROP Default_Filter "h;hpp;hxx;hm;inl;cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\src\com\BindStatusCallback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\IXMLDOMCharacterDataImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\IXMLDOMNodeImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\IXMLDOMNodeImpl.inl
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\IXMLDOMTextImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\NodeContainerImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xml4com.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xml4comCP.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMAttribute.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMCDATASection.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMComment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMDocument.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMDocumentFragment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMDocumentType.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMElement.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMEntity.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMEntityReference.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMImplementation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMNamedNodeMap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMNodeList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMNotation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMParseError.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMProcessingInstruction.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMText.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLHTTPRequest.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\src\com\BindStatusCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xml4com.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xml4com.def
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xml4com.idl
# ADD MTL /I "..\..\..\..\..\src\com" /tlb "..\..\..\..\..\Build\Win32\xml4com\xml4com.tlb" /h "..\..\..\..\..\src\com\xml4com.h" /iid "..\..\..\..\..\src\com\xml4com_i.c"
# SUBTRACT MTL /Oicf
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xml4com.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMDocumentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMNodeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMNotation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMParseError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLDOMUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\XMLHTTPRequest.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xmldocument.rgs
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\com\xmlhttprequest.rgs
# End Source File
# End Group
# End Group
# End Target
# End Project
