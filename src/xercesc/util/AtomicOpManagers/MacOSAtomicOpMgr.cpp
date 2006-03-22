
/*
 * Copyright 1999-2000,2004-2005 The Apache Software Foundation.
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
 */

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/AtomicOpManagers/MacOSAtomicOpMgr.hpp>

#include <CoreServices/CoreServices.h>

XERCES_CPP_NAMESPACE_BEGIN


MacOSAtomicOpMgr::MacOSAtomicOpMgr()
{
}


MacOSAtomicOpMgr::~MacOSAtomicOpMgr()
{
}


// Atomic operations
void*
MacOSAtomicOpMgr::compareAndSwap(void**            toFill
							 , const void* const newValue
							 , const void* const toCompare)
{
    // Replace *toFill with newValue iff *toFill == toCompare,
    // returning previous value of *toFill

    Boolean success = CompareAndSwap(
        reinterpret_cast<UInt32>(toCompare),
        reinterpret_cast<UInt32>(newValue),
        reinterpret_cast<UInt32*>(toFill));

    return (success) ? const_cast<void*>(toCompare) : *toFill;
}


//
//	Atomic Increment and Decrement
//
//	Apple's routines return the value as it was before the
//	operation, while these routines want to return it as it
//	is after. So we perform the translation before returning
//	the value.
//
int
MacOSAtomicOpMgr::increment(int &location)
{
    return IncrementAtomic(reinterpret_cast<long*>(&location)) + 1;
}


int
MacOSAtomicOpMgr::decrement(int &location)
{
    return DecrementAtomic(reinterpret_cast<long*>(&location)) - 1;
}


XERCES_CPP_NAMESPACE_END
