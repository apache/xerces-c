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
#include <util/BitOps.hpp>
#include <util/XMLUCS4Transcoder.hpp>
#include <util/TranscodingException.hpp>
#include <memory.h>



// ---------------------------------------------------------------------------
//  XMLUCS4Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLUCS4Transcoder::XMLUCS4Transcoder(const  XMLCh* const    encodingName
                                    , const unsigned int    blockSize
                                    , const bool            swapped) :

    XMLTranscoder(encodingName, blockSize)
    , fSpareCh(0)
    , fSwapped(swapped)
{
}


XMLUCS4Transcoder::~XMLUCS4Transcoder()
{
}


// ---------------------------------------------------------------------------
//  XMLUCS4Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
bool XMLUCS4Transcoder::supportsSrcOfs() const
{
    // Yes we support this
    return true;
}


XMLCh
XMLUCS4Transcoder::transcodeOne(const   XMLByte* const  srcData
                                , const unsigned int    srcBytes
                                ,       unsigned int&   bytesEaten)
{
    // If there is a spare char, return it first
    if (fSpareCh)
    {
        const XMLCh retCh = fSpareCh;
        fSpareCh = 0;
        return retCh;
    }

    // If not enough bytes for an input char, return zero
    if (srcBytes < sizeof(UCS4Ch))
        return 0;

    //
    //  Get the next char. If the source is swapped from native format,
    //  unswap it
    //
    UCS4Ch nextCh = *((const UCS4Ch*)srcData);
    if (fSwapped)
        nextCh = BitOps::swapBytes(nextCh);

    //
    //  See if this requires a surrogate pair to store. If so, then we
    //  return the first and save the next. Else just return the one
    //  resulting char.
    //
    XMLCh retCh;
    if (nextCh & 0xFFFF0000)
    {
        const XMLCh ch1 = XMLCh(((nextCh - 0x10000) >> 10) + 0xD800);
        const XMLCh ch2 = XMLCh(((nextCh - 0x10000) & 0x3FF) + 0xDC00);

        retCh = ch1;
        fSpareCh = ch2;
    }
     else
    {
        // No surrogate, so just cast it to the correct type
        retCh = XMLCh(nextCh);
    }

    // We ate one UCS4 char's worth of bytes
    bytesEaten = sizeof(UCS4Ch);
    return retCh;
}


unsigned int
XMLUCS4Transcoder::transcodeXML(const   XMLByte* const          srcData
                                , const unsigned int            srcCount
                                ,       XMLCh* const            toFill
                                , const unsigned int            maxChars
                                ,       unsigned int&           bytesEaten
                                ,       unsigned char* const    charSizes)
{
    // If debugging, make sure that the block size is legal
    #if defined(XML4C_DEBUG)
    checkBlockSize(maxChars);
    #endif

    // This will be the index into the output buffer and init the bytes eaten
    unsigned int charsRead = 0;
    bytesEaten = 0;

    //
    //  If there is a spare character, then we have to take that one first
    //  being sure to bump the chars read.
    //
    if (fSpareCh)
    {
        //
        //  This char was a trailing surrogate so it took no space from
        //  the original source itself. The leading surrogate accounted
        //  for all the source eaten.
        //
        charSizes[charsRead] = 0;
        toFill[charsRead++] = fSpareCh;
        fSpareCh = chNull;
    }

    //
    //  Calculate how many UCS-4 characters could possibly be pulled out
    //  of the raw buffer right now. If none, return zero. Shouldn't happen
    //  unless an odd number of bytes is in the buffer, since an empty
    //  buffer would have been refilled before we were called.
    //
    const unsigned int charsAvail = (srcCount / sizeof(UCS4Ch));
    if (!charsAvail)
        return 0;

    //
    //  Calculate the maximum chars we can do. Its the lesser of the chars
    //  requested and the UCS-4 chars available in the buffer. We have to
    //  account for the possibility that we used one target position on
    //  the trailing surrogate above.
    //
    const unsigned int charsToDo = ((maxChars - charsRead) < charsAvail) ?
                                    (maxChars - charsRead): charsAvail;

    // Just loop until we get the max chars we need or run out of source
    const UCS4Ch* asUCS4 = (const UCS4Ch*)srcData;
    while (charsRead < charsToDo)
    {
        // Get the next int out of the buffer
        UCS4Ch nextVal = *asUCS4++;
        bytesEaten += sizeof(UCS4Ch);

        // If it needs to be swapped, then do it
        if (fSwapped)
            nextVal = BitOps::swapBytes(nextVal);

        // Handle a surrogate pair if needed
        if (nextVal & 0xFFFF0000)
        {
            const XMLCh ch1 = XMLCh(((nextVal - 0x10000) >> 10) + 0xD800);
            const XMLCh ch2 = XMLCh(((nextVal - 0x10000) & 0x3FF) + 0xDC00);

            //
            //  If we have room for two chars, then put them both in and bump
            //  the chars read by two. Otherwise, put one in and store the
            //  other in the spare char for the next round.
            //
            if (charsRead + 1 == maxChars)
            {
                charSizes[charsRead] = sizeof(UCS4Ch);
                toFill[charsRead++] = ch1;
                fSpareCh = ch2;
            }
             else
            {
                //
                //  We have room so store them both. But note that the
                //  second one took up no source bytes!
                //
                charSizes[charsRead] = sizeof(UCS4Ch);
                toFill[charsRead++] = ch1;
                charSizes[charsRead] = 0;
                toFill[charsRead++] = ch2;
            }
        }
         else
        {
            // No surrogate, so just store it and bump the count
            charSizes[charsRead] = sizeof(UCS4Ch);
            toFill[charsRead++] = XMLCh(nextVal);
        }
    }
    return charsRead;
}
