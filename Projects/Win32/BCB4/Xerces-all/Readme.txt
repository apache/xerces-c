$Id$

This directory and its children contain the project group
and the project files for building Xerces for C++-Builder 4.

In order to build and use it, it is necessary to have C++-Builder 4
installed.  For using xerces from within C++-Builder projects
C++-Builder made libraries are needed because of the library format and
the name mangling of C++ functions (this can be compared by viewing
the DLLs with right mouse - properties - fast view - Export table
section).

For building from the C++Builder IDE...

+ Load the Xerces-ALL.BPG Project Group in the IDE.

+ Select Project | Build All Projects.

  Notes:
   If you change any of the Options for the individual projects,
   the final output path WILL get corrupted.  Additionally, if C++Builder
   is installed on the same drive as the Xerces source files,
   other paths in the .BPR project files may also get corrupted.
   Borland has acknowledged these problems but will not fix them.

For building from the command line...

+ Run the MakeBuildDirs.bat to create the output directories.

+ ChDir to each of the individual project directories (starting
  with XercesLib of course) and run the Borland make utility
  (e.g. "make -B -f XercesLib.bpr")

Either way, the output (.dll, .lib, .exe) files will be created
under Build\Win32\BCB4 (Build is on the same level as src).

-- In Win32PlatformUtils.cpp, the compareAndSwap method uses inline
assembler to avoid an API not suported in Windows 95.
If you don't need Win95 support, changing it to  #if 0  will greatly
improve your build times.