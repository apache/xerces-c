/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights
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

#include "DOMString.hpp"
#include "AttrImpl.hpp"
#include "NodeIDMap.hpp"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <stdio.h>

XERCES_CPP_NAMESPACE_BEGIN


static const int gPrimes[] = {997, 9973, 99991, 999983, 0 };  // To do - add a few more.

static const float gMaxFill = 0.8f;   // The maximum fraction of the total
                                    // table entries to consume before exanding.

NodeIDMap::NodeIDMap(int initialSize,
                     MemoryManager* const manager)
: fMemoryManager(manager)
{
    for (fSizeIndex = 0; gPrimes[fSizeIndex] < initialSize; fSizeIndex++)
    {
        if (gPrimes[fSizeIndex] == 0)
        {
            // We need a bigger size than the largest available one.
            //   Big trouble.
            fSizeIndex--;
            ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::NodeIDMap_GrowErr, fMemoryManager);
        }
    }

    fSize = gPrimes[fSizeIndex];
    fNumEntries = 0;
    fMaxEntries = (unsigned long)(float(fSize) * gMaxFill);

    fTable = (AttrImpl**) manager->allocate(fSize * sizeof(AttrImpl*));// new AttrImpl *[fSize];
    unsigned int i;
    for (i=0; i<fSize; i++)
        fTable[i] = 0;
};


NodeIDMap::~NodeIDMap()
{
    // delete[] fTable;
    fMemoryManager->deallocate(fTable);//fTable = 0;
};



void NodeIDMap::add(AttrImpl *attr)
{
	//
	//  If the table is getting too full, grow it.  We arbitrarily limit
	//   the table to 80 full, which should limit the average number of
	//   rehashes to a reasonable value.
	//
	if (fNumEntries >= fMaxEntries)
		growTable();
    fNumEntries++;

	//
	// Hash the value string from the ID attribute being added to the table
	//      0 < Initial hash value < table size.
	//      An initial hash of zero would cause the rehash to fail.
	//
	DOMString id=attr->getValue();
	unsigned int initalHash = XMLString::hashN(id.rawBuffer(), id.length(), fSize-1, fMemoryManager);
	initalHash++;
	unsigned int currentHash = initalHash;

	//
	// Loop looking for an empty slot for this ID.
	//   Don't even bother checking to see if the ID is already there -
	//   the table is only filled by the parser from valid documents, which
	//   can not have duplicates.  Behavior of invalid docs is not defined.
	//
    while (true)
	{
		AttrImpl *tableSlot = fTable[currentHash];
		if (tableSlot == 0 ||
			tableSlot == (AttrImpl *)-1)
			break;
		currentHash += initalHash;  // rehash
        if (currentHash >= fSize)
            currentHash = currentHash % fSize;
    }

    //
    // We've found our slot.  Stick the pointer to the attr into it.
    //
    fTable[currentHash] = attr;

};


void NodeIDMap::remove(AttrImpl *attr)
{
    //
	// Hash the value string from the ID attribute being added to the table
	//      0 < Initial hash value < table size.
	//      An initial hash of zero would cause the rehash to fail.
	//
	DOMString id=attr->getValue();
	unsigned int initalHash = XMLString::hashN(id.rawBuffer(), id.length(), fSize-1, fMemoryManager);
	initalHash++;
	unsigned int currentHash = initalHash;

	//
	// Loop looking for a slot pointing to an attr with this id.
    //
    while (true)
	{
		AttrImpl *tableSlot = fTable[currentHash];
		if (tableSlot == 0)
        {
            // There is no matching entry in the table
            return;
        }

        if (tableSlot == attr)
        {
            //  Found the attribute.  Set the slot to -1 to indicate
            //   that it was once used, meaning that lookups, while never
            //   matching here, can not stop either, but must rehash again
            //   and continue searching.
            fTable[currentHash] = (AttrImpl *)-1;
            return;
        }

        currentHash += initalHash;  // rehash.
        if (currentHash >= fSize)
            currentHash = currentHash % fSize;
    }

};


AttrImpl *NodeIDMap::find(const DOMString &id)
{
    //
    //  Get the hashcode for the supplied string.
    //
	unsigned int initalHash = XMLString::hashN(id.rawBuffer(), id.length(), fSize-1, fMemoryManager);
	initalHash++;
	unsigned int currentHash = initalHash;

	//
	// Loop looking for a slot pointing to an attr with this id.
    //
    while (true)
	{
		AttrImpl *tableSlot = fTable[currentHash];
		if (tableSlot == 0)
        {
            // There is no matching entry in the table
            return 0;
        }


        if ((tableSlot != (AttrImpl *)-1) && tableSlot->getValue().equals(id))
            return tableSlot;

        currentHash += initalHash;  // rehash
        if (currentHash >= fSize)
            currentHash = currentHash % fSize;
    }
    return 0;  // Never gets here, but keeps some compilers happy.
};


//
//  Grow the table to the next larger size.
//      It has gotten too full for efficient operation.
//     (We never fill it all the way)
//
void NodeIDMap::growTable()
{
    AttrImpl     **oldTable = fTable;
    unsigned int oldSize  = fSize;

    //
    //  Figure the new table size.
    //
#if defined(XERCES_DEBUG)
    fprintf(stderr, "growing...\n");
#endif
    fSizeIndex++;
    fSize = gPrimes[fSizeIndex];
    if (fSize == 0)
    {
        // We need to grow bigger than the largest available size.
        //   Big trouble.
        fSizeIndex--;
        ThrowXMLwithMemMgr(RuntimeException, XMLExcepts::NodeIDMap_GrowErr, fMemoryManager);
    }

    //
    //  Allocate the new table.
    //
    fTable = (AttrImpl**) fMemoryManager->allocate(fSize * sizeof(AttrImpl*));//new AttrImpl *[fSize];
    unsigned int i;
    for (i=0; i<fSize; i++)
        fTable[i] = 0;

    fMaxEntries = (unsigned long)(float(fSize) * gMaxFill);

    //
    // Move entries over from the old table to the new one.
    //
    for (i=0; i<oldSize; i++)
    {
        if ((oldTable[i] != 0)  &&  (oldTable[i] != (AttrImpl *)-1))
            add(oldTable[i]);
    }

    fMemoryManager->deallocate(oldTable);//delete [] oldTable;

};


XERCES_CPP_NAMESPACE_END


