/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights
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
 * Revision 1.2  2003/06/03 14:00:34  neilg
 * attempt to make VC6 happy.
 *
 * Revision 1.1  2003/06/02 22:22:26  neilg
 * new test for the pluggable memory management mechanism.  This tests all 4 common Xerces parsers (SAX, SAX2, DOMBuilder and DeprecatedDOM) and ensures all allocated memory is dallocated, and that the manager which created the memory is called to deallocate it.
 *
 */

#if defined(XERCES_TMPLSINC)
#include "SimpleValueHashTableOf.hpp"
#endif

// ---------------------------------------------------------------------------
//  Include
// ---------------------------------------------------------------------------

#include <xercesc/util/NullPointerException.hpp>


// This is just an old version of the ValueHashTableOf implementation
// from xercesc/util; we need a new one here because this
// cannot use the pluggalbe memory management facilities.

// forward declarations

// ---------------------------------------------------------------------------
//  ValueHashTableOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal> SimpleValueHashTableOf<TVal>::SimpleValueHashTableOf(const unsigned int modulus
            , SimpleHashPtr* hashBase)
	: fBucketList(0), fHashModulus(modulus)
{
	initialize(modulus);
	// set hasher
	fHash = hashBase;
}

template <class TVal> SimpleValueHashTableOf<TVal>::SimpleValueHashTableOf(const unsigned int modulus)
	: fBucketList(0), fHashModulus(modulus)
{
	initialize(modulus);

	// create default hasher
	fHash = new HashXMLCh();
}

template <class TVal> void SimpleValueHashTableOf<TVal>::initialize(const unsigned int modulus)
{
	if (modulus == 0)
        ThrowXML(IllegalArgumentException, XMLExcepts::HshTbl_ZeroModulus);

    // Allocate the bucket list and zero them
    fBucketList = new ValueHashTableBucketElem<TVal>*[fHashModulus];
    for (unsigned int index = 0; index < fHashModulus; index++)
        fBucketList[index] = 0;
}

template <class TVal> SimpleValueHashTableOf<TVal>::~SimpleValueHashTableOf()
{
    removeAll();

    // Then delete the bucket list & hasher
    delete [] fBucketList;
	delete fHash;
}


// ---------------------------------------------------------------------------
//  SimpleValueHashTableOf: Element management
// ---------------------------------------------------------------------------
template <class TVal> bool SimpleValueHashTableOf<TVal>::isEmpty() const
{
    // Just check the bucket list for non-empty elements
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        if (fBucketList[buckInd] != 0)
            return false;
    }
    return true;
}

template <class TVal> bool SimpleValueHashTableOf<TVal>::
containsKey(const void* const key) const
{
    unsigned int hashVal;
    const ValueHashTableBucketElem<TVal>* findIt = findBucketElem(key, hashVal);
    return (findIt != 0);
}

template <class TVal> void SimpleValueHashTableOf<TVal>::
removeKey(const void* const key)
{
    unsigned int hashVal;
    removeBucketElem(key, hashVal);
}

template <class TVal> void SimpleValueHashTableOf<TVal>::removeAll()
{
    // Clean up the buckets first
    for (unsigned int buckInd = 0; buckInd < fHashModulus; buckInd++)
    {
        // Get the bucket list head for this entry
        ValueHashTableBucketElem<TVal>* curElem = fBucketList[buckInd];
        ValueHashTableBucketElem<TVal>* nextElem;
        while (curElem)
        {
            // Save the next element before we hose this one
            nextElem = curElem->fNext;

            // delete the current element and move forward
            delete curElem;
            curElem = nextElem;
        }

        // Clean out this entry
        fBucketList[buckInd] = 0;
    }
}


// ---------------------------------------------------------------------------
//  SimpleValueHashTableOf: Getters
// ---------------------------------------------------------------------------
template <class TVal> TVal& SimpleValueHashTableOf<TVal>::get(const void* const key)
{
    unsigned int hashVal;
    ValueHashTableBucketElem<TVal>* findIt = findBucketElem(key, hashVal);
    if (!findIt)
        ThrowXML(NoSuchElementException, XMLExcepts::HshTbl_NoSuchKeyExists);

    return findIt->fData;
}

template <class TVal> const TVal& SimpleValueHashTableOf<TVal>::
get(const void* const key) const
{
    unsigned int hashVal;
    const ValueHashTableBucketElem<TVal>* findIt = findBucketElem(key, hashVal);
    if (!findIt)
        ThrowXML(NoSuchElementException, XMLExcepts::HshTbl_NoSuchKeyExists);

    return findIt->fData;
}


// ---------------------------------------------------------------------------
//  SimpleValueHashTableOf: Putters
// ---------------------------------------------------------------------------
template <class TVal> void SimpleValueHashTableOf<TVal>::put(void* key, const TVal& valueToAdopt)
{
    // First see if the key exists already
    unsigned int hashVal;
    ValueHashTableBucketElem<TVal>* newBucket = findBucketElem(key, hashVal);

    //
    //  If so,then update its value. If not, then we need to add it to
    //  the right bucket
    //
    if (newBucket)
    {
        newBucket->fData = valueToAdopt;
		newBucket->fKey = key;
    }
     else
    {
        newBucket = new ValueHashTableBucketElem<TVal>(key, valueToAdopt, fBucketList[hashVal]);
        fBucketList[hashVal] = newBucket;
    }
}



// ---------------------------------------------------------------------------
//  SimpleValueHashTableOf: Private methods
// ---------------------------------------------------------------------------
template <class TVal> ValueHashTableBucketElem<TVal>* SimpleValueHashTableOf<TVal>::
findBucketElem(const void* const key, unsigned int& hashVal)
{
    // Hash the key
    hashVal = fHash->getHashVal(key, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

    // Search that bucket for the key
    ValueHashTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
		if (fHash->equals(key, curElem->fKey))
            return curElem;

        curElem = curElem->fNext;
    }
    return 0;
}

template <class TVal> const ValueHashTableBucketElem<TVal>* SimpleValueHashTableOf<TVal>::
findBucketElem(const void* const key, unsigned int& hashVal) const
{
    // Hash the key
    hashVal = fHash->getHashVal(key, fHashModulus);
    if (hashVal > fHashModulus)
        ThrowXML(RuntimeException, XMLExcepts::HshTbl_BadHashFromKey);

    // Search that bucket for the key
    const ValueHashTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    while (curElem)
    {
        if (fHash->equals(key, curElem->fKey))
            return curElem;

        curElem = curElem->fNext;
    }
    return 0;
}


template <class TVal> void SimpleValueHashTableOf<TVal>::
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
    ValueHashTableBucketElem<TVal>* curElem = fBucketList[hashVal];
    ValueHashTableBucketElem<TVal>* lastElem = 0;

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
//  ValueHashTableOfEnumerator: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TVal> ValueHashTableOfEnumerator<TVal>::
ValueHashTableOfEnumerator(SimpleValueHashTableOf<TVal>* const toEnum, const bool adopt)
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

template <class TVal> ValueHashTableOfEnumerator<TVal>::~ValueHashTableOfEnumerator()
{
    if (fAdopted)
        delete fToEnum;
}


// ---------------------------------------------------------------------------
//  ValueHashTableOfEnumerator: Enum interface
// ---------------------------------------------------------------------------
template <class TVal> bool ValueHashTableOfEnumerator<TVal>::hasMoreElements() const
{
    //
    //  If our current has is at the max and there are no more elements
    //  in the current bucket, then no more elements.
    //
    if (!fCurElem && (fCurHash == fToEnum->fHashModulus))
        return false;
    return true;
}

template <class TVal> TVal& ValueHashTableOfEnumerator<TVal>::nextElement()
{
    // Make sure we have an element to return
    if (!hasMoreElements())
        ThrowXML(NoSuchElementException, XMLExcepts::Enum_NoMoreElements);

    //
    //  Save the current element, then move up to the next one for the
    //  next time around.
    //
    ValueHashTableBucketElem<TVal>* saveElem = fCurElem;
    findNext();

    return saveElem->fData;
}


template <class TVal> void ValueHashTableOfEnumerator<TVal>::Reset()
{
    fCurHash = (unsigned int)-1;
    fCurElem = 0;
    findNext();
}



// ---------------------------------------------------------------------------
//  ValueHashTableOfEnumerator: Private helper methods
// ---------------------------------------------------------------------------
template <class TVal> void ValueHashTableOfEnumerator<TVal>::findNext()
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

