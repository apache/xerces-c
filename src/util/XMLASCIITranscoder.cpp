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
#include <util/XMLASCIITranscoder.hpp>
#include <util/XMLString.hpp>
#include <util/TranscodingException.hpp>
#include <memory.h>



// ---------------------------------------------------------------------------
//  XMLASCIITranscoder: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLASCIITranscoder::XMLASCIITranscoder( const   XMLCh* const    encodingName
                                        , const unsigned int    blockSize) :

    XMLTranscoder(encodingName, blockSize)
{
}


XMLASCIITranscoder::~XMLASCIITranscoder()
{
}


// ---------------------------------------------------------------------------
//  XMLASCIITranscoder: Implementation of the transcoder API
// ---------------------------------------------------------------------------
bool XMLASCIITranscoder::supportsSrcOfs() const
{
    // Yes we support this
    return true;
}


XMLCh
XMLASCIITranscoder::transcodeOne(const  XMLByte* const  srcData
                                , const unsigned int    srcBytes
                                ,       unsigned int&   bytesEaten)
{
    // If no source, then give up
    if (!srcBytes)
        return 0;

    // If the source char is invalid, then give up
    if (*srcData > 0x7F)
    {
        ThrowXML1
        (
            TranscodingException
            , XML4CExcepts::Trans_NotInSourceSet
            , XMLUni::fgUSASCIIEncodingString
        );
    }

    // We eat one source byte and just cast the ASCII char to XMLCh
    bytesEaten = 1;
    return XMLCh(*srcData);
}


unsigned int
XMLASCIITranscoder::transcodeXML(const  XMLByte* const          srcData
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
    //  max output chars and the source count.
    //
    const unsigned int countToDo = srcCount < maxChars ? srcCount : maxChars;

    //
    //  Now loop through that many source chars and just cast each one
    //  over to the XMLCh format. Check each source that its really a
    //  valid ASCI char.
    //
    const XMLByte*  inPtr = srcData;
    XMLCh*          outPtr = toFill;
    unsigned int    countDone = 0;
    for (; countDone < countToDo; countDone++)
    {
        // Do the optimistic work up front
        if (*inPtr < 0x80)
        {
            *outPtr++ = XMLCh(*inPtr++);
            continue;
        }

        //
        //  We got non source encoding char. If we got more than 32 chars,
        //  the just break out. We'll come back here later to hit this again
        //  and give an error much closer to the real source position.
        //
        if (countDone > 32)
            break;

        ThrowXML1
        (
            TranscodingException
            , XML4CExcepts::Trans_NotInSourceSet
            , XMLUni::fgUSASCIIEncodingString
        );
    }

    // Set the bytes we ate
    bytesEaten = countDone;

    // Set the char sizes to the fixed size
    memset(charSizes, 1, countDone);

    // Return the chars we transcoded
    return countDone;
}
