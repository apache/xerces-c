#
# Copyright (c) 2003 IBM, Inc.
#
#  File:     resources.mak
#  Location: <xercesc_root>\src\xercesc\util\MsgLoaders\ICU\resources
#
#  Windows nmake makefile for compiling (and packaging) the resources
#  for ICU message loader.
#
#  List of resource files to be built.
#
#    . When adding a resource source (.txt) file for a new locale,
#           the corresponding .res file must be added to this list,
#    . AND to the file res-file-list.txt
#
#  If built versioned dll/lib directly, then we need to change the
#  the symbol, XercesMessages_dat to XercesMessages2_2_0_dat.
#

# for VER
include ..\..\..\..\..\..\version.incl

RESFILES= en_US.res 

PKGNAME       = XercesMessages
TARGET_DLL    = $(PKGNAME).DLL
TARGET_LIB    = $(PKGNAME).lib
TAR_VER_DLL   = $(PKGNAME)$(VER).dll
TAR_VER_LIB   = $(PKGNAME)$(VER).lib

GENRB    = $(ICUROOT)\bin\genrb.exe
PKGDATA  = $(ICUROOT)\bin\pkgdata
REN      = ren

#
#  File name extensions for inference rule matching.
#    clear out the built-in ones (for .c and the like), and add
#    the definition for .txt to .res.
#
.SUFFIXES :
.SUFFIXES : .txt

#
#  Inference rule, for compiling a .txt file into a .res file.
#  -t fools make into thinking there are files such as es.res, etc
#
.txt.res:
	$(GENRB) -t --package-name $(PKGNAME) -d . $*.txt

#
#  all - nmake starts here by default
#
all: $(TARGET_DLL)

$(TARGET_DLL): $(RESFILES)
	$(PKGDATA) --name $(PKGNAME) -v -O R:$(ICUROOT) --mode dll -d . res-file-list.txt
	$(REN) $(TARGET_DLL) $(TAR_VER_DLL)
	$(REN) $(TARGET_LIB) $(TAR_VER_LIB)
