/*
 * Copyright 1999-2004 The Apache Software Foundation.
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

/**
 * $Log$
 * Revision 1.8  2004/12/13 16:36:43  cargilld
 * Performance improvement from Christian Will and bug fix from David Bertoni.
 *
 * Revision 1.7  2004/09/29 20:33:51  peiyongz
 * resize the internal buffer even the handler can successfully flush the buffer
 *
 * Revision 1.5  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/08/13 17:07:43  peiyongz
 * using 2 instead of 1.25 to expand capacity
 *
 * Revision 1.1.1.1  2002/02/01 22:21:50  peiyongz
 * sane_include
 *
 * Revision 1.5  2000/08/17 00:04:50  andyh
 * Fix error in growing of XMLBuffer from within ensureCapacity()
 * Fixes crash pointed out by Simon Fell.
 *
 * Revision 1.4  2000/05/15 22:31:11  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.3  2000/02/06 07:47:47  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.2  1999/11/23 01:04:19  roddey
 * Updates to some of the internal VC++ project files and some small
 * fixes to make XMLBuffer correctly character size agnostic.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:29  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:35  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/framework/XMLBuffer.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/RuntimeException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLBuffer: Buffer management
// ---------------------------------------------------------------------------

void XMLBuffer::insureCapacity(const unsigned int extraNeeded)
{    
    // If we can't handle it, try doubling the buffer size.
    unsigned int newCap = (fIndex + extraNeeded) * 2;

    // If a maximum size is set, and double the current buffer size exceeds that
    // maximum, first check if the maximum size will accomodate the extra needed.
    if (fFullHandler && (newCap > fFullSize))
    {
        // If the maximum buffer size accomodates the extra needed, resize to
        // the maximum
        if (fIndex + extraNeeded <= fFullSize) 
        {
            newCap = fFullSize;
        }

        // Otherwise, allow the registered full-handler to try to empty the buffer.
        // If it claims success, and we can accommodate the extra needed in the buffer
        // to be expanded, resize to the maximum
        // Note the order of evaluation: bufferFull() has the intentional side-effect
        // of modifying fIndex.
        else if (fFullHandler->bufferFull(*this) && (fIndex + extraNeeded <= fFullSize))
        {
            newCap = fFullSize;
        }

        // Finally, if the full-handler failed, or the buffer (of maximum size) 
        // still can't accomodate the extra needed, we must fail.
        else
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::Array_BadNewSize, fMemoryManager);
    }

    // Note the previous if block can modify newCap, so we may not need to allocate
    // at all.
    if (newCap > fCapacity)
    {
        // Allocate new buffer
        XMLCh* newBuf = (XMLCh*) fMemoryManager->allocate((newCap+1) * sizeof(XMLCh)); //new XMLCh[newCap+1];
 
        // Copy over the old stuff
        memcpy(newBuf, fBuffer, fIndex * sizeof(XMLCh));

        // Clean up old buffer and store new stuff
        fMemoryManager->deallocate(fBuffer); //delete [] fBuffer;
        fBuffer = newBuf;
        fCapacity = newCap;
    }
}

XERCES_CPP_NAMESPACE_END

