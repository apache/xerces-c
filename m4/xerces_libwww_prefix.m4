dnl @synopsis XERCES_LIBWWW_PREFIX
dnl
dnl Determines the prefix for libwww
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-05-23
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_LIBWWW_PREFIX],
	[
	AC_ARG_WITH([libwww],
		[AS_HELP_STRING([--with-libwww[[[[=DIR]]]]],[Specify location of libwww])],
		[with_libwww=m4_if($with_libwww, [yes], [], $with_libwww)],
		[with_libwww=])
		
	# Determine if libwww is available
	AC_CACHE_CHECK([for libwww], [xerces_cv_libwww_prefix], [	
		xerces_cv_libwww_prefix=
		if test x"$with_libwww" != x"no"; then
			search_list="$with_libwww /usr/local /usr"
			for i in $search_list; do
				if test -r "$i/include/w3c-libwww/WWWInit.h"; then
					xerces_cv_libwww_prefix=$i
					break
				fi
			done
		fi
	])
	
	
	AC_SUBST([LIBWWW_PREFIX], [$xerces_cv_libwww_prefix])
	]
)

