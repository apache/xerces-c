# The Apache Software License, Version 1.1
#
# Copyright (c) 2003 The Apache Software Foundation.  All rights
# reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# 3. The end-user documentation included with the redistribution,
#    if any, must include the following acknowledgment:
#       "This product includes software developed by the
#        Apache Software Foundation (http://www.apache.org/)."
#    Alternately, this acknowledgment may appear in the software itself,
#    if and wherever such third-party acknowledgments normally appear.
#
# 4. The names "Xerces" and "Apache Software Foundation" must
#    not be used to endorse or promote products derived from this
#    software without prior written permission. For written
#    permission, please contact apache\@apache.org.
#
# 5. Products derived from this software may not be called "Apache",
#    nor may "Apache" appear in their name, without prior written
#    permission of the Apache Software Foundation.
#
# THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
# ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
# ====================================================================
#
# This software consists of voluntary contributions made by many
# individuals on behalf of the Apache Software Foundation, and was
# originally based on software copyright (c) 1999, International
# Business Machines, Inc., http://www.ibm.com .  For more information
# on the Apache Software Foundation, please see
# <http://www.apache.org/>.
#
#
#
# REVISIT:  this should really be generalized to support platforms
# other than Windows/MSVC++ and Linux/gcc.  However, the portability of 
# wide character I/O is very non-portable; so this is a nontrivial 
# prospect.  That's why this Makefile is provided only for gcc
# at the moment.

XLAT_CPP_OBJECTS = \
    Xlat.o \
    Xlat_CppSrc.o \
    Xlat_ICUResourceBundle.o \
    Xlat_MsgCatalog.o \
    Xlat_Win32RC.o


PLATFORM = LINUX
COMPILER = g++
GCC = yes
GXX = yes
CXXFLAGS =   -w -O  -D_GNU_SOURCE
CFLAGS =   -w -O  -D_GNU_SOURCE
LDFLAGS =    
EXTRA_LIBS =  
LIBRARY_SEARCH_PATHS=-L../../../lib/
LIBRARY_NAMES=-lxerces-c
INCLUDES=-I../../../include
CMP= -c ${CXXFLAGS}
CC= ${COMPILER} -c -D_REENTRANT -fpic
LINK = ${COMPILER} -fpic ${LDFLAGS}
EXTRA_LINK_OPTIONS=-lc ${EXTRA_LIBS}
APP_NAME=NLSXlat

all:: ${APP_NAME}

${APP_NAME}:: ${XLAT_CPP_OBJECTS}
	${LINK} ${XLAT_CPP_OBJECTS} -o ${APP_NAME} ${LIBRARY_SEARCH_PATHS} ${LIBRARY_NAMES} ${EXTRA_LINK_OPTIONS}

Xlat.o:: Xlat.cpp Xlat_ErrHandler.hpp Xlat_Formatter.hpp Xlat_Types.hpp
	${CC} ${INCLUDES} ${CMP} -o Xlat.o Xlat.cpp 
Xlat_CppSrc.o:: Xlat_CppSrc.cpp Xlat_CppSrc.hpp 
	${CC} ${INCLUDES} ${CMP} -o Xlat_CppSrc.o Xlat_CppSrc.cpp 
Xlat_ICUResourceBundle.o:: Xlat_ICUResourceBundle.cpp Xlat_ICUResourceBundle.hpp 
	${CC} ${INCLUDES} ${CMP} -o Xlat_ICUResourceBundle.o Xlat_ICUResourceBundle.cpp
Xlat_MsgCatalog.o:: Xlat_MsgCatalog.cpp Xlat_MsgCatalog.hpp 
	${CC} ${INCLUDES} ${CMP} -o Xlat_MsgCatalog.o Xlat_MsgCatalog.cpp
Xlat_Win32RC.o:: Xlat_Win32RC.cpp Xlat_Win32RC.hpp
	${CC} ${INCLUDES} ${CMP} -o Xlat_Win32RC.o Xlat_Win32RC.cpp

clean::
	rm -f ${XLAT_CPP_OBJECTS} ${APP_NAME}

