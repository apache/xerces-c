/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id$
 * $Log$
 * Revision 1.9  2004/09/08 13:55:57  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/01/29 11:46:29  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.7  2003/05/16 21:36:55  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.6  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.5  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.4  2002/08/12 21:38:22  peiyongz
 * Bug#11462: MemBufFormatTarget issue(2) .., proposed patch from
 *                      Esmond Pitt (pitte@anz.com)
 *
 * Revision 1.3  2002/07/22 23:23:15  tng
 * DOM L3: MemBufFormatTarget stores fDataBuf as XMLByte directly, consistent design as MemBufInputSource
 *
 * Revision 1.2  2002/06/05 15:47:13  peiyongz
 * data member changed, reset() added.
 *
 * Revision 1.1  2002/05/28 22:40:46  peiyongz
 * DOM3 Save Interface: DOMWriter/DOMWriterFilter
 *
 */

#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/util/XMLString.hpp>
#include <string.h>

XERCES_CPP_NAMESPACE_BEGIN

MemBufFormatTarget::MemBufFormatTarget( int                  initCapacity
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
    );//new XMLByte[fCapacity+4];

    // Keep it null terminated
    fDataBuf[0] = XMLByte(0);
}

MemBufFormatTarget::~MemBufFormatTarget()
{
    fMemoryManager->deallocate(fDataBuf);//delete [] fDataBuf;
}

void MemBufFormatTarget::writeChars(const XMLByte* const toWrite
                                  , const unsigned int   count
                                  , XMLFormatter * const)
{

    if (count) {
        insureCapacity(count);
        memcpy(&fDataBuf[fIndex], toWrite, count * sizeof(XMLByte));
        fIndex += count;
    }

}

const XMLByte* MemBufFormatTarget::getRawBuffer() const
{
    fDataBuf[fIndex] = 0;
    fDataBuf[fIndex + 1] = 0;
    fDataBuf[fIndex + 2] = 0;
    fDataBuf[fIndex + 3] = 0;
    return fDataBuf;
}

void MemBufFormatTarget::reset()
{
    fIndex = 0;
    fDataBuf[0] = 0;
    fDataBuf[fIndex + 1] = 0;
    fDataBuf[fIndex + 2] = 0;
    fDataBuf[fIndex + 3] = 0;
}

// ---------------------------------------------------------------------------
//  MemBufFormatTarget: Private helper methods
// ---------------------------------------------------------------------------
void MemBufFormatTarget::insureCapacity(const unsigned int extraNeeded)
{
    // If we can handle it, do nothing yet
    if (fIndex + extraNeeded < fCapacity)
        return;

    // Oops, not enough room. Calc new capacity and allocate new buffer
    const unsigned int newCap = (unsigned int)((fIndex + extraNeeded) * 2);
    XMLByte* newBuf = (XMLByte*) fMemoryManager->allocate
    (
        (newCap+4) * sizeof(XMLByte)
    );//new XMLByte[newCap+4];

    // Copy over the old stuff
    memcpy(newBuf, fDataBuf, fCapacity * sizeof(XMLByte) + 4);

    // Clean up old buffer and store new stuff
    fMemoryManager->deallocate(fDataBuf); //delete [] fDataBuf;
    fDataBuf = newBuf;
    fCapacity = newCap;
}

XERCES_CPP_NAMESPACE_END

