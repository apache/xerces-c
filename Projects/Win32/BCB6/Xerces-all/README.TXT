Feb 7, 2002 - Xerces C and Borland C++Builder6
==============================================

The enclosed project file no longer produce a Xerces library that is dependent upon
the Borland memory manager (borlndmm.dll).  However there is still a dependency upon
the multi-threaded C++ Run Time Library.  The RTL is named cc3260mt.dll and is
included in this distribution for your convenience.

The supplied project files support Borland C++Builder6.  The overwhelming size of the
Xerces project will cause some problems for the new IDE.  The version 6 IDE appends
the source file directories to the include and lib paths, even though it is not
necessary.  The resulting project file becomes corrupted due to the unnatural line
lengths.  The file cannot be edited by hand to correct the situation, as the IDE will
simply "fix" the paths for you when it opens the project.  This will discovered late
in BCB6 field test, at a point in time when it was impossible to alter the project
options dialog box.  A work around was to have the IDE check a registry entry that
would prohibit it from modifying paths.  Set the following value in the registry:

[HKEY_CURRENT_USER\Software\Borland\C++Builder\6.0\Globals]
"DontManagePaths"="1"

or simply double click on the .reg file in this directory.

Don Mastrovito