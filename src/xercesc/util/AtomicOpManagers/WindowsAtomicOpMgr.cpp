
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

#include <windows.h>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/AtomicOpManagers/WindowsAtomicOpMgr.hpp>

XERCES_CPP_NAMESPACE_BEGIN


WindowsAtomicOpMgr::WindowsAtomicOpMgr()
{
}


WindowsAtomicOpMgr::~WindowsAtomicOpMgr()
{
}


// Atomic operations
void*
WindowsAtomicOpMgr::compareAndSwap(void**            toFill
							 , const void* const newValue
							 , const void* const toCompare)
{
#if defined WIN64
    return ::InterlockedCompareExchangePointer(toFill, (void*)newValue, (void*)toCompare);
#else

    //
    //  InterlockedCompareExchange is only supported on Windows 98,
    //  Windows NT 4.0, and newer -- not on Windows 95...
    //  If you are willing to give up Win95 support change this to #if 0
    //  otherwise we are back to using assembler.
    //  (But only if building with compilers that support inline assembler.)
    //
    #if (defined(_MSC_VER) || defined(__BCPLUSPLUS__)) && !defined(XERCES_NO_ASM)

    void*   result;
    __asm
    {
        mov             eax, toCompare;
        mov             ebx, newValue;
        mov             ecx, toFill
        lock cmpxchg    [ecx], ebx;
        mov             result, eax;
    }
    return result;

    #else

    //
    //  Note we have to cast off the constness of some of these because
    //  the system APIs are not C++ aware in all cases.
    //

    return (void*) ::InterlockedCompareExchange((LPLONG)toFill, (LONG)newValue, (LONG)toCompare);

    #endif

#endif

}


int
WindowsAtomicOpMgr::increment(int &location)
{
    return ::InterlockedIncrement(&(long &)location);
}


int
WindowsAtomicOpMgr::decrement(int &location)
{
    return ::InterlockedDecrement(&(long &)location);
}


XERCES_CPP_NAMESPACE_END
