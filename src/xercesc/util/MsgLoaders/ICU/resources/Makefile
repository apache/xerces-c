#
# Copyright (c) 2002 IBM, Inc. 
#
#  File:     Makefile
#  Location: <xercesc_root>\src\xercesc\util\MsgLoaders\ICU\resources
#
#  UNIX makefile for compiling (and packaging) the resources
#  for ICU message loader.
#
#  List of resource files to be built.
#
#    . When adding a resource source (.txt) file for a new locale, 
#           the corresponding .res file must be added to this list, 
#    . AND to the file res-file-list.txt if *dll is to be built
#
#

PKGNAME  = XercescErrMsg
TARGETS  = $(PKGNAME)_root.res $(PKGNAME)_en.res  $(PKGNAME)_fr.res
ICUDIR   = ../../../../../../../icu
GENRB    = $(ICUDIR)/bin/genrb.exe
GENRBOPT = -s. -d. --package-name $(PKGNAME)  

all : $(TARGETS)
	@echo All targets are up to date

clean : 
	-erase $(TARGETS)

$(PKGNAME)_root.res : root.txt
	$(GENRB) $(GENRBOPT) $?
    
$(PKGNAME)_en.res : en.txt
	$(GENRB) $(GENRBOPT) $?

#
# Note: $(GENRB) $(GENRBOPT) --encoding cp1251 $?
#
$(PKGNAME)_fr.res : fr.txt
	$(GENRB) $(GENRBOPT) $?
	

















