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
 * Revision 1.9  2003/04/14 08:41:00  gareth
 * Xlat now works under linux - Big thanks to Neil Graham (I no longer have to find a windows box). Still slight problems working with glibc before 2.2.4 (If you mess up the parameters it seg faults due to handling of wprintf)
 *
 * Revision 1.8  2002/11/05 22:10:06  tng
 * Explicit code using namespace in application.
 *
 * Revision 1.7  2002/09/30 22:09:58  peiyongz
 * To generate icu resource file (in text) for error message.
 *
 * Revision 1.6  2002/07/04 17:40:07  tng
 * Use new DOM in Xlat.
 *
 * Revision 1.5  2002/02/01 23:48:37  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/05/03 19:09:38  knoaman
 * Support Warning/Error/FatalError messaging.
 * Validity constraints errors are treated as errors, with the ability by user to set
 * validity constraints as fatal errors.
 *
 * Revision 1.3  2000/03/02 19:55:53  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:41  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:14  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:42:05  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

// ---------------------------------------------------------------------------
//  Some globally used types
// ---------------------------------------------------------------------------
enum MsgTypes
{
    MsgType_Warning
    , MsgType_Error
    , MsgType_FatalError

    , MsgTypes_Count
};


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include "Xlat_ErrHandler.hpp"
#include "Xlat_Types.hpp"
#include "Xlat_Formatter.hpp"
#include "Xlat_CppSrc.hpp"
#include "Xlat_Win32RC.hpp"
#include "Xlat_MsgCatalog.hpp"
#include "Xlat_ICUResourceBundle.hpp"

XERCES_CPP_NAMESPACE_USE


// ---------------------------------------------------------------------------
//  Some const global data
// ---------------------------------------------------------------------------
extern XMLCh* typePrefixes[MsgTypes_Count];


// this ugly hack is needed because cygwin/linux and Windows (MSVC++) 
// have irreconcileable differences about what to do with chars, wchar_t and XMLCh
// in wfprintf.  Windows thinks that XMLCh * is fine here whereas 
// char * is not; gcc will allow XMLCh to be cast to wchar_t but happily
// prints out gobbledygook in this case; it only seems happy when 
// the native transcoder is used to convert the XMLCh to a char *
#if defined(__linux__) || defined(__CYGWIN__)
    extern char *fTmpStr;
    #define xmlStrToPrintable(xmlStr) \
        (fTmpStr = XMLString::transcode(xmlStr))
    #define releasePrintableStr \
        XMLString::release(&fTmpStr);
    #define longChars(str) str
#elif defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__)
    extern wchar_t *longChars(const char *str);
    #define xmlStrToPrintable(xmlStr) xmlStr
    #define releasePrintableStr 
#else
    #error Code requires port to host OS!
#endif

