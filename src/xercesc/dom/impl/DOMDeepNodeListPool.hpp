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

//
//  This file is part of the internal implementation of the C++ XML DOM.
//  It should NOT be included or used directly by application programs.
//
//  Applications should include the file <xercesc/dom/DOM.hpp> for the entire
//  DOM API, or xercesc/dom/DOM*.hpp for individual DOM classes, where the class
//  name is substituded for the *.
//

#if !defined(DOMDeepNODELISTPOOL_HPP)
#define DOMDeepNODELISTPOOL_HPP


#include <xercesc/util/HashBase.hpp>
#include <xercesc/util/IllegalArgumentException.hpp>
#include <xercesc/util/NoSuchElementException.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/XMLEnumerator.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/HashXMLCh.hpp>
#include <xercesc/util/HashPtr.hpp>

XERCES_CPP_NAMESPACE_BEGIN


// This hash table is modified from RefHash3KeysIdPool with first key as object ptr (DOMNode),
// second and third keys are both XMLCh* string

template <class TVal> struct DOMDeepNodeListPoolTableBucketElem;


//
//  This should really be a nested class, but some of the compilers we
//  have to support cannot deal with that!
//
template <class TVal>
struct DOMDeepNodeListPoolTableBucketElem : public XMemory
{
    DOMDeepNodeListPoolTableBucketElem
    (
        void* key1
        , XMLCh* key2
        , XMLCh* key3
        , TVal* const value
        , DOMDeepNodeListPoolTableBucketElem<TVal>* next
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    ) :
    fData(value)
    , fNext(next)
    , fKey1(key1)
    , fKey2(0)
    , fKey3(0)
    {
        if (key2)
            fKey2 = XMLString::replicate(key2, manager);

        if (key3)
            fKey3 = XMLString::replicate(key3, manager);
    }

    TVal*                                     fData;
    DOMDeepNodeListPoolTableBucketElem<TVal>* fNext;
    void*                                     fKey1;
    XMLCh*                                    fKey2;
    XMLCh*                                    fKey3;

    ~DOMDeepNodeListPoolTableBucketElem() {};
};


template <class TVal> class DOMDeepNodeListPool
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    // backwards compatability - default hasher is HashXMLCh
    DOMDeepNodeListPool
    (
        const XMLSize_t modulus
      , const XMLSize_t initSize = 128
    );

    // backwards compatability - default hasher is HashXMLCh
    DOMDeepNodeListPool
    (
        const XMLSize_t modulus
      , const bool adoptElems
      , const XMLSize_t initSize = 128
    );

    // if a hash function is passed in, it will be deleted when the hashtable is deleted.
    // use a new instance of the hasher class for each hashtable, otherwise one hashtable
    // may delete the hasher of a different hashtable if both use the same hasher.
    DOMDeepNodeListPool
    (
         const XMLSize_t modulus
       , const bool adoptElems
       , HashBase* hash
       , const XMLSize_t initSize = 128
    );

    ~DOMDeepNodeListPool();

    // -----------------------------------------------------------------------
    //  Element management
    // -----------------------------------------------------------------------
    bool isEmpty() const;
    bool containsKey(const void* const key1, const XMLCh* const key2, const XMLCh* const key3) const;
    void removeAll();
    void cleanup();


    // -----------------------------------------------------------------------
    //  Getters
    // -----------------------------------------------------------------------
    TVal* getByKey(const void* const key1, const XMLCh* const key2, const XMLCh* const key3);
    const TVal* getByKey(const void* const key1, const XMLCh* const key2, const XMLCh* const key3) const;

    TVal* getById(const XMLSize_t elemId);
    const TVal* getById(const XMLSize_t elemId) const;

    // -----------------------------------------------------------------------
    //  Putters
    // -----------------------------------------------------------------------
	XMLSize_t put(void* key1, XMLCh* key2, XMLCh* key3, TVal* const valueToAdopt);

private:

    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------
    DOMDeepNodeListPoolTableBucketElem<TVal>* findBucketElem(const void* const key1, const XMLCh* const key2, const XMLCh* const key3, XMLSize_t& hashVal);
    const DOMDeepNodeListPoolTableBucketElem<TVal>* findBucketElem(const void* const key1, const XMLCh* const key2, const XMLCh* const key3, XMLSize_t& hashVal) const;
    void initialize(const XMLSize_t modulus);

    // -----------------------------------------------------------------------
    // Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    DOMDeepNodeListPool(const DOMDeepNodeListPool<TVal> &);
    DOMDeepNodeListPool<TVal> & operator = (const DOMDeepNodeListPool<TVal> &);

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
    //
    //  fIdPtrs
    //  fIdPtrsCount
    //      This is the array of pointers to the bucket elements in order of
    //      their assigned ids. So taking id N and referencing this array
    //      gives you the element with that id. The count field indicates
    //      the current size of this list. When fIdCounter+1 reaches this
    //      value the list must be expanded.
    //
    //  fIdCounter
    //      This is used to give out unique ids to added elements. It starts
    //      at zero (which means empty), and is bumped up for each newly added
    //      element. So the first element is 1, the next is 2, etc... This
    //      means that this value is set to the top index of the fIdPtrs array.
    // -----------------------------------------------------------------------
    bool                                       fAdoptedElems;
    DOMDeepNodeListPoolTableBucketElem<TVal>** fBucketList;
    XMLSize_t                                  fHashModulus;
    HashBase*                                  fHash;
    TVal**                                     fIdPtrs;
    XMLSize_t                                  fIdPtrsCount;
    XMLSize_t                                  fIdCounter;
    MemoryManager*                             fMemoryManager;
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/dom/impl/DOMDeepNodeListPool.c>
#endif

#endif
