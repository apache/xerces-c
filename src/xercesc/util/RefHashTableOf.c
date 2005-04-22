/*
 * Copyright 1999-2000,2004 The Apache Software Foundation.
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

/**
 * $Log$
 * Revision 1.22  2005/04/22 16:12:48  dbertoni
 * Patch for Jira issue XERCESC-1410.
 *
 * Revision 1.21  2005/02/25 09:16:51  gareth
 * Fix thread safety issues. Jira #30380. Thanks to David Bertoni.
 *
 * Revision 1.20  2005/02/08 09:21:11  amassari
 * Removed warnings
 *
 * Revision 1.19  2005/01/07 15:12:10  amassari
 * Removed warnings
 *
 * Revision 1.18  2004/11/19 00:50:22  cargilld
 * Memory improvement to utility classes from Christian Will.  Remove dependency on XMemory.
 *
 * Revision 1.17  2004/11/18 01:35:20  cargilld
 * Performance improvement to utility classes from Christian Will.  Avoid unnecessary checks and replace with assert calls.
 *
 * Revision 1.16  2004/09/08 13:56:23  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.15  2004/04/22 10:47:44  cargilld
 * Misc fix to update fCount in the correct place.
 *
 * Revision 1.14  2004/03/01 15:03:08  peiyongz
 * new getter: getHashModulus
 *
 * Revision 1.13  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.12  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.11  2003/08/20 11:51:39  gareth
 * Reorderd initializer list to prevent compiler warning.
 *
 * Revision 1.10  2003/05/18 14:02:05  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.9  2003/05/16 21:36:59  knoaman
 * Memory manager implementation: Modify constructors to pass in the memory manager.
 *
 * Revision 1.8  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.7  2003/05/15 10:37:08  gareth
 * Optimization. We now resize the hash when appropriate. Patch by Nathan Codding.
 *
 * Revision 1.6  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.5  2002/07/11 18:49:53  knoaman
 * Add setAdoptElements method.
 * Rename removeBucketElemSafe to orphanKey.
 *
 * Revision 1.4  2002/07/05 11:31:04  tng
 * Fix typo.
 *
 * Revision 1.3  2002/07/04 15:24:57  tng
 * DOM L3: add transferElement and removeBucketElemSafe for use in DOMDocument::renameNode.
 *
 * Revision 1.2  2002/06/12 17:14:03  tng
 * Add function cleanup, reinitialize and nextElementKey for ease of use.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:12  peiyongz
 * sane_include
 *
 * Revision 1.9  2001/07/19 18:43:18  peiyongz
 * fix: detect null poiniter in enumerator's ctor.
 *
 * Revision 1.8  2001/06/04 13:45:04  tng
 * The "hash" argument clashes with STL hash.  Fixed by Pei Yong Zhang.
 *
 * Revision 1.7  2000/09/06 00:24:16  andyh
 * Clean up misc compiler warnings
 *
 * Revision 1.6  2000/07/07 22:16:50  jpolast
 * remove old put(value) function.  use put(key,value) instead.
 *
 * Revision 1.5  2000/06/29 18:27:09  jpolast
 * bug fix for passing hasher class references to constructor
 *
 * Revision 1.4  2000/06/27 22:11:12  jpolast
 * added more general functionality to hashtables.
 * able to specify which hasher to use.
 * default: HashXMLCh [hashes XMLCh* strings]
 *
 * future todo: make hasher class references static so only
 * one instance of a hasher is ever created.
 *
 * Revision 1.3  2000/03/02 19:54:44  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.2  2000/02/06 07:48:03  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.1.1.1  1999/11/09 01:04:59  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:12  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


// ---------------------------------------------------------------------------
//  Include
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/RefHashTableOf.hpp>
#endif

#include <xercesc/util/Janitor.hpp>
#include <xercesc/util/NullPointerException.hpp>
#include <assert.h>
#include <new>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  RefHashTableOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal>
RefHashTableOf<TVal>::RefHashTableOf( const unsigned int modulus
                                    , const bool adoptElems
                                    , MemoryManager* const manager)

    : fMemoryManager(manager)
    , fAdoptedElems(adoptElems)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fInitialModulus(modulus)
    , fCount(0)
    , fHash(0)

{
    initialize(modulus);
	
	// create default hasher
	fHash = new (fMemoryManager) HashXMLCh();
}

template <class TVal>
RefHashTableOf<TVal>::RefHashTableOf( const unsigned int modulus
                                    , const bool adoptElems
                                    , HashBase* hashBase
                                    , MemoryManager* const manager)

    : fMemoryManager(manager)
    , fAdoptedElems(adoptElems)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fInitialModulus(modulus)
    , fCount(0)
    , fHash(0)
{
    initialize(modulus);
    // set hasher
    fHash = hashBase;
}

template <class TVal>
RefHashTableOf<TVal>::RefHashTableOf(const unsigned int modulus
                                     , MemoryManager* const manager)

    : fMemoryManager(manager)
    , fAdoptedElems(true)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fInitialModulus(modulus)
    , fCount(0)
    , fHash(0)
{
    initialize(modulus);

    // create default hasher
    fHash = new (fMemoryManager) HashXMLCh();
}

template <class TVal> void RefHashTableOf<TVal>::initialize(const unsigned int modulus)
{
    if (modulus == 0)
        ThrowXMLwithMemMgr(IllegalArgumentException, XMLExcepts::HshTbl_ZeroModulus, fMemoryManager);

    // Allocate the bucket list and zero them
    fBucketList = (RefHashTableBucketElem<TVal>**) fMemoryManager->allocate
    (
        fHashModulus * sizeof(RefHashTableBucketElem<TVal>*)
    ); //new RefHashTableBucketElem<TVal>*[fHashModulus];
    for (unsigned int index = 0; index < fHashModulus; index++)
        fBucketList[index] = 0;
}

template <class TVal> RefHashTableOf<TVal>::~RefHashTableOf()
{
    cleanup();
}


// ---------------------------------------------------------------------------
//  RefHashTableOf: Element management
// ---------------------------------------------------------------------------
template <class TVal> bool RefHashTableOf<TVal>::isEmpty() const
{
    return fCount==0;
}

template <class TVal> bool RefHashTableOf<TVal>::
containsKey(const void* const key) const
{
    unsigned int hashVal;
    const RefHashTableBucketElem<TVal>* findIt = findBucketElem(key, hashVal);
    return (findIt != 0);
}

template <class TVal> void RefHashTableOf<TVal>::
removeKey(const void* const key)
{
    // Hash the key
    unsigned int hashVal = fHash->getHashVal(key, fHashModulus, fMemoryManager);
    assert(hashVal < fHashModulus);

    //
    //  Search the given bucket for this key. Keep up with the previous
    //  element so we can patch around it.
    //
    RefHashTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    RefHashTableBucketElem<TVal>* lastElem = 0;

    while (curElem)
    {
        if (fHash->equals(key, curElem->fKey))
        {
            if (!lastElem)
            {
                // It was the first in the bucket
                fBucketList[hashVal] = curElem->fNext;
            }
             else
            {
                // Patch around the current element
                lastElem->fNext = curElem->fNext;
            }

            // If we adopted the data, then delete it too
            //    (Note:  the userdata hash table instance has data type of void *.
            //    This will generate compiler warnings here on some platforms, but they
            //    can be ignored since fAdoptedElements is false.
            if (fAdoptedElems)
                delete curElem->fData;

            // Then delete the current element and move forward
 	        // delete curElem;
            // destructor doesn't do anything...
			// curElem->~RefHashTableBucketElem();
            fMemoryManager->deallocate(curElem);            

            fCount--;

            return;
        }

        // Move both pointers upwards
        lastElem = curElem;
        curElem = curElem->fNext;
    }

    // We never found that key
    ThrowXMLwithMemMgr(NoSuchElementException, XMLExcepts::HshTbl_NoSuchKeyExists, fMemoryManager);
}

template <class TVal> void RefHashTableOf<TVal>::removeAll()
{
    if(isEmpty())
        return;

    // Clean up the buckets first
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        // Get the bucket list head for this entry
        RefHashTableBucketElem<TVal>* curElem = fBucketList[buckInd];
        RefHashTableBucketElem<TVal>* nextElem;
        while (curElem)
        {
            // Save the next element before we hose this one
            nextElem = curElem->fNext;

            // If we adopted the data, then delete it too
            //    (Note:  the userdata hash table instance has data type of void *.
            //    This will generate compiler warnings here on some platforms, but they
            //    can be ignored since fAdoptedElements is false.
            if (fAdoptedElems)
                delete curElem->fData;

            // Then delete the current element and move forward
 	        // delete curElem;
            // destructor doesn't do anything...
			// curElem->~RefHashTableBucketElem();
            fMemoryManager->deallocate(curElem);            
            curElem = nextElem;
        }

        // Clean out this entry
        fBucketList[buckInd] = 0;
    }

    fCount = 0;
}

// This method returns the data associated with a key. The key entry is deleted. The caller
// now owns the returned data (case of hashtable adopting the data).
// This function is called by transferElement so that the undeleted data can be transferred
// to a new key which will own that data.
template <class TVal> TVal* RefHashTableOf<TVal>::
orphanKey(const void* const key)
{
    // Hash the key
    TVal* retVal = 0;
    unsigned int hashVal = fHash->getHashVal(key, fHashModulus, fMemoryManager);
    assert(hashVal < fHashModulus);

    //
    //  Search the given bucket for this key. Keep up with the previous
    //  element so we can patch around it.
    //
    RefHashTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    RefHashTableBucketElem<TVal>* lastElem = 0;

    while (curElem)
    {
        if (fHash->equals(key, curElem->fKey))
        {
            if (!lastElem)
            {
                // It was the first in the bucket
                fBucketList[hashVal] = curElem->fNext;
            }
            else
            {
                // Patch around the current element
                lastElem->fNext = curElem->fNext;
            }

            retVal = curElem->fData;

            // Delete the current element
            // delete curElem;
            // destructor doesn't do anything...
			// curElem->~RefHashTableBucketElem();
            fMemoryManager->deallocate(curElem);            
            break;
        }

        // Move both pointers upwards
        lastElem = curElem;
        curElem = curElem->fNext;
    }

    // We never found that key
    if (!retVal)
        ThrowXMLwithMemMgr(NoSuchElementException, XMLExcepts::HshTbl_NoSuchKeyExists, fMemoryManager);

    return retVal;
}

//
// cleanup():
//   similar to destructor
//   called to cleanup the memory, in case destructor cannot be called
//
template <class TVal> void RefHashTableOf<TVal>::cleanup()
{
    removeAll();

    // Then delete the bucket list & hasher
    fMemoryManager->deallocate(fBucketList); //delete [] fBucketList;
    fBucketList = 0;
    delete fHash;
}

//
// reinitialize():
//   similar to constructor
//   called to re-construct the fElemList from scratch again
//
template <class TVal> void RefHashTableOf<TVal>::reinitialize(HashBase* hashBase)
{
    if (fBucketList || fHash)
        cleanup();

    fHashModulus = fInitialModulus;
    initialize(fHashModulus);

    if (hashBase)
        fHash = hashBase;
    else
        fHash = new (fMemoryManager) HashXMLCh();   // create default hasher
}



// this function transfer the data from key1 to key2
// this is equivalent to calling
//  1.  get(key1) to retrieve the data,
//  2.  removeKey(key1),
//  3.  and then put(key2, data)
// except that the data is not deleted in "removeKey" even it is adopted so that it
// can be transferred to key2.
// whatever key2 has originally will be purged (if adopted)
template <class TVal> void RefHashTableOf<TVal>::transferElement(const void* const key1, void* key2)
{
    put(key2, orphanKey(key1));
}


// ---------------------------------------------------------------------------
//  RefHashTableOf: Getters
// ---------------------------------------------------------------------------
template <class TVal> TVal* RefHashTableOf<TVal>::get(const void* const key)
{
    unsigned int hashVal;
    RefHashTableBucketElem<TVal>* findIt = findBucketElem(key, hashVal);
    if (!findIt)
        return 0;
    return findIt->fData;
}

template <class TVal> const TVal* RefHashTableOf<TVal>::
get(const void* const key) const
{
    unsigned int hashVal;
    const RefHashTableBucketElem<TVal>* findIt = findBucketElem(key, hashVal);
    if (!findIt)
        return 0;
    return findIt->fData;
}

template <class TVal>
MemoryManager* RefHashTableOf<TVal>::getMemoryManager() const
{
    return fMemoryManager;
}

template <class TVal>
unsigned int RefHashTableOf<TVal>::getHashModulus() const
{
    return fHashModulus;
}

template <class TVal>
unsigned int RefHashTableOf<TVal>::getCount() const
{
    return fCount;
}

// ---------------------------------------------------------------------------
//  RefHashTableOf: Getters
// ---------------------------------------------------------------------------
template <class TVal>
void RefHashTableOf<TVal>::setAdoptElements(const bool aValue)
{
    fAdoptedElems = aValue;
}

// ---------------------------------------------------------------------------
//  RefHashTableOf: Putters
// ---------------------------------------------------------------------------
template <class TVal> void RefHashTableOf<TVal>::put(void* key, TVal* const valueToAdopt)
{
    // Apply 0.75 load factor to find threshold.
    unsigned int threshold = fHashModulus * 3 / 4;
    
    // If we've grown too big, expand the table and rehash.
    if (fCount >= threshold)
        rehash();

    // First see if the key exists already
    unsigned int hashVal;
    RefHashTableBucketElem<TVal>* newBucket = findBucketElem(key, hashVal);

    //
    //  If so,then update its value. If not, then we need to add it to
    //  the right bucket
    //
    if (newBucket)
    {
        if (fAdoptedElems)
            delete newBucket->fData;
        newBucket->fData = valueToAdopt;
		newBucket->fKey = key;
    }
    else
    {
        //newBucket = new (fMemoryManager) RefHashTableBucketElem<TVal>(key, valueToAdopt, fBucketList[hashVal]);
		newBucket =
             new (fMemoryManager->allocate(sizeof(RefHashTableBucketElem<TVal>)))
             RefHashTableBucketElem<TVal>(key, valueToAdopt, fBucketList[hashVal]);        
        fBucketList[hashVal] = newBucket;
        fCount++;
    }
}



// ---------------------------------------------------------------------------
//  RefHashTableOf: Private methods
// ---------------------------------------------------------------------------
template <class TVal> void RefHashTableOf<TVal>::rehash()
{
    const unsigned int newMod = fHashModulus * 2;

    RefHashTableBucketElem<TVal>** newBucketList =
        (RefHashTableBucketElem<TVal>**) fMemoryManager->allocate
    (
        newMod * sizeof(RefHashTableBucketElem<TVal>*)
    );//new RefHashTableBucketElem<TVal>*[newMod];

    // Make sure the new bucket list is destroyed if an
    // exception is thrown.
    ArrayJanitor<RefHashTableBucketElem<TVal>*>  guard(newBucketList, fMemoryManager);

    memset(newBucketList, 0, newMod * sizeof(newBucketList[0]));
    
    
    // Rehash all existing entries.
    for (unsigned int index = 0; index < fHashModulus; index++)
    {
        // Get the bucket list head for this entry
        RefHashTableBucketElem<TVal>* curElem = fBucketList[index];

        while (curElem)
        {
            // Save the next element before we detach this one
            RefHashTableBucketElem<TVal>* const nextElem = curElem->fNext;

            const unsigned int hashVal = fHash->getHashVal(curElem->fKey, newMod, fMemoryManager);
            assert(hashVal < newMod);

            RefHashTableBucketElem<TVal>* const newHeadElem = newBucketList[hashVal];

            // Insert at the start of this bucket's list.
            curElem->fNext = newHeadElem;
            newBucketList[hashVal] = curElem;

            curElem = nextElem;
        }
    }

    RefHashTableBucketElem<TVal>** const oldBucketList = fBucketList;

    // Everything is OK at this point, so update the
    // member variables.
    fBucketList = guard.release();
    fHashModulus = newMod;

    // Delete the old bucket list.
    fMemoryManager->deallocate(oldBucketList);//delete[] oldBucketList;
    
}

template <class TVal> RefHashTableBucketElem<TVal>* RefHashTableOf<TVal>::
findBucketElem(const void* const key, unsigned int& hashVal)
{
    // Hash the key
    hashVal = fHash->getHashVal(key, fHashModulus, fMemoryManager);
    assert(hashVal < fHashModulus);

    // Search that bucket for the key
    RefHashTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
		if (fHash->equals(key, curElem->fKey))
            return curElem;

        curElem = curElem->fNext;
    }
    return 0;
}

template <class TVal> const RefHashTableBucketElem<TVal>* RefHashTableOf<TVal>::
findBucketElem(const void* const key, unsigned int& hashVal) const
{
    // Hash the key
    hashVal = fHash->getHashVal(key, fHashModulus, fMemoryManager);
    assert(hashVal < fHashModulus);

    // Search that bucket for the key
    const RefHashTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
        if (fHash->equals(key, curElem->fKey))
            return curElem;

        curElem = curElem->fNext;
    }
    return 0;
}


// ---------------------------------------------------------------------------
//  RefHashTableOfEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal> RefHashTableOfEnumerator<TVal>::
RefHashTableOfEnumerator(RefHashTableOf<TVal>* const toEnum
                         , const bool adopt
                         , MemoryManager* const manager)
	: fAdopted(adopt), fCurElem(0), fCurHash((unsigned int)-1), fToEnum(toEnum)
    , fMemoryManager(manager)
{
    if (!toEnum)
        ThrowXMLwithMemMgr(NullPointerException, XMLExcepts::CPtr_PointerIsZero, fMemoryManager);

    //
    //  Find the next available bucket element in the hash table. If it
    //  comes back zero, that just means the table is empty.
    //
    //  Note that the -1 in the current hash tells it to start from the
    //  beginning.
    //
    findNext();
}

template <class TVal> RefHashTableOfEnumerator<TVal>::~RefHashTableOfEnumerator()
{
    if (fAdopted)
        delete fToEnum;
}


template <class TVal> RefHashTableOfEnumerator<TVal>::
RefHashTableOfEnumerator(const RefHashTableOfEnumerator<TVal>& toCopy) :
    XMLEnumerator<TVal>(toCopy)
    , XMemory(toCopy)
    , fAdopted(toCopy.fAdopted)
    , fCurElem(toCopy.fCurElem)
    , fCurHash(toCopy.fCurHash)
    , fToEnum(toCopy.fToEnum)
    , fMemoryManager(toCopy.fMemoryManager)
{
}
// ---------------------------------------------------------------------------
//  RefHashTableOfEnumerator: Enum interface
// ---------------------------------------------------------------------------
template <class TVal> bool RefHashTableOfEnumerator<TVal>::hasMoreElements() const
{
    //
    //  If our current has is at the max and there are no more elements
    //  in the current bucket, then no more elements.
    //
    if (!fCurElem && (fCurHash == fToEnum->fHashModulus))
        return false;
    return true;
}

template <class TVal> TVal& RefHashTableOfEnumerator<TVal>::nextElement()
{
    // Make sure we have an element to return
    if (!hasMoreElements())
        ThrowXMLwithMemMgr(NoSuchElementException, XMLExcepts::Enum_NoMoreElements, fMemoryManager);

    //
    //  Save the current element, then move up to the next one for the
    //  next time around.
    //
    RefHashTableBucketElem<TVal>* saveElem = fCurElem;
    findNext();

    return *saveElem->fData;
}

template <class TVal> void* RefHashTableOfEnumerator<TVal>::nextElementKey()
{
    // Make sure we have an element to return
    if (!hasMoreElements())
        ThrowXMLwithMemMgr(NoSuchElementException, XMLExcepts::Enum_NoMoreElements, fMemoryManager);

    //
    //  Save the current element, then move up to the next one for the
    //  next time around.
    //
    RefHashTableBucketElem<TVal>* saveElem = fCurElem;
    findNext();

    return saveElem->fKey;
}

template <class TVal> void RefHashTableOfEnumerator<TVal>::Reset()
{
    fCurHash = (unsigned int)-1;
    fCurElem = 0;
    findNext();
}



// ---------------------------------------------------------------------------
//  RefHashTableOfEnumerator: Private helper methods
// ---------------------------------------------------------------------------
template <class TVal> void RefHashTableOfEnumerator<TVal>::findNext()
{
    //
    //  If there is a current element, move to its next element. If this
    //  hits the end of the bucket, the next block will handle the rest.
    //
    if (fCurElem)
        fCurElem = fCurElem->fNext;

    //
    //  If the current element is null, then we have to move up to the
    //  next hash value. If that is the hash modulus, then we cannot
    //  go further.
    //
    if (!fCurElem)
    {
        fCurHash++;
        if (fCurHash == fToEnum->fHashModulus)
            return;

        // Else find the next non-empty bucket
        while (fToEnum->fBucketList[fCurHash]==0)
        {
            // Bump to the next hash value. If we max out return
            fCurHash++;
            if (fCurHash == fToEnum->fHashModulus)
                return;
        }
        fCurElem = fToEnum->fBucketList[fCurHash];
    }
}

XERCES_CPP_NAMESPACE_END
