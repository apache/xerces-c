# Microsoft Developer Studio Generated NMAKE File, Based on SAX2Count.dsp
!IF "$(CFG)" == ""
CFG=SAX2Count - Win64 Debug
!MESSAGE No configuration specified. Defaulting to SAX2Count - Win64 Debug.
!ENDIF 

!IF "$(CFG)" != "SAX2Count - Win64 Release" && "$(CFG)" != "SAX2Count - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SAX2Count.mak" CFG="SAX2Count - Win64 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SAX2Count - Win64 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SAX2Count - Win64 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "SAX2Count - Win64 Release"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SAX2Count.exe"

!ELSE 

ALL : "XercesLib - Win64 Release" "$(OUTDIR)\SAX2Count.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\SAX2Count.obj"
	-@erase "$(INTDIR)\SAX2CountHandlers.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SAX2Count.exe"
	-@erase "$(OUTDIR)\SAX2Count.ilk"

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
BSC32_FLAGS=/o"$(OUTDIR)\SAX2Count.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2.lib /version:1.0 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\SAX2Count.pdb" /machine:IX86 /out:"$(OUTDIR)\SAX2Count.exe" /libpath:"..\..\..\..\..\Build\Win64\Nmake\Release" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\SAX2Count.obj" \
	"$(INTDIR)\SAX2CountHandlers.obj" \
	"$(OUTDIR)\xerces-c_2.lib"

"$(OUTDIR)\SAX2Count.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SAX2Count - Win64 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\SAX2Count.exe"

!ELSE 

ALL : "XercesLib - Win64 Debug" "$(OUTDIR)\SAX2Count.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\SAX2Count.obj"
	-@erase "$(INTDIR)\SAX2CountHandlers.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SAX2Count.exe"
	-@erase "$(OUTDIR)\SAX2Count.ilk"
	-@erase "$(OUTDIR)\SAX2Count.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP=$(CPP)
CPP_PROJ=/MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\src" /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "WIN64" /D "_WINDOWS" /D "PLATFORM_WIN32" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/o"$(OUTDIR)\SAX2Count.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2D.lib /version:1.0 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\SAX2Count.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\SAX2Count.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Build\Win64\Nmake\Debug" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\SAX2Count.obj" \
	"$(INTDIR)\SAX2CountHandlers.obj" \
	"$(OUTDIR)\xerces-c_2D.lib"

"$(OUTDIR)\SAX2Count.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("SAX2Count.dep")
!INCLUDE "SAX2Count.dep"
!ELSE 
!MESSAGE Warning: cannot find "SAX2Count.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SAX2Count - Win64 Release" || "$(CFG)" == "SAX2Count - Win64 Debug"

!IF  "$(CFG)" == "SAX2Count - Win64 Release"

"XercesLib - Win64 Release" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" 
   cd "..\SAX2Count"

"XercesLib - Win64 ReleaseCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" RECURSE=1 CLEAN 
   cd "..\SAX2Count"

!ELSEIF  "$(CFG)" == "SAX2Count - Win64 Debug"

"XercesLib - Win64 Debug" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" 
   cd "..\SAX2Count"

"XercesLib - Win64 DebugCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\SAX2Count"

!ENDIF 

SOURCE=..\..\..\..\..\samples\SAX2Count\SAX2Count.cpp

"$(INTDIR)\SAX2Count.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\..\..\samples\SAX2Count\SAX2CountHandlers.cpp

"$(INTDIR)\SAX2CountHandlers.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

