$Log$
Revision 1.1  2000/02/03 23:46:37  rahulj
Initial checkin. Contributed by Dietrich Wolf.


This directory contains the project group and the project files for
building Xerces for C++-Builder 4.

In order to build and use it, it is necessary to have C++-Builder 4
installed.  For using xerces from within C++-Builder projects
C++-Builder made libraries are needed because of the library format and
the name mangling of C++ functions (this can be compared by viewing
the DLLs with right mouse - properties - fast view - Export table
section).

The projects can be compiled from within the IDE (Project|Build) or
from the command line by using make (Borland make !).

The results (.dll, .lib, .exe) are under Build\Win32\BCB4.
