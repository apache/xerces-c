/*
 * Copyright 1999-2004 The Apache Software Foundation.
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
 * Revision 1.12  2004/12/13 16:35:21  cargilld
 * Performance improvement from Christian Will.
 *
 * Revision 1.11  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.10  2004/03/02 23:21:37  peiyongz
 * eliminate leakage
 *
 * Revision 1.9  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.7  2003/10/29 16:18:41  peiyongz
 * Implement serialization/deserialization
 *
 * Revision 1.6  2003/10/09 13:49:30  neilg
 * make StringPool functions virtual so that we can implement a synchronized version of StringPool for thread-safe updates.
 *
 * Revision 1.5  2003/05/18 14:02:05  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.4  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/05/15 19:07:45  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:12  peiyongz
 * sane_include
 *
 * Revision 1.6  2001/10/22 15:43:35  tng
 * [Bug 3361] "String pool id was not legal" error in Attributes::getURI().
 *
 * Revision 1.5  2000/07/07 22:16:51  jpolast
 * remove old put(value) function.  use put(key,value) instead.
 *
 * Revision 1.4  2000/05/15 22:31:20  andyh
 * Replace #include<memory.h> with <string.h> everywhere.
 *
 * Revision 1.3  2000/03/02 19:54:46  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:04  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:10  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:14  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/StringPool.hpp>
#include <assert.h>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  XMLStringPool: Constructors and Destructor
// ---------------------------------------------------------------------------
XMLStringPool::XMLStringPool(const  unsigned int  modulus,
                             MemoryManager* const manager) :

    fMemoryManager(manager)
    , fIdMap(0)
    , fHashTable(0)
    , fMapCapacity(64)
    , fCurId(1)
{
    // Create the hash table, passing it the modulus
    fHashTable = new (fMemoryManager) RefHashTableOf<PoolElem>(modulus, false, fMemoryManager);

    // Do an initial allocation of the id map and zero it all out
    fIdMap = (PoolElem**) fMemoryManager->allocate
    (
        fMapCapacity * sizeof(PoolElem*)
    ); //new PoolElem*[fMapCapacity];
    memset(fIdMap, 0, sizeof(PoolElem*) * fMapCapacity);
}

XMLStringPool::~XMLStringPool()
{
    // delete all buckelements, since the hashtable doesn't adopt the elements anymore
    for (unsigned int index = 1; index < fCurId; index++)
    {
        //fIdMap[index]->~PoolElem();                                // we have no destructor
        fMemoryManager->deallocate((void*) fIdMap[index]->fString);  // deallocate memory
        fMemoryManager->deallocate(fIdMap[index]);                   // deallocate memory
    }
    delete fHashTable;
    fMemoryManager->deallocate(fIdMap); //delete [] fIdMap;
}

// ---------------------------------------------------------------------------
//  XMLStringPool: Pool management methods
// ---------------------------------------------------------------------------
void XMLStringPool::flushAll()
{
    // delete all buckelements, since the hashtable doesn't adopt the elements anymore
    for (unsigned int index = 1; index < fCurId; index++)
    {
        //fIdMap[index]->~PoolElem();                                // we have no destructor
        fMemoryManager->deallocate((void*) fIdMap[index]->fString);  // deallocate memory
        fMemoryManager->deallocate(fIdMap[index]);                   // deallocate memory
    }
    fCurId = 1;
    fHashTable->removeAll();
}

// ---------------------------------------------------------------------------
//  XMLStringPool: Private helper methods
// ---------------------------------------------------------------------------
unsigned int XMLStringPool::addNewEntry(const XMLCh* const newString)
{
    // See if we need to expand the id map
    if (fCurId == fMapCapacity)
    {
        // Calculate the new capacity, create a temp new map, and zero it
        const unsigned int newCap = (unsigned int)(fMapCapacity * 1.5);
        PoolElem** newMap = (PoolElem**) fMemoryManager->allocate
        (
            newCap * sizeof(PoolElem*)
        ); //new PoolElem*[newCap];
        memset(newMap, 0, sizeof(PoolElem*) * newCap);

        //
        //  Copy over the old elements from the old map. They are just pointers
        //  so we can do it all at once.
        //
        memcpy(newMap, fIdMap, sizeof(PoolElem*) * fMapCapacity);

        // Clean up the old map and store the new info
        fMemoryManager->deallocate(fIdMap); //delete [] fIdMap;
        fIdMap = newMap;
        fMapCapacity = newCap;
    }

    //
    //  Ok, now create a new element and add it to the hash table. Then store
    //  this new element in the id map at the current id index, then bump the
    //  id index.
    //
    PoolElem* newElem = (PoolElem*) fMemoryManager->allocate(sizeof(PoolElem));
    newElem->fId      = fCurId;
    newElem->fString  = XMLString::replicate(newString, fMemoryManager);
    fHashTable->put((void*)newElem->fString, newElem);
    fIdMap[fCurId] = newElem;

    // Bump the current id and return the id of the new elem we just added
    fCurId++;
    return newElem->fId;
}

/***
 * Support for Serialization/De-serialization
 ***/

IMPL_XSERIALIZABLE_TOCREATE(XMLStringPool)

void XMLStringPool::serialize(XSerializeEngine& serEng)
{
    /***
     * Since we are pretty sure that fIdMap and fHashTable is 
     * not shared by any other object, therefore there is no owned/referenced
     * issue. Thus we can serialize the raw data only, rather than serializing 
     * both fIdMap and fHashTable.
     *
     * And we can rebuild the fIdMap and fHashTable out of the raw data during
     * deserialization.
     *
    ***/
    if (serEng.isStoring())
    {
        serEng<<fCurId;
        for (unsigned int index = 1; index < fCurId; index++)
        {
            const XMLCh* stringData = getValueForId(index);
            serEng.writeString(stringData);
        }
    }
    else
    {
        unsigned int mapSize;
        serEng>>mapSize;
        assert(1 == fCurId);  //make sure empty

        for (unsigned int index = 1; index < mapSize; index++)
        {
            XMLCh* stringData;
            serEng.readString(stringData);
            addNewEntry(stringData);

            //we got to deallocate this string 
            //since stringpool will duplicate this string in the PoolElem and own that copy
            fMemoryManager->deallocate(stringData);
        }
    }
}

XMLStringPool::XMLStringPool(MemoryManager* const manager) :
    fMemoryManager(manager)
    , fIdMap(0)
    , fHashTable(0)
    , fMapCapacity(64)
    , fCurId(1)
{
    // Create the hash table, passing it the modulus
    fHashTable = new (fMemoryManager) RefHashTableOf<PoolElem>(109, false, fMemoryManager);

    // Do an initial allocation of the id map and zero it all out
    fIdMap = (PoolElem**) fMemoryManager->allocate
    (
        fMapCapacity * sizeof(PoolElem*)
    ); //new PoolElem*[fMapCapacity];
    memset(fIdMap, 0, sizeof(PoolElem*) * fMapCapacity);
}

XERCES_CPP_NAMESPACE_END
