# Microsoft Developer Studio Generated NMAKE File, Based on StdInParse.dsp
!IF "$(CFG)" == ""
CFG=StdInParse - Win64 Debug
!MESSAGE No configuration specified. Defaulting to StdInParse - Win64 Debug.
!ENDIF 

!IF "$(CFG)" != "StdInParse - Win64 Release" && "$(CFG)" != "StdInParse - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StdInParse.mak" CFG="StdInParse - Win64 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StdInParse - Win64 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "StdInParse - Win64 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "StdInParse - Win64 Release"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\StdInParse.exe"

!ELSE 

ALL : "XercesLib - Win64 Release" "$(OUTDIR)\StdInParse.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdInParse.obj"
	-@erase "$(INTDIR)\StdInParseHandlers.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\StdInParse.exe"
	-@erase "$(OUTDIR)\StdInParse.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=$(CPP)
CPP_PROJ=/MD /W3 /GX /O2 /Ob2 /I "..\..\..\..\..\src" /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "WIN64" /D "_WINDOWS" /D "PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
#BSC32=bscmake.exe
BSC32_FLAGS=/o"$(OUTDIR)\StdInParse.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2.lib /version:1.0 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\StdInParse.pdb" /machine:IX86 /out:"$(OUTDIR)\StdInParse.exe" /libpath:"..\..\..\..\..\Build\Win64\Nmake\Release" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\StdInParse.obj" \
	"$(INTDIR)\StdInParseHandlers.obj" \
	"$(OUTDIR)\xerces-c_2.lib"

"$(OUTDIR)\StdInParse.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "StdInParse - Win64 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\StdInParse.exe"

!ELSE 

ALL : "XercesLib - Win64 Debug" "$(OUTDIR)\StdInParse.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\StdInParse.obj"
	-@erase "$(INTDIR)\StdInParseHandlers.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\StdInParse.exe"
	-@erase "$(OUTDIR)\StdInParse.ilk"
	-@erase "$(OUTDIR)\StdInParse.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=$(CPP)
CPP_PROJ=/MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\src" /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "WIN64" /D "_WINDOWS" /D "PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
#BSC32=bscmake.exe
BSC32_FLAGS=/o"$(OUTDIR)\StdInParse.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2D.lib /version:3.0 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\StdInParse.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\StdInParse.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Build\Win64\Nmake\Debug" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\StdInParse.obj" \
	"$(INTDIR)\StdInParseHandlers.obj" \
	"$(OUTDIR)\xerces-c_2D.lib"

"$(OUTDIR)\StdInParse.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("StdInParse.dep")
!INCLUDE "StdInParse.dep"
!ELSE 
!MESSAGE Warning: cannot find "StdInParse.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "StdInParse - Win64 Release" || "$(CFG)" == "StdInParse - Win64 Debug"
SOURCE=..\..\..\..\..\samples\StdInParse\StdInParse.cpp

"$(INTDIR)\StdInParse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\samples\StdInParse\StdInParseHandlers.cpp

"$(INTDIR)\StdInParseHandlers.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "StdInParse - Win64 Release"

"XercesLib - Win64 Release" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" 
   cd "..\StdInParse"

"XercesLib - Win64 ReleaseCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" RECURSE=1 CLEAN 
   cd "..\StdInParse"

!ELSEIF  "$(CFG)" == "StdInParse - Win64 Debug"

"XercesLib - Win64 Debug" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" 
   cd "..\StdInParse"

"XercesLib - Win64 DebugCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\StdInParse"

!ENDIF 


!ENDIF 

