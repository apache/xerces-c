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

/**
 * $Log$
 * Revision 1.2  2002/02/04 21:50:38  tng
 * [Bug 6114] Memory leaks on iDOM getElementsByTagName().
 *
 * Revision 1.1.1.1  2002/02/01 22:21:53  peiyongz
 * sane_include
 *
 * Revision 1.1  2001/06/04 14:55:32  tng
 * IDOM: Add IRange and IDeepNodeList Support.
 *
 */


// ---------------------------------------------------------------------------
//  Include
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/idom/IDDeepNodeListPool.hpp>
#endif


// ---------------------------------------------------------------------------
//  IDDeepNodeListPool: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal> IDDeepNodeListPool<TVal>::IDDeepNodeListPool(
              const unsigned int modulus
            , const bool adoptElems
            , const unsigned int    initSize) :
	 fAdoptedElems(adoptElems)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fIdPtrs(0)
    , fIdPtrsCount(initSize)
    , fIdCounter(0)
{
    initialize(modulus);

    // create default hasher
    fHash = new HashPtr();

    //
    //  Allocate the initial id pointers array. We don't have to zero them
    //  out since the fIdCounter value tells us which ones are valid. The
    //  zeroth element is never used (and represents an invalid pool id.)
    //
    if (!fIdPtrsCount)
        fIdPtrsCount = 256;
    fIdPtrs = new TVal*[fIdPtrsCount];
    fIdPtrs[0] = 0;
}

template <class TVal> IDDeepNodeListPool<TVal>::IDDeepNodeListPool(
              const unsigned int modulus
            , const bool adoptElems
            , HashBase* hash
            , const unsigned int    initSize) :
	 fAdoptedElems(adoptElems)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fIdPtrs(0)
    , fIdPtrsCount(initSize)
    , fIdCounter(0)
{
    initialize(modulus);
    // set hasher
    fHash = hash;

    //
    //  Allocate the initial id pointers array. We don't have to zero them
    //  out since the fIdCounter value tells us which ones are valid. The
    //  zeroth element is never used (and represents an invalid pool id.)
    //
    if (!fIdPtrsCount)
        fIdPtrsCount = 256;
    fIdPtrs = new TVal*[fIdPtrsCount];
    fIdPtrs[0] = 0;
}

template <class TVal> IDDeepNodeListPool<TVal>::IDDeepNodeListPool(const unsigned int modulus
            , const unsigned int    initSize) :
	 fAdoptedElems(true)
    , fBucketList(0)
    , fHashModulus(modulus)
    , fIdPtrs(0)
    , fIdPtrsCount(initSize)
    , fIdCounter(0)
{
    initialize(modulus);

    // create default hasher
    fHash = new HashPtr();

    //
    //  Allocate the initial id pointers array. We don't have to zero them
    //  out since the fIdCounter value tells us which ones are valid. The
    //  zeroth element is never used (and represents an invalid pool id.)
    //
    if (!fIdPtrsCount)
        fIdPtrsCount = 256;
    fIdPtrs = new TVal*[fIdPtrsCount];
    fIdPtrs[0] = 0;
}

template <class TVal> void IDDeepNodeListPool<TVal>::initialize(const unsigned int modulus)
{
	if (modulus == 0)
        ThrowXML(IllegalArgumentException, XMLExcepts::HshTbl_ZeroModulus);

    // Allocate the bucket list and zero them
    fBucketList = new IDDeepNodeListPoolTableBucketElem<TVal>*[fHashModulus];
    for (unsigned int index = 0; index < fHashModulus; index++)
        fBucketList[index] = 0;
}

template <class TVal> IDDeepNodeListPool<TVal>::~IDDeepNodeListPool()
{
    removeAll();

    // Then delete the bucket list & hasher & id pointers list
    delete [] fIdPtrs;
    delete [] fBucketList;
    delete fHash;
}


// ---------------------------------------------------------------------------
//  IDDeepNodeListPool: Element management
// ---------------------------------------------------------------------------
template <class TVal> bool IDDeepNodeListPool<TVal>::isEmpty() const
{
    // Just check the bucket list for non-empty elements
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        if (fBucketList[buckInd] != 0)
            return false;
    }
    return true;
}

template <class TVal> bool IDDeepNodeListPool<TVal>::
containsKey(const void* const key1, const XMLCh* const key2, const XMLCh* const key3) const
{
    unsigned int hashVal;
    const IDDeepNodeListPoolTableBucketElem<TVal>* findIt = findBucketElem(key1, key2, key3, hashVal);
    return (findIt != 0);
}

template <class TVal> void IDDeepNodeListPool<TVal>::removeAll()
{
    // Clean up the buckets first
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        // Get the bucket list head for this entry
        IDDeepNodeListPoolTableBucketElem<TVal>* curElem = fBucketList[buckInd];
        IDDeepNodeListPoolTableBucketElem<TVal>* nextElem;
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
            delete [] curElem->fKey2;
            delete [] curElem->fKey3;

            delete curElem;
            curElem = nextElem;
        }

        // Clean out this entry
        fBucketList[buckInd] = 0;
    }

    // Reset the id counter
    fIdCounter = 0;
}

template <class TVal> void IDDeepNodeListPool<TVal>::cleanup()
{
    removeAll();

    // Then delete the bucket list & hasher & id pointers list
    delete [] fIdPtrs;
    delete [] fBucketList;
    delete fHash;
}



// ---------------------------------------------------------------------------
//  IDDeepNodeListPool: Getters
// ---------------------------------------------------------------------------
template <class TVal> TVal*
IDDeepNodeListPool<TVal>::getByKey(const void* const key1, const XMLCh* const key2, const XMLCh* const key3)
{
    unsigned int hashVal;
    IDDeepNodeListPoolTableBucketElem<TVal>* findIt = findBucketElem(key1, key2, key3, hashVal);
    if (!findIt)
        return 0;
    return findIt->fData;
}

template <class TVal> const TVal*
IDDeepNodeListPool<TVal>::getByKey(const void* const key1, const XMLCh* const key2, const XMLCh* const key3) const
{
    unsigned int hashVal;
    const IDDeepNodeListPoolTableBucketElem<TVal>* findIt = findBucketElem(key1, key2, key3, hashVal);
    if (!findIt)
        return 0;
    return findIt->fData;
}

template <class TVal> TVal*
IDDeepNodeListPool<TVal>::getById(const unsigned int elemId)
{
    // If its either zero or beyond our current id, its an error
    if (!elemId || (elemId > fIdCounter))
        ThrowXML(IllegalArgumentException, XMLExcepts::Pool_InvalidId);

    return fIdPtrs[elemId];
}

template <class TVal> const TVal*
IDDeepNodeListPool<TVal>::getById(const unsigned int elemId) const
{
    // If its either zero or beyond our current id, its an error
    if (!elemId || (elemId > fIdCounter))
        ThrowXML(IllegalArgumentException, XMLExcepts::Pool_InvalidId);

    return fIdPtrs[elemId];
}

// ---------------------------------------------------------------------------
//  IDDeepNodeListPool: Putters
// ---------------------------------------------------------------------------
template <class TVal> unsigned int
IDDeepNodeListPool<TVal>::put(void* key1, XMLCh* key2, XMLCh* key3, TVal* const valueToAdopt)
{
    // First see if the key exists already
    unsigned int hashVal;
    IDDeepNodeListPoolTableBucketElem<TVal>* newBucket = findBucketElem(key1, key2, key3, hashVal);

    //
    //  If so,then update its value. If not, then we need to add it to
    //  the right bucket
    //
    if (newBucket)
    {
        if (fAdoptedElems)
            delete newBucket->fData;

        delete[] newBucket->fKey2;
        delete[] newBucket->fKey3;

        newBucket->fData = valueToAdopt;
        newBucket->fKey1 = key1;
        newBucket->fKey2 = key2;
        newBucket->fKey3 = key3;
    }
     else
    {
        newBucket = new IDDeepNodeListPoolTableBucketElem<TVal>(key1, key2, key3, valueToAdopt, fBucketList[hashVal]);
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
        TVal** newArray = new TVal*[newCount];

        // Copy over the old contents to the new array
        memcpy(newArray, fIdPtrs, fIdPtrsCount * sizeof(TVal*));

        // Ok, toss the old array and store the new data
        delete [] fIdPtrs;
        fIdPtrs = newArray;
        fIdPtrsCount = newCount;
    }
    const unsigned int retId = ++fIdCounter;
    fIdPtrs[retId] = valueToAdopt;

    // Return the id that we gave to this element
    return retId;
}

// ---------------------------------------------------------------------------
//  IDDeepNodeListPool: Private methods
// ---------------------------------------------------------------------------
template <class TVal> IDDeepNodeListPoolTableBucketElem<TVal>* IDDeepNodeListPool<TVal>::
findBucketElem(const void* const key1, const XMLCh* const key2, const XMLCh* const key3, unsigned int& hashVal)
{
    // Hash the key
    hashVal = fHash->getHashVal(key1, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

    // Search that bucket for the key
    IDDeepNodeListPoolTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
        //key2 and key3 are XMLCh*, compareString takes null pointer vs zero len string the same
        //but we need them to be treated as different keys in this case
        if (fHash->equals(key1, curElem->fKey1) && (!XMLString::compareString(key2, curElem->fKey2)) && (!XMLString::compareString(key3, curElem->fKey3))) {
            if (!key2 || !curElem->fKey2) {
                if (key2 || curElem->fKey2) {
                    curElem = curElem->fNext;
                    continue;
                }
            }
            if (!key3 || !curElem->fKey3) {
                if (key3 || curElem->fKey3) {
                    curElem = curElem->fNext;
                    continue;
                }
            }

            return curElem;
        }

        curElem = curElem->fNext;
    }
    return 0;
}

template <class TVal> const IDDeepNodeListPoolTableBucketElem<TVal>* IDDeepNodeListPool<TVal>::
findBucketElem(const void* const key1, const XMLCh* const key2, const XMLCh* const key3, unsigned int& hashVal) const
{
    // Hash the key
    hashVal = fHash->getHashVal(key1, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

    // Search that bucket for the key
    const IDDeepNodeListPoolTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
        //key2 and key3 are XMLCh*, compareString takes null pointer vs zero len string the same
        //but we need them to be treated as different keys in this case
        if (fHash->equals(key1, curElem->fKey1) && (!XMLString::compareString(key2, curElem->fKey2)) && (!XMLString::compareString(key3, curElem->fKey3))) {
            if (!key2 || !curElem->fKey2) {
                if (key2 || curElem->fKey2) {
                    curElem = curElem->fNext;
                    continue;
                }
            }
            if (!key3 || !curElem->fKey3) {
                if (key3 || curElem->fKey3) {
                    curElem = curElem->fNext;
                    continue;
                }
            }
            return curElem;
        }

        curElem = curElem->fNext;
    }
    return 0;
}