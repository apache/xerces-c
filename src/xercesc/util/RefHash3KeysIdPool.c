/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
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

/**
 * $Log$
 * Revision 1.4  2003/05/16 06:01:52  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.2  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:12  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/07/19 18:43:18  peiyongz
 * fix: detect null poiniter in enumerator's ctor.
 *
 * Revision 1.3  2001/06/04 13:45:04  tng
 * The "hash" argument clashes with STL hash.  Fixed by Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:26:29  tng
 * Copyright update.
 *
 * Revision 1.1  2001/03/21 21:56:12  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 */


// ---------------------------------------------------------------------------
//  Include
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/RefHash3KeysIdPool.hpp>
#endif

#include <xercesc/util/NullPointerException.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  RefHash3KeysIdPool: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal>
RefHash3KeysIdPool<TVal>::RefHash3KeysIdPool( const unsigned int modulus
                                            , const bool         adoptElems
                                            , const unsigned int initSize
                                            , MemoryManager* const manager) :
    fMemoryManager(manager)
    , fAdoptedElems(adoptElems)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fHash(0)
    , fIdPtrs(0)
    , fIdPtrsCount(initSize)
    , fIdCounter(0)
{
    initialize(modulus);

    // create default hasher
    fHash = new (fMemoryManager) HashXMLCh();

    //
    //  Allocate the initial id pointers array. We don't have to zero them
    //  out since the fIdCounter value tells us which ones are valid. The
    //  zeroth element is never used (and represents an invalid pool id.)
    //
    if (!fIdPtrsCount)
        fIdPtrsCount = 256;
    fIdPtrs = (TVal**) fMemoryManager->allocate(fIdPtrsCount * sizeof(TVal*)); //new TVal*[fIdPtrsCount];
    fIdPtrs[0] = 0;
}

template <class TVal>
RefHash3KeysIdPool<TVal>::RefHash3KeysIdPool( const unsigned int modulus
                                            , const bool         adoptElems
                                            , HashBase*          hashBase
                                            , const unsigned int initSize
                                            , MemoryManager* const manager) :
	fMemoryManager(manager)
    , fAdoptedElems(adoptElems)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fHash(0)
    , fIdPtrs(0)
    , fIdPtrsCount(initSize)
    , fIdCounter(0)
{
    initialize(modulus);
    // set hasher
    fHash = hashBase;

    //
    //  Allocate the initial id pointers array. We don't have to zero them
    //  out since the fIdCounter value tells us which ones are valid. The
    //  zeroth element is never used (and represents an invalid pool id.)
    //
    if (!fIdPtrsCount)
        fIdPtrsCount = 256;
    fIdPtrs = (TVal**) fMemoryManager->allocate(fIdPtrsCount * sizeof(TVal*)); //new TVal*[fIdPtrsCount];
    fIdPtrs[0] = 0;
}

template <class TVal>
RefHash3KeysIdPool<TVal>::RefHash3KeysIdPool( const unsigned int modulus
                                            , const unsigned int initSize
                                            , MemoryManager* const manager) :
	fMemoryManager(manager)
    , fAdoptedElems(true)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fHash(0)
    , fIdPtrs(0)
    , fIdPtrsCount(initSize)
    , fIdCounter(0)
{
    initialize(modulus);

    // create default hasher

    fHash = new (fMemoryManager) HashXMLCh();

    //
    //  Allocate the initial id pointers array. We don't have to zero them
    //  out since the fIdCounter value tells us which ones are valid. The
    //  zeroth element is never used (and represents an invalid pool id.)
    //
    if (!fIdPtrsCount)
        fIdPtrsCount = 256;
    fIdPtrs = (TVal**) fMemoryManager->allocate(fIdPtrsCount * sizeof(TVal*)); //new TVal*[fIdPtrsCount];
    fIdPtrs[0] = 0;
}

template <class TVal> void RefHash3KeysIdPool<TVal>::initialize(const unsigned int modulus)
{
	if (modulus == 0)
        ThrowXML(IllegalArgumentException, XMLExcepts::HshTbl_ZeroModulus);

    // Allocate the bucket list and zero them
    fBucketList = (RefHash3KeysTableBucketElem<TVal>**) fMemoryManager->allocate
    (
        fHashModulus * sizeof(RefHash3KeysTableBucketElem<TVal>*)
    ); //new RefHash3KeysTableBucketElem<TVal>*[fHashModulus];
    for (unsigned int index = 0; index < fHashModulus; index++)
        fBucketList[index] = 0;
}

template <class TVal> RefHash3KeysIdPool<TVal>::~RefHash3KeysIdPool()
{
    removeAll();

    // Then delete the bucket list & hasher & id pointers list
    fMemoryManager->deallocate(fIdPtrs); //delete [] fIdPtrs;
    fMemoryManager->deallocate(fBucketList); //delete [] fBucketList;
    delete fHash;
}


// ---------------------------------------------------------------------------
//  RefHash3KeysIdPool: Element management
// ---------------------------------------------------------------------------
template <class TVal> bool RefHash3KeysIdPool<TVal>::isEmpty() const
{
    // Just check the bucket list for non-empty elements
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        if (fBucketList[buckInd] != 0)
            return false;
    }
    return true;
}

template <class TVal> bool RefHash3KeysIdPool<TVal>::
containsKey(const void* const key1, const int key2, const int key3) const
{
    unsigned int hashVal;
    const RefHash3KeysTableBucketElem<TVal>* findIt = findBucketElem(key1, key2, key3, hashVal);
    return (findIt != 0);
}

template <class TVal> void RefHash3KeysIdPool<TVal>::removeAll()
{
    // Clean up the buckets first
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        // Get the bucket list head for this entry
        RefHash3KeysTableBucketElem<TVal>* curElem = fBucketList[buckInd];
        RefHash3KeysTableBucketElem<TVal>* nextElem;
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
            delete curElem;
            curElem = nextElem;
        }

        // Clean out this entry
        fBucketList[buckInd] = 0;
    }

    // Reset the id counter
    fIdCounter = 0;
}


// ---------------------------------------------------------------------------
//  RefHash3KeysIdPool: Getters
// ---------------------------------------------------------------------------
template <class TVal> TVal*
RefHash3KeysIdPool<TVal>::getByKey(const void* const key1, const int key2, const int key3)
{
    unsigned int hashVal;
    RefHash3KeysTableBucketElem<TVal>* findIt = findBucketElem(key1, key2, key3, hashVal);
    if (!findIt)
        return 0;
    return findIt->fData;
}

template <class TVal> const TVal*
RefHash3KeysIdPool<TVal>::getByKey(const void* const key1, const int key2, const int key3) const
{
    unsigned int hashVal;
    const RefHash3KeysTableBucketElem<TVal>* findIt = findBucketElem(key1, key2, key3, hashVal);
    if (!findIt)
        return 0;
    return findIt->fData;
}

template <class TVal> TVal*
RefHash3KeysIdPool<TVal>::getById(const unsigned int elemId)
{
    // If its either zero or beyond our current id, its an error
    if (!elemId || (elemId > fIdCounter))
        ThrowXML(IllegalArgumentException, XMLExcepts::Pool_InvalidId);

    return fIdPtrs[elemId];
}

template <class TVal> const TVal*
RefHash3KeysIdPool<TVal>::getById(const unsigned int elemId) const
{
    // If its either zero or beyond our current id, its an error
    if (!elemId || (elemId > fIdCounter))
        ThrowXML(IllegalArgumentException, XMLExcepts::Pool_InvalidId);

    return fIdPtrs[elemId];
}

// ---------------------------------------------------------------------------
//  RefHash3KeysIdPool: Putters
// ---------------------------------------------------------------------------
template <class TVal> unsigned int
RefHash3KeysIdPool<TVal>::put(void* key1, int key2, int key3, TVal* const valueToAdopt)
{
    // First see if the key exists already
    unsigned int hashVal;
    RefHash3KeysTableBucketElem<TVal>* newBucket = findBucketElem(key1, key2, key3, hashVal);

    //
    //  If so,then update its value. If not, then we need to add it to
    //  the right bucket
    //
    if (newBucket)
    {
        if (fAdoptedElems)
            delete newBucket->fData;
        newBucket->fData = valueToAdopt;
        newBucket->fKey1 = key1;
        newBucket->fKey2 = key2;
        newBucket->fKey3 = key3;
    }
     else
    {
        newBucket = new (fMemoryManager) RefHash3KeysTableBucketElem<TVal>(key1, key2, key3, valueToAdopt, fBucketList[hashVal]);
        fBucketList[hashVal] = newBucket;
    }

    //
    //  Give this new one the next available id and add to the pointer list.
    //  Expand the list if that is now required.
    //
    if (fIdCounter + 1 == fIdPtrsCount)
    {
        // Create a new count 1.5 times larger and allocate a new array
        unsigned int newCount = (unsigned int)(fIdPtrsCount * 1.5);
        TVal** newArray = (TVal**) fMemoryManager->allocate
        (
            newCount * sizeof(TVal*)
        ); //new TVal*[newCount];

        // Copy over the old contents to the new array
        memcpy(newArray, fIdPtrs, fIdPtrsCount * sizeof(TVal*));

        // Ok, toss the old array and store the new data
        fMemoryManager->deallocate(fIdPtrs); //delete [] fIdPtrs;
        fIdPtrs = newArray;
        fIdPtrsCount = newCount;
    }
    const unsigned int retId = ++fIdCounter;
    fIdPtrs[retId] = valueToAdopt;

    // Set the id on the passed element
    valueToAdopt->setId(retId);

    // Return the id that we gave to this element
    return retId;
}

// ---------------------------------------------------------------------------
//  RefHash3KeysIdPool: Private methods
// ---------------------------------------------------------------------------
template <class TVal> RefHash3KeysTableBucketElem<TVal>* RefHash3KeysIdPool<TVal>::
findBucketElem(const void* const key1, const int key2, const int key3, unsigned int& hashVal)
{
    // Hash the key
    hashVal = fHash->getHashVal(key1, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

    // Search that bucket for the key
    RefHash3KeysTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
		if (fHash->equals(key1, curElem->fKey1) && (key2==curElem->fKey2) && (key3==curElem->fKey3))
            return curElem;

        curElem = curElem->fNext;
    }
    return 0;
}

template <class TVal> const RefHash3KeysTableBucketElem<TVal>* RefHash3KeysIdPool<TVal>::
findBucketElem(const void* const key1, const int key2, const int key3, unsigned int& hashVal) const
{
    // Hash the key
    hashVal = fHash->getHashVal(key1, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

    // Search that bucket for the key
    const RefHash3KeysTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
        if (fHash->equals(key1, curElem->fKey1) && (key2==curElem->fKey2) && (key3==curElem->fKey3))
            return curElem;

        curElem = curElem->fNext;
    }
    return 0;
}


// ---------------------------------------------------------------------------
//  RefHash3KeysIdPoolEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal> RefHash3KeysIdPoolEnumerator<TVal>::
RefHash3KeysIdPoolEnumerator(RefHash3KeysIdPool<TVal>* const toEnum, const bool adopt)
	: fAdoptedElems(adopt), fCurIndex(0), fToEnum(toEnum)
{
    if (!toEnum)
        ThrowXML(NullPointerException, XMLExcepts::CPtr_PointerIsZero);

    //
    //  Find the next available bucket element in the pool. We use the id
    //  array since its very easy to enumerator through by just maintaining
    //  an index. If the id counter is zero, then its empty and we leave the
    //  current index to zero.
    //
    if (toEnum->fIdCounter)
        fCurIndex = 1;
}

template <class TVal> RefHash3KeysIdPoolEnumerator<TVal>::~RefHash3KeysIdPoolEnumerator()
{
    if (fAdoptedElems)
        delete fToEnum;
}


// ---------------------------------------------------------------------------
//  RefHash3KeysIdPoolEnumerator: Enum interface
// ---------------------------------------------------------------------------
template <class TVal> bool RefHash3KeysIdPoolEnumerator<TVal>::hasMoreElements() const
{
    // If our index is zero or past the end, then we are done
    if (!fCurIndex || (fCurIndex > fToEnum->fIdCounter))
        return false;
    return true;
}

template <class TVal> TVal& RefHash3KeysIdPoolEnumerator<TVal>::nextElement()
{
    // If our index is zero or past the end, then we are done
    if (!fCurIndex || (fCurIndex > fToEnum->fIdCounter))
        ThrowXML(NoSuchElementException, XMLExcepts::Enum_NoMoreElements);

    // Return the current element and bump the index
    return *fToEnum->fIdPtrs[fCurIndex++];
}

template <class TVal> void RefHash3KeysIdPoolEnumerator<TVal>::Reset()
{
    fCurIndex = 0;
}

XERCES_CPP_NAMESPACE_END
