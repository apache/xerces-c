/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
 * Revision 1.1  1999/11/09 01:05:01  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:45:12  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */


#if !defined(REFHASHTABLEOF_HPP)
#define REFHASHTABLEOF_HPP


#include <util/XML4CDefs.hpp>
#include <util/KeyValuePair.hpp>
#include <util/IllegalArgumentException.hpp>
#include <util/NoSuchElementException.hpp>
#include <util/RuntimeException.hpp>
#include <util/XMLExceptMsgs.hpp>
#include <util/XMLEnumerator.hpp>
#include <util/XMLString.hpp>


//
//  Forward declare the enumerator so he can be our friend. Can you say
//  friend? Sure...
//
template <class TVal> class RefHashTableOfEnumerator;
template <class TVal> struct RefHashTableBucketElem;



template <class TVal> struct RefHashTableBucketElem
{
    RefHashTableBucketElem(TVal* const value, RefHashTableBucketElem<TVal>* next) :

        fData(value)
        , fNext(next)
        {
        }

    TVal*                           fData;
    RefHashTableBucketElem<TVal>*   fNext;
};



template <class TVal> class RefHashTableOf
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    RefHashTableOf
    (
        const   unsigned int    modulus
        , const bool            adoptElems = true
    );
    ~RefHashTableOf();


    // -----------------------------------------------------------------------
    //  Element management
    // -----------------------------------------------------------------------
    bool isEmpty() const;
    bool containsKey(const XMLCh* const key) const;
    void removeKey(const XMLCh* const key);
    void removeAll();


    // -----------------------------------------------------------------------
    //  Getters
    // -----------------------------------------------------------------------
    TVal* get(const XMLCh* const key);
    const TVal* get(const XMLCh* const key) const;


    // -----------------------------------------------------------------------
    //  Putters
    // -----------------------------------------------------------------------
    void put(TVal* const valueToAdopt);


private :
    // -----------------------------------------------------------------------
    //  Declare our friends
    // -----------------------------------------------------------------------
    friend class RefHashTableOfEnumerator<TVal>;

private:

    // -----------------------------------------------------------------------
    //  Private methods
    // -----------------------------------------------------------------------
    RefHashTableBucketElem<TVal>* findBucketElem
    (
        const XMLCh* const   key,
        unsigned int&        hashVal
    );
    const RefHashTableBucketElem<TVal>* findBucketElem
    (
        const   XMLCh* const    key
        ,       unsigned int&   hashVal
    )   const;
    void removeBucketElem(const XMLCh* const key, unsigned int& hashVal);


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
    // -----------------------------------------------------------------------
    bool                                fAdoptedElems;
    RefHashTableBucketElem<TVal>**      fBucketList;
    unsigned int                        fHashModulus;
};



//
//  An enumerator for a value array. It derives from the basic enumerator
//  class, so that value vectors can be generically enumerated.
//
template <class TVal> class RefHashTableOfEnumerator : public XMLEnumerator<TVal>
{
public :
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    RefHashTableOfEnumerator
    (
                RefHashTableOf<TVal>* const toEnum
        , const bool                        adopt = false
    );
    ~RefHashTableOfEnumerator();


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
    RefHashTableBucketElem<TVal>*         fCurElem;
    unsigned int                          fCurHash;
    RefHashTableOf<TVal>*                 fToEnum;
};

#if !defined(XML4C_TMPLSINC)
#include <util/RefHashTableOf.c>
#endif

#endif
