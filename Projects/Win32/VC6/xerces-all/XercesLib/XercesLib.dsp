# Microsoft Developer Studio Project File - Name="XercesLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XercesLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XercesLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XercesLib.mak" CFG="XercesLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XercesLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XercesLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XercesLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\VC6\Release\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /Za /W3 /GX /O2 /Ob2 /I "..\..\src" /I "..\..\src\dom" /I "..\..\..\..\..\src" /D "NDEBUG" /D "PLATFORM_WIN32" /D "_CRTDBG_MAP_ALLOC" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_SAX2" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "WIN32" /D "_WINDOWS" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /D "XML_USE_NETACCESSOR_WINSOCK" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib advapi32.lib ws2_32.lib /nologo /base:"0x12000000" /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Release\xerces-c_1_5_1.dll" /implib:"..\..\..\..\..\Build\Win32\VC6\Release/xerces-c_1.lib" /version:1.5.1
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "XercesLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\VC6\Debug\obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G6 /MDd /Za /W3 /Gm /GX /Zi /Od /I "..\..\src" /I "..\..\src\dom" /I "..\..\..\..\..\src" /D "_CRTDBG_MAP_ALLOC" /D "PROJ_SAX2" /D "_DEBUG" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "WIN32" /D "_WINDOWS" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /D "XML_USE_NETACCESSOR_WINSOCK" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib advapi32.lib ws2_32.lib /nologo /base:"0x12000000" /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Debug/xerces-c_1_5_1D.dll" /implib:"..\..\..\..\..\Build\Win32\VC6\Debug/xerces-c_1D.lib" /pdbtype:sept /version:1.5.1
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XercesLib - Win32 Release"
# Name "XercesLib - Win32 Debug"
# Begin Group "util"

# PROP Default_Filter ".cpp,.hpp"
# Begin Group "Win32"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=.\IDMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Win32Defs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Win32PlatformUtils.cpp
# ADD CPP /Ze
# End Source File
# End Group
# Begin Group "MsgLoaders"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\InMemory\CppErrMsgs_EN_US.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\Win32\Win32MsgLoader.cpp
# ADD CPP /Ze
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\MsgLoaders\Win32\Win32MsgLoader.hpp
# End Source File
# End Group
# Begin Group "Transcoders"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService.cpp
# ADD CPP /Ze
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Transcoders\Win32\Win32TransService.hpp
# End Source File
# End Group
# Begin Group "NetAccessors"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\BinHTTPURLInputStream.cpp
# ADD CPP /Ze
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\BinHTTPURLInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\WinSockNetAccessor.cpp
# ADD CPP /Ze
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\NetAccessors\WinSock\WinSockNetAccessor.hpp
# End Source File
# End Group
# Begin Group "regx"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ASCIIRangeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ASCIIRangeFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\BlockRangeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\BlockRangeFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\BMPattern.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\BMPattern.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\CharToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\CharToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ClosureToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ClosureToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ConcatToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ConcatToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ConditionToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ConditionToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\Match.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\Match.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ModifierToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ModifierToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\Op.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\Op.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\OpFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\OpFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ParenToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ParenToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ParserForXMLSchema.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\ParserForXMLSchema.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RangeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RangeFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RangeToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RangeToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RangeTokenMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RangeTokenMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RegularExpression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RegularExpression.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RegxDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RegxParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RegxParser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RegxUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\RegxUtil.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\StringToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\StringToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\Token.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\Token.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\TokenFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\TokenFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\TokenInc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\UnicodeRangeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\UnicodeRangeFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\UnionToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\UnionToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\XMLRangeFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\XMLRangeFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\XMLUniCharacter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\regx\XMLUniCharacter.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\src\util\ArrayIndexOutOfBoundsException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\AutoSense.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Base64.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinFileInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinFileInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinMemInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BinMemInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BitOps.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\BitSet.cpp
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

SOURCE=..\..\..\..\..\src\util\HashBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HashPtr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HashPtr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HashXMLCh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HashXMLCh.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HeaderDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HexBin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\HexBin.hpp
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
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\KVStringPair.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Mutexes.cpp
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
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\PlatformUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\QName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\QName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RefArrayOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RefHash2KeysTableOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\RefHash3KeysIdPool.hpp
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
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\StringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TranscodingException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TransService.cpp
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

SOURCE=..\..\..\..\..\src\util\XercesDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML256TableTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML256TableTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML88591Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XML88591Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLASCIITranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLASCIITranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLBigDecimal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLBigDecimal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLBigInteger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLBigInteger.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLChTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLChTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLDeleterFor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLDouble.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLDouble.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEBCDICTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEBCDICTranscoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLEnumerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLExceptMsgs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLFloat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLFloat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLIBM1140Transcoder.cpp
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
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLStringTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLStringTokenizer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUCS4Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUCSTranscoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUni.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUni.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUniDefs.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUri.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUri.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLURL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLURL.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF16Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF16Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF8Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLUTF8Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLWin1252Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLWin1252Transcoder.hpp
# End Source File
# End Group
# Begin Group "framework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\LocalFileInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\LocalFileInputSource.Hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\Makefile.in
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\MemBufInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\StdInInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\StdInInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\URLInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\URLInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDef.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttDefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBuffer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBufferMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLBufferMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLDocumentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLElementDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLElementDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLEntityDecl.cpp
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

SOURCE=..\..\..\..\..\src\framework\XMLFormatter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLFormatter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLPScanToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRecognizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRecognizer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLRefInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLValidityCodes.hpp
# End Source File
# End Group
# Begin Group "internal"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\CharTypeTables.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ElemStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ElemStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\EndOfEntityException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttributesImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttributesImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLInternalErrorHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLReader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLReader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\XMLScanner2.cpp
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

SOURCE=..\..\..\..\..\src\sax\SAXException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\SAXException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\SAXParseException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax\SAXParseException.hpp
# End Source File
# End Group
# Begin Group "parsers"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\DOMParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\DOMParser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\IDOMParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\IDOMParser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\SAX2XMLReaderImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\SAX2XMLReaderImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\SAXParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\parsers\SAXParser.hpp
# End Source File
# End Group
# Begin Group "dom"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrMapImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrMapImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrNSImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrNSImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CDATASectionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CDATASectionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CharacterDataImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CharacterDataImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ChildNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ChildNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CommentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CommentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DeepNodeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DeepNodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentFragmentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentFragmentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Attr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Attr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CDATASection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CharacterData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Comment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Comment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Document.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Document.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Element.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Element.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Entity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Entity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_EntityReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_EntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Node.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Node.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeIterator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Notation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Notation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_ProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_ProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Range.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Range.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_RangeException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_RangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Text.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_TreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_XMLDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_XMLDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DomMemDebug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DomMemDebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMStringImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementDefinitionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementDefinitionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementNSImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementNSImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityReferenceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityReferenceImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\MemDebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NamedNodeMapImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NamedNodeMapImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NameNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIDMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIDMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIteratorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIteratorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NotationImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NotationImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ParentNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ParentNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RangeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RangeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RefCountedImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RefCountedImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TextImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TextImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TreeWalkerImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TreeWalkerImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\XMLDeclImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\XMLDeclImpl.hpp
# End Source File
# End Group
# Begin Group "validators"

# PROP Default_Filter ".cpp,.hpp"
# Begin Group "common"

# PROP Default_Filter "*.cpp, *.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMAny.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMAny.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMBinaryOp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMBinaryOp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMLeaf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMStateSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMUnaryOp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\CMUnaryOp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\ContentLeafNameTypeVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\ContentLeafNameTypeVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\ContentSpecNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\ContentSpecNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\DFAContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\DFAContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\Grammar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\GrammarResolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\GrammarResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\MixedContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\MixedContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\SimpleContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\common\SimpleContentModel.hpp
# End Source File
# End Group
# Begin Group "datatype"

# PROP Default_Filter "*.cpp, *.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\AnyURIDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\AnyURIDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\Base64BinaryDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\Base64BinaryDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\BooleanDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\BooleanDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DatatypeValidatorFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DatatypeValidatorFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DecimalDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DecimalDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DoubleDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\DoubleDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\ENTITYDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\ENTITYDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\FloatDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\FloatDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\HexBinaryDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\HexBinaryDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\IDDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\IDDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\IDREFDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\IDREFDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\InvalidDatatypeFacetException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\InvalidDatatypeValueException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\ListDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\ListDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\NOTATIONDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\NOTATIONDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\QNameDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\QNameDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\StringDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\StringDatatypeValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\UnionDatatypeValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\datatype\UnionDatatypeValidator.hpp
# End Source File
# End Group
# Begin Group "DTD"

# PROP Default_Filter "*.cpp, *.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DocTypeHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDef.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDAttDefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDElementDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDElementDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDEntityDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDGrammar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDGrammar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDScanner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDScanner.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator.hpp
# End Source File
# End Group
# Begin Group "schema"

# PROP Default_Filter "*.cpp, *.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\ComplexTypeInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\ComplexTypeInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\GeneralAttributeCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\GeneralAttributeCheck.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\NamespaceScope.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\NamespaceScope.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaAttDef.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaAttDef.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaAttDefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaAttDefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaElementDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaElementDecl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaGrammar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaGrammar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaSymbols.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaSymbols.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SchemaValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SubstitutionGroupComparator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\SubstitutionGroupComparator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\TraverseSchema.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\TraverseSchema.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\XercesAttGroupInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\XercesAttGroupInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\XercesGroupInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\XercesGroupInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\XUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\schema\XUtil.hpp
# End Source File
# End Group
# End Group
# Begin Group "sax2"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\sax2\Attributes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax2\ContentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax2\DefaultHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax2\sax2Dummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax2\SAX2XMLReader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\sax2\XMLReaderFactory.hpp
# End Source File
# End Group
# Begin Group "idom"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDAttrImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDAttrImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDAttrMapImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDAttrMapImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDAttrNSImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDAttrNSImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDCasts.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDCDATASectionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDCDATASectionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDCharacterDataImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDCharacterDataImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDChildNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDChildNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDCommentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDCommentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDeepNodeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDeepNodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDeepNodeListPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDocumentFragmentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDocumentFragmentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDocumentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDocumentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDocumentTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDocumentTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDOMImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDDOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDElementImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDElementImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDElementNSImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDElementNSImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDEntityImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDEntityImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDEntityReferenceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDEntityReferenceImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNamedNodeMapImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNamedNodeMapImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeIDMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeIDMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeIteratorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeIteratorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNodeVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNotationImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDNotationImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Attr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_CDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_CharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Comment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Document.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_DocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_DocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_DOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Element.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Entity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_EntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_NamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Node.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_NodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_NodeIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_NodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Notation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_ProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Range.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_RangeException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_RangeException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDOM_TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDParentNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDParentNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDProcessingInstructionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDProcessingInstructionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDRangeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDRangeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDTextImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDTextImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDTreeWalkerImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\idom\IDTreeWalkerImpl.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Version.rc
# End Source File
# End Target
# End Project
