/*
 * Copyright 2003,2004 The Apache Software Foundation.
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
 * $Log$
 * Revision 1.4  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.3  2003/10/02 09:33:23  gareth
 * Removed ^Z from end of files which was preventing compilation under gcc 2.96.
 *
 * Revision 1.2  2003/10/01 16:32:38  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.1  2003/04/21 16:20:41  knoaman
 * Initial check-in.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/MemoryManagerImpl.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

void* MemoryManagerImpl::allocate(size_t size)
{
    void* memptr;
    try {
        memptr = ::operator new(size);
    }
    catch(...) {
        throw OutOfMemoryException();
    }
    if (memptr != NULL) {
        return memptr;
    }
    throw OutOfMemoryException();
}

void MemoryManagerImpl::deallocate(void* p)
{
    ::operator delete(p);
}

XERCES_CPP_NAMESPACE_END
