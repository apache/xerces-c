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


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <util/XML88591Transcoder.hpp>
#include <util/TranscodingException.hpp>
#include <memory.h>



// ---------------------------------------------------------------------------
//  XML88591Transcoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XML88591Transcoder::XML88591Transcoder( const   XMLCh* const    encodingName
                                        , const unsigned int    blockSize) :

    XMLTranscoder(encodingName, blockSize)
{
}


XML88591Transcoder::~XML88591Transcoder()
{
}


// ---------------------------------------------------------------------------
//  XML88591Transcoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
bool XML88591Transcoder::supportsSrcOfs() const
{
    // Yes we support this
    return true;
}


XMLCh
XML88591Transcoder::transcodeOne(const  XMLByte* const  srcData
                                , const unsigned int    srcBytes
                                ,       unsigned int&   bytesEaten)
{
    // If not enough source bytes, return zero
    if (!srcBytes)
        return 0;

    // We are going to eat one byte
    bytesEaten = 1;

    // And return the value, cast to an XMLCh
    return XMLCh(*srcData);
}


unsigned int
XML88591Transcoder::transcodeXML(const  XMLByte* const          srcData
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

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  max output chars and the number of chars in the source.
    //
    const unsigned int countToDo = srcCount < maxChars ? srcCount : maxChars;

    //
    //  Loop through the bytes to do and convert over each byte. Its just
    //  a cast to the wide char type.
    //
    const XMLByte*  srcPtr = srcData;
    XMLCh*          destPtr = toFill;
    const XMLByte*  srcEnd = srcPtr + countToDo;
    while (srcPtr < srcEnd)
        *destPtr++ = XMLCh(*srcPtr++);

    // Set the bytes eaten, and set the char size array to the fixed size
    bytesEaten = countToDo;
    memset(charSizes, 1, countToDo);

    // Return the chars we transcoded
    return countToDo;
}
