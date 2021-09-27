/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
 * $Id$
 */

#if !defined(XERCESC_INCLUDE_GUARD_XERCESDEFS_HPP)
#define XERCESC_INCLUDE_GUARD_XERCESDEFS_HPP

// ---------------------------------------------------------------------------
//  The file xerces_hdr_config defines critical configuration information
//	used by the remainder of this file.
//
//	There are two major configuration files:
//		- xerces_autoconf_config.hpp-- Contains defines that are safe for
//									   access through public headers.
//
//		- config.h					-- Contains defines that may conflict
//									   with other packages; should only be
//									   included by Xerces implementation files.
//
//	Both of these files are generated through the autoconf/configure process.
// ---------------------------------------------------------------------------

//
// If this is an autoconf configured build, we include Xerces_autoconf_config.hpp
// Otherwise we include a preconfigured config appropriate for the particular
// platform that the specific makefile should copy over.
//
//  If the next line generates an error then you haven't run ./configure
#include	<xercesc/util/Xerces_autoconf_config.hpp>

// ---------------------------------------------------------------------------
//  Include standard headers that we may rely on below.
// ---------------------------------------------------------------------------
#include <cstdint>
#include <cstddef>
#ifdef XERCES_INCLUDE_WCHAR_H
#include <cwchar>
#endif

// ---------------------------------------------------------------------------
//  Include the Xerces version information; this is kept in a separate file to
//  make modification simple and obvious. Updates to the version header file
// ---------------------------------------------------------------------------
#include    <xercesc/util/XercesVersion.hpp>

/**
 * XML size type.
 */
typedef size_t                                  XMLSize_t;

/**
 * XML Character.  Platform-dependent 16-bit type.
 */
typedef XERCES_XMLCH_T                          XMLCh;

// ---------------------------------------------------------------------------
//  Integer definitions
// ---------------------------------------------------------------------------

/**
 * Unsigned 16-bit integer.
 */
typedef uint16_t                                XMLUInt16;
/**
 * Unsigned 32-bit integer.
 */
typedef uint32_t                                XMLUInt32;
/**
 * Unsigned 64-bit integer.
 */
typedef uint64_t                                XMLUInt64;

/**
 * Signed 16-bit integer.
 */
typedef int16_t                                 XMLInt16;
/**
 * Signed 32-bit integer.
 */
typedef int32_t                                 XMLInt32;
/**
 * Signed 64-bit integer.
 */
typedef int64_t                                 XMLInt64;

/**
 * File position.
 */
typedef uint64_t                                XMLFilePos;

/**
 * File location (line number, character position).
 */
typedef uint64_t                                XMLFileLoc;

/**
 * Byte.
 */
typedef uint8_t             XMLByte;

/**
 * UTF-16 character; fixed at 16 bits.
 */
typedef char16_t            UTF16Ch;

/**
 * UCS-4 character; fixed at 32 bits.
 */
typedef char32_t            UCS4Ch;


// ---------------------------------------------------------------------------
//  According to whether the compiler supports L"" type strings, we define
//  the XMLStrL() macro one way or another.
// ---------------------------------------------------------------------------
#if defined(XERCES_LSTRSUPPORT)
#define XMLStrL(str)  L##str
#else
#define XMLStrL(str)  str
#endif


// ---------------------------------------------------------------------------
// Define versioned namespace
// ---------------------------------------------------------------------------

namespace XERCES_CPP_NAMESPACE { }
namespace xercesc = XERCES_CPP_NAMESPACE;

// ---------------------------------------------------------------------------
//  Set up the import/export keyword  for our core projects. The
//  PLATFORM_XXXX keywords are set in the per-development environment
//  include above.
// ---------------------------------------------------------------------------

// The DLL_EXPORT flag should be defined on the command line during the build of a DLL
// configure conspires to make this happen.

#if defined(DLL_EXPORT)
  #if defined(XERCES_BUILDING_LIBRARY)
    #define XMLUTIL_EXPORT XERCES_PLATFORM_EXPORT
    #define XMLPARSER_EXPORT XERCES_PLATFORM_EXPORT
    #define SAX_EXPORT XERCES_PLATFORM_EXPORT
    #define SAX2_EXPORT XERCES_PLATFORM_EXPORT
    #define CDOM_EXPORT XERCES_PLATFORM_EXPORT
    #define PARSERS_EXPORT  XERCES_PLATFORM_EXPORT
    #define VALIDATORS_EXPORT  XERCES_PLATFORM_EXPORT
    #define XINCLUDE_EXPORT  XERCES_PLATFORM_EXPORT
  #else
    #define XMLUTIL_EXPORT XERCES_PLATFORM_IMPORT
    #define XMLPARSER_EXPORT XERCES_PLATFORM_IMPORT
    #define SAX_EXPORT XERCES_PLATFORM_IMPORT
    #define SAX2_EXPORT XERCES_PLATFORM_IMPORT
    #define CDOM_EXPORT XERCES_PLATFORM_IMPORT
    #define PARSERS_EXPORT  XERCES_PLATFORM_IMPORT
    #define VALIDATORS_EXPORT  XERCES_PLATFORM_IMPORT
    #define XINCLUDE_EXPORT  XERCES_PLATFORM_IMPORT
  #endif
  #if defined(XERCES_BUILDING_DEPRECATED_LIBRARY)
    #define DEPRECATED_DOM_EXPORT XERCES_PLATFORM_EXPORT
  #else
    #define DEPRECATED_DOM_EXPORT XERCES_PLATFORM_IMPORT
  #endif
#else
  #define XMLUTIL_EXPORT 
  #define XMLPARSER_EXPORT 
  #define SAX_EXPORT 
  #define SAX2_EXPORT
  #define CDOM_EXPORT
  #define DEPRECATED_DOM_EXPORT 
  #define PARSERS_EXPORT 
  #define VALIDATORS_EXPORT
  #define XINCLUDE_EXPORT
#endif

#endif
