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
 * Revision 1.3  2000/01/05 20:24:58  roddey
 * Some changes to simplify life for the Messge Catalog message loader. The formatter
 * for the message loader now spits out a simple header of ids that allows the loader to
 * be independent of hard coded set numbers.
 *
 * Revision 1.2  1999/12/20 22:51:09  roddey
 * Updated to deal with the new transcoder interface.
 *
 * Revision 1.1.1.1  1999/11/09 01:01:24  twl
 * Initial checkin
 *
 * Revision 1.5  1999/11/08 20:42:06  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "Xlat.hpp"



// ---------------------------------------------------------------------------
//  MsgCatFormatter: Constructors and Destructor
// ---------------------------------------------------------------------------
MsgCatFormatter::MsgCatFormatter() :

    fOutFl(0)
    , fTranscoder(0)
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

MsgCatFormatter::~MsgCatFormatter()
{
}


// ---------------------------------------------------------------------------
//  MsgCatFormatter: Implementation of the formatter interface
// ---------------------------------------------------------------------------
void MsgCatFormatter::endDomain(const   XMLCh* const    domainName
                                , const unsigned int    msgCount)
{
    fwprintf(fOutFl, L"\n\n");
}


void MsgCatFormatter::endMsgType(const MsgTypes type)
{
}


void MsgCatFormatter::endOutput()
{
    // Close the output files
    fclose(fOutFl);
    fclose(fOutHpp);
}

void
MsgCatFormatter::nextMessage(const  XMLCh* const            msgText
                            , const XMLCh* const            msgId
                            , const unsigned int            messageId
                            , const unsigned int            curId)
{
    //
    //  We have to transcode the message text to UTF-8 in order to be able
    //  to write it out to a message catalog (which is not Unicode enabled.)
    //  If the text is just US-ASCII, this won't have any effect, but don't
    //  bother checking, just do it simple and stupid.
    //
    char* outData = fTranscoder->transcode(msgText);
    fwprintf(fOutFl, L"%3d  %S\n", curId, outData);
    delete [] outData;
}


void MsgCatFormatter::startDomain(  const   XMLCh* const    domainName
                                    , const XMLCh* const    nameSpace)
{
    // Output a constant to the header file
    fwprintf(fOutHpp, L"const unsigned int CatId_%s = %d;\n", nameSpace, fSeqId);

    //
    //  Output the leading part of the array declaration. Its just an
    //  array of pointers to Unicode chars.
    //
    fwprintf(fOutFl, L"$set %u\n", fSeqId);

    // And bump the sequence id
    fSeqId++;
}


void MsgCatFormatter::startMsgType(const MsgTypes type)
{
}


void MsgCatFormatter::startOutput(  const   XMLCh* const    locale
                                    , const XMLCh* const    outPath)
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

    swprintf(tmpBuf, L"%s/%s_%s.Msg", outPath, L"XMLMsgCat", locale);
    fOutFl = _wfopen(tmpBuf, L"wt");
    if (!fOutFl)
    {
        wprintf(L"Could not open the output file: %s\n\n", tmpBuf);
        throw ErrReturn_OutFileOpenFailed;
    }

    // Set the message delimiter
    fwprintf(fOutFl, L"$quote \"\n");


    swprintf(tmpBuf, L"%s/%s.hpp", outPath, L"XMLMsgCat_Ids");
    fOutHpp = _wfopen(tmpBuf, L"wt");
    if (!fOutHpp)
    {
        wprintf(L"Could not open the output file: %s\n\n", tmpBuf);
        throw ErrReturn_OutFileOpenFailed;
    }

    // Reset the sequence id
    fSeqId = 1;
}
