/*
 * (C) Copyright IBM Corp. 1999  All rights reserved.
 *
 * US Government Users Restricted Rights Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 * The program is provided "as is" without any warranty express or
 * implied, including the warranty of non-infringement and the implied
 * warranties of merchantibility and fitness for a particular purpose.
 * IBM will not be liable for any damages suffered by you as a result
 * of using the Program. In no event will IBM be liable for any
 * special, indirect or consequential damages or lost profits even if
 * IBM has been advised of the possibility of their occurrence. IBM
 * will not be liable for any third party claims against you.
 */

/**
 * @version Revision: 96 1.10 src/com/ibm/xml/util/CSetDefs.hpp, xml4c2Utils, xml4c2 
 */

#ifndef OS400SETDEFS_H
#define OS400SETDEFS_H 
#include <util/XML4CDefs.hpp>

// ---------------------------------------------------------------------------
// Define these away for this platform
// ---------------------------------------------------------------------------
#define PLATFORM_EXPORT
#define PLATFORM_IMPORT


// ---------------------------------------------------------------------------
// Indicate that we do not support native bools
// ---------------------------------------------------------------------------
#define NO_NATIVE_BOOL


// ---------------------------------------------------------------------------
//  Define our version of the XML character
// ---------------------------------------------------------------------------
typedef unsigned short XMLCh;
// ---------------------------------------------------------------------------
//  Define unsigned 16 and 32 bits integers
// ---------------------------------------------------------------------------
typedef unsigned short XMLUInt16;
typedef unsigned int   XMLUInt32;



// ---------------------------------------------------------------------------
//  Force on the XML4C2 debug token if it was on in the build environment
// ---------------------------------------------------------------------------
#if 0
#define XML4C2_DEBUG
#endif


// ---------------------------------------------------------------------------
//  Provide some common string ops that are different/notavail on CSet
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  The name of the DLL that is built by the CSet C++ version of the
//  system. We append a previously defined token which holds the DLL
//  versioning string. This is defined in XML4CDefs.hpp which is what this
//  file is included into.
// ---------------------------------------------------------------------------
const char* const XML4C2_DLLName = "QXML4C";
#endif
