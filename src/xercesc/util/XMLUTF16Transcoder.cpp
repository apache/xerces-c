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
#include <xercesc/util/BitOps.hpp>
#include <xercesc/util/XMLUTF16Transcoder.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN



// ---------------------------------------------------------------------------
//  XMLUTF16Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLUTF16Transcoder::XMLUTF16Transcoder( const   XMLCh* const    encodingName
                                        , const unsigned int    blockSize
                                        , const bool            swapped
                                        , MemoryManager* const manager) :

    XMLTranscoder(encodingName, blockSize, manager)
    , fSwapped(swapped)
{
}


XMLUTF16Transcoder::~XMLUTF16Transcoder()
{
}


// ---------------------------------------------------------------------------
//  XMLUTF16Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
unsigned int
XMLUTF16Transcoder::transcodeFrom(  const   XMLByte* const       srcData
                                    , const unsigned int         srcCount
                                    ,       XMLCh* const         toFill
                                    , const unsigned int         maxChars
                                    ,       unsigned int&        bytesEaten
                                    ,       unsigned char* const charSizes)
{
    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxChars);
    #endif

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  max output chars and the number of chars in the source.
    //
    const unsigned int srcChars = srcCount / sizeof(UTF16Ch);
    const unsigned int countToDo = srcChars < maxChars ? srcChars : maxChars;

    // Look at the source data as UTF16 chars
    const UTF16Ch* asUTF16 = (const UTF16Ch*)srcData;

    // And get a mutable pointer to the output
    XMLCh* outPtr = toFill;

    //
    //  If its swapped, we have to do a char by char swap and cast. Else
    //  we have to check whether our XMLCh and UTF16Ch types are the same
    //  size or not. If so, we can optimize by just doing a buffer copy.
    //
    if (fSwapped)
    {
        //
        //  And then do the swapping loop for the count we precalculated. Note
        //  that this also handles size conversion as well if XMLCh is not the
        //  same size as UTF16Ch.
        //
        for (unsigned int index = 0; index < countToDo; index++)
            *outPtr++ = BitOps::swapBytes(*asUTF16++);
    }
     else
    {
        //
        //  If the XMLCh type is the same size as a UTF16 value on this
        //  platform, then we can do just a buffer copy straight to the target
        //  buffer since our source chars are UTF-16 chars. If its not, then
        //  we still have to do a loop and assign each one, in order to
        //  implicitly convert.
        //
        if (sizeof(XMLCh) == sizeof(UTF16Ch))
        {
            //  Notice we convert char count to byte count here!!!
            memcpy(toFill, srcData, countToDo * sizeof(UTF16Ch));
        }
         else
        {
            for (unsigned int index = 0; index < countToDo; index++)
                *outPtr++ = XMLCh(*asUTF16++);
        }
    }

    // Set the bytes eaten
    bytesEaten = countToDo * sizeof(UTF16Ch);

    // Set the character sizes to the fixed size
    memset(charSizes, sizeof(UTF16Ch), countToDo);

    // Return the chars we transcoded
    return countToDo;
}


unsigned int
XMLUTF16Transcoder::transcodeTo(const   XMLCh* const    srcData
                                , const unsigned int    srcCount
                                ,       XMLByte* const  toFill
                                , const unsigned int    maxBytes
                                ,       unsigned int&   charsEaten
                                , const UnRepOpts)
{
    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxBytes);
    #endif

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  chars that we can fit into the output buffer, and the source
    //  chars available.
    //
    const unsigned int maxOutChars = maxBytes / sizeof(UTF16Ch);
    const unsigned int countToDo = srcCount < maxOutChars ? srcCount : maxOutChars;

    //
    //  Get a pointer tot he output buffer in the UTF-16 character format
    //  that we need to work with. And get a mutable pointer to the source
    //  character buffer.
    //
    UTF16Ch*        outPtr = (UTF16Ch*)toFill;
    const XMLCh*    srcPtr = srcData;

    //
    //  If the target format is swapped from our native format, then handle
    //  it one way, else handle it another.
    //
    if (fSwapped)
    {
        //
        //  And then do the swapping loop for the count we precalculated. Note
        //  that this also handles size conversion as well if XMLCh is not the
        //  same size as UTF16Ch.
        //
        for (unsigned int index = 0; index < countToDo; index++)
        {
            // To avoid flakey compilers, use a temp
            const UTF16Ch tmpCh = UTF16Ch(*srcPtr++);
            *outPtr++ = BitOps::swapBytes(tmpCh);
        }
    }
     else
    {
        //
        //  If XMLCh and UTF16Ch are the same size, we can just do a fast
        //  memory copy. Otherwise, we have to do a loop and downcast each
        //  character into its new 16 bit storage.
        //
        if (sizeof(XMLCh) == sizeof(UTF16Ch))
        {
            //  Notice we convert char count to byte count here!!!
            memcpy(toFill, srcData, countToDo * sizeof(UTF16Ch));
        }
         else
        {
            for (unsigned int index = 0; index < countToDo; index++)
                *outPtr++ = UTF16Ch(*srcPtr++);
        }
    }

    // Set the chars eaten to the calculated number we ate
    charsEaten = countToDo;

    //Return the bytes we ate. Note we convert to a byte count here!
    return countToDo * sizeof(UTF16Ch);
}


bool XMLUTF16Transcoder::canTranscodeTo(const unsigned int) const
{
    // We can handle anything
    return true;
}

XERCES_CPP_NAMESPACE_END

