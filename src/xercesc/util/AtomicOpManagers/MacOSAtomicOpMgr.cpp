
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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

#include <libkern/OSAtomic.h>

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
    bool success = OSAtomicCompareAndSwapPtrBarrier(
    	const_cast<void*>(toCompare),
    	const_cast<void*>(newValue),
    	toFill
    	);

    return (success) ? const_cast<void*>(toCompare) : *toFill;
}


//
//	Atomic Increment and Decrement
//
//	The return value is the value following the increment or decrement operation
//
int
MacOSAtomicOpMgr::increment(int &location)
{
    return OSAtomicIncrement32Barrier(reinterpret_cast<int32_t*>(&location));
}


int
MacOSAtomicOpMgr::decrement(int &location)
{
    return OSAtomicDecrement32Barrier(reinterpret_cast<int32_t*>(&location));
}


XERCES_CPP_NAMESPACE_END
