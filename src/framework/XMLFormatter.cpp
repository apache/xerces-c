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

/**
 * $Log$
 * Revision 1.1  2000/03/28 19:43:17  roddey
 * Fixes for signed/unsigned warnings. New work for two way transcoding
 * stuff.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/PlatformUtils.hpp>
#include <util/XMLString.hpp>
#include <util/TransService.hpp>
#include <util/TranscodingException.hpp>
#include <util/XMLExceptMsgs.hpp>
#include <framework/XMLFormatter.hpp>


// ---------------------------------------------------------------------------
//  XMLFormatter: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLFormatter::XMLFormatter( const   char* const             outEncoding
                            , const EscapeFlags             escapeFlags
                            ,       XMLFormatTarget* const  target) :
    fEscapeFlags(escapeFlags)
    , fOutEncoding(0)
    , fTarget(target)
    , fXCoder(0)
{
    // Transcode the encoding string
    fOutEncoding = XMLString::transcode(outEncoding);

    // Try to create a transcoder for this encoding
    XMLTransService::Codes resCode;
    fXCoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor
    (
        fOutEncoding
        , resCode
        , 64 * 1024
    );

    if (!fXCoder)
    {
        delete [] fOutEncoding;
        fOutEncoding = 0;
        ThrowXML1
        (
            TranscodingException
            , XMLExcepts::Trans_CantCreateCvtrFor
            , outEncoding
        );
    }
}


XMLFormatter::XMLFormatter( const   XMLCh* const            outEncoding
                            , const EscapeFlags             escapeFlags
                            ,       XMLFormatTarget* const  target) :
    fEscapeFlags(escapeFlags)
    , fOutEncoding(0)
    , fTarget(target)
    , fXCoder(0)
{
    // Copy the encoding string
    fOutEncoding = XMLString::replicate(outEncoding);

    // Try to create a transcoder for this encoding
    XMLTransService::Codes resCode;
    fXCoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor
    (
        fOutEncoding
        , resCode
        , kTmpBufSize
    );

    if (!fXCoder)
    {
        delete [] fOutEncoding;
        fOutEncoding = 0;
        ThrowXML1
        (
            TranscodingException
            , XMLExcepts::Trans_CantCreateCvtrFor
            , outEncoding
        );
    }
}

XMLFormatter::~XMLFormatter()
{
    delete [] fOutEncoding;
    delete fXCoder;

    // We DO NOT own the target object!
}


// ---------------------------------------------------------------------------
//  XMLFormatter: Formatting methods
// ---------------------------------------------------------------------------
void
XMLFormatter::formatBuf(const XMLCh* const toFormat, const unsigned int count)
{
    //
    //  If we don't have any escape flags set, then we can do the most
    //  efficient loop, else we have to do it the hard way.
    //
    unsigned int    srcCount = count;
    const XMLCh*    srcPtr = toFormat;
    if (fEscapeFlags == NoEscapes)
    {
        //
        //  Just do a whole buffer at a time into the temp buffer, cap it
        //  off, and send it to the target.
        //
        while (srcCount)
        {
            unsigned int charsEaten;

            const unsigned srcChars = srcCount > kTmpBufSize ?
                                      kTmpBufSize : srcCount;

            const unsigned int outBytes = fXCoder->transcodeTo
            (
                srcPtr
                , srcChars
                , fTmpBuf
                , kTmpBufSize
                , charsEaten
                , XMLTranscoder::UnRep_Throw
            );

            #if XML_DEBUG
            if ((outBytes > kTmpBufSize)
            ||  (charsEaten > srcCount))
            {
                // <TBD> The transcoder is freakin out maaaannn
            }
            #endif

            if (outBytes)
            {
                fTmpBuf[outBytes] = 0;
                fTarget->writeChars(fTmpBuf);
            }

            srcCount -= charsEaten;
        }
    }
     else
    {
    }
}

XMLFormatter& XMLFormatter::operator<<(const XMLCh* const toFormat)
{
    const unsigned int len = XMLString::stringLen(toFormat);
    formatBuf(toFormat, len);
    return *this;
}

XMLFormatter& XMLFormatter::operator<<(const char* const toFormat)
{
    fTarget->writeChars((const XMLByte*)toFormat);
    return *this;
}
