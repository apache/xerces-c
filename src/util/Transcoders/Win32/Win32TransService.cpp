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
 * Revision 1.2  1999/12/01 18:54:26  roddey
 * Small syntactical change to make it compile under Borland's compiler.
 * It does not make it worse under other compilers, so why not. Basically
 * it was just changing ::iswspace() to iswspace(), i.e. get rid of the ::
 * prefix, which freaked out Borland for some reason.
 *
 * Revision 1.1.1.1  1999/11/09 01:06:04  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "Win32TransService.hpp"
#include <windows.h>


// ---------------------------------------------------------------------------
//  Win32TransService: Constructors and Destructor
// ---------------------------------------------------------------------------
Win32TransService::Win32TransService()
{
}

Win32TransService::~Win32TransService()
{
}


// ---------------------------------------------------------------------------
//  Win32TransService: The virtual transcoding service API
// ---------------------------------------------------------------------------
int Win32TransService::compareIString(  const   XMLCh* const    comp1
                                        , const XMLCh* const    comp2)
{
    return _wcsicmp(comp1, comp2);
}

int Win32TransService::compareNIString( const   XMLCh* const    comp1
                                        , const XMLCh* const    comp2
                                        , const unsigned int    maxChars)
{
    return _wcsnicmp(comp1, comp2, maxChars);
}

bool Win32TransService::isSpace(const XMLCh toCheck) const
{
    return (iswspace(toCheck) != 0);
}

XMLTranscoder* Win32TransService::makeNewDefTranscoder()
{
    // Just allocate a new transcoder of our type
    return new Win32Transcoder;
}

XMLTranscoder*
Win32TransService::makeNewTranscoderFor(const   XMLCh* const            encodingName
                                        ,       XMLTransService::Codes& resValue
                                        , const unsigned int            )
{
    //
    //  NOTE: We don't use the block size here
    //

    //
    //  This is a minimalist transcoding service, that only supports a local
    //  default transcoder. All named encodings return zero as a failure,
    //  which means that only the intrinsic encodings supported by the parser
    //  itself will work for XML data.
    //
    resValue = XMLTransService::UnsupportedEncoding;
    return 0;
}




// ---------------------------------------------------------------------------
//  Win32Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
Win32Transcoder::Win32Transcoder()
{
}

Win32Transcoder::~Win32Transcoder()
{
}


// ---------------------------------------------------------------------------
//  Win32Transcoder: The virtual transcoder API
// ---------------------------------------------------------------------------
unsigned int Win32Transcoder::calcRequiredSize(const char* const srcText)
{
    if (!srcText)
        return 0;

    const unsigned int retVal = ::mbstowcs(0, srcText, 0);
    if (retVal == (unsigned int)-1)
        return 0;
    return retVal;
}


unsigned int Win32Transcoder::calcRequiredSize(const XMLCh* const srcText)
{
    if (!srcText)
        return 0;

    const unsigned int retVal = ::wcstombs(0, srcText, 0);
    if (retVal == (unsigned int)-1)
        return 0;
    return retVal;
}


XMLCh Win32Transcoder::transcodeOne(const   char* const     srcData
                                    , const unsigned int    srcBytes
                                    ,       unsigned int&   bytesEaten)
{
    XMLCh toFill;
    size_t eaten = ::mbtowc(&toFill, srcData, srcBytes);
    if (eaten == size_t(-1))
    {
        bytesEaten = 0;
        return 0;
    }

    // Return the bytes we ate and the resulting char
    bytesEaten = eaten;
    return toFill;
}


char* Win32Transcoder::transcode(const XMLCh* const toTranscode)
{
    if (!toTranscode)
        return 0;

    char* retVal = 0;
    if (toTranscode)
    {
        // Calc the needed size
        const unsigned int neededLen = ::wcstombs(0, toTranscode, 0);
        if (neededLen == (unsigned int)-1)
            return 0;

        retVal = new char[neededLen + 1];
        ::wcstombs(retVal, toTranscode, neededLen + 1);
    }
     else
    {
        retVal = new char[1];
        retVal[0] = 0;
    }
    return retVal;
}


bool Win32Transcoder::transcode(const   XMLCh* const    toTranscode
                                ,       char* const     toFill
                                , const unsigned int    maxBytes)
{
    // Watch for a couple of pyscho corner cases
    if (!toTranscode || !maxBytes)
    {
        toFill[0] = 0;
        return true;
    }

    if (!*toTranscode)
    {
        toFill[0] = 0;
        return true;
    }

    // Ok, go ahead and try the transcoding. If it fails, then 
    if (::wcstombs(toFill, toTranscode, maxBytes + 1) == size_t(-1))
        return false;

    // Cap it off just in case
    toFill[maxBytes] = 0;
    return true;
}


XMLCh* Win32Transcoder::transcode(const char* const toTranscode)
{
    if (!toTranscode)
        return 0;

    XMLCh* retVal = 0;
    if (toTranscode)
    {
        const unsigned int len = ::mbstowcs(0, toTranscode, 0);
        if (len == (unsigned int)-1)
            return 0;

        retVal = new XMLCh[len + 1];
        ::mbstowcs(retVal, toTranscode, len + 1);
    }
     else
    {
        retVal = new XMLCh[1];
        retVal[0] = 0;
    }
    return retVal;
}


bool Win32Transcoder::transcode(const   char* const     toTranscode
                                ,       XMLCh* const    toFill
                                , const unsigned int    maxChars)
{
    // Check for a couple of psycho corner cases
    if (!toTranscode || !maxChars)
    {
        toFill[0] = 0;
        return true;
    }

    if (!*toTranscode)
    {
        toFill[0] = 0;
        return true;
    }

    // Looks ok so lets transcode it
    if (::mbstowcs(toFill, toTranscode, maxChars + 1) == size_t(-1))
        return false;
    return true;
}


unsigned int
Win32Transcoder::transcodeXML(  const   char* const             srcData
                                , const unsigned int            srcCount
                                ,       XMLCh* const            toFill
                                , const unsigned int            maxChars
                                ,       unsigned int&           bytesEaten)
{
    //
    //  For this one, because we have to maintain the offset table, we have
    //  to do them one char at a time until we run out of source data.
    //
    unsigned int countIn = 0;
    unsigned int countOut = 0;
    while (countOut < maxChars)
    {
        const int bytesEaten = ::mbtowc
        (
            &toFill[countOut]
            , &srcData[countIn]
            , srcCount - countIn
        );

        // We are done, so break out
        if (bytesEaten == -1)
            break;

        countIn += (unsigned int)bytesEaten;
        countOut++;
    }

    // Give back the counts of eaten and transcoded
    bytesEaten = countIn;
    return countOut;
}
