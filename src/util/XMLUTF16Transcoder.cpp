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
#include <util/XMLUTF16Transcoder.hpp>
#include <util/TranscodingException.hpp>
#include <memory.h>



// ---------------------------------------------------------------------------
//  XMLUTF16Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLUTF16Transcoder::XMLUTF16Transcoder( const   XMLCh* const    encodingName
                                        , const unsigned int    blockSize
                                        , const bool            swapped) :

    XMLTranscoder(encodingName, blockSize)
    , fSwapped(swapped)
{
}


XMLUTF16Transcoder::~XMLUTF16Transcoder()
{
}


// ---------------------------------------------------------------------------
//  XMLUTF16Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
bool XMLUTF16Transcoder::supportsSrcOfs() const
{
    // Yes we support this
    return true;
}


XMLCh
XMLUTF16Transcoder::transcodeOne(const  XMLByte* const  srcData
                                , const unsigned int    srcBytes
                                ,       unsigned int&   bytesEaten)
{
    // If not enough source bytes, return zero
    if (srcBytes < sizeof(UTF16Ch))
        return 0;

    // We are going to eat one UTF16 char's worth of bytes
    bytesEaten = sizeof(UTF16Ch);

    // Get the byte out and swap it if needed
    UTF16Ch nextCh = *((const UTF16Ch*)srcData);
    if (fSwapped)
        nextCh = BitOps::swapBytes(nextCh);

    // And return the value, cast to an XMLCh
    return XMLCh(nextCh);
}


unsigned int
XMLUTF16Transcoder::transcodeXML(const  XMLByte* const          srcData
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

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  max output chars and the number of chars in the source.
    //
    const unsigned int srcChars = srcCount / sizeof(UTF16Ch);
    const unsigned int countToDo = srcChars < maxChars ? srcChars : maxChars;

    // Look at the source data as UTF16 chars
    const UTF16Ch* asUTF16 = (const UTF16Ch*)srcData;

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
        //  same size as UTF16Char.
        //
        for (unsigned int index = 0; index < countToDo; index++)
            toFill[index] = BitOps::swapBytes(*asUTF16++);
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
                toFill[index] = XMLCh(*asUTF16++);
        }
    }

    // Set the bytes eaten
    bytesEaten = countToDo * sizeof(UTF16Ch);

    // Set the character sizes to the fixed size
    memset(charSizes, sizeof(UTF16Ch), countToDo);

    // Return the chars we transcoded
    return countToDo;
}
