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

/*
 * $Log$
 * Revision 1.1  2001/02/27 18:24:01  tng
 * Schema: Add utility RefHash2KeysTableOf.
 *
 */


#if !defined(REFHASH2KEYSTABLEOF_HPP)
#define REFHASH2KEYSTABLEOF_HPP


#include <util/XercesDefs.hpp>
#include <util/KeyValuePair.hpp>
#include <util/HashBase.hpp>
#include <util/IllegalArgumentException.hpp>
#include <util/NoSuchElementException.hpp>
#include <util/RuntimeException.hpp>
#include <util/XMLExceptMsgs.hpp>
#include <util/XMLEnumerator.hpp>
#include <util/XMLString.hpp>
#include <util/HashBase.hpp>
#include <util/HashXMLCh.hpp>

// This hash table is similar to RefHashTableOf with an additional integer as key2

//
//  Forward declare the enumerator so he can be our friend. Can you say
//  friend? Sure...
//
template <class TVal> class RefHash2KeysTableOfEnumerator;
template <class TVal> struct RefHash2KeysTableBucketElem;


//
//  This should really be a nested class, but some of the compilers we
//  have to support cannot deal with that!
//
template <class TVal> struct RefHash2KeysTableBucketElem
{
    RefHash2KeysTableBucketElem(void* key1, int key2, TVal* const value, RefHash2KeysTableBucketElem<TVal>* next)
		: fData(value), fNext(next), fKey1(key1), fKey2(key2)
        {
        }

    TVal*                           fData;
    RefHash2KeysTableBucketElem<TVal>*   fNext;
	void*							fKey1;
	int							fKey2;
};


template <class TVal> class RefHash2KeysTableOf
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
	// backwards compatability - default hasher is HashXMLCh
    RefHash2KeysTableOf(const unsigned int modulus);
	// backwards compatability - default hasher is HashXMLCh
    RefHash2KeysTableOf(const unsigned int modulus, const bool adoptElems);
	// if a hash function is passed in, it will be deleted when the hashtable is deleted.
	// use a new instance of the hasher class for each hashtable, otherwise one hashtable
	// may delete the hasher of a different hashtable if both use the same hasher.
    RefHash2KeysTableOf(const unsigned int modulus, const bool adoptElems, HashBase* hash);
    ~RefHash2KeysTableOf();


    // -----------------------------------------------------------------------
    //  Element management
    // -----------------------------------------------------------------------
    bool isEmpty() const;
    bool containsKey(const void* const key1, const int key2) const;
    void removeKey(const void* const key1, const int key2);
    void removeAll();


    // -----------------------------------------------------------------------
    //  Getters
    // -----------------------------------------------------------------------
    TVal* get(const void* const key1, const int key2);
    const TVal* get(const void* const key1, const int key2) const;


    // -----------------------------------------------------------------------
    //  Putters
    // -----------------------------------------------------------------------
	void put(void* key1, int key2, TVal* const valueToAdopt);


private :
    // -----------------------------------------------------------------------
    //  Declare our friends
    // -----------------------------------------------------------------------
    friend class RefHash2KeysTableOfEnumerator<TVal>;

private:

    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------
    RefHash2KeysTableBucketElem<TVal>* findBucketElem(const void* const key1, const int key2, unsigned int& hashVal);
    const RefHash2KeysTableBucketElem<TVal>* findBucketElem(const void* const key1, const int key2, unsigned int& hashVal) const;
    void removeBucketElem(const void* const key1, const int key2, unsigned int& hashVal);
	void initialize(const unsigned int modulus);


    // -----------------------------------------------------------------------
    //  Data members
    //
    //  fAdoptedElems
    //      Indicates whether the values added are adopted or just referenced.
    //      If adopted, then they are deleted when they are removed from the
    //      hash table.
    //
    //  fBucketList
    //      This is the array that contains the heads of all of the list
    //      buckets, one for each possible hash value.
    //
    //  fHashModulus
    //      The modulus used for this hash table, to hash the keys. This is
    //      also the number of elements in the bucket list.
	//
	//  fHash
	//      The hasher for the key1 data type.
    // -----------------------------------------------------------------------
    bool                                fAdoptedElems;
    RefHash2KeysTableBucketElem<TVal>**      fBucketList;
    unsigned int                        fHashModulus;
	HashBase*							fHash;
};



//
//  An enumerator for a value array. It derives from the basic enumerator
//  class, so that value vectors can be generically enumerated.
//
template <class TVal> class RefHash2KeysTableOfEnumerator : public XMLEnumerator<TVal>
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    RefHash2KeysTableOfEnumerator(RefHash2KeysTableOf<TVal>* const toEnum, const bool adopt = false);
    ~RefHash2KeysTableOfEnumerator();


    // -----------------------------------------------------------------------
    //  Enum interface
    // -----------------------------------------------------------------------
    bool hasMoreElements() const;
    TVal& nextElement();
    void Reset();


private :
    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------
    void findNext();


    // -----------------------------------------------------------------------
    //  Data Members
    //
    //  fAdopted
    //      Indicates whether we have adopted the passed vector. If so then
    //      we delete the vector when we are destroyed.
    //
    //  fCurElem
    //      This is the current bucket bucket element that we are on.
    //
    //  fCurHash
    //      The is the current hash buck that we are working on. Once we hit
    //      the end of the bucket that fCurElem is in, then we have to start
    //      working this one up to the next non-empty bucket.
    //
    //  fToEnum
    //      The value array being enumerated.
    // -----------------------------------------------------------------------
    bool                                  fAdopted;
    RefHash2KeysTableBucketElem<TVal>*         fCurElem;
    unsigned int                          fCurHash;
    RefHash2KeysTableOf<TVal>*                 fToEnum;
};

#if !defined(XERCES_TMPLSINC)
#include <util/RefHash2KeysTableOf.c>
#endif

#endif
