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
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/XMemory.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <assert.h>

XERCES_CPP_NAMESPACE_BEGIN

void* XMemory::operator new(size_t size)
{
	size_t headerSize = XMLPlatformUtils::alignPointerForNewBlockAllocation(
										sizeof(MemoryManager*));
	
    void* const block = XMLPlatformUtils::fgMemoryManager->allocate
        (
	        headerSize + size
        );
    *(MemoryManager**)block = XMLPlatformUtils::fgMemoryManager;

    return (char*)block + headerSize;
}

#if defined(XML_VISUALCPP)

void* XMemory::operator new(size_t size, const char* file, int line)
{ 
		 return operator new(size); 
}
 
void XMemory::operator delete(void* p, const char* file, int line)
{ 
		 operator delete(p); 
}
 
#endif

void* XMemory::operator new(size_t size, MemoryManager* manager)
{
    assert(manager != 0);
	
	size_t headerSize = XMLPlatformUtils::alignPointerForNewBlockAllocation(
										sizeof(MemoryManager*));
    void* const block = manager->allocate(headerSize + size);
    *(MemoryManager**)block = manager;

    return (char*)block + headerSize;
}

void XMemory::operator delete(void* p)
{
    if (p != 0)
    {
		size_t headerSize = XMLPlatformUtils::alignPointerForNewBlockAllocation(
											sizeof(MemoryManager*));
        void* const block = (char*)p - headerSize;

        MemoryManager* const manager = *(MemoryManager**)block;
        assert(manager != 0);
        manager->deallocate(block);
    }
}

//The Borland compiler is complaining about duplicate overloading of delete
#if !defined(XML_BORLAND)

void XMemory::operator delete(void* p, MemoryManager* manager)
{
    assert(manager != 0);
	
    if (p != 0)
    {
        size_t headerSize = XMLPlatformUtils::alignPointerForNewBlockAllocation(sizeof(MemoryManager*));
        void* const block = (char*)p - headerSize;

        /***
         * assert(*(MemoryManager**)block == manager);                 
         *
         * NOTE: for compiler which can't properly trace the memory manager used in the 
         *       placement new, we use the memory manager embedded in the memory rather 
         *       than the one passed in
         */ 
        MemoryManager* pM = *(MemoryManager**)block;
        pM->deallocate(block);
    }
}

#endif

XERCES_CPP_NAMESPACE_END

