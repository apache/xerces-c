/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2002/02/01 22:22:19  peiyongz
 * Initial revision
 *
 * Revision 1.5  2000/03/02 19:55:09  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.4  2000/02/06 07:48:18  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.3  1999/11/12 20:37:01  rahulj
 * Changed library name to xerces-c.lib.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:40  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:25  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
// NOTE:
//
//   Xerces is not officially supported with the KAI compiler. This file
//   was sent in by one of the users and is included in the distribution
//   just for convenience. Please send any defects / modification
//   reports to xml4c@us.ibm.com
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//  Include some runtime files that will be needed product wide
// ---------------------------------------------------------------------------
#include <memory.h>
#include <strings.h>



// ---------------------------------------------------------------------------
//  A define in the build for each project is also used to control whether
//  the export keyword is from the project's viewpoint or the client's.
//  These defines provide the platform specific keywords that they need
//  to do this.
// ---------------------------------------------------------------------------
#define PLATFORM_EXPORT
#define PLATFORM_IMPORT   



// ---------------------------------------------------------------------------
//  Indicate that we support native bools
// ---------------------------------------------------------------------------
// #define NO_NATIVE_BOOL


// ---------------------------------------------------------------------------
//  Define our version of the XMLCh.
// ---------------------------------------------------------------------------
typedef unsigned short XMLCh;


// ---------------------------------------------------------------------------
//  The name of the DLL that is built by the Visual C++ version of the
//  system. We append a previously defined token which holds the DLL
//  versioning string. This is defined in XercesDefs.hpp which is what this
//  file is included into.
// ---------------------------------------------------------------------------
const char* const Xerces_DLLName = "KAIXERCESC" Xerces_DLLVersionStr;


// ---------------------------------------------------------------------------
//  Define some platform specific defines
// ---------------------------------------------------------------------------
#define XERCES_EXPORT



// ---------------------------------------------------------------------------
//  Provide prototypes for some string functions that are not provided on all
//  platforms.
// ---------------------------------------------------------------------------
int stricmp(const char* const str1, const char* const  str2);
int strnicmp(const char* const str1, const char* const  str2, const unsigned int count);

