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
 * Revision 1.8  2000/03/02 19:55:09  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.7  2000/03/02 01:50:30  aruna1
 * Sun CC 5.0 related changes
 *
 * Revision 1.6  2000/02/06 07:48:18  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.5  2000/01/14 02:52:14  aruna1
 * XMLCh now mapped to wchar_t
 *
 * Revision 1.4  1999/11/17 21:49:42  abagchi
 * Added the definitions of XMLUInt16 and XMLUInt32
 * PR:
 * Obtained from:
 * Submitted by:
 * Reviewed by:
 *
 * Revision 1.3  1999/11/12 20:37:00  rahulj
 * Changed library name to xerces-c.lib.
 *
 * Revision 1.1.1.1  1999/11/09 01:07:39  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:25  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
// Define these away for this platform
// ---------------------------------------------------------------------------
#define PLATFORM_EXPORT
#define PLATFORM_IMPORT


// ---------------------------------------------------------------------------
// Indicate that we do not support native bools
// ---------------------------------------------------------------------------
#ifdef XML_SUNCC
#define NO_NATIVE_BOOL
#endif


// ---------------------------------------------------------------------------
//  Define our version of the XML character
// ---------------------------------------------------------------------------
typedef wchar_t XMLCh;


// ---------------------------------------------------------------------------
//  Define unsigned 16 and 32 bits integers
// ---------------------------------------------------------------------------
typedef unsigned short  XMLUInt16;
typedef unsigned int    XMLUInt32;


// ---------------------------------------------------------------------------
//  Force on the Xerces debug token if it was on in the build environment
// ---------------------------------------------------------------------------
#if 0
#define XERCES_DEBUG
#endif


// ---------------------------------------------------------------------------
//  Provide some common string ops that are different/notavail on CC
// ---------------------------------------------------------------------------
inline char toupper(const char toUpper) 
{
    if ((toUpper >= 'a') && (toUpper <= 'z'))
        return char(toUpper - 0x20);
    return toUpper;
}

inline char tolower(const char toLower)
{
    if ((toLower >= 'A') && (toLower <= 'Z'))
        return char(toLower + 0x20);
    return toLower;
}

int stricmp(const char* const str1, const char* const  str2);
int strnicmp(const char* const str1, const char* const  str2, const unsigned int count);


// ---------------------------------------------------------------------------
//  The name of the DLL that is built by the CC version of the system.
// ---------------------------------------------------------------------------
const char* const Xerces_DLLName = "libxerces-c";
