# Microsoft Developer Studio Generated NMAKE File, Based on PParse.dsp
!IF "$(CFG)" == ""
CFG=PParse - Win64 Debug
!MESSAGE No configuration specified. Defaulting to PParse - Win64 Debug.
!ENDIF 

!IF "$(CFG)" != "PParse - Win64 Release" && "$(CFG)" != "PParse - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PParse.mak" CFG="PParse - Win64 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PParse - Win64 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PParse - Win64 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "PParse - Win64 Release"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\PParse.exe"

!ELSE 

ALL : "XercesLib - Win64 Release" "$(OUTDIR)\PParse.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\PParse.obj"
	-@erase "$(INTDIR)\PParseHandlers.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\PParse.exe"
	-@erase "$(OUTDIR)\PParse.ilk"

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
BSC32_FLAGS=/o"$(OUTDIR)\PParse.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2.lib /version:1.0 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\PParse.pdb" /machine:IX86 /out:"$(OUTDIR)\PParse.exe" /libpath:"..\..\..\..\..\Build\Win64\Nmake\Release" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\PParse.obj" \
	"$(INTDIR)\PParseHandlers.obj" \
	"$(OUTDIR)\xerces-c_2.lib"

"$(OUTDIR)\PParse.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "PParse - Win64 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\PParse.exe"

!ELSE 

ALL : "XercesLib - Win64 Debug" "$(OUTDIR)\PParse.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\PParse.obj"
	-@erase "$(INTDIR)\PParseHandlers.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\PParse.exe"
	-@erase "$(OUTDIR)\PParse.ilk"
	-@erase "$(OUTDIR)\PParse.pdb"

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
BSC32_FLAGS=/o"$(OUTDIR)\PParse.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2D.lib /version:1.0 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\PParse.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\PParse.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Build\Win64\Nmake\Debug" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\PParse.obj" \
	"$(INTDIR)\PParseHandlers.obj" \
	"$(OUTDIR)\xerces-c_2D.lib"

"$(OUTDIR)\PParse.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("PParse.dep")
!INCLUDE "PParse.dep"
!ELSE 
!MESSAGE Warning: cannot find "PParse.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "PParse - Win64 Release" || "$(CFG)" == "PParse - Win64 Debug"
SOURCE=..\..\..\..\..\samples\PParse\PParse.cpp

"$(INTDIR)\PParse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\samples\PParse\PParseHandlers.cpp

"$(INTDIR)\PParseHandlers.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "PParse - Win64 Release"

"XercesLib - Win64 Release" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" 
   cd "..\PParse"

"XercesLib - Win64 ReleaseCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" RECURSE=1 CLEAN 
   cd "..\PParse"

!ELSEIF  "$(CFG)" == "PParse - Win64 Debug"

"XercesLib - Win64 Debug" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" 
   cd "..\PParse"

"XercesLib - Win64 DebugCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\PParse"

!ENDIF 


!ENDIF 

