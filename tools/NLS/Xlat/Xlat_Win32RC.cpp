/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.7  2004/09/08 13:57:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.6  2003/04/14 08:41:00  gareth
 * Xlat now works under linux - Big thanks to Neil Graham (I no longer have to find a windows box). Still slight problems working with glibc before 2.2.4 (If you mess up the parameters it seg faults due to handling of wprintf)
 *
 * Revision 1.5  2002/11/12 17:24:58  tng
 * DOM Message: add new domain for DOM Messages.
 *
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
	XMLString::release(&fCurDomainName);
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
    XMLString::release(&fCurDomainName);    
    if (!XMLString::compareString(XMLUni::fgXMLErrDomain, domainName))
    {    	
        fCurDomainName = XMLString::transcode("gXMLErrArray");
        fMsgOffset = 0;
    }
     else if (!XMLString::compareString(XMLUni::fgExceptDomain, domainName))
    {
        fCurDomainName = XMLString::transcode("gXMLExceptArray");
        fMsgOffset = 0x2000;
    }
     else if (!XMLString::compareString(XMLUni::fgValidityDomain, domainName))
    {
        fCurDomainName = XMLString::transcode("gXMLValidityArray");
        fMsgOffset = 0x4000;
    }
     else if (!XMLString::compareString(XMLUni::fgXMLDOMMsgDomain, domainName))
    {
        fCurDomainName = XMLString::transcode("gXMLDOMMsgArray");
        fMsgOffset = 0x6000;
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
    // make sure the append will work
    tmpBuf[0] = 0;
    XMLCh *tmpXMLStr = XMLString::transcode("CppErrMsgs_");
    XMLCh *tmpXMLStr2 = XMLString::transcode(".RC");

    XMLString::catString(tmpBuf, outPath);
    XMLString::catString(tmpBuf, tmpXMLStr );
    XMLString::catString(tmpBuf, locale);
    XMLString::catString(tmpBuf, tmpXMLStr2 );
    XMLString::release(&tmpXMLStr);
    XMLString::release(&tmpXMLStr2);
    char *tmpStr = XMLString::transcode(tmpBuf);    
    fOutFl = fopen(tmpStr, "wt");
    XMLString::release(&tmpStr);
    if ((!fOutFl) || (fwide(fOutFl,1) < 0))
    {

        wprintf(L"Could not open the output file: %s\n\n", xmlStrToPrintable(tmpBuf) );
        releasePrintableStr
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
