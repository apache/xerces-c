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


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XMLUTF8Transcoder.hpp>
#include <util/UTFDataFormatException.hpp>


// ---------------------------------------------------------------------------
//  Local static data
//
//  gUTFBytes
//      A list of counts of trailing bytes for each initial byte in the input.
//
//  gUTFOffsets
//      A list of values to offset each result char type, according to how
//      many source bytes when into making it.
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

static const XMLUInt32 gUTFOffsets[6] =
{
    0, 0x3080, 0xE2080, 0x3C82080, 0xFA082080, 0x82022080
};




// ---------------------------------------------------------------------------
//  XMLUTF8Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLUTF8Transcoder::XMLUTF8Transcoder(const  XMLCh* const    encodingName
                                    , const unsigned int    blockSize) :

    XMLTranscoder(encodingName, blockSize)
    , fSpareCh(0)
{
}

XMLUTF8Transcoder::~XMLUTF8Transcoder()
{
}


// ---------------------------------------------------------------------------
//  XMLUTF8Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
bool XMLUTF8Transcoder::supportsSrcOfs() const
{
    // Yes we support this
    return true;
}


XMLCh
XMLUTF8Transcoder::transcodeOne(const   XMLByte* const  srcData
                                , const unsigned int    srcBytes
                                ,       unsigned int&   bytesEaten)
{
    // If there is a spare char, then do that first
    if (fSpareCh)
    {
        const XMLCh retCh = fSpareCh;
        fSpareCh = 0;
        return retCh;
    }

    // If there are no bytes, then give up now
    if (!srcBytes)
        return 0;

    // Just call the other version with a max of one output character
    XMLCh chTarget;
    unsigned char dummy;
    if (!transcodeXML(srcData, srcBytes, &chTarget, 1, bytesEaten, &dummy))
        return 0;

    return chTarget;
}


unsigned int
XMLUTF8Transcoder::transcodeXML(const   XMLByte* const          srcData
                                , const unsigned int            srcCount
                                ,       XMLCh* const            toFill
                                , const unsigned int            maxChars
                                ,       unsigned int&           bytesEaten
                                ,       unsigned char* const    charSizes)
{
    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxChars);
    #endif

    // This will track the target chars we've transcoded out so far
    unsigned int charsRead = 0;

    //
    //  If we have a spare, then store it first and zero out the spare.
    //  Be sure to bump the count of chars read.
    //
    if (fSpareCh)
    {
        //
        //  A spare is a trailing surrogate. So it actually takes up no
        //  space in the source data. Its leading surrogate accounted for
        //  all the bytes eaten.
        //
        charSizes[charsRead] = 0;
        toFill[charsRead++] = fSpareCh;
        fSpareCh = 0;
    }

    //
    //  Just loop until we run out of input or hit the max chars that
    //  was requested.
    //
    bytesEaten = 0;
    const XMLByte* srcPtr = srcData;
    while (charsRead < maxChars)
    {
        // See how many trailing src bytes this sequence is going to require
        const unsigned int trailingBytes = gUTFBytes[*srcPtr];

        //
        //  If there are not enough source bytes to do this one, then we
        //  are done.
        //
        if (bytesEaten + trailingBytes >= srcCount)
            break;

        // Looks ok, so lets build up the value
        XMLUInt32 tmpVal = 0;
        switch(trailingBytes)
        {
            case 5 : tmpVal += *srcPtr++; tmpVal <<= 6;
            case 4 : tmpVal += *srcPtr++; tmpVal <<= 6;
            case 3 : tmpVal += *srcPtr++; tmpVal <<= 6;
            case 2 : tmpVal += *srcPtr++; tmpVal <<= 6;
            case 1 : tmpVal += *srcPtr++; tmpVal <<= 6;
            case 0 : tmpVal += *srcPtr++;
        }
        tmpVal -= gUTFOffsets[trailingBytes];

        //
        //  If it will fit into a single char, then put it in. Otherwise
        //  encode it as a surrogate pair. If its not valid, use the
        //  replacement char.
        //
        if (!(tmpVal & 0xFFFF0000))
        {
            charSizes[charsRead] = trailingBytes + 1;
            toFill[charsRead++] = XMLCh(tmpVal);
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
            if (charsRead > 32)
                break;
            ThrowXML(UTFDataFormatException, XMLExcepts::Reader_BadUTF8Seq);
        }
         else
        {
            // Store the leading surrogate char
            tmpVal -= 0x10000;
            charSizes[charsRead] = trailingBytes + 1;
            toFill[charsRead++] = XMLCh((tmpVal >> 10) + 0xD800);

            //
            //  If we don't have room for the trailing one, then store
            //  it in the spare char. Else store it in the buffer.
            //
            if (charsRead >= maxChars)
            {
                fSpareCh = XMLCh(tmpVal & 0x3FF) + 0xDC00;
            }
             else 
            {
                // This one accounts for no bytes eaten
                charSizes[charsRead] = 0;
                toFill[charsRead++] = XMLCh(tmpVal & 0x3FF) + 0xDC00;
            }
        }

        // Update the bytes eaten
        bytesEaten += trailingBytes + 1;
    }

    // Return the characters read
    return charsRead;
}
