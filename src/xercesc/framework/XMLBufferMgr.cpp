/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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
 * Revision 1.5  2004/09/08 13:55:58  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/12/17 00:18:33  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.3  2003/05/15 18:26:07  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:00:21  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:21:51  peiyongz
 * sane_include
 *
 * Revision 1.4  2000/05/15 22:31:11  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.3  2000/03/02 19:54:24  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:47:47  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:08:30  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:44:36  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
//#include <string.h>
#include <xercesc/framework/XMLBufferMgr.hpp>
#include <xercesc/util/RuntimeException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  Constructors and Destructor
// ---------------------------------------------------------------------------
XMLBufferMgr::XMLBufferMgr(MemoryManager* const manager) :

    fBufCount(32)
    , fMemoryManager(manager)
    , fBufList(0)
{
    // Allocate the buffer list and zero it out
    fBufList = (XMLBuffer**) fMemoryManager->allocate(fBufCount * sizeof(XMLBuffer*)); // new XMLBuffer*[fBufCount];
    for (unsigned int index = 0; index < fBufCount; index++)
        fBufList[index] = 0;
}

XMLBufferMgr::~XMLBufferMgr()
{
    // Delete any buffers that got allocated
    for (unsigned int index = 0; index < fBufCount; index++)
        delete fBufList[index];

    // And then the buffer list
    fMemoryManager->deallocate(fBufList); //delete [] fBufList;
}


// ---------------------------------------------------------------------------
//  Buffer management
// ---------------------------------------------------------------------------
XMLBuffer& XMLBufferMgr::bidOnBuffer()
{
    //
    //  Look for a buffer that is not in use. If we hit a null entry, then
    //  we have to add one.
    //
    for (unsigned int index = 0; index < fBufCount; index++)
    {
        // No more buffers available, so create one and take it
        if (!fBufList[index])
        {
            fBufList[index] = new (fMemoryManager) XMLBuffer(1023, fMemoryManager);
            fBufList[index]->setInUse(true);
            return *fBufList[index];
        }

        //
        //  There's one here, so see if its use. If not, mark it, reset it,
        //  and take it
        //
        if (!fBufList[index]->getInUse())
        {
            fBufList[index]->reset();
            fBufList[index]->setInUse(true);
            return *(fBufList[index]);
        }
    }

    // We did not find one, so freak out
    ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::BufMgr_NoMoreBuffers, fMemoryManager);

    // NOTE: Dummy return to make some compilers happy. Never really gets called!
    return *fBufList[0];
}


void XMLBufferMgr::releaseBuffer(XMLBuffer& toRelease)
{
    // Look for this buffer in the list
    for (unsigned int index = 0; index < fBufCount; index++)
    {
        if (fBufList[index] == &toRelease)
        {
            // Unmark it
            toRelease.setInUse(false);
            return;
        }
    }

    // It was not a legal buffer
    ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::BufMgr_BufferNotInPool, fMemoryManager);
}

XERCES_CPP_NAMESPACE_END

