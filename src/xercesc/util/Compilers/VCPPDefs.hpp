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
 * Revision 1.12  2005/01/26 14:48:14  amassari
 * If the option /Zc:wchar_t is specified, map XMLCh to wchar_t, or trying to use wide-char Windows API will generate compiler errors (jira# 413)
 *
 * Revision 1.11  2004/11/15 13:30:40  amassari
 * Add build target for VC7.1 to create a static library
 *
 * Revision 1.10  2004/09/08 13:56:32  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.9  2003/05/29 13:52:36  gareth
 * fixed typo for version number
 *
 * Revision 1.8  2003/05/29 11:18:37  gareth
 * Added macros in so we can determine whether to do things like iostream as opposed to iostream.h and whether to use std:: or not.
 *
 * Revision 1.7  2002/11/04 14:45:20  tng
 * C++ Namespace Support.
 *
 * Revision 1.6  2002/06/25 16:05:24  tng
 * DOM L3: move the operator delete to DOMDocumentImpl.hpp
 *
 * Revision 1.5  2002/05/28 12:57:17  tng
 * Fix typo.
 *
 * Revision 1.4  2002/05/27 18:02:40  tng
 * define XMLSize_t, XMLSSize_t and their associate MAX
 *
 * Revision 1.3  2002/05/21 19:45:53  tng
 * Define DOMSize_t and XMLSize_t
 *
 * Revision 1.2  2002/04/17 20:30:01  tng
 * [Bug 7583] Build warnings with MS Visual Studio .NET.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:19  peiyongz
 * sane_include
 *
 * Revision 1.13  2001/06/04 20:11:54  tng
 * IDOM: Complete IDNodeIterator, IDTreeWalker, IDNodeFilter.
 *
 * Revision 1.12  2001/06/04 13:45:06  tng
 * The "hash" argument clashes with STL hash.  Fixed by Pei Yong Zhang.
 *
 * Revision 1.11  2001/05/29 18:50:24  tng
 * IDOM: call allocate directly for array allocation to avoid overloading operator new[] which leads to compilation error on SUN CC 4.2
 *
 * Revision 1.10  2001/05/28 20:59:21  tng
 * IDOM: move operator new[] to VCPPDefs as only Windows VCPP requires its presense
 *
 * Revision 1.9  2001/05/23 20:35:03  tng
 * IDOM: Move operator delete to VCPPDefs.hpp as only VCPP needs a matching delete operator.
 *
 * Revision 1.8  2001/03/02 20:53:08  knoaman
 * Schema: Regular expression - misc. updates for error messages,
 * and additions of new functions to XMLString class.
 *
 * Revision 1.7  2000/06/16 21:13:23  rahulj
 * Add 'D' suffix to the library name for the 'DEBUG' build
 * configuration.
 *
 * Revision 1.6  2000/03/02 19:55:09  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.5  2000/02/06 07:48:18  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/14 01:19:22  roddey
 * Added a define of XML_LSTRSUPPORT to indicate supoprt of L"" type
 * prefixes on this compiler.
 *
 * Revision 1.3  2000/01/14 00:51:30  roddey
 * Added the requested XMLStrL() macro to support some portable
 * optimization of DOM code. This still needs to be added to the other
 * per-compiler files.
 *
 * Revision 1.2  1999/11/10 21:26:14  abagchi
 * Changed the DLL name
 *
 * Revision 1.1.1.1  1999/11/09 01:07:41  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:25  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(VCPPDEFS_HPP)
#define VCPPDEFS_HPP

// ---------------------------------------------------------------------------
//  Include some runtime files that will be needed product wide
// ---------------------------------------------------------------------------
#include <sys/types.h>  // for size_t and ssize_t
#include <limits.h>  // for MAX of size_t and ssize_t

// ---------------------------------------------------------------------------
//  A define in the build for each project is also used to control whether
//  the export keyword is from the project's viewpoint or the client's.
//  These defines provide the platform specific keywords that they need
//  to do this.
// ---------------------------------------------------------------------------
#ifdef XML_LIBRARY
  #define PLATFORM_EXPORT
  #define PLATFORM_IMPORT
#else
  #define PLATFORM_EXPORT     __declspec(dllexport)
  #define PLATFORM_IMPORT     __declspec(dllimport)
#endif

// ---------------------------------------------------------------------------
//  Indicate that we do not support native bools
//  If the compiler can handle boolean itself, do not define it
// ---------------------------------------------------------------------------
// #define NO_NATIVE_BOOL

// ---------------------------------------------------------------------------
//  Each compiler might support L"" prefixed constants. There are places
//  where it is advantageous to use the L"" where it supported, to avoid
//  unnecessary transcoding.
//  If your compiler does not support it, don't define this.
// ---------------------------------------------------------------------------
#define XML_LSTRSUPPORT

// ---------------------------------------------------------------------------
//  Indicate that we support C++ namespace
//  Do not define it if the compile cannot handle C++ namespace
// ---------------------------------------------------------------------------
#define XERCES_HAS_CPP_NAMESPACE

// ---------------------------------------------------------------------------
//  Define our version of the XML character
// ---------------------------------------------------------------------------
#ifdef _NATIVE_WCHAR_T_DEFINED 
typedef wchar_t  XMLCh;
#else
typedef unsigned short  XMLCh;
#endif

// ---------------------------------------------------------------------------
//  Define unsigned 16 and 32 bits integers
// ---------------------------------------------------------------------------
typedef unsigned short  XMLUInt16;
typedef unsigned int    XMLUInt32;

// ---------------------------------------------------------------------------
//  Define signed 32 bits integers
// ---------------------------------------------------------------------------
typedef int             XMLInt32;

// ---------------------------------------------------------------------------
//  XMLSize_t is the unsigned integral type.
// ---------------------------------------------------------------------------
#if defined(_SIZE_T) && defined(SIZE_MAX) && defined(_SSIZE_T) && defined(SSIZE_MAX)
    typedef size_t              XMLSize_t;
    #define XML_SIZE_MAX        SIZE_MAX
    typedef ssize_t             XMLSSize_t;
    #define XML_SSIZE_MAX       SSIZE_MAX
#else
    typedef unsigned long       XMLSize_t;
    #define XML_SIZE_MAX        ULONG_MAX
    typedef long                XMLSSize_t;
    #define XML_SSIZE_MAX       LONG_MAX
#endif

// ---------------------------------------------------------------------------
//  Force on the Xerces debug token if it was on in the build environment
// ---------------------------------------------------------------------------
#if defined(_DEBUG)
#define XERCES_DEBUG
#endif

#if _MSC_VER > 1300
#define XERCES_NEW_IOSTREAMS
#define XERCES_STD_NAMESPACE
#endif

// ---------------------------------------------------------------------------
//  The name of the DLL that is built by the Visual C++ version of the
//  system. We append a previously defined token which holds the DLL
//  versioning string. This is defined in XercesDefs.hpp which is what this
//  file is included into.
// ---------------------------------------------------------------------------
#if defined(XERCES_DEBUG)
const char* const Xerces_DLLName = "xerces-c_" Xerces_DLLVersionStr "D";
#else
const char* const Xerces_DLLName = "xerces-c_" Xerces_DLLVersionStr;
#endif


#endif //VCPPDEFS_HPP

