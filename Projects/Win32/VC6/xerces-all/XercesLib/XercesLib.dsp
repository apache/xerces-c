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
# ADD CPP /nologo /G6 /MD /Za /W3 /GX /O2 /Ob2 /I "..\..\src" /I "..\..\src\dom" /I "..\..\..\..\..\src" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "DEVENV_VCPP" /D "PLATFORM_WIN32" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib /nologo /base:"0x10000000" /version:1.0 /subsystem:windows /dll /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Release\xerces-c_1_0.dll" /implib:"..\..\..\..\..\Build\Win32\VC6\Release/xerces-c_1.lib"
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
# ADD CPP /nologo /G6 /MDd /Za /W3 /Gm /GX /ZI /Od /I "..\..\src" /I "..\..\src\dom" /I "..\..\..\..\..\src" /D "PROJ_XMLPARSER" /D "PROJ_XMLUTIL" /D "PROJ_PARSERS" /D "PROJ_SAX4C" /D "PROJ_DOM" /D "PROJ_VALIDATORS" /D "XML_SINGLEDLL" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "XML_USE_WIN32_TRANSCODER" /D "XML_USE_WIN32_MSGLOADER" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib /nologo /base:"0x10000000" /version:1.0 /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Debug/xerces-c_1_0.dll" /implib:"..\..\..\..\..\Build\Win32\VC6\Debug/xerces-c_1.lib" /pdbtype:sept
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

SOURCE=..\..\..\..\..\src\util\MsgLoaders\Win32\Win32MsgLoader.Cpp
# ADD CPP /Ze
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
# Begin Source File

SOURCE=..\..\..\..\..\src\util\ArrayIndexOutOfBoundsException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\AutoSense.hpp
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

SOURCE=..\..\..\..\..\src\util\HeaderDummy.cpp
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

SOURCE=..\..\..\..\..\src\util\StdOut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\StdOut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\StringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\StringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TextOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\TextOutputStream.hpp
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

SOURCE=..\..\..\..\..\src\util\URL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\URL.hpp
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

SOURCE=..\..\..\..\..\src\util\XMLMsgLoader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\util\XMLString.hpp
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
# End Group
# Begin Group "framework"

# PROP Default_Filter ""
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

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\framework\XMLNotationDecl.hpp
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

SOURCE=..\..\..\..\..\src\internal\MemBufInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\MemBufInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\ReaderMgr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\StdInInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\StdInInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\URLInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\URLInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\internal\VecAttrListImpl.hpp
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

SOURCE=..\..\..\..\..\src\dom\NodeFilterImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeFilterImpl.hpp
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

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.hpp
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
# End Group
# Begin Group "validators"

# PROP Default_Filter ".cpp,.hpp"
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMBinaryOp.cpp
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
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\CMUnaryOp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\ContentSpecNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\ContentSpecNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DFAContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DFAContentModel.hpp
# End Source File
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

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\DTDValidator2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\MixedContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\MixedContentModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\SimpleContentModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\validators\DTD\SimpleContentModel.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\src\util\Platforms\Win32\Version.rc
# End Source File
# End Target
# End Project
