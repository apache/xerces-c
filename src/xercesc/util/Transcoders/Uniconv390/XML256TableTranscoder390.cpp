/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2004 The Apache Software Foundation.  All rights
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
 * Revision 1.1  2004/02/06 15:02:11  cargilld
 * Intrinsic transcoding support for 390.
 *
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/BitOps.hpp>
#include <xercesc/util/TranscodingException.hpp>
#include <xercesc/util/Transcoders/Uniconv390/XML256TableTranscoder390.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <string.h>

extern "OS" void TROT(const XMLByte * input, XMLCh * output,
                      int length, const XMLCh *table,
                      int STOP);


XERCES_CPP_NAMESPACE_BEGIN


// ---------------------------------------------------------------------------
//  XML256TableTranscoder390: Public Destructor
// ---------------------------------------------------------------------------
XML256TableTranscoder390::~XML256TableTranscoder390()
{
    // We don't own the tables, we just reference them
}


// ---------------------------------------------------------------------------
//  XML256TableTranscoder390: Implementation of the transcoder API
// ---------------------------------------------------------------------------
unsigned int
XML256TableTranscoder390::transcodeFrom(const  XMLByte* const       srcData
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
    const unsigned int countToDo = srcCount < maxChars ? srcCount : maxChars;

    //
    //  Loop through the count we have to do and map each char via the
    //  lookup table.
    //
    const XMLByte*  srcPtr = srcData;
    const XMLByte*  endPtr = (srcPtr + countToDo);
    XMLCh*          outPtr = toFill;
    XMLCh stop = 0xffff; 

    TROT(srcData, toFill, countToDo, fFromTable, stop);

    // Set the bytes eaten
    bytesEaten = countToDo;

    // Set the character sizes to the fixed size
    memset(charSizes, 1, countToDo);

    // Return the chars we transcoded
    return countToDo;
}


unsigned int
XML256TableTranscoder390::transcodeTo( const   XMLCh* const    srcData
                                    , const unsigned int    srcCount
                                    ,       XMLByte* const  toFill
                                    , const unsigned int    maxBytes
                                    ,       unsigned int&   charsEaten
                                    , const UnRepOpts       options)
{
    // If debugging, make sure that the block size is legal
    #if defined(XERCES_DEBUG)
    checkBlockSize(maxBytes);
    #endif

    //
    //  Calculate the max chars we can do here. Its the lesser of the
    //  max output chars and the number of chars in the source.
    //
    const unsigned int countToDo = srcCount < maxBytes ? srcCount : maxBytes;

    //
    //  Loop through the count we have to do and map each char via the
    //  lookup table.
    //
    const XMLCh*    srcPtr = srcData;
    const XMLCh*    endPtr = (srcPtr + countToDo);
    XMLByte*        outPtr = toFill;
    XMLByte         nextOut;
    while (srcPtr < endPtr)
    {
        //
        //  Get the next src char out to a temp, then do a binary search
        //  of the 'to' table for this entry.
        //
        if ((nextOut = xlatOneTo(*srcPtr)))
        {
            *outPtr++ = nextOut;
            srcPtr++;
            continue;
        }

        //
        //  Its not representable so, according to the options, either
        //  throw or use the replacement.
        //
        if (options == UnRep_Throw)
        {
            XMLCh tmpBuf[17];
            XMLString::binToText((unsigned int)*srcPtr, tmpBuf, 16, 16, getMemoryManager());
            ThrowXMLwithMemMgr2
            (
                TranscodingException
                , XMLExcepts::Trans_Unrepresentable
                , tmpBuf
                , getEncodingName()
                , getMemoryManager()
            );
        }

        // Eat the source char and use the replacement char
        srcPtr++;
        *outPtr++ = 0x3F;
    }

    // Set the chars eaten
    charsEaten = countToDo;

    // Return the bytes we transcoded
    return countToDo;
}


bool XML256TableTranscoder390::canTranscodeTo(const unsigned int toCheck) const
{
    return (xlatOneTo(toCheck) != 0);
}


// ---------------------------------------------------------------------------
//  XML256TableTranscoder390: Hidden constructor
// ---------------------------------------------------------------------------
XML256TableTranscoder390::
XML256TableTranscoder390(  const   XMLCh* const                     encodingName
                        , const unsigned int                     blockSize
                        , const XMLCh* const                     fromTable
                        , const XMLTransService::TransRec* const toTable
                        , const unsigned int                     toTableSize
                        , MemoryManager* const                   manager) :

    XMLTranscoder(encodingName, blockSize, manager)
    , fFromTable(fromTable)
    , fToSize(toTableSize)
    , fToTable(toTable)
{
}


// ---------------------------------------------------------------------------
//  XML256TableTranscoder390: Private helper methods
// ---------------------------------------------------------------------------
XMLByte XML256TableTranscoder390::xlatOneTo(const XMLCh toXlat) const
{
    unsigned int    lowOfs = 0;
    unsigned int    hiOfs = fToSize - 1;
    
    do
    {
        // Calc the mid point of the low and high offset.
        const unsigned int midOfs = ((hiOfs - lowOfs) / 2) + lowOfs;

        //
        //  If our test char is greater than the mid point char, then
        //  we move up to the upper half. Else we move to the lower
        //  half. If its equal, then its our guy.
        //
        if (toXlat > fToTable[midOfs].intCh)
        {
            lowOfs = midOfs;
        }
         else if (toXlat < fToTable[midOfs].intCh)
        {
            hiOfs = midOfs;
        }
         else
        {
            return fToTable[midOfs].extCh;
        }
    }   while (lowOfs + 1 < hiOfs);

    // Check the high end of the range otherwise the
    // last item in the table may never be found.
        if (toXlat == fToTable[hiOfs].intCh)
        {
            return fToTable[hiOfs].extCh;
        }

    return 0;
}

XERCES_CPP_NAMESPACE_END
