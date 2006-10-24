dnl @synopsis XERCES_ATOMICOPMGR_SELECTION
dnl
dnl Determines the which XMLAtomicOpMgr to use
dnl
dnl @category C
dnl @author James Berry
dnl @version 2005-05-25
dnl @license AllPermissive
dnl
dnl $Id$

AC_DEFUN([XERCES_ATOMICOPMGR_SELECTION],
	[
	AC_REQUIRE([XERCES_NO_THREADS])
	
	AC_MSG_CHECKING([for which AtomicOp Manager to use])
	aopmgr=
	
	# If no threads is specified, use the NoThread Atomic Op Mgr
	AS_IF([test x$xerces_cv_no_threads = xyes],
		[
			aopmgr=NoThreads
			AC_DEFINE([XERCES_USE_ATOMICOPMGR_NOTHREAD], 1, [Define to use the NoThread AtomicOp mgr])
		])
	
	# Platform specific checks
	AS_IF([test -z "$aopmgr"],
		[
			case $host_os in
			windows* | cygwin* | mingw*)
				aopmgr=Windows;
				AC_DEFINE([XERCES_USE_ATOMICOPMGR_WINDOWS], 1, [Define to use the Windows AtomicOp mgr])
				;;
			darwin*)
				# On MacOS, use the built-in AtomicOp primitives
				AS_IF([test x$ac_cv_header_CoreServices_CoreServices_h = xyes],
					[
						aopmgr=MacOS
						AC_DEFINE([XERCES_USE_ATOMICOPMGR_MACOS], 1, [Define to use the MacOS AtomicOp mgr])
						XERCES_LINK_DARWIN_FRAMEWORK([CoreServices])
					])
				;;
			esac
		])
	
	# Fall back to using posix mutex
	AS_IF([test -z "$aopmgr"],
		[
			aopmgr=POSIX;
			AC_DEFINE([XERCES_USE_ATOMICOPMGR_POSIX], 1, [Define to use the POSIX AtomicOp mgr])
		])

	AC_MSG_RESULT($aopmgr)
	
	# Define the auto-make conditionals which determine what actually gets compiled
	# Note that these macros can't be executed conditionally, which is why they're here, not above.
	AM_CONDITIONAL([XERCES_USE_ATOMICOPMGR_NOTHREAD],	[test x"$aopmgr" = xNoThreads])
	AM_CONDITIONAL([XERCES_USE_ATOMICOPMGR_POSIX],		[test x"$aopmgr" = xPOSIX])
	AM_CONDITIONAL([XERCES_USE_ATOMICOPMGR_MACOS],		[test x"$aopmgr" = xMacOS])
	AM_CONDITIONAL([XERCES_USE_ATOMICOPMGR_WINDOWS],	[test x"$aopmgr" = xWindows])
	
	]
)

