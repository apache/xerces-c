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
 * Revision 1.5  2004/09/08 13:56:13  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.4  2003/10/02 09:33:23  gareth
 * Removed ^Z from end of files which was preventing compilation under gcc 2.96.
 *
 * Revision 1.3  2003/10/01 16:32:38  neilg
 * improve handling of out of memory conditions, bug #23415.  Thanks to David Cargill.
 *
 * Revision 1.2  2003/09/06 22:37:55  jberry
 * Fix bug #22938. Deletion of void* is illegal. Thanks Dave Bertoni.
 *
 * Revision 1.1  2003/08/26 21:55:02  jberry
 * Add new memory allocator that allocates using new[], for use where returned memory must be able to be deleted using delete []. This saves duplicated code in cases where a routine is optionally called with a specific memory manager, such as in the case of transcode
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/internal/MemoryManagerArrayImpl.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

void* MemoryManagerArrayImpl::allocate(size_t size)
{
    
    void* memptr;
    try {
        //return ::operator new[](size);
        //return new char[size];
        memptr = new char[size];
    }
    catch(...) {
        throw OutOfMemoryException();
    }
    if (memptr != NULL) {
        return memptr;
    }
    throw OutOfMemoryException();
}

void MemoryManagerArrayImpl::deallocate(void* p)
{
    //::operator delete[](p);
	delete [] (char*)p;
}

XERCES_CPP_NAMESPACE_END

