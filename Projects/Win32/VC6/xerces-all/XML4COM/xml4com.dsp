# Microsoft Developer Studio Project File - Name="xml4com" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=xml4com - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "xml4com.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xml4com.mak" CFG="xml4com - Win32 Debug"
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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "_DEBUG" /D "_MBCS" /D "_ATL_DEBUG_INTERFACES" /D "_ATL_DEBUG_QI" /D "_ATL_DEBUG_REFCOUNT" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\xml4com\Debug/xerces-c_1_1.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\Debug
TargetPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\Debug\xerces-c_1_1.dll
InputPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\Debug\xerces-c_1_1.dll
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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\xml4com\DebugU/xerces-c_1_1.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\DebugU
TargetPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\DebugU\xerces-c_1_1.dll
InputPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\DebugU\xerces-c_1_1.dll
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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_MBCS" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\Build\Win32\xml4com\ReleaseMinSize/xerces-c_1_1.dll"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseMinSize
TargetPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseMinSize\xerces-c_1_1.dll
InputPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseMinSize\xerces-c_1_1.dll
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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\Build\Win32\xml4com\ReleaseMinDependency/xerces-c_1_1.dll"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseMinDependency
TargetPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseMinDependency\xerces-c_1_1.dll
InputPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseMinDependency\xerces-c_1_1.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_UNICODE" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinSize/xerces-c_1_1.dll"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinSize
TargetPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseUMinSize\xerces-c_1_1.dll
InputPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseUMinSize\xerces-c_1_1.dll
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
# ADD CPP /nologo /MT /W3 /GX /O1 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\com" /D "NDEBUG" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "DEVENV_VCPP" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib shlwapi.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinDependency/xerces-c_1_1.dll"
# Begin Custom Build - Performing registration
OutDir=.\..\..\..\..\..\Build\Win32\xml4com\ReleaseUMinDependency
TargetPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseUMinDependency\xerces-c_1_1.dll
InputPath=\xerces-c-src_1_1_0\Build\Win32\xml4com\ReleaseUMinDependency\xerces-c_1_1.dll
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
# Begin Group "util"

# PROP Default_Filter ".cpp,.hpp"
# Begin Group "Win32"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Win32Defs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Win32PlatformUtils.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "MsgLoaders"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\InMemory\CppErrMsgs_EN_US.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\Win32\Win32MsgLoader.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "Transcoders"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\src\util\ArrayIndexOutOfBoundsException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\AutoSense.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinFileInputStream.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinFileInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinInputStream.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinMemInputStream.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinMemInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BitOps.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BitSet.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BitSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\CountedPointer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\EmptyStackException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\FlagJanitor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HeaderDummy.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\IllegalArgumentException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\InvalidCastException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\IOException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Janitor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\KeyValuePair.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\KVStringPair.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\KVStringPair.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Mutexes.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Mutexes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NameIdPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NoDefTranscoderException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NoSuchElementException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NullPointerException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\PlatformUtils.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RefArrayOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RefHashTableOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RefStackOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RefVectorOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RuntimeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\StringPool.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\StringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TextOutputStream.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TextOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TranscodingException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TransService.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TransService.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\UnexpectedEOFException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\UnsupportedEncodingException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\UTFDataFormatException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\ValueArrayOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\ValueStackOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\ValueVectorOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML4CDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML88591Transcoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML88591Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLASCIITranscoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLASCIITranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLChTranscoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLChTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEBCDICTranscoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEBCDICTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEnumerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLException.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLExceptMsgs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLIBM1140Transcoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLIBM1140Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLMsgLoader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLNetAccessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLString.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUCS4Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUCSTranscoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUni.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLURL.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLURL.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF16Transcoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF16Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF8Transcoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF8Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLWin1252Transcoder.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLWin1252Transcoder.hpp
# End Source File
# End Group
# Begin Group "framework"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\LocalFileInputSource.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\MemBufInputSource.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\URLInputSource.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\URLInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDef.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDef.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBuffer.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBuffer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBufferMgr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBufferMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLContentModel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLDocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLElementDecl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLElementDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLEntityDecl.cpp
# SUBTRACT CPP /YX /Yc /Yu
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

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRecognizer.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRecognizer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRefInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidator.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidator.hpp
# End Source File
# End Group
# Begin Group "internal"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\CharTypeTables.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ElemStack.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ElemStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\EndOfEntityException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLReader.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLReader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner2.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# End Group
# Begin Group "sax"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\AttributeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\DocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\DTDHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\Dummy.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\EntityResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\ErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\HandlerBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\InputSource.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\InputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\Locator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\Parser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\SAXParseException.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\SAXParseException.hpp
# End Source File
# End Group
# Begin Group "parsers"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\DOMParser.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\DOMParser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\SAXParser.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\SAXParser.hpp
# End Source File
# End Group
# Begin Group "dom"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CDATASectionImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CDATASectionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CharacterDataImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CharacterDataImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CommentImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CommentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DeepNodeListImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DeepNodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentFragmentImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentFragmentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentTypeImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Attr.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Attr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CDATASection.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CharacterData.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Comment.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Comment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Document.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Document.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentFragment.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentType.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMException.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMImplementation.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Element.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Element.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Entity.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Entity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_EntityReference.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_EntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NamedNodeMap.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Node.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Node.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeFilter.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeIterator.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeList.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Notation.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Notation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_ProcessingInstruction.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_ProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Text.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_TreeWalker.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DomMemDebug.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DomMemDebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMString.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DStringPool.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementDefinitionImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementDefinitionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityReferenceImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityReferenceImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\MemDebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NamedNodeMapImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NamedNodeMapImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIteratorImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIteratorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeListImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeVector.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NotationImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NotationImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RefCountedImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RefCountedImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TextImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TextImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TreeWalkerImpl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TreeWalkerImpl.hpp
# End Source File
# End Group
# Begin Group "validators"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMBinaryOp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMBinaryOp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMLeaf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMStateSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMUnaryOp.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMUnaryOp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\ContentSpecNode.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\ContentSpecNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DFAContentModel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DFAContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DocTypeHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDef.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDef.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDefList.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDElementDecl.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDElementDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDEntityDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator2.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\MixedContentModel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\MixedContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\SimpleContentModel.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\SimpleContentModel.hpp
# End Source File
# End Group
# End Target
# End Project
