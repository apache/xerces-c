/*
 * Copyright 1999-2004,2005 The Apache Software Foundation.
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
 * $Id$
 */

// For non-autoconf based builds, this header is provided to set a reasonable set of defaults

#ifndef XERCES_NO_AUTOCONFIG_CONFIG_HPP
#define XERCES_NO_AUTOCONFIG_CONFIG_HPP

#include <basetsd.h>

/* #undef XERCES_AUTOCONF */
/* #undef XERCES_HAVE_SYS_TYPES_H */
#define HAVE_STRICMP        1
#define HAVE_STRNICMP       1
#define HAVE_SYS_TIMEB_H    1
#define HAVE_FTIME          1
#define HAVE_LIMITS_H       1

#define XERCES_16BIT_INT short int
#define XERCES_32BIT_INT int
#define XERCES_XMLCH_T  unsigned short
#define XERCES_SIZE_T   SIZE_T
#define XERCES_SSIZE_T  SSIZE_T

#define XERCES_HAS_CPP_NAMESPACE
#define XERCES_STD_NAMESPACE
#define XERCES_NEW_IOSTREAMS
/* #undef XERCES_NO_NATIVE_BOOL */
#define XERCES_LSTRSUPPORT

#ifdef XERCES_STATIC_LIBRARY
#define XERCES_PLATFORM_EXPORT
#define XERCES_PLATFORM_IMPORT
#else
#define XERCES_PLATFORM_EXPORT __declspec(dllexport)
#define XERCES_PLATFORM_IMPORT __declspec(dllimport)
#define DLL_EXPORT
#endif

// ---------------------------------------------------------------------------
//  XMLSize_t is the unsigned integral type.
// ---------------------------------------------------------------------------
typedef XERCES_SIZE_T				XMLSize_t;
typedef XERCES_SSIZE_T				XMLSSize_t;

// ---------------------------------------------------------------------------
//  Define our version of the XML character
// ---------------------------------------------------------------------------
typedef XERCES_XMLCH_T				XMLCh;

// ---------------------------------------------------------------------------
//  Define unsigned 16 and 32 bit integers
// ---------------------------------------------------------------------------
typedef unsigned XERCES_16BIT_INT	XMLUInt16;
typedef unsigned XERCES_32BIT_INT	XMLUInt32;

// ---------------------------------------------------------------------------
//  Define signed 32 bit integers
// ---------------------------------------------------------------------------
typedef XERCES_32BIT_INT			XMLInt32;

// ---------------------------------------------------------------------------
//  XMLFilePos is the type used to represent a file position.
// ---------------------------------------------------------------------------
typedef unsigned XERCES_32BIT_INT	XMLFilePos;

// ---------------------------------------------------------------------------
//  Force on the Xerces debug token if it is on in the build environment
// ---------------------------------------------------------------------------
#if defined(_DEBUG)
#define XERCES_DEBUG
#endif

#endif
