# Microsoft Developer Studio Project File - Name="DOM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DOM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DOM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DOM.mak" CFG="DOM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DOM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DOM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DOM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\..\Build\Win32\VC6\Release\"
# PROP Intermediate_Dir "..\..\..\..\..\Build\Win32\VC6\Release\Obj"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DOM_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /Za /W3 /GX /O2 /Ob2 /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\DOM" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "PROJ_DOM" /FD /c
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
# ADD LINK32 kernel32.lib user32.lib IXUtil.Lib IXSAX.Lib IXCore.Lib /nologo /base:"0x15000000" /version:3.0 /dll /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Release\IXDOM.dll" /libpath:"..\..\..\..\..\Build\Win32\VC6\Release"

!ELSEIF  "$(CFG)" == "DOM - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DOM_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /Za /W3 /Gm /GX /Zi /Od /I "..\..\..\..\..\src" /I "..\..\..\..\..\src\DOM" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "PROJ_DOM" /FD /GZ /c
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
# ADD LINK32 kernel32.lib user32.lib IXUtil.Lib IXSAX.Lib IXCore.Lib /nologo /base:"0x15000000" /version:3.0 /dll /debug /machine:I386 /out:"..\..\..\..\..\Build\Win32\VC6\Debug/IXDOM.dll" /pdbtype:sept /libpath:"..\..\..\..\..\Build\Win32\VC6\Debug"

!ENDIF 

# Begin Target

# Name "DOM - Win32 Release"
# Name "DOM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrNSImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CDATASectionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CharacterDataImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CommentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DeepNodeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentFragmentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentTypeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Attr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CDATASection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CharacterData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Comment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Document.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Element.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Entity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_EntityReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Node.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeIterator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Notation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_ProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Text.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_TreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DomMemDebug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementDefinitionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementNSImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityReferenceImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NamedNodeMapImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIteratorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NotationImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RefCountedImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TextImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TreeWalkerImpl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\AttrNSImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CDATASectionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CharacterDataImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\CommentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DeepNodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentFragmentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DocumentTypeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Attr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_CharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Comment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Document.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_DOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Element.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Entity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_EntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Node.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeIterator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_NodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Notation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_ProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_Text.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOM_TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DomMemDebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DOMStringImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\DStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementDefinitionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ElementNSImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\EntityReferenceImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\MemDebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NamedNodeMapImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NameNodeFilter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeContainer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeIteratorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NodeVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\NotationImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\ProcessingInstructionImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\RefCountedImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TextImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\src\dom\TreeWalkerImpl.hpp
# End Source File
# End Group
# End Target
# End Project
