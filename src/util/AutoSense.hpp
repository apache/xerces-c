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
#elif defined(_SEQUENT_)
    #define XML_PTX
    #define XML_UNIX
#elif defined(_HP_UX) || defined(__hpux) || defined(_HPUX_SOURCE)
    #define XML_HPUX
    #define XML_UNIX
#elif defined(SOLARIS) || defined(__SVR4) || defined(UNIXWARE)
    #if defined(UNIXWARE)
        #define XML_UNIXWARE
        #define XML_CSET
        #define XML_SCOCC
        #define XML_UNIX
    #else
        #define XML_SOLARIS
        #define XML_UNIX
    #endif
#elif defined(__linux__)
    #define XML_LINUX
    #define XML_UNIX
#elif defined(IRIX)
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

#elif defined(macintosh)
    #define XML_MACOS
#else
    #error Code requires port to host OS!
#endif


// ---------------------------------------------------------------------------
//  This section attempts to autodetect the compiler being used. It will set
//  up Xerces specific defines that can be used by the rest of the code.
// ---------------------------------------------------------------------------
#if defined(_MSC_VER)
    #define XML_VISUALCPP
#elif defined(__BORLANDC__)
    #define XML_BORLAND
#elif defined(__xlC__)
    #define XML_CSET
#elif defined(XML_SOLARIS) || defined(XML_UNIXWARE)
    #if defined(__SUNPRO_CC) & __SUNPRO_CC >=0x500
        #define XML_SUNCC5
	#elif defined(__SUNPRO_CC) & __SUNPRO_CC <0x500
        #define XML_SUNCC
    #elif defined(_EDG_RUNTIME_USES_NAMESPACES)
        #define XML_SOLARIS_KAICC
    #elif defined(__GNUG__)
		#define XML_GCC
    #endif
#elif defined (__GNUG__) || defined(__linux__)
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
#elif defined(__OS400__)
#else
    #error Code requires port to current development environment
#endif

#endif
