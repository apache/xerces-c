Feb 27, 2003 - Xerces C and Borland C++ Compiler v 5.5.1
========================================================

 - changes for Xerces C 2.2.0

How to build dll with Win95 support (tasm32 required, so only for BCB5):
	make -f Xerces-all.mak -DWITHASM=Y

Aug 23, 2002 - Xerces C and Borland C++ Compiler v 5.5.1
========================================================

Before build:
	MakeBuildDirs.bat

How to build dll (without deprecated DOM API) and tests:
	make -f Xerces-all.mak

How to build dll (with deprecated DOM API, approx. 300k larger) and tests:
	make -f Xerces-all.mak -DWITHDEPRDOM=Y

Vitaly Prapirny (marl@mebius.net)
