# Microsoft Developer Studio Project File - Name="XMLUtil" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XMLUtil - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XMLUtil.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XMLUtil.mak" CFG="XMLUtil - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XMLUtil - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XMLUtil - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XMLUtil - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XMLUTIL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /Za /W3 /GX /O2 /Ob2 /I "..\..\..\..\..\src" /I "\icu_1_4_0\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "PROJ_XMLUTIL" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /D "XML_USE_NETACCESSOR_WINSOCK" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib advapi32.lib wsock32.lib icuuc.lib /nologo /base:"0x10000000" /version:3.0 /dll /map /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Release/IXUtil.dll" /libpath:"..\..\..\..\..\Build\Win32\VC6\Release" /libpath:"\icu_1_4_0\Lib\Release"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "XMLUtil - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XMLUTIL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /Za /W3 /Gm /GX /Zi /Od /I "..\..\..\..\..\src" /I "\icu_1_4_0\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "PROJ_XMLUTIL" /D "XML_USE_ICU_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /D "XML_USE_NETACCESSOR_WINSOCK" /FD /GZ /c
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
# ADD LINK32 kernel32.lib user32.lib advapi32.lib wsock32.lib icuuc.lib /nologo /base:"0x10000000" /version:3.0 /dll /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Debug/IXUtil.dll" /libpath:"..\..\..\..\..\Build\Win32\VC6\Debug" /libpath:"\icu_1_4_0\Lib\Debug"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "XMLUtil - Win32 Release"
# Name "XMLUtil - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinFileInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinMemInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BitSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HeaderDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\KVStringPair.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Mutexes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\PlatformUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\StringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TransService.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Win32PlatformUtils.cpp
# ADD CPP /Ze
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML256TableTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML88591Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLASCIITranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLChTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEBCDICTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLIBM1140Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUCSTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUni.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLURL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF16Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF8Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLWin1252Transcoder.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\ArrayIndexOutOfBoundsException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\AutoSense.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinFileInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinMemInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BitOps.hpp
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

SOURCE=..\..\..\..\..\src\util\KVStringPair.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Mutexes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NameIdPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NoSuchElementException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NullPointerException.hpp
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

SOURCE=..\..\..\..\..\src\util\StringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TranscodingException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TransENameMap.hpp
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

SOURCE=..\..\..\..\..\src\util\Compilers\VCPPDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Win32Defs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML256TableTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML88591Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLASCIITranscoder.Hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLChTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLDeleterFor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEBCDICTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEnumerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLExceptMsgs.hpp
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

SOURCE=..\..\..\..\..\src\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUCS4Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLURL.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF16Transcoder.Hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF8Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLWin1252Transcoder.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Version.rc
# End Source File
# End Group
# Begin Group "Transcoder Header Files"

# PROP Default_Filter "hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\ICU\ICUTransService.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService.hpp
# End Source File
# End Group
# Begin Group "Transcoder Source Files"

# PROP Default_Filter "cpp; c"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\ICU\ICUTransService.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService.cpp
# ADD CPP /Ze
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService2.cpp

!IF  "$(CFG)" == "XMLUtil - Win32 Release"

!ELSEIF  "$(CFG)" == "XMLUtil - Win32 Debug"

# ADD CPP /Ze

!ENDIF 

# End Source File
# End Group
# Begin Group "MsgLoader Header Files"

# PROP Default_Filter "Hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\InMemory\CppErrMsgs_EN_US.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\InMemory\InMemMsgLoader.Hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\Win32\Win32MsgLoader.Hpp
# End Source File
# End Group
# Begin Group "MsgLoader Source Files"

# PROP Default_Filter "cpp; c"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\InMemory\InMemMsgLoader.Cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\Win32\Win32MsgLoader.Cpp
# ADD CPP /Ze
# End Source File
# End Group
# Begin Group "NetAccessor Header Files"

# PROP Default_Filter "hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\BinHTTPURLInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\WinSockNetAccessor.hpp
# End Source File
# End Group
# Begin Group "NetAccessor Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\BinHTTPURLInputStream.cpp
# ADD CPP /Ze
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\WinSockNetAccessor.cpp
# ADD CPP /Ze
# End Source File
# End Group
# End Target
# End Project
