# Microsoft Developer Studio Generated NMAKE File, Based on all.dsp
!IF "$(CFG)" == ""
CFG=all - Win64 Debug
!MESSAGE No configuration specified. Defaulting to all - Win64 Debug.
!ENDIF 

!IF "$(CFG)" != "all - Win64 Release" && "$(CFG)" != "all - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "all.mak" CFG="all - Win64 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "all - Win64 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "all - Win64 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : 

!ELSE 

ALL : "DeprecatedDOMCount - Win64 Release" "DOMTraversalTest - Win64 Release" "XercesLib - Win64 Release" "ThreadTest - Win64 Release" "StdInParse - Win64 Release" "SEnumVal - Win64 Release" "SAXPrint - Win64 Release" "SAXCount - Win64 Release" "SAX2Print - Win64 Release" "SAX2Count - Win64 Release" "Redirect - Win64 Release" "RangeTest - Win64 Release" "PParse - Win64 Release" "MemParse - Win64 Release" "InitTermTest - Win64 Release" "EnumVal - Win64 Release" "EncodingTest - Win64 Release" "DOMTest - Win64 Release" "DOMPrint - Win64 Release" "DOMMemTest - Win64 Release" "DOMCount - Win64 Release" "CreateDOMDocument - Win64 Release" 

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"CreateDOMDocument - Win64 ReleaseCLEAN" "DOMCount - Win64 ReleaseCLEAN" "DOMMemTest - Win64 ReleaseCLEAN" "DOMPrint - Win64 ReleaseCLEAN" "DOMTest - Win64 ReleaseCLEAN" "EncodingTest - Win64 ReleaseCLEAN" "EnumVal - Win64 ReleaseCLEAN" "InitTermTest - Win64 ReleaseCLEAN" "MemParse - Win64 ReleaseCLEAN" "PParse - Win64 ReleaseCLEAN" "RangeTest - Win64 ReleaseCLEAN" "Redirect - Win64 ReleaseCLEAN" "SAX2Count - Win64 ReleaseCLEAN" "SAX2Print - Win64 ReleaseCLEAN" "SAXCount - Win64 ReleaseCLEAN" "SAXPrint - Win64 ReleaseCLEAN" "SEnumVal - Win64 ReleaseCLEAN" "StdInParse - Win64 ReleaseCLEAN" "ThreadTest - Win64 ReleaseCLEAN" "XercesLib - Win64 ReleaseCLEAN" "DOMTraversalTest - Win64 ReleaseCLEAN" "DeprecatedDOMCount - Win64 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

!IF  "$(CFG)" == "all - Win64 Release"

MTL=midl.exe
MTL_PROJ=

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

MTL=midl.exe
MTL_PROJ=

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("all.dep")
!INCLUDE "all.dep"
!ELSE 
!MESSAGE Warning: cannot find "all.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "all - Win64 Release" || "$(CFG)" == "all - Win64 Debug"

!IF  "$(CFG)" == "all - Win64 Release"

"CreateDOMDocument - Win64 Release" : 
   cd "..\CreateDOMDocument"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\CreateDOMDocument.mak" CFG="CreateDOMDocument - Win64 Release" 
   cd "..\all"

"CreateDOMDocument - Win64 ReleaseCLEAN" : 
   cd "..\CreateDOMDocument"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\CreateDOMDocument.mak" CFG="CreateDOMDocument - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"CreateDOMDocument - Win64 Debug" : 
   cd "..\CreateDOMDocument"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\CreateDOMDocument.mak" CFG="CreateDOMDocument - Win64 Debug" 
   cd "..\all"

"CreateDOMDocument - Win64 DebugCLEAN" : 
   cd "..\CreateDOMDocument"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\CreateDOMDocument.mak" CFG="CreateDOMDocument - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"DOMCount - Win64 Release" : 
   cd "..\DOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMCount.mak" CFG="DOMCount - Win64 Release" 
   cd "..\all"

"DOMCount - Win64 ReleaseCLEAN" : 
   cd "..\DOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMCount.mak" CFG="DOMCount - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"DOMCount - Win64 Debug" : 
   cd "..\DOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMCount.mak" CFG="DOMCount - Win64 Debug" 
   cd "..\all"

"DOMCount - Win64 DebugCLEAN" : 
   cd "..\DOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMCount.mak" CFG="DOMCount - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"DOMMemTest - Win64 Release" : 
   cd "..\DOMMemTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMMemTest.mak" CFG="DOMMemTest - Win64 Release" 
   cd "..\all"

"DOMMemTest - Win64 ReleaseCLEAN" : 
   cd "..\DOMMemTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMMemTest.mak" CFG="DOMMemTest - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"DOMMemTest - Win64 Debug" : 
   cd "..\DOMMemTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMMemTest.mak" CFG="DOMMemTest - Win64 Debug" 
   cd "..\all"

"DOMMemTest - Win64 DebugCLEAN" : 
   cd "..\DOMMemTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMMemTest.mak" CFG="DOMMemTest - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"DOMPrint - Win64 Release" : 
   cd "..\DOMPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMPrint.mak" CFG="DOMPrint - Win64 Release" 
   cd "..\all"

"DOMPrint - Win64 ReleaseCLEAN" : 
   cd "..\DOMPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMPrint.mak" CFG="DOMPrint - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"DOMPrint - Win64 Debug" : 
   cd "..\DOMPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMPrint.mak" CFG="DOMPrint - Win64 Debug" 
   cd "..\all"

"DOMPrint - Win64 DebugCLEAN" : 
   cd "..\DOMPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMPrint.mak" CFG="DOMPrint - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"DOMTest - Win64 Release" : 
   cd "..\DOMTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTest.mak" CFG="DOMTest - Win64 Release" 
   cd "..\all"

"DOMTest - Win64 ReleaseCLEAN" : 
   cd "..\DOMTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTest.mak" CFG="DOMTest - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"DOMTest - Win64 Debug" : 
   cd "..\DOMTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTest.mak" CFG="DOMTest - Win64 Debug" 
   cd "..\all"

"DOMTest - Win64 DebugCLEAN" : 
   cd "..\DOMTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTest.mak" CFG="DOMTest - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"EncodingTest - Win64 Release" : 
   cd "..\EncodingTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EncodingTest.mak" CFG="EncodingTest - Win64 Release" 
   cd "..\all"

"EncodingTest - Win64 ReleaseCLEAN" : 
   cd "..\EncodingTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EncodingTest.mak" CFG="EncodingTest - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"EncodingTest - Win64 Debug" : 
   cd "..\EncodingTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EncodingTest.mak" CFG="EncodingTest - Win64 Debug" 
   cd "..\all"

"EncodingTest - Win64 DebugCLEAN" : 
   cd "..\EncodingTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EncodingTest.mak" CFG="EncodingTest - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"EnumVal - Win64 Release" : 
   cd "..\EnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EnumVal.mak" CFG="EnumVal - Win64 Release" 
   cd "..\all"

"EnumVal - Win64 ReleaseCLEAN" : 
   cd "..\EnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EnumVal.mak" CFG="EnumVal - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"EnumVal - Win64 Debug" : 
   cd "..\EnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EnumVal.mak" CFG="EnumVal - Win64 Debug" 
   cd "..\all"

"EnumVal - Win64 DebugCLEAN" : 
   cd "..\EnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\EnumVal.mak" CFG="EnumVal - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"InitTermTest - Win64 Release" : 
   cd "..\InitTermTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\InitTermTest.mak" CFG="InitTermTest - Win64 Release" 
   cd "..\all"

"InitTermTest - Win64 ReleaseCLEAN" : 
   cd "..\InitTermTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\InitTermTest.mak" CFG="InitTermTest - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"InitTermTest - Win64 Debug" : 
   cd "..\InitTermTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\InitTermTest.mak" CFG="InitTermTest - Win64 Debug" 
   cd "..\all"

"InitTermTest - Win64 DebugCLEAN" : 
   cd "..\InitTermTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\InitTermTest.mak" CFG="InitTermTest - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"MemParse - Win64 Release" : 
   cd "..\MemParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\MemParse.mak" CFG="MemParse - Win64 Release" 
   cd "..\all"

"MemParse - Win64 ReleaseCLEAN" : 
   cd "..\MemParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\MemParse.mak" CFG="MemParse - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"MemParse - Win64 Debug" : 
   cd "..\MemParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\MemParse.mak" CFG="MemParse - Win64 Debug" 
   cd "..\all"

"MemParse - Win64 DebugCLEAN" : 
   cd "..\MemParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\MemParse.mak" CFG="MemParse - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"PParse - Win64 Release" : 
   cd "..\PParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\PParse.mak" CFG="PParse - Win64 Release" 
   cd "..\all"

"PParse - Win64 ReleaseCLEAN" : 
   cd "..\PParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\PParse.mak" CFG="PParse - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"PParse - Win64 Debug" : 
   cd "..\PParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\PParse.mak" CFG="PParse - Win64 Debug" 
   cd "..\all"

"PParse - Win64 DebugCLEAN" : 
   cd "..\PParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\PParse.mak" CFG="PParse - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"RangeTest - Win64 Release" : 
   cd "..\RangeTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\RangeTest.mak" CFG="RangeTest - Win64 Release" 
   cd "..\all"

"RangeTest - Win64 ReleaseCLEAN" : 
   cd "..\RangeTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\RangeTest.mak" CFG="RangeTest - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"RangeTest - Win64 Debug" : 
   cd "..\RangeTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\RangeTest.mak" CFG="RangeTest - Win64 Debug" 
   cd "..\all"

"RangeTest - Win64 DebugCLEAN" : 
   cd "..\RangeTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\RangeTest.mak" CFG="RangeTest - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"Redirect - Win64 Release" : 
   cd "..\Redirect"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\Redirect.mak" CFG="Redirect - Win64 Release" 
   cd "..\all"

"Redirect - Win64 ReleaseCLEAN" : 
   cd "..\Redirect"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\Redirect.mak" CFG="Redirect - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"Redirect - Win64 Debug" : 
   cd "..\Redirect"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\Redirect.mak" CFG="Redirect - Win64 Debug" 
   cd "..\all"

"Redirect - Win64 DebugCLEAN" : 
   cd "..\Redirect"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\Redirect.mak" CFG="Redirect - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"SAX2Count - Win64 Release" : 
   cd "..\SAX2Count"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Count.mak" CFG="SAX2Count - Win64 Release" 
   cd "..\all"

"SAX2Count - Win64 ReleaseCLEAN" : 
   cd "..\SAX2Count"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Count.mak" CFG="SAX2Count - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"SAX2Count - Win64 Debug" : 
   cd "..\SAX2Count"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Count.mak" CFG="SAX2Count - Win64 Debug" 
   cd "..\all"

"SAX2Count - Win64 DebugCLEAN" : 
   cd "..\SAX2Count"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Count.mak" CFG="SAX2Count - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"SAX2Print - Win64 Release" : 
   cd "..\SAX2Print"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Print.mak" CFG="SAX2Print - Win64 Release" 
   cd "..\all"

"SAX2Print - Win64 ReleaseCLEAN" : 
   cd "..\SAX2Print"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Print.mak" CFG="SAX2Print - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"SAX2Print - Win64 Debug" : 
   cd "..\SAX2Print"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Print.mak" CFG="SAX2Print - Win64 Debug" 
   cd "..\all"

"SAX2Print - Win64 DebugCLEAN" : 
   cd "..\SAX2Print"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAX2Print.mak" CFG="SAX2Print - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"SAXCount - Win64 Release" : 
   cd "..\SAXCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXCount.mak" CFG="SAXCount - Win64 Release" 
   cd "..\all"

"SAXCount - Win64 ReleaseCLEAN" : 
   cd "..\SAXCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXCount.mak" CFG="SAXCount - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"SAXCount - Win64 Debug" : 
   cd "..\SAXCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXCount.mak" CFG="SAXCount - Win64 Debug" 
   cd "..\all"

"SAXCount - Win64 DebugCLEAN" : 
   cd "..\SAXCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXCount.mak" CFG="SAXCount - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"SAXPrint - Win64 Release" : 
   cd "..\SAXPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXPrint.mak" CFG="SAXPrint - Win64 Release" 
   cd "..\all"

"SAXPrint - Win64 ReleaseCLEAN" : 
   cd "..\SAXPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXPrint.mak" CFG="SAXPrint - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"SAXPrint - Win64 Debug" : 
   cd "..\SAXPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXPrint.mak" CFG="SAXPrint - Win64 Debug" 
   cd "..\all"

"SAXPrint - Win64 DebugCLEAN" : 
   cd "..\SAXPrint"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SAXPrint.mak" CFG="SAXPrint - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"SEnumVal - Win64 Release" : 
   cd "..\SEnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SEnumVal.mak" CFG="SEnumVal - Win64 Release" 
   cd "..\all"

"SEnumVal - Win64 ReleaseCLEAN" : 
   cd "..\SEnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SEnumVal.mak" CFG="SEnumVal - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"SEnumVal - Win64 Debug" : 
   cd "..\SEnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SEnumVal.mak" CFG="SEnumVal - Win64 Debug" 
   cd "..\all"

"SEnumVal - Win64 DebugCLEAN" : 
   cd "..\SEnumVal"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\SEnumVal.mak" CFG="SEnumVal - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"StdInParse - Win64 Release" : 
   cd "..\StdInParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\StdInParse.mak" CFG="StdInParse - Win64 Release" 
   cd "..\all"

"StdInParse - Win64 ReleaseCLEAN" : 
   cd "..\StdInParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\StdInParse.mak" CFG="StdInParse - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"StdInParse - Win64 Debug" : 
   cd "..\StdInParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\StdInParse.mak" CFG="StdInParse - Win64 Debug" 
   cd "..\all"

"StdInParse - Win64 DebugCLEAN" : 
   cd "..\StdInParse"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\StdInParse.mak" CFG="StdInParse - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"ThreadTest - Win64 Release" : 
   cd "..\ThreadTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\ThreadTest.mak" CFG="ThreadTest - Win64 Release" 
   cd "..\all"

"ThreadTest - Win64 ReleaseCLEAN" : 
   cd "..\ThreadTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\ThreadTest.mak" CFG="ThreadTest - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"ThreadTest - Win64 Debug" : 
   cd "..\ThreadTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\ThreadTest.mak" CFG="ThreadTest - Win64 Debug" 
   cd "..\all"

"ThreadTest - Win64 DebugCLEAN" : 
   cd "..\ThreadTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\ThreadTest.mak" CFG="ThreadTest - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"XercesLib - Win64 Release" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" 
   cd "..\all"

"XercesLib - Win64 ReleaseCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"XercesLib - Win64 Debug" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" 
   cd "..\all"

"XercesLib - Win64 DebugCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"DOMTraversalTest - Win64 Release" : 
   cd "..\DOMTraversalTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTraversalTest.mak" CFG="DOMTraversalTest - Win64 Release" 
   cd "..\all"

"DOMTraversalTest - Win64 ReleaseCLEAN" : 
   cd "..\DOMTraversalTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTraversalTest.mak" CFG="DOMTraversalTest - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"DOMTraversalTest - Win64 Debug" : 
   cd "..\DOMTraversalTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTraversalTest.mak" CFG="DOMTraversalTest - Win64 Debug" 
   cd "..\all"

"DOMTraversalTest - Win64 DebugCLEAN" : 
   cd "..\DOMTraversalTest"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DOMTraversalTest.mak" CFG="DOMTraversalTest - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 

!IF  "$(CFG)" == "all - Win64 Release"

"DeprecatedDOMCount - Win64 Release" : 
   cd "..\DeprecatedDOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DeprecatedDOMCount.mak" CFG="DeprecatedDOMCount - Win64 Release" 
   cd "..\all"

"DeprecatedDOMCount - Win64 ReleaseCLEAN" : 
   cd "..\DeprecatedDOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DeprecatedDOMCount.mak" CFG="DeprecatedDOMCount - Win64 Release" RECURSE=1 CLEAN 
   cd "..\all"

!ELSEIF  "$(CFG)" == "all - Win64 Debug"

"DeprecatedDOMCount - Win64 Debug" : 
   cd "..\DeprecatedDOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DeprecatedDOMCount.mak" CFG="DeprecatedDOMCount - Win64 Debug" 
   cd "..\all"

"DeprecatedDOMCount - Win64 DebugCLEAN" : 
   cd "..\DeprecatedDOMCount"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\DeprecatedDOMCount.mak" CFG="DeprecatedDOMCount - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\all"

!ENDIF 


!ENDIF 

