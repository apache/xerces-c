/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.6  2004/09/08 13:57:07  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.5  2003/04/14 08:41:00  gareth
 * Xlat now works under linux - Big thanks to Neil Graham (I no longer have to find a windows box). Still slight problems working with glibc before 2.2.4 (If you mess up the parameters it seg faults due to handling of wprintf)
 *
 * Revision 1.4  2002/12/17 17:39:09  tng
 * Xlat: generate icu message file with array format as default.
 *
 * Revision 1.3  2002/12/12 23:40:38  peiyongz
 * normlize locale string.
 *
 * Revision 1.2  2002/11/05 21:15:16  peiyongz
 * generate correct <locale>.txt file
 *
 * Revision 1.1  2002/09/30 22:09:28  peiyongz
 * To generate icu resource file (in text) for error message.
 *
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "Xlat.hpp"

// ---------------------------------------------------------------------------
//  ICUResBundFormatter: Constructors and Destructor
// ---------------------------------------------------------------------------
ICUResBundFormatter::ICUResBundFormatter()
:fOutFl(0)
,fTranscoder(0)
{
    //
    //  Try to create a transcoder for the format that we were told
    //  to output in.
    //
    //  <TBD> Right now we are just using an LCP transcoding, which is not
    //  really the right thing to do!
    //
    fTranscoder = XMLPlatformUtils::fgTransService->makeNewLCPTranscoder();
    if (!fTranscoder)
    {
        wprintf(L"Could not create LCP transcoder\n");
        throw ErrReturn_NoTranscoder;
    }
}

ICUResBundFormatter::~ICUResBundFormatter()
{
	delete fTranscoder;
}

// ---------------------------------------------------------------------------
//  ICUResBundFormatter: Implementation of the formatter interface
// ---------------------------------------------------------------------------
void ICUResBundFormatter::endDomain(const   XMLCh*        const    domainName
                                  , const   unsigned int           msgCount)
{
    fwprintf(fOutFl, L"\t\t} \n\n");
}


void ICUResBundFormatter::endMsgType(const MsgTypes type)
{
#ifndef ICU_RESBUNDLE_IN_TABLE_FORM
     fwprintf(fOutFl, L"\t\t\"%s End \" ,\n", xmlStrToPrintable(typePrefixes[type]) );
     releasePrintableStr
#endif
}


void ICUResBundFormatter::endOutput()
{
    // Close the output files
    fwprintf(fOutFl, L"} \n\n");
    fclose(fOutFl);
}

void
ICUResBundFormatter::nextMessage(const  XMLCh* const            msgText
                               , const  XMLCh* const            msgId
                               , const  unsigned int            messageId
                               , const  unsigned int            curId)
{
    //
    //  We have to transcode the message text to UTF-8 in order to be able
    //  to write it out to a message catalog (which is not Unicode enabled.)
    //  If the text is just US-ASCII, this won't have any effect, but don't
    //  bother checking, just do it simple and stupid.
    //

    //
	//  array_name {
	//               "xxx" ,
	//               "yyy" ,
	//  }
	//
	//
	//  table_name {
	//       AB { "xxx" }
	//       CD { "yyy" }
	//  }
	//

#ifdef ICU_RESBUNDLE_IN_TABLE_FORM
    fwprintf(fOutFl, L"\t\t %3d { \"%s \" } \n", curId, xmlStrToPrintable(msgText));
    releasePrintableStr
	// need to print leading 0 if less than 100, not tested yet
#else
    fwprintf(fOutFl, L"\t\t\"%s \" ,\n", xmlStrToPrintable(msgText));
    releasePrintableStr
	// need a space between the last character and the closing "
#endif

}


void ICUResBundFormatter::startDomain(const   XMLCh* const    domainName
                                    , const   XMLCh* const    nameSpace)
{
    //
    //  Output:
    //           // an array
    //           domainName {
	//
	int index = XMLString::lastIndexOf(domainName, chForwardSlash);

#ifdef ICU_RESBUNDLE_IN_TABLE_FORM
    fwprintf(fOutFl, L"\n\t // a table \n");
#else
    fwprintf(fOutFl, L"\n\t // an array \n");
#endif

    fwprintf(fOutFl, L"\t%s { \n" , xmlStrToPrintable(&(domainName[index+1])));
    releasePrintableStr

}


void ICUResBundFormatter::startMsgType(const MsgTypes type)
{
#ifndef ICU_RESBUNDLE_IN_TABLE_FORM
    fwprintf(fOutFl, L"\t\t\"%s Start \" , \n", xmlStrToPrintable(typePrefixes[type]) );
    releasePrintableStr
#endif
}


void ICUResBundFormatter::startOutput(const   XMLCh* const    locale
                                    , const   XMLCh* const    outPath)
{
    //
    //  Ok, lets try to open the the output file. All of the messages
    //  for all the domains are put into a single Msg file, which can be
    //  compiled into the program.
    //
    //  CppErrMsgs_xxxx.Msg
    //
    //  where xxx is the locale suffix passed in.
    //
    const unsigned int bufSize = 4095;
    XMLCh tmpBuf[bufSize + 1];
    tmpBuf[0] = 0;
    XMLCh *tmpXMLStr = XMLString::transcode(".txt");



    XMLString::catString(tmpBuf, outPath);
    XMLString::catString(tmpBuf, locale);
    XMLString::catString(tmpBuf, tmpXMLStr );
    XMLString::release(&tmpXMLStr);
    char *tmpStr = XMLString::transcode(tmpBuf);
    fOutFl = fopen(tmpStr, "wt");
    XMLString::release(&tmpStr);
    if ((!fOutFl) || (fwide(fOutFl, 1) < 0))
    {        
        wprintf(L"Could not open the output file: %s\n\n", xmlStrToPrintable(tmpBuf) );        
        releasePrintableStr
        throw ErrReturn_OutFileOpenFailed;
    }

    // Set the message delimiter
    fwprintf(fOutFl, L"%s { \n", xmlStrToPrintable(locale) );
    releasePrintableStr

}
