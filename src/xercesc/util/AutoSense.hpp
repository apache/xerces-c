/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.18  2005/01/13 12:36:01  amassari
 * Support for UnixWare 7.1.1 (jira# 1148)
 *
 * Revision 1.17  2004/12/16 03:21:39  cargilld
 * Fix 390 build.
 *
 * Revision 1.16  2004/11/18 16:20:04  cargilld
 * Changes for linker problems with linux build using xlc.
 *
 * Revision 1.15  2004/09/08 13:56:21  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.14  2004/02/17 15:56:50  neilg
 * fix for bug 25035; much thanks to Abe Backus
 *
 * Revision 1.13  2004/02/04 13:26:44  amassari
 * Added support for the Interix platform (Windows Services for Unix 3.5)
 *
 * Revision 1.12  2003/05/21 21:08:04  knoaman
 * gcc 2.95.x is generating an internal error for some template definitions, so
 * we use the default memory manger in such cases.
 *
 * Revision 1.11  2003/05/12 09:44:19  gareth
 * Port to NetBSD. Patch by Hiramatsu Yoshifumi.
 *
 * Revision 1.10  2003/03/27 16:16:43  tng
 * use __IBMCPP__ instead of __xlC__ to determine xlC compiler
 *
 * Revision 1.9  2003/03/13 22:11:46  tng
 * [Bug 17858] Support for QNX/Neutrino.  Patch from Chris McKillop.
 *
 * Revision 1.8  2002/12/02 20:40:49  tng
 * [Bug 12490] Patches required to build Xerces-C++ on BeOS R5.  Patch from Andrew Bachmann.
 *
 * Revision 1.7  2002/10/29 18:58:33  tng
 * Support for Linux/390
 *
 * Revision 1.6  2002/08/23 20:56:00  tng
 * [Bug 11981] inproper "AND" operator in AutoSense.hpp.
 *
 * Revision 1.5  2002/08/19 18:35:56  tng
 * [Bug 6467] Installing Xerces C++ on cygwin environment.
 *
 * Revision 1.4  2002/07/12 16:48:49  jberry
 * Remove reliance on XML_MACOSX. XML_MACOS is used solely. Where qualification
 * by compiler is required, look for the compiler explicitly such as with
 * XML_METROWERKS or __APPLE__ (for the Apple GCC compiler).
 *
 * Add a few tweaks for compatibility with GCC3.1.
 *
 * This change may address Bug 10649.
 *
 * Revision 1.3  2002/03/04 20:31:14  tng
 * [Bug 2868] AIX 4.3.3 mutex/atomic-operation changes for build.
 *
 * Revision 1.2  2002/02/27 22:38:34  peiyongz
 * Bug# 6445 Caldera (SCO) OpenServer Port : patch from Martin Kalen
 *
 * Revision 1.1.1.1  2002/02/01 22:22:09  peiyongz
 * sane_include
 *
 * Revision 1.21  2001/11/29 18:25:18  tng
 * FreeBSD support by Michael Huedepohl.
 *
 * Revision 1.20  2001/05/11 13:26:26  tng
 * Copyright update.
 *
 * Revision 1.19  2001/03/23 19:01:25  tng
 * [Bug 1018]  AutoSense looks for "IRIX" when it should look for "sgi" or "__sgi".  By Jeff Harrell.
 *
 * Revision 1.18  2001/02/09 14:40:01  tng
 * Update support for SCO UnixWare 7 (gcc).  Tested under UnixWare 7.1.1 with gcc version 2.95.2 19991024 (release) with gmake 3.79.1.  Updated by Martin Kalen.
 *
 * Revision 1.17  2001/01/26 21:59:25  tng
 * Change bug-todo list to xml format.  Other documentation fixes.
 *
 * Revision 1.16  2000/10/09 18:15:43  jberry
 * Modify sensing of Mac OS X.
 * PR:
 * Obtained from:
 * Submitted by:
 * Reviewed by:
 * PR:
 * Obtained from:
 * Submitted by:
 * Reviewed by:
 *
 * Revision 1.15  2000/09/21 00:54:18  aruna1
 * OS2 related changes given by Bill Schindler
 *
 * Revision 1.14  2000/08/01 18:26:02  aruna1
 * Tru64 support added
 *
 * Revision 1.13  2000/07/18 18:25:58  andyh
 * Mac OS update.
 * Contributed by James Berry <jberry@criticalpath.com>
 *
 * Revision 1.12  2000/04/04 20:11:29  abagchi
 * Added PTX support
 *
 * Revision 1.11  2000/03/02 19:54:37  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.10  2000/03/02 01:51:00  aruna1
 * Sun CC 5.0 related changes
 *
 * Revision 1.9  2000/02/24 20:05:23  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.8  2000/02/22 01:00:10  aruna1
 * GNUGDefs references removed. Now only GCCDefs is used instead
 *
 * Revision 1.7  2000/02/06 07:48:00  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.6  2000/02/01 23:43:22  abagchi
 * AS/400 related change
 *
 * Revision 1.5  2000/01/21 22:12:29  abagchi
 * OS390 Change: changed OE390 to OS390
 *
 * Revision 1.4  1999/12/18 00:47:01  rahulj
 * Merged in some changes for OS390.
 *
 * Revision 1.3  1999/12/17 01:28:53  rahulj
 * Merged in changes submitted for UnixWare 7 port. Platform
 * specific files are still missing.
 *
 * Revision 1.2  1999/12/01 17:16:16  rahulj
 * Added support for IRIX 6.5.5 using SGI MIPSpro C++ 7.3 and 7.21 generating 32 bit objects. Changes submitted by Marc Stuessel
 *
 * Revision 1.1.1.1  1999/11/09 01:03:55  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:03  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef AUTOSENSE_HPP
#define AUTOSENSE_HPP

// ---------------------------------------------------------------------------
//  This section attempts to auto detect the operating system. It will set
//  up XercesC specific defines that are used by the rest of the code.
// ---------------------------------------------------------------------------
#if defined(_AIX)
    #define XML_AIX
    #define XML_UNIX
    #if defined(_AIXVERSION_430)
        #define XML_AIX43  // for use of POSIX compliant pthread functions
    #endif
#elif defined(_SEQUENT_)
    #define XML_PTX
    #define XML_UNIX
#elif defined(_HP_UX) || defined(__hpux) || defined(_HPUX_SOURCE)
    #define XML_HPUX
    #define XML_UNIX
#elif defined(SOLARIS) || defined(__SVR4)
    #define XML_SOLARIS
    #define XML_UNIX
#elif defined(_SCO_DS)
    #define XML_OPENSERVER
    #define XML_UNIX
#elif defined(__UNIXWARE__) || defined(__USLC__)
    #define XML_UNIXWARE
    #define XML_UNIX
#elif defined(__BEOS__)
    #define XML_BEOS
    #define XML_UNIX
#elif defined(__QNXNTO__)
    #define XML_QNX
    #define XML_UNIX
#elif defined(__linux__)
    #define XML_LINUX
    #define XML_UNIX
    #if defined(__s390__)
        #define XML_LINUX_390
    #endif
#elif defined(__FreeBSD__)
    #define XML_FREEBSD
    #define XML_UNIX
#elif defined(IRIX) || defined(__sgi)
    #define XML_IRIX
    #define XML_UNIX
#elif defined(__MVS__)
    #define XML_OS390
    #define XML_UNIX
#elif defined(EXM_OS390)
    #define XML_OS390
    #define XML_UNIX
#elif defined(__OS400__)
    #define XML_AS400
    #define XML_UNIX
#elif defined(__OS2__)
    #define XML_OS2
#elif defined(__TANDEM)
    #define XML_TANDEM
    #define XML_UNIX
    #define XML_CSET
#elif defined(__CYGWIN__)
    #define XML_CYGWIN
    #ifndef WIN32
      #define WIN32
    #endif
#elif defined(_WIN32) || defined(WIN32)
    #define XML_WIN32
    #ifndef WIN32
      #define WIN32
    #endif
#elif defined(__WINDOWS__)

    // IBM VisualAge special handling
    #if defined(__32BIT__)
        #define XML_WIN32
    #else
        #define XML_WIN16
    #endif
#elif defined(__MSDXML__)
    #define XML_DOS

#elif defined(macintosh) || (defined(__APPLE__) && defined(__MACH__))
    #define XML_MACOS
#elif defined(__alpha) && defined(__osf__)
    #define XML_TRU64
#elif defined(__NetBSD__)
    #define XML_NETBSD
#elif defined(__INTERIX)
    #define XML_INTERIX
    #define XML_UNIX
#else
    #error Code requires port to host OS!
#endif


// ---------------------------------------------------------------------------
//  This section attempts to autodetect the compiler being used. It will set
//  up Xerces specific defines that can be used by the rest of the code.
// ---------------------------------------------------------------------------
#if defined(__BORLANDC__)
    #define XML_BORLAND
#elif defined(_MSC_VER)
    #define XML_VISUALCPP
#elif defined(XML_SOLARIS)
    #if defined(__SUNPRO_CC) && (__SUNPRO_CC >=0x500)
        #define XML_SUNCC5
	#elif defined(__SUNPRO_CC) && (__SUNPRO_CC <0x500)
        #define XML_SUNCC
    #elif defined(_EDG_RUNTIME_USES_NAMESPACES)
        #define XML_SOLARIS_KAICC
    #elif defined(__GNUG__)
		#define XML_GCC
    #else
        #error Code requires port to current development environment
    #endif
#elif defined (__QNXNTO__)
    #define XML_QCC
#elif defined(__IBMC__) || defined(__IBMCPP__)
    #if defined(XML_WIN32)
        #define XML_IBMVAW32
    #elif defined(XML_OS2)
        #define XML_IBMVAOS2
        #if (__IBMC__ >= 400 || __IBMCPP__ >= 400)
            #define XML_IBMVA4_OS2
        #endif
    #elif defined(XML_AIX) || defined(__linux__)
        #define XML_CSET              
    #elif defined(__MVS__) && defined(__cplusplus)
        #define XML_MVSCPP
    #elif defined(EXM_OS390) && defined(__cplusplus)
        #define XML_MVSCPP
    #endif
#elif defined (__GNUG__) || defined(__BEOS__) || defined(__linux__) || defined(__CYGWIN__)
    #define XML_GCC
#elif defined(XML_HPUX)
    #if defined(EXM_HPUX)
        #define XML_HPUX_KAICC
    #elif (__cplusplus == 1)
        #define XML_HPUX_CC
    #elif (__cplusplus == 199707 || __cplusplus == 199711)
        #define XML_HPUX_aCC
    #endif
#elif defined(XML_IRIX)
    #define XML_MIPSPRO_CC
#elif defined(XML_PTX)
    #define XML_PTX_CC
#elif defined(XML_TANDEM)
    #define XML_TANDEMCC
#elif defined(__MVS__) && defined(__cplusplus)
    #define XML_MVSCPP
#elif defined(EXM_OS390) && defined(__cplusplus)
    #define XML_MVSCPP
#elif defined(XML_TRU64) && defined(__DECCXX)
    #define XML_DECCXX
#elif defined(__MWERKS__)
    #define XML_METROWERKS
#elif defined(__OS400__)
#elif defined(XML_UNIXWARE)
    #define XML_SCOCC
#else
    #error Code requires port to current development environment
#endif

// ---------------------------------------------------------------------------
//  The gcc compiler 2.95... is generating an internal error for some template
//  definitions. So, if we are compiling with gcc, have a specific define that
//  we can later use in the code.
// ---------------------------------------------------------------------------
#if defined(__GNUC__)
#define XML_GCC_VERSION (__GNUC__ * 10000 \
                         + __GNUC_MINOR__ * 100 \
                         + __GNUC_PATCHLEVEL__)
#endif


#endif
