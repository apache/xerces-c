/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights
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
 * $Id$
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XMLUTF8Transcoder.hpp>
#include <xercesc/util/UTFDataFormatException.hpp>


XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Local static data
//
//  gUTFBytes
//      A list of counts of trailing bytes for each initial byte in the input.
//
//  gUTFByteIndicator
//      For a UTF8 sequence of n bytes, n>=2, the first byte of the
//      sequence must contain n 1's followed by precisely 1 0 with the
//      rest of the byte containing arbitrary bits.  This array stores
//      the required bit pattern for validity checking.
//  gUTFByteIndicatorTest
//      When bitwise and'd with the observed value, if the observed
//      value is correct then a result matching gUTFByteIndicator will
//      be produced.
//
//  gUTFOffsets
//      A list of values to offset each result char type, according to how
//      many source bytes when into making it.
//
//  gFirstByteMark
//      A list of values to mask onto the first byte of an encoded sequence,
//      indexed by the number of bytes used to create the sequence.
// ---------------------------------------------------------------------------
static const XMLByte gUTFBytes[256] =
{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    ,   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    ,   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
    ,   3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
};

static const XMLByte gUTFByteIndicator[6] =
{
    0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
};
static const XMLByte gUTFByteIndicatorTest[6] =
{
    0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE
};

static const XMLUInt32 gUTFOffsets[6] =
{
    0, 0x3080, 0xE2080, 0x3C82080, 0xFA082080, 0x82082080
};

static const XMLByte gFirstByteMark[7] =
{
    0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
};



// ---------------------------------------------------------------------------
//  XMLUTF8Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLUTF8Transcoder::XMLUTF8Transcoder(const  XMLCh* const    encodingName
                                    , const unsigned int    blockSize
                                    , MemoryManager* const  manager) :

    XMLTranscoder(encodingName, blockSize, manager)
{
}

XMLUTF8Transcoder::~XMLUTF8Transcoder()
{
}


// ---------------------------------------------------------------------------
//  XMLUTF8Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
unsigned int
XMLUTF8Transcoder::transcodeFrom(const  XMLByte* const          srcData
                                , const unsigned int            srcCount
                                ,       XMLCh* const            toFill
                                , const unsigned int            maxChars
                                ,       unsigned int&           bytesEaten
                                ,       unsigned char* const    charSizes)
{
    // Watch for pathological scenario. Shouldn't happen, but...
    if (!srcCount || !maxChars)
        return 0;

    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxChars);
    #endif

    //
    //  Get pointers to our start and end points of the input and output
    //  buffers.
    //
    const XMLByte*  srcPtr = srcData;
    const XMLByte*  srcEnd = srcPtr + srcCount;
    XMLCh*          outPtr = toFill;
    XMLCh*          outEnd = outPtr + maxChars;
    unsigned char*  sizePtr = charSizes;



    //
    //  We now loop until we either run out of input data, or room to store
    //  output chars.
    //
    while ((srcPtr < srcEnd) && (outPtr < outEnd))
    {
        // Special-case ASCII, which is a leading byte value of <= 127
        if (*srcPtr <= 127)
        {
            *outPtr++ = XMLCh(*srcPtr++);
            *sizePtr++ = 1;
            continue;
        }

        // See how many trailing src bytes this sequence is going to require
        const unsigned int trailingBytes = gUTFBytes[*srcPtr];

        //
        //  If there are not enough source bytes to do this one, then we
        //  are done. Note that we done >= here because we are implicitly
        //  counting the 1 byte we get no matter what.
        //
        //  If we break out here, then there is nothing to undo since we
        //  haven't updated any pointers yet.
        //
        if (srcPtr + trailingBytes >= srcEnd)
            break;

        // Looks ok, so lets build up the value
        // or at least let's try to do so--remembering that
        // we cannot assume the encoding to be valid:

        // first, test first byte
        if((gUTFByteIndicatorTest[trailingBytes] & *srcPtr) != gUTFByteIndicator[trailingBytes]) {
            char pos[2] = {(char)0x31, 0}; 
            char len[2] = {(char)(trailingBytes+0x31), 0};
            char byte[2] = {*srcPtr,0};
            ThrowXMLwithMemMgr3(UTFDataFormatException, XMLExcepts::UTF8_FormatError, pos, byte, len, getMemoryManager());
        }

        XMLUInt32 tmpVal = *srcPtr++;
        tmpVal <<= 6;
        for(unsigned int i=1; i<trailingBytes; i++) 
        {
            if((*srcPtr & 0xC0) == 0x80) 
            {
                tmpVal += *srcPtr++; 
                tmpVal <<= 6;
            } 
            else
            {
                char len[2] = {(char)(trailingBytes+0x31), 0};
                char pos[2]= {(char)(i+0x31), 0};
                char byte[2] = {*srcPtr,0};
                ThrowXMLwithMemMgr3(UTFDataFormatException, XMLExcepts::UTF8_FormatError, pos, byte, len, getMemoryManager());
            }
        }
        if((*srcPtr & 0xC0) == 0x80) 
        {
            tmpVal += *srcPtr++;
        }
        else 
        {
            char len[2] = {(char)(trailingBytes+0x31), 0};
            char byte[2] = {*srcPtr,0};
            ThrowXMLwithMemMgr3(UTFDataFormatException, XMLExcepts::UTF8_FormatError, len, byte, len, getMemoryManager());
        }
        // since trailingBytes comes from an array, this logic is redundant
        //  default :
        //      ThrowXMLwithMemMgr(TranscodingException, XMLExcepts::Trans_BadSrcSeq);
        //}
        tmpVal -= gUTFOffsets[trailingBytes];

        //
        //  If it will fit into a single char, then put it in. Otherwise
        //  encode it as a surrogate pair. If its not valid, use the
        //  replacement char.
        //
        if (!(tmpVal & 0xFFFF0000))
        {
            *sizePtr++ = trailingBytes + 1;
            *outPtr++ = XMLCh(tmpVal);
        }
         else if (tmpVal > 0x10FFFF)
        {
            //
            //  If we've gotten more than 32 chars so far, then just break
            //  out for now and lets process those. When we come back in
            //  here again, we'll get no chars and throw an exception. This
            //  way, the error will have a line and col number closer to
            //  the real problem area.
            //
            if ((outPtr - toFill) > 32)
                break;

            ThrowXMLwithMemMgr(TranscodingException, XMLExcepts::Trans_BadSrcSeq, getMemoryManager());
        }
         else
        {
            //
            //  If we have enough room to store the leading and trailing
            //  chars, then lets do it. Else, pretend this one never
            //  happened, and leave it for the next time. Since we don't
            //  update the bytes read until the bottom of the loop, by
            //  breaking out here its like it never happened.
            //
            if (outPtr + 1 >= outEnd)
                break;

            // Store the leading surrogate char
            tmpVal -= 0x10000;
            *sizePtr++ = trailingBytes + 1;
            *outPtr++ = XMLCh((tmpVal >> 10) + 0xD800);

            //
            //  And then the trailing char. This one accounts for no
            //  bytes eaten from the source, so set the char size for this
            //  one to be zero.
            //
            *sizePtr++ = 0;
            *outPtr++ = XMLCh((tmpVal & 0x3FF) + 0xDC00);
        }
    }

    // Update the bytes eaten
    bytesEaten = srcPtr - srcData;

    // Return the characters read
    return outPtr - toFill;
}


unsigned int
XMLUTF8Transcoder::transcodeTo( const   XMLCh* const    srcData
                                , const unsigned int    srcCount
                                ,       XMLByte* const  toFill
                                , const unsigned int    maxBytes
                                ,       unsigned int&   charsEaten
                                , const UnRepOpts       options)
{
    // Watch for pathological scenario. Shouldn't happen, but...
    if (!srcCount || !maxBytes)
        return 0;

    //
    //  Get pointers to our start and end points of the input and output
    //  buffers.
    //
    const XMLCh*    srcPtr = srcData;
    const XMLCh*    srcEnd = srcPtr + srcCount;
    XMLByte*        outPtr = toFill;
    XMLByte*        outEnd = toFill + maxBytes;

    while (srcPtr < srcEnd)
    {
        //
        //  Tentatively get the next char out. We have to get it into a
        //  32 bit value, because it could be a surrogate pair.
        //
        XMLUInt32 curVal = *srcPtr;

        //
        //  If its a leading surrogate, then lets see if we have the trailing
        //  available. If not, then give up now and leave it for next time.
        //
        unsigned int srcUsed = 1;
        if ((curVal >= 0xD800) && (curVal <= 0xDBFF))
        {
            if (srcPtr + 1 >= srcEnd)
                break;

            // Create the composite surrogate pair
            curVal = ((curVal - 0xD800) << 10)
                    + ((*(srcPtr + 1) - 0xDC00) + 0x10000);

            // And indicate that we ate another one
            srcUsed++;
        }

        // Figure out how many bytes we need
        unsigned int encodedBytes;
        if (curVal < 0x80)
            encodedBytes = 1;
        else if (curVal < 0x800)
            encodedBytes = 2;
        else if (curVal < 0x10000)
            encodedBytes = 3;
        else if (curVal < 0x200000)
            encodedBytes = 4;
        else if (curVal < 0x4000000)
            encodedBytes = 5;
        else if (curVal <= 0x7FFFFFFF)
            encodedBytes = 6;
        else
        {
            // If the options say to throw, then throw
            if (options == UnRep_Throw)
            {
                XMLCh tmpBuf[17];
                XMLString::binToText(curVal, tmpBuf, 16, 16, getMemoryManager());
                ThrowXMLwithMemMgr2
                (
                    TranscodingException
                    , XMLExcepts::Trans_Unrepresentable
                    , tmpBuf
                    , getEncodingName()
                    , getMemoryManager()
                );
            }

            // Else, use the replacement character
            *outPtr++ = chSpace;
            srcPtr += srcUsed;
            continue;
        }

        //
        //  If we cannot fully get this char into the output buffer,
        //  then leave it for the next time.
        //
        if (outPtr + encodedBytes > outEnd)
            break;

        // We can do it, so update the source index
        srcPtr += srcUsed;

        //
        //  And spit out the bytes. We spit them out in reverse order
        //  here, so bump up the output pointer and work down as we go.
        //
        outPtr += encodedBytes;
        switch(encodedBytes)
        {
            case 6 : *--outPtr = XMLByte((curVal | 0x80UL) & 0xBFUL);
                     curVal >>= 6;
            case 5 : *--outPtr = XMLByte((curVal | 0x80UL) & 0xBFUL);
                     curVal >>= 6;
            case 4 : *--outPtr = XMLByte((curVal | 0x80UL) & 0xBFUL);
                     curVal >>= 6;
            case 3 : *--outPtr = XMLByte((curVal | 0x80UL) & 0xBFUL);
                     curVal >>= 6;
            case 2 : *--outPtr = XMLByte((curVal | 0x80UL) & 0xBFUL);
                     curVal >>= 6;
            case 1 : *--outPtr = XMLByte
                     (
                        curVal | gFirstByteMark[encodedBytes]
                     );
        }

        // Add the encoded bytes back in again to indicate we've eaten them
        outPtr += encodedBytes;
    }

    // Fill in the chars we ate
    charsEaten = (srcPtr - srcData);

    // And return the bytes we filled in
    return (outPtr - toFill);
}


bool XMLUTF8Transcoder::canTranscodeTo(const unsigned int toCheck) const
{
    // We can represent anything in the Unicode (with surrogates) range
    return (toCheck <= 0x10FFFF);
}

XERCES_CPP_NAMESPACE_END

