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
 * Revision 1.8  2002/12/12 19:30:48  peiyongz
 * Message file name changed.
 *
 * Revision 1.7  2002/11/12 17:24:58  tng
 * DOM Message: add new domain for DOM Messages.
 *
 * Revision 1.6  2002/11/04 15:24:50  tng
 * C++ Namespace Support.
 *
 * Revision 1.5  2002/07/31 18:39:53  tng
 * [Bug 3788] very long lines in CppErrMsgs_EN_US.hpp causes problems for OS390 compiler.
 *
 * Revision 1.4  2000/03/02 19:55:53  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.3  2000/02/06 07:48:41  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  2000/01/05 20:24:58  roddey
 * Some changes to simplify life for the Messge Catalog message loader. The formatter
 * for the message loader now spits out a simple header of ids that allows the loader to
 * be independent of hard coded set numbers.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:16  twl
 * Initial checkin
 *
 * Revision 1.4  1999/11/08 20:42:05  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "Xlat.hpp"


// ---------------------------------------------------------------------------
//  CppSrcFormatter: Implementation of the formatter interface
// ---------------------------------------------------------------------------
void CppSrcFormatter::endDomain(const   XMLCh* const    domainName
                                , const unsigned int    msgCount)
{
    // And close out the array declaration
    fwprintf(fOutFl, L"\n};\n");

    // Output the const size value
    fwprintf(fOutFl, L"const unsigned int %s%s = %d;\n\n", xmlStrToPrintable(fCurDomainName), longChars("Size"), msgCount);
    releasePrintableStr
}


void CppSrcFormatter::endMsgType(const MsgTypes type)
{
    if (fFirst)
    {
        fwprintf(fOutFl, L"    { ");
        fFirst = false;
    }
     else
    {
        fwprintf(fOutFl, L"  , { ");
    }

    XMLCh* rawData = typePrefixes[type];
    while (*rawData)
        fwprintf(fOutFl, L"0x%04lX,", *rawData++);

    XMLCh* tmpXMLStr = rawData = XMLString::transcode("End");
    while (*rawData)
        fwprintf(fOutFl, L"0x%04lX,", *rawData++);
    XMLString::release(&tmpXMLStr);
    fwprintf(fOutFl, L"0x00 }\n");
}


void CppSrcFormatter::endOutput()
{
    fwprintf
    (
        fOutFl,
          L"XERCES_CPP_NAMESPACE_END\n\n"
    );
    // Close the output file
    fclose(fOutFl);
}

void
CppSrcFormatter::nextMessage(const  XMLCh* const            msgText
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
    const XMLCh* rawData = msgText;
    if (fFirst)
    {
        fwprintf(fOutFl, L"    { ");
        fFirst = false;
    }
     else
    {
        fwprintf(fOutFl, L"  , { ");
    }

    unsigned int i = 0;
    while (*rawData) {
        fwprintf(fOutFl, L"0x%04lX,", *rawData++);
        if (++i == 35) {
            i = 0;
            fwprintf(fOutFl, L"\n");
            fwprintf(fOutFl, L"      ");
        }
    }
    fwprintf(fOutFl, L"0x00 }\n");
}


void CppSrcFormatter::startDomain(  const   XMLCh* const    domainName
                                    , const XMLCh* const)
{
    //
    //  We have a different array name for each domain, so store that for
    //  later use and for use below.
    //
    XMLString::release(&fCurDomainName);
    if (!XMLString::compareString(XMLUni::fgXMLErrDomain, domainName))
    {
        fCurDomainName = XMLString::transcode("gXMLErrArray");
    }
     else if (!XMLString::compareString(XMLUni::fgExceptDomain, domainName))
    {
        fCurDomainName = XMLString::transcode("gXMLExceptArray");
    }
     else if (!XMLString::compareString(XMLUni::fgValidityDomain, domainName))
    {
        fCurDomainName = XMLString::transcode("gXMLValidityArray");
    }
     else if (!XMLString::compareString(XMLUni::fgXMLDOMMsgDomain, domainName))
    {
        fCurDomainName = XMLString::transcode("gXMLDOMMsgArray");
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
    fwprintf(fOutFl, L"const XMLCh %s[][128] = \n{\n", xmlStrToPrintable(fCurDomainName));
    releasePrintableStr

    // Reset the first message trigger
    fFirst = true;
}


void CppSrcFormatter::startMsgType(const MsgTypes type)
{
    if (fFirst)
    {
        fwprintf(fOutFl, L"    { ");
        fFirst = false;
    }
     else
    {
        fwprintf(fOutFl, L"  , { ");
    }

    XMLCh* rawData = typePrefixes[type];
    while (*rawData)
        fwprintf(fOutFl, L"0x%04lX,", *rawData++);

    XMLCh *tmpXMLStr = rawData = XMLString::transcode("Start");
    while (*rawData)
        fwprintf(fOutFl, L"0x%04lX,", *rawData++);
    XMLString::release(&tmpXMLStr);

    fwprintf(fOutFl, L"0x00 }\n");
}


void CppSrcFormatter::startOutput(  const   XMLCh* const    locale
                                    , const XMLCh* const    outPath)
{
    //
    //  Ok, lets try to open the the output file. All of the messages
    //  for all the domains are put into a single Cpp file, which can be
    //  compiled into the program.
    //
    //  CppErrMsgs_xxxx.cpp
    //
    //  where xxx is the locale suffix passed in.
    //
    const unsigned int bufSize = 4095;
    XMLCh tmpBuf[bufSize + 1];
    tmpBuf[0] = 0;
    XMLCh *tmpXMLStr = XMLString::transcode("XercesMessages_");
    XMLCh *tmpXMLStr2 = XMLString::transcode(".hpp");

    XMLString::catString(tmpBuf, outPath); 
    XMLString::catString(tmpBuf, tmpXMLStr );
    XMLString::catString(tmpBuf, locale);
    XMLString::catString(tmpBuf, tmpXMLStr2 );
    XMLString::release(&tmpXMLStr);
    XMLString::release(&tmpXMLStr2);
    char *tmpStr = XMLString::transcode(tmpBuf);
    fOutFl = fopen(tmpStr, "wt");
    XMLString::release(&tmpStr);
    if ((!fOutFl) || (fwide(fOutFl, 1) <  0))
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
          L"#include <xercesc/util/XercesDefs.hpp>\n\n"
          L"XERCES_CPP_NAMESPACE_BEGIN\n\n"
    );
}
