/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Log$
 * Revision 1.14  2001/06/05 13:52:25  tng
 * Change Version number from Xerces 1.4 to 1.5.  By Pei Yong Zhang.
 *
 * Revision 1.13  2001/05/11 13:26:32  tng
 * Copyright update.
 *
 * Revision 1.12  2001/02/08 14:15:33  tng
 * enable COMPAQ Tru64 UNIX machines to build xerces-c with gcc (tested using COMPAQ gcc version2.95.2 19991024 (release) and Tru64 V5.0 1094).  Added by Martin Kalen.
 *
 * Revision 1.11  2001/01/25 19:17:06  tng
 * const should be used instead of static const.  Fixed by Khaled Noaman.
 *
 * Revision 1.10  2001/01/12 22:09:07  tng
 * Various update for Xerces 1.4
 *
 * Revision 1.9  2000/11/07 18:14:39  andyh
 * Fix incorrect version number in gXercesMinVersion.
 * From Pieter Van-Dyck
 *
 * Revision 1.8  2000/11/02 07:23:27  roddey
 * Just a test of checkin access
 *
 * Revision 1.7  2000/08/18 21:29:14  andyh
 * Change version to 1.3 in preparation for upcoming Xerces 1.3
 * and XML4C 3.3 stable releases
 *
 * Revision 1.6  2000/08/07 20:31:34  jpolast
 * include SAX2_EXPORT module
 *
 * Revision 1.5  2000/08/01 18:26:02  aruna1
 * Tru64 support added
 *
 * Revision 1.4  2000/07/29 05:36:37  jberry
 * Fix misspelling in Mac OS port
 *
 * Revision 1.3  2000/07/19 18:20:12  andyh
 * Macintosh port: fix problems with yesterday's code checkin.  A couple
 * of the changes were mangled or missed.
 *
 * Revision 1.2  2000/04/04 20:11:29  abagchi
 * Added PTX support
 *
 * Revision 1.1  2000/03/02 19:54:50  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.13  2000/03/02 01:51:00  aruna1
 * Sun CC 5.0 related changes
 *
 * Revision 1.12  2000/02/24 20:05:26  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.11  2000/02/22 01:00:10  aruna1
 * GNUGDefs references removed. Now only GCCDefs is used instead
 *
 * Revision 1.10  2000/02/06 07:48:05  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.9  2000/02/01 23:43:32  abagchi
 * AS/400 related change
 *
 * Revision 1.8  2000/01/21 22:12:29  abagchi
 * OS390 Change: changed OE390 to OS390
 *
 * Revision 1.7  2000/01/14 01:18:35  roddey
 * Added a macro, XMLStrL(), which is defined one way or another according
 * to whether the per-compiler file defines XML_LSTRSUPPORT or not. This
 * allows conditional support of L"" type prefixes.
 *
 * Revision 1.6  2000/01/14 00:52:06  roddey
 * Updated the version information for the next release, i.e. 1.1.0
 *
 * Revision 1.5  1999/12/17 01:28:53  rahulj
 * Merged in changes submitted for UnixWare 7 port. Platform
 * specific files are still missing.
 *
 * Revision 1.4  1999/12/16 23:47:10  rahulj
 * Updated for version 1.0.1
 *
 * Revision 1.3  1999/12/01 17:16:16  rahulj
 * Added support for IRIX 6.5.5 using SGI MIPSpro C++ 7.3 and 7.21 generating 32 bit objects. Changes submitted by Marc Stuessel
 *
 * Revision 1.2  1999/11/10 02:02:51  abagchi
 * Changed version numbers
 *
 * Revision 1.1.1.1  1999/11/09 01:05:35  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:19  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(XERCESDEFS_HPP)
#define XERCESDEFS_HPP

// ---------------------------------------------------------------------------
//  These are the various representations of the current version of Xerces.
//  These are updated for every build. They must be at the top because they
//  can be used by various per-compiler headers below.
// ---------------------------------------------------------------------------
#define Xerces_DLLVersionStr "1_5"
const char* const    gXercesVersionStr = "1_5";
const char* const    gXercesFullVersionStr = "1_5_0";
const unsigned int   gXercesMajVersion = 1;
const unsigned int   gXercesMinVersion = 5;
const unsigned int   gXercesRevision   = 0;


// ---------------------------------------------------------------------------
//  Include the header that does automatic sensing of the current platform
//  and compiler.
// ---------------------------------------------------------------------------
#include    <util/AutoSense.hpp>



// ---------------------------------------------------------------------------
//  According to the platform we include a platform specific file. This guy
//  will set up any platform specific stuff, such as character mode.
// ---------------------------------------------------------------------------
#if defined(XML_WIN32)
#include    <util/Platforms/Win32/Win32Defs.hpp>
#endif

#if defined(XML_AIX)
#include    <util/Platforms/AIX/AIXDefs.hpp>
#endif

#if defined(XML_SOLARIS)
#include    <util/Platforms/Solaris/SolarisDefs.hpp>
#endif

#if defined(XML_UNIXWARE)
#include    <util/Platforms/UnixWare/UnixWareDefs.hpp>
#endif

#if defined(XML_HPUX)
#include    <util/Platforms/HPUX/HPUXDefs.hpp>
#endif

#if defined(XML_IRIX)
#include    <util/Platforms/IRIX/IRIXDefs.hpp>
#endif

#if defined(XML_TANDEM)
#include    <util/Platforms/Tandem/TandemDefs.hpp>
#endif

#if defined(XML_LINUX)
#include    <util/Platforms/Linux/LinuxDefs.hpp>
#endif

#if defined(XML_OS390)
#include    <util/Platforms/OS390/OS390Defs.hpp>
#endif

#if defined(XML_PTX)
#include    <util/Platforms/PTX/PTXDefs.hpp>
#endif

#if defined(XML_OS2)
#include    <util/Platforms/OS2/OS2Defs.hpp>
#endif

#if defined(XML_MACOS) || defined(XML_MACOSX)
#include	<util/Platforms/MacOS/MacOSDefs.hpp>
#endif

#if defined(XML_AS400)
#include	<util/Platforms/OS400/OS400Defs.hpp>
#endif

#if defined(XML_TRU64)
#include	<util/Platforms/Tru64/Tru64Defs.hpp>
#endif


// ---------------------------------------------------------------------------
//  And now we subinclude a header according to the development environment
//  we are on. This guy defines for each platform some basic stuff that is
//  specific to the development environment.
// ---------------------------------------------------------------------------
#if defined(XML_VISUALCPP)
#include    <util/Compilers/VCPPDefs.hpp>
#endif

#if defined(XML_CSET)
#include    <util/Compilers/CSetDefs.hpp>
#endif

#if defined(XML_BORLAND)
#include    <util/Compilers/BorlandCDefs.hpp>
#endif

#if defined(XML_SUNCC) || defined(XML_SUNCC5)
#include    <util/Compilers/SunCCDefs.hpp>
#endif

#if defined(XML_SCOCC)
#include    <util/Compilers/SCOCCDefs.hpp>
#endif

#if defined(XML_SOLARIS_KAICC)
#include    <util/Compilers/SunKaiDefs.hpp>
#endif

#if defined(XML_HPUX_CC) || defined(XML_HPUX_aCC) || defined(XML_HPUX_KAICC)
#include    <util/Compilers/HPCCDefs.hpp>
#endif

#if defined(XML_MIPSPRO_CC)
#include    <util/Compilers/MIPSproDefs.hpp>
#endif

#if defined(XML_TANDEMCC)
#include    <util/Compilers/TandemCCDefs.hpp>
#endif

#if defined(XML_GCC)
#include    <util/Compilers/GCCDefs.hpp>
#endif

#if defined(XML_MVSCPP)
#include    <util/Compilers/MVSCPPDefs.hpp>
#endif

#if defined(XML_IBMVAW32)
#include    <util/Compilers/IBMVAW32Defs.hpp>
#endif

#if defined(XML_IBMVAOS2)
#include    <util/Compilers/IBMVAOS2Defs.hpp>
#endif

#if defined(XML_METROWERKS)
#include	<util/Compilers/CodeWarriorDefs.hpp>
#endif

#if defined(XML_PTX_CC)
#include	<util/Compilers/PTXCCDefs.hpp>
#endif

#if defined(XML_AS400)
#include	<util/Compilers/OS400SetDefs.hpp>
#endif

#if defined(XML_DECCXX)
#include	<util/Compilers/DECCXXDefs.hpp>
#endif


// ---------------------------------------------------------------------------
//  Some general typedefs that are defined for internal flexibility.
//
//  Note  that UTF16Ch is fixed at 16 bits, whereas XMLCh floats in size per
//  platform, to whatever is the native wide char format there. UCS4Ch is
//  fixed at 32 bits. The types we defined them in terms of are defined per
//  compiler, using whatever types are the right ones for them to get these
//  16/32 bit sizes.
// ---------------------------------------------------------------------------
typedef unsigned char       XMLByte;
typedef XMLUInt16           UTF16Ch;
typedef XMLUInt32           UCS4Ch;



// ---------------------------------------------------------------------------
//  Handle boolean. If the platform can handle booleans itself, then we
//  map our boolean type to the native type. Otherwise we create a default
//  one as an int and define const values for true and false.
//
//  This flag will be set in the per-development environment stuff above.
// ---------------------------------------------------------------------------
#if defined(NO_NATIVE_BOOL)
  #ifndef bool
    typedef int     bool;
  #endif
  #ifndef true
    #define  true     1
  #endif
  #ifndef false
    #define false 0
  #endif
#endif


// ---------------------------------------------------------------------------
//  According to whether the compiler suports L"" type strings, we define
//  the XMLStrL() macro one way or another.
// ---------------------------------------------------------------------------
#if defined(XML_LSTRSUPPORT)
#define XMLStrL(str)  L##str
#else
#define XMLStrL(str)  str
#endif



// ---------------------------------------------------------------------------
//  Set up the import/export keyword  for our core projects. The
//  PLATFORM_XXXX keywords are set in the per-development environment
//  include above.
// ---------------------------------------------------------------------------
#if defined(PROJ_XMLUTIL)
#define XMLUTIL_EXPORT PLATFORM_EXPORT
#else
#define XMLUTIL_EXPORT PLATFORM_IMPORT
#endif

#if defined(PROJ_XMLPARSER)
#define XMLPARSER_EXPORT PLATFORM_EXPORT
#else
#define XMLPARSER_EXPORT PLATFORM_IMPORT
#endif

#if defined(PROJ_SAX4C)
#define SAX_EXPORT PLATFORM_EXPORT
#else
#define SAX_EXPORT PLATFORM_IMPORT
#endif

#if defined(PROJ_SAX2)
#define SAX2_EXPORT PLATFORM_EXPORT
#else
#define SAX2_EXPORT PLATFORM_IMPORT
#endif

#if defined(PROJ_DOM)
#define CDOM_EXPORT PLATFORM_EXPORT
#else
#define CDOM_EXPORT PLATFORM_IMPORT
#endif

#if defined(PROJ_PARSERS)
#define PARSERS_EXPORT  PLATFORM_EXPORT
#else
#define PARSERS_EXPORT  PLATFORM_IMPORT
#endif

#if defined(PROJ_VALIDATORS)
#define VALIDATORS_EXPORT  PLATFORM_EXPORT
#else
#define VALIDATORS_EXPORT  PLATFORM_IMPORT
#endif

#endif
