/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2003 The Apache Software Foundation.  All rights
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
 * $Id$
 * $Log$
 * Revision 1.2  2003/12/17 00:01:44  peiyongz
 * getSize()
 *
 * Revision 1.1  2003/12/16 16:56:51  peiyongz
 * BinMemOutputStream
 *
 *
 */

#include <xercesc/internal/BinMemOutputStream.hpp>
#include <xercesc/util/XMLString.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN

BinMemOutputStream::BinMemOutputStream( int                  initCapacity
                                      , MemoryManager* const manager)
: fMemoryManager(manager)
, fDataBuf(0)
, fIndex(0)
, fCapacity(initCapacity)
{
    // Buffer is one larger than capacity, to allow for zero term
    fDataBuf = (XMLByte*) fMemoryManager->allocate
    (
        (fCapacity + 4) * sizeof(XMLByte)
    );

    // Keep it null terminated
    fDataBuf[0] = XMLByte(0);
}

BinMemOutputStream::~BinMemOutputStream()
{
    fMemoryManager->deallocate(fDataBuf);
}

void BinMemOutputStream::writeBytes(       XMLByte*     const      toGo
                                   , const unsigned int            maxToWrite)
{

    if (maxToWrite) 
    {
        insureCapacity(maxToWrite);
        memcpy(&fDataBuf[fIndex], toGo, maxToWrite * sizeof(XMLByte));
        fIndex += maxToWrite;
    }

}

const XMLByte* BinMemOutputStream::getRawBuffer() const
{
    fDataBuf[fIndex] = 0;
    fDataBuf[fIndex + 1] = 0;
    fDataBuf[fIndex + 2] = 0;
    fDataBuf[fIndex + 3] = 0;
    return fDataBuf;
}

void BinMemOutputStream::reset()
{
    fIndex = 0;

    for (int i = 0; i < 4; i++)
    {
        fDataBuf[fIndex + i] = 0;
    }
}

unsigned int BinMemOutputStream::curPos() const
{
    return fIndex;
}

unsigned int BinMemOutputStream::getSize() const
{
    return fIndex;
}

// ---------------------------------------------------------------------------
//  BinMemOutputStream: Private helper methods
// ---------------------------------------------------------------------------
void BinMemOutputStream::insureCapacity(const unsigned int extraNeeded)
{
    // If we can handle it, do nothing yet
    if (fIndex + extraNeeded < fCapacity)
        return;

    // Oops, not enough room. Calc new capacity and allocate new buffer
    const unsigned int newCap = (unsigned int)((fIndex + extraNeeded) * 2);
    XMLByte* newBuf = (XMLByte*) fMemoryManager->allocate
    (
        (newCap+4) * sizeof(XMLByte)
    );

    // Copy over the old stuff
    memcpy(newBuf, fDataBuf, fCapacity * sizeof(XMLByte) + 4);

    // Clean up old buffer and store new stuff
    fMemoryManager->deallocate(fDataBuf); 
    fDataBuf = newBuf;
    fCapacity = newCap;
}


XERCES_CPP_NAMESPACE_END

