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
 * Revision 1.15  2000/03/02 19:55:34  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.14  2000/02/10 21:28:53  aruna1
 * Problem solved for converting UChar to XMLCh. This problem showed up on solaris as UChar was != XMLCh
 *
 * Revision 1.13  2000/02/06 07:48:32  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.12  2000/01/25 22:49:56  roddey
 * Moved the supportsSrcOfs() method from the individual transcoder to the
 * transcoding service, where it should have been to begin with.
 *
 * Revision 1.11  2000/01/25 19:19:07  roddey
 * Simple addition of a getId() method to the xcode and netacess abstractions to
 * allow each impl to give back an id string.
 *
 * Revision 1.10  2000/01/24 20:39:47  roddey
 * Fixed a bug introduced in the recent move to always have XMLCh be
 * the same as wchar_t.
 *
 * Revision 1.9  2000/01/21 23:59:06  roddey
 * Added code to deal with system configurations where XMLCh is not
 * the same size as ICU's UChar.
 *
 * Revision 1.8  2000/01/19 23:21:11  abagchi
 * Made this file compatible with ICU 1.4
 *
 * Revision 1.7  2000/01/19 00:58:07  roddey
 * Update to support new ICU 1.4 release.
 *
 * Revision 1.6  1999/12/18 00:22:32  roddey
 * Changes to support the new, completely orthagonal, transcoder architecture.
 *
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
#include <util/Janitor.hpp>
#include <util/TranscodingException.hpp>
#include <util/XMLString.hpp>
#include "ICUTransService.hpp"
#include <string.h>
#include <unicode/uloc.h>
#include <unicode/unicode.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>



// ---------------------------------------------------------------------------
//  Local, const data
// ---------------------------------------------------------------------------
static const XMLCh gMyServiceId[] =
{
    chLatin_I, chLatin_C, chLatin_U, chNull
};



// ---------------------------------------------------------------------------
//  Local functions
// ---------------------------------------------------------------------------

//
//  When XMLCh and ICU's UChar are not the same size, we have to do a temp
//  conversion of all strings. These local helper methods make that easier.
//
static UChar* convertToUChar(const XMLCh* const toConvert)
{
    UChar* tmpBuf = new UChar[XMLString::stringLen(toConvert) + 1];
    const XMLCh* srcPtr = toConvert;
    UChar* outPtr = tmpBuf;
    while (*srcPtr)
        *outPtr++ = UChar(*srcPtr++);
    *outPtr = 0;

    return tmpBuf;
}


static XMLCh* convertToXMLCh(const UChar* const toConvert)
{
    const unsigned int srcLen = u_strlen(toConvert);
    XMLCh* retBuf = new XMLCh[srcLen + 1];

    XMLCh* outPtr = retBuf;
    const UChar* srcPtr = toConvert;
    while (*srcPtr)
        *outPtr++ = XMLCh(*srcPtr++);
    *outPtr = 0;

    return retBuf;
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
        //
        //  If an inequality, then return the difference. Note that the XMLCh
        //  might be bigger physically than UChar, but it won't hold anything
        //  larger than 0xFFFF, so our cast here will work for both possible
        //  sizes of XMLCh.
        //
        if (Unicode::toUpperCase(UChar(*psz1)) != Unicode::toUpperCase(UChar(*psz2)))
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
        //
        //  If an inequality, then return the difference. Note that the XMLCh
        //  might be bigger physically than UChar, but it won't hold anything
        //  larger than 0xFFFF, so our cast here will work for both possible
        //  sizes of XMLCh.
        //
        if (Unicode::toUpperCase(UChar(*psz1)) != Unicode::toUpperCase(UChar(*psz2)))
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


const XMLCh* ICUTransService::getId() const
{
    return gMyServiceId;
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
    return (Unicode::isSpaceChar(UChar(toCheck)) != 0);
}


XMLLCPTranscoder* ICUTransService::makeNewLCPTranscoder()
{
    //
    //  Try to create a default converter. If it fails, return a null
    //  pointer which will basically cause the system to give up because
    //  we really can't do anything without one.
    //
    UErrorCode uerr = U_ZERO_ERROR;
    UConverter* converter = ucnv_open(NULL, &uerr);
    if (!converter)
        return 0;

    // That went ok, so create an ICU LCP transcoder wrapper and return it
    return new ICULCPTranscoder(converter);
}


bool ICUTransService::supportsSrcOfs() const
{
    // This implementation supports source offset information
    return true;
}


void ICUTransService::upperCase(XMLCh* const toUpperCase) const
{
    XMLCh* outPtr = toUpperCase;
    while (*outPtr)
    {
        *outPtr = XMLCh(Unicode::toUpperCase(UChar(*outPtr)));
        outPtr++;
    }
}



// ---------------------------------------------------------------------------
//  ICUTransService: The protected virtual transcoding service API
// ---------------------------------------------------------------------------
XMLTranscoder* ICUTransService::
makeNewXMLTranscoder(const  XMLCh* const            encodingName
                    ,       XMLTransService::Codes& resValue
                    , const unsigned int            blockSize)
{
    //
    //  If UChar and XMLCh are not the same size, then we have premassage the
    //  encoding name into a UChar type string.
    //
    const UChar* actualName;
    UChar* tmpName = 0;
    if (sizeof(UChar) == sizeof(XMLCh))
    {
        actualName = (const UChar*)encodingName;
    }
     else
    {
        tmpName = convertToUChar(encodingName);
        actualName = tmpName;
    }

    ArrayJanitor<UChar> janTmp(tmpName);

    UErrorCode uerr = U_ZERO_ERROR;
    UConverter* converter = ucnv_openU(actualName, &uerr);
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
        fSrcOffsets = new XMLUInt32[blockSize];

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
XMLCh ICUTranscoder::transcodeOne(  const   XMLByte* const  srcData
                                    , const unsigned int    srcBytes
                                    ,       unsigned int&   bytesEaten)
{
    // Check for stupid stuff
    if (!srcBytes)
        return 0;

    UErrorCode err = U_ZERO_ERROR;
    const XMLByte* startSrc = srcData;
    const UChar chRet = ucnv_getNextUChar
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
    return XMLCh(chRet);
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
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxChars);
    #endif

    // Set up pointers to the source buffers
    const XMLByte*  startSrc = srcData;
    const XMLByte*  endSrc = srcData + srcCount;

    //
    //  And now do the target buffer. This works differently according to
    //  whether XMLCh and UChar are the same size or not.
    //
    UChar* startTarget;
    if (sizeof(XMLCh) == sizeof(UChar))
        startTarget = (UChar*)toFill;
     else
        startTarget = new UChar[maxChars];
    UChar* orgTarget = startTarget;

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
        , startTarget + maxChars
        , (const char**)&startSrc
        , (const char*)endSrc
        , (fFixed ? 0 : (int32_t*)fSrcOffsets)
        , false
        , &err
    );

    if ((err != U_ZERO_ERROR) && (err != U_INDEX_OUTOFBOUNDS_ERROR))
    {
        if (orgTarget != (UChar*)toFill)
            delete [] orgTarget;
        ThrowXML(TranscodingException, XMLExcepts::Trans_CouldNotXCodeXMLData);
    }

    // Calculate the bytes eaten and store in caller's param
    bytesEaten = startSrc - srcData;

    // And the characters decoded
    const unsigned int charsDecoded = startTarget - orgTarget;

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

    //
    //  If XMLCh and UChar are not the same size, then we need to copy over
    //  the temp buffer to the new one.
    //
    if (sizeof(UChar) != sizeof(XMLCh))
    {
        XMLCh* outPtr = toFill;
        startTarget = orgTarget;
        for (unsigned int index = 0; index < charsDecoded; index++)
            *outPtr++ = XMLCh(*startTarget++);

        // And delete the temp buffer
        delete [] orgTarget;
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

    //
    //  We do two different versions of this, according to whether XMLCh
    //  is the same size as UChar or not.
    //
    UErrorCode err = U_ZERO_ERROR;
    int32_t targetCap;
    if (sizeof(XMLCh) == sizeof(UChar))
    {
        // Use a faux scope to synchronize while we do this
        {
            XMLMutexLock lockConverter(&fMutex);

            targetCap = ucnv_fromUChars
            (
                fConverter
                , 0
                , 0
                , (const UChar*)srcText
                , &err
            );
        }
    }
     else
    {
        // Copy the source to a local temp
        UChar* tmpBuf = convertToUChar(srcText);
        ArrayJanitor<UChar> janTmp(tmpBuf);

        // Use a faux scope to synchronize while we do this
        {
            XMLMutexLock lockConverter(&fMutex);

            targetCap = ucnv_fromUChars
            (
                fConverter
                , 0
                , 0
                , tmpBuf
                , &err
            );
        }
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

    // Use a faux scope to synchronize while we do this
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
        return retBuf;

    if (!*toTranscode)
    {
        retBuf = new char[1];
        retBuf[0] = 0;
        return retBuf;
    }

    //
    //  Get the length of the source string since we'll have to use it in
    //  a couple places below.
    //
    const unsigned int srcLen = XMLString::stringLen(toTranscode);

    //
    //  If XMLCh and UChar are not the same size, then we have to make a
    //  temp copy of the text to pass to ICU.
    //
    const UChar* actualSrc;
    UChar* ncActual = 0;
    if (sizeof(XMLCh) == sizeof(UChar))
    {
        actualSrc = (const UChar*)toTranscode;
    }
     else
    {
        // Allocate a non-const temp buf, but store it also in the actual
        ncActual = convertToUChar(toTranscode);
        actualSrc = ncActual;
    }

    // Insure that the temp buffer, if any, gets cleaned up via the nc pointer
    ArrayJanitor<UChar> janTmp(ncActual);

    // Caculate a return buffer size not too big, but less likely to overflow
    int32_t targetLen = (int32_t)(srcLen * 1.25);

    // Allocate the return buffer
    retBuf = new char[targetLen + 1];

    //
    //  Lock now while we call the converter. Use a faux block to dot he
    //  lock so that it unlocks immediately afterwards.
    UErrorCode err = U_ZERO_ERROR;
    int32_t targetCap;
    {
        XMLMutexLock lockConverter(&fMutex);

        targetCap = ucnv_fromUChars
        (
            fConverter
            , retBuf
            , targetLen + 1
            , actualSrc
            , &err
        );
    }

    // If targetLen is not enough then buffer overflow might occur
    if (err == U_BUFFER_OVERFLOW_ERROR)
    {
        //
        //  Reset the error, delete the old buffer, allocate a new one,
        //  and try again.
        //
        err = U_ZERO_ERROR;
        delete [] retBuf;
        retBuf = new char[targetCap + 1];

        // Lock again before we retry
        XMLMutexLock lockConverter(&fMutex);
        targetCap = ucnv_fromUChars
        (
            fConverter
            , retBuf
            , targetCap
            , actualSrc
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

    if (!*toTranscode)
    {
        XMLCh* retVal = new XMLCh[1];
        retVal[0] = 0;
        return retVal;
    }

    //
    //  Get the length of the string to transcode. The Unicode string will
    //  almost always be no more chars than were in the source, so this is
    //  the best guess as to the storage needed.
    //
    const int32_t srcLen = (int32_t)strlen(toTranscode);

    // We need a target buffer of UChars to fill in
    UChar* targetBuf = 0;

    // Now lock while we do these calculations
    UErrorCode err = U_ZERO_ERROR;
    int32_t targetCap;
    {
        XMLMutexLock lockConverter(&fMutex);

        //
        //  Here we don't know what the target length will be so use 0 and
        //  expect an U_BUFFER_OVERFLOW_ERROR in which case it'd get resolved
        //  by the correct capacity value.
        //
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
        targetBuf = new UChar[targetCap + 1];
        ucnv_toUChars
        (
            fConverter
            , targetBuf
            , targetCap
            , toTranscode
            , srcLen
            , &err
        );
    }

    if (U_FAILURE(err))
    {
        // Clean up if we got anything allocated
        delete [] targetBuf;
        return 0;
    }

    // Cap it off to make sure
    targetBuf[targetCap] = 0;

    //
    //  If XMLCh and UChar are the same size, then we can return retVal
    //  as is. Else, we have to allocate another buffer and copy the data
    //  over to it.
    //
    XMLCh* actualRet;
    if (sizeof(XMLCh) == sizeof(UChar))
    {
        actualRet = (XMLCh*)targetBuf;
    }
     else
    {
        actualRet = convertToXMLCh(targetBuf);
        delete [] targetBuf;
    }
    return actualRet;
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

    // We'll need this in a couple of places below
    const unsigned int srcLen = strlen(toTranscode);

    //
    //  Set up the target buffer. If XMLCh and UChar are not the same size
    //  then we have to use a temp buffer and convert over.
    //
    UChar* targetBuf;
    if (sizeof(XMLCh) == sizeof(UChar))
        targetBuf = (UChar*)toFill;
    else
        targetBuf = new UChar[maxChars + 1];

    UErrorCode err = U_ZERO_ERROR;
    {
        XMLMutexLock lockConverter(&fMutex);
        ucnv_toUChars
        (
            fConverter
            , targetBuf
            , maxChars + 1
            , toTranscode
            , srcLen
            , &err
        );
    }

    if (U_FAILURE(err))
    {
        if (targetBuf != (UChar*)toFill)
            delete [] targetBuf;
        return false;
    }

    // If the sizes are not the same, then copy the data over
    if (sizeof(XMLCh) != sizeof(UChar))
    {
        UChar* srcPtr = targetBuf;
        XMLCh* outPtr = toFill;
        while (*srcPtr)
            *outPtr++ = XMLCh(*srcPtr++);
        *outPtr = 0;

        // And delete the temp buffer
        delete [] targetBuf;
    }

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

    //
    //  If XMLCh and UChar are not the same size, then we have to make a
    //  temp copy of the text to pass to ICU.
    //
    const UChar* actualSrc;
    UChar* ncActual = 0;
    if (sizeof(XMLCh) == sizeof(UChar))
    {
        actualSrc = (const UChar*)toTranscode;
    }
     else
    {
        // Allocate a non-const temp buf, but store it also in the actual
        ncActual = convertToUChar(toTranscode);
        actualSrc = ncActual;
    }

    // Insure that the temp buffer, if any, gets cleaned up via the nc pointer
    ArrayJanitor<UChar> janTmp(ncActual);

    UErrorCode err = U_ZERO_ERROR;
    int32_t targetCap;
    {
        XMLMutexLock lockConverter(&fMutex);
        targetCap = ucnv_fromUChars
        (
            fConverter
            , toFill
            , maxChars + 1
            , actualSrc
            , &err
        );
    }

    if (U_FAILURE(err))
        return false;

    toFill[targetCap] = 0;
    return true;
}
