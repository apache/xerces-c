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
 * Revision 1.4  2000/03/02 19:55:54  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:42  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/05 20:24:58  roddey
 * Some changes to simplify life for the Messge Catalog message loader. The formatter
 * for the message loader now spits out a simple header of ids that allows the loader to
 * be independent of hard coded set numbers.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:30  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:42:07  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "Xlat.hpp"


// ---------------------------------------------------------------------------
//  Win32RCFormatter: Public Constructors and Destructor
// ---------------------------------------------------------------------------
Win32RCFormatter::Win32RCFormatter() :

    fCurDomainName(0)
    , fMsgOffset(0)
    , fOutFl(0)
{
}

Win32RCFormatter::~Win32RCFormatter()
{
}


// ---------------------------------------------------------------------------
//  Win32RCFormatter: Implementation of the formatter interface
// ---------------------------------------------------------------------------
void Win32RCFormatter::endDomain(const  XMLCh* const    domainName
                                , const unsigned int    msgCount)
{
    // And close out the message table declaration
    fwprintf(fOutFl, L"END\n");
}


void Win32RCFormatter::endMsgType(const MsgTypes type)
{
    // No-op for us
}


void Win32RCFormatter::endOutput()
{
    // Close the output file
    fclose(fOutFl);
}


void
Win32RCFormatter::nextMessage(  const  XMLCh* const             msgText
                                , const XMLCh* const            msgId
                                , const unsigned int            messageId
                                , const unsigned int            curId)
{
    //
    //  We do not transcode to the output format in this case. Instead we
    //  just store the straight Unicode format. Because we cannot assume 'L'
    //  type prefix support, we have to put them out as numeric character
    //  values.
    //
    fwprintf(fOutFl, L"    %-16d  L\"", messageId + fMsgOffset);

    const XMLCh* rawData = msgText;
    while (*rawData)
        fwprintf(fOutFl, L"\\x%04lX", *rawData++);
    fwprintf(fOutFl, L"\\x00\"\n");
}


void Win32RCFormatter::startDomain( const   XMLCh* const    domainName
                                    , const XMLCh* const)
{
    //
    //  We have a different array name for each domain, so store that for
    //  later use and for use below.
    //
    if (!XMLString::compareString(XMLUni::fgXMLErrDomain, domainName))
    {
        fCurDomainName = L"gXMLErrArray";
        fMsgOffset = 0;
    }
     else if (!XMLString::compareString(XMLUni::fgExceptDomain, domainName))
    {
        fCurDomainName = L"gXMLExceptArray";
        fMsgOffset = 0x2000;
    }
     else if (!XMLString::compareString(XMLUni::fgValidityDomain, domainName))
    {
        fCurDomainName = L"gXMLValidityArray";
        fMsgOffset = 0x4000;
    }
     else
    {
        wprintf(L"Unknown message domain: %s\n", domainName);
        throw ErrReturn_SrcFmtError;
    }

    //
    //  Output the leading part of the array declaration. Its just an
    //  array of pointers to Unicode chars.
    //
    fwprintf
    (
        fOutFl
        , L"STRINGTABLE DISCARDABLE\nBEGIN\n"
    );
}


void Win32RCFormatter::startMsgType(const MsgTypes type)
{
    // No-op for us
}


void Win32RCFormatter::startOutput(  const  XMLCh* const locale
                                    , const XMLCh* const outPath)
{
    //
    //  Ok, lets try to open the the output file. All of the messages for all
    //  the domains are put into a single message tabble in a single RC file,
    //  which can be linked into the program.
    //
    //  CppErrMsgs_xxxx.RC
    //
    //  where xxx is the locale suffix passed in.
    //
    const unsigned int bufSize = 4095;
    XMLCh tmpBuf[bufSize + 1];

    swprintf(tmpBuf, L"%s/%s_%s.RC", outPath, L"CppErrMsgs", locale);
    fOutFl = _wfopen(tmpBuf, L"wt");
    if (!fOutFl)
    {
        wprintf(L"Could not open the output file: %s\n\n", tmpBuf);
        throw ErrReturn_OutFileOpenFailed;
    }

    //
    //  Ok, lets output the grunt data at the start of the file. We put out a
    //  comment that indicates its a generated file, and the title string.
    //
    fwprintf
    (
        fOutFl
        , L"// ----------------------------------------------------------------\n"
          L"//  This file was generated from the XML error message source.\n"
          L"//  so do not edit this file directly!!\n"
          L"// ----------------------------------------------------------------\n\n"
    );
}
