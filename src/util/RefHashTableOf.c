/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
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
#include <util/RefHashTableOf.hpp>
#endif

#include <util/NullPointerException.hpp>

// ---------------------------------------------------------------------------
//  RefHashTableOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal> RefHashTableOf<TVal>::RefHashTableOf(const unsigned int modulus, const bool adoptElems) 
	: fAdoptedElems(adoptElems), fBucketList(0), fHashModulus(modulus)
{
    initialize(modulus);
	
	// create default hasher
	fHash = new HashXMLCh();
}

template <class TVal> RefHashTableOf<TVal>::RefHashTableOf(const unsigned int modulus, const bool adoptElems, HashBase* hashBase)
	: fAdoptedElems(adoptElems), fBucketList(0), fHashModulus(modulus)
{
	initialize(modulus);
	// set hasher
	fHash = hashBase;
}

template <class TVal> RefHashTableOf<TVal>::RefHashTableOf(const unsigned int modulus)
	: fAdoptedElems(true), fBucketList(0), fHashModulus(modulus)
{
	initialize(modulus);

	// create default hasher
	fHash = new HashXMLCh();
}

template <class TVal> void RefHashTableOf<TVal>::initialize(const unsigned int modulus)
{
	if (modulus == 0)
        ThrowXML(IllegalArgumentException, XMLExcepts::HshTbl_ZeroModulus);

    // Allocate the bucket list and zero them
    fBucketList = new RefHashTableBucketElem<TVal>*[fHashModulus];
    for (unsigned int index = 0; index < fHashModulus; index++)
        fBucketList[index] = 0;
}

template <class TVal> RefHashTableOf<TVal>::~RefHashTableOf()
{
    removeAll();

    // Then delete the bucket list & hasher
    delete [] fBucketList;
	delete fHash;
}


// ---------------------------------------------------------------------------
//  RefHashTableOf: Element management
// ---------------------------------------------------------------------------
template <class TVal> bool RefHashTableOf<TVal>::isEmpty() const
{
    // Just check the bucket list for non-empty elements
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        if (fBucketList[buckInd] != 0)
            return false;
    }
    return true;
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
    unsigned int hashVal;
    removeBucketElem(key, hashVal);
}

template <class TVal> void RefHashTableOf<TVal>::removeAll()
{
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
            delete curElem;
            curElem = nextElem;
        }

        // Clean out this entry
        fBucketList[buckInd] = 0;
    }
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


// ---------------------------------------------------------------------------
//  RefHashTableOf: Putters
// ---------------------------------------------------------------------------
template <class TVal> void RefHashTableOf<TVal>::put(void* key, TVal* const valueToAdopt)
{
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
        newBucket = new RefHashTableBucketElem<TVal>(key, valueToAdopt, fBucketList[hashVal]);
        fBucketList[hashVal] = newBucket;
    }
}



// ---------------------------------------------------------------------------
//  RefHashTableOf: Private methods
// ---------------------------------------------------------------------------
template <class TVal> RefHashTableBucketElem<TVal>* RefHashTableOf<TVal>::
findBucketElem(const void* const key, unsigned int& hashVal)
{
    // Hash the key
    hashVal = fHash->getHashVal(key, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

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
    hashVal = fHash->getHashVal(key, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

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


template <class TVal> void RefHashTableOf<TVal>::
removeBucketElem(const void* const key, unsigned int& hashVal)
{
    // Hash the key
    hashVal = fHash->getHashVal(key, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

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

            // If we adopted the elements, then delete the data
            if (fAdoptedElems)
                delete curElem->fData;

            // Delete the current element
            delete curElem;

            return;
        }

        // Move both pointers upwards
        lastElem = curElem;
        curElem = curElem->fNext;
    }

    // We never found that key
    ThrowXML(NoSuchElementException, XMLExcepts::HshTbl_NoSuchKeyExists);
}




// ---------------------------------------------------------------------------
//  RefHashTableOfEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal> RefHashTableOfEnumerator<TVal>::
RefHashTableOfEnumerator(RefHashTableOf<TVal>* const toEnum, const bool adopt)
	: fAdopted(adopt), fCurElem(0), fCurHash((unsigned int)-1), fToEnum(toEnum)
{
    if (!toEnum)  
        ThrowXML(NullPointerException, XMLExcepts::CPtr_PointerIsZero);        

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
        ThrowXML(NoSuchElementException, XMLExcepts::Enum_NoMoreElements);

    //
    //  Save the current element, then move up to the next one for the
    //  next time around.
    //
    RefHashTableBucketElem<TVal>* saveElem = fCurElem;
    findNext();

    return *saveElem->fData;
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
        while (true)
        {
            if (fToEnum->fBucketList[fCurHash])
                break;

            // Bump to the next hash value. If we max out return
            fCurHash++;
            if (fCurHash == fToEnum->fHashModulus)
                return;
        }
        fCurElem = fToEnum->fBucketList[fCurHash];
    }
}
