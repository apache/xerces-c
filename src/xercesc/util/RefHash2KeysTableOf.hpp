/*
 * Copyright 2001-2004 The Apache Software Foundation.
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
 * Revision 1.15  2005/02/02 09:27:54  amassari
 * Added rehashing capabilities
 *
 * Revision 1.14  2004/12/30 14:52:34  amassari
 * Added API to remove all entries having the same primary key
 *
 * Revision 1.13  2004/11/19 00:50:22  cargilld
 * Memory improvement to utility classes from Christian Will.  Remove dependency on XMemory.
 *
 * Revision 1.12  2004/09/08 13:56:22  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.11  2004/08/30 15:18:35  amassari
 * - Added transferElement API
 * - The iterator class now can iterate over the items having the same primary key
 *
 * Revision 1.10  2004/03/01 15:03:08  peiyongz
 * new getter: getHashModulus
 *
 * Revision 1.9  2004/01/29 11:48:46  cargilld
 * Code cleanup changes to get rid of various compiler diagnostic messages.
 *
 * Revision 1.8  2003/12/17 00:18:35  cargilld
 * Update to memory management so that the static memory manager (one used to call Initialize) is only for static data.
 *
 * Revision 1.7  2003/10/20 11:45:06  gareth
 * Made enumerators inherit from XMemory.
 *
 * Revision 1.6  2003/10/17 21:10:40  peiyongz
 * nextElementKey() added
 *
 * Revision 1.5  2003/05/18 14:02:05  knoaman
 * Memory manager implementation: pass per instance manager.
 *
 * Revision 1.4  2003/05/15 19:04:35  knoaman
 * Partial implementation of the configurable memory manager.
 *
 * Revision 1.3  2002/11/04 15:22:04  tng
 * C++ Namespace Support.
 *
 * Revision 1.2  2002/06/12 17:15:12  tng
 * Remove redundant include header file.
 *
 * Revision 1.1.1.1  2002/02/01 22:22:12  peiyongz
 * sane_include
 *
 * Revision 1.4  2001/12/22 01:06:08  jasons
 * Made the destructors virtual for:
 *
 * * ~RefHash2KeysTableOfEnumerator
 * * ~RefHash3KeysIdPoolEnumerator
 *
 * This fixes bug #5514
 *
 * Revision 1.3  2001/06/04 13:45:03  tng
 * The "hash" argument clashes with STL hash.  Fixed by Pei Yong Zhang.
 *
 * Revision 1.2  2001/05/11 13:26:28  tng
 * Copyright update.
 *
 * Revision 1.1  2001/02/27 18:24:01  tng
 * Schema: Add utility RefHash2KeysTableOf.
 *
 */


#if !defined(REFHASH2KEYSTABLEOF_HPP)
#define REFHASH2KEYSTABLEOF_HPP


#include <xercesc/util/HashBase.hpp>
#include <xercesc/util/IllegalArgumentException.hpp>
#include <xercesc/util/NoSuchElementException.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/HashXMLCh.hpp>
#include <xercesc/util/PlatformUtils.hpp>

XERCES_CPP_NAMESPACE_BEGIN

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
    ~RefHash2KeysTableBucketElem() {};

    TVal*                           fData;
    RefHash2KeysTableBucketElem<TVal>*   fNext;
	void*							fKey1;
	int							fKey2;

private:
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    RefHash2KeysTableBucketElem(const RefHash2KeysTableBucketElem<TVal>&);
    RefHash2KeysTableBucketElem<TVal>& operator=(const RefHash2KeysTableBucketElem<TVal>&);
};


template <class TVal> class RefHash2KeysTableOf : public XMemory
{
public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
	// backwards compatability - default hasher is HashXMLCh
    RefHash2KeysTableOf
    (
        const unsigned int modulus
		, MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
	// backwards compatability - default hasher is HashXMLCh
    RefHash2KeysTableOf
    (
        const unsigned int modulus
        , const bool adoptElems
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
	// if a hash function is passed in, it will be deleted when the hashtable is deleted.
	// use a new instance of the hasher class for each hashtable, otherwise one hashtable
	// may delete the hasher of a different hashtable if both use the same hasher.
    RefHash2KeysTableOf
    (
        const unsigned int modulus
        , const bool adoptElems
        , HashBase* hashBase
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );
    ~RefHash2KeysTableOf();


    // -----------------------------------------------------------------------
    //  Element management
    // -----------------------------------------------------------------------
    bool isEmpty() const;
    bool containsKey(const void* const key1, const int key2) const;
    void removeKey(const void* const key1, const int key2);
    void removeKey(const void* const key1);
    void removeAll();
    void transferElement(const void* const key1, void* key2);

    // -----------------------------------------------------------------------
    //  Getters
    // -----------------------------------------------------------------------
    TVal* get(const void* const key1, const int key2);
    const TVal* get(const void* const key1, const int key2) const;

    MemoryManager* getMemoryManager() const;
    unsigned int   getHashModulus()   const;

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
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    RefHash2KeysTableOf(const RefHash2KeysTableOf<TVal>&);
    RefHash2KeysTableOf<TVal>& operator=(const RefHash2KeysTableOf<TVal>&);

    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------
    RefHash2KeysTableBucketElem<TVal>* findBucketElem(const void* const key1, const int key2, unsigned int& hashVal);
    const RefHash2KeysTableBucketElem<TVal>* findBucketElem(const void* const key1, const int key2, unsigned int& hashVal) const;
    void initialize(const unsigned int modulus);
    void rehash();


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
    //  fCount
    //      The number of elements currently in the map
    //
    //  fHash
    //      The hasher for the key1 data type.
    // -----------------------------------------------------------------------
    MemoryManager*                      fMemoryManager;
    bool                                fAdoptedElems;
    RefHash2KeysTableBucketElem<TVal>** fBucketList;
    unsigned int                        fHashModulus;
    unsigned int                        fCount;
    HashBase*							fHash;
};



//
//  An enumerator for a value array. It derives from the basic enumerator
//  class, so that value vectors can be generically enumerated.
//
template <class TVal> class RefHash2KeysTableOfEnumerator : public XMLEnumerator<TVal>, public XMemory
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    RefHash2KeysTableOfEnumerator(RefHash2KeysTableOf<TVal>* const toEnum
        , const bool adopt = false
        , MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager);
    virtual ~RefHash2KeysTableOfEnumerator();


    // -----------------------------------------------------------------------
    //  Enum interface
    // -----------------------------------------------------------------------
    bool hasMoreElements() const;
    TVal& nextElement();
    void Reset();

    // -----------------------------------------------------------------------
    //  New interface 
    // -----------------------------------------------------------------------
    void nextElementKey(void*&, int&);
    void setPrimaryKey(const void* key);

private :
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    RefHash2KeysTableOfEnumerator(const RefHash2KeysTableOfEnumerator<TVal>&);
    RefHash2KeysTableOfEnumerator<TVal>& operator=(const RefHash2KeysTableOfEnumerator<TVal>&);

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
    //
    //  fLockPrimaryKey
    //      Indicates that we are requested to iterate over the secondary keys
    //      associated with the given primary key
    //
    // -----------------------------------------------------------------------
    bool                                    fAdopted;
    RefHash2KeysTableBucketElem<TVal>*      fCurElem;
    unsigned int                            fCurHash;
    RefHash2KeysTableOf<TVal>*              fToEnum;
    MemoryManager* const                    fMemoryManager;
    const void*                             fLockPrimaryKey;
};

XERCES_CPP_NAMESPACE_END

#if !defined(XERCES_TMPLSINC)
#include <xercesc/util/RefHash2KeysTableOf.c>
#endif

#endif
