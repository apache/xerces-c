/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

/**
 * $Log$
 * Revision 1.1  1999/11/09 01:09:05  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:23  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

//
// file DStringPool.cpp
//

#include "DStringPool.hpp"
#include <util/XMLString.hpp>
#include <util/PlatformUtils.hpp>

//
//  DStringPoolEntry - one of these structs is allocated for each
//                      DOMString in the pool.  Each slot in the
//                      hash table array itself is a pointer to the head
//                      of a singly-linked list of these structs.
//
struct DStringPoolEntry
{
    DStringPoolEntry    *fNext;
    DOMString           fString;
};



DStringPool::DStringPool(int hashTableSize)
{
    fHashTableSize = hashTableSize;
    fHashTable = new DStringPoolEntry *[hashTableSize];
    for (int i=0; i<fHashTableSize; i++)
        fHashTable[i] = 0;
};


//  Destructor.   Iterate through the pool, deleting each of the 
//                DSTringPoolEntry structs, then delete the hash
//                array itself.
//
DStringPool::~DStringPool()
{
    for (int slot=0; slot<fHashTableSize; slot++)
    {
        DStringPoolEntry    *spe;
        DStringPoolEntry    *nextSPE;
        for (spe=fHashTable[slot]; spe != 0; spe = nextSPE )
        {
            // spe->string = 0;
            nextSPE = spe->fNext;
            delete spe;    // Note that this will invoke the destructor
                           //   on spe->fString.
        }
    }
    delete fHashTable;
    fHashTable = 0;
};


const DOMString &DStringPool::getPooledString(const XMLCh *in)
{
    DStringPoolEntry    **pspe;
    DStringPoolEntry    *spe;

    int    inHash     = XMLString::hash(in, fHashTableSize);
    pspe = &fHashTable[inHash];
    while (*pspe != 0)
    {
        if ((*pspe)->fString.equals(in))
            return (*pspe)->fString;
        pspe = &((*pspe)->fNext);
    }
    *pspe = spe = new DStringPoolEntry;
    spe->fNext = 0;
    spe->fString = DOMString(in);
    return spe->fString;
};


const DOMString &DStringPool::getPooledString(const DOMString &in)
{
    DStringPoolEntry    **pspe;
    DStringPoolEntry    *spe;

    XMLCh *inCharData = in.rawBuffer();
    int    inLength   = in.length();
    int    inHash     = XMLString::hashN(inCharData, inLength, fHashTableSize);

    pspe = &fHashTable[inHash];
    while (*pspe != 0)
    {
        if ((*pspe)->fString.equals(in))
            return (*pspe)->fString;
        pspe = &((*pspe)->fNext);
    }
    *pspe = spe = new DStringPoolEntry;
    spe->fNext = 0;
    spe->fString = DOMString(in);
    return spe->fString;
};



//
//  getLiteralString
//
//     This is a static function that is somewhat separate from the rest
//      of the string pool.  It is used to manage the one-time creation of 
//      static strings that are reused freqently within the DOM implementation.
//      This is primarily things like the default names for the various
//      node types ("#text" and the like).
//
const DOMString &DStringPool::getStaticString(const char *in, DOMString **loc)
{
    if (*loc == 0)
    {
        DOMString *t = new DOMString(in);   // This is one of the very few
                                            //   places that a DOMString variable
                                            //   is heap allocated.  Normal usage
                                            //   is to create local instances and
                                            //   pass them around by value.
        if (XMLPlatformUtils::compareAndSwap((void **)loc, t, 0) != 0)
            delete t;
    }
    return **loc;
}


