/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights
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
 * originally based on software copyright (c) 2001, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 */

#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "DOMStringPool.hpp"
#include "DOMDocumentImpl.hpp"

//
//  DStringPoolEntry - one of these structs is allocated for each
//                      XMLCh String in the pool.  Each slot in the
//                      hash table array itself is a pointer to the head
//                      of a singly-linked list of these structs.
//
//                      Although this struct is delcared with a string length of one,
//                      the factory method allocates enough storage to hold the full
//                      string length.
//
struct DOMStringPoolEntry
{
    DOMStringPoolEntry    *fNext;
    XMLCh                 fString[1];
};


//
// createSPE - factory method for creating sting pool entry structs.
//             Allocates sufficient storage to hold the entire string
//
static DOMStringPoolEntry *createSPE(const XMLCh *str, DOMDocumentImpl *doc)
{
    //  Compute size to allocate.  Note that there's 1 char of string declared in the
    //       struct, so we don't need to add one again to account for the trailing null.
    //
    size_t sizeToAllocate = sizeof(DOMStringPoolEntry) + XMLString::stringLen(str)*sizeof(XMLCh);
    DOMStringPoolEntry *newSPE = (DOMStringPoolEntry *)doc->allocate(sizeToAllocate);
    newSPE->fNext = 0;
    XMLCh * nonConstStr = (XMLCh *)newSPE->fString;
    XMLString::copyString(nonConstStr, str);
    return newSPE;
}



DOMStringPool::DOMStringPool(int hashTableSize, DOMDocumentImpl *doc)
{
    fDoc           = doc;          // needed to get access to the doc's storage allocator.
    fHashTableSize = hashTableSize;

    //fHashTable = new (fDoc) DOMStringPoolEntry *[hashTableSize];
    void* p = doc->allocate(sizeof(DOMStringPoolEntry*) * hashTableSize);
    fHashTable = (DOMStringPoolEntry**) p;
    for (int i=0; i<fHashTableSize; i++)
        fHashTable[i] = 0;
};


//  Destructor.    Nothing to do, since storage all belongs to the document.
//
DOMStringPool::~DOMStringPool()
{
};


const XMLCh *DOMStringPool::getPooledString(const XMLCh *in)
{
    DOMStringPoolEntry    **pspe;
    DOMStringPoolEntry    *spe;

    int    inHash     = XMLString::hash(in, fHashTableSize);
    pspe = &fHashTable[inHash];
    while (*pspe != 0)
    {
        if (XMLString::compareString((*pspe)->fString, in) == 0)
            return (*pspe)->fString;
        pspe = &((*pspe)->fNext);
    }

    // This string hasn't been seen before.  Add it to the pool.
    *pspe = spe = createSPE(in, fDoc);
    return spe->fString;
};




