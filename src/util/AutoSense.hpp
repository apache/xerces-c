/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

/**
 * $Log$
 * Revision 1.1  1999/11/09 01:03:55  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:45:03  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef AUTOSENSE_HPP
#define AUTOSENSE_HPP

// ---------------------------------------------------------------------------
//  This section attempts to auto detect the operating system. It will set
//  up XML4C specific defines that are used by the rest of the code.
// ---------------------------------------------------------------------------
#if defined(_AIX)
    #define XML_AIX
    #define XML_UNIX
#elif defined(_HP_UX) \
||  defined(__hpux) \
||  defined(_HPUX_SOURCE)
    #define XML_HPUX
    #define XML_UNIX
#elif defined(SOLARIS) || defined(__SVR4)
    #define XML_SOLARIS
    #define XML_UNIX
#elif defined(__linux__)
    #define XML_LINUX
    #define XML_UNIX
#elif defined(__MVS__)
    #define XML_OE390
    #define XML_UNIX
#elif defined(AS400)
    #define XML_AS400
    #define XML_UNIX
#elif defined(__OS2__)
    #define XML_OS2
#elif defined(__TANDEM)
    #define XML_TANDEM
    #define XML_UNIX
    #define XML_CSET
#elif defined(_WIN32) \
|| defined(WIN32)
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

#elif defined(macintosh)
    #define XML_MACOS
#else
    #error Code requires port to host OS!
#endif


// ---------------------------------------------------------------------------
//  This section attempts to autodetect the compiler being used. It will set
//  up XML4C specific defines that can be used by the rest of the code.
// ---------------------------------------------------------------------------
#if defined(_MSC_VER)
    #define XML_VISUALCPP
#elif defined(__BORLANDC__)
    #define XML_BORLAND
#elif defined(__xlC__)
    #define XML_CSET
#elif defined(XML_SOLARIS)
    #if defined(__SUNPRO_CC)
        #define XML_SUNCC
    #elif defined(_EDG_RUNTIME_USES_NAMESPACES)
        #define XML_SOLARIS_KAICC
    #elif defined(__GNUG__)
        #define XML_GNUG
    #endif
#elif defined(__GNUG__)
    #define XML_GNUG
#elif defined(XML_HPUX)
    #if defined(EXM_HPUX)
        #define XML_HPUX_KAICC
    #elif (__cplusplus == 1)
        #define XML_HPUX_CC
    #elif (__cplusplus == 199707 || __cplusplus == 199711)
        #define XML_HPUX_aCC
    #endif
#elif defined(XML_TANDEM)
    #define XML_TANDEMCC
#elif defined(__linux__)
    #define XML_GCC
#elif defined(__MVS__) && defined(__cplusplus)
    #define XML_MVSCPP
#elif defined(__IBMCPP__)
    #if defined(XML_WIN32)
        #define XML_IBMVAW32
    #elif defined(XML_OS2)
        #define XML_IBMVAOS2
    #endif    
#elif defined(__IBMC__)
    #if defined(XML_WIN32)
        #define XML_IBMVAW32
    #elif defined(XML_OS2)
        #define XML_IBMVAOS2
    #endif    
#elif defined(__MWERKS__)
    #define XML_METROWERKS
#else
    #error Code requires port to current development environment
#endif

#endif
