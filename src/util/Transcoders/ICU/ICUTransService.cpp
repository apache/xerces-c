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
 * Revision 1.5  1999/12/15 19:43:45  roddey
 * Now implements the new transcoding abstractions, with separate interface
 * classes for XML transcoders and local code page transcoders.
 *
 * Revision 1.4  1999/12/07 23:08:41  roddey
 * Add in code to test for some control characters and report them as whitespace.
 * ICU is not doing this currently, so we need to do it until they get that fixed.
 *
 * Revision 1.3  1999/11/18 20:16:52  abagchi
 * Now works with ICU 1.3.1
 *
 * Revision 1.2  1999/11/17 22:36:41  rahulj
 * Code works with ICU transcoding service
 *
 * Revision 1.1.1.1  1999/11/09 01:06:07  twl
 * Initial checkin
 *
 * Revision 1.3  1999/11/08 20:45:33  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/TranscodingException.hpp>
#include "ICUTransService.hpp"
#include <string.h>
#include <uloc.h>
#include <unicode.h>
#include <ucnv.h>
#include <ustring.h>



// ---------------------------------------------------------------------------
//  ICUTransService: Public, static methods
// ---------------------------------------------------------------------------
void ICUTransService::setICUPath(const char* const pathToSet)
{
    uloc_setDataDirectory(pathToSet);
}


// ---------------------------------------------------------------------------
//  ICUTransService: Constructors and Destructor
// ---------------------------------------------------------------------------
ICUTransService::ICUTransService()
{
}

ICUTransService::~ICUTransService()
{
}


// ---------------------------------------------------------------------------
//  ICUTransService: The virtual transcoding service API
// ---------------------------------------------------------------------------
int ICUTransService::compareIString(const   XMLCh* const    comp1
                                    , const XMLCh* const    comp2)
{
    const XMLCh* psz1 = comp1;
    const XMLCh* psz2 = comp2;

    unsigned int curCount = 0;
    while (true)
    {
        // If an inequality, then return the difference
        if (Unicode::toUpperCase(*psz1) != Unicode::toUpperCase(*psz2))
            return int(*psz1) - int(*psz2);

        // If either has ended, then they both ended, so equal
        if (!*psz1 || !*psz2)
            break;

        // Move upwards for the next round
        psz1++;
        psz2++;
    }
    return 0;
}


int ICUTransService::compareNIString(const  XMLCh* const    comp1
                                    , const XMLCh* const    comp2
                                    , const unsigned int    maxChars)
{
    const XMLCh* psz1 = comp1;
    const XMLCh* psz2 = comp2;

    unsigned int curCount = 0;
    while (true)
    {
        // If an inequality, then return difference
        if (Unicode::toUpperCase(*psz1) != Unicode::toUpperCase(*psz2))
            return int(*psz1) - int(*psz2);

        // If either ended, then both ended, so equal
        if (!*psz1 || !*psz2)
            break;

        // Move upwards to next chars
        psz1++;
        psz2++;

        //
        //  Bump the count of chars done. If it equals the count then we 
        //  are equal for the requested count, so break out and return
        //  equal.
        //
        curCount++;
        if (maxChars == curCount)
            break;
    }
    return 0;
}


bool ICUTransService::isSpace(const XMLCh toCheck) const
{
    //
    //  <TBD>
    //  For now, we short circuit some of the control chars because ICU
    //  is not correctly reporting them as space. Later, when they change
    //  this, we can get rid of this special case.
    //
    if ((toCheck == 0x09)
    ||  (toCheck == 0x0A)
    ||  (toCheck == 0x0D))
    {
        return true;
    }
    return (Unicode::isSpaceChar(toCheck) != 0);
}


XMLLCPTranscoder* ICUTransService::makeNewLCPTranscoder()
{
    //
    //  Try to create a default converter. If it fails, return a null pointer
    //  which will basically cause the system to give up because we really can't
    //  do anything without one.
    //
    UErrorCode uerr = U_ZERO_ERROR;
    UConverter* converter = ucnv_open(NULL, &uerr);
    if (!converter)
        return 0;

    // That went ok, so create an ICU LCP transcoder wrapper and return it
    return new ICULCPTranscoder(converter);
}


XMLTranscoder*
ICUTransService::makeNewTranscoderFor(  const   XMLCh* const            encodingName
                                        ,       XMLTransService::Codes& resValue
                                        , const unsigned int            blockSize)
{
    UErrorCode uerr = U_ZERO_ERROR;
    UConverter* converter = ucnv_openU(encodingName, &uerr);
    if (!converter)
    {
        resValue = XMLTransService::UnsupportedEncoding;
        return 0;
    }
    return new ICUTranscoder(encodingName, converter, blockSize);
}




// ---------------------------------------------------------------------------
//  ICUTranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
ICUTranscoder::ICUTranscoder(const  XMLCh* const        encodingName
                            ,       UConverter* const   toAdopt
                            , const unsigned int        blockSize) :

    XMLTranscoder(encodingName, blockSize)
    , fConverter(toAdopt)
    , fFixed(false)
    , fSrcOffsets(0)
{
    // If there is a block size, then allocate our source offset array
    if (blockSize)
        fSrcOffsets = new long[blockSize];

    // Remember if its a fixed size encoding
    fFixed = (ucnv_getMaxCharSize(fConverter) == ucnv_getMinCharSize(fConverter));
}

ICUTranscoder::~ICUTranscoder()
{
    delete [] fSrcOffsets;

    // If there is a converter, ask ICU to clean it up
    if (fConverter)
    {
        // <TBD> Does this actually delete the structure???
        ucnv_close(fConverter);
        fConverter = 0;
    }
}


// ---------------------------------------------------------------------------
//  ICUTranscoder: The virtual transcoder API
// ---------------------------------------------------------------------------
bool ICUTranscoder::supportsSrcOfs() const
{
    // This implementation supports source offset information
    return true;
}


XMLCh ICUTranscoder::transcodeOne(  const   XMLByte* const  srcData
                                    , const unsigned int    srcBytes
                                    ,       unsigned int&   bytesEaten)
{
    // Check for stupid stuff
    if (!srcBytes)
        return 0;

    UErrorCode err = U_ZERO_ERROR;
    const XMLByte* startSrc = srcData;
    const XMLCh chRet = ucnv_getNextUChar
    (
        fConverter
        , (const char**)&startSrc
        , (const char*)((srcData + srcBytes) - 1)
        , &err
    );

    // Bail out if an error
    if (U_FAILURE(err))
        return 0;

    // Calculate the bytes eaten and return the char
    bytesEaten = startSrc - srcData;
    return chRet;
}


unsigned int
ICUTranscoder::transcodeXML(const   XMLByte* const          srcData
                            , const unsigned int            srcCount
                            ,       XMLCh* const            toFill
                            , const unsigned int            maxChars
                            ,       unsigned int&           bytesEaten
                            ,       unsigned char* const    charSizes)
{
    // If debugging, insure the block size is legal
    #if defined(XML4C_DEBUG)
    checkBlockSize(maxChars);
    #endif

    // Set up pointers to the source and destination buffers.
    UChar*          startTarget = toFill;
    const XMLByte*  startSrc = srcData;
    const XMLByte*  endSrc = srcData + srcCount;

    //
    //  Transoode the buffer.  Buffer overflow errors are normal, occuring
    //  when the raw input buffer holds more characters than will fit in
    //  the Unicode output buffer.
    //
    UErrorCode  err = U_ZERO_ERROR;
    ucnv_toUnicode
    (
        fConverter
        , &startTarget
        , toFill + maxChars
        , (const char**)&startSrc
        , (const char*)endSrc
        , (fFixed ? 0 : fSrcOffsets)
        , false
        , &err
    );

    if ((err != U_ZERO_ERROR) && (err != U_INDEX_OUTOFBOUNDS_ERROR))
        ThrowXML(TranscodingException, XML4CExcepts::Trans_CouldNotXCodeXMLData);

    // Calculate the bytes eaten and store in caller's param
    bytesEaten = startSrc - srcData;

    // And the characters decoded
    const unsigned int charsDecoded = startTarget - toFill;

    //
    //  Translate the array of char offsets into an array of character
    //  sizes, which is what the transcoder interface semantics requires.
    //  If its fixed, then we can optimize it.
    //
    if (fFixed)
    {
        const unsigned char fillSize = (unsigned char)ucnv_getMaxCharSize(fConverter);;
        memset(charSizes, fillSize, maxChars);
    }
     else
    {
        //
        //  We have to convert the series of offsets into a series of
        //  sizes. If just one char was decoded, then its the total bytes
        //  eaten. Otherwise, do a loop and subtract out each element from
        //  its previous element.
        //
        if (charsDecoded == 1)
        {
            charSizes[0] = (unsigned char)bytesEaten;
        }
         else
        {
            // <TBD> Does ICU return an extra element to allow us to figure
            //  out the last char size? It better!!
            unsigned int index;
            for (index = 0; index < charsDecoded; index++)
            {
                charSizes[index] = (unsigned char)(fSrcOffsets[index + 1]
                                                    - fSrcOffsets[index]);
            }
        }
    }

    // Return the chars we put into the target buffer
    return charsDecoded;
}




// ---------------------------------------------------------------------------
//  ICULCPTranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
ICULCPTranscoder::ICULCPTranscoder(UConverter* const toAdopt) :

    fConverter(toAdopt)
{
}

ICULCPTranscoder::~ICULCPTranscoder()
{
    // If there is a converter, ask ICU to clean it up
    if (fConverter)
    {
        // <TBD> Does this actually delete the structure???
        ucnv_close(fConverter);
        fConverter = 0;
    }
}


// ---------------------------------------------------------------------------
//  ICULCPTranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
unsigned int ICULCPTranscoder::calcRequiredSize(const XMLCh* const srcText)
{
    if (!srcText)
        return 0;

    // Lock and attempt the calculation
    UErrorCode err = U_ZERO_ERROR;
    int32_t targetCap;
    {
        XMLMutexLock lockConverter(&fMutex);

        targetCap = ucnv_fromUChars
        (
            fConverter
            , 0
            , 0
            , srcText
            , &err
        );
    }

    if (err != U_BUFFER_OVERFLOW_ERROR)
        return 0;

    return (unsigned int)targetCap;
}

unsigned int ICULCPTranscoder::calcRequiredSize(const char* const srcText)
{
    if (!srcText)
        return 0;

    int32_t targetCap;
    UErrorCode err = U_ZERO_ERROR;
    {
        XMLMutexLock lockConverter(&fMutex);

        targetCap = ucnv_toUChars
        (
            fConverter
            , 0
            , 0
            , srcText
            , strlen(srcText)
            , &err
        );
    }

    if (err != U_BUFFER_OVERFLOW_ERROR)
        return 0;

    // Subtract one since it includes the terminator space
    return (unsigned int)(targetCap - 1);
}


char* ICULCPTranscoder::transcode(const XMLCh* const toTranscode)
{
    char* retBuf = 0;

    // Check for a couple of special cases
    if (!toTranscode)
        return 0;

    if (!*toTranscode)
    {
        retBuf = new char[1];
        retBuf[0] = 0;
        return retBuf;
    }

    // Caculate a return buffer size not too big, but less likely to overflow
    int32_t targetLen = (int32_t)(u_strlen(toTranscode) * 1.25);

    // Allocate the return buffer
    retBuf = new char[targetLen + 1];

    // Lock now while we call the converter.
    UErrorCode err = U_ZERO_ERROR;
    int32_t targetCap;
    {
        XMLMutexLock lockConverter(&fMutex);

        //Convert the Unicode string to char*
        targetCap = ucnv_fromUChars
        (
            fConverter
            , retBuf
            , targetLen + 1
            , toTranscode
            , &err
        );
    }

    // If targetLen is not enough then buffer overflow might occur
    if (err == U_BUFFER_OVERFLOW_ERROR)
    {
        // Reset the error, delete the old buffer, allocate a new one, and try again
        err = U_ZERO_ERROR;
        delete [] retBuf;
        retBuf = new char[targetCap];

        // Lock again before we retry
        XMLMutexLock lockConverter(&fMutex);
        targetCap = ucnv_fromUChars
        (
            fConverter
            , retBuf
            , targetCap
            , toTranscode
            , &err
        );
    }

    if (U_FAILURE(err))
    {
        delete [] retBuf;
        return 0;
    }

    // Cap it off and return
    retBuf[targetCap] = 0;
    return retBuf;
}

XMLCh* ICULCPTranscoder::transcode(const char* const toTranscode)
{
    // Watch for a few pyscho corner cases
    if (!toTranscode)
        return 0;

    XMLCh* retVal = 0;
    if (!*toTranscode)
    {
        retVal = new XMLCh[1];
        retVal[0] = 0;
        return retVal;
    }

    //
    //  Get the length of the string to transcode. The Unicode string will
    //  almost always be no more chars than were in the source, so this is
    //  the best guess as to the storage needed.
    //
    const int32_t srcLen = (int32_t)strlen(toTranscode);

    // Now lock while we do these calculations
    UErrorCode err = U_ZERO_ERROR;
    {
        XMLMutexLock lockConverter(&fMutex);

        //
        //  Here we don't know what the target length will be so use 0 and
        //  expect an U_BUFFER_OVERFLOW_ERROR in which case it'd get resolved
        //  by the correct capacity value.
        //
        int32_t targetCap;
        targetCap = ucnv_toUChars
        (
            fConverter
            , 0
            , 0
            , toTranscode
            , srcLen
            , &err
        );

        if (err != U_BUFFER_OVERFLOW_ERROR)
            return 0;

        err = U_ZERO_ERROR;
        retVal = new XMLCh[targetCap];
        ucnv_toUChars
        (
            fConverter
            , retVal
            , targetCap
            , toTranscode
            , srcLen
            , &err
        );
    }

    if (U_FAILURE(err))
    {
        // Clean up if we got anything allocated
        delete [] retVal;
        return 0;
    }

    return retVal;
}


bool ICULCPTranscoder::transcode(const  char* const     toTranscode
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

    // Lock and do the transcode operation
    UErrorCode err = U_ZERO_ERROR;
    const int32_t srcLen = (int32_t)strlen(toTranscode);
    {
        XMLMutexLock lockConverter(&fMutex);
        ucnv_toUChars
        (
            fConverter
            , toFill
            , maxChars + 1
            , toTranscode
            , srcLen
            , &err
        );
    }

    if (U_FAILURE(err))
        return false;

    return true;
}


bool ICULCPTranscoder::transcode(   const   XMLCh* const    toTranscode
                                    ,       char* const     toFill
                                    , const unsigned int    maxChars)
{
    // Watch for a few psycho corner cases
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


    UErrorCode err = U_ZERO_ERROR;
    int32_t targetCap;
    {
        XMLMutexLock lockConverter(&fMutex);
        targetCap = ucnv_fromUChars
        (
            fConverter
            , toFill
            , maxChars + 1
            , toTranscode
            , &err
        );
    }

    if (U_FAILURE(err))
        return false;

    toFill[targetCap] = 0;
    return true;
}
