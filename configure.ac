#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
#

#
# Copyright 2005 The Apache Software Foundation.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# $Id$
#

AC_PREREQ(2.60)
AC_INIT([xercesc],[3.0])
AC_CONFIG_SRCDIR([src/Makefile.am])
AC_CONFIG_LIBOBJ_DIR([lib])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h src/xercesc/util/Xerces_autoconf_config.hpp])
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile src/xercesc/util/MsgLoaders/ICU/resources/Makefile obj/Makefile tests/Makefile samples/Makefile])

# Check the target system
AC_CANONICAL_HOST

# Initialize automake
AM_INIT_AUTOMAKE([foreign subdir-objects dist-bzip2 tar-ustar])
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# If using MSVC (under Cygwin) or Solaris Forte, and the user didn't specify a linker, force a reasonable
# choice in order to avoid the "cannot find 'ld'" error from the AC_PROG_LIBTOOL
if test x$LD = x; then
	case $CXX in
	'cl.exe' | 'cl')
		LD='link'
		;;
	'CC')
		LD='CC'
		;;
	esac
fi

AC_PROG_LN_S
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Checks for libraries.
AC_CHECK_LIB([socket], [socket])
AC_CHECK_LIB([nsl], [gethostbyname])

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h langinfo.h limits.h locale.h \
				memory.h netdb.h netinet/in.h nl_types.h stddef.h stdint.h stdlib.h \
				string.h strings.h \
				sys/param.h sys/socket.h sys/time.h sys/timeb.h \
				unistd.h wchar.h wctype.h \
				CoreServices/CoreServices.h \
				])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE

AC_CHECK_SIZEOF(wchar_t)
AC_CHECK_TYPE(size_t)
AC_CHECK_TYPE(ssize_t)

AC_TYPE_OFF_T
AC_TYPE_SIZE_T

XERCES_INT_TYPES

AC_CXX_HAVE_BOOL
AC_CXX_HAVE_NAMESPACES
AC_CXX_HAVE_STD_NAMESPACE
AC_CXX_HAVE_STD_LIBS
AC_CXX_HAVE_LSTRING

ACX_PTHREAD

# Checks for library functions.
#AC_FUNC_ERROR_AT_LINE
#AC_FUNC_MALLOC
#AC_FUNC_MEMCMP
#AC_FUNC_STRCOLL
#AC_FUNC_STRTOD
AC_CHECK_FUNCS([clock_gettime ftime getcwd gethostbyaddr gethostbyname gettimeofday localeconv \
				mblen memmove memset nl_langinfo pathconf realpath setlocale socket \
				strcasecmp strncasecmp stricmp strnicmp strchr strdup \
				strrchr strstr strtol strtoul \
				towupper towlower \
				mbrlen wcsrtombs mbsrtowcs \
				])
				
AC_SUBST([SHREXT], [$shrext_cmds])

#
# Orchestrate the replacement of missing functions
#
AC_REPLACE_FUNCS([stricmp strnicmp towlower towupper])


######################################################
# Look for availability of available packages
######################################################

XERCES_PRETTY_MAKE
XERCES_PATH_DELIMITERS

XERCES_CURL_PREFIX
XERCES_LIBWWW_PREFIX

XERCES_NETACCESSOR_SELECTION
XERCES_TRANSCODER_SELECTION
XERCES_MSGLOADER_SELECTION
XERCES_FILEMGR_SELECTION
XERCES_MUTEXMGR_SELECTION
XERCES_ATOMICOPMGR_SELECTION

######################################################
# Define some namespace-protected macros for use in the publicly visible
# XercesConfig.h file.
######################################################
AC_DEFINE([XERCES_AUTOCONF], 1, [Define to true if autoconf is used in this configuration])

AS_IF([test x$ac_cv_header_sys_types_h = xyes],
	AC_DEFINE([XERCES_HAVE_SYS_TYPES_H], 1, [Define to 1 if we have sys/types.h]))
AS_IF([test x$ac_cv_header_inttypes_h = xyes],
	AC_DEFINE([XERCES_HAVE_INTTYPES_H],    1, [Define to 1 if we have inttypes.h]))

AC_MSG_CHECKING([whether we are using SunPro 5.8 compiler])
AC_EGREP_CPP([0x580], [__SUNPRO_C],
	[
	 COMPILER="SUNPRO"
	 AC_MSG_RESULT([yes])
	],
	[
	 COMPILER=""
	 AC_MSG_RESULT([no])
	]
	)

case $CXX in
'cl.exe' | 'cl')
	CXXFLAGS="`echo $CXXFLAGS | $SED 's%-g%/Od%'` /nologo /GX"
	;;
esac

case $CC in
'cl.exe' | 'cl')
	CFLAGS="`echo $CFLAGS | $SED 's%-g%/Od%'` /nologo /GX"
	;;
esac

case $host in
*-*-msdos* | *-*-mingw32* | *-*-cygwin* | *-*-windows* )
	platform_export="__declspec(dllexport)"
	platform_import="__declspec(dllimport)"
	;; 
* )
	case $COMPILER in
	SUNPRO)
		platform_export="__declspec(dllexport)"
		platform_import="__declspec(dllimport)"
		;; 
	*)
		platform_export=""
		platform_import=""
		;; 
	esac
esac
AC_DEFINE_UNQUOTED([XERCES_PLATFORM_EXPORT], [$platform_export], [Define as the platform's export attribute])
AC_DEFINE_UNQUOTED([XERCES_PLATFORM_IMPORT], [$platform_import], [Define as the platform's import attribute])

AS_IF([test ! $ac_cv_cxx_have_bool],
	AC_DEFINE([XERCES_NO_NATIVE_BOOL], 1, [Define if there is no native bool support in this environment]))

AS_IF([test $ac_cv_cxx_have_lstring],
	AC_DEFINE([XERCES_LSTRSUPPORT], 1, [Define if there is support for L"widestring"]))

# check if the Windows API is defined as using wchar_t or unsigned short; if it's wchar_t, we need to map XMLCh to be wchar_t
# (this is safe because on Windows wchar_t is used to store UTF-16 codepoints, while it is not true on Unix)
AC_MSG_CHECKING([whether the Windows SDK is available and using wchar_t as wide string])
AC_COMPILE_IFELSE(  [AC_LANG_PROGRAM([[#include <windows.h>
                                        wchar_t file[] = L"dummy.file";]], 
                                     [[DeleteFileW(file);]])],
                    [ 
                      xerces_cv_type_xmlch=wchar_t
                   	  AC_MSG_RESULT([yes])
                      AC_DEFINE_UNQUOTED([XERCES_INCLUDE_WCHAR_H], 1, [Define to have Xerces_autoconf_config.hpp include wchar.h])
                    ],
	                [
                      xerces_cv_type_xmlch=$xerces_cv_type_u16bit_int
                      AC_MSG_RESULT([no])
                    ]
                 )

AC_DEFINE_UNQUOTED([XERCES_XMLCH_T], [$xerces_cv_type_xmlch], [Define to the 16 bit type used to represent Xerces UTF-16 characters])
	
AS_IF([test x$ac_cv_type_size_t = xyes],
	AC_DEFINE([XERCES_SIZE_T], [size_t], [Define as the appropriate size_t type]),
	AC_DEFINE([XERCES_SIZE_T], [long], [Define as the appropriate size_t type]))
	
AS_IF([test x$ac_cv_type_ssize_t = xyes],
	AC_DEFINE([XERCES_SSIZE_T], [ssize_t], [Define as the appropriate ssize_t type]),
	AC_DEFINE([XERCES_SSIZE_T], [unsigned long], [Define as the appropriate ssize_t type]))
	
AS_IF([test x$ac_cv_cxx_have_namespaces = xyes],
	AC_DEFINE([XERCES_HAS_CPP_NAMESPACE], 1, [Define if namespaces is supported by the compiler]))
	
AS_IF([test x$ac_cv_cxx_have_std_namespace = xyes],
	AC_DEFINE([XERCES_STD_NAMESPACE], 1, [Define if the std namespace is supported]))
	
AS_IF([test x$ac_cv_cxx_have_std_libs = xyes],
	AC_DEFINE([XERCES_NEW_IOSTREAMS], 1, [Define if the isstream library can be included as <iostream>]))


AC_OUTPUT
