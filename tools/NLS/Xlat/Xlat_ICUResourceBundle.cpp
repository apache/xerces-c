/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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
 * $Id$
 * $Log$
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
#ifdef ICU_RESBUNDLE_IN_ARRAY_FORM
     fwprintf(fOutFl, L"\t\t\"%S End \" ,\n", typePrefixes[type]);  
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

	char* outData = fTranscoder->transcode(msgText);

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

#ifdef ICU_RESBUNDLE_IN_ARRAY_FORM
    fwprintf(fOutFl, L"\t\t\"%S \" ,\n", outData);  
	// need a space between the last character and the closing "
#else
    fwprintf(fOutFl, L"\t\t %3d { \"%S \" } \n", curId, outData);
	// need to print leading 0 if less than 100, not tested yet
#endif

    delete [] outData;
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
	char *tmpDomainName = fTranscoder->transcode(&(domainName[index+1]));

#ifdef ICU_RESBUNDLE_IN_ARRAY_FORM
    fwprintf(fOutFl, L"\n\t // an array \n");
#else
    fwprintf(fOutFl, L"\n\t // a table \n");
#endif

    fwprintf(fOutFl, L"\t%S { \n" , tmpDomainName);

	delete [] tmpDomainName;

}


void ICUResBundFormatter::startMsgType(const MsgTypes type)
{
#ifdef ICU_RESBUNDLE_IN_ARRAY_FORM
    fwprintf(fOutFl, L"\t\t\"%S Start \" , \n", typePrefixes[type]);  
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

    swprintf(tmpBuf, L"%s/%s_%s.Msg", outPath, L"root", locale);
    fOutFl = _wfopen(tmpBuf, L"wt");
    if (!fOutFl)
    {
        wprintf(L"Could not open the output file: %s\n\n", tmpBuf);
        throw ErrReturn_OutFileOpenFailed;
    }

    // Set the message delimiter
    fwprintf(fOutFl, L"root { \n");

}
