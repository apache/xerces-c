dnl @synopsis XERCES_NETACCESSOR_SELECTION
dnl
dnl Determines the which netaccessor to use
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-05-23
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_NETACCESSOR_SELECTION],
	[

	######################################################
	# Test for availability of each netaccessor on this host.
	# For each netaccessor that's available, and hasn't been disabled, add it to our list.
	# If the netaccessor has been explicitly "enable"d, then vote for it strongly,
	# in upper case.
	######################################################
	na_list=
	
	AC_REQUIRE([XERCES_CURL_PREFIX])	
	AC_MSG_CHECKING([whether we can support the libcurl-based NetAccessor])
	list_add=
	AS_IF([test x"$xerces_cv_curl_prefix" != x], [
		AC_ARG_ENABLE([netaccessor-curl],
			AS_HELP_STRING([--enable-netaccessor-curl],
				[Enable libcurl-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=CURL])],
			[list_add=curl])
	])
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list -$list_add-"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	AC_REQUIRE([XERCES_LIBWWW_PREFIX])	
	AC_MSG_CHECKING([whether we can support the libwww-based NetAccessor])
	list_add=
	AS_IF([test x"$xerces_cv_libwww_prefix" != x], [
		AC_ARG_ENABLE([netaccessor-libwww],
			AS_HELP_STRING([--enable-netaccessor-libwww],
				[Enable libwww-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=LIBWWW])],
			[list_add=libwww])
	])
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list -$list_add-"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	
	AC_MSG_CHECKING([whether we can support the sockets-based NetAccessor])
	list_add=
	AS_IF([test x"$ac_cv_header_sys_socket_h" = xyes],
		[AC_ARG_ENABLE([netaccessor-socket],
			AS_HELP_STRING([--enable-netaccessor-socket],
				[Enable sockets-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=SOCKET])],
			[list_add=socket])
		])
	AS_IF([test x"$list_add" != x],
		[na_list="$na_list -$list_add-"; AC_MSG_RESULT(yes)],
		[AC_MSG_RESULT(no)]
	)
	
	# Check for OS-specific transcoders
	case $host_os in
	darwin*)
		list_add=
		AC_MSG_CHECKING([whether we can support the CFURL NetAccessor (Mac OS X)])
		AS_IF([test x"$ac_cv_header_CoreServices_CoreServices_h" = xyes], [
			AC_ARG_ENABLE([netaccessor-cfurl],
				AS_HELP_STRING([--enable-netaccessor-cfurl],
					[Enable cfurl-based NetAccessor support]),
				[AS_IF([test x"$enableval" = xyes],
					[list_add=CFURL])],
				[list_add=cfurl])
			]
		)
		AS_IF([test x"$list_add" != x],
			[na_list="$na_list -$list_add-"; AC_MSG_RESULT(yes)],
			[AC_MSG_RESULT(no)]
		)
		;;
	windows* | mingw*)
		list_add=
		AC_MSG_CHECKING([whether we can support the WinSock NetAccessor (Windows)])
		AC_ARG_ENABLE([netaccessor-winsock],
			AS_HELP_STRING([--enable-netaccessor-winsock],
				[Enable winsock-based NetAccessor support]),
			[AS_IF([test x"$enableval" = xyes],
				[list_add=WINSOCK])],
			[list_add=winsock])
		AS_IF([test x"$list_add" != x],
			[na_list="$na_list -$list_add-"; AC_MSG_RESULT(yes)],
			[AC_MSG_RESULT(no)]
		)
		;;
	esac
		
	
	######################################################
	# Determine which netaccessor to use.
	#
	# We do this in two passes. Accessors that have been enabled with "yes",
	# and which start out in upper case, get the top priority on the first pass.
	# On the second pass, we consider those which are simply available, but
	# which were not "disable"d (these won't even be in our list).
	######################################################
	netaccessor=
	AC_MSG_CHECKING([for which NetAccessor to use (choices:$na_list)])
	for i in 1 2; do
		# Swap upper/lower case in string:
		na_list=`echo $na_list | tr '[a-z][A-Z]' '[A-Z][a-z]'`
		
		# Check for each netaccessor, in implicit rank order
		case $na_list in
		
		*-curl-*)
			netaccessor=curl
			AC_DEFINE([XERCES_USE_NETACCESSOR_CURL], 1, [Define to use the CURL NetAccessor])
			LIBS="${LIBS} -L${xerces_cv_curl_prefix}/lib -lcurl"
			break
			;;
			
		*-cfurl-*)
			netaccessor=cfurl
			AC_DEFINE([XERCES_USE_NETACCESSOR_CFURL], 1, [Define to use the Mac OS X CFURL NetAccessor])
			XERCES_LINK_DARWIN_FRAMEWORK([CoreServices])
			break
			;;
			
		*-winsock-*)
			netaccessor=winsock
			AC_DEFINE([XERCES_USE_NETACCESSOR_WINSOCK], 1, [Define to use the WinSock NetAccessor])
			break
			;;
			
		*-socket-*)
			netaccessor=socket
			AC_DEFINE([XERCES_USE_NETACCESSOR_SOCKET], 1, [Define to use the Sockets-based NetAccessor])
			break
			;;
			
		*-libwww-*)
			netaccessor=libwww
			AC_DEFINE([XERCES_USE_NETACCESSOR_LIBWWW], 1, [Define to use the libwww NetAccessor])
			LIBS="${LIBS} -L${xerces_cv_libwww_prefix}/lib -lwww"
			break
			;;
			
		*)
			AS_IF([test $i -eq 2], [
				AC_MSG_RESULT([none available; there will be no network access!!!])
				]
			)
			;;
			
		esac
	done
	if test x"$netaccessor" != x; then
		AC_MSG_RESULT($netaccessor)
	fi
	
	# Define the auto-make conditionals which determine what actually gets compiled
	# Note that these macros can't be executed conditionally, which is why they're here, not above.
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_CURL],		[test x"$netaccessor" = xcurl])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_CFURL],		[test x"$netaccessor" = xcfurl])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_WINSOCK],	[test x"$netaccessor" = xwinsock])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_SOCKET],		[test x"$netaccessor" = xsocket])
	AM_CONDITIONAL([XERCES_USE_NETACCESSOR_LIBWWW],		[test x"$netaccessor" = xlibwww])

	]
)

