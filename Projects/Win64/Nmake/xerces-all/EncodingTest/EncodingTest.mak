# Microsoft Developer Studio Generated NMAKE File, Based on EncodingTest.dsp
!IF "$(CFG)" == ""
CFG=EncodingTest - Win64 Debug
!MESSAGE No configuration specified. Defaulting to EncodingTest - Win64 Debug.
!ENDIF 

!IF "$(CFG)" != "EncodingTest - Win64 Release" && "$(CFG)" != "EncodingTest - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EncodingTest.mak" CFG="EncodingTest - Win64 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EncodingTest - Win64 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "EncodingTest - Win64 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "EncodingTest - Win64 Release"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Release\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\EncodingTest.exe"

!ELSE 

ALL : "XercesLib - Win64 Release" "$(OUTDIR)\EncodingTest.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\EncodingTest.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\EncodingTest.exe"

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
BSC32_FLAGS=/o"$(OUTDIR)\EncodingTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2.lib /version:1.0 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\EncodingTest.pdb" /machine:IX86 /out:"$(OUTDIR)\EncodingTest.exe" /libpath:"..\..\..\..\..\Build\Win64\Nmake\Release" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\EncodingTest.obj" \
	"$(OUTDIR)\xerces-c_2.lib"

"$(OUTDIR)\EncodingTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "EncodingTest - Win64 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug
INTDIR=.\..\..\..\..\..\Build\Win64\Nmake\Debug\obj
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\Nmake\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\EncodingTest.exe"

!ELSE 

ALL : "XercesLib - Win64 Debug" "$(OUTDIR)\EncodingTest.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"XercesLib - Win64 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\EncodingTest.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\EncodingTest.exe"
	-@erase "$(OUTDIR)\EncodingTest.ilk"
	-@erase "$(OUTDIR)\EncodingTest.pdb"

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
BSC32_FLAGS=/o"$(OUTDIR)\EncodingTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_2D.lib /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\EncodingTest.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\EncodingTest.exe" /pdbtype:sept /libpath:"..\..\..\..\..\Build\Win64\Nmake\Debug" /machine:IA64 
LINK32_OBJS= \
	"$(INTDIR)\EncodingTest.obj" \
	"$(OUTDIR)\xerces-c_2D.lib"

"$(OUTDIR)\EncodingTest.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("EncodingTest.dep")
!INCLUDE "EncodingTest.dep"
!ELSE 
!MESSAGE Warning: cannot find "EncodingTest.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "EncodingTest - Win64 Release" || "$(CFG)" == "EncodingTest - Win64 Debug"
SOURCE=..\..\..\..\..\tests\EncodingTest\EncodingTest.cpp

"$(INTDIR)\EncodingTest.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "EncodingTest - Win64 Release"

"XercesLib - Win64 Release" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" 
   cd "..\EncodingTest"

"XercesLib - Win64 ReleaseCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Release" RECURSE=1 CLEAN 
   cd "..\EncodingTest"

!ELSEIF  "$(CFG)" == "EncodingTest - Win64 Debug"

"XercesLib - Win64 Debug" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" 
   cd "..\EncodingTest"

"XercesLib - Win64 DebugCLEAN" : 
   cd "..\XercesLib"
   $(MAKE) CPP=$(CPP) /$(MAKEFLAGS) /F ".\XercesLib.mak" CFG="XercesLib - Win64 Debug" RECURSE=1 CLEAN 
   cd "..\EncodingTest"

!ENDIF 


!ENDIF 

